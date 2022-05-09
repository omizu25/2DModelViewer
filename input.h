//=============================================================================
//
// 入力処理 [input.h]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================
#ifndef _INPUT_H_	//このマクロ定義がされなかったら
#define _INPUT_H_	//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//ライブラリーファイルのリンク
//----------------------------------------------------------------------------
#pragma comment(lib,"dinput8.lib")	//入力処理に必要(キーボード)
#pragma comment(lib,"xinput.lib")	//入力処理に必要（ジョイパット）

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include "main.h"

//----------------------------------------------------------------------------
//列挙型
//----------------------------------------------------------------------------

//キー種類(ジョイパッド)
typedef enum
{
	JOYKEY_CROSS_UP = 0,	//十字ボタン上
	JOYKEY_CROSS_DOWN,		//十字ボタン下
	JOYKEY_CROSS_LEFT,		//十字ボタン左
	JOYKEY_CROSS_RIGHT,		//十字ボタン右
	JOYKEY_START,			//スタートボタン
	JOYKEY_BACK,			//バックボタン
	JOYKEY_LEFT_THUMB,		//左スティック押込み
	JOYKEY_RIGHT_THUMB,		//右スティック押込み
	JOYKEY_LEFT_SHOULDER,	//L１ボタン
	JOYKEY_RIGHT_SHOULDER,	//R１ボタン
	JOYKEY_LEFT_TRIGGER,	//L２トリガー
	JOYKEY_RIGHT_TRIGGER,	//R２トリガー
	JOYKEY_A,				//Aボタン
	JOYKEY_B,				//Bボタン
	JOYKEY_X,				//Xボタン
	JOYKEY_Y,				//Yボタン
	JOYKEY_STICK_UP = 0,	//上	(8方向スティック)
	JOYKEY_STICK_UP_RIGHT,	//右上	(8方向スティック)
	JOYKEY_STICK_RIGHT,		//右	(8方向スティック)
	JOYKEY_STICK_DOWN_RIGHT,//右下	(8方向スティック)
	JOYKEY_STICK_DOWN,		//下	(8方向スティック)
	JOYKEY_STICK_DOWN_LEFT,	//左下	(8方向スティック)
	JOYKEY_STICK_LEFT,		//左	(8方向スティック)
	JOYKEY_STICK_UP_LEFT,	//左上	(8方向スティック)
	JOYKEY_MAX
}JOYKEY;

//スティック識別(ジョイパッド)
typedef enum
{
	JOYKEY_RIGHT_STICK = 0,
	JOYKEY_LEFT_STICK,
	JOYKEY_RIGHT_LEFT_MAX
}JOYKEY_RIGHT_LEFT;

//移動系のキーまとめ
typedef enum
{
	MOVE_KEY_UP = 0,	//上キー
	MOVE_KEY_DOWN,		//下キー
	MOVE_KEY_RIGHT,		//右キー
	MOVE_KEY_LEFT,		//左キー
	MOVE_KEY_MAX
}MOVE_KEY;

//キータイプ
typedef enum
{
	FUNCTION_KEY_DESISION = 0,	//決定キー
	FUNCTION_KEY_PAUSE,			//ポーズキー
	FUNCTION_KEY_MAX
}FUNCTION_KEY;

//----------------------------------------------------------------------------
//プロトタイプ宣言
//----------------------------------------------------------------------------

//入力処理のプロトタイプ宣言
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);	//入力処理全部の初期化
void UninitInput(void);								//入力処理全部の終了処理
void UpdateInput(void);								//入力処理全部の更新処理

//プロトタイプ宣言キーボード
bool GetKeyboardPress(int nKey);	//キーボードプレス処理
bool GetKeyboardTrigger(int nKey);	//キーボードトリガー処理
bool GetKeyboardAllPress(void);		//キーボード全キープレス処理
bool GetKeyboardAllTrigger(void);	//キーボード全キートリガー処理

//プロトタイプ宣言ジョイパット
bool GetJoypadPress(JOYKEY Key, int nPlayer);										//ジョイパッドプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);										//ジョイパッドトリガー処理
D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft, int nPlayer);				//ジョイパッドスティック処理
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);									//ジョイパッドトリガーペダル処理
bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key, int nPlayer);		//ジョイパッドスティックプレス８方向
bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key, int nPlayer);	//ジョイパッドスティックトリガー８方向			
bool GetJoypadStickAllTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key);				//ジョイパッドスティックトリガー８方向の全プレイヤーの対象
bool GetJoypadAllTrigger(JOYKEY Key);												//ジョイパッドトリガーの全プレイヤーの対象

bool GetMoveKeyPress(MOVE_KEY Key);				//移動系のキーまとめプレス
bool GetMoveKeyTrigger(MOVE_KEY Key);			//移動系のキーまとめトリガー
bool GetFunctionKeyTrigger(FUNCTION_KEY Key);	//機能系のキーまとめ

#endif
