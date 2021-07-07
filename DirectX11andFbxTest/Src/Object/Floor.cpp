#include "Floor.h"
#include "../Utility/Calculation.h"

//初期化関数
void Floor::Init()
{
	floor_info.m_pos = Vector3(0.0f, 0.0f, 0.0f);       //座標 
	floor_info.m_degree = Vector3(0.0f, 0.0f, 0.0f);    //描画サイズ
	floor_info.m_scale = Vector3(10.0f, 10.0f, 10.0f);  //回転角度

	floor_info.world_matrix = Calculation::Matrix(floor_info.m_pos, floor_info.m_scale, floor_info.m_degree);

	floor_info.m_key = "Floor"; //描画用キー
}

//描画関数
void Floor::Draw()
{
	FbxController::Instance()->DrawFbx(floor_info.m_key, floor_info.world_matrix, DrawType::MainDraw);
}

//影描画関数
void Floor::ShadowDraw()
{
	FbxController::Instance()->DrawFbx(floor_info.m_key, floor_info.world_matrix, DrawType::ShadowDraw);
}