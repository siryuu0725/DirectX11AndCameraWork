#include "FbxController.h"

FbxController* FbxController::mp_instance = nullptr;

FbxController* FbxController::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new FbxController(); }

	return mp_instance;
}

//メッシュデータセット関数
void FbxController::LoadFbxMesh(std::string key_, const char* pFilename_, VertexShaderType type_)
{
	m_MeshData[key_] = new FbxMeshFile();

	m_MeshData[key_]->Load(pFilename_, DirectGraphics::Instance()->GetDevice(), ShaderManager::Instance()->GetVertexShader(type_));
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
void FbxController::ReleaseModel(std::string key_)
{
	delete m_MeshData[key_];
	m_MeshData[key_] = nullptr;
}

//Instance解放関数(Debug用)
void FbxController::ReleaseInstance()
{
	delete mp_instance;
	mp_instance = nullptr;
}