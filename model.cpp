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

#include <stdio.h>
#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const char*	FILE_NAME = "data/TEXT/motion.txt";	// �t�@�C����
const int	MAX_TEXT = 1024;					// �e�L�X�g�̍ő吔
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
void LoadModel(FILE* pFile);
void LoadParts(FILE* pFile, int parts);
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
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void)
{
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawModel(void)
{
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
			LoadModel(pFile);
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
void LoadModel(FILE* pFile)
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
			LoadParts(pFile, parts);
			parts++;
		}
	}
}

//--------------------------------------------------
// �p�[�c�̓ǂݍ���
//--------------------------------------------------
void LoadParts(FILE* pFile, int parts)
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
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxTexture);
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
		}
		else if (strcmp(read, "ROT") == 0)
		{// ����
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->rot.x);
			fscanf(pFile, "%f", &pParts->rot.y);
			fscanf(pFile, "%f", &pParts->rot.z);
		}
		else if (strcmp(read, "SIZE") == 0)
		{// �T�C�Y
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%f", &pParts->size.x);
			fscanf(pFile, "%f", &pParts->size.y);
			fscanf(pFile, "%f", &pParts->size.z);
		}
		else if (strcmp(read, "PRIORITY") == 0)
		{// �`��̗D�揇��
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%d", &pParts->idxPriority);
		}
	}
}
}// namespace�͂����܂�
