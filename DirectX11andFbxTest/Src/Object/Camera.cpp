#include "Camera.h"
#include "../System/DirectInput.h"
#include "../System/Window.h"
#include "../Utility/Calculation.h"
#include "../Utility/Collision/ObjectCollision.h"

//初期化関数
void Camera::Init()
{
	m_camerainfo.world_matrix= DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	m_camerainfo.m_pos = Vector3(0.0f, 40.0f, -50.0f);     //カメラ座標
	m_camerainfo.m_camera_up = Vector3(0.0f, 1.0f, 0.0f);  //上向きベクトル
	m_camerainfo.m_forward = Vector3(0.0f, 15.0f, 0.0f);   //注視点

	//XMVECTORに変換
	m_camerainfo.m_eye_vec = DirectX::XMVectorSet(m_camerainfo.m_pos.x, m_camerainfo.m_pos.y, m_camerainfo.m_pos.z, 0.0f); //注視点
	m_camerainfo.m_camera_up_vec = DirectX::XMVectorSet(m_camerainfo.m_camera_up.x, m_camerainfo.m_camera_up.y, m_camerainfo.m_camera_up.z, 0.0f); //上向きベクトル
	m_camerainfo.m_forward_vec = DirectX::XMVectorSet(m_camerainfo.m_forward.x, m_camerainfo.m_forward.y, m_camerainfo.m_forward.z, 0.0f);    //X軸回転用

	m_camerainfo.mat_view = DirectX::XMMatrixIdentity();

	//任意の軸回転用ベクトル
	m_camerainfo.m_zik_vec = m_camerainfo.m_forward - m_camerainfo.m_pos; 
	m_camerainfo.m_zik_vec = Vector3(m_camerainfo.m_zik_vec.z, 0.0f, m_camerainfo.m_zik_vec.x);
	Calculation::ThreeNormalization(m_camerainfo.m_zik_vec);

	m_camerainfo.m_rote_speed = 2.0f;                 //回転スピード
	m_camerainfo.m_rote_angle = m_camerainfo.m_pos.y; //現在の回転角度

	m_camerainfo.m_leap_vec = m_camerainfo.m_pos;       //線形補間ベクトル

	m_camerainfo.m_collision_pos = m_camerainfo.m_pos; //当たり判定用座標
	m_camerainfo.m_intersection1 = Vector3(0.0f, 0.0f, 0.0f);
	m_camerainfo.m_intersection2 = Vector3(0.0f, 0.0f, 0.0f);

	m_camerainfo.m_direction = m_camerainfo.m_forward - m_camerainfo.m_pos; //方法ベクトル
	Calculation::ThreeNormalization(m_camerainfo.m_direction);

	CreateFrustum(); //視錐台関数
}

//視錐台関数
void Camera::CreateFrustum()
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	// プロジェクションマトリクス設定
	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = (float)(rect.right - rect.left) / (rect.bottom - rect.top);
	float    nearZ = 0.1f;
	float    farZ = 20000000.0f;
	DirectX::XMMATRIX proj_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

	// コンスタントバッファの設定
	DirectX::XMStoreFloat4x4(&DirectGraphics::Instance()->GetConstantBufferData()->Projection, XMMatrixTranspose(proj_matrix));
}

//更新関数
void Camera::Update()
{
	//移動
	Move();

	//回転
	RotEyePos();

	//地面に埋まった状態の座標ではなく押し出した後の座標から当たり判定を取る
	m_camerainfo.m_collision_pos = m_camerainfo.m_pos + m_camerainfo.m_floor_intersection;
	m_camerainfo.m_direction = m_camerainfo.m_forward - m_camerainfo.m_pos;

	//第三者に情報送信
	ObjectCollision::Instance()->SetCameraInfo(m_camerainfo);

	//当たり判定
	HitRectBlock();
	HitCircleBlock();

	//線形補間
	Vector3 end = m_camerainfo.m_pos + m_camerainfo.m_floor_intersection;
	m_camerainfo.m_leap_vec = Calculation::Lerp(m_camerainfo.m_leap_vec, end, CameraRoteTime);

	m_camerainfo.m_eye_vec = DirectX::XMVectorSet(m_camerainfo.m_leap_vec.x , m_camerainfo.m_leap_vec.y, m_camerainfo.m_leap_vec.z, 0.0f); //注視点
	m_camerainfo.m_camera_up_vec = DirectX::XMVectorSet(m_camerainfo.m_camera_up.x, m_camerainfo.m_camera_up.y, m_camerainfo.m_camera_up.z, 0.0f); //上向きベクトル
	m_camerainfo.m_forward_vec = DirectX::XMVectorSet(m_camerainfo.m_forward.x, m_camerainfo.m_forward.y, m_camerainfo.m_forward.z, 0.0f);    //X軸回転用

	//ビュー座標変換用の行列算出 start
	m_camerainfo.mat_view = DirectX::XMMatrixLookAtLH(m_camerainfo.m_eye_vec, m_camerainfo.m_forward_vec, m_camerainfo.m_camera_up_vec);

	DirectX::XMStoreFloat4x4(&DirectGraphics::Instance()->GetConstantBufferData()->View, DirectX::XMMatrixTranspose(m_camerainfo.mat_view));
	DirectX::XMStoreFloat4(&DirectGraphics::Instance()->GetConstantBufferData()->CameraPos, m_camerainfo.m_eye_vec);
}

//移動関数
void Camera::Move()
{
#pragma region Debug用
	//左移動
	//if (Inputter::Instance()->GetKey(Inputter::AKey))
	//{
	//	m_camerainfo.m_eye_pos.x += 3.0f;
	//	m_camerainfo.m_forward.x += 3.0f;
	//}
	////右移動
	//if (Inputter::Instance()->GetKey(Inputter::DKey))
	//{
	//	m_camerainfo.m_eye_pos.x -= 3.0f;
	//	m_camerainfo.m_forward.x -= 3.0f;
	//}
	////前移動
	//if (Inputter::Instance()->GetKey(Inputter::WKey))
	//{
	//	m_camerainfo.m_eye_pos.z -= 3.0f;
	//	m_camerainfo.m_forward.z -= 3.0f;
	//}
	////後ろ移動
	//if (Inputter::Instance()->GetKey(Inputter::SKey))
	//{
	//	m_camerainfo.m_eye_pos.z += 3.0f;
	//	m_camerainfo.m_forward.z += 3.0f;
	//}
#pragma endregion

	//プレイヤーの移動量を加算
	m_camerainfo.m_pos += m_camerainfo.m_move_vec;
	m_camerainfo.m_forward += m_camerainfo.m_move_vec;
}

//矩形ブロック当たり判定関数
void Camera::HitRectBlock()
{
	if (ObjectCollision::Instance()->HitRectBlock() == true)
	{
		ObjectInfo rectblock_info;
		mp_block->GetRectBlockInfo(rectblock_info, ObjectCollision::Instance()->GetRectBlockID());

		Vector3 nor[6];  //各面の方向ベクトル
		Vector3 vertex_pos[6]; //各面の中点座標

		//各面の方向ベクトル
		nor[0] = Vector3(1.0f, 0.0f, 0.0f);   //右面
		nor[1] = Vector3(-1.0f, 0.0f, 0.0f);  //左面
		nor[2] = Vector3(0.0f, 1.0f, 0.0f);	  //上面
		nor[3] = Vector3(0.0f, -1.0f, 0.0f);  //下面
		nor[4] = Vector3(0.0f, 0.0f, 1.0f);	  //前面
		nor[5] = Vector3(0.0f, 0.0f, -1.0f);  //後面

		//各面の中点座標
		vertex_pos[0] = Vector3(rectblock_info.m_pos.x + (rectblock_info.m_size.x / 2), rectblock_info.m_pos.y, rectblock_info.m_pos.z);
		vertex_pos[1] = Vector3(rectblock_info.m_pos.x - (rectblock_info.m_size.x / 2), rectblock_info.m_pos.y, rectblock_info.m_pos.z);
		vertex_pos[2] = Vector3(rectblock_info.m_pos.x, rectblock_info.m_pos.y + (rectblock_info.m_size.y / 2), rectblock_info.m_pos.z);
		vertex_pos[3] = Vector3(rectblock_info.m_pos.x, rectblock_info.m_pos.y - (rectblock_info.m_size.y / 2), rectblock_info.m_pos.z);
		vertex_pos[4] = Vector3(rectblock_info.m_pos.x, rectblock_info.m_pos.y, rectblock_info.m_pos.z + (rectblock_info.m_size.z / 2));
		vertex_pos[5] = Vector3(rectblock_info.m_pos.x, rectblock_info.m_pos.y, rectblock_info.m_pos.z - (rectblock_info.m_size.z / 2));

		std::vector<__int8> vertex_no;
		Vector3 start_vec;
		Vector3 end_vec;

		const __int8 cueb_surface = 6;

		//どの面にベクトルが貫通しているか探す
		for (int i = 0; i < cueb_surface; i++)
		{
			start_vec = m_camerainfo.m_collision_pos - vertex_pos[i];   //カメラから中点ベクトル
			end_vec = m_camerainfo.m_forward - vertex_pos[i]; //カメラの注視点から中点ベクトル

			float start_dot = Calculation::Dot(start_vec, nor[i]);
			float end_dot = Calculation::Dot(end_vec, nor[i]);

			//2つのベクトルの内積の積が0以下だった場合=面を貫通している
			if (start_dot * end_dot <= 0.0f)
			{
				vertex_no.push_back(i);
			}
		}


		//貫通している面が2つある時
		if (vertex_no.size() > 1)
		{
			float vec_length = Calculation::Length(m_camerainfo.m_forward - vertex_pos[vertex_no[0]]);
			float vec_length2 = Calculation::Length(m_camerainfo.m_forward - vertex_pos[vertex_no[1]]);

			//注視点により近い面の要素番号を使うためもう一つを削除
			if (vec_length2 <= vec_length)
			{
				vertex_no.erase(vertex_no.begin());
			}
			else
			{
				vertex_no.erase(vertex_no.begin() + 1);
			}
		}
		
		//面を貫通している時
		if (vertex_no.size() > 0)
		{
			//貫通している面からカメラまでの距離
			start_vec = m_camerainfo.m_collision_pos - vertex_pos[vertex_no[0]];
			start_vec.x *= nor[vertex_no[0]].x;
			start_vec.y *= nor[vertex_no[0]].y;
			start_vec.z *= nor[vertex_no[0]].z;

			//貫通している面から注視点までの距離
			end_vec = m_camerainfo.m_forward - vertex_pos[vertex_no[0]];
			end_vec.x *= nor[vertex_no[0]].x;
			end_vec.y *= nor[vertex_no[0]].y;
			end_vec.z *= nor[vertex_no[0]].z;


			//カメラから貫通点と貫通点から注視点までの比率
			float hiritu = abs(Calculation::Length(end_vec)) / (abs(Calculation::Length(end_vec)) + abs(Calculation::Length(start_vec)));

			Vector3 out;
			Vector3 Vec = m_camerainfo.m_collision_pos - m_camerainfo.m_forward;

			out.x = m_camerainfo.m_forward.x + (Vec.x * hiritu);
			out.y = m_camerainfo.m_forward.y + (Vec.y * hiritu);
			out.z = m_camerainfo.m_forward.z + (Vec.z * hiritu);

			//最終的な移動量算出
			m_camerainfo.m_floor_intersection += out - m_camerainfo.m_collision_pos;
		}
	}
}

//円形ブロック当たり判定関数
void Camera::HitCircleBlock()
{
	if (ObjectCollision::Instance()->HitCircleBlock() == true)
	{
		ObjectInfo circleblock_info;
		mp_block->GetCircleBlockInfo(circleblock_info, ObjectCollision::Instance()->GetCircleBlockID());

		Vector3 cilinder_start = circleblock_info.m_pos;
		cilinder_start.y += circleblock_info.m_height / 2;

		Vector3 cilinder_end = circleblock_info.m_pos;
		cilinder_end.y -= circleblock_info.m_height / 2;

		Vector3 p = cilinder_start - m_camerainfo.m_collision_pos;  //レイの始点から円柱軸の1点までのベクトル
		cilinder_end = cilinder_end - m_camerainfo.m_collision_pos; //レイの始点から円柱軸のもう1点までのベクトル

		Vector3 s = cilinder_end - p; //円柱軸のベクトル

		Calculation::ThreeNormalization(m_camerainfo.m_direction);

		// 各種内積値
		float Dvv = m_camerainfo.m_direction.x * m_camerainfo.m_direction.x + m_camerainfo.m_direction.y * m_camerainfo.m_direction.y + m_camerainfo.m_direction.z * m_camerainfo.m_direction.z;
		float Dsv = s.x * m_camerainfo.m_direction.x + s.y * m_camerainfo.m_direction.y + s.z * m_camerainfo.m_direction.z; 
		float Dpv = p.x * m_camerainfo.m_direction.x + p.y * m_camerainfo.m_direction.y + p.z * m_camerainfo.m_direction.z; 
		float Dss = s.x * s.x + s.y * s.y + s.z * s.z; 
		float Dps = p.x * s.x + p.y * s.y + p.z * s.z; 
		float Dpp = p.x * p.x + p.y * p.y + p.z * p.z; 
		float rr = circleblock_info.m_radius * circleblock_info.m_radius;

		// 円柱が定義されない
		if (Dss != 0.0f)
		{
			float A = Dvv - Dsv * Dsv / Dss;
			float B = Dpv - Dps * Dsv / Dss;
			float C = Dpp - Dps * Dps / Dss - rr;

			if (A != 0.0f)
			{
				float s_ = B * B - A * C;

				if (s_ > 0.0f)
				{
					s_ = sqrtf(s_);

					float a1 = (B - s_) / A;
					float a2 = (B + s_) / A;

					m_camerainfo.m_intersection1 = m_camerainfo.m_collision_pos + (m_camerainfo.m_direction * a1);
					m_camerainfo.m_intersection2 = m_camerainfo.m_collision_pos + (m_camerainfo.m_direction * a2);

					float vec_length = Calculation::Length(m_camerainfo.m_forward - m_camerainfo.m_intersection1);
					float vec_length2 = Calculation::Length(m_camerainfo.m_forward - m_camerainfo.m_intersection2);

					//注視点により近い面の要素番号を使うためもう一つを削除
					if (vec_length <= vec_length2)
					{
						//最終的な移動量算出
						m_camerainfo.m_floor_intersection += m_camerainfo.m_intersection1 - m_camerainfo.m_collision_pos;
					}
					else
					{
						//最終的な移動量算出
						m_camerainfo.m_floor_intersection += m_camerainfo.m_intersection2 - m_camerainfo.m_collision_pos;
					}
				}
			}
		}
	}
}

//カメラ回転関数
void Camera::RotEyePos()
{
	m_camerainfo.m_floor_intersection = Vector3(0.0f, 0.0f, 0.0f);

	//左回転
	if (Inputter::Instance()->GetKey(Inputter::JKey))
	{
		//回転角度設定
		m_camerainfo.m_yaw = m_camerainfo.m_rote_speed;

		//座標回転
		m_camerainfo.m_pos = Calculation::Rotate(m_camerainfo.m_pos, m_camerainfo.m_player_pos, Radian(m_camerainfo.m_yaw));
		//任意の軸回転
		m_camerainfo.m_zik_vec = Calculation::Rotate(m_camerainfo.m_zik_vec, Vector3(0.0f, 0.0f, 0.0f), Radian(m_camerainfo.m_yaw));
	}
	//左回転
	else if (Inputter::Instance()->GetKey(Inputter::LKey))
	{
		//回転角度設定
		m_camerainfo.m_yaw = -m_camerainfo.m_rote_speed;
		//座標回転
		m_camerainfo.m_pos = Calculation::Rotate(m_camerainfo.m_pos, m_camerainfo.m_player_pos, Radian(m_camerainfo.m_yaw));
		//任意の軸回転
		m_camerainfo.m_zik_vec = Calculation::Rotate(m_camerainfo.m_zik_vec, Vector3(0.0f, 0.0f, 0.0f), Radian(m_camerainfo.m_yaw));
	}
	//上回転
	else if (Inputter::Instance()->GetKey(Inputter::IKey))
	{
		//回転角度設定
		m_camerainfo.m_yaw = m_camerainfo.m_rote_speed;
		//現在の回転角度(上下)
		m_camerainfo.m_rote_angle += m_camerainfo.m_yaw;

		Calculation::ThreeNormalization(m_camerainfo.m_zik_vec);

		if (m_camerainfo.m_rote_angle <= MaxCameraRote_Y)
		{
            #pragma region 座標回転(Quaternionによる回転方法)
			Quaternion a = Calculation::AngleAxis(-m_camerainfo.m_yaw, m_camerainfo.m_zik_vec);
			//カメラ座標を一度原点基準に戻す
			m_camerainfo.m_pos -= m_camerainfo.m_player_pos;
			//回転
			m_camerainfo.m_pos = a * m_camerainfo.m_pos;
			//プレイヤーの移動地を加算
			m_camerainfo.m_pos += m_camerainfo.m_player_pos;
            #pragma endregion

            #pragma region 座標回転(ロドリゲスの回転行列による回転方法)
			//m_camerainfo.m_pos = Calculation::RodriguesRotation(m_camerainfo.m_pos, m_camerainfo.m_player_pos, m_camerainfo.m_zik_vec, Radian(m_camerainfo.m_yaw));
            #pragma endregion
		}
		else
		{
			m_camerainfo.m_rote_angle = MaxCameraRote_Y;
		}
	}
	//下回転
	else if (Inputter::Instance()->GetKey(Inputter::KKey))
	{
		//回転角度設定
		m_camerainfo.m_yaw = m_camerainfo.m_rote_speed;
		//現在の回転角度(上下)
		m_camerainfo.m_rote_angle -= m_camerainfo.m_yaw;

		//m_camerainfo.m_zik_vec += m_camerainfo.m_player_pos;

		Calculation::ThreeNormalization(m_camerainfo.m_zik_vec);

		//座標回転
		if (m_camerainfo.m_rote_angle >= -MaxCameraRote_Y)
		{
            #pragma region 座標回転(Quaternionによる回転方法)
			Quaternion b = Calculation::AngleAxis(m_camerainfo.m_yaw, m_camerainfo.m_zik_vec);
			//カメラ座標を一度原点基準に戻す
			m_camerainfo.m_pos -= m_camerainfo.m_player_pos;
			//回転
			m_camerainfo.m_pos = b * m_camerainfo.m_pos;
			//プレイヤーの移動地を加算
			m_camerainfo.m_pos += m_camerainfo.m_player_pos;
            #pragma endregion

            #pragma region 座標回転(ロドリゲスの回転行列による回転方法)
			//m_camerainfo.m_pos = Calculation::RodriguesRotation(m_camerainfo.m_pos, m_camerainfo.m_player_pos, m_camerainfo.m_zik_vec, Radian(-m_camerainfo.m_yaw));
            #pragma endregion
		}
		else
		{
			m_camerainfo.m_rote_angle = -MaxCameraRote_Y;
		}
	}

	//地面に埋まっているとき
	if (m_camerainfo.m_pos.y < MinCameraPos_Y)
	{
		Difference();
	}
}

//めり込み差分算出関数
void Camera::Difference()
{
	//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB
	float hiritu = abs(m_camerainfo.m_forward.y) / (abs(m_camerainfo.m_forward.y) + abs(m_camerainfo.m_pos.y));

	Vector3 out;
	Vector3 Vec = m_camerainfo.m_pos - m_camerainfo.m_forward;

	//交点算出
	out.x = m_camerainfo.m_forward.x + (Vec.x * hiritu);
	out.y = m_camerainfo.m_forward.y + (Vec.y * hiritu) + CameraUpPos;
	out.z = m_camerainfo.m_forward.z + (Vec.z * hiritu);

	//最終押し出す量
	m_camerainfo.m_floor_intersection = out - m_camerainfo.m_pos;
}
