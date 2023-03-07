//========================================
// 
// ���f���r���[���̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** model-viewer.cpp ***
//========================================
#include <stdio.h>
#include "main.h"
// Character
#include "chr_model.h"
// System
#include "model-viewer.h"
// _R.N.Lib
#include "../R.N.Lib\RNmain.h"

//****************************************
// �}�N����`
//****************************************
//========== *** ����֘A ***
//===== *** �J���� ***
// �J����(3D)�̃J�[�\���̉�]��
#define CAMERA3D_ROT_FORCE (D3DXVECTOR3(0.004f,-0.006f,0.0f))
// �J����(3D)�̉�]��
#define CAMERA3D_ROT_FORCE_BY_CURSOR (D3DXVECTOR3(0.0005f,0.00075f,0.0f))
// �J����(3D)�̉�]�̌����l
#define CAMERA3D_SPIN_DAMP (0.9f)
// �J����(3D)�̏����̍����̏��
#define CAMERA3D_HEIGHT_MAX (0.99f)
// �J����(3D)�̏����̍����̉���
#define CAMERA3D_HEIGHT_MIN (0.0f)

//****************************************
// �񋓌^�̒�`
//****************************************
// ���f���r���[���̃e�L�X�g
typedef enum
{
	MODELVIEWER_TEXT_MODELTYPE,				// ���f���̎��
	MODELVIEWER_TEXT_MOTIONNUMBER,			// ���[�V�����ԍ�
	MODELVIEWER_TEXT_MOTIONFRAME,			// ���[�V�����t���[��
	MODELVIEWER_TEXT_MODELTYPECHANGE,		// ���f���̎�ޕύX
	MODELVIEWER_TEXT_MOTIONNUMBERCHANGE,	// ���[�V�����ԍ��ύX
	MODELVIEWER_TEXT_PAUSE,					// �|�[�Y
	MODELVIEWER_TEXT_CAMERAUPDOWN,			// �J�����㉺
	MODELVIEWER_TEXT_HIDETEXT,				// �e�L�X�g���B��
	MODELVIEWER_TEXT_HIDEHITTEST,			// �����蔻����B��
	MODELVIEWER_TEXT_RELOAD,				// �ēǍ�
	MODELVIEWER_TEXT_MODELLOAD,				// ���f���ǂݍ���
	MODELVIEWER_TEXT_MAX,
}MODELVIEWER_TEXT;

//****************************************
// �\���̂̒�`
//****************************************
//========== *** ���̑� ***
// ���f���r���[���̃e�L�X�g�ݒ���
typedef struct 
{
	// �\���`��
	char aString[TXT_MAX];
	D3DXVECTOR3 pos;	// �ʒu
	int nFont;			// �t�H���g
	TEXT_DISP textDisp;	// �\���`��
	float fScale;		// �g��{��
}TextInfoModelViewer;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �X�V ***
// ���f���r���[���̓��͏���
void InputModelViewer(ModelViewer *pEdtr);
// ���f���r���[���̃e�L�X�g�ݒ菈��
void SetTextModelViewer(ModelViewer *pEdtr);
//========== *** ���o�� ***
// ���f���r���[���̃}�b�v���̓ǂݍ��ݏ���
void LoadModelDataModelViewer(ModelViewer *pEdtr);
// ���f���r���[���̃}�b�v���̍ēǂݍ��ݏ���
void ReloadModelDataModelViewer(ModelViewer *pEdtr);
//========== *** �擾 ***
// ���f���r���[���̃p�����[�^�[�����l���擾
ModelViewer InitParameterModelViewer(void);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
ModelViewer g_modelViewer3D;	// ���f���r���[���̏��

//****************************************
// �O���[�o���萔�錾
//****************************************
// ���f���r���[���̃e�L�X�g���
const TextInfoModelViewer g_aTextInfoModelViewer[MODELVIEWER_TEXT_MAX] = {
	{ "MODEL TYPE   : %d/%d"       ,D3DXVECTOR3(8,8,0.0f)                 ,0,TEXT_DISP_LEFT ,16.0f },	// ���f���̎��
	{ "MOTION NUMBER: %d/%d"       ,D3DXVECTOR3(8,24,0.0f)                ,0,TEXT_DISP_LEFT ,16.0f },	// ���[�V�����ԍ�
	{ "MOTION FRAME : %d/%d"       ,D3DXVECTOR3(8,40,0.0f)                ,0,TEXT_DISP_LEFT ,16.0f },	// ���[�V�����t���[��
	{ "MODEL TYPE CHANGE:[1][2]KEY",D3DXVECTOR3(SCREEN_WIDTH - 8,8,0.0f)  ,0,TEXT_DISP_RIGHT,16.0f },	// ���f���̎�ޕύX
	{ "MOTION CHANGE:[3][4]KEY"    ,D3DXVECTOR3(SCREEN_WIDTH - 8,24,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// ���[�V�����ԍ��ύX
	{ "PAUSE:   [P]KEY"            ,D3DXVECTOR3(SCREEN_WIDTH - 8,40,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// �|�[�Y
	{ "CAMERA UP/DOWN:[U][J]KEY"   ,D3DXVECTOR3(SCREEN_WIDTH - 8,56,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// �J�����㉺
	{ "HIDE TEXT:  [F1]KEY"        ,D3DXVECTOR3(SCREEN_WIDTH - 8,72,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// �e�L�X�g���B��
	{ "HIDE HITTEST:  [F2]KEY"     ,D3DXVECTOR3(SCREEN_WIDTH - 8,88,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// �e�L�X�g���B��
	{ "RELOAD:  [F3]KEY"           ,D3DXVECTOR3(SCREEN_WIDTH - 8,104,0.0f),0,TEXT_DISP_RIGHT,16.0f },	// �ēǍ�
	{ "MODEL LOAD:  [F4]KEY"       ,D3DXVECTOR3(SCREEN_WIDTH - 8,120,0.0f),0,TEXT_DISP_RIGHT,16.0f },	// ���f���ǂݍ���
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �X�V *** |------------------------
//============================================================
//========================================
// InputModelViewer�֐� - ���f���r���[���̓��͏��� -
// Author:RIKU NISHIMURA
//========================================
void InputModelViewer(ModelViewer *pEdtr)
{
	// �|�[�Y�t���O��؂�ւ���
	pEdtr->bPause ^= GetKeyboardTrigger(DIK_P);

	// �e�L�X�g�\���t���O��؂�ւ���
	pEdtr->bShowText ^= GetKeyboardTrigger(DIK_F1);

	// �����蔻��\���t���O��؂�ւ���
	pEdtr->bShowHitTest ^= GetKeyboardTrigger(DIK_F2);

	// �����蔻��̕\��/��\��
	ShowHitTest(pEdtr->bShowHitTest);

	// �J�����̏㉺
	GetCamera3D()->posR.y += CAMERA3D_Y_MOVE * GetKeyboardPress(DIK_U);
	GetCamera3D()->posR.y -= CAMERA3D_Y_MOVE * GetKeyboardPress(DIK_J);

	if (GetKeyboardTrigger(DIK_F3)) 
	{// �ēǂݍ��ݏ��� 
		ReloadModelDataModelViewer(pEdtr);
	}
	
	if (GetKeyboardTrigger(DIK_F4))
	{// �ǂݍ��ݏ��� 
		LoadModelDataModelViewer(pEdtr);
	}

	// �J��������
	{
		if (GetMousePress(MOUSEBUTTON_RIGHT))
		{// �}�E�X�̉E�{�^����������Ă��鎞�A
			// �J�[�\���̈ړ��ʂɉ����ĉ�]
			AxisRotationCamera3D(DIRECTION_UP,		GetCursorMove().y * CAMERA3D_ROT_FORCE_BY_CURSOR.x);
			AxisRotationCamera3D(DIRECTION_LEFT,	GetCursorMove().x * CAMERA3D_ROT_FORCE_BY_CURSOR.y);
		}
		else
		{// ��������Y�����Ȃ����A
			if (GetKeyboardPress(DIK_UP		)){ AxisRotationCamera3D(DIRECTION_UP,		CAMERA3D_ROT_FORCE.x); }	// �㎲��]
			if (GetKeyboardPress(DIK_DOWN	)){ AxisRotationCamera3D(DIRECTION_DOWN,	CAMERA3D_ROT_FORCE.x); }	// ������]
			if (GetKeyboardPress(DIK_LEFT	)){ AxisRotationCamera3D(DIRECTION_LEFT,	CAMERA3D_ROT_FORCE.y); }	// ������]
			if (GetKeyboardPress(DIK_RIGHT	)){ AxisRotationCamera3D(DIRECTION_RIGHT,	CAMERA3D_ROT_FORCE.y); }	// �E����]
		}

		// �J����(3D)�̃|�C���^
		Camera3D *pCamera3D = GetCamera3D();

		pCamera3D->rot += pCamera3D->spin;				// �������X�V
		pCamera3D->spin *= CAMERA3D_SPIN_DAMP;			// ��]�ʂ�����
		pCamera3D->fHeight += pCamera3D->fVerticalMove;	// �������X�V
		pCamera3D->fVerticalMove *= CAMERA3D_SPIN_DAMP;	// �c�����̈ړ��ʂ�����

		// �����𐧌�
		RotControl(&pCamera3D->rot);

		// �����𐧌�
		FloatControl(&pCamera3D->fHeight, CAMERA3D_HEIGHT_MAX, CAMERA3D_HEIGHT_MIN);

		// ���_�̈ʒu��ݒ�
		pCamera3D->posV.x = pCamera3D->posR.x + (sinf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
		pCamera3D->posV.y = pCamera3D->posR.y + (pCamera3D->fLength * pCamera3D->fHeight);
		pCamera3D->posV.z = pCamera3D->posR.z + (cosf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
	}
}

//========================================
// SetTextModelViewer�֐� - ���f���r���[���̃e�L�X�g�ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetTextModelViewer(ModelViewer *pEdtr) 
{
	if (pEdtr->bShowText)
	{// �e�L�X�g�\���t���O
		char aString[TXT_MAX];	// ����p������

		for (int nCntText = 0; nCntText < MODELVIEWER_TEXT_MAX; nCntText++)
		{
			const TextInfoModelViewer txt = g_aTextInfoModelViewer[nCntText];
			Chr_model *pChr = GetChr_model();

			// �\��������ݒ�
			switch (nCntText)
			{
			case MODELVIEWER_TEXT_MODELTYPE:
				sprintf(aString, txt.aString, pChr->nType, GetChr_modelControl()->nTypeNum - 1);;
				break;
			case MODELVIEWER_TEXT_MOTIONNUMBER:
				sprintf(aString, txt.aString,pChr->nMotion, GetChr_modelType()[pChr->nType].motionSet.nMotionNum - 1);
				break;
			case MODELVIEWER_TEXT_MOTIONFRAME:
				sprintf(aString, txt.aString, pChr->partsInfo.nCounterMotion, GetChr_modelType()[pChr->nType].motionSet.aMotion[pChr->nMotion].nLoopTime);
				break;
			default:
				sprintf(aString, txt.aString);
				break;
			}

			SetText2D(aString, txt.nFont, txt.textDisp, txt.pos, INITCOLOR, txt.fScale, txt.fScale);
		}
	}
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================
//========================================
// LoadModelDataModelViewer�֐� - ���f���r���[���̃��f�����̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadModelDataModelViewer(ModelViewer *pEdtr)
{
	char fileName[TXT_MAX];	// �t�@�C�����i�[

	if (OpenFile(fileName, "data\\MODELDATA", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// CHR:���f���̓ǂݍ��ݏ���
		LoadChr_model(fileName);
		// CHR:���f���̏���������
		InitChr_model();
		// ���݂̃t�@�C������ݒ�
		sprintf(pEdtr->aNowFile, fileName);
	}
}

//========================================
// ReloadModelDataModelViewer�֐� - ���f���r���[���̃��f�����̍ēǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void ReloadModelDataModelViewer(ModelViewer *pEdtr)
{
	// CHR:���f���̓ǂݍ��ݏ���
	LoadChr_model(pEdtr->aNowFile);
	// CHR:���f���̏���������
	InitChr_model();
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// InitParameterModelViewer�֐� - ���f���r���[���̃p�����[�^�[�����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
ModelViewer InitParameterModelViewer(void)
{
	return{
		false,	// �|�[�Y�t���O
		true,	// �e�L�X�g�\���t���O
		true,	// �����蔻��\���t���O
		"",		// ���݂̃t�@�C����
	};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================
//========================================
// InitModelViewer�֐� - ���f���r���[���̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitModelViewer(void)
{
	// ���f���r���[���̃p�����[�^�[��������
	g_modelViewer3D = InitParameterModelViewer();
}

//========================================
// UodateModelViewer�֐� - ���f���r���[���̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateModelViewer(void)
{
	// ���f���r���[���̏��̃|�C���^
	ModelViewer *pEdtr = &g_modelViewer3D;

	// ���͏���
	InputModelViewer(pEdtr);

	// �e�L�X�g�ݒ菈��
	SetTextModelViewer(pEdtr);
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================
//========================================
// GetModelViewer�֐� - ���f���r���[���̃|�C���^���擾 -
// Author:RIKU NISHIMURA
//========================================
ModelViewer *GetModelViewer(void)
{
	return &g_modelViewer3D;
}