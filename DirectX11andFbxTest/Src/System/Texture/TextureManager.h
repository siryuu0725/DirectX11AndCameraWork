/**
 * @file TextureManager.h
 * @brief 床オブジェクト
 * @details　中身：床クラス
**/

#include "Texture.h"
#include <string>
#include <unordered_map>


#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

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
	void Draw(std::string kye_, Vector3 pos_, Vector3 scale_, Vector3 angle_);

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
	void LoadTexture(std::string key_, TexOriginPoint type_, const char* file_name_);

private:
	static TextureManager* mp_instance;

	std::unordered_map<std::string, Texture*> m_texture;

};


#endif