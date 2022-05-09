//**************************************************
//
// Hackathon ( model.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "input.h"
#include "model.h"
#include "rectangle3D.h"
#include "color.h"
#include "utility.h"

#include <stdio.h>
#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const char*	FILE_NAME = "data/TEXT/motion.txt";	// �t�@�C����
const int	MAX_TEXT = 1024;					// �e�L�X�g�̍ő吔
const int	IDX_PARENT = -1;					// �e�̔ԍ�
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
SModel	s_model;	// ���f���̏��
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�֐��v���g�^�C�v�錾
//==================================================
namespace
{
void Load(void);
void LoadModel(FILE* pFile, ETexture* pTexture);
void LoadParts(FILE* pFile, ETexture* pTexture, int parts);
void Move(void);
void Rot(void);
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void)
{
	// �������̃N���A
	memset(&s_model, 0, sizeof(s_model));

	// �ǂݍ���
	Load();

	for (int i = 0; i < s_model.numParts; i++)
	{
		SParts* pParts = &s_model.pParts[i];

		// ���_�o�b�t�@�̐���
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pParts->pVtxBuff,
			NULL);

		VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		pParts->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		float width = pParts->width * 0.5f;
		float height = pParts->height * 0.5f;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-width, +height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+width, +height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-width, -height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+width, -height, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = GetColor(COLOR_WHITE);
		pVtx[1].col = GetColor(COLOR_WHITE);
		pVtx[2].col = GetColor(COLOR_WHITE);
		pVtx[3].col = GetColor(COLOR_WHITE);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		pParts->pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void)
{
	if (s_model.pParts != nullptr)
	{// NULL�`�F�b�N
		for (int i = 0; i < s_model.numParts; i++)
		{
			SParts* pParts = &s_model.pParts[i];

			if (pParts->pVtxBuff != NULL)
			{// ���_�o�b�t�@�̉��
				pParts->pVtxBuff->Release();
				pParts->pVtxBuff = NULL;
			}
		}

		delete[] s_model.pParts;
		s_model.pParts = nullptr;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void)
{
	// �ړ�
	Move();

	// ��]
	Rot();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i= 0; i < s_model.numParts; i++)
	{
		SParts* pParts = &s_model.pParts[i];

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pParts->pVtxBuff, 0, sizeof(VERTEX_3D));

		// �p�[�c�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pParts->mtxWorld);

		// �p�[�c�̌����𔽉f
		D3DXMatrixRotationZ(&mtxRot, pParts->rot);
		D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxRot);

		// �p�[�c�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pParts->pos.x, pParts->pos.y, pParts->pos.z);
		D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxTrans);

		D3DXMATRIX mtxParent;	// �v�Z�p�}�g���b�N�X

		if (pParts->idxParent == IDX_PARENT)
		{// �e
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&s_model.mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationZ(&mtxRot, s_model.rot);
			D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);
			D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);

			mtxParent = s_model.mtxWorld;
		}
		else
		{// �q
			mtxParent = s_model.pParts[pParts->idxParent].mtxWorld;
		}

		// �e���f���Ƃ̃}�g���b�N�X�̊|���Z
		D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pParts->mtxWorld);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, GetTexture(pParts->texture));

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);						// �v���~�e�B�u(�|���S��)��

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
	}

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

namespace
{
//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void Load(void)
{
	FILE* pFile = nullptr;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile == nullptr)
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
		return;
	}

	char read[MAX_TEXT] = {};
	ETexture* pTextute = nullptr;
	int numTextute = 0;
	int countTexture = 0;
	
	while (fscanf(pFile, "%s", read) != EOF)
	{// �t�@�C���̍Ōオ����܂ŌJ��Ԃ�
		if (strncmp(read, "#=", 2) == 0)
		{// �g��
			continue;
		}
		else if (strncmp(read, "#", 1) == 0)
		{// ���o��
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "NUM_TEXTURE") == 0)
		{// ���f���̎g�p��
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &numTextute);

			if (pTextute == nullptr)
			{// NULL�`�F�b�N
				pTextute = new ETexture[numTextute];
			}
			else
			{
				assert(false);
			}
		}
		else if (strcmp(read, "TEXTURE_FILENAME") == 0)
		{// ���f���t�@�C����
			char texture[MAX_TEXT] = {};
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%s", &texture);

			if (pTextute != nullptr)
			{// NULL�`�F�b�N
				// �e�N�X�`���̎�ނ̎擾
				pTextute[countTexture] = GetFileNameTexture(texture);
				countTexture++;
			}
		}
		else if (strcmp(read, "MODELSET") == 0)
		{// ���f���̏��
			// ���f���̓ǂݍ���
			LoadModel(pFile, pTextute);
		}
	}

	// �t�@�C�������
	fclose(pFile);

	if (pTextute != nullptr)
	{// NULL�`�F�b�N
		delete[] pTextute;
		pTextute = nullptr;
	}
}

//--------------------------------------------------
// ���f���̓ǂݍ���
//--------------------------------------------------
void LoadModel(FILE* pFile, ETexture* pTexture)
{
	char read[MAX_TEXT] = {};
	int parts = 0;

	while (strcmp(read, "END_MODELSET") != 0)
	{// �I��肪����܂ŌJ��Ԃ�
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// �R�����g
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "NUM_PARTS") == 0)
		{// �p�[�c�̎g�p��
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &s_model.numParts);

			if (s_model.pParts == nullptr)
			{// NULL�`�F�b�N
				s_model.pParts = new SParts[s_model.numParts];
			}
			else
			{
				assert(false);
			}
		}
		else if (strcmp(read, "MOVE") == 0)
		{// �ړ���
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &s_model.moveLoad);
		}
		else if (strcmp(read, "PARTSSET") == 0)
		{// �p�[�c�̏��
			// �p�[�c�̓ǂݍ���
			LoadParts(pFile, pTexture, parts);
			parts++;
		}
	}
}

//--------------------------------------------------
// �p�[�c�̓ǂݍ���
//--------------------------------------------------
void LoadParts(FILE* pFile, ETexture* pTexture, int parts)
{
	char read[MAX_TEXT] = {};
	SParts* pParts = &s_model.pParts[parts];

	while (strcmp(read, "END_PARTSSET") != 0)
	{// �I��肪����܂ŌJ��Ԃ�
		fscanf(pFile, "%s", &read);

		if (strncmp(read, "#", 1) == 0)
		{// �R�����g
			fgets(read, MAX_TEXT, pFile);
			continue;
		}

		if (strcmp(read, "INDEX") == 0)
		{// �g�p����e�N�X�`���̔ԍ�
			int idx = 0;
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &idx);
			pParts->texture = pTexture[idx];
		}
		else if (strcmp(read, "PARENT") == 0)
		{// �e�̔ԍ�
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxParent);
		}
		else if (strcmp(read, "POS") == 0)
		{// �ʒu
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->pos.x);
			fscanf(pFile, "%f", &pParts->pos.y);
			fscanf(pFile, "%f", &pParts->pos.z);
			pParts->posOld = pParts->pos;
		}
		else if (strcmp(read, "ROT") == 0)
		{// ����
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->rot);
			pParts->rotOld = pParts->rot;
		}
		else if (strcmp(read, "WIDTH") == 0)
		{// ��
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->width);
		}
		else if (strcmp(read, "HEIGHT") == 0)
		{// ����
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->height);
		}
		else if (strcmp(read, "PRIORITY") == 0)
		{// �`��̗D�揇��
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxPriority);
		}
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
void Move(void)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetMoveKeyPress(MOVE_KEY_LEFT))
	{// ���L�[�������ꂽ
		vec.x -= 1.0f;
	}
	if (GetMoveKeyPress(MOVE_KEY_RIGHT))
	{// �E�L�[�������ꂽ
		vec.x += 1.0f;
	}
	if (GetMoveKeyPress(MOVE_KEY_UP))
	{// ��L�[�������ꂽ
		vec.y += 1.0f;
	}
	if (GetMoveKeyPress(MOVE_KEY_DOWN))
	{// ���L�[�������ꂽ
		vec.y -= 1.0f;
	}

	if ((vec.x == 0.0f) && (vec.y == 0.0f))
	{// �ړ����ĂȂ�
		return;
	}

	// �x�N�g���̐��K��
	D3DXVec3Normalize(&vec, &vec);

	s_model.rotDest = atan2f(vec.y, vec.x) - (D3DX_PI * 0.5f);
	s_model.move += vec * s_model.moveLoad;

	// �ړ�
	s_model.pos.x += s_model.move.x;
	s_model.pos.y += s_model.move.y;

	// �����E�ړ��ʂ��X�V (����������)
	s_model.move.x += (0.0f - s_model.move.x) * 1.0f;
	s_model.move.y += (0.0f - s_model.move.y) * 1.0f;
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
void Rot(void)
{
	if (s_model.rot == s_model.rotDest)
	{// ��]���ĂȂ�
		return;
	}

	// �p�x�̐��K��
	NormalizeAngle(&s_model.rotDest);

	float rot = s_model.rotDest - s_model.rot;

	// �p�x�̐��K��
	NormalizeAngle(&rot);

	//�����E�������X�V (����������)
	s_model.rot += rot * 0.25f;

	// �p�x�̐��K��
	NormalizeAngle(&s_model.rot);
}
}// namespace�͂����܂�
