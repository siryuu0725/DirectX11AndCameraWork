/**
 * @file UICategory.h
 * @brief 各シーンで使用するテクスチャの種類を宣言する
**/
#ifndef UICATEGORY_H_
#define UICATEGORY_H_

//!UIの種類
enum class TitleUICategory :__int8
{
	BG,              //!背景
	CategoryMax,     //!UI数
};

//!UIの種類
enum class LoadUICategory :__int8
{
	RotTex,     //!回転テクスチャ
	NowLoad,    //!文字「NowLoad」
	CategoryMax,
};

//!UIの種類
enum class ResultUICategory :__int8
{
	BG,
	CategoryMax,
};
#endif