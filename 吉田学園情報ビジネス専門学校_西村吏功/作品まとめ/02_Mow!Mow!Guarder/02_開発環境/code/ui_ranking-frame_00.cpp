//========================================
// 
// UI:�����L���O�t���[��[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_ranking-frame_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "md_game_00.h"				// MD :�Q�[��				[00]
#include "sys_ranking_00.h"			// SYS:�����L���O			[00]
#include "ui_ranking-frame_00.h"	// UI :�����L���O�t���[��	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�����L���O�t���[��[00] �̃e�N�X�`���̑��΃p�X
// UI:�����L���O�t���[��[00] �̕�
// UI:�����L���O�t���[��[00] �̍���
#define UI_RANKINGFRAME_00_TEXTURE_PATH		"data\\TEXTURE\\UserInterface\\ui_ranking-frame_000.png"
#define UI_RANKINGFRAME_00_TEXTURE_WIDTH	(PIXEL*256)
#define UI_RANKINGFRAME_00_TEXTURE_HEIGHT	(PIXEL*256)

// UI:�����L���O�t���[��[00] �̏o���ɂ����鎞��
// UI:�����L���O�t���[��[00] �̏�����̂ɂ����鎞��
#define UI_RANKINGFRAME_00_IN_POP_TIME		(10)
#define UI_RANKINGFRAME_00_IN_CLEAR_TIME	(10)

// UI:�����L���O�t���[��[00] �̃e�L�X�g�̊J�n�ʒuX
// UI:�����L���O�t���[��[00] �̃e�L�X�g�̏I���ʒuX
// UI:�����L���O�t���[��[00] �̃e�L�X�g�̈ʒuY
// UI:�����L���O�t���[��[00] �̃e�L�X�g�̊ԊuY
// UI:�����L���O�t���[��[00] �̃e�L�X�g�Z�b�g�ɂ����鎞��
// UI:�����L���O�t���[��[00] �̖��O�̑��ΈʒuX
#define UI_RANKINGFRAME_00_TEXT_START_POS_X		(PIXEL*-90)
#define UI_RANKINGFRAME_00_TEXT_END_POS_X		(PIXEL*-50)
#define UI_RANKINGFRAME_00_TEXT_POS_Y			(PIXEL*-56)
#define UI_RANKINGFRAME_00_TEXT_SPACE_Y			(PIXEL*16)
#define UI_RANKINGFRAME_00_TEXT_SET_TIME		(60)
#define UI_RANKINGFRAME_00_NAME_RERATIVE_POS_X	(PIXEL*36)

// UI:�����L���O�t���[��[00] �̓_�łɂ����鎞��
// UI:�����L���O�t���[��[00] �̓_�ŐF
#define UI_RANKINGFRAME_00_BLINK_TIME	(4)
#define UI_RANKINGFRAME_00_BLINK_COLOR	Color{243,189,63,255}

// UI:�����L���O�t���[��[00] �̖��̑��ΈʒuX
#define UI_RANKINGFRAME_00_ARROW_RERATIVE_POS_X	(PIXEL*6*6)

// UI:�����L���O�t���[��[00] �̏���̕\���`��
// UI:�����L���O�t���[��[00] �̏���̑��ΈʒuY
#define UI_RANKINGFRAME_00_UP_ARROW_DISPLAY			"^"
#define UI_RANKINGFRAME_00_UP_ARROW_RERATIVE_POS_Y	(PIXEL*-8)

// UI:�����L���O�t���[��[00] �̉����̕\���`��
// UI:�����L���O�t���[��[00] �̉����̑��ΈʒuY
#define UI_RANKINGFRAME_00_DOWN_ARROW_DISPLAY			"_"
#define UI_RANKINGFRAME_00_DOWN_ARROW_RERATIVE_POS_Y	(PIXEL*8)

// UI:�����L���O�t���[��[00] �̑I��SE
// UI:�����L���O�t���[��[00] �̌���SE
#define UI_RANKINGFRAME_00_SELECT_SE		(SOUND_LABEL_SE_SELECT_000)
#define UI_RANKINGFRAME_00_DETERMINATION_SE	(SOUND_LABEL_SE_DETERMINATION_001)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�����L���O�t���[��[00] �̃p�����[�^�[�̏���������
void InitParameterUi_rankingFrame_00(void);

// UI:�����L���O�t���[��[00] �̏�ԏ���
void StateProcessUi_rankingFrame_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureUi_rankingFrame_00	// �e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_rankingFrame_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_rankingFrame_00		g_ui_rankingFrame_00;			// UI:�����L���O�t���[��[00] �̏��

//========== *** UI:�����L���O�t���[��[00] �̏����擾 ***
Ui_rankingFrame_00 *GetUi_rankingFrame_00(void) 
{
	return &g_ui_rankingFrame_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^ 
						= &g_ui_rankingFrame_00;

	pUi->state			= UI_RANKINGFRAME_00_STATE_CLEAR;	// ���
	pUi->nCounterState	= 0;								// ��ԃJ�E���^�[
	pUi->fAlpha			= 0.0f;								// �S�̂̓����x
	pUi->pos			= INITD3DXVECTOR3;					// �ʒu
	pUi->scale			= { 1.0f,0.0f,0.0f };				// �g��{��
	pUi->bNameEntry		= false;							// ���O���̓t���O
	pUi->nUpdateRank	= -1;								// �X�V����
	pUi->nCounterBlink	= 0;								// �_�ŃJ�E���^�[
}

//========================================
// NameEntryUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̖��O���͏��� -
//========================================
void NameEntryUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^ 
						= &g_ui_rankingFrame_00;

	if ((!pUi->bNameEntry) || (pUi->nCounterState < UI_RANKINGFRAME_00_TEXT_SET_TIME))
	{// ���O���̓t���O���U or ��ԃJ�E���^�[���e�L�X�g�Z�b�g�ɂ����鎞�Ԃ�������Ă��鎞�A
		return;	// �������I������
	}

	if ((GetKeyboardRepeat(DIK_DOWN))
		||
		(GetKeyboardRepeat(DIK_S))
		||
		(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
		||
		(GetButtonRepeat(BUTTON_DOWN)))
	{// ���ɓ��͂��ꂽ���A
		pUi->nCountChar--;	// �����J�E���g�����Z
		// �I��SE���Đ�
		PlaySound(UI_RANKINGFRAME_00_SELECT_SE);
	}
	else if (
		(GetKeyboardRepeat(DIK_UP))
		||
		(GetKeyboardRepeat(DIK_W))
		||
		(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
		||
		(GetButtonRepeat(BUTTON_UP)))
	{// ��ɓ��͂��ꂽ���A
		pUi->nCountChar++;	// �����J�E���g�����Z
		// �I��SE���Đ�
		PlaySound(UI_RANKINGFRAME_00_SELECT_SE);
	}

	// �����J�E���g�����[�v����
	IntLoopControl(&pUi->nCountChar, RANKING_NAME_MAX_CHAR, RANKING_NAME_MIN_CHAR);

	if (((GetKeyboardTrigger(DIK_BACKSPACE))
		||
		(GetButtonTrigger(BUTTON_B)))
		&&
		(pUi->nCountName > 0))
	{// �폜�����͂��ꂽ���A�����O�J�E���g��0�������Ă��鎞�A
		// ���O���͂̃J�E���g�����Z
		pUi->nCountName--;

		// ���݂̃J�E���g�ȍ~�̕������󔒂ɂ���
		GetSys_ranking_00()[pUi->nUpdateRank].aName[pUi->nCountName + 1] = '\0';

		// �����J�E���g��ݒ�
		pUi->nCountChar = GetSys_ranking_00()[pUi->nUpdateRank].aName[pUi->nCountName];
	}
	else if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A) || (GetMouseTrigger(MOUSE_LEFT))))
	{// ENTER�L�[ or A�{�^�� or �}�E�X���{�^�������͂��ꂽ���A
		// ����SE���Đ�
		PlaySound(UI_RANKINGFRAME_00_DETERMINATION_SE);

		if (++pUi->nCountName >= RANKING_NAME_NUM) 
		{// ���O���͂̃J�E���g�����Z�������ʁA�����L���O���̕������ɒB�������A
			pUi->bNameEntry = false;	// ���O���̓t���O���U�ɂ���
			return;						// �������I������
		}
		pUi->nCountChar	// �����J�E���g��������
			= RANKING_NAME_START_CHAR;
	}

	// ���݂̃J�E���g�̕����𔽉f����
	GetSys_ranking_00()[pUi->nUpdateRank].aName[pUi->nCountName] = pUi->nCountChar;

	// ���̊�ʒu
	D3DXVECTOR3 arrowPos = pUi->pos +
		D3DXVECTOR3(
			UI_RANKINGFRAME_00_TEXT_END_POS_X + (pUi->nCountName * (GetFont()[FONT_000].c_fSpaceX * PIXEL)) + UI_RANKINGFRAME_00_ARROW_RERATIVE_POS_X,
			UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * pUi->nUpdateRank),
			0.0f);

	// ����
	SetText2D(
		UI_RANKINGFRAME_00_UP_ARROW_DISPLAY,
		FONT_002,
		DISPLAY_CENTER,
		arrowPos +
		D3DXVECTOR3(0.0f, UI_RANKINGFRAME_00_UP_ARROW_RERATIVE_POS_Y, 0.0f),
		INITCOLOR,
		1.0f,
		1.0f);

	// �����
	SetText2D(
		UI_RANKINGFRAME_00_DOWN_ARROW_DISPLAY,
		FONT_002,
		DISPLAY_CENTER,
		arrowPos +
		D3DXVECTOR3(0.0f, UI_RANKINGFRAME_00_DOWN_ARROW_RERATIVE_POS_Y, 0.0f),
		INITCOLOR,
		1.0f,
		1.0f);
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^ 
						= &g_ui_rankingFrame_00;

	++pUi->nCounterBlink %= (UI_RANKINGFRAME_00_BLINK_TIME * 2);	// �_�ŃJ�E���^�[�����Z����

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_RANKINGFRAME_00_STATE_POP:
	{
		Sys_ranking_00	*pSys	// �����L���O[00] �̏��̃|�C���^
						= GetSys_ranking_00();

		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, " %s  %-3s %8d", GetRankText(nCntRanking), pSys->aName, pSys->nScore);
			Color setColor =
				pUi->nUpdateRank == nCntRanking ?
				!(pUi->nCounterBlink / UI_RANKINGFRAME_00_BLINK_TIME) ?
				INITCOLOR : UI_RANKINGFRAME_00_BLINK_COLOR :
				INITCOLOR;
			SetText2D(
				aString,
				FONT_000,
				DISPLAY_LEFT,
				pUi->pos +
				D3DXVECTOR3(
					UI_RANKINGFRAME_00_TEXT_END_POS_X,
					UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * nCntRanking),
					0.0f),
				setColor,
				1.0f,
				1.0f);
		}
		break;
	}
		//========== *** �e�L�X�g�Z�b�g ***
	case UI_RANKINGFRAME_00_STATE_TEXT_SET: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// ��ԃJ�E���^�[�𐧌�
		IntControl(&pUi->nCounterState, UI_RANKINGFRAME_00_TEXT_SET_TIME, 0);

		Sys_ranking_00	*pSys	// �����L���O[00] �̏��̃|�C���^
						= GetSys_ranking_00();

		const int	nTime	// 1�̃����L���O������ɂ����鎞��
					= UI_RANKINGFRAME_00_TEXT_SET_TIME / RANKING_NUM;

		if (pUi->nCounterState % nTime == 0) 
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
		}

		for (int nCntRanking = 0; nCntRanking < IntControlReturn((pUi->nCounterState / nTime) + 1, RANKING_NUM, 0); nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, " %s  %-3s %8d", GetRankText(nCntRanking), pSys->aName, pSys->nScore);
			float fRate =	// �i�s����
				(float)(IntControlReturn(pUi->nCounterState, nTime * (nCntRanking + 1), 0) -
				(nTime * nCntRanking)) / (float)nTime;
			SetText2D(
				aString,
				FONT_000,
				DISPLAY_LEFT,
				pUi->pos +
				D3DXVECTOR3(
					(UI_RANKINGFRAME_00_TEXT_START_POS_X * (1.0f - fRate)) + (UI_RANKINGFRAME_00_TEXT_END_POS_X * fRate),
					UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * nCntRanking),
					0.0f),
					{ 255,255,255,(int)(255 * fRate) },
				1.0f,
				1.0f);

			if (pUi->nUpdateRank == nCntRanking)
			{// �X�V���ʂƃJ�E���g����v���Ă��鎞�A
				Color setColor =
					!(pUi->nCounterBlink / UI_RANKINGFRAME_00_BLINK_TIME) ?
					INITCOLOR : UI_RANKINGFRAME_00_BLINK_COLOR;
				SetText2D(
					pSys->aName,
					FONT_000,
					DISPLAY_LEFT,
					pUi->pos +
					D3DXVECTOR3(
					(UI_RANKINGFRAME_00_TEXT_START_POS_X * (1.0f - fRate)) + (UI_RANKINGFRAME_00_TEXT_END_POS_X * fRate) + UI_RANKINGFRAME_00_NAME_RERATIVE_POS_X,
						UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * nCntRanking),
						0.0f),
						{ setColor.r,setColor.g,setColor.b,(int)(255 * fRate) },
					1.0f,
					1.0f);
			}
		}

		if ((pUi->nCounterState >= UI_RANKINGFRAME_00_TEXT_SET_TIME)
			&&
			(!pUi->bNameEntry))
		{// ��ԃJ�E���^�[���e�L�X�g�Z�b�g�ɂ����鎞�ԂɒB���� & ���O���̓t���O���U�̎��A
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_POP);	// ��Ԃ��o���ɂ���
		}

		break;
	}
		//========== *** �o���� ***
	case UI_RANKINGFRAME_00_STATE_IN_POP: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_RANKINGFRAME_00_IN_POP_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_RANKINGFRAME_00_IN_POP_TIME)
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// ��Ԃ��e�L�X�g�Z�b�g�ɂ���
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_TEXT_SET);
		}

		break;
	}
		//========== *** �����Ă��� ***
	case UI_RANKINGFRAME_00_STATE_CLEAR: {
		break;
	}
		//========== *** ������ ***
	case UI_RANKINGFRAME_00_STATE_IN_CLEAR: {
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		float	fRate	// ����
				= (float)pUi->nCounterState / (float)UI_RANKINGFRAME_00_IN_POP_TIME;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_RANKINGFRAME_00_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_CLEAR);
		}

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̏��������� -
//========================================
void InitUi_rankingFrame_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_rankingFrame_00();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_rankingFrame_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_rankingFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, UI_RANKINGFRAME_00_TEXTURE_PATH, &g_pTextureUi_rankingFrame_00);

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_RANKINGFRAME_00_TEXTURE_WIDTH,
		UI_RANKINGFRAME_00_TEXTURE_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_rankingFrame_00->Unlock();
}

//========================================
// UninitUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̏I������ -
//========================================
void UninitUi_rankingFrame_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureUi_rankingFrame_00 != NULL)
	{
		g_pTextureUi_rankingFrame_00->Release();
		g_pTextureUi_rankingFrame_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_rankingFrame_00 != NULL) 
	{
		g_pVtxBuffUi_rankingFrame_00->Release();
		g_pVtxBuffUi_rankingFrame_00 = NULL;
	}
}

//========================================
// UpdateUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̍X�V���� -
//========================================
void UpdateUi_rankingFrame_00(void) 
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_rankingFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^
						= &g_ui_rankingFrame_00;

	// ���O���͏���
	NameEntryUi_rankingFrame_00();

	// ��ԏ���
	StateProcessUi_rankingFrame_00();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		g_ui_rankingFrame_00.pos,
		INITD3DXVECTOR3,
		false,
		UI_RANKINGFRAME_00_TEXTURE_WIDTH
		* pUi->scale.fWidth,
		UI_RANKINGFRAME_00_TEXTURE_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_rankingFrame_00.fAlpha) });
}

//========================================
// DrawUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̕`�揈�� -
//========================================
void DrawUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^
						= &g_ui_rankingFrame_00;

	if (pUi->state == UI_RANKINGFRAME_00_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_rankingFrame_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureUi_rankingFrame_00);

	// UI:�����L���O�t���[��[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState�֐� - UI:�����L���O�t���[��[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE state) 
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^
						= &g_ui_rankingFrame_00;

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}

//========================================
// SetUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̐ݒ菈�� -
//========================================
void SetUi_rankingFrame_00(D3DXVECTOR3 pos)
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^ 
						= &g_ui_rankingFrame_00;

	if (pUi->state != UI_RANKINGFRAME_00_STATE_CLEAR) 
	{// �����Ă����ԂłȂ����A
		// �������I������
		return;
	}

	// �ʒu����
	pUi->pos = pos;

	// �o�����̏�Ԃɂ���
	SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_IN_POP);
}

//========================================
// SetNameEntryUi_rankingFrame_00�֐� - UI:�����L���O�t���[��[00] �̖��O���͐ݒ菈�� -
//========================================
void SetNameEntryUi_rankingFrame_00(int nUpdateRank)
{
	Ui_rankingFrame_00	*pUi	// UI:�����L���O�t���[��[00] �̏��̃|�C���^ 
						= &g_ui_rankingFrame_00;

	pUi->nUpdateRank = nUpdateRank;	// �X�V���ʂ���

	if (pUi->nUpdateRank != -1) 
	{// �X�V���ʂ�-1(�X�V����)�łȂ����A
		pUi->bNameEntry = true;	// ���O���̓t���O��^�ɂ���
		pUi->nCountName = 0;	// ���O�J�E���g��������
		pUi->nCountChar			// �����J�E���g��������
			= RANKING_NAME_START_CHAR;
		pUi->nCounterBlink = 0;	// �_�ŃJ�E���^�[��������

		// �X�V�������ʂ̖��O��������
		strinit(GetSys_ranking_00()[pUi->nUpdateRank].aName, RANKING_NAME_NUM);
	}
	else 
	{// �X�V������-1(�X�V����)�̎��A
		pUi->bNameEntry = false;	// ���O���̓t���O���U�ɂ���
	}
}