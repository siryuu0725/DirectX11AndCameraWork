#include "VertexShader.h"

//シェーダ作成関数
bool VertexShader::Create(ID3D11Device* device, const char* file_name)
{
	// ファイル読み込み
	if (Load(file_name) == false)
	{
		return false;
	}

	// VertexShader作成
	if (FAILED(device->CreateVertexShader(
		m_Data,					// 生成元となるデータ
		m_Size,					// データのサイズ
		/*
			ID3D11ClassLinkageのポインタを設定
				ID3D11ClassLinkageはシェーダの実行関数を
				動的に切り替えることができる
				それによって、条件分岐をシェーダ内に書かなくてもすむので、
				シェーダの速度低下を抑えることができる。
				今回は使用しないのでnullptr
		*/
		nullptr,
		&m_Interface)))			// 作成後のInterface格納先
	{
		return false;
	}

	return true;
}
