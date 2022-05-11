//**************************************************
//
// Hackathon ( model.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "input.h"
#include "model.h"
#include "color.h"
#include "utility.h"

#include <stdio.h>
#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[3D] 位置・法線・カラー・テクスチャ

/*↓ 頂点情報[3D] ↓*/

struct VERTEX_3D
{
	D3DXVECTOR3	pos;	// 頂点座標
	D3DXVECTOR3	nor;	// 法線ベクトル
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2	tex;	// テクスチャ座標
};

const char*	FILE_NAME = "data/TEXT/motion.txt";	// ファイル名
const int	MAX_TEXT = 1024;					// テキストの最大数
const int	IDX_PARENT = -1;					// 親の番号
const float	MAX_BLEND = 3.0f;					// ブレンドの最大値
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
SModel	s_model;			// モデルの情報
EType	s_IdxMotion;		// モーション番号
int		s_nSelectMotion;	// 選ばれているモーション
int		s_nFrame;			// フレーム数
int		s_nIdxKey;			// キー番号
bool	s_bMotionBlend;		// モーションブレンド
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void Load(void);
void LoadModel(FILE* pFile, ETexture* pTexture);
void LoadParts(FILE* pFile, ETexture* pTexture, SParts* pParts);
void LoadMotion(FILE* pFile, SMotion* pMotion);
void LoadKeySet(FILE* pFile, SKeySet* pKeySet);
void LoadKey(FILE* pFile, SKey* pKey);
void Move(void);
void Rot(void);
void Motion(void);
void NextMotion(EType type);
void OldSave(void);
void MotionBlend(void);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void)
{
	// メモリのクリア
	memset(&s_model, 0, sizeof(s_model));

	// 読み込み
	Load();

	VERTEX_3D *pVtx = nullptr;	// 頂点情報へのポインタ

	for (int i = 0; i < s_model.numParts; i++)
	{
		SParts* pParts = &s_model.pParts[i];

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pParts->pVtxBuff,
			NULL);

		// 頂点情報をロックし、頂点情報へのポインタを取得
		pParts->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float width = pParts->width * 0.5f;
		float height = pParts->height * 0.5f;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-width, +height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+width, +height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-width, -height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+width, -height, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = GetColor(COLOR_WHITE);
		pVtx[1].col = GetColor(COLOR_WHITE);
		pVtx[2].col = GetColor(COLOR_WHITE);
		pVtx[3].col = GetColor(COLOR_WHITE);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		pParts->pVtxBuff->Unlock();
	}

	for (int i = 0; i < s_model.numParts; i++)
	{
		s_model.pParts[i].savePos = s_model.pParts[i].pos;
		s_model.pParts[i].saveRot = s_model.pParts[i].rot;
		s_model.pParts[i].posOld = s_model.pParts[i].pos;
		s_model.pParts[i].rotOld = s_model.pParts[i].rot;
	}

	s_model.posOld = s_model.pos;
	s_model.rotDest = s_model.rot;
	s_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_IdxMotion = TYPE_NEUTRAL;
	s_nSelectMotion = 0;
	s_nFrame = 0;;
	s_nIdxKey = 0;
	s_bMotionBlend = true;
	
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitModel(void)
{
	if (s_model.pParts != nullptr)
	{// NULLチェック
		for (int i = 0; i < s_model.numParts; i++)
		{
			SParts* pParts = &s_model.pParts[i];

			if (pParts->pVtxBuff != NULL)
			{// 頂点バッファの解放
				pParts->pVtxBuff->Release();
				pParts->pVtxBuff = NULL;
			}
		}

		delete[] s_model.pParts;
		s_model.pParts = nullptr;
	}

	for (int i = 0; i < TYPE_MAX; i++)
	{
		SMotion* pMotion = &s_model.motion[i];

		for (int j = 0; j < pMotion->nNumKey; j++)
		{
			SKeySet* pKeySet = &pMotion->pKeySet[i];

			if (pKeySet->pKey != nullptr)
			{// NULLチェック
				delete[] pKeySet->pKey;
				pKeySet->pKey = nullptr;
			}
		}

		if (pMotion->pKeySet != nullptr)
		{// NULLチェック
			delete[] pMotion->pKeySet;
			pMotion->pKeySet = nullptr;
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateModel(void)
{
	// 移動
	Move();

	// 回転
	Rot();

	// モーション
	Motion();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i= 0; i < s_model.numParts; i++)
	{
		SParts* pParts = &s_model.pParts[i];

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pParts->pVtxBuff, 0, sizeof(VERTEX_3D));

		// パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&pParts->mtxWorld);

		// パーツの向きを反映
		D3DXMatrixRotationZ(&mtxRot, pParts->rot);
		D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxRot);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParts->pos.x, pParts->pos.y, pParts->pos.z);
		D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxTrans);

		if (pParts->idxParent == IDX_PARENT)
		{// 親
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&s_model.mtxWorld);

			// 向きを反映
			D3DXMatrixRotationZ(&mtxRot, s_model.rot);
			D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);
			D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);

			mtxParent = s_model.mtxWorld;
		}
		else
		{// 子
			mtxParent = s_model.pParts[pParts->idxParent].mtxWorld;
		}

		// 親モデルとのマトリックスの掛け算
		D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pParts->mtxWorld);

		// テクスチャの設定
		pDevice->SetTexture(0, GetTexture(pParts->texture));

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			0,						// 描画する最初の頂点インデックス
			2);						// プリミティブ(ポリゴン)数

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
	}

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

namespace
{
//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void Load(void)
{
	FILE* pFile = nullptr;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile == nullptr)
	{// ファイルが開かない場合
		assert(false);
		return;
	}

	char read[MAX_TEXT] = {};
	ETexture* pTextute = nullptr;
	int numTextute = 0;
	int countTexture = 0;
	int countMotion = 0;
	
	while (fscanf(pFile, "%s", read) != EOF)
	{// ファイルの最後が来るまで繰り返す
		if (strncmp(read, "#=", 2) == 0)
		{// 枠線
			continue;
		}
		else if (strncmp(read, "#", 1) == 0)
		{// 見出し
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "NUM_TEXTURE") == 0)
		{// モデルの使用数
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &numTextute);

			if (pTextute == nullptr)
			{// NULLチェック
				pTextute = new ETexture[numTextute];
			}
			else
			{
				assert(false);
			}
		}
		else if (strcmp(read, "TEXTURE_FILENAME") == 0)
		{// モデルファイル名
			char texture[MAX_TEXT] = {};
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%s", &texture);

			if (pTextute != nullptr)
			{// NULLチェック
				// テクスチャの種類の取得
				pTextute[countTexture] = GetFileNameTexture(texture);
				countTexture++;
			}
		}
		else if (strcmp(read, "MODELSET") == 0)
		{// モデルの情報
			// モデルの読み込み
			LoadModel(pFile, pTextute);
		}
		else if (strcmp(read, "MOTIONSET") == 0)
		{// モーションの情報
			assert(countMotion >= 0 && countMotion < TYPE_MAX);

			// モーションの読み込み
			LoadMotion(pFile, &s_model.motion[countMotion]);
			countMotion++;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	if (pTextute != nullptr)
	{// NULLチェック
		delete[] pTextute;
		pTextute = nullptr;
	}
}

//--------------------------------------------------
// モデルの読み込み
//--------------------------------------------------
void LoadModel(FILE* pFile, ETexture* pTexture)
{
	char read[MAX_TEXT] = {};
	int countParts = 0;

	while (strcmp(read, "END_MODELSET") != 0)
	{// 終わりが来るまで繰り返す
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// コメント
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "NUM_PARTS") == 0)
		{// パーツの使用数
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &s_model.numParts);

			if (s_model.pParts == nullptr)
			{// NULLチェック
				s_model.pParts = new SParts[s_model.numParts];
			}
			else
			{
				assert(false);
			}
		}
		else if (strcmp(read, "MOVE") == 0)
		{// 移動量
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &s_model.moveLoad);
		}
		else if (strcmp(read, "PARTSSET") == 0)
		{// パーツの情報
			assert(countParts >= 0 && countParts < s_model.numParts);

			// パーツの読み込み
			LoadParts(pFile, pTexture, &s_model.pParts[countParts]);
			countParts++;
		}
	}
}

//--------------------------------------------------
// パーツの読み込み
//--------------------------------------------------
void LoadParts(FILE* pFile, ETexture* pTexture, SParts* pParts)
{
	char read[MAX_TEXT] = {};

	while (strcmp(read, "END_PARTSSET") != 0)
	{// 終わりが来るまで繰り返す
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// コメント
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "INDEX") == 0)
		{// 使用するテクスチャの番号
			int idx = 0;
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &idx);
			pParts->texture = pTexture[idx];
		}
		else if (strcmp(read, "PARENT") == 0)
		{// 親の番号
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxParent);
		}
		else if (strcmp(read, "POS") == 0)
		{// 位置
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->pos.x);
			fscanf(pFile, "%f", &pParts->pos.y);
			fscanf(pFile, "%f", &pParts->pos.z);
		}
		else if (strcmp(read, "ROT") == 0)
		{// 向き
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->rot);
		}
		else if (strcmp(read, "WIDTH") == 0)
		{// 幅
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->width);
		}
		else if (strcmp(read, "HEIGHT") == 0)
		{// 高さ
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->height);
		}
		else if (strcmp(read, "PRIORITY") == 0)
		{// 描画の優先順位
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxPriority);
		}
	}
}

//--------------------------------------------------
// モーションの読み込み
//--------------------------------------------------
void LoadMotion(FILE* pFile, SMotion *pMotion)
{
	char read[MAX_TEXT] = {};
	int countKeySet = 0;

	while (strcmp(read, "END_MOTIONSET") != 0)
	{// 終わりが来るまで繰り返す
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// コメント
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "LOOP") == 0)
		{// ループするかどうか
			int loop = 0;
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &loop);

			if (loop == 0)
			{// ループしない
				pMotion->bLoop = false;
			}
			else
			{
				pMotion->bLoop = true;
			}
		}
		else if (strcmp(read, "NUM_KEY") == 0)
		{// キー数
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pMotion->nNumKey);

			if (pMotion->pKeySet == nullptr)
			{// NULLチェック
				pMotion->pKeySet = new SKeySet[pMotion->nNumKey];

				for (int i = 0; i < pMotion->nNumKey; i++)
				{
					pMotion->pKeySet[i].pKey = nullptr;
				}
			}
			else
			{
				assert(false);
			}
		}
		else if (strcmp(read, "KEYSET") == 0)
		{// キー設定の情報
			assert(countKeySet >= 0 && countKeySet < pMotion->nNumKey);

			// キー設定の読み込み
			LoadKeySet(pFile, &pMotion->pKeySet[countKeySet]);
			countKeySet++;
		}
	}
}

//--------------------------------------------------
// キー設定の読み込み
//--------------------------------------------------
void LoadKeySet(FILE* pFile, SKeySet *pKeySet)
{
	char read[MAX_TEXT] = {};
	int countKey = 0;

	if (pKeySet->pKey == nullptr)
	{// NULLチェック
		pKeySet->pKey = new SKey[s_model.numParts];
	}
	else
	{
		assert(false);
	}

	while (strcmp(read, "END_KEYSET") != 0)
	{// 終わりが来るまで繰り返す
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// コメント
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "FRAME") == 0)
		{// ループするかどうか
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pKeySet->nFrame);
		}
		else if (strcmp(read, "KEY") == 0)
		{// キーの情報
			assert(countKey >= 0 && countKey < s_model.numParts);

			// キーの読み込み
			LoadKey(pFile, &pKeySet->pKey[countKey]);
			countKey++;
		}
	}
}

//--------------------------------------------------
// キーの読み込み
//--------------------------------------------------
void LoadKey(FILE* pFile, SKey *pKey)
{
	char read[MAX_TEXT] = {};

	while (strcmp(read, "END_KEY") != 0)
	{// 終わりが来るまで繰り返す
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// コメント
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "POS") == 0)
		{// 位置
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pKey->pos.x);
			fscanf(pFile, "%f", &pKey->pos.y);
			fscanf(pFile, "%f", &pKey->pos.z);
		}
		else if (strcmp(read, "ROT") == 0)
		{// 向き
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pKey->rot);
		}
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
void Move(void)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetMoveKeyPress(MOVE_KEY_LEFT))
	{// 左キーが押された
		vec.x -= 1.0f;
	}
	if (GetMoveKeyPress(MOVE_KEY_RIGHT))
	{// 右キーが押された
		vec.x += 1.0f;
	}
	if (GetMoveKeyPress(MOVE_KEY_UP))
	{// 上キーが押された
		vec.y += 1.0f;
	}
	if (GetMoveKeyPress(MOVE_KEY_DOWN))
	{// 下キーが押された
		vec.y -= 1.0f;
	}

	if ((vec.x == 0.0f) && (vec.y == 0.0f))
	{// 移動してない
		// 次のモーション
		NextMotion(TYPE_NEUTRAL);
		
		return;
	}

	// 次のモーション
	NextMotion(TYPE_MOVE);

	// ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	s_model.rotDest = atan2f(vec.y, vec.x) - (D3DX_PI * 0.5f);
	s_model.move += vec * s_model.moveLoad;

	// 移動
	s_model.pos.x += s_model.move.x;
	s_model.pos.y += s_model.move.y;

	// 慣性・移動量を更新 (減衰させる)
	s_model.move.x += (0.0f - s_model.move.x) * 1.0f;
	s_model.move.y += (0.0f - s_model.move.y) * 1.0f;
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
void Rot(void)
{
	if (s_model.rot == s_model.rotDest)
	{// 回転してない
		return;
	}

	// 角度の正規化
	NormalizeAngle(&s_model.rotDest);

	float rot = s_model.rotDest - s_model.rot;

	// 角度の正規化
	NormalizeAngle(&rot);

	//慣性・向きを更新 (減衰させる)
	s_model.rot += rot * 0.25f;

	// 角度の正規化
	NormalizeAngle(&s_model.rot);
}

//--------------------------------------------------
// モーション
//--------------------------------------------------
void Motion(void)
{
	s_nFrame++;

	if (s_bMotionBlend)
	{// モーションブレンドをしている
		// モーションブレンド
		MotionBlend();

		return;
	}

	SMotion* pMotion = &s_model.motion[s_IdxMotion];
	SKeySet* pKeySet = &pMotion->pKeySet[s_nIdxKey];

	if (s_nFrame >= pMotion->pKeySet[s_nIdxKey].nFrame)
	{// フレーム数が指定を超えた
		// 前回の保存
		OldSave();
		
		s_nIdxKey++;

		if (pMotion->bLoop)
		{// ループする
			s_nIdxKey %= pMotion->nNumKey;
		}
		else
		{// ループしない
			if (s_nIdxKey >= pMotion->nNumKey)
			{// キー数が超えた
				// 次のモーション
				NextMotion(TYPE_NEUTRAL);
			}
		}

		s_nFrame = 0;
	}

	for (int i = 0; i < s_model.numParts; i++)
	{
		SParts* pParts = &s_model.pParts[i];

		D3DXVECTOR3 pos = pKeySet->pKey[i].pos;
		pos -= pParts->posOld;
		pos += pParts->savePos;
		pos /= (float)pKeySet->nFrame;

		pParts->pos += pos;

		float rot = pKeySet->pKey[i].rot;
		rot -= pParts->rotOld;
		rot += pParts->saveRot;
		rot /= (float)pKeySet->nFrame;

		// 角度の正規化
		NormalizeAngle(&rot);

		pParts->rot += rot;
	}
}

//--------------------------------------------------
// 次のモーション
//--------------------------------------------------
void NextMotion(EType type)
{
	if (s_IdxMotion == type)
	{// 現在と同じ
		return;
	}

	s_nFrame = 0;
	s_nIdxKey = 0;
	s_IdxMotion = type;
	s_bMotionBlend = true;
}

//--------------------------------------------------
// 前回の保存
//--------------------------------------------------
void OldSave(void)
{
	for (int i = 0; i < s_model.numParts; i++)
	{
		SParts *pParts = &s_model.pParts[i];
		SKey *pKey = &s_model.motion[s_IdxMotion].pKeySet[s_nIdxKey].pKey[i];

		pParts->posOld = pParts->savePos + pKey->pos;
		pParts->rotOld = pParts->saveRot + pKey->rot;
	}
}

//--------------------------------------------------
// モーションブレンド
//--------------------------------------------------
void MotionBlend(void)
{
	s_nIdxKey = 0;
	bool bArrivalPos = true;
	bool bArrivalRot = true;

	for (int i = 0; i < s_model.numParts; i++)
	{
		SParts *pParts = &s_model.pParts[i];
		SKey *pKey = &s_model.motion[s_IdxMotion].pKeySet[s_nIdxKey].pKey[i];

		D3DXVECTOR3 pos = pParts->savePos + pKey->pos;

		if (!Homing(&pParts->pos, pParts->pos, pos, MAX_BLEND))
		{// ホーミング
			bArrivalPos = false;
		}

		float rotDest = pParts->saveRot + pKey->rot;
		float length = (rotDest - pParts->rot) / MAX_BLEND;

		// 角度の正規化
		NormalizeAngle(&length);

		pParts->rot += length;
	}

	if (bArrivalPos && bArrivalRot)
	{// フレーム数が超えた
		// 前回の保存
		OldSave();

		s_bMotionBlend = false;
		s_nFrame = 0;
		s_nIdxKey++;
	}
}
}// namespaceはここまで
