/**
 * @file FbxMeshFile.h
 * @brief FBXファイルの読み込み描画等を行う
 * @details 中身：FBXクラス
**/
#ifndef FBX_MESH_FILE_H_
#define FBX_MESH_FILE_H_
#include <fbxsdk.h>
#include <unordered_map>
#include <vector>
#include "../DirectGraphics.h"

static const int MOTION_MAX = 256;
static const int BONE_MAX = 255;

// ボーン
struct Bone
{
	char			  Name[64];
	DirectX::XMMATRIX Offset;
	DirectX::XMMATRIX Transform;
};

struct Motion
{
	
	UINT FrameNum; //!フレーム数
	std::vector<DirectX::XMMATRIX> Key[BONE_MAX]; //!キーフレーム
};

class FbxMeshFile
{
public:
	FbxMeshFile();
	~FbxMeshFile();

	/**
	* @brief モデル読み込み関数
	* @param[in] file_name ファイルパス
	* @param[in] device デバイス
	* @param[in] vertex_shader モデルと関連付けるVertexShader
	* @details モデルデータを読み込む
	* @return 読み込みの成否 成功(true)
	*/
	bool Load(const char* file_name, ID3D11Device* device, VertexShader* vertex_shader);

	/**
	* @brief モデル描画関数
	* @param[in] world_matrix_ ワールド行列
	* @param[in] draw_type_ 描画タイプ(影かメインか)
	* @details 指定された情報でモデルを描画する
	* @return 描画の成否 成功(true)
	*/
	void Render(DirectX::XMMATRIX world_matrix_, DrawType draw_type_);

	/**
	* @brief モーション読み込み関数
	* @param[in] keyword_ モーション名
	* @param[in] fileName_ ファイルパス
	* @details 指定されたキーで読み込んだモーションを登録する
	* @return 読み込みの成否 成功(true)
	*/
	bool LoadMotion(std::string keyword_, const char* fileName_);

	/**
	* @brief モーション指定関数
	* @param[in] animName_ モーション名
	* @details 指定された名前のモーションをsetする
	*/
	void Play(std::string animName_);

	/**
	* @brief アニメーション関数
	* @details 指定されたモーションを動かす
	*/
	void Animate();

	/**
	* @brief 解放関数
	*/
	void Release();
private:
	struct MeshData
	{
		ID3D11Buffer* m_VertexBuffer;			//!頂点バッファ(Shader送信用)
		ID3D11Buffer* m_IndexBuffer;			//!インデックスバッファ(Shader送信用)
		std::vector<CustomVertex> m_Vertices;   //!頂点情報
		std::vector<UINT> m_Indices;            //!インデックス情報
		std::string m_MaterialName;             //!マテリアル名
		__int16 m_ParentBoneId;                 //!親子関係ID
	};

private:
	/**
	* @brief メッシュ生成関数
	* @param[in] file_name ファイル名
	* @details ファイルからメッシュを生成する
	* @return 作成の成否 成功(true)
	*/
	bool LoadFbxFail(const char* file_name);

	/**
	* @brief VertexBuffer作成関数
	* @param[in] device DirectX11グラフィクスデバイス
	* @details VertexBufferを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateVertexBuffer(ID3D11Device* device);

	/**
	* @brief IndexBuffer作成関数
	* @param[in] device DirectX11グラフィクスデバイス
	* @details IndexBufferを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateIndexBuffer(ID3D11Device* device);

	/**
	* @brief 入力レイアウト作成関数
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
    * @details 入力レイアウトを作成する
	* @return 作成の成否 成功(true)
	*/
	bool CreateInputLayout(ID3D11Device* device, VertexShader* vertex_shader);

	/**
	* @brief メッシュデータ検索関数
	* @param[in] node_ Meshを作るためのNodeデータ
	* @details ノードを辿ってメッシュデータを探す
	* @return 検索の成否 成功(true) 
	*/
	bool IsMesh(FbxNode* node_);

	/**
	* @brief ノード探索関数
	* @param[in] node_ Meshを作るためのNodeデータ
	* @param[in] non_skin_list スキンが無いリスト保存用
	* @details ノード探索法でメッシュデータを作成する
	*/
	void ProbeNode(FbxNode* node_, std::unordered_map<int, std::string>& non_skin_list_);

	/**
	* @brief メッシュデータ作成関数
	* @param[in] mesh Meshを作るためのNodeのMeshデータ
	* @param[in] non_skin_list スキンが無いリスト保存用
	* @details メッシュデータを作成する
	*/
	void CreateMesh(FbxMesh* mesh, std::unordered_map<int, std::string>& non_skin_list);


	/**
	* @brief 頂点インデックスデータ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details 頂点インデックスデータを読み込む
	*/
	void LoadIndices(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief 頂点データ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details 頂点データを読み込む
	*/
	void LoadVertices(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief 法線データ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details 法線データを読み込む
	*/
	void LoadNormals(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief 頂点カラーデータ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details 頂点カラーデータを読み込む
	*/
	void LoadColors(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief UVデータ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details UVデータを読み込む
	*/
	void LoadUV(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief マテリアルデータ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details マテリアルデータを読み込む
	*/
	void LoadMaterial(FbxSurfaceMaterial* material);

	/**
	* @brief テクスチャデータ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details テクスチャデータを読み込む
	*/
	bool LoadTexture(FbxFileTexture* material, std::string& keyword);

	/**
	* @brief マテリアル名読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details マテリアル名を読み込む
	*/
	void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief マテリアルデータ保存関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details マテリアルデータを保存する
	*/
	void SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material);
	
	/**
	* @brief ボーンデータ読み込み関数
	* @param[out] mesh_data バッファ保存用
	* @param[in] mesh バッファ取得をするMeshデータ
	* @details ボーンデータを読み込む
	*/
	void LoadBones(MeshData& meshData_, FbxMesh* mesh_);

	/**
	* @brief ボーン検索関数
	* @param[out] boneName_ ボーン名
	* @details 指定したボーン名に含まれているかどうか調べる
	* @return ボーンID
	*/
	int FindBone(const char* boneName_);

	/**
	* @brief キーフレーム読み込み関数
	* @details キーフレーム読み込む
	* @param[out] keyword_ モーション名
	* @param[in] bone 指定ボーン
	* @param[in] boneNode_ ボーン保存ノード
	*/
	void LoadKeyFrame(std::string keyword_, int bone, FbxNode* boneNode_);

private:
	std::vector<MeshData> m_MeshList;                                  //!メッシュデータ
	std::unordered_map<std::string, ObjMaterial> m_Materials;          //!マテリアル保存
	ID3D11InputLayout* m_InputLayout;								   //!入力レイアウト
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Textures;       //!テクスチャ情報保存
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_MaterialLinks;  //!テクスチャとマテリアルの関係

	std::unordered_map<std::string, Motion>	 m_Motion; //!モーション保存
	Bone m_Bone[BONE_MAX];        //!ボーン保存
	std::string m_PlayMotion;     //!モーション名
	UINT m_BoneNum;   //!ボーン数

	float m_StartFrame; //!初期フレーム
	float m_Frame;	    //!現在のフレーム
};

#endif