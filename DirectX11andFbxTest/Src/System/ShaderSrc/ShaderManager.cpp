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
	for (__int8 i = 0; i < (__int8)VertexShaderType::VertexMaxNum; i++)
	{
		m_VertexShader.push_back(new VertexShader);
	}

	for (__int8 i = 0; i < (__int8)PixelShaderType::PixelMaxNum; i++)
	{
		m_PixelShader.push_back(new PixelShader);
	}
	//m_VertexShader[VertexShaderType::StaticVertex] = new VertexShader();
	//m_VertexShader[VertexShaderType::SkinVertex] = new VertexShader();
	//m_PixelShader[PixelShaderType::TexPixel] = new PixelShader();

	if (m_VertexShader[(__int8)VertexShaderType::StaticVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/StaticVertexShader.cso") == false)
	{
		return false;
	}
	if (m_VertexShader[(__int8)VertexShaderType::SkinVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/SkinVertexShader.cso") == false)
	{
		return false;
	}


	if (m_PixelShader[(__int8)PixelShaderType::TexPixel]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/TexPixelShader.cso") == false)
	{
		return false;
	}


	if (m_VertexShader[(__int8)VertexShaderType::ShadowStaticVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/ShadowStaticVertexShader.cso") == false)
	{
		return false;
	}
	if (m_VertexShader[(__int8)VertexShaderType::ShadowSkinVertex]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/ShadowSkinVertexShader.cso") == false)
	{
		return false;
	}

	if (m_PixelShader[(__int8)PixelShaderType::ShadowTexPixel]->Create(DirectGraphics::Instance()->GetDevice(),
		"Res/Shader/ShadowPixelShader.cso") == false)
	{
		return false;
	}

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
	/*for (auto itr = m_VertexShader.begin(); itr != m_VertexShader.end(); ++itr)
	{
		if (m_VertexShader[itr] != nullptr)
		{
			delete m_VertexShader;
			m_VertexShader = nullptr;
		}
	}*/

	for (__int8 i = 0; i < (__int8)VertexShaderType::VertexMaxNum; i++)
	{
		if (m_VertexShader[i] != nullptr)
		{
			delete m_VertexShader[i];
			m_VertexShader[i] = nullptr;
		}
	}

	for (__int8 i = 0; i < (__int8)PixelShaderType::PixelMaxNum; i++)
	{
		if (m_PixelShader[i] != nullptr)
		{
			delete m_PixelShader[i];
			m_PixelShader[i] = nullptr;
		}
	}
}
