/**
 * @file PixelShader.h
 * @brief PixelShaderの作成を行う
**/
#ifndef PIXEL_SHADER_H_
#define PIXEL_SHADER_H_

#include "ShaderBase.h"

class PixelShader : public ShaderBase
{
public:
	PixelShader() :
		m_Interface(nullptr)
	{}

	virtual ~PixelShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief PixelShader作成関数
	* @details 指定されたファイルからPixelShaderを作成する
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char* file_name);

	/** PixelShaderInterfaceのゲッター */
	inline ID3D11PixelShader* GetShaderInterface() { return m_Interface; }
private:
	ID3D11PixelShader* m_Interface;			//!< @brief PixelShaderInterface
};

#endif
