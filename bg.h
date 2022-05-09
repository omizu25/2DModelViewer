//**************************************************
//
// Hackathon ( bg.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _BG_H_	// このマクロ定義がされてなかったら
#define _BG_H_	// ２重インクルード防止のマクロ定義

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBG(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBG(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBG(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBG(void);

#endif // !_BG_H_
