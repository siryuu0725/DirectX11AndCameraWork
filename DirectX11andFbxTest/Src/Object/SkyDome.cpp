#include "SkyDome.h"
#include "../Utility/Calculation.h"

//初期化関数
void SkyDome::Init()
{
	skydome_info.m_pos = Vector3(0.0f, 0.0f, 0.0f);     //座標
	skydome_info.m_degree = Vector3(0.0f, 0.0f, 0.0f);	//描画サイズ
	skydome_info.m_scale = Vector3(1.0f, 1.0f, 1.0f);	//回転角度

	skydome_info.world_matrix = Calculation::Matrix(skydome_info.m_pos, skydome_info.m_scale, skydome_info.m_degree);
	
	skydome_info.m_key = "SkyDome"; //描画用キー

}

//描画関数
void SkyDome::Draw()
{
	fbx_insttance->DrawFbx(skydome_info.m_key, skydome_info.world_matrix, DrawType::MainDraw);
}