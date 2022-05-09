//**************************************************
//
// Hackathon ( bg.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "bg.h"
#include "texture.h"
#include "color.h"
#include "input.h"
#include "utility.h"
#include "rectangle.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_idx;	// �w�i�̃C���f�b�N�X
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�֐��v���g�^�C�v�錾
//==================================================
namespace
{
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBG(void)
{
	// ��`�̐ݒ�
	s_idx = SetRectangle(TEXTURE_NONE);

	D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// ��`�̈ʒu�̐ݒ�
	SetPosRectangle(s_idx, pos, size);

	// ��`�̐F�̐ݒ�
	SetColorRectangle(s_idx, GetColor(COLOR_GRAY));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBG(void)
{
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBG(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBG(void)
{
}