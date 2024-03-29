//**************************************************
//
// Hackathon ( color.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _COLOR_H_	// このマクロ定義がされてなかったら
#define _COLOR_H_	// ２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "main.h"

//==================================================
// 列挙型
//==================================================
enum EColor
{
	COLOR_RED = 0,		// 赤
	COLOR_GREEN,		// 緑
	COLOR_BLUE,			// 青
	COLOR_YELLOW,		// 黄色
	COLOR_PURPLE,		// 紫
	COLOR_LIGHTBLUE,	// 水色
	COLOR_WHITE,		// 白
	COLOR_GRAY,			// 灰色
	COLOR_BLACK,		// 黒
	COLOR_MAX,
};

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 取得
// 引数  : EColor inColor / 列挙型 種類
// 返値  : D3DXCOLOR / 色
//--------------------------------------------------
D3DXCOLOR GetColor(EColor inColor);

#endif // !_COLOR_H_
