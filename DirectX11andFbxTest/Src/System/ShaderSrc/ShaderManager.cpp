#include "ShaderManager.h"
#include "../DirectGraphics.h"

ShaderManager* ShaderManager::mp_instance = nullptr;

ShaderManager* ShaderManager::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new ShaderManager; }

	return mp_instance;
}

//シェーダ作成関数
bool ShaderManager::CreateShader()
{
	//頂点シェーダー追加
	for (__int8 i = 0; i < (__int8)VertexShaderType::VertexMaxNum; i++)
	{
		m_VertexShader.push_back(new VertexShader);
	}

	//ピクセルシェーダー追加
	for (__int8 i = 0; i < (__int8)PixelShaderType::PixelMaxNum; i++)
	{
		m_PixelShader.push_back(new PixelShader);
	}

#pragma region オブジェクト描画用
	//静的オブジェクト用頂点シェーダー
	if (m_VertexShader[(__int8)VertexShaderType::StaticVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/StaticVertexShader.cso") == false)
	{
		return false;
	}
	//スキンありオブジェクト用頂点シェーダー
	if (m_VertexShader[(__int8)VertexShaderType::SkinVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/SkinVertexShader.cso") == false)
	{
		return false;
	}

	//オブジェクト用ピクセルシェーダー
	if (m_PixelShader[(__int8)PixelShaderType::NomalPixel]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/PixelShader.cso") == false)
	{
		return false;
	}
#pragma endregion

#pragma region 影描画用
	//影描画用頂点シェーダー(静的オブジェクト)
	if (m_VertexShader[(__int8)VertexShaderType::ShadowStaticVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/ShadowStaticVertexShader.cso") == false)
	{
		return false;
	}
	//影描画用頂点シェーダー(スキンありオブジェクト)
	if (m_VertexShader[(__int8)VertexShaderType::ShadowSkinVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/ShadowSkinVertexShader.cso") == false)
	{
		return false;
	}
	//影描画用ピクセルシェーダー
	if (m_PixelShader[(__int8)PixelShaderType::ShadowTexPixel]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/ShadowPixelShader.cso") == false)
	{
		return false;
	}
#pragma endregion

#pragma region 2DTexture描画用
	//2DTexture用頂点シェーダー
	if (m_VertexShader[(__int8)VertexShaderType::TexVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/TexVertexShader.cso") == false)
	{
		return false;
	}
	//2DTexture用ピクセルシェーダー
	if (m_PixelShader[(__int8)PixelShaderType::TexPixel]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/TexPixelShader.cso") == false)
	{
		return false;
	}
#pragma endregion

	return true;
}

//シェーダセット関数
void ShaderManager::SetUpShader(VertexShaderType type_, PixelShaderType pixel_type_)
{
	//VerteXShader、PixelShaderを設定
	DirectGraphics::Instance()->GetContext()->VSSetShader(m_VertexShader[(__int8)type_]->GetShaderInterface(), nullptr, 0);
	DirectGraphics::Instance()->GetContext()->PSSetShader(m_PixelShader[(__int8)pixel_type_]->GetShaderInterface(), nullptr, 0);
}

//シェーダ解放関数
void ShaderManager::Release()
{
	for (__int8 i = 0; i < (__int8)VertexShaderType::VertexMaxNum; i++)
	{
		if (m_VertexShader[i] != nullptr)
		{
			delete m_VertexShader[i];
		}
	}

	for (__int8 i = 0; i < (__int8)PixelShaderType::PixelMaxNum; i++)
	{
		if (m_PixelShader[i] != nullptr)
		{
			delete m_PixelShader[i];
		}
	}
}

//Instance解放関数(Debug用)
void ShaderManager::ReleaseInstance()
{
	delete mp_instance;
}
