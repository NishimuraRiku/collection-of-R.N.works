//========================================
// 
// �V�X�e��:�X�e�[�W[00] �̏���
// Author:���� ����
// 
//========================================
// *** sys_stage_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "atk_bullet_00.h"
#include "bg_color.h"
#include "chr_fighter_00.h"
#include "chr_enemy_00.h"
#include "md_game_00.h"
#include "sys_stage_00.h"
#include "obj_plasma_block_00.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************

// �X�e�[�W[00] �̏��̃f�[�^�t�@�C���̑��΃p�X
// �z�u�Ԋu
// �ϐF�ɂ����鎞��
#define SYS_STAGE_00_STAGE_DATA_FILE_PATH	"data\\GAMEDATA\\STAGE_DATA.txt"
#define POSITIONING_SPACE					(PIXEL * 16)
#define CHANGE_COLOR_TIME					(60)

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterSys_stage_00(void);

// �z�u���̃p�����[�^�[�̏���������
void InitSetParameterSys_stage_00(void);

// �G�̔z�u����
void SetEnemy(void);

// �T�E���h�؂�ւ�����
void SoundChange(void);

// �X�e�[�W�̏��̓ǂݍ��ݏ���
void LoadStage(void);

// �E�F�[�u�̏��̓ǂݍ��ݏ���
void LoadWave(void);

// �X�e�[�W������
void StageSearch(FILE *pFile);

// �E�F�[�u������
void WaveSearch(FILE *pFile);

//****************************************
// �O���[�o���錾
//****************************************
Sys_stage_00 g_sys_stage_00;	// �X�e�[�W[00] �̏��

//========== *** �V�X�e��:�X�e�[�W[00] �̏����擾 ***
Sys_stage_00 *GetSys_stage_00(void)
{
	return &g_sys_stage_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterSys_stage_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterSys_stage_00(void) 
{
	g_sys_stage_00.nStageNum		= 0;		// �X�e�[�W�ԍ�
	g_sys_stage_00.nWaveMax			= 0;		// �E�F�[�u�̍ő吔
	g_sys_stage_00.nWaveCount		= -1;		// �E�F�[�u�̃J�E���g
	g_sys_stage_00.nDispWaveCount	= 0;		// �\������E�F�[�u�̃J�E���g
	g_sys_stage_00.nWaveTime		= 0;		// �E�F�[�u�̎���
	g_sys_stage_00.nWaveEndTime		= 0;		// �E�F�[�u�̏I������
	g_sys_stage_00.bWaveUp			= false;	// �E�F�[�u�𑝂₷���ۂ�
	g_sys_stage_00.nLastWave		= 0;		// �ŏI�E�F�[�u
	g_sys_stage_00.nLastDispWave	= 0;		// �ŏI�\���E�F�[�u
	g_sys_stage_00.nLastScore		= 0;		// �ŏI�X�R�A
	g_sys_stage_00.nLastBomb		= 0;		// �ŏI�{���ێ���
	g_sys_stage_00.nEnemySetNum		= 0;		// �G�̔z�u��

	// �z�u���̃p�����[�^�[�̏���������
	InitSetParameterSys_stage_00();

	g_sys_stage_00.sound	= (SOUND_LABEL)-1;	// �T�E���h�ԍ�
	g_sys_stage_00.soundNow	= (SOUND_LABEL)-1;	// ���ݍĐ����̃T�E���h�ԍ�
}

//========================================
// InitSetParameterSys_stage_00�֐� - �z�u���̃p�����[�^�[�̏��������� -
//========================================
void InitSetParameterSys_stage_00(void)
{
	for (int nCntSet = 0; nCntSet < ENEMY_SET_MAX; nCntSet++)
	{
		g_sys_stage_00.aEnemySet[nCntSet].nTime			= -1;
		g_sys_stage_00.aEnemySet[nCntSet].nType			= 0;
		g_sys_stage_00.aEnemySet[nCntSet].setPos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_sys_stage_00.aEnemySet[nCntSet].setRot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_sys_stage_00.aEnemySet[nCntSet].bAreaBoss		= false;
		g_sys_stage_00.aEnemySet[nCntSet].bBomb			= false;
		g_sys_stage_00.aEnemySet[nCntSet].nParentNum	= -1;
		g_sys_stage_00.aEnemySet[nCntSet].nChildNum		= -1;
	}
}

//========================================
// SetEnemy�֐� - �G�̔z�u���� -
//========================================
void SetEnemy(void)
{
	Sys_stage_00	*pSys	// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;

	for (int nCntSet = 0; nCntSet < pSys->nEnemySetNum; nCntSet++)
	{
		if (pSys->nWaveTime == pSys->aEnemySet[nCntSet].nTime)
		{// �E�F�[�u�̎��Ԃ��G��z�u���鎞�Ԃƈ�v�������A
			// �G[00] �̔z�u����
			SetChr_enemy_00(
				pSys->aEnemySet[nCntSet].setPos,
				pSys->aEnemySet[nCntSet].setRot,
				pSys->aEnemySet[nCntSet].nType,
				pSys->aEnemySet[nCntSet].bAreaBoss,
				pSys->aEnemySet[nCntSet].bBomb,
				pSys->aEnemySet[nCntSet].nParentNum,
				pSys->aEnemySet[nCntSet].nChildNum);
		}
	}

	// �E�F�[�u�̎��Ԃ����Z
	pSys->nWaveTime++;
}

//========================================
// SoundChange�֐� - �T�E���h�؂�ւ����� -
//========================================
void SoundChange(void) 
{
	Sys_stage_00	*pSys
					= &g_sys_stage_00;	// �X�e�[�W[00] �̏��

	if (pSys->sound != pSys->soundNow)
	{// �T�E���h���Đ����̃T�E���h�ƈقȂ鎞�A
		if (pSys->soundNow != -1)
		{// �Đ����̃T�E���h�ԍ���-1�łȂ����A
			// �Đ����̃T�E���h���~
			StopSound((SOUND_LABEL)pSys->soundNow);
		}
		if (pSys->sound != -1)
		{// �T�E���h�ԍ���-1�łȂ����A
			// �T�E���h���Đ�
			PlaySound((SOUND_LABEL)pSys->sound);
		}

		// �Đ����̃T�E���h��ݒ�
		pSys->soundNow = pSys->sound;
	}
}

//========================================
// LoadStage�֐� - �X�e�[�W�̏��̓ǂݍ��ݏ��� -
//========================================
void LoadStage(void)
{
	FILE			*pFile;				// �t�@�C���|�C���^
	char			aDataSearch			//
					[TXT_MAX];			// �f�[�^�����p
	Sys_stage_00	*pSys				// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;	//

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(SYS_STAGE_00_STAGE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// �X�e�[�W������
	StageSearch(pFile);

	// �E�F�[�u�̍ő吔��������
	pSys->nWaveMax = 0;

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if (strcmp(aDataSearch, "STAGE_END") == 0)
		{// �t�@�C�������
			fclose(pFile);
			// �J��Ԃ������𔲂���
			break;
		}
		else if (strcmp(aDataSearch, "WAVE") == 0)
		{// �E�F�[�u�̍ő吔�����Z
			pSys->nWaveMax++;
		}
	}
}

//========================================
// LoadWave�֐� - �E�F�[�u�̏��̓ǂݍ��ݏ��� -
//========================================
void LoadWave(void)
{
	FILE			*pFile;				// �t�@�C���|�C���^
	char			aDataSearch			// �f�[�^�����p
					[TXT_MAX];			//
	Sys_stage_00	*pSys				// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;	//

	// �z�u���̃p�����[�^�[�̏���������
	InitSetParameterSys_stage_00();

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(SYS_STAGE_00_STAGE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// �X�e�[�W������
	StageSearch(pFile);

	// �E�F�[�u������
	WaveSearch(pFile);

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if (strcmp(aDataSearch, "WAVE_END") == 0)
		{// �t�@�C�������
			fclose(pFile);
			// �J��Ԃ������𔲂���
			break;
		}//====================//
		else if (strcmp(aDataSearch, "�E�F�[�u�^�C��:") == 0)
		{// �E�F�[�u�̏I�����Ԃ�ǂݍ���
			fscanf(pFile, "%d", &pSys->nWaveEndTime);
		}//====================//
		else if (strcmp(aDataSearch, "�v���Y�}�E�H�[���̏��Y:") == 0)
		{// �v���Y�}�u���b�N[00] �̏��Y��ǂݍ���
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetTop);

			// �ʒu�𐳂����l�ɍX�V
			GetObj_plasma_block_00_square()->fTargetTop *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "�v���Y�}�E�H�[���̉���Y:") == 0)
		{// �v���Y�}�u���b�N[00] �̉���Y��ǂݍ���
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetBottom);

			// �ʒu�𐳂����l�ɍX�V
			GetObj_plasma_block_00_square()->fTargetBottom *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "�v���Y�}�E�H�[���̍���X:") == 0)
		{// �v���Y�}�u���b�N[00] �̍���X��ǂݍ���
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetLeft);

			// �ʒu�𐳂����l�ɍX�V
			GetObj_plasma_block_00_square()->fTargetLeft *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "�v���Y�}�E�H�[���̉E��X:") == 0)
		{// �v���Y�}�u���b�N[00] �̉E��X��ǂݍ���
			fscanf(pFile, "%f", &GetObj_plasma_block_00_square()->fTargetRight);

			// �ʒu�𐳂����l�ɍX�V
			GetObj_plasma_block_00_square()->fTargetRight *= POSITIONING_SPACE;
		}//====================//
		else if (strcmp(aDataSearch, "�E�F�[�u���𑝂₷���ۂ�:") == 0)
		{// �E�F�[�u���𑝂₷���ۂ���ǂݍ���
			int nBTemp;	// �ꎞ�ۑ��p

			fscanf(pFile, "%d", &nBTemp);

			pSys->bWaveUp = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "�G���A���Ԃ̕\��:") == 0)
		{// �G���A���Ԃ̕\���t���O��ǂݍ���
			int nBTemp;	// �ꎞ�ۑ��p

			fscanf(pFile, "%d", &nBTemp);

			pSys->bShowAreaTime = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "�Z�[�u�n�_:") == 0)
		{// �Z�[�u�n�_�t���O��ǂݍ���
			int nBTemp;	// �ꎞ�ۑ��p

			fscanf(pFile, "%d", &nBTemp);

			pSys->bSavePoint = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "�x��:") == 0)
		{// �x���t���O��ǂݍ���
			int nBTemp;	// �ꎞ�ۑ��p

			fscanf(pFile, "%d", &nBTemp);

			pSys->bWarning = nBTemp;
		}//====================//
		else if (strcmp(aDataSearch, "�T�E���h:") == 0)
		{// �T�E���h��ǂݍ���
			fscanf(pFile, "%s", aDataSearch); // ����

			// �T�E���h�̎�ނ𕶎��񂩂�ǂݍ���
			StringLoadSound(aDataSearch, &pSys->sound);
		}//====================//
		else if (strcmp(aDataSearch, "�w�i�F:") == 0)
		{// �w�i�F��ǂݍ���
			Color bgColor;	// �w�i�F

			fscanf(pFile, "%d", &bgColor.nR);
			fscanf(pFile, "%d", &bgColor.nG);
			fscanf(pFile, "%d", &bgColor.nB);
			fscanf(pFile, "%d", &bgColor.nA);

			// �w�i�F�̐ݒ菈��
			SetBg_color(bgColor, pSys->nWaveCount == 0 ? 0 : CHANGE_COLOR_TIME);
		}//====================//
		else if (strcmp(aDataSearch, "FLOW") == 0)
		{// �G�̔z�u��������������
			pSys->nEnemySetNum = 0;

			// �t���[�̕������ʎq��������܂œG�̔z�u���̓ǂݍ��݂��J��Ԃ�
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (strcmp(aDataSearch, "FLOW_END") == 0)
				{// �J��Ԃ������𔲂���
					break;
				}//====================//
				else if (strcmp(aDataSearch, "�z�u:") == 0)
				{// �z�u���鎞�Ԃ�ǂݍ���
					fscanf(pFile, "%d", &pSys->aEnemySet[pSys->nEnemySetNum].nTime);

					fscanf(pFile, "%s", aDataSearch); // ����

					if (strcmp(aDataSearch, "[") == 0)
					{// �z�u���̓ǂݍ��݂��J�n����
						while (1)
						{
							fscanf(pFile, "%s", aDataSearch); // ����

							if (strcmp(aDataSearch, "]") == 0)
							{// �J��Ԃ������𔲂���
								break;
							}//====================//
							else if (strcmp(aDataSearch, "���:") == 0)
							{// ��ނ�ǂݍ���
								fscanf(pFile, "%s", aDataSearch); // ����

								Chr_enemy_00Type	*pChrType	// �G[00] �̎�ޖ��̏��
													= GetChr_enemy_00Type();

								for (int nCntChrType = 0; nCntChrType < CHR_ENEMY_00_TYPE_MAX; nCntChrType++, pChrType++)
								{
									if (strcmp(aDataSearch, pChrType->aLoadName) == 0)
									{// �ǂݍ��ݖ�����v������
										// ��ނ�ݒ�
										pSys->aEnemySet[pSys->nEnemySetNum].nType = nCntChrType;
										break;
									}
								}
							}//====================//
							else if (strcmp(aDataSearch, "X:") == 0)
							{// �ݒu����ʒu��X���W��ǂݍ���
								fscanf(pFile, "%f", &pSys->aEnemySet[pSys->nEnemySetNum].setPos.x);

								// �ʒu�𐳂����l�ɍX�V
								pSys->aEnemySet[pSys->nEnemySetNum].setPos.x *= POSITIONING_SPACE;
							}//====================//
							else if (strcmp(aDataSearch, "Y:") == 0)
							{// �ݒu����ʒu��Y���W��ǂݍ���
								fscanf(pFile, "%f", &pSys->aEnemySet[pSys->nEnemySetNum].setPos.y);

								// �ʒu�𐳂����l�ɍX�V
								pSys->aEnemySet[pSys->nEnemySetNum].setPos.y *= POSITIONING_SPACE;
							}//====================//
							else if (strcmp(aDataSearch, "�p�x:") == 0)
							{// �ݒu����p�x��ǂݍ���
								fscanf(pFile, "%f", &pSys->aEnemySet[pSys->nEnemySetNum].setRot.z);

								// �p�x�𐳂����l�ɕϊ�
								pSys->aEnemySet[pSys->nEnemySetNum].setRot.z *= D3DX_PI;
							}//====================//
							else if (strcmp(aDataSearch, "�G���A�{�X") == 0)
							{// �G���A�{�X�t���O��^�ɂ���
								pSys->aEnemySet[pSys->nEnemySetNum].bAreaBoss = true;
							}//====================//
							else if (strcmp(aDataSearch, "�{��") == 0)
							{// ���e�����t���O��^�ɂ���
								pSys->aEnemySet[pSys->nEnemySetNum].bBomb = true;
							}//====================//
							else if (strcmp(aDataSearch, "�e:") == 0)
							{// �e�ԍ���ǂݍ���
								fscanf(pFile, "%d", &pSys->aEnemySet[pSys->nEnemySetNum].nParentNum);
							}//====================//
							else if (strcmp(aDataSearch, "�q:") == 0)
							{// �q���ԍ���ǂݍ���
								fscanf(pFile, "%d", &pSys->aEnemySet[pSys->nEnemySetNum].nChildNum);
							}//====================//
						}
					}

					// �G�̔z�u�������Z
					pSys->nEnemySetNum++;
				}
			}
		}//====================//
	}
}

//========================================
// StageSearch�֐� - �X�e�[�W������ -
//========================================
void StageSearch(FILE *pFile)
{
	int				nCntStage = 0;		// �X�e�[�W�̃J�E���g
	char			aDataSearch			// �f�[�^�����p
					[TXT_MAX];			//
	Sys_stage_00	*pSys				// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;	//

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if (strcmp(aDataSearch, "STAGE") == 0)
		{// �����p�̕�����[�X�e�[�W] ���ʎq�ƈ�v�������A
			if (nCntStage == pSys->nStageNum)
			{// �X�e�[�W�̃J�E���g���X�e�[�W�ԍ��ƈ�v���Ă������A
				// �J��Ԃ������𔲂���
				break;
			}
			else
			{// �X�e�[�W�̃J�E���g���X�e�[�W�ԍ��ƈ�v���Ă��Ȃ����A
				// �X�e�[�W�̃J�E���g�����Z
				nCntStage++;
			}
		}
	}
}

//========================================
// WaveSearch�֐� - �E�F�[�u������ -
//========================================
void WaveSearch(FILE *pFile)
{
	int				nCntWave			// �E�F�[�u�̃J�E���g
					= 0;				//
	char			aDataSearch			// �f�[�^�����p
					[TXT_MAX];			//
	Sys_stage_00	*pSys				// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;	//

	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if (strcmp(aDataSearch, "WAVE") == 0)
		{// �����p�̕�����[�E�F�[�u] ���ʎq�ƈ�v�������A
			if (nCntWave == pSys->nWaveCount)
			{// �E�F�[�u�̃J�E���g���E�F�[�u�ԍ��ƈ�v���Ă������A
				// �J��Ԃ������𔲂���
				break;
			}
			else
			{// �E�F�[�u�̃J�E���g���E�F�[�u�ԍ��ƈ�v���Ă��Ȃ����A
				// �E�F�[�u�̃J�E���g�����Z
				nCntWave++;
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSys_stage_00�֐� - �V�X�e��:�X�e�[�W[00] �̏��������� -
//========================================
void InitSys_stage_00(int nStageNum)
{
	// �p�����[�^�[��������
	InitParameterSys_stage_00();

	// �X�e�[�W�ԍ�����
	g_sys_stage_00.nStageNum = nStageNum;

	// �X�e�[�W�̏���ǂݍ���
	LoadStage();

	// �E�F�[�u�̐��ڏ���
	WaveTrendSys_stage_00();
}

//========================================
// UninitSys_stage_00�֐� - �V�X�e��:�X�e�[�W[00] �̏I������ -
//========================================
void UninitSys_stage_00(void) 
{

}

//========================================
// UpdateSys_stage_00�֐� - �V�X�e��:�X�e�[�W[00] �̍X�V���� -
//========================================
void UpdateSys_stage_00(void) 
{
	Sys_stage_00	*pSys	// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;

	if (GetGameState() == MD_GAME_00_STATE_NORMAL)
	{// �Q�[���̏�Ԃ��ʏ�̎��A
		// �G�̔z�u����
		SetEnemy();
	}

	if ((pSys->nWaveTime >= pSys->nWaveEndTime)
		&& (pSys->nWaveEndTime != -1))
	{// �E�F�[�u�̎��Ԃ��E�F�[�u�̏I�����ԂɒB���A�������łȂ����A
		// �E�F�[�u�̐��ڏ���
		WaveTrendSys_stage_00();
	}
}

//========================================
// WaveTrendSys_stage_00�֐� - �E�F�[�u�̐��ڏ��� -
//========================================
void WaveTrendSys_stage_00(void) 
{
	// �G[00] ��S�ď�������
	RemoveAllChr_enemy_00();

	// �G�̒e[00] �̑S�j�󏈗�
	AllDestructionAtk_bullet_00(ATK_PARENT_TYPE_ENEMY);

	Sys_stage_00	*pSys	// �X�e�[�W[00] �̏��
					= &g_sys_stage_00;

	// �E�F�[�u�̃J�E���g�����Z
	pSys->nWaveCount++;

	// �E�F�[�u�̎��Ԃ�������
	pSys->nWaveTime = 0;

	// �E�F�[�u���𑝂₷���ۂ���������
	pSys->bWaveUp = false;

	if (pSys->nWaveCount >= pSys->nWaveMax)
	{// �E�F�[�u�̃J�E���g���E�F�[�u�̍ő吔�ɒB�������A
		// �Q�[���N���A�Ɉڍs
		SetMd_game_00State(MD_GAME_00_STATE_GAME_CLEAR);

		// �������I��
		return;
	}

	// �E�F�[�u�̏���ǂݍ���
	LoadWave();

	// �T�E���h�؂�ւ�����
	SoundChange();

	if (pSys->bSavePoint)
	{// �Z�[�u�n�_�t���O���^�̎��A
		// ���݂̏�Ԃ�ۑ�
		pSys->nLastWave		= pSys->nWaveCount;
		pSys->nLastDispWave = pSys->nDispWaveCount;
		pSys->nLastScore	= GetChr_fighter_00()->nScore;
		pSys->nLastBomb		= GetChr_fighter_00()->nBomb;
	}

	if (pSys->bWaveUp)
	{// �E�F�[�u���𑝂₷���ۂ����^�̎��A
		// �\������E�F�[�u�̃J�E���g�����Z
		pSys->nDispWaveCount++;

		// �Q�[���̏�Ԃ��E�F�[�u�҂��ɐݒ�
		SetMd_game_00State(MD_GAME_00_STATE_WAVE_WAIT);

		// �v���Y�}�u���b�N[00] �̈ړ��ʂ�ݒ�
		SetMoveObj_plasma_block_00();
	}
	else if (pSys->bWarning) 
	{// �x���t���O���^�̎��A
		// �Q�[���̏�Ԃ��x���ɐݒ�
		SetMd_game_00State(MD_GAME_00_STATE_WARNING);
	}
	else 
	{// �E�F�[�u���𑝂₳���x���t���O���U�̎��A
		// �v���Y�}�u���b�N[00] �̈ړ��ʂ�ݒ�
		SetMoveObj_plasma_block_00();
	}
}

//========================================
// ContinueSys_stage_00�֐� - �V�X�e��:�X�e�[�W[00] �̃R���e�B�j���[���� -
//========================================
void ContinueSys_stage_00(void) 
{
	Sys_stage_00	*pSys					// �X�e�[�W[00] �̏��̃|�C���^
					= &g_sys_stage_00;		// 
	Chr_fighter_00	*pChr					// �@��[00] �̏��̃|�C���^
					= GetChr_fighter_00();	// 

	// �ŏI�ۑ����̏�Ԃɂ���
	pSys->nWaveCount		= pSys->nLastWave - 1;
	pSys->nDispWaveCount	= pSys->nLastDispWave;
	pChr->nScore			= pSys->nLastScore;
	pChr->nBomb				= pSys->nLastBomb;

	// �E�F�[�u�̐��ڏ���
	WaveTrendSys_stage_00();
}