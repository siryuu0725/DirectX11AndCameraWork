#include "ResultUI.h"

//初期化関数
void ResultUI::Init()
{
	m_ui_info.m_pos[(__int8)ResultUICategory::BG] = Vector3(0.0f, 0.0f, 0.0f);
	m_ui_info.m_scale[(__int8)ResultUICategory::BG] = Vector3(1.0f, 1.0f, 1.0f);
	m_ui_info.m_rot[(__int8)ResultUICategory::BG] = Vector3(0.0f, 0.0f, 0.0f);
}

//描画情報送信関数
void ResultUI::Draw()
{
	TextureManager* tex_instance = TextureManager::Instance();

	tex_instance->Draw("EndBG",
		m_ui_info.m_pos[(__int8)ResultUICategory::BG],
		m_ui_info.m_scale[(__int8)ResultUICategory::BG],
		m_ui_info.m_rot[(__int8)ResultUICategory::BG]);
}
