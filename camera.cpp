//**************************************************
// 
// 3Dゲーム制作 ( camera.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "bg.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float	MAX_NEAR = 10.0f;	// ニアの最大値
const float	MAX_FAR = 1000.0f;	// ファーの最大値
}

//==================================================
// スタティック変数
//==================================================
namespace
{
SCamera	s_camera;	// カメラの情報
}

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void Move(void);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCamera(void)
{
	// 視点・注視点・上方向・向き・距離を設定する
	s_camera.posV = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	s_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera.rot = D3DXVECTOR3((D3DX_PI * 0.5f), 0.0f, 0.0f);
	s_camera.posVDest = s_camera.posV;
	s_camera.posRDest = s_camera.posR;
	s_camera.rotDest = s_camera.rot;
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 固定でいい

	float disX, disZ;

	disX = s_camera.posV.x - s_camera.posR.x;
	disZ = s_camera.posV.z - s_camera.posR.z;

	s_camera.fDistance = sqrtf((disX * disX) + (disZ * disZ));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitCamera(void)
{
	
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateCamera(void)
{
	// 移動
	//Move();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetCamera(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&s_camera.mtxView,
		&s_camera.posV,
		&s_camera.posR,
		&s_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixOrthoLH(
		&s_camera.mtxProjection,
		(float)SCREEN_WIDTH,
		(float)SCREEN_HEIGHT,
		MAX_NEAR,
		MAX_FAR);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
SCamera *GetCamera(void)
{
	return &s_camera;
}

namespace
{
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

	// ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	s_camera.posR += vec * 5.0f;
	s_camera.posV += vec * 5.0f;
}
}// namespaceはここまで
