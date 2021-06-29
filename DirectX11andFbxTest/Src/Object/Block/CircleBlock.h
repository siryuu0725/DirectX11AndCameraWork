/**
 * @file CircleBlock.h
 * @brief 円柱型(上から見て円形)のブロックオブジェクトを扱う
 * @details　中身：円形ブロッククラス
**/
#ifndef CIRCLEBLOCK_H_
#define CIRCLEBLOCK_H_
#include "../ObjectBase.h"

/**
* 円形ブロッククラス
*/
class CircleBlock :public ObjectBase
{
public:
	/**
 　 * @brief  コンストラクタ
 　 * @param[in] pos_ 座標
 　 * @param[in] scale_ サイズ
 　 * @param[in] radius_ 半径
 　 * @param[in] key_ 描画用キー
 　 */
	CircleBlock(Vector3 pos_, Vector3 scale_, float radius_, float height_, std::string key_);

	~CircleBlock() {}

	/**
	* @brief  描画関数
	*/
	virtual void Draw()override;

	/**
    * @brief   影描画関数
    */
	virtual void ShadowDraw()override;

	/**
    * @brief   FBXモデル解放関数
    */
	virtual void ReleaseModel()override;

	/**
 　 * @brief  オブジェクト情報Getter
    * @param[out] copy_info_ 保存用オブジェクト構造体
	* @details 引数にオブジェクトの情報を渡す
  　*/
	const void GetCircleBlockInfo(ObjectInfo& copy_info_) { copy_info_ = m_circleblock_info; }

private:
	ObjectInfo m_circleblock_info;  //!円型ブロック情報
};

#endif
