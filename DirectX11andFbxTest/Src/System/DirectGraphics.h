/**
 * @file DirectGraphics.h
 * @brief 描画辺りを設定している
**/
#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "../System/ShaderSrc/VertexShader.h"
#include "../System/ShaderSrc/PixelShader.h"
#include "../Utility/Utility.h"
#include "ShaderSrc/ShaderManager.h"

constexpr float LightViewWight = 300.0f;  //ライトから見た時のViewの横幅
constexpr float LightViewHeight = 300.0f; //ライトから見た時のViewの縦幅


struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
	DirectX::XMMATRIX   Bone[255];
	DirectX::XMFLOAT4	CameraPos;
	DirectX::XMFLOAT4X4 LightView;
	DirectX::XMFLOAT4X4 LightProjection;
	DirectX::XMFLOAT4	LightVector;
	DirectX::XMFLOAT4   LightColor;
	DirectX::XMFLOAT4	MaterialAmbient;
	DirectX::XMFLOAT4	MaterialDiffuse;
	DirectX::XMFLOAT4	MaterialSpecular;
};

//Texture用
struct TexConstantBuffer
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 Projection;
};

struct CustomVertex
{
	Vector3 Position;		// 座標(x, y, z)
	Vector3 Normal;			// 法線
	Color Color;			// 頂点カラー
	Vector2 TexturePos;		// テクスチャ座標(u, v)

	UINT  Index[4];
	float Weight[4];

	CustomVertex()
	{
		ZeroMemory(this, sizeof(CustomVertex));
	}
};


struct ObjMaterial
{
	ObjMaterial()
	{
		for (int i = 0; i < 4; i++)
		{
			Ambient[i] = 1.0f;
			Diffuse[i] = 1.0f;
			Specular[i] = 1.0f;
		}
		//TextureKeyWord = "";
		//TextureName = "";
	}

	void SetAmbient(float r, float g, float b, float factor)
	{
		Ambient[0] = r;
		Ambient[1] = g;
		Ambient[2] = b;
		Ambient[3] = factor;
	}

	void SetDiffuse(float r, float g, float b, float factor)
	{
		Diffuse[0] = r;
		Diffuse[1] = g;
		Diffuse[2] = b;
		Diffuse[3] = factor;
	}

	void SetSpecular(float r, float g, float b, float factor)
	{
		Specular[0] = r;
		Specular[1] = g;
		Specular[2] = b;
		Specular[3] = factor;
	}

	void SerAlpha(float a_)
	{
		Alpha = a_;
	}

	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	float Alpha;
	//std::string TextureKeyWord;
	//std::string TextureName;
};

class DirectGraphics
{
public:
	/**
　　* @brief  インスタンス化関数
　　*/
	static DirectGraphics* Instance();


	DirectGraphics() :
		m_Device(nullptr),
		m_Context(nullptr),
		m_SwapChain(nullptr),
		m_RenderTargetView(nullptr),
		m_DepthStencilTexture(nullptr),
		m_DepthStencilView(nullptr)
		/*m_VertexShader(nullptr),
		m_PixelShader(nullptr)*/
	{}

	~DirectGraphics() {}

	/**
	* @brief 初期化関数
	* @details DirectX11の初期化を行い、成功したらtrue、失敗したらfalseを返す
	* @return 初期化の成否 成功(true)
	*/
	bool Init();

	/**
	* @brief DirectX11の解放関数
	* @details 保持しているデバイスなどを解放する
	*/
	void Release();

	/**
	* @brief 描画開始関数
	* @details 描画処理を行う場合、必ずこの関数の後に実行する
	* @details 実行しないと描画されない
	*/
	void StartRendering();

	/**
	* @brief 描画終了関数
	* @details 描画処理が完了した場合、必ずこの関数を実行する
	* @details 実行しないと描画内容が反映されない
	*/
	void FinishRendering();

	/**
	* @brief ライト設定関数
	* @details ライト情報の設定を行う
	*/
	void SetLight();

	/**
	* @brief RnderTaget設定関数
	*/
	void SetUpRnderTaget();

	/**
	* @brief 影描画開始関数
	* @details 描画処理を行う場合、必ずこの関数の後に実行する
	* @details 実行しないと描画されない
	*/
	void ShadowStartRendering();

	/**
	* @brief  影描画用RnderTaget設定関数
	*/
	void SetUpShadowRnderTaget();

	ID3D11Device* GetDevice()
	{
		return m_Device;
	}

	ID3D11Buffer* GetConstantBuffer()
	{
		return m_ConstantBuffer;
	}

	ID3D11Buffer* GetTexConstantBuffer()
	{
		return m_TexConstantBuffer;
	}

	ConstantBuffer* GetConstantBufferData()
	{
		return &m_ConstantBufferData;
	}

	TexConstantBuffer* GetTexConstantBufferData()
	{
		return &m_TexConstantBufferData;
	}

	ID3D11DeviceContext* GetContext()
	{
		return m_Context;
	}

	ID3D11SamplerState* GetSamplerState()
	{
		return m_SamplerState;
	}

	/**
	* @brief TextureSet関数
	* @details 使用するTextureを設定する
	* @param[out] texture texture情報
	*/
	void SetTexture(ID3D11ShaderResourceView* texture);

	/**
	*	@brief Instance解放関数(Debug用)
	*	@details 手動でインスタンスを解放する
	*/
	void ReleaseInstance();

private:
	/**
	* @brief DXGI_SWAP_CHAIN_DESCの設定関数
	* @details SwapChainを作成するうえで必要な設定をDXGI_SWAP_CHAIN_DESCに行う
	* @param[out] dxgi 設定を行うDXGI_SWAP_CHAIN_DESCのポインタ
	*/
	void SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* dxgi);

	/**
	* @brief DeviceとSwapChainの作成関数
	* @details DirectX11のDeviceとSwapChainを作成する
	* @details まとめた理由は生成関数をD3D11CreateDeviceAndSwapChainにしたため
	* @return 作成の成否 成功(true)
	*/
	bool CreateDeviceAndSwapChain();

	/**
	* @brief RenderTargetViewの作成関数
	* @details DirectX11のRenderTargetViewを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateRenderTargetView();

	/**
	* @brief 影描画用RenderTargetView作成関数
	* @details 影描画用のRenderTargetViewを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateShadowView();

	/**
	* @brief DepthバッファStencilバッファ作成関数
	* @return 作成の成否 成功(true)
	*/
	bool CreateDepthAndStencilView();

	/**
	* @brief シェーダ作成関数
	* @details 今回のプロジェクトで使用するシェーダを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateShader();

	/**
	* @brief ViewPort設定関数
	* @details ContextにViewPortの設定を行う関数@n
	* @details ゲーム中に変更がなければ最初に１度行えば問題ない
	*/
	void SetUpViewPort();

	/**
	* @brief TextureSampler作成関数
	* @details TextureSamplerの設定を行い、ID3D11SamplerStateを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateTextureSampler();

	/**
	* @brief ConstantBuffer作成関数
	* @details TextureSamplerの設定を行い、ID3D11SamplerStateを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateConstantBuffer();

	/**
	* @brief RenderTargetBlend設定関数
	* @details RenderTargetのBlend設定を行う
	*/
	void SetUpRenderState();

private:
	static DirectGraphics* mp_instance;

	ID3D11Device* m_Device;												//!DirectX11のDeviceのInterface
	ID3D11DeviceContext* m_Context;										//!Context
	IDXGISwapChain* m_SwapChain;										//!SwapChainのInterface
	ID3D11RenderTargetView* m_RenderTargetView;							//!RenderTargetViewのInterface
	ID3D11Texture2D* m_DepthStencilTexture;								//!ID3D11DepthStencilViewを生成するためのテクスチャ
	ID3D11DepthStencilView* m_DepthStencilView;							//!DepthStencilViewのInterface
	ID3D11SamplerState* m_SamplerState;									//!Textureサンプラー
	ID3D11Buffer* m_ConstantBuffer;										//!定数バッファ
	ConstantBuffer m_ConstantBufferData;								//!ConstantBufferデータ

	ID3D11Buffer* m_TexConstantBuffer;									//!Texture用定数バッファ
	TexConstantBuffer m_TexConstantBufferData;                          //!Texture用ConstantBufferデータ

	ID3D11RenderTargetView* m_ShadowRenderTargetView;					//!ShadowRenderTargetViewのInterface
	ID3D11Texture2D* m_ShadowTexture;
	ID3D11Texture2D* m_ShadowDepthStencilTexture;						//!ID3D11DepthStencilViewを生成するためのテクスチャ
	ID3D11DepthStencilView* m_ShadowDepthStencilView;					//!DepthStencilViewのInterface
	ID3D11ShaderResourceView* m_ShadowResourceView;

	D3D11_BLEND_DESC BlendDesc;
	D3D11_VIEWPORT m_ViewPort[2];
};

#endif
