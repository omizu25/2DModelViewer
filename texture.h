//**************************************************
// 
// Hackathon ( texture.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "main.h"

//==================================================
// 列挙型
//==================================================
enum ETexture
{
	TEXTURE_icon_122380_256 = 0,	// 仮画像1
	TEXTURE_icon_122540_256,		// 仮画像2
	TEXTURE_MAX,
	TEXTURE_NONE,	// 使用しない
};

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void LoadTextureAll(void);

//--------------------------------------------------
// 読み込み
// 引数  : ETexture inTexture / 列挙型 種類
//--------------------------------------------------
void LoadTexture(ETexture inTexture);

//--------------------------------------------------
// 全ての終了
//--------------------------------------------------
void UnloadTextureAll(void);

//--------------------------------------------------
// 終了
// 引数  : ETexture inTexture / 列挙型 種類
//--------------------------------------------------
void UnloadTexture(ETexture inTexture);

//--------------------------------------------------
// 取得
// 引数  : char* inFileName / 文字列 ファイル名
// 返値  : ETexture / テクスチャの種類
//--------------------------------------------------
ETexture GetFileNameTexture(char* inFileName);

//--------------------------------------------------
// 取得
// 引数  : ETexture inTexture / 列挙型 種類
// 返値  : LPDIRECT3DTEXTURE9 / テクスチャ
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(ETexture inTexture);

#endif // !_TEXTURE_H_
