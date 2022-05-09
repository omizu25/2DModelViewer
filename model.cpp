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

#include <stdio.h>
#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const char*	FILE_NAME = "data/TEXT/motion.txt";	// ファイル名
const int	MAX_TEXT = 1024;					// テキストの最大数
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
SModel	s_model;	// モデルの情報
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void Load(void);
void LoadModel(FILE* pFile);
void LoadParts(FILE* pFile, int parts);
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
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitModel(void)
{
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateModel(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawModel(void)
{
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
			LoadModel(pFile);
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
void LoadModel(FILE* pFile)
{
	char read[MAX_TEXT] = {};
	int parts = 0;

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
			// パーツの読み込み
			LoadParts(pFile, parts);
			parts++;
		}
	}
}

//--------------------------------------------------
// パーツの読み込み
//--------------------------------------------------
void LoadParts(FILE* pFile, int parts)
{
	char read[MAX_TEXT] = {};
	SParts* pParts = &s_model.pParts[parts];

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
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxTexture);
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
			fscanf(pFile, "%f", &pParts->rot.x);
			fscanf(pFile, "%f", &pParts->rot.y);
			fscanf(pFile, "%f", &pParts->rot.z);
		}
		else if (strcmp(read, "SIZE") == 0)
		{// サイズ
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->size.x);
			fscanf(pFile, "%f", &pParts->size.y);
			fscanf(pFile, "%f", &pParts->size.z);
		}
		else if (strcmp(read, "PRIORITY") == 0)
		{// 描画の優先順位
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxPriority);
		}
	}
}
}// namespaceはここまで
