//========================================
// 
// ���e�l��[00]�̏���
// Author:���� ����
// 
//========================================
// *** ui_bomb_get_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "md_game_00.h"
#include "ui_bomb_get_00.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************

// ���e�l��[00] �̎���
// ���e�l��[00] �̓_�Ŏ��̐F
// ���e�l��[00] �̓_�Ŏ��ɂ����鎞��
// ���e�l��[00] �̈ړ���
#define UI_BOMB_GET_00_LIFE				(40)
#define UI_BOMB_GET_00_BRINK_COLOR		{243, 191, 63, 255}
#define UI_BOMB_GET_00_BRINK_TIME		(4)
#define UI_BOMB_GET_00_MOVE				(D3DXVECTOR3(0.0f,-1.0f,0.0f))

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���e�l��[00] �̃p�����[�^�[�̏�����
void InitParameterUi_bomb_get_00(void);

//****************************************
// �O���[�o���錾
//****************************************
Ui_bomb_get_00 g_ui_bomb_get_00;	// ���e�l��[00] �̏��

//========== *** ���e�l��[00] �̏����擾 ***
Ui_bomb_get_00 *GetUi_bomb_get_00(void) 
{
	return &g_ui_bomb_get_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_bomb_get_00�֐� - ���e�l��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_bomb_get_00(void) 
{
	g_ui_bomb_get_00.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	g_ui_bomb_get_00.bUse			= false;							// �g�p���Ă��邩�t���O
	g_ui_bomb_get_00.nLife			= 0;								// ����
	g_ui_bomb_get_00.nCounterBrink	= 0;								// �_�ŃJ�E���^�[
	g_ui_bomb_get_00.bBrink			= false;							// �_�Ńt���O
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_bomb_get_00�֐� - ���e�l��[00]�̏��������� -
//========================================
void InitUi_bomb_get_00(void)
{
	// ���e�l��[00] �̃p�����[�^�[�̏���������
	InitParameterUi_bomb_get_00();
}

//========================================
// UninitUi_bomb_get_00�֐� - ���e�l��[00]�̏I������ -
//========================================
void UninitUi_bomb_get_00(void)
{
	// ��\���ɂ���
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_GET, false);
}

//========================================
// UpdateUi_bomb_get_00�֐� - ���e�l��[00]�̍X�V���� -
//========================================
void UpdateUi_bomb_get_00(void)
{
	Ui_bomb_get_00	*pUi					//
					= &g_ui_bomb_get_00;	// ���e�l��[00] �̏��

	if (pUi->bUse)
	{// ���e�l��[00] ���g�p����Ă��鎞�A
		// �\������
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_GET, true);
	}
	else
	{// ���e�l��[00] ���g�p����Ă��Ȃ����A
		// ��\���ɂ���
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_BOMB_GET, false);

		// �������I������
		return;
	}

	if (--pUi->nLife <= 0)
	{// ���������Z���A�s�������A
		// �g�p���Ă��Ȃ���Ԃɂ���
		pUi->bUse = false;
	}

	// �ʒu�Ɉړ��ʂ��X�V
	pUi->pos += UI_BOMB_GET_00_MOVE;

	D3DXVECTOR3 pos			//
				= pUi->pos;	// �ʒu

	// �ʒu���J�����ɍ��킹��
	MatchCameraPosition(&pos);

	if (++pUi->nCounterBrink >= UI_BOMB_GET_00_BRINK_TIME)
	{// �_�ŃJ�E���^�[�����Z���A�_�łɂ����鎞�ԂɒB�������A
		// �_�Ńt���O��������
		pUi->nCounterBrink = 0;

		// �_�Ńt���O��؂�ւ���
		pUi->bBrink ^= 1;
	}

	Color	col						//
			= { 255,255,255,255 };	// �F

	if (pUi->bBrink)
	{// �_�Ńt���O���^�̎��A
		// �F��_�Ŏ��̕��ɂ���
		col = UI_BOMB_GET_00_BRINK_COLOR;
	}

	// �e�L�X�g�̍X�V����
	UpdateTxt_00(
		TEXT_FIELD_UI, 
		MD_GAME_00_TEXT_BOMB_GET, 
		GetMd_game_00Text(MD_GAME_00_TEXT_BOMB_GET).aDisplay, 
		pos, 
		col, 
		TEXT_MODE_CENTER, 
		GetMd_game_00Text(MD_GAME_00_TEXT_BOMB_GET).font,
		1.0f,
		1.0f);
}

//========================================
// SetUi_bomb_get_00�֐� - ���e�l��[00]�̐ݒ菈�� -
//========================================
void SetUi_bomb_get_00(D3DXVECTOR3 pos)
{
	Ui_bomb_get_00	*pUi					//
					= &g_ui_bomb_get_00;	// ���e�l��[00] �̏��

	// �p�����[�^�[�̏���������
	InitParameterUi_bomb_get_00();

	// �ʒu����
	pUi->pos = pos;

	// ������ݒ肷��
	pUi->nLife = UI_BOMB_GET_00_LIFE;

	// �g�p���Ă����Ԃɂ���
	pUi->bUse = true;
}