//**************************************************
//
// Hackathon ( fade.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _FADE_H_	// このマクロ定義がされてなかったら
#define _FADE_H_	// ２重インクルード防止のマクロ定義

//==================================================
// 列挙型
//==================================================
enum EFade
{
	FADE_NONE = 0,	// 何もしていない状態
	FADE_OUT,		// フェードアウト(見えない)
	FADE_IN,		// フェードイン(見える)
	FADE_MAX
};

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitFade(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitFade(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateFade(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawFade(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void StartFadeOut(void);

//--------------------------------------------------
// 取得
// 返値  : EFade / 列挙型 状態
//--------------------------------------------------
EFade GetFade(void);

#endif // !_FADE_H_
