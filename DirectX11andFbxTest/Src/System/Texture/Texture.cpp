#include "Texture.h"
#include "../WICTextureLoader.h"
#include "../ShaderSrc/ShaderManager.h"
#include "../Window.h"
#include <DirectXTex.h>
#include <wincodec.h>
#include <codecvt>
#include "../../Utility/Calculation.h"

//テクスチャ読み込み関数
bool Texture::LoadTexture(const char* file_name_, TexOriginPoint point_type_)
{
	std::string file_name = file_name_;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(file_name);

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	if (FAILED(DirectX::LoadFromWICFile(wstr_file_name.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image)))						// シェーダで使用することができるテクスチャデータ
	{
		return false;
	}

	if (FAILED(DirectX::CreateShaderResourceView(
		DirectGraphics::Instance()->GetDevice(),
		image.GetImages(),
		image.GetImageCount(),
		metadata,
		&m_Textures)))
	{
		return false;
	}

	m_tex_width = (float)metadata.width;
	m_tex_height = (float)metadata.height;

	/*float top = 1.0f - (0.0f / (GetWindowSize().y / 2));
	float bottom = 1.0f - ((0.0f + m_tex_height) / (GetWindowSize().y / 2));

	float left = (0.0f / (GetWindowSize().x / 2)) - 1.0f;
	float right = ((0.0f + m_tex_width) / (GetWindowSize().x / 2)) - 1.0f;

	TexCustomVertex vertices[4]
	{
		{ {left,top,0.0f},{0.0f,0.0f} },
		{ {right,top,0.0f},{1.0f,0.0f} },
		{ {right,bottom,0.0f},{1.0f,1.0f} },
		{ {left,bottom,0.0f},{0.0f,1.0f} }
	};*/

	float top = (m_tex_height / 2);
	float bottom = (m_tex_height / 2);
	float left = (m_tex_width / 2);
	float right = (m_tex_width / 2);

	switch (point_type_)
	{
	case TexOriginPoint::LeftTop:
		/*vertices[4]=
		{
			{ {0.0f,0.0f,0.0f},{0.0f,0.0f} },
			{ {m_tex_width,0.0f,0.0f},{1.0f,0.0f} },
			{ {m_tex_width,m_tex_height,0.0f},{1.0f,1.0f} },
			{ {0.0f,m_tex_height,0.0f},{0.0f,1.0f} }
		};*/
		top = 0.0f;
		bottom = m_tex_height;

		left = 0.0f;
		right = m_tex_width;
		break;
	case TexOriginPoint::Center:
		top = -top;

		left = -left;
		break;
	default:
		break;
	}

	TexCustomVertex vertices[4]
	{
		{ {left,top,0.0f},{0.0f,0.0f} },
		{ {right,top,0.0f},{1.0f,0.0f} },
		{ {right,bottom,0.0f},{1.0f,1.0f} },
		{ {left,bottom,0.0f},{0.0f,1.0f} }
	};

	WORD indices[6]
	{
		0,1,3,
		1,2,3
	};

	//頂点バッファ作成
	CreateVertexBuffer(vertices, &m_vertex_buffer);

	//インデックスバッファ作成
	CreateIndexBuffer(indices, &m_index_buffer);

	if (CreateInputLayout(DirectGraphics::Instance()->GetDevice()) == false)
	{
		return false;
	}

	return true;
}

//入力レイアウト作成関数
bool Texture::CreateInputLayout(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点レイアウト作成
	if (FAILED(device->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		ShaderManager::Instance()->GetVertexShader(VertexShaderType::TexVertex)->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		ShaderManager::Instance()->GetVertexShader(VertexShaderType::TexVertex)->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout)))			// 作成された頂点レイアウトの格納先
	{
		return false;
	}

	return true;
}

//テクスチャ描画関数
void Texture::Draw(Vector3 pos_, Vector3 scale_, Vector3 angle_)
{

	ID3D11DeviceContext* context = DirectGraphics::Instance()->GetContext();
	//入力レイアウトセット
	context->IASetInputLayout(m_InputLayout);

	//シェーダ設定
	ShaderManager::Instance()->SetUpShader(VertexShaderType::TexVertex, PixelShaderType::TexPixel);

	//各行列作成
	DirectX::XMMATRIX world_matrix = Calculation::Matrix(pos_, scale_, angle_);
	//DirectX::XMMATRIX world_matrix = DirectX::XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
	//DirectX::XMMATRIX world_matrix = Calculation::Matrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	float width = GetWindowSize().x;
	float height = GetWindowSize().y;
	DirectX::XMMATRIX proj_matrix = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, GetWindowSize().x, GetWindowSize().y, 0.0f, 0.0f, 1.0f);

	//Texture用コンスタントバッファデータ取得
	TexConstantBuffer* constant_buffer_data = DirectGraphics::Instance()->GetTexConstantBufferData();

	// ワールドマトリクスをコンスタントバッファに設定
	XMStoreFloat4x4(&constant_buffer_data->World, DirectX::XMMatrixTranspose(world_matrix));
	XMStoreFloat4x4(&constant_buffer_data->Projection, DirectX::XMMatrixTranspose(proj_matrix));

	ID3D11Buffer* constant_buffer = DirectGraphics::Instance()->GetTexConstantBuffer();

	// コンスタントバッファ更新
	DirectGraphics::Instance()->GetContext()->UpdateSubresource(constant_buffer, 0, nullptr, constant_buffer_data, 0, 0);

	// コンテキストにコンスタントバッファを設定
	DirectGraphics::Instance()->GetContext()->VSSetConstantBuffers(0, 1, &constant_buffer);


	UINT strides = sizeof(TexCustomVertex);
	UINT offset = 0;

	context->IASetVertexBuffers(
		0,								// バッファ送信のスロット番号
		1,								// バッファの数
		&m_vertex_buffer,	                // 頂点バッファ
		&strides,						// バッファに使用している構造体のサイズ
		&offset);					    // 開始オフセット

	context->IASetIndexBuffer(
		m_index_buffer,
		DXGI_FORMAT_R16_UINT,
		0);



	context->PSSetShaderResources(0, 1, &m_Textures);


	ID3D11SamplerState* sampler = DirectGraphics::Instance()->GetSamplerState();
	context->PSSetSamplers(0, 1, &sampler);

	context->DrawIndexed(6, 0, 0);

	
}

//VertexBuffer作成関数
bool Texture::CreateVertexBuffer(TexCustomVertex* vertices,ID3D11Buffer** outvertex_buffer_)
{
	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(TexCustomVertex) * 4;	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = vertices;	// バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(DirectGraphics::Instance()->GetDevice()->CreateBuffer(
		&buffer_desc,								// バッファ情報
		&sub_resource,								// リソース情報
		outvertex_buffer_)))	// 作成されたバッファの格納先
	{
		return false;
	}

	return true;
}

//IndexBuffer作成関数
bool Texture::CreateIndexBuffer(WORD* indices_,ID3D11Buffer** outindex_buffer_)
{
	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(WORD) * 6;            // バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = indices_;	    // バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(DirectGraphics::Instance()->GetDevice()->CreateBuffer(
		&buffer_desc,								// バッファ情報
		&sub_resource,								// リソース情報
		outindex_buffer_)))	// 作成されたバッファの格納先
	{
		return false;
	}

	return true;
}
