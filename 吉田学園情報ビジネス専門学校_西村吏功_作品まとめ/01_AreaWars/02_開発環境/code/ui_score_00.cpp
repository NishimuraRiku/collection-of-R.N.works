//========================================
// 
// �X�R�A[00]�̏���
// Author:���� ����
// 
//========================================
// *** ui_score_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "md_game_00.h"
#include "ui_score_00.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************
#define UI_SCORE_00_LIFE			(40)				// �X�R�A[00] �̎���
#define UI_SCORE_00_POP_TIME		(20)				// �X�R�A[00] �̏o���ɂ����鎞��
#define UI_SCORE_00_CLEAR_TIME		(20)				// �X�R�A[00] �̃N���A�ɂ����鎞��
#define UI_SCORE_00_SCALE_DIAMETER	(0.005f)			// �X�R�A[00] �̑傫���ɂ�����{��
#define UI_SCORE_00_SCALE_MIN		(1.0f)				// �X�R�A[00] �̑傫���̍ŏ�
#define UI_SCORE_00_SCALE_MAX		(4.0f)				// �X�R�A[00] �̑傫���̍ő�
#define UI_SCORE_00_BRINK_COLOR		{243, 191, 63, 255}	// �X�R�A[00] �̓_�Ŏ��̐F
#define UI_SCORE_00_BRINK_TIME		(4)					// �X�R�A[00] �̓_�Ŏ��ɂ����鎞��
#define BLINK_SCORE_LINE			(1000)				// �X�R�A[00] �̓_�Ń��C��

//****************************************
// �O���[�o���錾
//****************************************
Ui_score_00 g_aUi_score_00[UI_SCORE_00_MAX];	// �X�R�A[00] �̏��

//========== *** �X�R�A[00] �̏����擾 ***
Ui_score_00 *GetUi_score_00(void) 
{
	return g_aUi_score_00;
}

//========================================
// InitUi_score_00�֐� - �X�R�A[00]�̏��������� -
//========================================
void InitUi_score_00(void)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++) 
	{
		g_aUi_score_00[nCntUi_score_00].bUse = false; // �g�p����Ă��邩�t���O ������������
		g_aUi_score_00[nCntUi_score_00].rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);	// ������������
	}
}

//========================================
// UninitUi_score_00�֐� - �X�R�A[00]�̏I������ -
//========================================
void UninitUi_score_00(void)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++)
	{
		// ��\���ɂ���
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, false);
	}
}

//========================================
// UpdateUi_score_00�֐� - �X�R�A[00]�̍X�V���� -
//========================================
void UpdateUi_score_00(void)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++)
	{
		if (g_aUi_score_00[nCntUi_score_00].bUse)
		{// �X�R�A[00] ���g�p����Ă��鎞�A
			// �\������
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, true);
		}
		else 
		{// �X�R�A[00] ���g�p����Ă��Ȃ����A
			// ��\���ɂ���
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, false);

			continue;
		}

		// ���������Z����
		g_aUi_score_00[nCntUi_score_00].nLife--;

		if (g_aUi_score_00[nCntUi_score_00].nLife <= 0)
		{// �������s�������A
			// �g�p���Ă��Ȃ���Ԃɂ���
			g_aUi_score_00[nCntUi_score_00].bUse = false;
		}

		D3DXVECTOR3 *move	= &g_aUi_score_00[nCntUi_score_00].move;	// �ړ���
		float fWidth		= g_aUi_score_00[nCntUi_score_00].fWidth;	// ��
		float fHeight		= g_aUi_score_00[nCntUi_score_00].fHeight;	// ����

		// �ʒu ���X�V
		g_aUi_score_00[nCntUi_score_00].pos.x += move->x;
		g_aUi_score_00[nCntUi_score_00].pos.y += move->y;

		// �ړ��ʂ��X�V(����)
		move->x += (0.0f - move->x) * 0.1f;
		move->y += (0.0f - move->y) * 0.1f;
		move->z += (0.0f - move->z) * 0.1f;

		D3DXVECTOR3 pos = g_aUi_score_00[nCntUi_score_00].pos;	// �ʒu

		// �ʒu�����킹��
		MatchCameraPosition(&pos);

		if (g_aUi_score_00[nCntUi_score_00].nScore >= BLINK_SCORE_LINE)
		{// �X�R�A���_�Ń��C�����z���Ă��鎞�A
			// �_�ŃJ�E���^�[�����Z
			g_aUi_score_00[nCntUi_score_00].nCounterBrink++;

			if (g_aUi_score_00[nCntUi_score_00].nCounterBrink >= UI_SCORE_00_BRINK_TIME) 
			{// �_�ŃJ�E���^�[���_�łɂ����鎞�ԂɒB�������A
				// �_�Ńt���O��������
				g_aUi_score_00[nCntUi_score_00].nCounterBrink = 0;

				// �_�Ńt���O��؂�ւ���
				g_aUi_score_00[nCntUi_score_00].bBrink ^= 1;
			}
		}
		
		{
			char aString[TXT_MAX];	// ������

			// �F��ݒ�
			Color col = { 255,255,255,255 };

			if (g_aUi_score_00[nCntUi_score_00].bBrink) 
			{
				col = UI_SCORE_00_BRINK_COLOR;
			}

			float fScore = g_aUi_score_00[nCntUi_score_00].nScore;	// �g�嗦�̊�ƂȂ�X�R�A�̒l
			float fScale = (fScore * ((float)g_aUi_score_00[nCntUi_score_00].nLife / (float)UI_SCORE_00_LIFE)) * UI_SCORE_00_SCALE_DIAMETER;

			// �傫���𐧌�
			if (fScale < UI_SCORE_00_SCALE_MIN) 
			{
				fScale = UI_SCORE_00_SCALE_MIN;
			}
			else if (fScale > UI_SCORE_00_SCALE_MAX)
			{
				fScale = UI_SCORE_00_SCALE_MAX;
			}

			sprintf(aString, "%d", g_aUi_score_00[nCntUi_score_00].nScore);
			UpdateTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, aString, pos, col, TEXT_MODE_CENTER, FONT_002, fScale, fScale);
		}
		
	}
}

//========================================
// SetUi_score_00�֐� - �X�R�A[00]�̐ݒ菈�� -
//========================================
void SetUi_score_00(D3DXVECTOR3 pos, int nScore)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++)
	{
		if (!g_aUi_score_00[nCntUi_score_00].bUse)
		{// �X�R�A[00]���g�p����Ă��Ȃ���

			// �X�R�A����
			g_aUi_score_00[nCntUi_score_00].nScore = nScore;

			// �ʒu����
			g_aUi_score_00[nCntUi_score_00].pos = pos;

			// ������ݒ肷��
			g_aUi_score_00[nCntUi_score_00].nLife = UI_SCORE_00_LIFE;

			// �_�ŃJ�E���^�[/�_�Ńt���O������������
			g_aUi_score_00[nCntUi_score_00].nCounterBrink = 0;
			g_aUi_score_00[nCntUi_score_00].bBrink = false;

			// �g�p���Ă����Ԃɂ���
			g_aUi_score_00[nCntUi_score_00].bUse = true;

			break;
		}
	}
}