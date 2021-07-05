#include "CircleBlock.h"
#include "../../System/DirectInput.h"
#include "../../Utility/Collision/ObjectCollision.h"

//コンストラクタ
CircleBlock::CircleBlock(Vector3 pos_, Vector3 scale_, float radius_, float height_, std::string key_)
{
	m_circleblock_info.m_key = key_;        //描画用キー

	m_circleblock_info.m_pos = pos_;        //座標
	m_circleblock_info.m_radius = radius_;  //半径
	m_circleblock_info.m_height = height_;  //半径

	m_circleblock_info.m_scale = scale_;    //描画サイズ
	m_circleblock_info.m_degree = Vector3(0.0f, 0.0f, 0.0f);  //回転角度

	//ワールド座標
	m_circleblock_info.world_matrix = Calculation::Matrix(m_circleblock_info.m_pos, m_circleblock_info.m_scale, m_circleblock_info.m_degree);

	//FbxController::Instance()->LoadFbxMesh(m_circleblock_info.m_key, "Res/FBX/CircleBlock.fbx", VertexShaderType::StaticVertex);
}

//描画関数
void CircleBlock::Draw()
{
	FbxController::Instance()->DrawFbx(m_circleblock_info.m_key, m_circleblock_info.world_matrix, DrawType::MainDraw);
}

//影描画関数
void CircleBlock::ShadowDraw()
{
	FbxController::Instance()->DrawFbx(m_circleblock_info.m_key, m_circleblock_info.world_matrix, DrawType::ShadowDraw);
}