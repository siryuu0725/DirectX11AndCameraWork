#include "BlockController.h"
#include "../../Utility/Collision/ObjectCollision.h"

//デストラクタ
BlockController::~BlockController()
{
	//円形ブロック
	for (__int8 i = 0; i < m_circleblocks.size(); i++)
	{
		delete m_circleblocks[i];
		m_circleblocks[i] = nullptr;
	}
	
	//矩形ブロック
	for (__int8 i = 0; i < m_rectblocks.size(); i++)
	{
		delete m_rectblocks[i];
		m_rectblocks[i] = nullptr;
	}
}

//初期化関数
void BlockController::Init()
{
	ObjectBase::ObjectInfo block_info;

	//円形ブロック
	m_circleblocks.push_back(new CircleBlock
	(Vector3(100.0f, 0.0f, -30.0f),
	Vector3(5.0f, 100.0f, 5.0f),
	17.0f, 50.0f, "circleblock"));

	//矩形ブロック
	m_rectblocks.push_back(new RectBlock
	(Vector3(10.0f, 12.0f, -30.0f),
	 Vector3(5.0f, 5.0f, 5.0f),
	 Vector3(0.0f, 0.0f, 0.0f),
	 Vector3(29.0f, 29.0f, 29.0f),
	 "rectblock"));

	SetCollisionInfo();
}

//描画情報送信関数
void BlockController::Draw()
{
	//円形ブロック
	for (__int8 i = 0; i < m_circleblocks.size(); i++)
	{
		m_circleblocks[i]->Draw();
	}
	
	//矩形ブロック
	for (__int8 i = 0; i < m_rectblocks.size(); i++)
	{
		m_rectblocks[i]->Draw();
	}
}

void BlockController::ShadowDraw()
{
	//円形ブロック
	for (__int8 i = 0; i < m_circleblocks.size(); i++)
	{
		m_circleblocks[i]->ShadowDraw();
	}

	//矩形ブロック
	for (__int8 i = 0; i < m_rectblocks.size(); i++)
	{
		m_rectblocks[i]->ShadowDraw();
	}
}

//FBXモデル解放関数
//void BlockController::ReleaseModel()
//{
//	//円形ブロック
//	for (__int8 i = 0; i < m_circleblocks.size(); i++)
//	{
//		m_circleblocks[i]->ReleaseModel();
//	}
//
//	//矩形ブロック
//	for (__int8 i = 0; i < m_rectblocks.size(); i++)
//	{
//		m_rectblocks[i]->ReleaseModel();
//	}
//}

void BlockController::SetCollisionInfo()
{
	//円形ブロック
	for (__int8 i = 0; i < m_circleblocks.size(); i++)
	{
		CircleBlock::ObjectInfo circleblock_info;

		m_circleblocks[i]->GetCircleBlockInfo(circleblock_info);

		ObjectCollision::Instance()->SetCircleBlockInfo(circleblock_info);
	}

	//矩形ブロック
	for (__int8 i = 0; i < m_rectblocks.size(); i++)
	{
		RectBlock::ObjectInfo rectblock_info;

		m_rectblocks[i]->GetRectBlockInfo(rectblock_info);

		ObjectCollision::Instance()->SetRectBlockInfo(rectblock_info);
	}
}

//円形ブロック情報Getter
const void BlockController::GetCircleBlockInfo(CircleBlock::ObjectInfo& copy_info_, int id_)
{
	m_circleblocks[id_]->GetCircleBlockInfo(copy_info_);
}

//矩形ブロック情報Getter
const void BlockController::GetRectBlockInfo(RectBlock::ObjectInfo& copy_info_, int id_)
{
	m_rectblocks[id_]->GetRectBlockInfo(copy_info_);
}
