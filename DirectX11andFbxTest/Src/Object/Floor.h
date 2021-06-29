﻿/**
 * @file Floor.h
 * @brief 床オブジェクト
 * @details　中身：床クラス
**/

#ifndef FLOOR_H_
#define FLOOR_H_
#include "ObjectBase.h"
#include <DirectXTex.h>

class Floor :public ObjectBase
{
public:
	Floor() {}
	~Floor() {}

	/**
	* @brief  初期化関数
	* @details オブジェクト情報初期化
	*/
	virtual void Init()override;

	/**
	* @brief   描画関数
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

private:
	ObjectInfo floor_info;
};

#endif