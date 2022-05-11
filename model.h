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
// ��`
//==================================================

enum EType
{
	TYPE_NEUTRAL = 0,	// �j���[�g����
	TYPE_MOVE,			// �ړ�
	TYPE_MAX,
	TYPE_NONE
};

/*�� �L�[ ��*/

struct SKey
{
	D3DXVECTOR3	pos;	// �ʒu
	float		rot;	// ����
};

/*�� �L�[�ݒ� ��*/

struct SKeySet
{
	int		frame;	// �t���[����
	SKey*	pKey;	// �L�[
};

/*�� ���[�V�����ݒ� ��*/

struct SMotion
{
	bool		loop;		// ���[�v���邩�ǂ���
	int			numKey;		// �L�[��
	SKeySet*	pKeySet;	// �L�[�ݒ�
};

/*�� �p�[�c ��*/

struct SParts
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				posOld;			// �O��̈ʒu
	D3DXVECTOR3				savePos;		// �ʒu��ۑ�
	D3DXMATRIX				mtxWorld;		// �}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;		// ���_�o�b�t�@
	ETexture				texture;		// �e�N�X�`��
	int						idxRectangle;	// ��`�̔ԍ�
	int						idxParent;		// �e�̔ԍ�
	int						idxPriority;	// �`��̗D�揇��
	float					rot;			// ����
	float					rotOld;			// �O��̌���
	float					saveRot;		// �����̕ۑ�
	float					width;			// ��
	float					height;			// ����
};

/*�� ���f����*/

struct SModel
{
	D3DXVECTOR3	pos;				// �ʒu
	D3DXVECTOR3	posOld;				// �O��̈ʒu
	D3DXVECTOR3	move;				// �ړ���
	D3DXMATRIX	mtxWorld;			// �}�g���b�N�X
	SParts*		pParts;				// �p�[�c�̏��
	SMotion		motion[TYPE_MAX];	// ���[�V�����̏��
	int			numParts;			// �p�[�c��
	float		moveLoad;			// �ǂݍ��񂾈ړ���
	float		rot;				// ����
	float		rotDest;			// �ړI�̌���
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
