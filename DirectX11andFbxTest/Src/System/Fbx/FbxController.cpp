#include "FbxController.h"

FbxController* FbxController::mp_instance = nullptr;

FbxController* FbxController::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new FbxController(); }

	return mp_instance;
}

//メッシュデータ読み込みまとめ関数
void FbxController::LoadFbx()
{
	LoadFbxMesh("rectblock", "Res/FBX/RectBlock.fbx", VertexShaderType::StaticVertex);
	LoadFbxMesh("circleblock", "Res/FBX/CircleBlock.fbx", VertexShaderType::StaticVertex);
	LoadFbxMesh("Player", "Res/FBX/unitychan_ch_change.fbx", VertexShaderType::SkinVertex);
	LoadFbxMesh("Floor", "Res/FBX/SkyFloor.fbx", VertexShaderType::StaticVertex);
	LoadFbxMesh("SkyDome", "Res/FBX/sky_dome.fbx", VertexShaderType::StaticVertex);

}

//アニメーションファイル読み込み関数
void FbxController::LoadAnimation()
{

	LoadAnimation("Player", "Run", "Res/FBX/unitychan_RUN00_F.FBX");
	LoadAnimation("Player", "Stay", "Res/FBX/unitychan_WAIT00.FBX");

	SetAnimationName("Player", "Run");
}

//Fbfファイル描画関数
void FbxController::DrawFbx(std::string key_, DirectX::XMMATRIX world_matrix_, DrawType draw_type_)
{
	if (&m_MeshData[key_] == nullptr)return;

	m_MeshData[key_]->Render(world_matrix_, draw_type_);
}

//アニメーションファイル読み込み関数
void FbxController::LoadAnimation(std::string key_, std::string anim_name_, const char* fail_name_)
{

	if (&m_MeshData[key_] == nullptr)return;

	m_MeshData[key_]->LoadMotion(anim_name_, fail_name_);

}

//メッシュデータセット関数
void FbxController::LoadFbxMesh(std::string key_, const char* pFilename_, VertexShaderType type_)
{
	m_MeshData[key_] = new FbxMeshFile();

	m_MeshData[key_]->Load(pFilename_, DirectGraphics::Instance()->GetDevice(), ShaderManager::Instance()->GetVertexShader(type_));
}


//アニメーション指定関数
void FbxController::SetAnimationName(std::string key_, std::string anim_name_)
{
	if (&m_MeshData[key_] == nullptr)return;

	m_MeshData[key_]->Play(anim_name_);
}

//アニメーション関数
void FbxController::PlayAnimation(std::string key_)
{
	if (&m_MeshData[key_] == nullptr)return;

	m_MeshData[key_]->Animate();
}

//モデル解放関数
void FbxController::ReleaseModel()
{
	for (auto mesh_data : m_MeshData)
	{
		mesh_data.second->Release();
		delete mesh_data.second;
	}
}

//Instance解放関数(Debug用)
void FbxController::ReleaseInstance()
{
	delete mp_instance;
}