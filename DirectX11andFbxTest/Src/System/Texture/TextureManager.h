/**
 * @file TextureManager.h
 * @brief 床オブジェクト
 * @details　中身：床クラス
**/

#include <map>
#include <string>
#include "../DirectGraphics.h"
#include "Texture.h"

#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

//Textureを使用するシーンの種類
enum class SceneTextureType
{
	TitleScene,
	GameScene,
	EndScene,
	TypeNum,
};

class TextureManager
{
public:
	TextureManager() {}
	~TextureManager() {}

	static TextureManager* Instance();

	/**
	* @brief テクスチャ読み込みまとめ
	* @details 全てのテクスチャをここで読み込む
	*/
	void Load();

	/**
	* @brief テクスチャ描画関数
	* @param[in] pos_ 描画座標
	*/
	void Draw(SceneTextureType type_, std::string kye_,Vector3 pos_);

	/**
	* @brief テクスチャ解放関数
	*/
	void Release();

private:
	/**
	* @brief テクスチャ読み込み関数
	* @param[in] file_name_ ファイルパス
	* @details 入力レイアウト、VertexBuffer、IndexBufferの作成も行う
	*/
	void LoadTexture(SceneTextureType type_, std::string key_,const char* file_name_);

private:
	static TextureManager* mp_instance;

	std::map<std::string, Texture*> m_title_texture;
	std::map<std::string, Texture*> m_game_texture;
	std::map<std::string, Texture*> m_end_texture;

};


#endif