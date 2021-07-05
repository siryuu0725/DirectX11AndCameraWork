/**
 * @file FbxManager.h
 * @brief FBXファイルの読み込み描画等を管理する
 * @details 中身：FBX管理クラス
**/
#ifndef FBXCONTROLLER_H_
#define FBXCONTROLLER_H_
#include "FbxMeshFile.h"
#include "../ShaderSrc/ShaderManager.h"

/**
* FBX管理クラス
*/
class FbxController
{
public:
	static FbxController* Instance();

	FbxController() {}
	~FbxController() {}

	/**
	 * @brief  メッシュデータ読み込みまとめ関数
	 * @details 各オブジェクトのLoadFbxMesh関数をまとめたもの
	 */
	void LoadFbx();

	/**
	* @brief  Fbfファイル描画関数
	* @param[in] key_ オブジェクトのキー
	* @param[in] mat_world_ オブジェクトのワールド座標
　　* @param[in] draw_type_ 描画タイプ(影かメインか)
	* @details 各オブジェクトのLoadFbxMesh関数をまとめたもの
	*/
	void DrawFbx(std::string key_, DirectX::XMMATRIX world_matrix_, DrawType draw_type_);

	/**
	* @brief  アニメーションファイル読み込み関数
	* @details アニメーションを読み込み
	*/
	void LoadAnimation();

	/**
	* @brief  アニメーション指定関数
	* @details 実行したいアニメーションの設定
	* @param[in] key_ オブジェクトのキー
	* @param[in] anim_name_ アニメーションの名前
	*/
	void SetAnimationName(std::string key_, std::string anim_name_);

	/**
	* @brief  アニメーション関数
	* @param[in] key_ オブジェクトのキー
	* @details アニメーションを行う(フレーム数を動かす)
	*/
	void PlayAnimation(std::string key_);

	/**
	* @brief  モデル解放関数
	* @param[in] key_ オブジェクトのキー
	* @details 指定したモデルを解放する
	*/
	void ReleaseModel();

	/**
    * @brief Instance解放関数(Debug用)
    * @details 手動でインスタンスを解放する
    */
	void ReleaseInstance();

public:
	/**
	* @brief  Meshデータセット関数
	* @param[in] key_ オブジェクトのキー
	* @param[in] pFilename_ 読み込むFbxファイルのパス
	* @details 引数のキーのメッシュデータに引数のパスのFbxデータを代入する
	*/
	void LoadFbxMesh(std::string key_, const char* pFilename_, VertexShaderType type_);

	/**
	* @brief  アニメーションファイル読み込み関数
	* @param[in] key_ オブジェクトのキー
	* @param[in] anim_name_ アニメーションの名前
	* @details 指定したキーにアニメーションを読み込み
	*/
	void LoadAnimation(std::string key_, std::string anim_name_, const char* fail_name_);

private:
	static FbxController* mp_instance;

	std::map<std::string, FbxMeshFile*> m_MeshData;

};


#endif
