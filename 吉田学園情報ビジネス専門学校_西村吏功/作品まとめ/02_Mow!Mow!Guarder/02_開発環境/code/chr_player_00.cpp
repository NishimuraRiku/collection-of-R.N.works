//========================================
// 
// CHR:�v���C���[[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_player_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "specific.h"
#include "sound.h"
#include "text.h"
#include "eff_screen_00.h"		// EFF:�X�N���[��	[00]
#include "eff_shadow_00.h"		// EFF:�e			[00]
#include "md_game_00.h"			// MD :�Q�[�����	[00]
#include "atk_bullet_00.h"		// ATK:�e			[00]
#include "atk_sword_00.h"		// ATK:��			[00]
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "itm_coin_00.h"		// ITM:�R�C��		[00]
#include "obj_block_00.h"		// OBJ:�u���b�N		[00]
#include "obj_core_00.h"		// OBJ:�R�A			[00]
#include "obj_discharger_00.h"	// OBJ:���d���u		[00]
#include "obj_mirror_00.h"		// OBJ:�~���[		[00]
#include "obj_signboard_00.h"	// OBJ:�Ŕ�			[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include "obj_switch_00.h"		// OBJ:�X�C�b�`		[00]
#include "obj_turret_00.h"		// OBJ:�^���b�g		[00]
#include "ui_damage_00.h"		// UI :�_���[�W		[00]
#include "ui_tips_00.h"			// UI :TIPS			[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// CHR:�v���C���[[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define CHR_PLAYER_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\CHR_PLAYER_00_TYPE_DATA.txt"

// CHR:�v���C���[[00] �̎�ނ̐�
#define CHR_PLAYER_00_TYPE_NUM	(1)

// CHR:�v���C���[[00] �̉�]�{��
// CHR:�v���C���[[00] �̗͂̌����{��
// CHR:�v���C���[[00] �̈ړ��ʂ̌����{��
// CHR:�v���C���[[00] �̎a�����ɉ����͂̔{��
#define CHR_PLAYER_00_ROT_DIAMETER			(0.5f)
#define CHR_PLAYER_00_FORCE_DAMP			(0.8f)
#define CHR_PLAYER_00_MOVE_DAMP				(0.8f)
#define CHR_PLAYER_00_SLASH_FORCE_DIAMETER	(2.0f)

// CHR:�v���C���[[00] �̏�������
// CHR:�v���C���[[00] �̏����I���A�C�e��
// CHR:�v���C���[[00] �̏��������R�C��
#define CHR_PLAYER_00_INIT_ROT			D3DXVECTOR3(0.0f,D3DX_PI,0.0f)
#define CHR_PLAYER_00_INIT_SELECT_ITEM	(CHR_PLAYER_00_ITEM_SWORD)
#define CHR_PLAYER_00_INIT_COIN			(100)

// CHR:�v���C���[[00] �̒����_�̑��Έʒu
// CHR:�v���C���[[00] �̑_���Ă��鎞�̒����_�̑��Έʒu
// CHR:�v���C���[[00] �̑_���Ă��鎞�̒����_�̑��Έʒu�̈ړ��ɂ����鎞��
// CHR:�v���C���[[00] �̒����_�̑��Έʒu�̈ړ��{��
#define CHR_PLAYER_00_RELATIVE_POS_R				(D3DXVECTOR3(0.0f,15.0f,0.0f))
#define CHR_PLAYER_00_AIM_RELATIVE_POS_R			(D3DXVECTOR3(0.0f,20.0f,0.0f))
#define CHR_PLAYER_00_AIM_TIME						(30)
#define CHR_PLAYER_00_RELATIVE_POS_R_MOVE_DIAMETER	(0.25f)

// CHR:�v���C���[[00] �̖��G����
// CHR:�v���C���[[00] �̖��G���̓_�łɂ����鎞��
// CHR:�v���C���[[00] �̕�����̖��G����
// CHR:�v���C���[[00] �̃_���[�W����
// CHR:�v���C���[[00] �̃_���[�W�F
// CHR:�v���C���[[00] �̃_���[�W���̐U��
#define CHR_PLAYER_00_INVINCIBLE_TIME				(10)
#define CHR_PLAYER_00_INVINCIBLE_BLINK_TIME			(2)
#define CHR_PLAYER_00_AFTER_RESPAWN_INVINCIBLE_TIME	(60)
#define CHR_PLAYER_00_DAMAGE_TIME					(10)
#define CHR_PLAYER_00_DAMAGE_COLOR					Color{166,0,0,0}
#define CHR_PLAYER_00_DAMAGE_VIBRATION				(4.0f)

// CHR:�v���C���[[00] ��OBJ:�~���[[00] �Ƃ̓����蔻��ԍ�
// CHR:�v���C���[[00] �̐ݒ�ʒu�̓����蔻��ԍ�
#define CHR_PLAYER_00_HITTEST_WITH_OBJ_MIRROR_00	(1)
#define CHR_PLAYER_00_HITTEST_SET_POS				(2)

// CHR:�v���C���[[00] �̐ݒu������
#define CHR_PLAYER_00_SET_DIRECTION	(8)

// CHR:�v���C���[[00] �̃C���x���g���I��SE
// CHR:�v���C���[[00] �̍w��SE
// CHR:�v���C���[[00] �̐ݒuSE
#define CHR_PLAYER_00_INVENTORY_SELECT_SE	(SOUND_LABEL_SE_SELECT_000)
#define CHR_PLAYER_00_BUY_SE				(SOUND_LABEL_SE_BUY_000)
#define CHR_PLAYER_00_SET_SE				(SOUND_LABEL_SE_SET_000)

// CHR:�v���C���[[00] �̕����ɂ����鎞�Ԃ̏����l
// CHR:�v���C���[[00] �̕����ɂ����鎞�Ԃ̉��Z�l
// CHR:�v���C���[[00] �̕����e�L�X�g�̈ʒu
#define CHR_PLAYER_00_INIT_RESPAWN_TIME		(60*5)
#define CHR_PLAYER_00_ADD_RESPAWN_TIME		(60*0)
#define CHR_PLAYER_00_RESPAWN_HP_RATE		(0.25f)
#define CHR_PLAYER_00_RESPAWN_TEXT_POS		D3DXVECTOR3(BUFFER_WIDTH*0.5f,BUFFER_HEIGHT*0.5f,0.0f)
#define CHR_PLAYER_00_RESPAWN_TEXT_COLOR	Color{ 219,43,0,255 }

//****************************************
// �v���g�^�C�v�錾
//****************************************
// CHR:�v���C���[[00] �̃p�����[�^�[�̏���������
// CHR:�v���C���[[00] �̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterChr_player_00(void);
void InitParameterChr_player_00Control(void);

// CHR:�v���C���[[00] �̏�Ԑݒ菈��
// CHR:�v���C���[[00] �̏�Ԃɉ������J�n����
// CHR:�v���C���[[00] �̏�Ԃɉ������X�V����
// CHR:�v���C���[[00] �̏�Ԃɉ������I������
void SetChr_player_00State(CHR_PLAYER_00_STATE state);
void StartStateChr_player_00(void);
void UpdateStateChr_player_00(void);
void EndStateChr_player_00(void);

// CHR:�v���C���[[00] �̃L�[���͏���
// CHR:�v���C���[[00] �̃L�[�{�[�h�ɂ��ړ�����
// CHR:�v���C���[[00] �̃X�e�B�b�N�ɂ��ړ�����
// CHR:�v���C���[[00] �̈ړ�����
// CHR:�v���C���[[00] �̈ʒu�X�V����
// CHR:�v���C���[[00] �̃J����(3D)�̒����_�ݒ菈��
// CHR:�v���C���[[00] �̏Փ˃`�F�b�N����
// CHR:�v���C���[[00] �̏Փˏ��ɉ���������
// CHR:�v���C���[[00] �̐ݒ�ʒu�̏Փ˃`�F�b�N����
void KeyInputChr_player_00(void);
void MoveChr_player_00ByKeyboard(DIRECTION drct);
void MoveChr_player_00ByStick(void);
void MoveChr_player_00(float fMove);
void UpdatePosChr_player_00(void);
void SetCamera3DPosRChr_player_00(void);
void CheckCollisionChr_player_00(VECTOR vector);
void CollisionProcessChr_player_00(void);
void CheckCollisionChr_player_00SetPos(void);

// CHR:�v���C���[[00] �̃��[�V�����ݒ菈��
void MotionSetChr_player_00(void);

// CHR:�v���C���[[00] �̃A�C�e���g�p����
void ItemUsingChr_player_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_player_00		// CHR:�v���C���[[00] �̃e�N�X�`���ւ̃|�C���^
						[CHR_PLAYER_00_TYPE_NUM]
						[PARTS_3D_MAX]
						[MATERIAL_3D_MAX]
						= {};
LPD3DXMESH				g_aMeshChr_player_00		// CHR:�v���C���[[00] �̃��b�V��(���_���)�ւ̃|�C���^
						[CHR_PLAYER_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
LPD3DXBUFFER			g_pBuffMatChr_player_00[PARTS_3D_MAX]= {};	// CHR:�v���C���[[00] �̃}�e���A���ւ̃|�C���^
DWORD					g_aNumMatChr_player_00		// CHR:�v���C���[[00] �̃}�e���A���̐�
						[CHR_PLAYER_00_TYPE_NUM]
						[PARTS_3D_MAX]
						= {};
D3DXMATRIX				g_aMtxWorldChr_player_00	// CHR:�v���C���[[00] �̃��[���h�}�g���b�N�X
						[PARTS_3D_MAX];
Chr_player_00			g_chr_player_00;			// CHR:�v���C���[[00] �̏��
Chr_player_00Control	g_chr_player_00Control;		// CHR:�v���C���[[00] �̊Ǘ����
Chr_player_00Type		g_aChr_player_00Type		// CHR:�v���C���[[00] �̎�ޖ��̏��
						[CHR_PLAYER_00_TYPE_NUM];

// CHR:�v���C���[[00] �̃A�C�e�����̏��
const Chr_player_00Item g_aChr_player_00Item[CHR_PLAYER_00_ITEM_MAX] =
{
	{ false , false , false , "SWORD"	},
	{ false , true  , false , "GUN" },
	{ true  , false , true  , "BLOCK" },
	{ true  , false , true  , "TURRET" },
	{ true  , false , true  , "REFLECTOR" },
	{ true  , false , true  , "DISCHARGER" },
};

//========== *** CHR:�v���C���[[00] �̏����擾 ***
Chr_player_00 *GetChr_player_00(void) 
{
	return &g_chr_player_00;
}

//========== *** CHR:�v���C���[[00] �̊Ǘ������擾 ***
Chr_player_00Control *GetChr_player_00Control(void)
{
	return &g_chr_player_00Control;
}

//========== *** CHR:�v���C���[[00] �̎�ޖ��̏����擾 ***
Chr_player_00Type *GetChr_player_00Type(void)
{
	return g_aChr_player_00Type;
}

//========== *** CHR:�v���C���[[00] �̃A�C�e�����̏����擾 ***
const Chr_player_00Item *GetChr_player_00Item(void)
{
	return g_aChr_player_00Item;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterChr_player_00�֐� - CHR:�v���C���[[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterChr_player_00(void) 
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	pChr->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̈ʒu
	pChr->posOld				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ߋ��̈ʒu
	pChr->force					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��
	pChr->move					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	pChr->rot					= CHR_PLAYER_00_INIT_ROT;			// ����
	pChr->moveRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ�����
	pChr->targetRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW����
	pChr->setPos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ݒ�ʒu
	pChr->relativePosR			= CHR_PLAYER_00_RELATIVE_POS_R;		// �����_�̑��Έʒu
	pChr->targetRltvPosR		= CHR_PLAYER_00_RELATIVE_POS_R;		// �ڕW�̒����_�̑��Έʒu
	pChr->nType					= 0;								// ���
	pChr->state					= CHR_PLAYER_00_STATE_NORMAL;		// ���
	pChr->nCounterState			= 0;								// ��ԃJ�E���^�[��ݒ�
	pChr->nHP					=									// HP
	pChr->nHPMax				= 0;								// HP�̏��
	pChr->nCoin					= CHR_PLAYER_00_INIT_COIN;			// �R�C����
	pChr->nScore				= 0;								// �X�R�A
	pChr->nSelectItem			= CHR_PLAYER_00_INIT_SELECT_ITEM;	// �I���A�C�e��
	pChr->bWalk					= false;							// ���s�t���O
	pChr->bRotation				= false;							// �����]���t���O
	pChr->bSlash				= false;							// �a��
	pChr->nCounterAttack		= 0;								// �U���J�E���^�[
	pChr->nCounterInvincible	= 0;								// ���G�J�E���^�[
	pChr->nCounterDamage		= 0;								// �_���[�W�J�E���^�[
	pChr->nPedestalIndex		= -1;								// ����̔ԍ�
	pChr->nSignboardIndex		= -1;								// �Ŕ̔ԍ�
	pChr->nCoinIndex			= -1;								// �R�C���̔ԍ�
	for (int nCntItem = 0; nCntItem < CHR_PLAYER_00_ITEM_MAX; nCntItem++)
	{
		pChr->aItemCount[nCntItem] = 0;								// �A�C�e���J�E���g
	}
	pChr->nBulletAtkUpCount		= 0;								// �e�̍U���̓A�b�v�J�E���g
	pChr->nSwordAtkUpCount		= 0;								// ���̍U���̓A�b�v�J�E���g
	pChr->nRespawnTime			= CHR_PLAYER_00_INIT_RESPAWN_TIME;	// �����ɂ����鎞��
	pChr->hitTestInfo			= {};								// �����蔻��̊Ǘ����
	pChr->cmnCollision			= {};								// ���ʂ̏Փˏ��
	for (int nCntHitObj = 0; nCntHitObj < CHR_PLAYER_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};						// �I�u�W�F�N�g���̏Փˏ��
	}
	pChr->setPosCmnCollision	= {};								// �ݒ�ʒu�̋��ʂ̏Փˏ��
	pChr->partsInfo				= {};								// ���i�Ǘ�
	pChr->motion				= CHR_PLAYER_00_MOTION_WAIT;		// ���[�V����
}

//========================================
// InitParameterChr_player_00Control�֐� - CHR:�v���C���[[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterChr_player_00Control(void) 
{
	Chr_player_00Control	*pChrCtl	// CHR:�v���C���[[00] �̊Ǘ����̃|�C���^
							= &g_chr_player_00Control;

	pChrCtl->state = CHR_PLAYER_00_CONTROL_STATE_DYNAMIC;	// ���
}

//========================================
// StartStateChr_player_00�֐� - CHR:�v���C���[[00] �̏�Ԃɉ������J�n���� -
//========================================
void StartStateChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	switch (pChr->state)
	{
	case /*/ �ʏ� /*/CHR_PLAYER_00_STATE_NORMAL:
		break;
	case /*/ �_�E�� /*/CHR_PLAYER_00_STATE_DOWN:
		pChr->nCounterState = pChr->nRespawnTime;				// ��ԃJ�E���^�[��ݒ�
		pChr->nRespawnTime += CHR_PLAYER_00_ADD_RESPAWN_TIME;	// �����ɂ����鎞�Ԃ����Z
		pChr->move =	// �ړ��ʂƗ͂�����������
		pChr->force = INITD3DXVECTOR3;
		break;
	}
}

//========================================
// EndStateChr_player_00�֐� - CHR:�v���C���[[00] �̏�Ԃɉ������I������ -
//========================================
void EndStateChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	switch (pChr->state)
	{
	case /*/ �ʏ� /*/CHR_PLAYER_00_STATE_NORMAL:
		break;
	case /*/ �_�E�� /*/CHR_PLAYER_00_STATE_DOWN:
		// HP���ő�HP�ɑ΂��銄���Őݒ�
		pChr->nHP = pChr->nHPMax*CHR_PLAYER_00_RESPAWN_HP_RATE;
		// ���G�J�E���^�[��ݒ�
		pChr->nCounterInvincible = CHR_PLAYER_00_AFTER_RESPAWN_INVINCIBLE_TIME;
		break;
	}
}

//========================================
// UpdateStateChr_player_00�֐� - CHR:�v���C���[[00] �̏�Ԃɉ������X�V���� -
//========================================
void UpdateStateChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	switch (pChr->state)
	{
	case /*/ �ʏ� /*/CHR_PLAYER_00_STATE_NORMAL:
		break;
	case /*/ �_�E�� /*/CHR_PLAYER_00_STATE_DOWN: {
		char aString[TXT_MAX];	// ������
		sprintf(aString, "%d SEC TO RESPAWN,", (pChr->nCounterState / 60));
		SetText2D(
			aString, 
			FONT_002, 
			DISPLAY_CENTER, 
			CHR_PLAYER_00_RESPAWN_TEXT_POS, 
			CHR_PLAYER_00_RESPAWN_TEXT_COLOR, 
			1.0f, 1.0f);

			if (--pChr->nCounterState <= 0)
			{// ��ԃJ�E���^�[�����Z��������0�ȉ��̎��A
				SetChr_player_00State(CHR_PLAYER_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɐݒ�
			}
		break;
	}
	}
}

//========================================
// KeyInputChr_player_00�֐� - CHR:�v���C���[[00] �̃L�[���͏��� -
//========================================
void KeyInputChr_player_00(void)
{
	Chr_player_00			*pChr		// CHR:�v���C���[[00] �̏��̃|�C���^
							= &g_chr_player_00;
	Chr_player_00Control	*pChrCtl	// CHR:�v���C���[[00] �̊Ǘ����̃|�C���^
							= &g_chr_player_00Control;

	if ((pChr->bSlash) || (pChrCtl->state == CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP))
	{// �a���t���O���^ or �Ǘ���Ԃ����͒�~�̎��A
		return;	// �������I������
	}

	pChr->bWalk = true;		// ���s�t���O��^�ɂ���
	pChr->bRotation = true;	// �����]���t���O��^�ɂ���

	if (GetStick().aTplDiameter[STICK_TYPE_LEFT] > 0.0f)
	{// ���X�e�B�b�N���|����Ă��鎞�A
		// �X�e�B�b�N�ɂ��ړ�����
		MoveChr_player_00ByStick();
	}
	else if		(GetKeyboardPress(DIK_A))	{
		if		(GetKeyboardPress(DIK_W))	{ MoveChr_player_00ByKeyboard(DIRECTION_LEFT_BACK); }	// �����ړ�
		else if (GetKeyboardPress(DIK_S))	{ MoveChr_player_00ByKeyboard(DIRECTION_LEFT_FRONT); }	// ����O�ړ�
		else								{ MoveChr_player_00ByKeyboard(DIRECTION_LEFT); }		// ���ړ�
	}
	else if		(GetKeyboardPress(DIK_D))	{
		if		(GetKeyboardPress(DIK_W))	{ MoveChr_player_00ByKeyboard(DIRECTION_RIGHT_BACK); }	// �E���ړ�
		else if (GetKeyboardPress(DIK_S))	{ MoveChr_player_00ByKeyboard(DIRECTION_RIGHT_FRONT); }	// �E��O�ړ�
		else								{ MoveChr_player_00ByKeyboard(DIRECTION_RIGHT); }		// �E�ړ�
	}
	else if		(GetKeyboardPress(DIK_W))	{ MoveChr_player_00ByKeyboard(DIRECTION_BACK); }		// ���ړ�
	else if		(GetKeyboardPress(DIK_S))	{ MoveChr_player_00ByKeyboard(DIRECTION_FRONT); }		// ��O�ړ�
	else 
	{// �ړ����͂������������A
		pChr->bWalk		= false;	// ���s�t���O���U�ɂ���
		pChr->bRotation = false;	// �����]���t���O���U�ɂ���
	}

	if		((GetKeyboardTrigger(DIK_Q)) || (GetButtonTrigger(BUTTON_LEFT_SHOULDER))) { 
		pChr->nSelectItem--;	// �I���A�C�e�����Z & �C���x���g���I��SE�Đ�
		PlaySound(CHR_PLAYER_00_INVENTORY_SELECT_SE);
	}
	else if ((GetKeyboardTrigger(DIK_E)) || (GetButtonTrigger(BUTTON_RIGHT_SHOULDER))) {
		pChr->nSelectItem++;	// �I���A�C�e�����Z & �C���x���g���I��SE�Đ�
		PlaySound(CHR_PLAYER_00_INVENTORY_SELECT_SE);
	}
	IntLoopControl(&pChr->nSelectItem, CHR_PLAYER_00_ITEM_MAX, 0);	// �I���A�C�e�����[�v����
	
	if ((GetKeyboardTrigger(DIK_SPACE))
		||
		(GetButtonTrigger(BUTTON_RIGHT_TRIGGER))
		||
		((GetKeyboardPress(DIK_SPACE)) && (pChr->nSelectItem == CHR_PLAYER_00_ITEM_GUN))
		||
		(((GetButtonPress(BUTTON_RIGHT_TRIGGER))) && (pChr->nSelectItem == CHR_PLAYER_00_ITEM_GUN)))
	{// SPACE�L�[�������ꂽ or (SPACE�L�[��������Ă��� & �I���A�C�e�����e) �̎��A
		ItemUsingChr_player_00();	// �A�C�e���g�p����
	}

	if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A)) || (GetMouseTrigger(MOUSE_LEFT)))
	{// ENTER�L�[orA�{�^��or�}�E�X���{�^���������ꂽ���A
		if (pChr->nPedestalIndex != -1)
		{// ����̔ԍ���-1�łȂ����A
			if (PurchaseObj_pedestal_00())
			{// �w���o�������A
				PlaySound(CHR_PLAYER_00_BUY_SE);	// �w��SE���Đ�
			}
		}
		else if (pChr->nSignboardIndex != -1)
		{// �ŔƂ̋��������苗�����̎��A
			// UI:TIPS��ݒ�
			SetUi_tips_00(GetObj_signboard_00()[pChr->nSignboardIndex].nType);
		}
		else if (pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00].bHit)
		{// OBJ:�X�C�b�`[00] �̓�����t���O���^�̎��A
			PushObj_switch_00();	// OBJ:�X�C�b�`[00] �̉�������
		}
	}
}

//========================================
// ItemUsingChr_player_00�֐� - CHR:�v���C���[[00] �̃A�C�e���g�p���� -
//========================================
void ItemUsingChr_player_00(void) 
{
	Chr_player_00		*pChr		// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType		// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];
	bool				bSetItem	// �A�C�e���̐ݒu�t���O
						= g_aChr_player_00Item[pChr->nSelectItem].bSet;

	if ((bSetItem) && (GetChr_player_00()->setPosCmnCollision.bInside))
	{// �I�����Ă���A�C�e���̐ݒu�t���O���^ & �ݒ�ʒu�̏d�Ȃ�t���O���^�̎��A
		return;	// �������I������
	}

	if (g_aChr_player_00Item[pChr->nSelectItem].bCount)
	{// �I�����Ă���A�C�e���̃J�E���g�t���O���U�̎��A
		if (!IntConsumption(&pChr->aItemCount[pChr->nSelectItem], 1))
		{// �A�C�e���̃J�E���g����������ʑ���Ȃ��������A
			return;	// �������I������
		}
		else if (bSetItem)
		{// [�A�C�e���̃J�E���g����������ʑ���Ă���] & �I�����Ă���A�C�e���̐ݒu�t���O���^�̎��A
			pChr->motion = CHR_PLAYER_00_MOTION_SET;	// ���[�V������ݒu�ɂ���
		}
	}

	switch (pChr->nSelectItem)
	{
		//========== *** �� ***
	case CHR_PLAYER_00_ITEM_SWORD: {
		if (!pChr->bSlash) 
		{// �a���t���O���U�̎��A
			pChr->bSlash = true;		// �a���t���O��^�ɂ���
			pChr->nCounterAttack = 0;	// �U���J�E���^�[��������
			pChr->force.x += sinf(pChr->rot.y) * pType->fMoveForce * CHR_PLAYER_00_SLASH_FORCE_DIAMETER;
			pChr->force.z += cosf(pChr->rot.y) * pType->fMoveForce * CHR_PLAYER_00_SLASH_FORCE_DIAMETER;
			PlaySound(GetAtk_sword_00Type()->slashSE);	// �a��SE���Đ�
			pChr->motion = CHR_PLAYER_00_MOTION_SLASH;	// ���[�V�������a���ɂ���
		}
	}
		break;
		//========== *** �e ***
	case CHR_PLAYER_00_ITEM_GUN: {
		if (pChr->nCounterAttack <= 0) 
		{// �U���J�E���^�[��0�ȉ��̎��AATK:�e[00] �̐ݒ菈��
			SetAtk_bullet_00(pChr->pos, pChr->rot, pType->nBulletType, ATK_BULLET_00_PARENTTYPE_PLAYER);
			pChr->nCounterAttack = pType->nShotTime;	// �U���J�E���^�[��ݒ�
			pChr->motion = CHR_PLAYER_00_MOTION_SHOT;	// ���[�V�����𔭎˂ɂ���
		}
	}
		break;
		//========== *** �u���b�N ***
	case CHR_PLAYER_00_ITEM_BLOCK: {
		// OBJ:�u���b�N[00] �̐ݒ菈��
		SetObj_block_00(pChr->setPos);
		PlaySound(CHR_PLAYER_00_SET_SE);	// �ݒuSE���Đ�
	}
		break;
		//========== *** �^���b�g ***
	case CHR_PLAYER_00_ITEM_TURRET: {
		// OBJ:�^���b�g[00] �̐ݒ菈��
		SetObj_turret_00(pChr->setPos, D3DXVECTOR3(0.0f, ConvertAngleToDirection(pChr->rot.y, CHR_PLAYER_00_SET_DIRECTION), 0.0f));
		PlaySound(CHR_PLAYER_00_SET_SE);	// �ݒuSE���Đ�
	}
		break;
		//========== *** ���ˑ��u ***
	case CHR_PLAYER_00_ITEM_REFLECTOR: {
		// OBJ:�~���[[00] �̐ݒu����
		SetObj_mirror_00(pChr->setPos);
		PlaySound(CHR_PLAYER_00_SET_SE);	// �ݒuSE���Đ�
	}
		break;
		//========== *** ���d���u ***
	case CHR_PLAYER_00_ITEM_DISCHARGER: {
		// OBJ:���d���u[00] �̐ݒu����
		SetObj_discharger_00(pChr->setPos);
		PlaySound(CHR_PLAYER_00_SET_SE);	// �ݒuSE���Đ�
	}
		break;
	}
}

//========================================
// MoveChr_player_00ByKeyboard�֐� - CHR:�v���C���[[00] �̃L�[�{�[�h�ɂ��ړ����� -
//========================================
void MoveChr_player_00ByKeyboard(DIRECTION drct) 
{
	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	// �ړ������ɉ~��������
	pChr->moveRot.y = D3DX_PI;

	switch (drct)
	{
	case DIRECTION_LEFT			:pChr->moveRot.y *= -0.5f	; break;	// ��
	case DIRECTION_RIGHT		:pChr->moveRot.y *=  0.5f	; break;	// �E
	case DIRECTION_BACK			:pChr->moveRot.y *=  0.0f	; break;	// ��
	case DIRECTION_FRONT		:pChr->moveRot.y *=  1.0f	; break;	// ��O
	case DIRECTION_LEFT_BACK	:pChr->moveRot.y *= -0.25f	; break;	// ����
	case DIRECTION_LEFT_FRONT	:pChr->moveRot.y *= -0.75f	; break;	// ����O
	case DIRECTION_RIGHT_BACK	:pChr->moveRot.y *=  0.25f	; break;	// �E��
	case DIRECTION_RIGHT_FRONT	:pChr->moveRot.y *=  0.75f	; break;	// �E��O
	}

	// �ړ�����
	MoveChr_player_00(pType->fMoveForce);
}

//========================================
// MoveChr_player_00ByStick�֐� - CHR:�v���C���[[00] �̃X�e�B�b�N�ɂ��ړ����� -
//========================================
void MoveChr_player_00ByStick(void)
{
	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	// �ڕW�����ɃX�e�B�b�N�̊p�x����
	pChr->moveRot.y = -GetStick().aAngle[STICK_TYPE_LEFT] + D3DX_PI;

	// �ړ�����
	MoveChr_player_00(pType->fMoveForce * GetStick().aTplDiameter[STICK_TYPE_LEFT]);
}

//========================================
// MoveChr_player_00�֐� - CHR:�v���C���[[00] �̈ړ����� -
//========================================
void MoveChr_player_00(float fMove) 
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	D3DXVECTOR3 posV = GetCamera3D()->posV;	// ���_
	D3DXVECTOR3 posR = GetCamera3D()->posR;	// �����_

	// ���_���璍���_�܂ł̊p�x�����߁A�ړ������ɉ��Z
	pChr->moveRot.y += FindAngle(
		D3DXVECTOR3(posV.x, posV.z, 0.0f),
		D3DXVECTOR3(posR.x, posR.z, 0.0f));

	// �ړ��ʂ��X�V
	pChr->move.x += sinf(pChr->moveRot.y) * fMove;
	pChr->move.z += cosf(pChr->moveRot.y) * fMove;
}

//========================================
// UpdatePosChr_player_00�֐� - CHR:�v���C���[[00] �̈ʒu�X�V���� -
//========================================
void UpdatePosChr_player_00(void) 
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	// ���ʂ̏Փˏ���������
	pChr->cmnCollision = {};

	// �I�u�W�F�N�g���̏Փˏ���������
	for (int nCntHitObj = 0; nCntHitObj < CHR_PLAYER_00_HITOBJ_MAX; nCntHitObj++)
	{
		pChr->aObjCollision[nCntHitObj] = {};
	}

	// �ݒ�ʒu�̋��ʂ̏Փˏ���������
	pChr->setPosCmnCollision = {};

	pChr->nPedestalIndex = -1;	// ����̔ԍ���������
	pChr->nCoinIndex = -1;		// �R�C���̔ԍ���������

	pChr->pos.x += pChr->move.x;				// X���W�Ɉړ��ʂ�K�p
	pChr->move.x *= CHR_PLAYER_00_MOVE_DAMP;	// X�����̈ړ��ʂ�����
	pChr->pos.x += pChr->force.x;				// X���W�ɗ͂�K�p
	pChr->force.x *= CHR_PLAYER_00_FORCE_DAMP;	// X�����̗͂�����
	CheckCollisionChr_player_00(VECTOR_X);		// X�����̏Փ˃`�F�b�N����

	pChr->pos.z += pChr->move.z;				// Z���W�Ɉړ��ʂ�K�p
	pChr->move.z *= CHR_PLAYER_00_MOVE_DAMP;	// Z�����̈ړ��ʂ�����
	pChr->pos.z += pChr->force.z;				// Z���W�ɗ͂�K�p
	pChr->force.z *= CHR_PLAYER_00_FORCE_DAMP;	// Z�����̗͂�����
	CheckCollisionChr_player_00(VECTOR_Z);		// Z�����̏Փ˃`�F�b�N����

	CollisionProcessChr_player_00();	// �Փˏ��ɉ���������

	// �ݒu�ʒu��ݒ�
	pChr->setPos = pChr->pos;
	pChr->setPos.x = (int)((pChr->setPos.x + (OBJECT_SET_SPACE * 0.5f * PrusMinus(pChr->setPos.x))) / OBJECT_SET_SPACE) * OBJECT_SET_SPACE;
	pChr->setPos.z = (int)((pChr->setPos.z + (OBJECT_SET_SPACE * 0.5f * PrusMinus(pChr->setPos.z))) / OBJECT_SET_SPACE) * OBJECT_SET_SPACE;

	// �ݒ�ʒu�̏Փ˃`�F�b�N����
	CheckCollisionChr_player_00SetPos();

	if (g_aChr_player_00Item[pChr->nSelectItem].bAim)
	{// �I�����Ă���A�C�e���̑_���t���O���^�̎��A
		D3DXVECTOR3 posV = GetCamera3D()->posV;	// ���_
		D3DXVECTOR3 posR = GetCamera3D()->posR;	// �����_
		float		fAngleVR					// ���_���璍���_�܂ł̊p�x
					= FindAngle(D3DXVECTOR3(posV.x, posV.z, 0.0f), D3DXVECTOR3(posR.x, posR.z, 0.0f));
		pChr->targetRot.y = fAngleVR;			// �ڕW����Y�Ɉړ���������
		pChr->bRotation = true;					// �����]���t���O��^�ɂ���
	}
	else 
	{// �I�����Ă���A�C�e���̑_���t���O���U�̎��A.
		pChr->targetRot = pChr->moveRot;		// �ڕW�����Ɉړ���������
	}
}

//========================================
// SetCamera3DPosRChr_player_00�֐� - CHR:�v���C���[[00] �̃J����(3D)�̒����_�ݒ菈�� -
//========================================
void SetCamera3DPosRChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	// �I�����Ă���A�C�e���̑_���t���O�ɉ����ĖڕW�̑��Έʒu��ݒ�
	g_aChr_player_00Item[pChr->nSelectItem].bAim ?
		pChr->targetRltvPosR = CHR_PLAYER_00_AIM_RELATIVE_POS_R :
		pChr->targetRltvPosR = CHR_PLAYER_00_RELATIVE_POS_R;

	// �����_�̑��Έʒu��ڕW�̑��Έʒu�ɐ��ڂ�����
	pChr->relativePosR += (pChr->targetRltvPosR - pChr->relativePosR) * CHR_PLAYER_00_RELATIVE_POS_R_MOVE_DIAMETER;

	D3DXVECTOR3 setPos = pChr->pos;			// �ݒ�ʒu
	D3DXVECTOR3 posV = GetCamera3D()->posV;	// ���_
	D3DXVECTOR3 posR = GetCamera3D()->posR;	// �����_
	float		fAngleVR					// ���_���璍���_�܂ł̊p�x
				= FindAngle(D3DXVECTOR3(posV.x, posV.z, 0.0f), D3DXVECTOR3(posR.x, posR.z, 0.0f));

	// �ݒ�ʒu�ɑ��Έʒu��K�p
	setPos.y += pChr->relativePosR.y;
	setPos.x += sinf(fAngleVR) * pChr->relativePosR.z;
	setPos.z += cosf(fAngleVR) * pChr->relativePosR.z;
	
	// �J����(3D)�̒����_��ݒ�
	SetCamera3DPosR(setPos);
}

//========================================
// CheckCollisionChr_player_00�֐� - CHR:�v���C���[[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionChr_player_00(VECTOR vector)
{
	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo myCollInfo =
	{
		&pChr->pos,
		pChr->posOld,
		NULL,
		&pChr->rot,
		pChr->rot,
		pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
	};

	// �Փ˔���
	/*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_BLOCK_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:�R�A		[00] /*/CollisionObj_core_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_CORE_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:�X�e�[�W	[00] /*/CollisionObj_stage_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_STAGE_00], &pChr->cmnCollision, myCollInfo);
	/*/ OBJ:�X�C�b�`	[00] /*/CollisionObj_switch_00	(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SWITCH_00], &pChr->cmnCollision, myCollInfo);

	// �Փ˔���(���)
	if (GetObj_pedestal_00Control()->nCounterArrival == OBJ_PEDESTAL_00_ARRIVAL_TIME)
	{// OBJ:���	[00] �̓����J�E���^�[�������ɂ����鎞�Ԃƈ�v���Ă������A
		int nIndex;	// �ԍ�

		nIndex = CollisionObj_pedestal_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_PEDESTAL_00], &pChr->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// OBJ:���[00] �ƐڐG�������A
			pChr->nPedestalIndex = nIndex;	// ����̔ԍ���ݒ�
		}
	}

	// �Փ˔���(�R�C��)
	{
		int nIndex;	// �ԍ�

		nIndex = CollisionItm_coin_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_ITM_COIN_00], &pChr->cmnCollision, myCollInfo);
		if (-1 != nIndex) {// ITM:�R�C��[00] �ƐڐG�������A
			pChr->nCoinIndex = nIndex;	// �R�C���̔ԍ���ݒ�
		}
	}

	// �Փ˔���ɕK�v�ȏ��
	myCollInfo =
	{
		&pChr->pos,
		pChr->posOld,
		NULL,
		&pChr->rot,
		pChr->rot,
		pType->hitTestSet.aHitTest[CHR_PLAYER_00_HITTEST_WITH_OBJ_MIRROR_00]
	};

	/*/ OBJ:�~���[	[00] /*/CollisionObj_mirror_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_MIRROR_00], &pChr->cmnCollision, myCollInfo);
	
	// �Փ˔���(�Ŕ�)
	pChr->nSignboardIndex = CollisionObj_signboard_00(vector, &pChr->aObjCollision[CHR_PLAYER_00_HITOBJ_OBJ_SIGNBOARD_00], &pChr->cmnCollision, myCollInfo);
}

//========================================
// CollisionProcessChr_player_00�֐� - CHR:�v���C���[[00] �̏Փˏ��ɉ��������� -
//========================================
void CollisionProcessChr_player_00(void)
{
	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;

	if (pChr->nCoinIndex != -1) 
	{// �R�C���̔ԍ���-1�łȂ����A
		DestroyItm_coin_00(pChr->nCoinIndex);	// ITM:�R�C��[00] �̔j�󏈗�
		pChr->nCoin++;							// �R�C���������Z
	}
}

//========================================
// CheckCollisionChr_player_00SetPos�֐� - CHR:�v���C���[[00] �̐ݒ�ʒu�̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionChr_player_00SetPos(void)
{
	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	if (!g_aChr_player_00Item[pChr->nSelectItem].bSet) 
	{// �I�����Ă���A�C�e���̐ݒu�t���O���U�̎��A
		return;	// �������I������
	}

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&pChr->setPos,
		pChr->setPos,
		NULL,
		NULL,
		INITD3DXVECTOR3,
		pType->hitTestSet.aHitTest[CHR_PLAYER_00_HITTEST_SET_POS]
	};

	// �Փ˔���
	/*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00			(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:���d���u	[00] /*/CollisionObj_discharger_00		(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:�~���[		[00] /*/CollisionObj_mirror_00			(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:�^���b�g	[00] /*/CollisionObj_turret_00			(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
	/*/ OBJ:�X�e�[�W	[00] /*/CollisionObj_stage_00WithSetPos	(VECTOR_NONE, &pChr->setPosCmnCollision, &pChr->setPosCmnCollision, myCollInfo);
}

//========================================
// MotionSetChr_player_00�֐� - CHR:�v���C���[[00] �̃��[�V�����ݒ菈�� -
//========================================
void MotionSetChr_player_00(void) 
{
	Chr_player_00		*pChr		// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType		// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];
	Motion3D			*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
						= &pType->motionSet.aMotion[pChr->motion];;

	if (pChr->state == CHR_PLAYER_00_STATE_DOWN)
	{// ��Ԃ��_�E���̎��A�_�E��
		pChr->motion = CHR_PLAYER_00_MOTION_DOWN;
	}
	else if (pMotion->bLoop) 
	{// ���݂̃��[�V�����̃��[�v�t���O���^�̎��A
		pChr->motion =	// ���s/�ҋ@
			pChr->bWalk ?
			CHR_PLAYER_00_MOTION_WALK :
			CHR_PLAYER_00_MOTION_WAIT;
	}

	// ���[�V�����ݒ�
	SetMotion3D(
		&pChr->partsInfo,
		pChr->motion);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_player_00�֐� - CHR:�v���C���[[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadChr_player_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Chr_player_00Type	*pChrType	// CHR:�v���C���[[00] �̎�ޖ��̏��
						= g_aChr_player_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(CHR_PLAYER_00_TYPE_DATA_FILE_PATH, "r");

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
			// CHR:�v���C���[[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))			{ pChrType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "HIT_POINT:"))		{ fscanf(pFile, "%d", &pChrType->nHP); }			// HP
				else if (!strcmp(aDataSearch, "MOVE_FORCE:"))		{ fscanf(pFile, "%f", &pChrType->fMoveForce); }		// �ړ���
				else if (!strcmp(aDataSearch, "SLASH_DAMAGE:"))		{ fscanf(pFile, "%d", &pChrType->nSlashDamage); }	// �a���_���[�W
				else if (!strcmp(aDataSearch, "BULLET_TYPE:"))		{ fscanf(pFile, "%d", &pChrType->nBulletType); }	// �e�̎��
				else if (!strcmp(aDataSearch, "SHOT_TIME:"))		{ fscanf(pFile, "%d", &pChrType->nShotTime); }		// ���ˊԊu
				else if (!strcmp(aDataSearch, "SHADOW_RADIUS:"))	{ fscanf(pFile, "%f", &pChrType->fShadowRadius); }	// �e�̔��a
				else if (!strcmp(aDataSearch, "DAMAGE_SE:"))		{ fscanf(pFile, "%d", &pChrType->damageSE); }		// �_���[�WSE
				else if (!strcmp(aDataSearch, "HITTESTSET"))		{ LoadHitTestSet(pFile, &pChrType->hitTestSet); }	// �����蔻��ݒ���
				else if (!strcmp(aDataSearch, "PARTSSET"))			{ LoadParts3DSet(pFile, &pChrType->partsSet); }		// ���i�ݒ���
				else if (!strcmp(aDataSearch, "MOTIONSET"))			{ LoadMotion3DSet(pFile, &pChrType->motionSet); }	// ���[�V�����ݒ���
			}
		}
	}
}

//========================================
// InitChr_player_00�֐� - CHR:�v���C���[[00] �̏��������� -
//========================================
void InitChr_player_00(void)
{
	// �Ǘ����̃p�����[�^�̏���������
	InitParameterChr_player_00Control();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXMATERIAL		*pMat;	// �}�e���A���ւ̃|�C���^

	for (int nCntType = 0; nCntType < CHR_PLAYER_00_TYPE_NUM; nCntType++)
	{
		Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_player_00Type[nCntType];

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
				&g_pBuffMatChr_player_00[nCntParts],
				NULL,
				&g_aNumMatChr_player_00[nCntType][nCntParts],
				&g_aMeshChr_player_00[nCntType][nCntParts]);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatChr_player_00[nCntParts]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_player_00[nCntType][nCntParts]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aTextureChr_player_00[nCntType][nCntParts][nCntMat]);
				}
			}
		}
	}

	// �p�����[�^�[�̏���������
	InitParameterChr_player_00();

	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	// ���i(3D)�̃g�����X�t�H�[����������
	InitParts3DTransform(&pChr->partsInfo, &pType->partsSet);

	// HP��������
	pChr->nHP =
	pChr->nHPMax = pType->nHP;

	// �J����(3D)�̒����_��ݒ�
	SetCamera3DPosR(pChr->pos + CHR_PLAYER_00_RELATIVE_POS_R);
}

//========================================
// UninitChr_player_00�֐� - CHR:�v���C���[[00] �̏I������ -
//========================================
void UninitChr_player_00(void)
{
	for (int nCntType = 0; nCntType < CHR_PLAYER_00_TYPE_NUM; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++)
		{
			// ���b�V���̔j��
			if (g_aMeshChr_player_00[nCntType][nCntParts] != NULL)
			{
				g_aMeshChr_player_00[nCntType][nCntParts]->Release();
				g_aMeshChr_player_00[nCntType][nCntParts] = NULL;
			}

			// �}�e���A���̔j��
			if (g_pBuffMatChr_player_00[nCntParts] != NULL)
			{
				g_pBuffMatChr_player_00[nCntParts]->Release();
				g_pBuffMatChr_player_00[nCntParts] = NULL;
			}

			// �e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < MATERIAL_3D_MAX; nCntMat++) {
				if (g_aTextureChr_player_00[nCntType][nCntParts][nCntMat] != NULL)
				{
					g_aTextureChr_player_00[nCntType][nCntParts][nCntMat]->Release();
					g_aTextureChr_player_00[nCntType][nCntParts][nCntMat] = NULL;
				}
			}
		}
	}
}

//========================================
// UpdateChr_player_00�֐� - CHR:�v���C���[[00] �̍X�V���� -
//========================================
void UpdateChr_player_00(void)
{
	Chr_player_00			*pChr		// CHR:�v���C���[[00] �̏��̃|�C���^
							= &g_chr_player_00;
	Chr_player_00Type		*pType		// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
							= &g_aChr_player_00Type[pChr->nType];
	Chr_player_00Control	*pChrCtl	// CHR:�v���C���[[00] �̊Ǘ����̃|�C���^
							= &g_chr_player_00Control;

	if (pChrCtl->state != CHR_PLAYER_00_CONTROL_STATE_STATIC)
	{// �Ǘ���Ԃ��ÓI�łȂ����A
		// ���݂̈ʒu���ߋ��̈ʒu�Ƃ��ĕۑ�
		pChr->posOld = pChr->pos;

		// ��Ԃɉ������X�V����
		UpdateStateChr_player_00();

		if (pChr->state != CHR_PLAYER_00_STATE_DOWN)
		{// ��Ԃ��_�E���łȂ����A
			// �L�[���͏���
			KeyInputChr_player_00();

			// �ʒu�X�V����
			UpdatePosChr_player_00();

			// ����/�ړ�����/�ڕW�����𐧌�
			RotControl(&pChr->rot);
			RotControl(&pChr->moveRot);
			RotControl(&pChr->targetRot);

			// �p�x��ڕW�p�x�Ɍ����Đ��ڂ���
			pChr->rot.y += (AngleDifference(pChr->rot.y, pChr->targetRot.y) * CHR_PLAYER_00_ROT_DIAMETER) * pChr->bRotation * (pChr->bSlash ^ 1);
		}

		if (pChrCtl->state != CHR_PLAYER_00_CONTROL_STATE_INPUT_STOP)
		{// �Ǘ���Ԃ����͒�~�łȂ����A
			// �J����(3D)�̒����_�ݒ菈��
			SetCamera3DPosRChr_player_00();
		}

		// ���i(3D)�̃g�����X�t�H�[�����擾
		GetParts3DTransform(&pChr->partsInfo, &pType->partsSet);

		// ���[�V�����̐ݒ菈��
		MotionSetChr_player_00();

		// ���[�V�����̍X�V����
		if (UpdateMotion3D(
			pChr->partsInfo.nMotion,
			g_aChr_player_00Type[pChr->nType].motionSet,
			&pChr->partsInfo,
			g_aChr_player_00Type[pChr->nType].partsSet)) 
		{// ���[�V���������[�v�ɒB�������A
			Motion3D	*pMotion	// ���݂̃��[�V�����̏��̃|�C���^
						= &pType->motionSet.aMotion[pChr->motion];
			if (!pMotion->bLoop)
			{// ���݂̃��[�V�����̃��[�v�t���O���U�̎��A
				pChr->motion = CHR_PLAYER_00_MOTION_WAIT;	// ���[�V������ҋ@�ɐݒ�
			}
		}
	}

	// EFF:�e[00] �̐ݒ菈��
	SetEff_shadow_00(pChr->pos, pType->fShadowRadius);

	if (pChr->bSlash) 
	{// �a���t���O���^�̎��A
		if (++pChr->nCounterAttack >= CHR_PLAYER_00_SLASH_TIME) 
		{// �U���J�E���^�[�����Z�������ʎa�����ԂɒB�������A
			pChr->bSlash = false;	
		}
	}

	if (pChr->nCounterInvincible > 0)
	{// ���G�J�E���^�[��0�������Ă��鎞�A
		pChr->nCounterInvincible--;	// ���G�J�E���^�[�����Z����
	}
	if (pChr->nCounterDamage > 0)
	{// �_���[�W�J�E���^�[��0�������Ă��鎞�A
		pChr->nCounterDamage--;	// �_���[�W�J�E���^�[�����Z����
	}

	if ((pChr->nSelectItem == CHR_PLAYER_00_ITEM_GUN) && (pChr->nCounterAttack > 0))
	{// �I���A�C�e�����e & �U���J�E���^�[��0�������Ă��鎞�A
		pChr->nCounterAttack--;	// �U���J�E���^�[�����Z
	}
}

//========================================
// DrawChr_player_00�֐� - CHR:�v���C���[[00] �̕`�揈�� -
//========================================
void DrawChr_player_00(void)
{
	Chr_player_00		*pChr				// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX			mtxSelf;			// �{�̂̃}�g���b�N�X

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pChr->rot.y, pChr->rot.x, pChr->rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// �{�̂̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pChr->pos.x, pChr->pos.y, pChr->pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	for (int nCntParts = 0; nCntParts < pType->partsSet.nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		// ���i(3D)�̏��̃|�C���^
					= &pChr->partsInfo.aParts[nCntParts];
		Parts3DType *pPartsType	// ���i(3D)�̕��i�̎�ޖ��̏��̃|�C���^
					= &pType->partsSet.aPartsType[nCntParts];
		D3DXMATRIX	mtxParent;	// �e�}�g���b�N�X

		// ���i�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtxWorldChr_player_00[nCntParts]);

		// ���i�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rotResult.y, pParts->rotResult.x, pParts->rotResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_player_00[nCntParts], &g_aMtxWorldChr_player_00[nCntParts], &mtxRot);

		// ���i�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pParts->posResult.x, pParts->posResult.y, pParts->posResult.z);
		D3DXMatrixMultiply(&g_aMtxWorldChr_player_00[nCntParts], &g_aMtxWorldChr_player_00[nCntParts], &mtxTrans);

		if (pPartsType->nParent != -1) 
		{// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�
			mtxParent = g_aMtxWorldChr_player_00[pPartsType->nParent];
		}
		else 
		{// ���i�̐e�ԍ���-1(�e����)�̎��A�e�}�g���b�N�X��{�̃}�g���b�N�X�Őݒ�
			mtxParent = mtxSelf;
		}

		// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(
			&g_aMtxWorldChr_player_00[nCntParts],
			&g_aMtxWorldChr_player_00[nCntParts],
			&mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtxWorldChr_player_00[nCntParts]);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatChr_player_00[nCntParts]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aNumMatChr_player_00[pChr->nType][nCntParts]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			SetMaterial(pDevice, &pMat[nCntMat].MatD3D,
			{
				255,
				(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_PLAYER_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - ((float)pChr->nCounterDamage / (float)CHR_PLAYER_00_DAMAGE_TIME))),
				(int)(255 * (1.0f - (pChr->nCounterInvincible % CHR_PLAYER_00_INVINCIBLE_BLINK_TIME)))
			});

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureChr_player_00[pChr->nType][nCntParts][nCntMat]);

			// �|���S��(�p�[�c)�̕`��
			g_aMeshChr_player_00[pChr->nType][nCntParts]->DrawSubset(nCntMat);
		}
	}
}

//========================================
// CollisionChr_player_00�֐� - CHR:�v���C���[[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionChr_player_00(VECTOR vector, Collision *pChrCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;
	
	if ((pChrCollision->bHit) || (pChr->state==CHR_PLAYER_00_STATE_DOWN))
	{// ������t���O���^ or CHR:�v���C���[[00] �̏�Ԃ��_�E���̎��A
		return -1;	// �������I������
	}

	int					nIndex	// CHR:�v���C���[[00] �̕Ԃ��̔ԍ�
						= -1;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionCheck(
		vector,
		pChrCollision,
		myCollInfo,
		{
			&pChr->pos,
			pChr->pos,
			NULL,
			&pChr->rot,
			pChr->rot,
			pType->hitTestSet.aHitTest[pChr->hitTestInfo.nHitTest]
		},
		COLLCHK_MODE_LOOKDOWN);

	if (pChrCollision->bHit)
	{// ������t���O���^�̎��A
		nIndex = 0;	// �Ԃ��̔ԍ���ݒ�
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pChrCollision);

	// �ԍ���Ԃ�
	return nIndex;
}

//========================================
// DamageChr_player_00�֐� - CHR:�v���C���[[00] �̃_���[�W���� -
//========================================
void DamageChr_player_00(int nDamage)
{
	if ((GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL) || (nDamage == 0))
	{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̎��A
		return;	// �������I������
	}

	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	if ((pChr->nCounterInvincible > 0) || (pChr->state == CHR_PLAYER_00_STATE_DOWN))
	{// ���G�J�E���^�[��0�������Ă��� or ��Ԃ��_�E���̎��A
		return;	// �������I������
	}

	nDamage *= fDamageRand();	// �����␳
	pChr->nHP -= nDamage;		// HP����_���[�W�����Z
	pChr->nCounterDamage		// �_���[�W�J�E���^�[��ݒ�
		= CHR_PLAYER_00_DAMAGE_TIME;
	SetUi_damage_00(			// UI:�_���[�W[00] �̐ݒ菈��
		pChr->pos, nDamage, UI_DAMAGE_00_COLOR_TYPE_PLAYER);
	SetEff_screen_00(			// EFF:�X�N���[��[00] �̐ݒ菈��
		CHR_PLAYER_00_DAMAGE_TIME, CHR_PLAYER_00_DAMAGE_COLOR);
	PlaySound(pType->damageSE);	// �_���[�WSE���Đ�

	// �J����(3D)�̐U����ݒ�
	SetCamera3DVibration(CHR_PLAYER_00_DAMAGE_VIBRATION);

	if (pChr->nHP <= 0)
	{// HP��0�ȉ��̎��A
		pChr->nHP = 0;	// HP��0�ɂ���
		// ��Ԃ��_�E���ɐݒ�
		SetChr_player_00State(CHR_PLAYER_00_STATE_DOWN);		
	}
	else
	{// HP��0�������Ă��鎞�A���G�J�E���^�[��ݒ�
		pChr->nCounterInvincible = CHR_PLAYER_00_INVINCIBLE_TIME;
	}
}

//========================================
// HealChr_player_00�֐� - CHR:�v���C���[[00] �̉񕜏��� -
//========================================
void HealChr_player_00(int nHeal)
{
	Chr_player_00		*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
						= &g_chr_player_00;
	Chr_player_00Type	*pType	// CHR:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
						= &g_aChr_player_00Type[pChr->nType];

	pChr->nHPMax =		// �ő�HP���X�V
		pType->nHP *
		(1.0f + (OBJ_PEDESTAL_00_PLAYER_HP_ADDRATE * GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS].nCntPriceUp));
	pChr->nHP += nHeal;	// HP�����Z
	IntControl(			// HP�𐧌�
		&pChr->nHP, 
		pChr->nHPMax, 0);
}

//========================================
// KnockBackChr_player_00�֐� - CHR:�v���C���[[00] �̃m�b�N�o�b�N���� -
//========================================
void KnockBackChr_player_00(D3DXVECTOR3 rot, float fForce)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	// �͂�������
	pChr->force.x += sinf(rot.y) * fForce;
	pChr->force.z += cosf(rot.y) * fForce;
}

//========================================
// SetChr_player_00State�֐� - CHR:�v���C���[[00] �̏�Ԑݒ菈�� -
//========================================
void SetChr_player_00State(CHR_PLAYER_00_STATE state)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;

	// ��Ԃɉ������I������
	EndStateChr_player_00();

	// ��Ԃ���
	pChr->state = state;

	// ��Ԃɉ������J�n����
	StartStateChr_player_00();
}

//========================================
// SetChr_player_00ControlState�֐� - CHR:�v���C���[[00] �̊Ǘ���Ԑݒ菈�� -
//========================================
void SetChr_player_00ControlState(CHR_PLAYER_00_CONTROL_STATE state)
{
	Chr_player_00Control	*pChrCtl	// CHR:�v���C���[[00] �̊Ǘ����̃|�C���^
							= &g_chr_player_00Control;

	pChrCtl->state = state;	// ��Ԃ���
}

#if _DEBUG_COMMENT
//========================================
// DrawCommentChr_player_00�֐� - CHR:�v���C���[[00] �̃R�����g�`�揈�� -
//========================================
void DrawCommentChr_player_00(void)
{
	Chr_player_00	*pChr	// CHR:�v���C���[[00] �̏��̃|�C���^
					= &g_chr_player_00;
	char			aString	// ������
					[TXT_MAX];

	CommentColorChange({ 88,248,152 });	// �R�����g�F�ݒ�
	sprintf(aString, "*========* �v���C���[��� *========*"); DrawComment(aString);
	sprintf(aString, " �ʒu: X %.1f Y %.1f Z %.1f", pChr->pos.x, pChr->pos.y, pChr->pos.z); DrawComment(aString);
	sprintf(aString, " ����: X %.1f Y %.1f Z %.1f", pChr->rot.x, pChr->rot.y, pChr->rot.z); DrawComment(aString);
	sprintf(aString, " �a��: %d", pChr->rot.x, pChr->rot.y, pChr->rot.z); DrawComment(aString);
	sprintf(aString, "*==================================*"); DrawComment(aString);
}
#endif