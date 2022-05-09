//**************************************************
// 
// Hackathon ( rectangle.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _RECTANGLE_H_	//このマクロ定義がされてなかったら
#define _RECTANGLE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "texture.h"

//==================================================
// 定義
//==================================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[2D]

/*↓ 頂点情報[2D] ↓*/

struct VERTEX_2D
{
	D3DXVECTOR3	pos;	// 頂点座標
	float		rhw;	// 座標変換用係数 (1.0fで固定)
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2	tex;	// テクスチャ座標
};

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectangle(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle(void);

//--------------------------------------------------
// 設定
// 引数  : TEXTURE inTexture / 列挙型 種類
//--------------------------------------------------
int SetRectangle(TEXTURE inTexture);

//--------------------------------------------------
// 設定 [ テクスチャあり ]
// 引数  : LPDIRECT3DTEXTURE9 pTexture / テクスチャ
//--------------------------------------------------
int SetRectangleWithTex(LPDIRECT3DTEXTURE9 pTexture);

//--------------------------------------------------
// 使うのを止める
// 引数  : int inIdx / インデックス
//--------------------------------------------------
void StopUseRectangle(int inIdx);

//--------------------------------------------------
// 位置の設定
// 引数1  : int inIdx / インデックス
// 引数2  : D3DXVECTOR3& inPos / 中心の位置
// 引数3  : D3DXVECTOR3& inSize / サイズ
//--------------------------------------------------
void SetPosRectangle(int inIdx, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inSize);

//--------------------------------------------------
// 回転する位置の設定
// 引数1  : int inIdx / インデックス
// 引数2  : D3DXVECTOR3& inPos / 中心の位置
// 引数3  : float inRot / 向き
// 引数4  : float inWidth / 幅
// 引数5  : float inHeight / 高さ
//--------------------------------------------------
void SetRotationPosRectangle(int inIdx, const D3DXVECTOR3& inPos, float inRot, float inWidth, float inHeight);

//--------------------------------------------------
// 色の設定
// 引数1  : int inIdx / インデックス
// 引数2  : D3DXCOLOR& inColor / 色
//--------------------------------------------------
void SetColorRectangle(int inIdx, const D3DXCOLOR& inColor);

//--------------------------------------------------
// テクスチャ座標の設定
// 引数1  : int inIdx / インデックス
// 引数2  : D3DXVECTOR2& inTexU / x = 左端, y = 右端
// 引数3  : D3DXVECTOR2& inTexV / x = 上端, y = 下端
//--------------------------------------------------
void SetTexRectangle(int inIdx, const D3DXVECTOR2& inTexU, const D3DXVECTOR2& inTexV);

//--------------------------------------------------
// 描画するかどうか
// 引数1  : int inIdx / インデックス
// 引数2  : bool inDraw / 描画するかどうか
//--------------------------------------------------
void SetDrawRectangle(int inIdx, bool inDraw);

//--------------------------------------------------
// 加算合成するかどうか
// 引数1  : int inIdx / インデックス
// 引数2  : bool inAdd / 加算合成するかどうか
//--------------------------------------------------
void SetAddRectangle(int inIdx, bool inAdd);

//--------------------------------------------------
// テクスチャの変更
// 引数1  : int inIdx / インデックス
// 引数2  : TEXTURE inTexture / 列挙型 種類
//--------------------------------------------------
void ChangeTextureRectangle(int inIdx, TEXTURE inTexture);

//--------------------------------------------------
// テクスチャの変更 [ テクスチャあり ]
// 引数1  : int inIdx / インデックス
// 引数2  : TEXTURE pTexture / 列挙型 種類
//--------------------------------------------------
void ChangeTextureRectangleWithTex(int inIdx, LPDIRECT3DTEXTURE9 pTexture);

//--------------------------------------------------
// 頂点バッファの取得
// 引数  : int inIdx / インデックス
// 返値  : LPDIRECT3DVERTEXBUFFER9 / 頂点バッファ
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int inIdx);

#endif // !_RECTANGLE_H_
