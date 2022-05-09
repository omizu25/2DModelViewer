//**************************************************
// 
// Hackathon ( rectangle.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "rectangle.h"
#include "color.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_RECTANGLE = 1024;	// 矩形の最大数
const int	NUM_VERTEX = 4;			// 頂点の数
const int	NUM_POLYGON = 2;		// ポリゴンの数

struct SRectangle
{
	LPDIRECT3DTEXTURE9		pTexture;	// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファ
	bool					use;		// 使用しているかどうか
	bool					draw;		// 描画するかどうか
	bool					add;		// 加算合成するかどうか
};
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
SRectangle	s_rectangle[MAX_RECTANGLE];	// 矩形の情報
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectangle(void)
{
	// メモリのクリア
	memset(s_rectangle, 0, sizeof(s_rectangle));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle(void)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		SRectangle *pRectangle = &s_rectangle[i];

		if (pRectangle->pVtxBuff != NULL)
		{// 頂点バッファの解放
			pRectangle->pVtxBuff->Release();
			pRectangle->pVtxBuff = NULL;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		SRectangle *pRectangle = &s_rectangle[i];

		if (!pRectangle->use || !pRectangle->draw)
		{// 使用していない、描画するしない
			continue;
		}

		/*↓ 使用している、描画する ↓*/

		if (pRectangle->add)
		{// 加算合成する
			// レンダーステートの設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pRectangle->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pRectangle->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			NUM_POLYGON);				// プリミティブ(ポリゴン)数

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

		if (pRectangle->add)
		{// 加算合成する
			// レンダーステートを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetRectangle(ETexture inTexture)
{
	// 設定 [ テクスチャあり ]
	return SetRectangleWithTex(GetTexture(inTexture));
}

//--------------------------------------------------
// 設定 [ テクスチャあり ]
//--------------------------------------------------
int SetRectangleWithTex(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		SRectangle *pRectangle = &s_rectangle[i];

		if (pRectangle->use)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pRectangle->pTexture = pTexture;
		pRectangle->use = true;
		pRectangle->draw = true;
		pRectangle->add = false;

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_2D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pRectangle->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		pRectangle->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点バッファをアンロックする
		pRectangle->pVtxBuff->Unlock();

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 位置の設定
		SetPosRectangle(i, pos, size);

		//色の設定
		SetColorRectangle(i, GetColor(COLOR_WHITE));

		D3DXVECTOR2 texU = D3DXVECTOR2(0.0f, 1.0f);
		D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

		// テクスチャ座標の設定
		SetTexRectangle(i, texU, texV);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 使用をやめる
//--------------------------------------------------
void StopUseRectangle(int inIdx)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	pRectangle->use = false;

	if (pRectangle->pVtxBuff != NULL)
	{// 頂点バッファの解放
		pRectangle->pVtxBuff->Release();
		pRectangle->pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void SetPosRectangle(int inIdx, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inSize)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(inIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = inSize.x * 0.5f;
	float fHeight = inSize.y * 0.5f;

	// 頂点座標の設定
	pVtx[0].pos = inPos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = inPos + D3DXVECTOR3(+fWidth, -fHeight, 0.0f);
	pVtx[2].pos = inPos + D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
	pVtx[3].pos = inPos + D3DXVECTOR3(+fWidth, +fHeight, 0.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 回転する位置の設定
//--------------------------------------------------
void SetRotationPosRectangle(int inIdx, const D3DXVECTOR3& inPos, float inRot, float inWidth, float inHeight)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(inIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXMATRIX mtx, mtxTrans;

	// 回転の反映
	D3DXMatrixRotationZ(&mtx, -inRot);

	// 位置の反映
	D3DXMatrixTranslation(&mtxTrans, inPos.x, inPos.y, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	D3DXVECTOR3 pVtxpos[NUM_VERTEX];

	float fHalfWidth = inWidth * 0.5f;
	float fHalfHeight = inHeight * 0.5f;

	pVtxpos[0] = D3DXVECTOR3(-fHalfWidth, -fHalfHeight, 0.0f);
	pVtxpos[1] = D3DXVECTOR3(+fHalfWidth, -fHalfHeight, 0.0f);
	pVtxpos[2] = D3DXVECTOR3(-fHalfWidth, +fHalfHeight, 0.0f);
	pVtxpos[3] = D3DXVECTOR3(+fHalfWidth, +fHalfHeight, 0.0f);
	
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		D3DXVec3TransformCoord(&pVtx[i].pos, &pVtxpos[i], &mtx);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void SetColorRectangle(int inIdx, const D3DXCOLOR& inColor)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(inIdx);

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
void SetTexRectangle(int inIdx, const D3DXVECTOR2& inTexU, const D3DXVECTOR2& inTexV)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(inIdx);

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
void SetDrawRectangle(int inIdx, bool inDraw)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle->draw = inDraw;
}

//--------------------------------------------------
// 加算合成するかどうか
//--------------------------------------------------
void SetAddRectangle(int inIdx, bool inAdd)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle->add = inAdd;
}

//--------------------------------------------------
// テクスチャの変更
//--------------------------------------------------
void ChangeTextureRectangle(int inIdx, ETexture inTexture)
{
	// テクスチャの変更 [ テクスチャあり ]
	ChangeTextureRectangleWithTex(inIdx, GetTexture(inTexture));
}

//--------------------------------------------------
// テクスチャの変更 [ テクスチャあり ]
//--------------------------------------------------
void ChangeTextureRectangleWithTex(int inIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle->pTexture = pTexture;
}

//--------------------------------------------------
// 頂点バッファを取得
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int inIdx)
{
	assert(inIdx >= 0 && inIdx < MAX_RECTANGLE);

	SRectangle *pRectangle = &s_rectangle[inIdx];

	if (!pRectangle->use)
	{// 使用していない
		return NULL;
	}

	/*↓ 使用している ↓*/

	return s_rectangle[inIdx].pVtxBuff;
}
