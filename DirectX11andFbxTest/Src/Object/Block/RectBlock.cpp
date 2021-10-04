#include "RectBlock.h"
#include "../../System/Fbx/FbxController.h"

//コンストラクタ
RectBlock::RectBlock(Vector3 pos_, Vector3 scale_, Vector3 angle_, Vector3 size_, std::string key_)
{
	m_rectblock_info.m_key = key_;      //描画用キー

	m_rectblock_info.m_pos = pos_;      //座標
	m_rectblock_info.m_scale = scale_;  //描画サイズ
	m_rectblock_info.m_degree = angle_; //回転角度

	m_rectblock_info.m_size = size_;    //立方体の幅

	//ワールド座標
	m_rectblock_info.world_matrix = Calculation::Matrix(m_rectblock_info.m_pos, m_rectblock_info.m_scale, m_rectblock_info.m_degree);
}

//描画関数
void RectBlock::Draw()
{
	FbxController::Instance()->DrawFbx(m_rectblock_info.m_key, m_rectblock_info.world_matrix, DrawType::MainDraw);
}

//影描画関数
void RectBlock::ShadowDraw()
{
	FbxController::Instance()->DrawFbx(m_rectblock_info.m_key, m_rectblock_info.world_matrix, DrawType::ShadowDraw);
}