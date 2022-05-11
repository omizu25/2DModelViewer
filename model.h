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
// 定義
//==================================================

enum EType
{
	TYPE_NEUTRAL = 0,	// ニュートラル
	TYPE_MOVE,			// 移動
	TYPE_MAX,
	TYPE_NONE
};

/*↓ キー ↓*/

struct SKey
{
	D3DXVECTOR3	pos;	// 位置
	float		rot;	// 向き
};

/*↓ キー設定 ↓*/

struct SKeySet
{
	int		frame;	// フレーム数
	SKey*	pKey;	// キー
};

/*↓ モーション設定 ↓*/

struct SMotion
{
	bool		loop;		// ループするかどうか
	int			numKey;		// キー数
	SKeySet*	pKeySet;	// キー設定
};

/*↓ パーツ ↓*/

struct SParts
{
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				posOld;			// 前回の位置
	D3DXVECTOR3				savePos;		// 位置を保存
	D3DXMATRIX				mtxWorld;		// マトリックス
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;		// 頂点バッファ
	ETexture				texture;		// テクスチャ
	int						idxRectangle;	// 矩形の番号
	int						idxParent;		// 親の番号
	int						idxPriority;	// 描画の優先順位
	float					rot;			// 向き
	float					rotOld;			// 前回の向き
	float					saveRot;		// 向きの保存
	float					width;			// 幅
	float					height;			// 高さ
};

/*↓ モデル↓*/

struct SModel
{
	D3DXVECTOR3	pos;				// 位置
	D3DXVECTOR3	posOld;				// 前回の位置
	D3DXVECTOR3	move;				// 移動量
	D3DXMATRIX	mtxWorld;			// マトリックス
	SParts*		pParts;				// パーツの情報
	SMotion		motion[TYPE_MAX];	// モーションの情報
	int			numParts;			// パーツ数
	float		moveLoad;			// 読み込んだ移動量
	float		rot;				// 向き
	float		rotDest;			// 目的の向き
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
