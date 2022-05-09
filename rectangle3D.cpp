//**************************************************
// 
// Hackathon ( rectangle3D.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "rectangle3D.h"
#include "color.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_RECTANGLE3D = 1024;	// 矩形の最大数
const int	NUM_VERTEX = 4;			// 頂点の数
const int	NUM_POLYGON = 2;		// ポリゴンの数

typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;	// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファ
	D3DXMATRIX				mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3				pos;		// 位置
	float					rot;		// 向き
	bool					use;		// 使用しているかどうか
	bool					draw;		// 描画するかどうか
	bool					add;		// 加算合成するかどうか
}MyRectangle3D;
}// namespaceはここまで

 //==================================================
 // スタティック変数
 //==================================================
namespace
{
MyRectangle3D	s_rectangle3D[MAX_RECTANGLE3D];	// 矩形の情報
}// namespaceはここまで

 //--------------------------------------------------
 // 初期化
 //--------------------------------------------------
void InitRectangle3D(void)
{
	// メモリのクリア
	memset(s_rectangle3D, 0, sizeof(s_rectangle3D));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle3D(void)
{
	for (int i = 0; i < MAX_RECTANGLE3D; i++)
	{
		MyRectangle3D *pRectangle3D = &s_rectangle3D[i];

		if (pRectangle3D->pVtxBuff != NULL)
		{// 頂点バッファの解放
			pRectangle3D->pVtxBuff->Release();
			pRectangle3D->pVtxBuff = NULL;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle3D(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// 計算用マトリックス

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < MAX_RECTANGLE3D; i++)
	{
		MyRectangle3D *pRectangle3D = &s_rectangle3D[i];

		if (!pRectangle3D->use || !pRectangle3D->draw)
		{// 使用していない、描画するしない
			continue;
		}

		/*↓ 使用している、描画する ↓*/

		if (pRectangle3D->add)
		{// 加算合成する
			// レンダーステートの設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pRectangle3D->pVtxBuff, 0, sizeof(VERTEX_3D));

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pRectangle3D->mtxWorld);

		// Zの向きを反映
		D3DXMatrixRotationZ(&mtxRot, pRectangle3D->rot);
		D3DXMatrixMultiply(&pRectangle3D->mtxWorld, &pRectangle3D->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pRectangle3D->pos.x, pRectangle3D->pos.y, pRectangle3D->pos.z);
		D3DXMatrixMultiply(&pRectangle3D->mtxWorld, &pRectangle3D->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pRectangle3D->mtxWorld);

		// テクスチャの設定
		pDevice->SetTexture(0, pRectangle3D->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			0,						// 描画する最初の頂点インデックス
			NUM_POLYGON);			// プリミティブ(ポリゴン)数

		if (pRectangle3D->add)
		{// 加算合成する
			// レンダーステートを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetRectangle3D(ETexture inTexture)
{
	// 設定 [ テクスチャあり ]
	return SetRectangle3DWithTex(GetTexture(inTexture));
}

//--------------------------------------------------
// 設定 [ テクスチャあり ]
//--------------------------------------------------
int SetRectangle3DWithTex(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i < MAX_RECTANGLE3D; i++)
	{
		MyRectangle3D *pRectangle3D = &s_rectangle3D[i];

		if (pRectangle3D->use)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pRectangle3D->pTexture = pTexture;
		pRectangle3D->use = true;
		pRectangle3D->draw = true;
		pRectangle3D->add = false;
		pRectangle3D->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pRectangle3D->rot = 0.0f;

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pRectangle3D->pVtxBuff,
			NULL);

		VERTEX_3D *pVtx = NULL;	// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		pRectangle3D->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点バッファをアンロックする
		pRectangle3D->pVtxBuff->Unlock();

		D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 位置の設定
		SetSizeRectangle3D(i, size);

		//色の設定
		SetColorRectangle3D(i, GetColor(COLOR_WHITE));

		D3DXVECTOR2 texU = D3DXVECTOR2(0.0f, 1.0f);
		D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

		// テクスチャ座標の設定
		SetTexRectangle3D(i, texU, texV);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 使用をやめる
//--------------------------------------------------
void StopUseRectangle3D(int inIdx)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	pRectangle3D->use = false;

	if (pRectangle3D->pVtxBuff != NULL)
	{// 頂点バッファの解放
		pRectangle3D->pVtxBuff->Release();
		pRectangle3D->pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void SetPosRectangle3D(int inIdx, const D3DXVECTOR3& inPos)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle3D->pos = inPos;
}

//--------------------------------------------------
// 向きの設定
//--------------------------------------------------
void SetRotRectangle3D(int inIdx, float inRot)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle3D->rot = inRot;
}

//--------------------------------------------------
// サイズの設定
//--------------------------------------------------
void SetSizeRectangle3D(int inIdx, const D3DXVECTOR3& inSize)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(inIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	float fWidth = inSize.x * 0.5f;
	float fHeight = inSize.y * 0.5f;
	float fDepth = inSize.z * 0.5f;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fWidth, +fHeight, +fDepth);
	pVtx[1].pos = D3DXVECTOR3(+fWidth, +fHeight, +fDepth);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, -fDepth);
	pVtx[3].pos = D3DXVECTOR3(+fWidth, -fHeight, -fDepth);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void SetColorRectangle3D(int inIdx, const D3DXCOLOR& inColor)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(inIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = inColor;
	pVtx[1].col = inColor;
	pVtx[2].col = inColor;
	pVtx[3].col = inColor;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// テクスチャ座標の設定
//--------------------------------------------------
void SetTexRectangle3D(int inIdx, const D3DXVECTOR2& inTexU, const D3DXVECTOR2& inTexV)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(inIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(inTexU.x, inTexV.x);
	pVtx[1].tex = D3DXVECTOR2(inTexU.y, inTexV.x);
	pVtx[2].tex = D3DXVECTOR2(inTexU.x, inTexV.y);
	pVtx[3].tex = D3DXVECTOR2(inTexU.y, inTexV.y);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawRectangle3D(int inIdx, bool inDraw)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle3D->draw = inDraw;
}

//--------------------------------------------------
// 加算合成するかどうか
//--------------------------------------------------
void SetAddRectangle3D(int inIdx, bool inAdd)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle3D->add = inAdd;
}

//--------------------------------------------------
// テクスチャの変更
//--------------------------------------------------
void ChangeTextureRectangle3D(int inIdx, ETexture inTexture)
{
	// テクスチャの変更 [ テクスチャあり ]
	ChangeTextureRectangle3DWithTex(inIdx, GetTexture(inTexture));
}

//--------------------------------------------------
// テクスチャの変更 [ テクスチャあり ]
//--------------------------------------------------
void ChangeTextureRectangle3DWithTex(int inIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle3D->pTexture = pTexture;
}

//--------------------------------------------------
// 頂点バッファを取得
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle3D(int inIdx)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE3D);

	MyRectangle3D *pRectangle3D = &s_rectangle3D[inIdx];

	if (!pRectangle3D->use)
	{// 使用していない
		return NULL;
	}

	/*↓ 使用している ↓*/

	return s_rectangle3D[inIdx].pVtxBuff;
}
