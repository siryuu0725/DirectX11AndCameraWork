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
	LoadTexture(SceneTextureType::TitleScene, "BG", "Res/Texture/TitleScene.png");
	LoadTexture(SceneTextureType::GameScene, "NowLoading", "Res/Texture/NowLoading.png");
	LoadTexture(SceneTextureType::EndScene, "BG", "Res/Texture/EndScene.png");
}

//テクスチャ読み込み関数
void TextureManager::LoadTexture(SceneTextureType type_, std::string key_, const char* file_name_)
{
	switch (type_)
	{
	case SceneTextureType::TitleScene:
		m_title_texture[key_] = new Texture();
		m_title_texture[key_]->LoadTexture(file_name_);
		break;
	case SceneTextureType::GameScene:
		m_game_texture[key_] = new Texture();
		m_game_texture[key_]->LoadTexture(file_name_);
		break;
	case SceneTextureType::EndScene:
		m_end_texture[key_] = new Texture();
		m_end_texture[key_]->LoadTexture(file_name_);
		break;
	default:
		break;
	}
	

}

//テクスチャ描画関数
void TextureManager::Draw(SceneTextureType type_, std::string key_, Vector3 pos_)
{
	switch (type_)
	{
	case SceneTextureType::TitleScene:
		m_title_texture[key_]->Draw(pos_);
		break;
	case SceneTextureType::GameScene:
		m_game_texture[key_]->Draw(pos_);
		break;
	case SceneTextureType::EndScene:
		m_end_texture[key_]->Draw(pos_);
		break;
	default:
		break;
	}
}

//テクスチャ解放関数
void TextureManager::Release()
{
	for (auto title_tex : m_title_texture)
	{
		delete title_tex.second;
	}

	for (auto game_tex : m_game_texture)
	{
		delete game_tex.second;
	}

	for (auto end_tex : m_end_texture)
	{
		delete end_tex.second;
	}

	m_title_texture.clear();
	m_game_texture.clear();
	m_end_texture.clear();
}

