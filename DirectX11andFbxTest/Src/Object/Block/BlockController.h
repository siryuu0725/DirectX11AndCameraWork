/**
 * @file BlockController.h
 * @brief ステージに配置されているブロックオブジェクトを管理している
 * @details　中身：ブロック管理クラス
**/
#ifndef BLOCKCONTROLLER_H_
#define BLOCKCONTROLLER_H_
#include "CircleBlock.h"
#include "RectBlock.h"
#include <vector>

/**
 * ブロック管理クラス
 */
class BlockController :public ObjectBase
{
public:
	BlockController() {}
	~BlockController();

	/**
	* @brief  初期化関数
	* @details 各ブロックのメンバ変数初期化
	*/
	virtual void Init()override;


	/**
    * @brief  描画関数
    */
	virtual void Draw()override;

	/**
    * @brief   影描画関数
    */
	virtual void ShadowDraw()override;

	/**
 　 * @brief 当たり判定用情報Set関数
	* @details ObjectCollisionで使用するオブジェクトコピー情報にオリジナルの情報を渡す
 　 */
	void SetCollisionInfo();

	/**
 　 * @brief 円形ブロック情報Getter
	* @param[out] copy_info_ 保存用オブジェクト構造体
	* @param[in] id_ 受け取りたいブロックのid
	* @details 引数に円形ブロックの情報を渡す
 　 */
	const void GetCircleBlockInfo(CircleBlock::ObjectInfo& copy_info_, int id_);

	/**
　　* @brief 矩形ブロック情報Getter
	* @param[out] copy_info_ 保存用オブジェクト構造体
	* @param[in] id_ 受け取りたいブロックのid
	* @details 引数に矩形ブロックの情報を渡す
　　*/
	const void GetRectBlockInfo(RectBlock::ObjectInfo& copy_info_, int id_);

private:
	std::vector<CircleBlock*> m_circleblocks;  //!円形ブロック
	std::vector<RectBlock*> m_rectblocks;	   //!矩形ブロック
};


#endif