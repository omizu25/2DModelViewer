//**************************************************
//
// Hackathon ( model.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _MODEL_H_	// ���̃}�N����`������ĂȂ�������
#define _MODEL_H_	// �Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include "texture.h"

//==================================================
// �\����
//==================================================

/*�� �L�[ ��*/

struct SKey
{
	D3DXVECTOR3	pos;	// �ʒu
	D3DXVECTOR3	rot;	// ����
};

/*�� �L�[�ݒ� ��*/

struct SKeySet
{
	int		nFrame;	// �t���[����
	SKey*	pKey;	// �L�[
};

/*�� ���[�V�����ݒ� ��*/

struct SMotion
{
	bool		bLoop;		// ���[�v���邩�ǂ���
	int			nNumKey;	// �L�[��
	SKeySet*	pKeySet;	// �L�[�ݒ�
};

/*�� �p�[�c ��*/

struct SParts
{
	D3DXVECTOR3	pos;			// �ʒu
	D3DXVECTOR3	posOld;			// �O��̈ʒu
	D3DXVECTOR3	posSet;			// �ݒ�̈ʒu
	D3DXVECTOR3	rot;			// ����
	D3DXVECTOR3	rotOld;			// �O��̌���
	D3DXVECTOR3	rotSet;			// �ݒ�̌���
	D3DXVECTOR3	size;			// �T�C�Y
	D3DXMATRIX	mtxWorld;		// ���[���h�}�g���b�N�X
	ETexture	texture;		// �e�N�X�`��
	int			idxParent;		// �e�̔ԍ�
	int			idxPriority;	// �`��̗D�揇��
};

/*�� ���f����*/

struct SModel
{
	D3DXVECTOR3	pos;		// �ʒu
	D3DXVECTOR3	posOld;		// �O��̈ʒu
	D3DXVECTOR3	rot;		// ����
	D3DXVECTOR3	rotDest;	// �ړI�̌���
	D3DXVECTOR3	move;		// �ړ���
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	SParts*		pParts;		// �p�[�c�̏��
	int			numParts;	// �p�[�c��
	float		moveLoad;	// �ǂݍ��񂾈ړ���
};

//==================================================
// �v���g�^�C�v�錾
//==================================================
//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void);

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void);

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void);

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawModel(void);

#endif // !_MODEL_H_
