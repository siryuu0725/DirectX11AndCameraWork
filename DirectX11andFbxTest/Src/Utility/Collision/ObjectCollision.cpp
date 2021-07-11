#include "ObjectCollision.h"

ObjectCollision* ObjectCollision::mp_instance = nullptr;

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
		//ブロックオブジェクトを上から見たときに囲っている円(BaundingCircle)と
		//カメラからのレイとの当たり判定を取る方法
		if (Collision::VectorAndCircle(m_camera_copy.m_collision_pos,
			m_camera_copy.m_direction + m_camera_copy.m_pos,
			m_rectblock_copy[i].m_pos,
			Calculation::Length(m_rectblock_copy[i].m_size / 2)) == true)
		{
			m_hit_circleblock_id = i;
			return true;
		}

		//BaundingSphereとカメラ座標で当たり判定を取る方法
		/*if (Collision::BoundingSphereAndPoint(m_rectblock_copy[i].m_pos, m_camera_copy.m_collision_pos, m_rectblock_copy[i].m_size) == true)
		{
			m_hit_rectblock_id = i;
			return true;
		}*/
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


//Instance解放関数(Debug用)
void ObjectCollision::ReleaseInstance()
{
	delete mp_instance;
}

