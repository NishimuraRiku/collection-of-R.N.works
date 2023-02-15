//========================================
// 
// ���C�t�{�[�i�X[00]�̏���
// Author:���� ����
// 
//========================================
// *** ui_life_bonus_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "md_game_00.h"
#include "ui_life_bonus_00.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************

// ���C�t�{�[�i�X[00] �̎���
// ���C�t�{�[�i�X[00] �̓_�Ŏ��̐F
// ���C�t�{�[�i�X[00] �̓_�Ŏ��ɂ����鎞��
// ���C�t�{�[�i�X[00] �̊J�n�ʒu
// ���C�t�{�[�i�X[00] �̈ړ���
#define UI_LIFE_BONUS_00_LIFE			(80)
#define UI_LIFE_BONUS_00_BRINK_COLOR	{255, 119, 183, 255}
#define UI_LIFE_BONUS_00_BRINK_TIME		(4)
#define UI_LIFE_BONUS_00_START_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + PIXEL * 8 * 8, PIXEL * 88, 0.0f))
#define UI_LIFE_BONUS_00_MOVE			(D3DXVECTOR3(0.0f,-1.0f,0.0f))

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���C�t�{�[�i�X[00] �̃p�����[�^�[�̏�����
void InitParameterUi_life_bonus_00(void);

//****************************************
// �O���[�o���錾
//****************************************
Ui_life_bonus_00 g_ui_life_bonus_00;	// ���C�t�{�[�i�X[00] �̏��

//========== *** ���C�t�{�[�i�X[00] �̏����擾 ***
Ui_life_bonus_00 *GetUi_life_bonus_00(void) 
{
	return &g_ui_life_bonus_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_life_bonus_00�֐� - ���C�t�{�[�i�X[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_life_bonus_00(void) 
{
	g_ui_life_bonus_00.pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	g_ui_life_bonus_00.bUse				= false;							// �g�p���Ă��邩�t���O
	g_ui_life_bonus_00.nLife			= 0;								// ����
	g_ui_life_bonus_00.nCounterBrink	= 0;								// �_�ŃJ�E���^�[
	g_ui_life_bonus_00.bBrink			= false;							// �_�Ńt���O
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_life_bonus_00�֐� - ���C�t�{�[�i�X[00]�̏��������� -
//========================================
void InitUi_life_bonus_00(void)
{
	// ���C�t�{�[�i�X[00] �̃p�����[�^�[�̏���������
	InitParameterUi_life_bonus_00();
}

//========================================
// UninitUi_life_bonus_00�֐� - ���C�t�{�[�i�X[00]�̏I������ -
//========================================
void UninitUi_life_bonus_00(void)
{
	// ��\���ɂ���
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE_BONUS, false);
}

//========================================
// UpdateUi_life_bonus_00�֐� - ���C�t�{�[�i�X[00]�̍X�V���� -
//========================================
void UpdateUi_life_bonus_00(void)
{
	Ui_life_bonus_00	*pUi					//
						= &g_ui_life_bonus_00;	// ���C�t�{�[�i�X[00] �̏��

	if (pUi->bUse)
	{// ���C�t�{�[�i�X[00] ���g�p����Ă��鎞�A
		// �\������
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE_BONUS, true);
	}
	else
	{// ���C�t�{�[�i�X[00] ���g�p����Ă��Ȃ����A
		// ��\���ɂ���
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_LIFE_BONUS, false);

		// �������I������
		return;
	}

	if (--pUi->nLife <= 0)
	{// ���������Z���A�s�������A
		// �g�p���Ă��Ȃ���Ԃɂ���
		pUi->bUse = false;
	}

	// �ʒu�Ɉړ��ʂ��X�V
	pUi->pos += UI_LIFE_BONUS_00_MOVE;

	D3DXVECTOR3 pos			//
				= pUi->pos;	// �ʒu

	if (++pUi->nCounterBrink >= UI_LIFE_BONUS_00_BRINK_TIME)
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
		col = UI_LIFE_BONUS_00_BRINK_COLOR;
	}

	// �����x�������ɉ����Đݒ�
	col.nA = 255 * ((float)pUi->nLife / (float)UI_LIFE_BONUS_00_LIFE);

	{
		char aString[TXT_MAX]; // �\�����镶����

		// �������ݒ�
		sprintf(aString, GetMd_game_00Text(MD_GAME_00_TEXT_LIFE_BONUS).aDisplay, pUi->nLifeBonus);

		// �e�L�X�g�̍X�V����
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_LIFE_BONUS,
			aString,
			pos,
			col,
			TEXT_MODE_CENTER,
			GetMd_game_00Text(MD_GAME_00_TEXT_LIFE_BONUS).font,
			1.0f,
			1.0f);
	}
}

//========================================
// SetUi_life_bonus_00�֐� - ���C�t�{�[�i�X[00]�̐ݒ菈�� -
//========================================
void SetUi_life_bonus_00(int nLifeBonus)
{
	Ui_life_bonus_00	*pUi					//
						= &g_ui_life_bonus_00;	// ���C�t�{�[�i�X[00] �̏��

	// �p�����[�^�[�̏���������
	InitParameterUi_life_bonus_00();

	// ���C�t�{�[�i�X����
	pUi->nLifeBonus = nLifeBonus;

	// �ʒu��ݒ�
	pUi->pos = UI_LIFE_BONUS_00_START_POS;

	// ������ݒ肷��
	pUi->nLife = UI_LIFE_BONUS_00_LIFE;

	// �g�p���Ă����Ԃɂ���
	pUi->bUse = true;
}