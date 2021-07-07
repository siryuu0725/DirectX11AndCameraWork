/**
 * @file VertexShader.h
 * @brief VertexShaderの作成を行う
**/
#ifndef VERTEX_SHADER_H_
#define VERTEX_SHADER_H_

#include "ShaderBase.h"

class VertexShader : public ShaderBase
{
public:
	VertexShader() :
		m_Interface(nullptr)
	{}

	virtual ~VertexShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief VertexShader作成関数
	* @details 指定されたファイルからVertexShaderを作成する
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char* file_name);

	/**
	* @brief ShaderInterfaceのゲッター
	*/
	inline ID3D11VertexShader* GetShaderInterface() { return m_Interface; }

private:
	ID3D11VertexShader* m_Interface;		//!VertexShaderInterface
};

#endif
