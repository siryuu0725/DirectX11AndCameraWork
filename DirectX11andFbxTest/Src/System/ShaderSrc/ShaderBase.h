/**
 * @file ShaderBase.h
 * @brief 各Shaderの基底クラスを持つ
**/
#ifndef SHADER_BASE_H_
#define SHADER_BASE_H_

#include <d3d11.h>
#include <stdio.h>

class ShaderBase
{
public:
	ShaderBase() :
		m_Size(0),
		m_Data(nullptr)
	{}

	virtual ~ShaderBase()
	{
		if (m_Data != nullptr)
		{
			delete[] m_Data;
		}
	}

	/**
	* @brief シェーダ作成関数
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	* @details 指定されたファイルからシェーダを作成する
	* @details 継承先では各シェーダを作成するように実装する
	* @return 作成結果 成功(true)
	*/
	virtual bool Create(ID3D11Device* device, const char* file_name) = 0;

	/**
	* @brief Shaderデータ(バイナリ)のゲッター
	*/
	const char* GetData()
	{
		return m_Data;
	}

	/**
	* @brief Shaderのデータサイズのゲッター
	*/
	long GetSize()
	{
		return m_Size;
	}
protected:

	/**
	* @brief シェーダ読み込み関数
	* @param[in] file_name 読み込み対象のファイル名
	* @details 指定されたファイルを読み込む
	* @details ファイルはcso(コンパイル済み)を対象としているのでhlslファイルを読み込むとエラーとして扱う
	* @return 読み込み結果 成功(true)
	*/
	bool Load(const char* file_name);

protected:
	char* m_Data;	//!シェーダデータ(バイナリ)
	long m_Size;	//!データのサイズ
};

#endif
