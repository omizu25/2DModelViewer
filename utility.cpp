//**************************************************
// 
// Hackathon ( utility.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "utility.h"

//--------------------------------------------------
// 角度の正規化
//--------------------------------------------------
void NormalizeAngle(float* pAngle)
{
	if (*pAngle >= D3DX_PI)
	{// 3.14より大きい
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle <= -D3DX_PI)
	{// -3.14より小さい
		*pAngle += D3DX_PI * 2.0f;
	}
}

//--------------------------------------------------
// ホーミング
//--------------------------------------------------
bool Homing(D3DXVECTOR3* pPosOut, const D3DXVECTOR3& inPosNow, const D3DXVECTOR3& inPosDest, float inSpeed)
{
	D3DXVECTOR3 vecDiff = inPosDest - inPosNow;

	float fLength = D3DXVec3Length(&vecDiff);

	if (fLength <= inSpeed)
	{// 速さより長さが小さい時
		*pPosOut = inPosDest;
		return true;
	}
	else
	{// 速さより長さが大きい時
		*pPosOut = inPosNow + ((vecDiff / fLength) * inSpeed);
		return false;
	}
}

//--------------------------------------------------
// 小数点のランダム
//--------------------------------------------------
float FloatRandam(float inMax, float inMin)
{
	return ((rand() / (float)RAND_MAX) * (inMax - inMin)) + inMin;
}

//--------------------------------------------------
// 整数のランダム
//--------------------------------------------------
int IntRandam(int inMax, int inMin)
{
	return (int)((rand() / (float)RAND_MAX) * (inMax - inMin)) + inMin;
}

//--------------------------------------------------
// sinカーブの値が1.0f～0.0fで帰ってくる
//--------------------------------------------------
float SinCurve(int inTime, float inCycle)
{
	return (sinf((inTime * inCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
}

//--------------------------------------------------
// cosカーブの値が1.0f～0.0fで帰ってくる
//--------------------------------------------------
float CosCurve(int inTime, float inCycle)
{
	return (cosf((inTime * inCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
}

//--------------------------------------------------
// カーブの値が帰ってくる
//--------------------------------------------------
float Curve(float inCurve, float inMax, float inMin)
{
	return (inCurve * (inMax - inMin)) + inMin;
}
