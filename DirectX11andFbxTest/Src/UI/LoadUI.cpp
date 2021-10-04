#include "LoadUI.h"

//初期化関数
void LoadUI::Init()
{
	m_ui_info.m_pos[(__int8)LoadUICategory::NowLoad] = Vector3(1000.0f, 900.0f, 0.0f);
	m_ui_info.m_scale[(__int8)LoadUICategory::NowLoad] = Vector3(1.0f, 1.0f, 1.0f);
	m_ui_info.m_rot[(__int8)LoadUICategory::NowLoad] = Vector3(0.0f, 0.0f, 0.0f);

	m_ui_info.m_pos[(__int8)LoadUICategory::RotTex] = Vector3(1780.0f, 980.0f, 0.0f);
	m_ui_info.m_scale[(__int8)LoadUICategory::RotTex] = Vector3(1.0f, 1.0f, 1.0f);
	m_ui_info.m_rot[(__int8)LoadUICategory::RotTex] = Vector3(0.0f, 0.0f, 0.0f);
}

//描画情報送信関数
void LoadUI::Draw()
{
	TextureManager::Instance()->Draw("NowLoading",
		m_ui_info.m_pos[(__int8)LoadUICategory::NowLoad],
		m_ui_info.m_scale[(__int8)LoadUICategory::NowLoad],
		m_ui_info.m_rot[(__int8)LoadUICategory::NowLoad]);

	TextureManager::Instance()->Draw("LoadingIcon",
		m_ui_info.m_pos[(__int8)LoadUICategory::RotTex],
		m_ui_info.m_scale[(__int8)LoadUICategory::RotTex],
		m_ui_info.m_rot[(__int8)LoadUICategory::RotTex]);
}

//更新関数
void LoadUI::Update()
{
	m_ui_info.m_rot[(__int8)LoadUICategory::RotTex].z += IconRoteSpeed;
}

