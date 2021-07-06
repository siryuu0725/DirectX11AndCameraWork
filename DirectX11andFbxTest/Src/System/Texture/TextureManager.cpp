#include "TextureManager.h"

TextureManager* TextureManager::mp_instance = nullptr;

TextureManager* TextureManager::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new TextureManager; }

	return mp_instance;
}

//テクスチャ読み込みまとめ
void TextureManager::Load()
{
	LoadTexture("TitleBG", TexOriginPoint::LeftTop,"Res/Texture/TitleScene.png");
	LoadTexture("NowLoading", TexOriginPoint::LeftTop, "Res/Texture/NowLoading.png");
	LoadTexture("LoadingIcon", TexOriginPoint::Center, "Res/Texture/LoadingIcon.png");
	LoadTexture("EndBG", TexOriginPoint::LeftTop, "Res/Texture/EndScene.png");
}

//テクスチャ読み込み関数
void TextureManager::LoadTexture(std::string key_, TexOriginPoint type_ ,const char* file_name_)
{
	m_texture[key_] = new Texture();
	m_texture[key_]->LoadTexture(file_name_, type_);
}

//テクスチャ描画関数
void TextureManager::Draw(std::string key_, Vector3 pos_, Vector3 scale_, Vector3 angle_)
{
	m_texture[key_]->Draw(pos_, scale_, angle_);
}

//テクスチャ解放関数
void TextureManager::Release()
{
	for (auto tex : m_texture)
	{
		delete tex.second;
	}

	m_texture.clear();
}

