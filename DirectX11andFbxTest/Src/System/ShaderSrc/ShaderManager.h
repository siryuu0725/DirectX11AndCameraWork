/**
 * @file ShaderManager.h
 * @brief 各シェーダーの管理を行う
 * @details 中身：ShaderManagerクラス
**/

#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_
#include "PixelShader.h"
#include "VertexShader.h"
#include <map>
#include <string>
#include <vector>

//描画タイプ
enum class DrawType
{
	MainDraw,     //メイン
	ShadowDraw,	  //影
	VertexMaxNum
};

//頂点シェーダ種類
enum class VertexShaderType
{
	StaticVertex,
	SkinVertex,
	ShadowStaticVertex,
	ShadowSkinVertex,
	VertexMaxNum
};

//ピクセルシェーダ種類
enum class PixelShaderType
{
	TexPixel,
	ShadowTexPixel,
	PixelMaxNum
};


class ShaderManager
{
public:
	ShaderManager() {}
	~ShaderManager() {}

	static ShaderManager* Instance();

	/**
	* @brief シェーダ作成関数
	* @details 頂点・ピクセルシェーダを各種類文作成する
	* @return 作成結果 成功(true)
	*/
	bool CreateShader();

	/**
	* @brief シェーダセット関数
　　* @param[in] vertex_type_ 頂点シェーダの種類
	* @param[in] pixel_type_ ピクセルシェーダの種類
	* @details 指定した頂点・ピクセルシェーダをセットする
	*/
	void SetUpShader(VertexShaderType vertex_type_, PixelShaderType pixel_type_);

	/**
    * @brief シェーダ解放関数
	* @details 頂点・ピクセルシェーダの解放を行う
	*/
	void Release();

	VertexShader* GetVertexShader(VertexShaderType type_)
	{
		return m_VertexShader[(__int8)type_];
	}
private:
	static ShaderManager* mp_instance;

	std::vector<PixelShader*> m_PixelShader;    //!ピクセルシェーダ
	std::vector<VertexShader*> m_VertexShader;	//!頂点シェーダ
};


#endif
