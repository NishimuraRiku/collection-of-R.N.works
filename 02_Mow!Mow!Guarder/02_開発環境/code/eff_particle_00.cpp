//========================================
// 
// EFF:�p�[�e�B�N��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_particle.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "eff_light_00.h"		// EFF:��			[00]
#include "eff_particle_00.h"	// EFF:�p�[�e�B�N��	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// EFF:�p�[�e�B�N��[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define EFF_PARTICLE_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\EFFECT\\EFF_PARTICLE_00_TYPE_DATA.txt"

// EFF:�p�[�e�B�N��[00] �̍ő吔
// EFF:�p�[�e�B�N��[00] �̎�ނ̍ő吔
#define EFF_PARTICLE_00_MAX			(256)
#define EFF_PARTICLE_00_TYPE_MAX	(8)

// EFF:�p�[�e�B�N��[00] �̖@���x�N�g��
// EFF:�p�[�e�B�N��[00] �̑��Έʒu
#define EFF_PARTICLE_00_NOR				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define EFF_PARTICLE_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//****************************************
// �\���̂̒�`
//****************************************
// EFF:�p�[�e�B�N��[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	
	// ���ފ֘A
	int	nType;	// ���

	// ��Ԋ֘A
	bool	bUse;		// �g�p����Ă��邩�t���O
	int		nLife;		// ����
	int		nLifeTemp;	// ���̎���
}Eff_particle_00;

// EFF:�p�[�e�B�N��[00] �̎�ޖ��̏��\����
typedef struct 
{
	int			nLightType;	// ���̎��
	int			nSetMin;	// �ݒ萔�̉���
	int			nSetMax;	// �ݒ萔�̏��
	int			nLifeMin;	// �����̉���
	int			nLifeMax;	// �����̏��
	D3DXVECTOR3 moveMin;	// �ړ��ʂ̉���
	D3DXVECTOR3 moveMax;	// �ړ��ʂ̏��
}Eff_particle_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:�p�[�e�B�N��[00] �̃p�����[�^�[�̏���������
void InitParameterEff_particle_00(Eff_particle_00 *pEff);

//****************************************
// �O���[�o���錾
//****************************************
Eff_particle_00		g_aEff_particle_00		// EFF:�p�[�e�B�N��[00] �̏��
					[EFF_PARTICLE_00_MAX];
Eff_particle_00Type	g_aEff_particle_00Type	// EFF:�p�[�e�B�N��[00] �̎�ޖ��̏��
					[EFF_PARTICLE_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_particle_00�֐� - EFF:�p�[�e�B�N��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterEff_particle_00(Eff_particle_00 *pEff)
{
	pEff->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pEff->move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	pEff->nType		= 0;								// ���
	pEff->bUse		= false;							// �g�p����Ă��邩�t���O
	pEff->nLife		= 0;								// ����
	pEff->nLifeTemp = 0;								// ���̎���
}

//========================================
// DestroyEff_particle_00�֐� - EFF:�p�[�e�B�N��[00] �̔j�󏈗� -
//========================================
void DestroyEff_particle_00(Eff_particle_00 *pEff) 
{
	pEff->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadEff_particle_00�֐� - EFF:�e[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadEff_particle_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Eff_particle_00Type	*pEffType	// EFF:�e[00] �̎�ޖ��̏��
						= g_aEff_particle_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(EFF_PARTICLE_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
	 //�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END")) { fclose(pFile); break; }	// �ǂݍ��݂��I��
		if (aDataSearch[0] == '#') { continue; }				// �܂�Ԃ�

		if (!strcmp(aDataSearch, "TYPE")) {
			// EFF:�e[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pEffType++; break; }							// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LIGHT_TYPE:"))	{ fscanf(pFile, "%d", &pEffType->nLightType); }	// ���̎��
				else if (!strcmp(aDataSearch, "SET_MIN:"))		{ fscanf(pFile, "%d", &pEffType->nSetMin); }	// 
				else if (!strcmp(aDataSearch, "SET_MAX:"))		{ fscanf(pFile, "%d", &pEffType->nSetMax); }	// 
				else if (!strcmp(aDataSearch, "LIFE_MIN:"))		{ fscanf(pFile, "%d", &pEffType->nLifeMin); }	// 
				else if (!strcmp(aDataSearch, "LIFE_MAX:"))		{ fscanf(pFile, "%d", &pEffType->nLifeMax); }	// 
				else if (!strcmp(aDataSearch, "MOVE_MIN:"))		{	// �ړ��ʂ̉���
					fscanf(pFile, "%f", &pEffType->moveMin.x);		// X
					fscanf(pFile, "%f", &pEffType->moveMin.y);		// Y
					fscanf(pFile, "%f", &pEffType->moveMin.z);		// Z
				}
				else if (!strcmp(aDataSearch, "MOVE_MAX:")) {		// �ړ��ʂ̏��
					fscanf(pFile, "%f", &pEffType->moveMax.x);		// X
					fscanf(pFile, "%f", &pEffType->moveMax.y);		// Y
					fscanf(pFile, "%f", &pEffType->moveMax.z);		// Z
				}
			}
		}
	}
}

//========================================
// InitEff_particle_00�֐� - EFF:�p�[�e�B�N��[00] �̏��������� -
//========================================
void InitEff_particle_00(void)
{
	Eff_particle_00	*pEff	// EFF:�p�[�e�B�N��[00] �̏��̃|�C���^
					= g_aEff_particle_00;

	for (int nCntEff = 0; nCntEff < EFF_PARTICLE_00_MAX; nCntEff++, pEff++)
	{
		// �p�����[�^�[�̏���������
		InitParameterEff_particle_00(&g_aEff_particle_00[nCntEff]);
	}
}

//========================================
// UninitEff_particle_00�֐� - EFF:�p�[�e�B�N��[00] �̏I������ -
//========================================
void UninitEff_particle_00(void)
{
	
}

//========================================
// UpdateEff_particle_00�֐� - EFF:�p�[�e�B�N��[00] �̍X�V���� -
//========================================
void UpdateEff_particle_00(void)
{
	Eff_particle_00	*pEff	// EFF:�p�[�e�B�N��[00] �̏��̃|�C���^
					= g_aEff_particle_00;
	
	for (int nCntEff = 0; nCntEff < EFF_PARTICLE_00_MAX; nCntEff++, pEff++)
	{
		if (!pEff->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}
		else if (--pEff->nLife <= 0) 
		{// ���������Z��������0�ȉ��̎��A
			pEff->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
			continue;			// �J��Ԃ�������܂�Ԃ�
		}

 		pEff->pos.x += (pEff->move.x * ((float)pEff->nLife / (float)pEff->nLifeTemp));
		pEff->pos.y += (pEff->move.y * ((float)pEff->nLife / (float)pEff->nLifeTemp));
		pEff->pos.z += (pEff->move.z * ((float)pEff->nLife / (float)pEff->nLifeTemp));

		// EFF:��[00] �̐ݒ菈��
		SetEff_light_00(pEff->pos, pEff->nType);
	}
}

//========================================
// SetEff_particle_00�֐� - EFF:�p�[�e�B�N��[00] �̐ݒ菈�� -
//========================================
void SetEff_particle_00(D3DXVECTOR3 pos, int nType)
{
	Eff_particle_00		*pEff	// EFF:�p�[�e�B�N��[00] �̏��̃|�C���^
						= g_aEff_particle_00;
	Eff_particle_00Type	*pType	// EFF:�p�[�e�B�N��[00] �̎�ޖ��̏��̃|�C���^
						= &g_aEff_particle_00Type[nType];

	int nSetNum = pType->nSetMin + (rand() % (pType->nSetMin - pType->nSetMin + 1));	// �ݒ萔�𗐐��Őݒ�

	if (nSetNum <= 0) 
	{// �ݒ萔��0�ȉ��̎��A
		// �������I������
		return;
	}

	for (int nCntEff = 0; nCntEff < EFF_PARTICLE_00_MAX; nCntEff++, pEff++)
	{
		if (pEff->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pEff->pos = pos;					// �ʒu����
		pEff->nType = pType->nLightType;	// ��ނ���
		pEff->bUse = true;					// �g�p����Ă����Ԃɂ���

		// �����𗐐��Őݒ�
		pEff->nLife = pType->nLifeMin + (rand() % (pType->nLifeMax - pType->nLifeMin));
		pEff->nLifeTemp = pEff->nLife;

		// �ړ��ʂ𗐐��Őݒ�
		pEff->move.x = pType->moveMin.x + fRand(pType->moveMax.x - pType->moveMin.x);
		pEff->move.y = pType->moveMin.y + fRand(pType->moveMax.y - pType->moveMin.y);
		pEff->move.z = pType->moveMin.z + fRand(pType->moveMax.z - pType->moveMin.z);

		if (--nSetNum <= 0)
		{// �ݒ萔�����Z��������0�ȉ��̎��A
			// �J��Ԃ������𔲂���
			break;
		}
	}
}