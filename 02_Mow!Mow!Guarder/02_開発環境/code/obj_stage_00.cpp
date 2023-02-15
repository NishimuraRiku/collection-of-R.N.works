//========================================
// 
// OBJ:�X�e�[�W[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_stage_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "color.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"			// CHR:�G			[00]
#include "chr_player_00.h"			// CHR:�v���C���[	[00]
#include "eff_shadow_00.h"			// EFF:�e			[00]
#include "obj_core_00.h"			// OBJ:�R�A			[00]
#include "obj_pedestal_00.h"		// OBJ:���			[00]
#include "obj_stage_00.h"			// OBJ:�X�e�[�W		[00]
#include "obj_switch_00.h"			// OBJ:�X�C�b�`		[00]
#include "md_game_00.h"				// MD :�Q�[��		[00]
#include "md_title_00.h"			// MD :�^�C�g��		[00]
#include "ui_attention-mark_00.h"	// UI :���Ӄ}�[�N	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�u���b�N[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_STAGE_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\OBJECT\\OBJ_STAGE_00_TYPE_DATA.txt"

// OBJ:�X�e�[�W[00] �̎�ނ̐�
#define OBJ_STAGE_00_TYPE_NUM	(1)

// OBJ:�X�e�[�W[00] �̏������
#define OBJ_STAGE_00_INIT_STATE	(OBJ_STAGE_00_STATE_INTERVAL)

// OBJ:�X�e�[�W[00] �̃e�L�X�g�̊J�n�ʒu
// OBJ:�X�e�[�W[00] �̃e�L�X�g�̏I���ʒu
// OBJ:�X�e�[�W[00] �̃e�L�X�g�̌����̂ɂ����鎞��
// OBJ:�X�e�[�W[00] �̃e�L�X�g�̏�����̂ɂ����鎞��
// OBJ:�X�e�[�W[00] �̃e�L�X�g�̃N���A���̐F
#define OBJ_STAGE_00_TEXT_START_POS			D3DXVECTOR3(SCREEN_WIDTH*0.5f,(SCREEN_HEIGHT*0.5f)+160.0f,0.0f)
#define OBJ_STAGE_00_TEXT_END_POS			D3DXVECTOR3(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f,0.0f)
#define OBJ_STAGE_00_TEXT_SCALE				(2.0f)
#define OBJ_STAGE_00_TEXT_APPEAR_TIME		(40)
#define OBJ_STAGE_00_TEXT_DISAPPEAR_TIME	(80)
#define OBJ_STAGE_00_TEXT_CLEAR_COLOR		Color{243,191,63,255}

// OBJ:�X�e�[�W[00] �̐ݒ�ʒu�Ƃ̓����蔻��ԍ�
#define OBJ_STAGE_00_HITTEST_WITH_SETPOS	(1)

// OBJ:�X�e�[�W[00] �̕ϐF�ɂ����鎞��
// OBJ:�X�e�[�W[00] �̕ϐF���镔�i
#define OBJ_STAGE_00_DISCOLORATION_TIME		(60)
#define OBJ_STAGE_00_DISCOLORATION_PARTS	(1)

// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�J�E���^�[�̏����l
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�J�E���^�[�̕����l
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�J�E���^�[�̃X�R�A�{��
#define OBJ_STAGE_00_TIME_BONUS_COUNTER_START		(60*60)
#define OBJ_STAGE_00_TIME_BONUS_COUNTER_DIVISION	(60)
#define OBJ_STAGE_00_TIME_BONUS_COUNTER_SCORE_RATE	(50)

// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̊J�n�ʒu
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̏I���ʒu
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̏o������
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̈ړ��ɂ����鎞��
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̏����n�߂鎞��
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̐F
// OBJ:�X�e�[�W[00] �̃^�C���{�[�i�X�e�L�X�g�̓_�ŐF
#define OBJ_STAGE_00_TIME_BONUS_TEXT_START_POS			D3DXVECTOR3(SCREEN_WIDTH+PIXEL* 61,PIXEL*35,0.0f)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_END_POS			D3DXVECTOR3(SCREEN_WIDTH+PIXEL*-61,PIXEL*35,0.0f)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME			(120)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_MOVE_TIME			(40)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME	(80)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_TIME			(2)
#define OBJ_STAGE_00_TIME_BONUS_TEXT_COLOR				Color{255,255,255,255}
#define OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_COLOR		Color{255,119,183,255}

// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̈ʒu
// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̐F
// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̒��ӎ��̐F
// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̕s�����x�̉���
// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̕s�����x�̏��
// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̊g��{���̉���
// OBJ:�X�e�[�W[00] �̃l�N�X�g�E�F�[�u�e�L�X�g�̊g��{���̏��
#define OBJ_STAGE_00_NEXTWAVE_POS					D3DXVECTOR3(SCREEN_CENTER_X,PIXEL*48,0.0f)
#define OBJ_STAGE_00_NEXTWAVE_TEXT_COLOR			Color{251,80,42,255}
#define OBJ_STAGE_00_NEXTWAVE_TEXT_WARNING_COLOR	Color{191,0,191,255}
#define OBJ_STAGE_00_NEXTWAVE_ALPHA_MIN				(0.75f)
#define OBJ_STAGE_00_NEXTWAVE_ALPHA_MAX				(1.0f)
#define OBJ_STAGE_00_NEXTWAVE_SCALE_MIN				(0.95f)
#define OBJ_STAGE_00_NEXTWAVE_SCALE_MAX				(1.0f)

// OBJ:�X�e�[�W[00] �̃J�E���gSE
#define OBJ_STAGE_00_COUNT_SE	(SOUND_LABEL_SE_COUNT_000)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�X�e�[�W[00] �̃p�����[�^�[�̏���������
void InitParameterObj_stage_00(void);

// OBJ:�X�e�[�W[00] �̃Q�[�g�ݒ���̓ǂݍ��ݏ���
// OBJ:�X�e�[�W[00] �̒��Ӄ}�[�N�ݒ���̓ǂݍ��ݏ���
// OBJ:�X�e�[�W[00] �̑���ݒ���̓ǂݍ��ݏ���
// OBJ:�X�e�[�W[00] �̕i�����ݒ���̓ǂݍ��ݏ���
// OBJ:�X�e�[�W[00] �̃E�F�[�u�ݒ���̓ǂݍ��ݏ���
void LoadGateSetObj_stage_00(FILE *pFile, Obj_stage_00GateSet *pGateSet);
void LoadAttentionMarkSetObj_stage_00(FILE *pFile, Obj_stage_00AttentionMarkSet *pAttentionMarkSet);
void LoadPedestalSetObj_stage_00(FILE *pFile, Obj_stage_00PedestalSet *pPedestalSet);
void LoadItemsSetObj_stage_00(FILE *pFile, Obj_stage_00ItemsSet *pItemsSet, Obj_stage_00Type *pObjType);
void LoadWaveSetObj_stage_00(FILE *pFile, Obj_stage_00WaveSet *pWaveSet);

// OBJ:�X�e�[�W[00] �̃Z�b�e�B���O����
void SettingObj_stage_00(void);

// OBJ:�X�e�[�W[00] �̏�Ԃɉ������J�n����
// OBJ:�X�e�[�W[00] �̏�Ԃɉ������X�V����
// OBJ:�X�e�[�W[00] �̏�Ԃɉ������I������
void StartStateObj_stage_00(void);
void UpdateStateObj_stage_00(void);
void EndStateObj_stage_00(void);

// OBJ:�X�e�[�W[00] �̌x���ݒ菈��
void SetWarningObj_stage_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9	g_aTextureObj_stage_00	// OBJ:�X�e�[�W[00] �̃e�N�X�`���ւ̃|�C���^
					[OBJ_STAGE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_aMeshObj_stage_00		// OBJ:�X�e�[�W[00] �̃��b�V��(���_���)�ւ̃|�C���^
					[OBJ_STAGE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
LPD3DXBUFFER		g_pBuffMatObj_stage_00	// OBJ:�X�e�[�W[00] �̃}�e���A���ւ̃|�C���^
					= NULL;
DWORD				g_aNumMatObj_stage_00	// OBJ:�X�e�[�W[00] �̃}�e���A���̐�
					[OBJ_STAGE_00_TYPE_NUM]
					[PARTS_3D_MAX]
					= {};
D3DXMATRIX			g_aMtxWorldObj_stage_00	// OBJ:�X�e�[�W[00] �̃��[���h�}�g���b�N�X
					[PARTS_3D_MAX];
Obj_stage_00		g_obj_stage_00;			// OBJ:�X�e�[�W[00] �̏��
Obj_stage_00Type	g_aObj_stage_00Type		// OBJ:�X�e�[�W[00] �̎�ޖ��̏��
					[OBJ_STAGE_00_TYPE_NUM];

//========== *** OBJ:�X�e�[�W[00] �̏����擾 ***
Obj_stage_00 *GetObj_stage_00(void) 
{
	return &g_obj_stage_00;
}

//========== *** OBJ:�X�e�[�W[00] �̎�ޖ��̏����擾 ***
Obj_stage_00Type *GetObj_stage_00Type(void)
{
	return g_aObj_stage_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_stage_00(void) 
{
	Obj_stage_00	*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
					= &g_obj_stage_00;

	pObj->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pObj->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pObj->nType					= 0;								// ���
	pObj->hitTestInfo			= {};								// �����蔻��̊Ǘ����
	pObj->nCounter				= 0;								// �J�E���^�[
	pObj->state					= OBJ_STAGE_00_STATE_NONE;			// ���
	pObj->nWave					= 0;								// �E�F�[�u�ԍ�
	pObj->nWaveOld				= 0;								// �ߋ��̃E�F�[�u�ԍ�
	pObj->nCntWaveMain			= 0;								// �E�F�[�u�J�E���g(���C��)
	pObj->nCntWaveSub			= 0;								// �E�F�[�u�J�E���g(�T�u)
	pObj->nKill					= 0;								// �L����
	pObj->textState				= OBJ_STAGE_00_TEXT_STATE_HIDE;		// �e�L�X�g�̏��
	pObj->nCounterText			= 0;								// �e�L�X�g�J�E���^�[
	pObj->nCounterDiscoloration = 0;								// �ϐF�J�E���^�[
	pObj->groundCol				= INITCOLOR;						// �n�ʂ̐F
	pObj->partsInfo				= {};								// ���i�Ǘ�
	pObj->setBoss				= {};								// �{�X�ݒ���
	pObj->nTimeBonus			= 0;								// �^�C���{�[�i�X
	pObj->nCounterTimeBonus		= 0;								// �^�C���{�[�i�X�J�E���^�[
	pObj->nCounterTimeBonusText = 0;								// �^�C���{�[�i�X�e�L�X�g�J�E���^�[
}

//========================================
// LoadGateSetObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̃Q�[�g�ݒ���̓ǂݍ��ݏ��� -
//========================================
void LoadGateSetObj_stage_00(FILE *pFile, Obj_stage_00GateSet *pGateSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];

	// �Q�[�g�̐���������
	pGateSet->nGateNum = 0;

	// �����蔻��ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "GATESET_END"))	{ break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "GATE"))			{
			Obj_stage_00Gate	*pGate	// �Q�[�g���̃|�C���^
								= &pGateSet->aGate[pGateSet->nGateNum];

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "GATE_END")) {
					pGateSet->nGateNum++;	// �Q�[�g�̐������Z
					break;					// �ǂݍ��݂��I��
				}
				else if (!strcmp(aDataSearch, "POS:")) {	// �ʒu
					fscanf(pFile, "%f", &pGate->pos.x);		// X
					fscanf(pFile, "%f", &pGate->pos.y);		// Y
					fscanf(pFile, "%f", &pGate->pos.z);		// Z
				}
				else if (!strcmp(aDataSearch, "BOSS_POINT:")) {	// �{�X�ݒ�ʒu
					fscanf(pFile, "%f", &pGate->bossPoint.x);	// X
					fscanf(pFile, "%f", &pGate->bossPoint.y);	// Y
					fscanf(pFile, "%f", &pGate->bossPoint.z);	// Z
				}
				else if (!strcmp(aDataSearch, "SETROT:")) {	// �ݒ����
					fscanf(pFile, "%f", &pGate->setRot.x);	// X
					fscanf(pFile, "%f", &pGate->setRot.y);	// Y
					fscanf(pFile, "%f", &pGate->setRot.z);	// Z
					pGate->setRot *= D3DX_PI;				// �~�����␳
				}
				else if (!strcmp(aDataSearch, "WIDTH:")) { fscanf(pFile, "%f", &pGate->fWidth); }	// ��
				else if (!strcmp(aDataSearch, "DEPTH:")) { fscanf(pFile, "%f", &pGate->fDepth); }	// ���s��
			}
		}
	}
}

//========================================
// LoadAttentionMarkSetObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̒��Ӄ}�[�N�ݒ���̓ǂݍ��ݏ��� -
//========================================
void LoadAttentionMarkSetObj_stage_00(FILE *pFile, Obj_stage_00AttentionMarkSet *pAttentionMarkSet)
{
	char						aDataSearch		// �f�[�^�����p
								[TXT_MAX];
	Obj_stage_00AttentionMark	*pAttentionMark	// ���Ӄ}�[�N�̏��̃|�C���^
								= pAttentionMarkSet->aAttentionMark;

	// �����蔻��ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "ATTENTIONMARKSET_END")) { break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ATTENTIONMARK")) {
			
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "ATTENTIONMARK_END")) {
					pAttentionMark++;	// ���Ӄ}�[�N�̏��̃|�C���^��i�߂�
					break;				// �ǂݍ��݂��I��
				}
				else if (!strcmp(aDataSearch, "POS:")) {			// �ʒu
					fscanf(pFile, "%f", &pAttentionMark->pos.x);	// X
					fscanf(pFile, "%f", &pAttentionMark->pos.y);	// Y
					fscanf(pFile, "%f", &pAttentionMark->pos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "ROT:")) {			// ����
					fscanf(pFile, "%f", &pAttentionMark->rot.x);	// X
					fscanf(pFile, "%f", &pAttentionMark->rot.y);	// Y
					fscanf(pFile, "%f", &pAttentionMark->rot.z);	// Z
					pAttentionMark->rot.x *= D3DX_PI;				// �~�����␳X
					pAttentionMark->rot.y *= D3DX_PI;				// �~�����␳Y
					pAttentionMark->rot.z *= D3DX_PI;				// �~�����␳Z
				}
			}
		}
	}
}

//========================================
// LoadPedestalSetObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̑���ݒ���̓ǂݍ��ݏ��� -
//========================================
void LoadPedestalSetObj_stage_00(FILE *pFile, Obj_stage_00PedestalSet *pPedestalSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];

	// ����̐���������
	pPedestalSet->nPedestalNum = 0;

	// �����蔻��ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "PEDESTALSET_END"))	{ break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "PEDESTAL"))			{
			Obj_stage_00Pedestal	*pPedestal	// ������̃|�C���^
									= &pPedestalSet->aPedestal[pPedestalSet->nPedestalNum];

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "PEDESTAL_END")) {
					pPedestalSet->nPedestalNum++;	// ����̐������Z
					break;							// �ǂݍ��݂��I��
				}
				else if (!strcmp(aDataSearch, "POS:"))	{	// �ʒu
					fscanf(pFile, "%f", &pPedestal->pos.x);	// X
					fscanf(pFile, "%f", &pPedestal->pos.y);	// Y
					fscanf(pFile, "%f", &pPedestal->pos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "TYPE:"))	{ fscanf(pFile, "%f", &pPedestal->nType); }	// ���
			}
		}
	}
}

//========================================
// LoadWaveSetObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̃E�F�[�u�ݒ���̓ǂݍ��ݏ��� -
//========================================
void LoadWaveSetObj_stage_00(FILE *pFile, Obj_stage_00WaveSet *pWaveSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];

	// �E�F�[�u�̐���������
	pWaveSet->nWaveNum = 0;

	// �E�F�[�u�ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "WAVESET_END"))	{ break; }										// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "LOOP_POINT:"))	{ fscanf(pFile, "%d", &pWaveSet->nLoopPoint); }	// ���[�v�n�_
		else if (!strcmp(aDataSearch, "WAVE"))			{
			Obj_stage_00Wave	*pWave	// �E�F�[�u���̃|�C���^
								= &pWaveSet->aWave[pWaveSet->nWaveNum];

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "WAVE_END")) {
					pWaveSet->nWaveNum++;	// �E�F�[�u�̐������Z
					break;					// �ǂݍ��݂��I��
				}
				else if (!strcmp(aDataSearch, "INTERVAL_TIME:")) {
					fscanf(pFile, "%d", &pWave->nIntervalTime);	// �C���^�[�o������
					pWave->nIntervalTime *= 60;					// �b�P�ʂɏC��
				}
				else if (!strcmp(aDataSearch, "INTERVAL_BGM:"))			{ fscanf(pFile, "%d", &pWave->intervalBGM); }		// �C���^�[�o��BGM
				else if (!strcmp(aDataSearch, "RAID_BGM:"))				{ fscanf(pFile, "%d", &pWave->raidBGM); }			// �P��BGM
				else if (!strcmp(aDataSearch, "INTERVAL_GROUND_COLOR:")){ LoadColor(pFile, &pWave->intervalGroundColor); }	// �C���^�[�o�����̒n�ʂ̐F
				else if (!strcmp(aDataSearch, "INTERVAL_SKY_COLOR:"))	{ LoadColor(pFile, &pWave->intervalSkyColor); }		// �C���^�[�o�����̋�̐F
				else if (!strcmp(aDataSearch, "RAID_GROUND_COLOR:"))	{ LoadColor(pFile, &pWave->raidGroundColor); }		// �P�����̒n�ʂ̐F
				else if (!strcmp(aDataSearch, "RAID_SKY_COLOR:"))		{ LoadColor(pFile, &pWave->raidSkyColor); }			// �P�����̋�̐F
				else if (!strcmp(aDataSearch, "WARNING_TEXT:"))			{ fscanf(pFile, "%d", &pWave->bWarningText); }		// ���Ӄe�L�X�g�t���O
				else if (!strcmp(aDataSearch, "MAIN_COUNT:"))			{ fscanf(pFile, "%d", &pWave->bMainCount); }		// ���C���J�E���g�t���O
				else if (!strcmp(aDataSearch, "ENEMYSET"))				{
					Obj_stage_00EnemySet	*pEnemySet	// �G�ݒ���̃|�C���^
											= pWave->aEnemySet;

					pWave->nEnemyNum = 0;	// �G�̐���������

					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if		(!strcmp(aDataSearch, "ENEMYSET_END"))	{ break; }	// �ǂݍ��݂��I��
						else if (!strcmp(aDataSearch, "SET:"))			{	// �ݒ���
							fscanf(pFile, "%d", &pEnemySet->nTime);			// ����
							pEnemySet->nTime *= 60;							// �b�P�ʂɏC��
							fscanf(pFile, "%d", &pEnemySet->nGate);			// �Q�[�g
							fscanf(pFile, "%d", &pEnemySet->nType);			// ���
							pEnemySet++;									// �G�ݒ�������Z
							pWave->nEnemyNum++;								// �G�̐������Z
						}
					}
				}
			}
		}
	}
}

//========================================
// LoadItemsSetObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̕i�����ݒ���̓ǂݍ��ݏ��� -
//========================================
void LoadItemsSetObj_stage_00(FILE *pFile, Obj_stage_00ItemsSet *pItemsSet, Obj_stage_00Type *pObjType)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];

	// �i�����̐���������
	pItemsSet->nItemsNum = 0;

	// �i�����ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "ITEMSSET_END"))	{ break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ITEMS"))			{
			Obj_stage_00Items	*pItems	// �i�������̃|�C���^
								= &pItemsSet->aItems[pItemsSet->nItemsNum];

			// ���i�ԍ���ǂݍ���
			for (int nCntPedestal = 0; nCntPedestal < pObjType->pedestalSet.nPedestalNum; nCntPedestal++)
			{
				fscanf(pFile, "%d", &pItems->aItem[nCntPedestal]);
			}

			pItemsSet->nItemsNum++;	// �i�����������Z
		}
	}
}

//========================================
// SettingObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̃Z�b�e�B���O���� -
//========================================
void SettingObj_stage_00(void) 
{
	Obj_stage_00Type	*pObjType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��
						= g_aObj_stage_00Type;

	for (int nCntPedestal = 0; nCntPedestal < pObjType->pedestalSet.nPedestalNum; nCntPedestal++) 
	{
		// OBJ:���[00] �̐ݒ菈��
		SetObj_pedestal_00(
			pObjType->pedestalSet.aPedestal[nCntPedestal].pos,
			pObjType->pedestalSet.aPedestal[nCntPedestal].nType);
	}

	for (int nCntGate = 0; nCntGate < pObjType->gateSet.nGateNum; nCntGate++)
	{
		// UI:���Ӄ}�[�N[00] �̐ݒ菈��
		SetUi_attentionMark_00(
			pObjType->attentionMarkSet.aAttentionMark[nCntGate].pos,
			pObjType->attentionMarkSet.aAttentionMark[nCntGate].rot);
	}

	// OBJ:�R�A[00] �̈ʒu����
	GetObj_core_00()->pos = pObjType->coreSetPos;

	// OBJ:�X�C�b�`[00] �̈ʒu����
	GetObj_switch_00()->pos = pObjType->switchSetPos;
}

//========================================
// SetStateObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateObj_stage_00(OBJ_STAGE_00_STATE state) 
{
	Obj_stage_00	*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
					= &g_obj_stage_00;

	// ��Ԃɉ������I������
	EndStateObj_stage_00();

	// ��Ԃ���
	pObj->state = state;

	// ��Ԃɉ������J�n����
	StartStateObj_stage_00();
}

//========================================
// StartStateObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̏�Ԃɉ������J�n���� -
//========================================
void StartStateObj_stage_00(void)
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	switch (pObj->state)
	{
	case /*/ �C���^�[�o�� /*/OBJ_STAGE_00_STATE_INTERVAL: {
		if ((pObj->nCntWaveMain > 0) ||
			(pObj->nCntWaveSub > 0))
		{// �E�F�[�u�J�E���g(���C��or�T�u)��0�������Ă��鎞�A
			// �e�L�X�g�̏�Ԃ����ꒆ�ɂ���
			pObj->textState = OBJ_STAGE_00_TEXT_STATE_APPEAR;
		}
		pObj->nCounterDiscoloration = 0;	// �ϐF�J�E���^�[��������
		pObj->nCounterTimeBonusText = 0;	// �^�C���{�[�i�X�e�L�X�g�J�E���^�[��������

		// �C���^�[�o��BGM���Đ�
		PlaySound(pType->waveSet.aWave[pObj->nWave].intervalBGM);

		break;
	}
	case /*/ �P�� /*/OBJ_STAGE_00_STATE_RAID: {
		pObj->nCounter = 0;									// �J�E���^�[��������
		pObj->nKill = 0;									// �L������������
		pObj->textState = OBJ_STAGE_00_TEXT_STATE_APPEAR;	// �e�L�X�g�̏�Ԃ����ꒆ�ɂ���
		pObj->nCounterText = 0;								// �e�L�X�g�J�E���^�[��������
		pObj->nCounterDiscoloration = 0;					// �ϐF�J�E���^�[��������
		pObj->nCounterTimeBonus								// �^�C���{�[�i�X�J�E���^�[��ݒ�
			= OBJ_STAGE_00_TIME_BONUS_COUNTER_START;

		// �P��BGM���Đ�
		PlaySound(pType->waveSet.aWave[pObj->nWave].raidBGM);

		break;
	}
	case /*/ �N���A /*/OBJ_STAGE_00_STATE_CLEAR: {
		StopSoundType(SOUND_TYPE_BGM);		// BGM���~
		pObj->nCounterDiscoloration = 0;	// �ϐF�J�E���^�[��������
		pObj->nTimeBonus = 0;				// �^�C���{�[�i�X�擾
		break;
	}
	case /*/ ��~ /*/OBJ_STAGE_00_STATE_STOP: {

		break;
	}
	}
}

//========================================
// EndStateObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̏�Ԃɉ������I������ -
//========================================
void EndStateObj_stage_00(void)
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	switch (pObj->state)
	{
	case /*/ �C���^�[�o�� /*/OBJ_STAGE_00_STATE_INTERVAL: {

		break;
	}
	case /*/ �P�� /*/OBJ_STAGE_00_STATE_RAID: {
		pObj->nCounter = 0;									// �J�E���^�[��������
		GetUi_attentionMark_00Control()->nCounterBrink = 0;	// UI:���Ӄ}�[�N[00] �̓_�ŃJ�E���^�[��������
		pObj->nCounterText = 0;								// �e�L�X�g�J�E���^�[��������

		if (GetObj_core_00()->nHP > 0)
		{// OBJ:�R�A[00] ��HP��0�������Ă��鎞�A
			// �^�C���{�[�i�X�擾
			pObj->nTimeBonus = (int)(pObj->nCounterTimeBonus / OBJ_STAGE_00_TIME_BONUS_COUNTER_DIVISION) * OBJ_STAGE_00_TIME_BONUS_COUNTER_SCORE_RATE;

			// �^�C���{�[�i�X���X�R�A�ɉ��Z
			GetChr_player_00()->nScore += pObj->nTimeBonus;
		}
		else 
		{// OBJ:�R�A[00] ��HP��0�ȉ��̎��A
			// �^�C���{�[�i�X��������
			pObj->nTimeBonus = 0;
		}

		break;
	}
	case /*/ �N���A /*/OBJ_STAGE_00_STATE_CLEAR: {
		
		break;
	}
	case /*/ ��~ /*/OBJ_STAGE_00_STATE_STOP: {

		break;
	}
	}
}

//========================================
// UpdateStateObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̏�Ԃɉ������X�V���� -
//========================================
void UpdateStateObj_stage_00(void) 
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];
	char				aString	// ������
						[TXT_MAX];
	Color				setCol;	// �ݒ�F

	switch (pObj->state)
	{
	case /*/ �C���^�[�o�� /*/OBJ_STAGE_00_STATE_INTERVAL: {
		Obj_stage_00Wave	*pWave	// �E�F�[�u�̏��̃|�C���^
							= &pType->waveSet.aWave[pObj->nWave];

		if (GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL)
		{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�̎��A
			if (++pObj->nCounter >= pWave->nIntervalTime)
			{// �J�E���^�[�����Z�������ʃC���^�[�o���^�C���ɒB�������A
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_RAID);	// ��Ԃ��P���ɐݒ�
			}

			if (pObj->nCounter % 60 == 0) 
			{// �J�E���g�̃^�C�~���O�ɒB�������A
				float fRate = (float)pObj->nCounter / (float)pWave->nIntervalTime;
				SoundVolumeChange(OBJ_STAGE_00_COUNT_SE, fRate * fRate);
				PlaySound(OBJ_STAGE_00_COUNT_SE);	// �J�E���gSE���Đ�
			}

			if (pObj->textState != OBJ_STAGE_00_TEXT_STATE_HIDE)
			{// �e�L�X�g�������Ă����ԂłȂ����A
				sprintf(aString, "WAVE CLEAR");			// �������ݒ�
				setCol = OBJ_STAGE_00_TEXT_CLEAR_COLOR;	// �ݒ�F��ݒ�
			}
		}
	}
		break;
	case /*/ �P�� /*/OBJ_STAGE_00_STATE_RAID: {
		Obj_stage_00Wave	*pWave	// �E�F�[�u�̏��̃|�C���^
							= &pType->waveSet.aWave[pObj->nWave];

		for (int nCntEnemySet = 0; nCntEnemySet < pWave->nEnemyNum; nCntEnemySet++)
		{
			if (pObj->nCounter == pWave->aEnemySet[nCntEnemySet].nTime) 
			{// �J�E���^�[�ƓG�̐ݒ莞�Ԃ���v���Ă��鎞�A
				Obj_stage_00Gate	*pGate =	// �Q�[�g���̃|�C���^
									&pType->gateSet.aGate[pWave->aEnemySet[nCntEnemySet].nGate];
				D3DXVECTOR3			setPos =	// �ݒ�ʒu
									pGate->pos;

				switch (GetChr_enemy_00Type()[pWave->aEnemySet[nCntEnemySet].nType].rank)
				{
				case /*/ ��� /*/CHR_ENEMY_00_RANK_LOWER:
					setPos = pGate->pos;	// �ݒ�ʒu���Q�[�g�̈ʒu�ɐݒ�

					// �ݒ�ʒu�ɗ�����K�p
					setPos.x = setPos.x - (pGate->fWidth * 0.5f) + fRand(pGate->fWidth);
					setPos.z = setPos.z - (pGate->fDepth * 0.5f) + fRand(pGate->fDepth);

					// CHR:�G[00] �̐ݒ菈��
					SetChr_enemy_00(setPos, pGate->setRot, pWave->aEnemySet[nCntEnemySet].nType);
					break;
				case /*/ �{�X /*/CHR_ENEMY_00_RANK_BOSS:
					// �{�X�ݒ�������ꂼ����(�\��)
					pObj->setBoss.pos	= pGate->bossPoint;
					pObj->setBoss.rot	= pGate->setRot;
					pObj->setBoss.nType = pWave->aEnemySet[nCntEnemySet].nType;

					// MD:�Q�[�����[00] �̏�Ԃ��{�X�o�����[�r�[(���_�ړ�)�ɐݒ�
					SetMd_game_00State(MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE);
					break;
				}
			}
		}

		pObj->nCounter++;	// �J�E���^�[�����Z

		if ((pObj->nKill >= pWave->nEnemyNum) && (AllyChr_enemy_00Count() == 0))
		{// �L�������G�̐��ɒB���� & CHR:�G[00] �̖����J�E���g��0�̎��A
			// �ߋ��̃E�F�[�u��ۑ�
			pObj->nWaveOld = pObj->nWave;

			if (++pObj->nWave >= pType->waveSet.nWaveNum) 
			{// �E�F�[�u�ԍ������Z�������ʃE�F�[�u���ȏ�̎��A
				pObj->nWave--;	// �E�F�[�u�ԍ������Z
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_CLEAR);	// ��Ԃ��N���A�ɐݒ�
				SetMd_game_00State(MD_GAME_00_STATE_CLEAR);		// MD:�Q�[�����[00] �̏�Ԃ��N���A�ɐݒ�
			}
			else 
			{// �E�F�[�u�ԍ������Z�������ʃE�F�[�u�������̎��A
				SetWarningObj_stage_00();		// �x���ݒ菈��
				ItemsChangeObj_pedestal_00();	// �i������ύX����

				if (GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bMainCount)
				{// ���݂̃E�F�[�u�̃��C���J�E���g�t���O���^�̎��A
					pObj->nCntWaveMain++;	// �E�F�[�u�J�E���g(���C��)�����Z
					pObj->nCntWaveSub = 0;	// �E�F�[�u�J�E���g(�T�u)��������
				}
				else
				{// ���݂̃E�F�[�u�̃��C���J�E���g�t���O���U�̎��A
					// �E�F�[�u�J�E���g(�T�u)�����Z
					pObj->nCntWaveSub++;
				}

				// ��Ԃ��C���^�[�o���ɐݒ�
				SetStateObj_stage_00(OBJ_STAGE_00_STATE_INTERVAL);
			}
		}

		if (pObj->textState != OBJ_STAGE_00_TEXT_STATE_HIDE)
		{// �e�L�X�g�������Ă����ԂłȂ����A
			sprintf(aString, "WAVE %d-%d",	// �������ݒ�
				pObj->nCntWaveMain + 1,
				pObj->nCntWaveSub + 1);
			setCol = { 255,255,255,255 };	// �ݒ�F��ݒ�
		}

		pObj->nCounterTimeBonus--;	// �^�C���{�[�i�X�J�E���^�[�����Z����
		IntControl(&pObj->nCounterTimeBonus, INT_MAX, 0);
	}
		break;
	case /*/ �N���A /*/OBJ_STAGE_00_STATE_CLEAR: {

		break;
	}
	case /*/ ��~ /*/OBJ_STAGE_00_STATE_STOP: {
		
		break;
	}
	}

	if (pObj->textState != OBJ_STAGE_00_TEXT_STATE_HIDE)
	{// �e�L�X�g�������Ă����ԂłȂ����A
		float		fAlpha;	// �s�����x
		D3DXVECTOR3 setPos;	// �ݒ�ʒu

		switch (pObj->textState)
		{
		case /*/ ���ꒆ /*/OBJ_STAGE_00_TEXT_STATE_APPEAR: {
			float fRate	// ����
				= (float)pObj->nCounterText / (float)OBJ_STAGE_00_TEXT_APPEAR_TIME;

			fAlpha = fRate;	// �s�����x��ݒ�
			setPos =		// �ݒ�ʒu��ݒ�
				(OBJ_STAGE_00_TEXT_START_POS * (1.0f - fRate)) +
				(OBJ_STAGE_00_TEXT_END_POS * fRate);

			if (++pObj->nCounterText >= OBJ_STAGE_00_TEXT_APPEAR_TIME)
			{// �e�L�X�g�J�E���^�[�����Z�������ʌ����̂ɂ����鎞�ԂɒB�������A
				pObj->textState = OBJ_STAGE_00_TEXT_STATE_DISAPPEAR;	// �e�L�X�g�̏�Ԃ��������ɂ���
				pObj->nCounterText = 0;									// �e�L�X�g�J�E���^�[��������
			}
		}
		break;
		case /*/ ������ /*/OBJ_STAGE_00_TEXT_STATE_DISAPPEAR: {
			float fRate	// ����
				= (float)pObj->nCounterText / (float)OBJ_STAGE_00_TEXT_DISAPPEAR_TIME;

			fAlpha = 1.0f - fRate;				// �s�����x��ݒ�
			setPos = OBJ_STAGE_00_TEXT_END_POS;	// �ݒ�ʒu��ݒ�

			if (++pObj->nCounterText >= OBJ_STAGE_00_TEXT_DISAPPEAR_TIME)
			{// �e�L�X�g�J�E���^�[�����Z�������ʏ�����̂ɂ����鎞�ԂɒB�������A
				pObj->textState = OBJ_STAGE_00_TEXT_STATE_HIDE;	// �e�L�X�g�̏�Ԃ������Ă����Ԃɂ���
				pObj->nCounterText = 0;							// �e�L�X�g�J�E���^�[��������
			}
		}
		break;
		}

		// �E�F�[�u�e�L�X�g�̐ݒ�
		{
			setCol.a = 255 * fAlpha;
			SetText2D(
				aString,
				FONT_003,
				DISPLAY_CENTER,
				setPos,
				setCol,
				OBJ_STAGE_00_TEXT_SCALE, OBJ_STAGE_00_TEXT_SCALE);
		}
	}

	if (pObj->nTimeBonus > 0)
	{// �^�C���{�[�i�X��0�������Ă��鎞�A
		pObj->nCounterTimeBonusText++;	// �^�C���{�[�i�X�e�L�X�g�J�E���^�[�����Z����
		IntControl(&pObj->nCounterTimeBonusText, OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME, 0);

		{
			float fMoveRate =
				(float)IntControlReturn(
					pObj->nCounterTimeBonusText,
					OBJ_STAGE_00_TIME_BONUS_TEXT_MOVE_TIME, 0) /
					(float)OBJ_STAGE_00_TIME_BONUS_TEXT_MOVE_TIME;
			float fClearRate =
				(float)(IntControlReturn(pObj->nCounterTimeBonusText,
					OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME,
					OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME) -
					OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME) /
					(float)(OBJ_STAGE_00_TIME_BONUS_TEXT_POP_TIME - OBJ_STAGE_00_TIME_BONUS_TEXT_CLEAR_START_TIME);
			setCol = (pObj->nCounterTimeBonusText % (OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_TIME * 2) < OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_TIME) ?
				OBJ_STAGE_00_TIME_BONUS_TEXT_COLOR : OBJ_STAGE_00_TIME_BONUS_TEXT_BLINK_COLOR;
			setCol.a *= (1.0f - fClearRate) * fMoveRate;
			sprintf(aString, "TIME BONUS +%d", pObj->nTimeBonus);
			SetText2D(
				aString,
				FONT_002,
				DISPLAY_RIGHT,
				(OBJ_STAGE_00_TIME_BONUS_TEXT_START_POS * (1.0f - fMoveRate)) +
				(OBJ_STAGE_00_TIME_BONUS_TEXT_END_POS * fMoveRate),
				setCol,
				1.0f, 1.0f);
		}
	}
}

//========================================
// SetWarningObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̌x���ݒ菈�� -
//========================================
void SetWarningObj_stage_00(void) 
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];
	Obj_stage_00Wave	*pWave	// �E�F�[�u�̏��̃|�C���^
						= &pType->waveSet.aWave[pObj->nWave];

	bool	aWarning	// �x���t���O
			[OBJ_STAGE_00_GATE_MAX]
			= { false };

	for (int nCntEnemySet = 0; nCntEnemySet < pWave->nEnemyNum; nCntEnemySet++)
	{
		aWarning[pWave->aEnemySet[nCntEnemySet].nGate] = true;	// �x���t���O��^�ɂ���
	}

	// UI:���Ӄ}�[�N�̉��t���O�Ɍx���t���O�𔽉f����
	for (int nCntGate = 0; nCntGate < pType->gateSet.nGateNum; nCntGate++)
	{
		GetUi_attentionMark_00()[nCntGate].bShow = aWarning[nCntGate];
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_stage_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_stage_00Type	*pObjType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��
						= g_aObj_stage_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_STAGE_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// �ǂݍ��݂��I��
		if (aDataSearch[0] == '#')			{ continue; }				// �܂�Ԃ�

		if (!strcmp(aDataSearch, "TYPE")) {
			// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; } // �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "CORE_SET_POS:"))	{	// �ʒu
					fscanf(pFile, "%f", &pObjType->coreSetPos.x);	// X
					fscanf(pFile, "%f", &pObjType->coreSetPos.y);	// Y
					fscanf(pFile, "%f", &pObjType->coreSetPos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "SWITCH_SET_POS:")) {	// �ʒu
					fscanf(pFile, "%f", &pObjType->switchSetPos.x);	// X
					fscanf(pFile, "%f", &pObjType->switchSetPos.y);	// Y
					fscanf(pFile, "%f", &pObjType->switchSetPos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "GATESET"))			{ LoadGateSetObj_stage_00(pFile, &pObjType->gateSet); }						// �Q�[�g�ݒ���
				else if (!strcmp(aDataSearch, "ATTENTIONMARKSET"))	{ LoadAttentionMarkSetObj_stage_00(pFile, &pObjType->attentionMarkSet); }	// ���Ӄ}�[�N�ݒ���
				else if (!strcmp(aDataSearch, "PEDESTALSET"))		{ LoadPedestalSetObj_stage_00(pFile, &pObjType->pedestalSet); }				// ����ݒ���
				else if (!strcmp(aDataSearch, "ITEMSSET"))			{ LoadItemsSetObj_stage_00(pFile, &pObjType->itemsSet, pObjType); }			// �i�����ݒ���
				else if (!strcmp(aDataSearch, "WAVESET"))			{ LoadWaveSetObj_stage_00(pFile, &pObjType->waveSet); }						// �E�F�[�u�ݒ���
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }							// �����蔻��ݒ���
				else if (!strcmp(aDataSearch, "PARTSSET"))			{ LoadParts3DSet(pFile, &pObjType->partsSet); }								// ���i�ݒ���
			}
		}
	}
}

//========================================
// InitObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̏��������� -
//========================================
void InitObj_stage_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^
	
	for (int nCntType = 0; nCntType < OBJ_STAGE_00_TYPE_NUM; nCntType++)
	{
		Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
							= &g_aObj_stage_00Type[nCntType];

		for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
		{
			Parts3DType *pPartsType	// ���i(3D)�̎�ޖ��̏��\����
						= &pType->partsSet.aPartsType[nCntParts];

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				pPartsType->aModelPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_pBuffMatObj_stage_00,
				NULL,
				&g_aNumMatObj_stage_00[nCntType][nCntParts],
				&g_aMeshObj_stage_00[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObj_stage_00->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_stage_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureObj_stage_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	// ���i(3D)�̃g�����X�t�H�[����������
	InitParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	// �p�����[�^�[�̏���������
	InitParameterObj_stage_00();

	if (GetMode() == MODE_GAME_00)
	{// ���[�h��MD:�Q�[�����[00] �̎��A
		// �Z�b�e�B���O����
		SettingObj_stage_00();

		// �x���ݒ菈��
		SetWarningObj_stage_00();

		// ��Ԃ�ݒ�
		SetStateObj_stage_00(OBJ_STAGE_00_INIT_STATE);
	}
}

//========================================
// UninitObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̏I������ -
//========================================
void UninitObj_stage_00(void)
{
	// ���b�V���̔j��
	for (int nCntType = 0; nCntType < OBJ_STAGE_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			if (g_aMeshObj_stage_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshObj_stage_00[nCntType][nCntParts]->Release();
				g_aMeshObj_stage_00[nCntType][nCntParts] = NULL;
			}
		}
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_stage_00 != NULL)
	{
		g_pBuffMatObj_stage_00->Release();
		g_pBuffMatObj_stage_00 = NULL;
	}
}

//========================================
// UpdateObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̍X�V���� -
//========================================
void UpdateObj_stage_00(void)
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	// ���i(3D)�̃g�����X�t�H�[�����擾
	GetParts3DTransform(&pObj->partsInfo, &pType->partsSet);

	{
		float	fRate		// �J�E���^�[�̐i�s��
				= (float)pObj->nCounterDiscoloration / (float)OBJ_STAGE_00_DISCOLORATION_TIME;

		if (GetMode() == MODE_TITLE_00)
		{// ���[�h��MD:�^�C�g�����[00] �̎��A
			pObj->groundCol = pType->waveSet.aWave[0].intervalGroundColor;
			GetMain()->bgCol = pType->waveSet.aWave[0].intervalSkyColor;
		}
		else if (
			pObj->state == OBJ_STAGE_00_STATE_INTERVAL ||
			pObj->state == OBJ_STAGE_00_STATE_CLEAR)
		{// ��Ԃ��C���^�[�o�� or �N���A�̎��A
			pObj->groundCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWaveOld].raidGroundColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalGroundColor, fRate));
			GetMain()->bgCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWaveOld].raidSkyColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalSkyColor, fRate));
		}
		else if (pObj->state == OBJ_STAGE_00_STATE_RAID)
		{// ��Ԃ��P���̎��A
			pObj->groundCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalGroundColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].raidGroundColor, fRate));
			GetMain()->bgCol = ColorAdd(
				ColorRate(pType->waveSet.aWave[pObj->nWave].intervalSkyColor, 1.0f - fRate),
				ColorRate(pType->waveSet.aWave[pObj->nWave].raidSkyColor, fRate));
		}
	}

	if (GetMode() == MODE_TITLE_00) 
	{// ���[�h��MD:�^�C�g�����[00] �̎��A
		pObj->rot += MD_TITLE_00_CAMERA3D_BG_SPIN;	// ��]��������
		RotControl(&pObj->rot);						// �����𐧌�

		return;	// �������I������
	}

	// ��ԏ���
	UpdateStateObj_stage_00();

	if (pObj->nCounterDiscoloration < OBJ_STAGE_00_DISCOLORATION_TIME) 
	{// �ϐF�J�E���^�[���ϐF�ɂ����鎞�Ԃ�������Ă��鎞�A
		pObj->nCounterDiscoloration++;	// �ϐF�J�E���^�[�����Z
	}

	if (GetMd_game_00()->state == MD_GAME_00_STATE_NORMAL)
	{// MD:�Q�[�����[00] �̏�Ԃ��ʏ�̎��A
		if (pObj->state == OBJ_STAGE_00_STATE_INTERVAL)
		{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̎��A
			// �J�E���^�[�̐i�s��
			float fRate = (float)(pObj->nCounter % 60) / 60.0f;

			// �F���J�E���^�[�̐i�s���ɉ����ē����x���Z�o�������̂ɐݒ�
			Color col = GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
				OBJ_STAGE_00_NEXTWAVE_TEXT_WARNING_COLOR:
				OBJ_STAGE_00_NEXTWAVE_TEXT_COLOR;
			col.a *= (OBJ_STAGE_00_NEXTWAVE_ALPHA_MIN + ((OBJ_STAGE_00_NEXTWAVE_ALPHA_MAX - OBJ_STAGE_00_NEXTWAVE_ALPHA_MIN) * (1.0f - fRate)));

			// �g��{�����J�E���^�[�̐i�s���ɉ����ĎZ�o
			float fScale = (OBJ_STAGE_00_NEXTWAVE_SCALE_MIN + ((OBJ_STAGE_00_NEXTWAVE_SCALE_MAX - OBJ_STAGE_00_NEXTWAVE_SCALE_MIN) * (1.0f - fRate)));

			char aString[TXT_MAX];	// ������
			sprintf(aString, "NEXT.%d", (GetObj_stage_00Type()->waveSet.aWave[pObj->nWave].nIntervalTime - pObj->nCounter - 1) / 60);
			SetText2D(
				aString,
				FONT_003,
				DISPLAY_CENTER,
				OBJ_STAGE_00_NEXTWAVE_POS,
				col, fScale, fScale);
		}
	}
}

//========================================
// DrawObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̕`�揈�� -
//========================================
void DrawObj_stage_00(void)
{
	Obj_stage_00		*pObj				// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX			mtxSelf;			// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_stage_00->GetBufferPointer();

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pObj->rot.y, pObj->rot.x, pObj->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// �{�̂̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pObj->pos.x, pObj->pos.y, pObj->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// ���i(3D)�̏��̃|�C���^
					= &pObj->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// �e�}�g���b�N�X

		// ���i�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtxWorldObj_stage_00[nCntParts]);

		// ���i�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_stage_00[nCntParts], &g_aMtxWorldObj_stage_00[nCntParts], &mtxRot);

		// ���i�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldObj_stage_00[nCntParts], &g_aMtxWorldObj_stage_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1)
		{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
			mtxParent = g_aMtxWorldObj_stage_00[pPartsType->nParent];
		}
		else
		{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
			mtxParent = mtxSelf;
		}

		// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(
			&g_aMtxWorldObj_stage_00[nCntParts],
			&g_aMtxWorldObj_stage_00[nCntParts],
			&mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldObj_stage_00[nCntParts]);

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatObj_stage_00[pObj->nType][nCntParts]; nCntMat++)
		{
			if (nCntParts == OBJ_STAGE_00_DISCOLORATION_PARTS) 
			{// �J�E���g���ϐF���镔�i�̎��A
				// �}�e���A���̐ݒ�
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D, pObj->groundCol);
			}
			else 
			{// �J�E���g���ϐF���镔�i�łȂ����A
				// �}�e���A���̐ݒ�
				SetMaterial(pDevice, &pMat[nCntMat].MatD3D, INITCOLOR);
			}

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureObj_stage_00[pObj->nType][nCntParts][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshObj_stage_00[pObj->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// CollisionObj_stage_00�֐� - OBJ:�X�e�[�W[00] �Ƃ̓����蔻�菈�� -
//========================================
void CollisionObj_stage_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionCheck(
		vector,
		pObjCollision,
		myCollInfo,
		{
			&pObj->pos,
			pObj->pos,
			NULL,
			&pObj->rot,
			pObj->rot,
			pType->hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
		},
		COLLCHK_MODE_LOOKDOWN);

	if (pObjCollision != NULL)
	{// �Փˏ��NULL�Ŗ������A
		// ���ʂ̏Փˏ����㏑��
		CollisionOverwrite(pCmnCollision, *pObjCollision);
	}
}

//========================================
// CollisionObj_stage_00WithSetPos�֐� - OBJ:�X�e�[�W[00] �Ƃ̓����蔻�菈��(With�ݒ�ʒu) -
//========================================
void CollisionObj_stage_00WithSetPos(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	Obj_stage_00		*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
						= &g_obj_stage_00;
	Obj_stage_00Type	*pType	// OBJ:�X�e�[�W[00] �̎�ޖ��̏��̃|�C���^
						= &g_aObj_stage_00Type[pObj->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionCheck(
		vector,
		pObjCollision,
		myCollInfo,
		{
			&pObj->pos,
			pObj->pos,
			NULL,
			&pObj->rot,
			pObj->rot,
			pType->hitTestSet.aHitTest[OBJ_STAGE_00_HITTEST_WITH_SETPOS]
		},
		COLLCHK_MODE_LOOKDOWN);
}

//========================================
// BossSetObj_stage_00�֐� - OBJ:�X�e�[�W[00] �̃{�X�ݒ菈�� -
//========================================
void BossSetObj_stage_00(void)
{
	Obj_stage_00	*pObj	// OBJ:�X�e�[�W[00] �̏��̃|�C���^
					= &g_obj_stage_00;

	// CHR:�G[00] �̃{�X�ݒ菈��
	SetBossChr_enemy_00(pObj->setBoss.pos, pObj->setBoss.rot, pObj->setBoss.nType);
}