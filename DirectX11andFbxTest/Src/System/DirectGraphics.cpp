#include "DirectGraphics.h"
#include "Window.h"

DirectGraphics* DirectGraphics::mp_instance = nullptr;

//インスタンス化関数
DirectGraphics* DirectGraphics::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new DirectGraphics; }

	return mp_instance;
}

//初期化関数
bool DirectGraphics::Init()
{
	// DeviceとSwapChainの作成
	if (CreateDeviceAndSwapChain() == false)
	{
		return false;
	}

	// RenderTargetViewの作成
	if (CreateRenderTargetView() == false)
	{
		return false;
	}

	// Depth、StencilViewの作成
	if (CreateDepthAndStencilView() == false)
	{
		return false;
	}

	//影描画用RenderTargetView作成関数
	if (CreateShadowView() == false)
	{
		return false;
	}

	//TextureSampler作成関数
	if (CreateTextureSampler() == false)
	{
		return false;
	}

	//ConstantBuffer作成関数
	if (CreateConstantBuffer() == false)
	{
		return false;
	}

	// シェーダ作成
	if (ShaderManager::Instance()->CreateShader() == false)
	{
		return false;
	}

	// ViewPort設定
	SetUpViewPort();

	// 変換行列設定
	SetLight();

	SetUpRenderState();

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ID3D11RasterizerState* state;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	if (FAILED(m_Device->CreateRasterizerState(&rasterizerDesc, &state)))
	{
		return false;
	}

	m_Context->RSSetState(state);
	return true;
}

bool DirectGraphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC dxgi;
	SetUpDxgiSwapChainDesc(&dxgi);

	D3D_FEATURE_LEVEL level;
	// デバイス生成とスワップチェーン作成を行う
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,					// ビデオアダプタ指定(nullptrは既定)
		D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプ
		nullptr,					// D3D_DRIVER_TYPE_SOFTWARE指定時に使用
		0,							// フラグ指定
		nullptr,					// D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
		0,							// 上のD3D_FEATURE_LEVEL配列の要素数
		D3D11_SDK_VERSION,			// SDKバージョン
		&dxgi,						// DXGI_SWAP_CHAIN_DESC
		&m_SwapChain,				// 関数成功時のSwapChainの出力先 
		&m_Device,					// 関数成功時のDeviceの出力先
		&level,						// 成功したD3D_FEATURE_LEVELの出力先
		&m_Context)))				// 関数成功時のContextの出力先
	{
		return false;
	}

	return true;
}

bool DirectGraphics::CreateRenderTargetView()
{
	// RenderTargetViewの対象となるBufferの取得
	ID3D11Texture2D* back_buffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer)))
	{
		return false;
	}

	// BufferからRenderTargetViewの作成
	if (FAILED(m_Device->CreateRenderTargetView(back_buffer, nullptr, &m_RenderTargetView)))
	{
		return false;
	}

	// Targetの取得終わったのでBufferを解放
	back_buffer->Release();

	return true;
}

//影描画用RenderTargetView作成関数
bool DirectGraphics::CreateShadowView()
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	D3D11_TEXTURE2D_DESC renderTextureDesc;
	ZeroMemory(&renderTextureDesc, sizeof(renderTextureDesc));
	renderTextureDesc.Width = (rect.right - rect.left) * 2;
	renderTextureDesc.Height = (rect.bottom - rect.top) * 2;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R32_FLOAT;
	renderTextureDesc.SampleDesc.Count = 1;
	renderTextureDesc.SampleDesc.Quality = 0;
	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	renderTextureDesc.CPUAccessFlags = 0;

	if (FAILED(m_Device->CreateTexture2D(&renderTextureDesc, nullptr, &m_ShadowTexture)))
	{
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory(&rtv_desc, sizeof(rtv_desc));
	rtv_desc.Format = renderTextureDesc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;

	if (FAILED(m_Device->CreateRenderTargetView(m_ShadowTexture, &rtv_desc, &m_ShadowRenderTargetView)))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resource_desc;
	ZeroMemory(&resource_desc, sizeof(resource_desc));
	resource_desc.Format = renderTextureDesc.Format;
	resource_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_desc.Texture2D.MipLevels = 1;

	if (FAILED(m_Device->CreateShaderResourceView(m_ShadowTexture, &resource_desc, &m_ShadowResourceView)))
	{
		return false;
	}

	renderTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	renderTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(m_Device->CreateTexture2D(&renderTextureDesc, nullptr, &m_ShadowDepthStencilTexture)))
	{
		return false;
	}

	if (FAILED(m_Device->CreateDepthStencilView(m_ShadowDepthStencilTexture, nullptr, &m_ShadowDepthStencilView)))
	{
		return false;
	}

	return true;
}

//DepthバッファStencilバッファ作成関数
bool DirectGraphics::CreateDepthAndStencilView()
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	//深度ステンシルバッファ作成
	D3D11_TEXTURE2D_DESC texture_desc;
	ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
	texture_desc.Width = (rect.right - rect.left);			// 横幅
	texture_desc.Height = (rect.bottom - rect.top);			// 縦幅
	/*
		ミップマップのレベル指定
			どのレベルまで生成するかという設定
			1はマルチサンプリングされたテクスチャを使用するあるので、
			ミップマップはなしと考えられる
			0は全生成とあるので可能な限りのレベルまでテクスチャが生成されると思われる
	*/
	texture_desc.MipLevels = 1;
	/*
		テクスチャ配列のサイズ指定
			テクスチャ配列について調べ切れていないので
			他のサンプルと同様に1を設定しておく
	*/
	texture_desc.ArraySize = 1;
	/*
		テクスチャのフォーマット
			DXGI_FORMAT_D24_UNORM_S8_UINTを使用する
			これはおそらくDepth24bit、Stencil8bitとされると思う

			※．UNORMはUnsigned NORMalizedの略で指定された範囲を0.0～1.0にするみたい
	*/
	texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// テクスチャーフォーマット
	// マルチサンプリング設定(使わない)
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;				// テクスチャの使用方法(デフォルト)
	texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// Bind設定はDepth、Stencilに設定
	/*
		リソースへのCPUのアクセス権限についての設定
			ReadとWriteがあるが、権限について現状は考える必要はないはずなので、
			デフォルト値であると思われる0をしておく

			※．readとwriteはenum(D3D11_CPU_ACCESS_FLAG)で用意されていた

	*/
	texture_desc.CPUAccessFlags = 0;
	/*
		リソースオプションのフラグ
			Microsoft Docsでフラグを確認する限りは
			通常使用でフラグを設定する必要はないと思われるので
			0としておく
	*/
	texture_desc.MiscFlags = 0;

	// texture_descの情報でテクスチャを作成
	if (FAILED(m_Device->CreateTexture2D(&texture_desc, nullptr, &m_DepthStencilTexture)))
	{
		return false;
	}

	// Depth、StencilViewの設定
	// DepthStencilView 
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = texture_desc.Format;				// Viewのフォーマット(Textureのものを使用)
	/*
		DSVが何次元であるかとTextureの種類を設定する値
			D3D11_DSV_DIMENSION_TEXTURE2Dのほかにも
			D3D11_DSV_DIMENSION_TEXTURE1Dや
			D3D11_DSV_DIMENSION_TEXTURE2D_ARRAYなどがあったので
			DSVが何次元であるかとTextureの種類を設定するメンバと思われる
			今回は何も通常のTextureとして用意しているはず、
			なので、D3D11_DSV_DIMENSION_TEXTURE2Dを指定
	*/
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;								// 最初に使用するミップマップのレベルを指定 

	// CreateTexture2Dとdsv_descからDepthとStencilバッファを作る
	if (FAILED(m_Device->CreateDepthStencilView(
		m_DepthStencilTexture,				// DSVとして使用されるTexture
		&dsv_desc,							// DSVの設定
		&m_DepthStencilView)))				// ID3D11DepthStencilViewの出力先
	{
		return false;
	}

	return true;
}

//TextureSampler作成関数
bool DirectGraphics::CreateTextureSampler()
{
	D3D11_SAMPLER_DESC sampler_desc;

	ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// サンプリング時の補間方法
	// UVW値が0.0～1.0の範囲外になった場合の対応設定
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(m_Device->CreateSamplerState(&sampler_desc, &m_SamplerState)))
	{
		return false;
	}

	return true;
}

//ConstantBuffer作成関数
bool DirectGraphics::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(ConstantBuffer);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	if (FAILED(m_Device->CreateBuffer(&buffer_desc, nullptr, &m_ConstantBuffer)))
	{
		return false;
	}

	//Texture用ConstantBuffer作成
	buffer_desc.ByteWidth = sizeof(TexConstantBuffer);

	if (FAILED(m_Device->CreateBuffer(&buffer_desc, nullptr, &m_TexConstantBuffer)))
	{
		return false;
	}

	return true;
}

//RenderTargetBlend設定関数
void DirectGraphics::SetUpRenderState()
{
	ID3D11BlendState* pBlendState = nullptr;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.IndependentBlendEnable = false;
	BlendDesc.RenderTarget[0].BlendEnable = true;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	/*BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_Device->CreateBlendState(&BlendDesc, &pBlendState);
	m_Context->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);
}

//シェーダ作成関数
bool DirectGraphics::CreateShader()
{
	if (ShaderManager::Instance()->CreateShader() == false)
	{
		return false;
	}

	return true;
}

//ViewPort設定関数
void DirectGraphics::SetUpViewPort()
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	//ビューポートの設定
	m_ViewPort[0].TopLeftX = 0;								// 左上X座標
	m_ViewPort[0].TopLeftY = 0;								// 左上Y座標
	m_ViewPort[0].Width = (float)(rect.right - rect.left);	// 横幅
	m_ViewPort[0].Height = (float)(rect.bottom - rect.top);	// 縦幅
	m_ViewPort[0].MinDepth = 0.0f;							// 最小深度
	m_ViewPort[0].MaxDepth = 1.0f;							// 最大深度

	m_ViewPort[1] = m_ViewPort[0];

	m_ViewPort[1].Width = (float)(rect.right - rect.left) * 2;	// 横幅
	m_ViewPort[1].Height = (float)(rect.bottom - rect.top) * 2;	// 縦幅
}

//ライト設定関数
void DirectGraphics::SetLight()
{
	Vector3 light_pos_vec3 = Vector3(0.0f, 50.0f, 50.0f);

	// ライトの座標
	DirectX::XMVECTOR light_pos = DirectX::XMVectorSet(light_pos_vec3.x, light_pos_vec3.y, light_pos_vec3.z, 0.0f);
	// ライトの注視点
	DirectX::XMVECTOR light_forcus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//ライトの上向きベクトル
	DirectX::XMVECTOR light_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//ビュー座標変換用の行列算出 start
	DirectX::XMMATRIX mat_view = DirectX::XMMatrixLookAtLH(light_pos, light_forcus, light_up);
	DirectX::XMStoreFloat4x4(&DirectGraphics::Instance()->GetConstantBufferData()->LightView, DirectX::XMMatrixTranspose(mat_view));


	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	// プロジェクションマトリクス設定
	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = (float)(rect.right - rect.left) / (rect.bottom - rect.top);
	float    nearZ = 0.1f;
	float    farZ = 2000.0f;
	//DirectX::XMMATRIX proj_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	DirectX::XMMATRIX proj_matrix = DirectX::XMMatrixOrthographicLH(LightViewWight, LightViewHeight, nearZ, farZ);

	// コンスタントバッファの設定
	DirectX::XMStoreFloat4x4(&DirectGraphics::Instance()->GetConstantBufferData()->LightProjection, XMMatrixTranspose(proj_matrix));

	// ライトのカラー設定
	m_ConstantBufferData.LightColor = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1);

	// ライトの設定
	DirectX::XMVECTOR light_vec = DirectX::XMVector3Normalize(DirectX::XMVectorSet(-light_pos_vec3.x, -light_pos_vec3.y, -light_pos_vec3.z, 0.0f));
	XMStoreFloat4(&m_ConstantBufferData.LightVector, light_vec);
}

void DirectGraphics::SetUpRnderTaget()
{
	// プリミティブの形状を指定
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// (OutputManger)RnderTagetの指定
	m_Context->OMSetRenderTargets(
		1,							// 使用するViewの数
		&m_RenderTargetView,		// 使用するRenderTargetView
		m_DepthStencilView);		// 使用するDepthStencilView
}



void DirectGraphics::SetUpShadowRnderTaget()
{
	// プリミティブの形状を指定
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// (OutputManger)RnderTagetの指定
	m_Context->OMSetRenderTargets(
		1,							// 使用するViewの数
		&m_ShadowRenderTargetView,  // 使用するRenderTargetView
		m_ShadowDepthStencilView);		// 使用するDepthStencilView
}

void DirectGraphics::SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* dxgi)
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	/*
		DirectX11版PresentationParameter
			バッファの数やサイズ、カラーフォーマット等を設定する
	*/
	ZeroMemory(dxgi, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxgi->BufferCount = 1;									// バッファの数
	dxgi->BufferDesc.Width = (rect.right - rect.left);		// バッファの横幅
	dxgi->BufferDesc.Height = (rect.bottom - rect.top);		// バッファの縦幅
	dxgi->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラーフォーマット
	dxgi->BufferDesc.RefreshRate.Numerator = 60;			// リフレッシュレートの分母
	dxgi->BufferDesc.RefreshRate.Denominator = 1;			// リフレッシュレートの分子
	dxgi->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使い方 Usage => 使用方法
	dxgi->OutputWindow = window_handle;						// 出力対象のウィンドウハンドル
	dxgi->SampleDesc.Count = 1;								// マルチサンプリングのサンプル数(未使用は1)
	dxgi->SampleDesc.Quality = 0;							// マルチサンプリングの品質(未使用は0)
	dxgi->Windowed = true;									// ウィンドウモード指定
}

void DirectGraphics::StartRendering()
{
	float clear_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// RenderTarget塗りつぶしカラー(RGBA)

	// DirecX9ではRenderTargetとDepth、Stencilバッファのクリアは別々にする
	m_Context->ClearRenderTargetView(
		m_RenderTargetView,							// クリア対象のView
		clear_color);								// クリアカラー

	m_Context->ClearDepthStencilView(
		m_DepthStencilView,							// クリア対象のView	
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// クリアフラグ(今回はDepth、Stencilともにクリア)
		1.0f,										// 深度クリア値
		0);										    // ステンシルクリア値

	m_Context->RSSetViewports(
		1,					// 設定するビューポートの数
		&m_ViewPort[0]);		// 設定するビューポート情報のポインタ
}

void DirectGraphics::ShadowStartRendering()
{
	float clear_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// RenderTarget塗りつぶしカラー(RGBA)

	// DirecX9ではRenderTargetとDepth、Stencilバッファのクリアは別々にする
	m_Context->ClearRenderTargetView(
		m_ShadowRenderTargetView,					// クリア対象のView
		clear_color);								// クリアカラー

	m_Context->ClearDepthStencilView(
		m_ShadowDepthStencilView,					// クリア対象のView	
		D3D11_CLEAR_DEPTH ,	                        // クリアフラグ(今回はDepth、Stencilともにクリア)
		1.0f,										// 深度クリア値
		0);										    // ステンシルクリア値

	m_Context->RSSetViewports(
		1,					// 設定するビューポートの数
		&m_ViewPort[1]);		// 設定するビューポート情報のポインタ
}

void DirectGraphics::FinishRendering()
{
	// 描画コマンドを送信する
	m_SwapChain->Present(
		/*
			垂直同期のタイミング
				0なら即時描画
				1以上なら指定した値の垂直同期後に描画
		*/
		0,
		/*
			出力オプション
				フレーム出力を行うためのオプションで
				基本は0で問題ないと思われる
				その他のフラグはDXGI_PRESENTで確認可能
		*/
		0);	// 出力オプション
}

void DirectGraphics::SetTexture(ID3D11ShaderResourceView* texture)
{
	// Samplerの設定
	m_Context->PSSetSamplers(
		0,					// スロット番号
		1,					// サンプラーの数
		&m_SamplerState);	// ID3D11SamplerState

	// PixelShaderで使用するテクスチャの設定
	m_Context->PSSetShaderResources(
		0,								// スロット番号
		1,								// リソースの数
		&texture);						// ID3D11ShaderResourceView

	m_Context->PSSetShaderResources(
		1,								// スロット番号
		1,								// リソースの数
		&m_ShadowResourceView);						// ID3D11ShaderResourceView

}

void DirectGraphics::Release()
{
	ShaderManager::Instance()->Release();

	if (m_Context != nullptr)
	{
		m_Context->ClearState();
		m_Context->Flush();
		m_Context->Release();
		m_Context = nullptr;
	}

	if (m_DepthStencilView != nullptr)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = nullptr;
	}

	if (m_RenderTargetView != nullptr)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_ShadowDepthStencilView != nullptr)
	{
		m_ShadowDepthStencilView->Release();
		m_ShadowDepthStencilView = nullptr;
	}

	if (m_ShadowRenderTargetView != nullptr)
	{
		m_ShadowRenderTargetView->Release();
		m_ShadowRenderTargetView = nullptr;
	}

	if (m_ShadowResourceView != nullptr)
	{
		m_ShadowResourceView->Release();
		m_ShadowResourceView = nullptr;
	}

	if (m_SwapChain != nullptr)
	{
		m_SwapChain->Release();
		m_SwapChain = nullptr;
	}

	if (m_Device != nullptr)
	{
		m_Device->Release();
		m_Device = nullptr;
	}
}
