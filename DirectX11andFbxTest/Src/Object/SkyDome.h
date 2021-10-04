/**
 * @file SkyDome.h
 * @brief 空オブジェクト
 * @details　中身：空クラス
**/

#ifndef SKYDOME_H_
#define SKYDOME_H_
#include "ObjectBase.h"

class SkyDome :public ObjectBase
{
public:
	SkyDome() {}
	~SkyDome() {}

	/**
	* @brief  初期化関数
	* @details オブジェクト情報初期化
	*/
	virtual void Init()override;

	/**
	* @brief   描画関数
	*/
	virtual void Draw()override;

private:
	ObjectInfo skydome_info; //!空情報
	FbxController* fbx_insttance = FbxController::Instance();

};

#endif