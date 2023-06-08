//========================================
// 
// �^�C�g�����S[00] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_title_logo_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "ui_title_logo_00.h"

//****************************************
// �}�N����`
//****************************************

// �^�C�g�����S[00] �̏o���ɂ����鎞��
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̈ړ��҂��ɂ����鎞��
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̈ړ��ɂ����鎞��
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̎l�p�`�̕ӂ̕�
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̎l�p�`�̏����l
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̎l�p�`�̍ŏI�l
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̓_�Ńp�^�[����
// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̓_�łɂ����鎞��
// �^�C�g�����S[00] �̈ʒu
// �^�C�g�����S[00] �̃v���Y�}�E�H�[��[00] �̈ʒu(����)
// �^�C�g�����S[00] �̃v���Y�}�E�H�[��[01] �̈ʒu(����)
// �^�C�g�����S[00] �̉e���̐F
#define UI_TITLE_LOGO_00_IN_POP_TIME					(80)
#define UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_WAIT_TIME		(40)
#define UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME			(MD_TITLE_00_TITLE_ANIM_TIME - UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_WAIT_TIME)
#define UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH				(PIXEL * 2)
#define UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE		Square{PIXEL * (-15 + 128),PIXEL * (15 + 128),PIXEL * -20 ,PIXEL * 20}
#define UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE			Square{PIXEL * -41,PIXEL * 45,PIXEL * -147,PIXEL * 146}
#define UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM	(2)
#define UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_TIME			(2)
#define UI_TITLE_LOGO_00_POS							(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, PIXEL * 72, 0.0f))
#define UI_TITLE_LOGO_00_SHADOW_COLOR					{0,0,0,255}

//****************************************
// �񋓌^�̒�`
//****************************************

// �^�C�g�����S[00] �̕��i�ԍ�
typedef enum
{
	UI_TITLE_LOGO_00_PARTS_BASE = 0,			// ���
	UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK,		// �v���Y�}�u���b�N
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00,		// �v���Y�}�E�H�[��[00]
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01,		// �v���Y�}�E�H�[��[01]
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP,		// �v���Y�}�E�H�[��(���)
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM,	// �v���Y�}�E�H�[��(����)
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT,	// �v���Y�}�E�H�[��(����)
	UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT,	// �v���Y�}�E�H�[��(�E��)
	UI_TITLE_LOGO_00_PARTS_MAX,
}UI_TITLE_LOGO_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �^�C�g�����S[00] �̃|���S�����̏��\����
typedef struct
{
	char			aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
					[TXT_MAX];		// 
	int				nPtnMaxX;		// �p�^�[���̏��(��)
	int				nPtnMaxY;		// �p�^�[���̏��(�c)
	int				nPtnMin;		// �p�^�[��No.�̉���
	int				nPtnMax;		// �p�^�[��No.�̏��
	int				nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nPolygonNum;	// �K�v�|���S����
	D3DXVECTOR3		relativePos;	// ���Έʒu

	int				nPtn;			// �p�^�[��No.
	int				nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Ui_title_logo_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterUi_title_logo_00(void);

// ���i(���)�̍X�V����
void UpdateUi_title_logo_00Parts_base(void);

// ���i(�v���Y�}�u���b�N)�̍X�V����
void UpdateUi_title_logo_00Parts_plasma_block(void);

// ���i(�v���Y�}�E�H�[��[00])�̍X�V����
void UpdateUi_title_logo_00Parts_plasma_wall_00(void);

// ���i(�v���Y�}�E�H�[��[01])�̍X�V����
void UpdateUi_title_logo_00Parts_plasma_wall_01(void);

// ���i(�v���Y�}�E�H�[��(��))�̍X�V����
void UpdateUi_title_logo_00Parts_plasma_wall_top(void);

// ��Ԃɉ������J�n����
void StartStateProcessUi_title_logo_00(void);

// ��Ԃɉ������I������
void EndStateProcessUi_title_logo_00(void);

// ��Ԃɉ������X�V����
void UpdateStateProcessUi_title_logo_00(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_title_logo_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_title_logo_00		// �e�N�X�`���ւ̃|�C���^
						[UI_TITLE_LOGO_00_PARTS_MAX]	//
						= {};							//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_title_logo_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;							//
Ui_title_logo_00		g_ui_title_logo_00;				// �^�C�g�����S[00] �̏��

// �^�C�g�����S[00] �̕��i���̏��
Ui_title_logo_00Parts g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\ui_title_logo_000_base.png"     , 1 , 1 , 0 , 0 , -1 , PIXEL * 512 , PIXEL * 128 , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ "data\\TEXTURE\\ui_title_logo_000_parts_00.png" , 4 , 2 , 0 , 7 , 4  , PIXEL * 16  , PIXEL * 16  , 1 , D3DXVECTOR3(PIXEL * -101, PIXEL * 45 , 0.0f) },
	{ "data\\TEXTURE\\ui_title_logo_000_parts_01.png" , 2 , 2 , 0 , 3 , 6  , PIXEL * 64  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * 133 , PIXEL * -34, 0.0f) },
	{ "data\\TEXTURE\\ui_title_logo_000_parts_02.png" , 2 , 2 , 0 , 3 , 6  , PIXEL * 64  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * -133, PIXEL * 38 , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
	{ ""											  , 1 , 1 , 0 , 0 , -1 , PIXEL * 0   , PIXEL * 0   , 1 , D3DXVECTOR3(PIXEL * 0   , PIXEL * 0  , 0.0f) },
};

// �^�C�g�����S[00] �̃v���Y�}�E�H�[���̓_�ŐF
Color g_aUi_title_logo_00PlasmaWallBrinkColor[UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM] =
{
	{ 255,218,171,255 },
	{ 255,153,59 ,255 },
};

//========== *** �^�C�g�����S[00] �����擾 ***
Ui_title_logo_00 *GetUi_title_logo_00(void)
{
	return &g_ui_title_logo_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_title_logo_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_title_logo_00(void)
{
	g_ui_title_logo_00.nCounterState	= 0;											// ��ԃJ�E���^�[
	g_ui_title_logo_00.fAlpha			= 0.0f;											// �S�̂̓����x
	g_ui_title_logo_00.pos				= UI_TITLE_LOGO_00_POS;							// �ʒu
	g_ui_title_logo_00.square			= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE;	// �l�p�`�̏��
}

//========================================
// UpdateUi_title_logo_00Parts_base�֐� - ���i(���)�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_base(void)
{
	Ui_title_logo_00Parts	*pParts							// 
							= &g_aUi_title_logo_00Parts		//
							[UI_TITLE_LOGO_00_PARTS_BASE];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_BASE));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_BASE].nPtnMaxY,
			NULL);

		if ((GetMd_title_00()->state == MD_TITLE_00_STATE_TEAM_LOGO)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_TITLE_ANIM)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_FLASH))
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�^�C�g���A�j���[�V����/�t���b�V���̂����ꂩ�̎��A
			// ���_�J���[���e���̂��̂ɐݒ�
			SetVertexColor(pVtx, UI_TITLE_LOGO_00_SHADOW_COLOR);
		}
		else
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���̂����ꂩ�ł��Ȃ����A
			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });
		}
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_block�֐� - ���i(�v���Y�}�u���b�N)�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_block(void)
{
	if (!g_ui_title_logo_00.bActive) 
	{// �񓮓I�̎��A
		// �������I������
		return;
	}

	Ui_title_logo_00Parts	*pParts									// 
							= &g_aUi_title_logo_00Parts				//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;									// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_00�֐� - ���i(�v���Y�}�E�H�[��[00])�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_00(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;										// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nAnimTime,
				ANIM_TYPE_TURN),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_01�֐� - ���i(�v���Y�}�E�H�[��[01])�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_01(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;										// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].fWidth,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtn,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMin,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMax,
				&g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nCounterAnim,
				g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nAnimTime,
				ANIM_TYPE_TURN),
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMaxX,
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01].nPtnMaxY,
			NULL);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_top�֐� - ���i(�v���Y�}�E�H�[��(��))�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_top(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;										// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3((g_ui_title_logo_00.square.fLeft + g_ui_title_logo_00.square.fRight) * 0.5f, g_ui_title_logo_00.square.fTop, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_ui_title_logo_00.square.fRight - g_ui_title_logo_00.square.fLeft,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_bottom�֐� - ���i(�v���Y�}�E�H�[��(��))�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_bottom(void)
{
	Ui_title_logo_00Parts	*pParts											// 
							= &g_aUi_title_logo_00Parts						//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;											// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3((g_ui_title_logo_00.square.fLeft + g_ui_title_logo_00.square.fRight) * 0.5f, g_ui_title_logo_00.square.fBottom, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_ui_title_logo_00.square.fRight - g_ui_title_logo_00.square.fLeft,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_left�֐� - ���i(�v���Y�}�E�H�[��(��))�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_left(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;										// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3(g_ui_title_logo_00.square.fLeft, (g_ui_title_logo_00.square.fTop + g_ui_title_logo_00.square.fBottom) * 0.5f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			g_ui_title_logo_00.square.fBottom - g_ui_title_logo_00.square.fTop,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// UpdateUi_title_logo_00Parts_plasma_wall_right�֐� - ���i(�v���Y�}�E�H�[��(�E))�̍X�V���� -
//========================================
void UpdateUi_title_logo_00Parts_plasma_wall_right(void)
{
	Ui_title_logo_00Parts	*pParts										// 
							= &g_aUi_title_logo_00Parts					//
							[UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT];	// ���i�̏��̃|�C���^
	VERTEX_2D				*pVtx;										// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_title_logo_00.pos + D3DXVECTOR3(g_ui_title_logo_00.square.fRight, (g_ui_title_logo_00.square.fTop + g_ui_title_logo_00.square.fBottom) * 0.5f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			UI_TITLE_LOGO_00_PLASMA_WALL_WIDTH,
			g_ui_title_logo_00.square.fBottom - g_ui_title_logo_00.square.fTop,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, g_aUi_title_logo_00PlasmaWallBrinkColor[g_ui_title_logo_00.nCntBrink]);
	}
}

//========================================
// StartStateProcessUi_title_logo_00�֐� - ��Ԃɉ������J�n���� -
//========================================
void StartStateProcessUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					// 
						= &g_ui_title_logo_00;	// �^�C�g�����S[00] �̏��̃|�C���^ 

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_TITLE_LOGO_00_STATE_POP:
	{
		// �^�C�g�����S[00] �̈ʒu����
		pUi->pos = UI_TITLE_LOGO_00_POS;

		// �S�Ă̕��i�̃A�j���[�V�����̃J�E���^�[��������
		for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
		{
			g_aUi_title_logo_00Parts[UI_TITLE_LOGO_00_PARTS_MAX].nCounterAnim = 0;
		}
	}
	break;
		//========== *** �o���� ***
	case UI_TITLE_LOGO_00_STATE_IN_POP:
	{
		
	}
	break;
		//========== *** �����Ă��� ***
	case UI_TITLE_LOGO_00_STATE_CLEAR:
	{
		
	}
	break;
		//========== *** �v���Y�}�E�H�[���̈ړ� ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE:
	{
		// �l�p�`�̏��������̂��̂ɂ���
		g_ui_title_logo_00.square = UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE;
	}
		break;
		//========== *** �v���Y�}�E�H�[���̈ړ��҂� ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT:
	{
		// �l�p�`�̏��������̂��̂ɂ���
		g_ui_title_logo_00.square = UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE;

		// �_�ŃJ�E���g�Ɠ_�ŃJ�E���^�[������������
		g_ui_title_logo_00.nCntBrink = 0;
		g_ui_title_logo_00.nCounterBrink = 0;
	}
	break;
	}
}

//========================================
// EndStateProcessUi_title_logo_00�֐� - ��Ԃɉ������I������ -
//========================================
void EndStateProcessUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					// 
						= &g_ui_title_logo_00;	// �^�C�g�����S[00] �̏��̃|�C���^ 

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_TITLE_LOGO_00_STATE_POP:
	{

	}
	break;
		//========== *** �o���� ***
	case UI_TITLE_LOGO_00_STATE_IN_POP:
	{

	}
	break;
		//========== *** �����Ă��� ***
	case UI_TITLE_LOGO_00_STATE_CLEAR:
	{

	}
	break;
		//========== *** �v���Y�}�E�H�[���̈ړ� ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE:
	{
		
	}
	break;
		//========== *** �v���Y�}�E�H�[���̈ړ��҂� ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT:
	{

	}
	break;
	}
}

//========================================
// UpdateStateProcessUi_title_logo_00�֐� - ��Ԃɉ������X�V���� -
//========================================
void UpdateStateProcessUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					// 
						= &g_ui_title_logo_00;	// �^�C�g�����S[00] �̏��̃|�C���^ 

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_TITLE_LOGO_00_STATE_POP:
	{

	}
	break;
		//========== *** �o���� ***
	case UI_TITLE_LOGO_00_STATE_IN_POP:
	{
		if (++pUi->nCounterState >= UI_TITLE_LOGO_00_IN_POP_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʏo���ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_POP);
		}
	}
	break;
		//========== *** �����Ă��� ***
	case UI_TITLE_LOGO_00_STATE_CLEAR:
	{

	}
	break;
		//========== *** �v���Y�}�E�H�[���̈ړ� ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE:
	{
		// �l�p�`�̏��Ƀv���Y�}�E�H�[���̎l�p�`�̏����̏����J�E���^�[�ɉ������������
		g_ui_title_logo_00.square.fTop		= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fTop	* (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));
		g_ui_title_logo_00.square.fBottom	= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fBottom * (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));
		g_ui_title_logo_00.square.fLeft		= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fLeft	* (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));
		g_ui_title_logo_00.square.fRight	= UI_TITLE_LOGO_00_PLASMA_WALL_START_SQUARE.fRight	* (1.0f - ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME));

		// �l�p�`�̏��Ƀv���Y�}�E�H�[���̎l�p�`�̍ŏI�̏����J�E���^�[�ɉ������������Z
		g_ui_title_logo_00.square.fTop		+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fTop		* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);
		g_ui_title_logo_00.square.fBottom	+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fBottom	* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);
		g_ui_title_logo_00.square.fLeft		+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fLeft	* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);
		g_ui_title_logo_00.square.fRight	+= UI_TITLE_LOGO_00_PLASMA_WALL_END_SQUARE.fRight	* ((float)pUi->nCounterState / (float)UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME);

		if (++pUi->nCounterBrink >= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_TIME)
		{// �_�ŃJ�E���^�[�����Z�������ʓ_�łɂ����鎞�ԂɒB�������A
			// �_�ŃJ�E���^�[�����������A�_�ŃJ�E���g���X�V
			pUi->nCounterBrink = 0;
			++pUi->nCntBrink %= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM;
		}

		if (++pUi->nCounterState >= UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃv���Y�}�E�H�[���̈ړ��ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_POP);
		}
	}
	break;
		//========== *** �v���Y�}�E�H�[���̈ړ��҂� ***
	case UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT:
	{
		if (++pUi->nCounterBrink >= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_TIME)
		{// �_�ŃJ�E���^�[�����Z�������ʓ_�łɂ����鎞�ԂɒB�������A
			// �_�ŃJ�E���^�[�����������A�_�ŃJ�E���g���X�V
			pUi->nCounterBrink = 0;
			++pUi->nCntBrink %= UI_TITLE_LOGO_00_PLASMA_WALL_BRINK_PATTERN_NUM;
		}

		if (++pUi->nCounterState >= UI_TITLE_LOGO_00_PLASMA_WALL_MOVE_WAIT_TIME)
		{// ��ԃJ�E���^�[�����Z�������ʃv���Y�}�E�H�[���̈ړ��҂��ɂ����鎞�ԂɒB�������A
			// �v���Y�}�E�H�[���̈ړ���Ԃɂ���
			SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE);
		}
	}
	break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_title_logo_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_title_logo_00Parts[nCntPolygon].nPolygonNum;
	}

	// �K�v�|���S������Ԃ�
	return nPolygonNum;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_title_logo_00�֐� - �^�C�g�����S[00] �̏��������� -
//========================================
void InitUi_title_logo_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_title_logo_00();

	// �����Ă����Ԃɂ���
	SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE_CLEAR);

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_title_logo_00(UI_TITLE_LOGO_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_title_logo_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_title_logo_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_title_logo_00Parts[nCntParts].aTexturePath, &g_aTextureUi_title_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_title_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_title_logo_00Parts[nCntParts].fWidth,
				g_aUi_title_logo_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx,
				0,
				g_aUi_title_logo_00Parts[nCntParts].nPtnMaxX,
				g_aUi_title_logo_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_title_logo_00->Unlock();
}

//========================================
// UninitUi_title_logo_00�֐� - �^�C�g�����S[00] �̏I������ -
//========================================
void UninitUi_title_logo_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_title_logo_00[nCntParts] != NULL)
		{
			g_aTextureUi_title_logo_00[nCntParts]->Release();
			g_aTextureUi_title_logo_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_title_logo_00 != NULL)
	{
		g_pVtxBuffUi_title_logo_00->Release();
		g_pVtxBuffUi_title_logo_00 = NULL;
	}
}

//========================================
// UpdateUi_title_logo_00�֐� - �^�C�g�����S[00] �̍X�V���� -
//========================================
void UpdateUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					//
						= &g_ui_title_logo_00;	// �^�C�g�����S[00] �̏��̃|�C���^

	if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
	{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���łȂ����A
		// ���I�t���O��^�ɂ���
		pUi->bActive = true;
	}
	else 
	{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���̂����ꂩ�ł��鎞�A
		// ���I�t���O���U�ɂ���
		pUi->bActive = false;
	}

	// ��ԏ���
	UpdateStateProcessUi_title_logo_00();

	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
		case UI_TITLE_LOGO_00_PARTS_BASE				:UpdateUi_title_logo_00Parts_base()					; break; // ���i(���)�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_BLOCK		:UpdateUi_title_logo_00Parts_plasma_block()			; break; // ���i(�v���Y�}�u���b�N)�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_00		:UpdateUi_title_logo_00Parts_plasma_wall_00()		; break; // ���i(�v���Y�}�u���b�N[00])�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_01		:UpdateUi_title_logo_00Parts_plasma_wall_01()		; break; // ���i(�v���Y�}�u���b�N[01])�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP		:UpdateUi_title_logo_00Parts_plasma_wall_top()		; break; // ���i(�v���Y�}�u���b�N(��))�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM	:UpdateUi_title_logo_00Parts_plasma_wall_bottom()	; break; // ���i(�v���Y�}�u���b�N(��))�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT	:UpdateUi_title_logo_00Parts_plasma_wall_left()		; break; // ���i(�v���Y�}�u���b�N(��))�̍X�V����
		case UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT	:UpdateUi_title_logo_00Parts_plasma_wall_right()	; break; // ���i(�v���Y�}�u���b�N(�E))�̍X�V����
		}
	}
}

//========================================
// DrawUi_title_logo_00�֐� - �^�C�g�����S[00] �̕`�揈�� -
//========================================
void DrawUi_title_logo_00(void)
{
	Ui_title_logo_00	*pUi					//
						= &g_ui_title_logo_00;	// �^�C�g�����S[00] �̏��̃|�C���^

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_title_logo_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < UI_TITLE_LOGO_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_title_logo_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_title_logo_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			if (pUi->state != UI_TITLE_LOGO_00_STATE_POP)
			{// �o����ԂłȂ����A
				if (((pUi->state == UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE)
					|| 
					(pUi->state == UI_TITLE_LOGO_00_STATE_PLASMA_WALL_MOVE_WAIT))
					&&
					((nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT)))
				{// �v���Y�}�E�H�[���̈ړ����������͈ړ��҂��ł��J�E���g���ړ����Ă���v���Y�}�E�H�[���ɊY�����鎞�A
					
				}
				else 
				{// �v���Y�}�E�H�[���̈ړ����łȂ��A�������̓J�E���g���ړ����Ă���v���Y�}�E�H�[���ɊY�����Ȃ����A
					// ������܂�Ԃ�
					continue;
				}
			}
			else 
			{// �o����Ԃ̎��A
				if ((nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_TOP)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_BOTTOM)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_LEFT)
					||
					(nCntParts == UI_TITLE_LOGO_00_PARTS_PLASMA_WALL_RIGHT))
				{// �J�E���g���ړ����Ă���v���Y�}�E�H�[���ɊY�����鎞�A
					// ������܂�Ԃ�
					continue;
				}
			}

			// �^�C�g�����S[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState�֐� - �^�C�g�����S[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_title_logo_00(UI_TITLE_LOGO_00_STATE state)
{
	Ui_title_logo_00	*pUi					//
						= &g_ui_title_logo_00;	// �^�C�g�����S[00] �̏��̃|�C���^ 

	// ��Ԃɉ������I������
	EndStateProcessUi_title_logo_00();

	// ��Ԃ���
	pUi->state = state;

	// ��Ԃɉ������J�n����
	StartStateProcessUi_title_logo_00();

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}