#include "ObjectCollision.h"

ObjectCollision* ObjectCollision::mp_instance = nullptr;

//コンストラクタ
ObjectCollision::ObjectCollision() :
	m_hit_circleblock_id(0), m_hit_rectblock_id(0)
{
}

ObjectCollision* ObjectCollision::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new ObjectCollision; }

	return mp_instance;
}

//矩形ブロック当たり判定関数
bool ObjectCollision::HitRectBlock()
{
	//個数分回す
	for (__int8 i = 0; i < m_rectblock_copy.size(); i++)
	{
		if (Collision::BoundingSphereAndPoint(m_rectblock_copy[i].m_pos, m_camera_copy.m_collision_pos, m_rectblock_copy[i].m_size) == true)
		{
			m_hit_rectblock_id = i;
			return true;
		}
	}

	return false;
}

//円形ブロック当たり判定関数
bool ObjectCollision::HitCircleBlock()
{
	//個数分回す
	for (__int8 i = 0; i < m_circleblock_copy.size(); i++)
	{
		if (Collision::VectorAndCircle(m_camera_copy.m_collision_pos,
			m_camera_copy.m_direction + m_camera_copy.m_pos, 
			m_circleblock_copy[i].m_pos,
			m_circleblock_copy[i].m_radius) == true)
		{
			m_hit_circleblock_id = i;
			return true;
		}
	}

	return false;
}

//各オブジェクト情報リセット関数
void ObjectCollision::ResetObjectInfo()
{
	m_rectblock_copy.clear();
	m_circleblock_copy.clear();
}

//Instance解放関数(Debug用)
void ObjectCollision::ReleaseInstance()
{
	delete mp_instance;
	mp_instance = nullptr;
}

