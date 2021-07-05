/**
 * @file Texture.h
 * @brief 床オブジェクト
 * @details　中身：床クラス
**/

#include <map>
#include <string>
#include "../DirectGraphics.h"

#ifndef TEXTURE_H_
#define TEXTURE_H_

class Texture
{
public:
	Texture() {}
	~Texture() {}

	struct TexCustomVertex
	{
		float pos[3];	//x-y-z
		float tex[2];	//x-y
	};

public :

	/**
	* @brief テクスチャ読み込み関数
	* @param[in] file_name_ ファイルパス
	* @details 入力レイアウト、VertexBuffer、IndexBufferの作成も行う
	* @return 読み込みの成否 成功(true)
	*/
	bool LoadTexture(const char* file_name_);

	/**
	* @brief テクスチャ描画関数
	* @param[in] pos_ 描画座標
	*/
	void Draw(Vector3 pos_);

private:
	/**
	* @brief 入力レイアウト作成関数
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	* @details 入力レイアウトを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateInputLayout(ID3D11Device* device);

	/**
	* @brief VertexBuffer作成関数
	* @param[in] device DirectX11グラフィクスデバイス
	* @details VertexBufferを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateVertexBuffer(TexCustomVertex* vertices, ID3D11Buffer** outvertex_buffer_);

	/**
	* @brief IndexBuffer作成関数
	* @param[in] device DirectX11グラフィクスデバイス
	* @details IndexBufferを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateIndexBuffer(WORD* indices_ ,ID3D11Buffer** outindex_buffer_);
	
private:
	ID3D11ShaderResourceView* m_Textures;       //!テクスチャ情報保存
	ID3D11InputLayout* m_InputLayout;			//!入力レイアウト

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;

	UINT m_tex_width;
	UINT m_tex_height;
};


#endif