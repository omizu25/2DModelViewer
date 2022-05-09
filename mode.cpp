//**************************************************
//
// Hackathon ( mode.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "fade.h"
#include "game.h"
#include "mode.h"
#include "rectangle.h"
#include "texture.h"
#include "utility.h"

#include <assert.h>
#include <time.h>

//==================================================
// スタティック変数
//==================================================
namespace
{
EMode	s_mode = MODE_NONE;		// 現在のモード
EMode	s_modeNext = MODE_NONE;	// 次のモード
void(*s_pUpdateFunc)(void);
void(*s_pDrawFunc)(void);
}// namesapceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void UpdateNone(void);
void DrawNone(void);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMode(void)
{
	// randの初期化
	srand((unsigned int)time(NULL));

	// 矩形の初期化
	InitRectangle();

	s_pUpdateFunc = UpdateNone;
	s_pDrawFunc = DrawNone;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMode(void)
{
	// ゲームの終了
	UninitGame();

	// 矩形の終了
	UninitRectangle();

	// テクスチャの解放
	UnloadTextureAll();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMode(void)
{
	s_pUpdateFunc();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMode(void)
{
	s_pDrawFunc();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// 次のモードが決まってない
		return;
	}

	if (GetFade() == FADE_NONE)
	{// 何もしていない状態なら
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// フェードイン状態じゃない
		return;
	}

	switch (s_mode)
	{// 現在のモードの終了
	case MODE_TITLE:	// タイトル
		break;

	case MODE_GAME:		// ゲーム
		UninitGame();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}

	// テクスチャの解放
	UnloadTextureAll();

	// 矩形の終了
	UninitRectangle();

	// 矩形の初期化
	InitRectangle();

	s_mode = s_modeNext;	// 現在の画面(モード)を切り替える
	
	switch (s_modeNext)
	{// 次のモードの初期化
	case MODE_TITLE:	// タイトル
		break;

	case MODE_GAME:		// ゲーム
		s_pUpdateFunc = UpdateGame;
		s_pDrawFunc = DrawGame;
		InitGame();
		break;
	
	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
EMode GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// 変更
//--------------------------------------------------
void ChangeMode(EMode inModeNext)
{
	assert(inModeNext >= 0 && inModeNext < MODE_MAX);

	s_modeNext = inModeNext;
}

namespace
{
//--------------------------------------------------
// 更新をしない
//--------------------------------------------------
void UpdateNone(void)
{
}

//--------------------------------------------------
// 描画をしない
//--------------------------------------------------
void DrawNone(void)
{
}
}// namespaceはここまで