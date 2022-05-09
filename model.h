//**************************************************
//
// Hackathon ( model.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _MODEL_H_	// このマクロ定義がされてなかったら
#define _MODEL_H_	// ２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "texture.h"

//==================================================
// 構造体
//==================================================

/*↓ キー ↓*/

struct SKey
{
	D3DXVECTOR3	pos;	// 位置
	D3DXVECTOR3	rot;	// 向き
};

/*↓ キー設定 ↓*/

struct SKeySet
{
	int		nFrame;	// フレーム数
	SKey*	pKey;	// キー
};

/*↓ モーション設定 ↓*/

struct SMotion
{
	bool		bLoop;		// ループするかどうか
	int			nNumKey;	// キー数
	SKeySet*	pKeySet;	// キー設定
};

/*↓ パーツ ↓*/

struct SParts
{
	D3DXVECTOR3	pos;			// 位置
	D3DXVECTOR3	posOld;			// 前回の位置
	D3DXVECTOR3	posSet;			// 設定の位置
	D3DXVECTOR3	rot;			// 向き
	D3DXVECTOR3	rotOld;			// 前回の向き
	D3DXVECTOR3	rotSet;			// 設定の向き
	D3DXVECTOR3	size;			// サイズ
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	ETexture	texture;		// テクスチャ
	int			idxParent;		// 親の番号
	int			idxPriority;	// 描画の優先順位
};

/*↓ モデル↓*/

struct SModel
{
	D3DXVECTOR3	pos;		// 位置
	D3DXVECTOR3	posOld;		// 前回の位置
	D3DXVECTOR3	rot;		// 向き
	D3DXVECTOR3	rotDest;	// 目的の向き
	D3DXVECTOR3	move;		// 移動量
	D3DXMATRIX	mtxWorld;	// ワールドマトリックス
	SParts*		pParts;		// パーツの情報
	int			numParts;	// パーツ数
	float		moveLoad;	// 読み込んだ移動量
};

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitModel(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateModel(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawModel(void);

#endif // !_MODEL_H_
