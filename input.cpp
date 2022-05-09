//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "input.h"
#include "game.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define NUM_KEY_MAX		(256)	//キーの最大数（キーボード）
#define PLAYER_MAX		(4)		//プレイヤーの最大人数

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------

//キーボード
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];				//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//キーボードのトリガー情報

//ジョイパッド
XINPUT_STATE g_JoyKeyState[PLAYER_MAX];						//ジョイパットのプレス情報
XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];				//ジョイパットのトリガー情報
D3DXVECTOR3 g_JoyStickPos[PLAYER_MAX];						//ジョイスティックの傾き
JOYKEY g_OldJoyKeyStick[PLAYER_MAX][JOYKEY_RIGHT_LEFT_MAX];	//前回のスティックの位置
FUNCTION_KEY g_OldFunctionKey;								//前回の機能キーの情報
MOVE_KEY g_OldMoveKey;										//前回の移動キーの情報

int g_nKeyCnt;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

//キーボード
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitKeyboard(void);								//終了処理
void UpdateKeyboard(void);								//更新処理

														//ジョイパッド
HRESULT InitJoypad(void);								//初期化
void UninitJoypad(void);								//終了処理
void UpdateJoypad(void);								//更新処理

//*************************************************************************************
//入力処理全体
//*************************************************************************************

//入力処理全部の初期化
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	g_nKeyCnt = 0;

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	
	//ジョイパッド初期化
	InitJoypad();

	return S_OK;
}

//入力処理全部の終了処理
void UninitInput(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//ジョイパッド終了処理
	UninitJoypad();
}

//入力処理全部の更新処理
void UpdateInput(void)
{
	if (GetEnablePause())
	{
		g_nKeyCnt = 20;
	}

	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッド更新処理
	UpdateJoypad();

	g_nKeyCnt--;
}


//*****************************************************************************
//キーボード入力処理
//*****************************************************************************

//キーボード初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//キーボードの終了処理
void UninitKeyboard(void)
{
	//入力デバイス（キーボード）の放棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破壊
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//キーボードの更新処理
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報
	int nCntKey;
	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();			//キーボードへのアクセス権を獲得
	}
}

//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//キーボードの全キープレス情報を取得
bool GetKeyboardAllPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardPress(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//キーボードの全キートリガー情報を取得
bool GetKeyboardAllTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardTrigger(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//*************************************************************************************
//ジョイパッド入力処理
//*************************************************************************************

//ジョイパッドの初期化
HRESULT InitJoypad(void)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//メモリーのクリア
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//ジョイパッドの終了
void UninitJoypad(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//ジョイパッドの更新
void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~g_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons; //トリガー情報を保存
			g_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //プレス処理
		}
	}
}

//ジョイパッドのプレス処理
bool GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパッドのトリガー処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパット（スティックプレス）処理
D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft, int nPlayer)
{
	switch (RightLeft)
	{
	case JOYKEY_LEFT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return g_JoyStickPos[nPlayer];
}

//ジョイパット（トリガーペダル）処理
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//ジョイパッドスティックプレス８方向
bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key, int nPlayer)
{
	D3DXVECTOR3 pos = GetJoypadStick(RightLeft, nPlayer);	//現在の傾きの取得
	pos.y *= -1.0f;//Yを分かりやすくするために＋－を反転

	if (pos.y > 0.5f
		&& JOYKEY_STICK_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y > 0.5f
		&& JOYKEY_STICK_UP_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& JOYKEY_STICK_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y < -0.5f
		&& JOYKEY_STICK_DOWN_RIGHT == Key)
	{
		return true;
	}
	else if (pos.y < -0.5f
		&& JOYKEY_STICK_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y < -0.5f
		&& JOYKEY_STICK_DOWN_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& JOYKEY_STICK_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y > 0.5f
		&& JOYKEY_STICK_UP_LEFT == Key)
	{
		return true;
	}
	return false;
}

//ジョイパッドスティックトリガー８方向
bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key, int nPlayer)
{
	if (GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key != g_OldJoyKeyStick[nPlayer][RightLeft])
	{
		g_OldJoyKeyStick[nPlayer][RightLeft] = Key;
		return true;
	}
	else if (GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key == g_OldJoyKeyStick[nPlayer][RightLeft])
	{
		return false;
	}
	else if (!GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key != g_OldJoyKeyStick[nPlayer][RightLeft])
	{
		return false;
	}

	g_OldJoyKeyStick[nPlayer][RightLeft] = JOYKEY_MAX;
	return false;
}

//ジョイパッドスティックトリガー８方向の全プレイヤーの対象
bool GetJoypadStickAllTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		if (GetJoypadStickTrigger(RightLeft, Key, nCnt))
		{
			return true;
		}
	}
	return false;
}

//ジョイパッドトリガーの全プレイヤーの対象
bool GetJoypadAllTrigger(JOYKEY Key)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		if (GetJoypadTrigger(Key, nCnt))
		{
			return true;
		}
	}
	return false;
}

//移動系のキーまとめプレス
bool GetMoveKeyPress(MOVE_KEY Key)
{
	if (Key == MOVE_KEY_UP)
	{
		if (GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_STICK_UP, 0)
			|| GetJoypadStickPress(JOYKEY_RIGHT_STICK, JOYKEY_STICK_UP, 0)
			|| GetJoypadPress(JOYKEY_CROSS_UP, 0)
			|| GetKeyboardPress(DIK_W)
			|| GetKeyboardPress(DIK_UP))
		{
			return true;
		}
	}
	else if (Key == MOVE_KEY_DOWN)
	{
		if (GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_STICK_DOWN, 0)
			|| GetJoypadStickPress(JOYKEY_RIGHT_STICK, JOYKEY_STICK_DOWN, 0)
			|| GetJoypadPress(JOYKEY_CROSS_DOWN, 0)
			|| GetKeyboardPress(DIK_S)
			|| GetKeyboardPress(DIK_DOWN))
		{
			return true;
		}
	}
	else if (Key == MOVE_KEY_LEFT)
	{
		if (GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_STICK_LEFT, 0)
			|| GetJoypadStickPress(JOYKEY_RIGHT_STICK, JOYKEY_STICK_LEFT, 0)
			|| GetJoypadPress(JOYKEY_CROSS_LEFT, 0)
			|| GetKeyboardPress(DIK_A)
			|| GetKeyboardPress(DIK_LEFT))
		{
			return true;
		}
	}
	else if (Key == MOVE_KEY_RIGHT)
	{
		if (GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_STICK_RIGHT, 0)
			|| GetJoypadStickPress(JOYKEY_RIGHT_STICK, JOYKEY_STICK_RIGHT, 0)
			|| GetJoypadPress(JOYKEY_CROSS_RIGHT, 0)
			|| GetKeyboardPress(DIK_D)
			|| GetKeyboardPress(DIK_RIGHT))
		{
			return true;
		}
	}

	return false;
}

//機能系のキーまとめ
bool GetFunctionKeyTrigger(FUNCTION_KEY Key)
{
	if (Key == FUNCTION_KEY_DESISION)
	{
		if (GetJoypadPress(JOYKEY_A, 0) || GetJoypadPress(JOYKEY_B, 0)
			|| GetKeyboardPress(DIK_SPACE)
			|| GetKeyboardPress(DIK_RETURN))
		{
			if (g_OldFunctionKey != Key)
			{
				g_OldFunctionKey = Key;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else if (Key == FUNCTION_KEY_PAUSE)
	{
		if (GetJoypadPress(JOYKEY_START, 0)
			|| GetKeyboardPress(DIK_P))
		{
			if (g_OldFunctionKey != Key)
			{
				g_OldFunctionKey = Key;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	g_OldFunctionKey = FUNCTION_KEY_MAX;
	return false;
}

//移動系のキーまとめトリガー
bool GetMoveKeyTrigger(MOVE_KEY Key)
{
	if (GetMoveKeyPress(Key)
		&& g_OldMoveKey == Key)
	{
		return false;
	}
	else if (GetMoveKeyPress(Key)
		&& g_OldMoveKey == MOVE_KEY_MAX)
	{
		g_OldMoveKey = Key;
		return true;
	}
	else if (GetMoveKeyPress(g_OldMoveKey))
	{
		return false;
	}

	g_OldMoveKey = MOVE_KEY_MAX;							//前回のライトのキーの情報のリセット
	return false;
}