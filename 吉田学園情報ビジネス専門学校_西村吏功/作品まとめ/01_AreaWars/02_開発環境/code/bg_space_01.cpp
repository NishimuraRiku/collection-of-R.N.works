//========================================
// 
// �F��[02] �̏���
// Author:���� ����
// 
//========================================
//  *** bg_space_01.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "bg_space_01.h"

//****************************************
// �}�N����`
//****************************************

// �F��[01] �̏o���ɂ����鎞��
// �F��[01] �̏�����̂ɂ����鎞��
// �F��[01] �̓����x�̍ő�l
// �F��[01] �̊J�n�ʒu
// �F��[01] �̈ړ��p�x
#define BG_SPACE_01_IN_POP_TIME		(10)
#define BG_SPACE_01_IN_CLEAR_TIME	(10)
#define BG_SPACE_01_ALPHA_MAX		(1.0f)
#define BG_SPACE_01_START_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f))
#define BG_SPACE_01_MOVE_ANGLE		(D3DX_PI * 0.7f)

//****************************************
// �񋓌^�̒�`
//****************************************

// �F��[01] �̕��i�ԍ�
typedef enum
{
	BG_SPACE_01_PARTS_SPACE_00 = 0,	// �F��[00]
	BG_SPACE_01_PARTS_SPACE_01,		// �F��[01]
	BG_SPACE_01_PARTS_SPACE_02,		// �F��[02]
	BG_SPACE_01_PARTS_SPACE_03,		// �F��[03]
	BG_SPACE_01_PARTS_MAX,
}BG_SPACE_01_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �F��[01] �̃|���S�����̏��\����
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
	D3DXVECTOR3		textureMove;	// �e�N�X�`���̈ړ���

	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		texturePos;		// �e�N�X�`���̈ʒu
	int				nPtn;			// �p�^�[��No.
	int				nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Bg_space_01Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �F��[01] �̃p�����[�^�[�̏���������
void InitParameterBg_space_01(void);

// ���i(�F��[00])�̍X�V����
void UpdateBg_space_01Parts_space_00(void);

// ���i(�F��[01])�̍X�V����
void UpdateBg_space_01Parts_space_01(void);

// ���i(�F��[02])�̍X�V����
void UpdateBg_space_01Parts_space_02(void);

// ���i(�F��[03])�̍X�V����
void UpdateBg_space_01Parts_space_03(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumBg_space_01(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureBg_space_01	// �e�N�X�`���ւ̃|�C���^
						[BG_SPACE_01_PARTS_MAX]	//
						= {};					//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_space_01	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;					//

// �F��[01] �̕��i���̏��
Bg_space_01Parts g_aBg_space_01Parts[BG_SPACE_01_PARTS_MAX] =
{
	{ "data\\TEXTURE\\bg_space_000_03.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.01f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.01f, 0.0f) },
	{ "data\\TEXTURE\\bg_space_000_02.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.16f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.16f, 0.0f) },
	{ "data\\TEXTURE\\bg_space_000_01.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.32f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.32f, 0.0f) },
	{ "data\\TEXTURE\\bg_space_000_00.png" , 1 , 1 , 0 , 0 , -1 , PIXEL * 1024 , PIXEL * 1024 , 1 , D3DXVECTOR3(sinf(BG_SPACE_01_MOVE_ANGLE) * 0.64f, cosf(BG_SPACE_01_MOVE_ANGLE) * 0.64f, 0.0f) },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_space_01�֐� - �F��[01] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterBg_space_01(void) 
{
	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX;nCntParts++) 
	{
		g_aBg_space_01Parts[nCntParts].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aBg_space_01Parts[nCntParts].texturePos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �e�N�X�`���̈ʒu
		g_aBg_space_01Parts[nCntParts].nPtn			= 0;								// �p�^�[��No.
		g_aBg_space_01Parts[nCntParts].nCounterAnim = 0;								// �A�j���[�V�����̃J�E���^�[
	}
}

//========================================
// UpdateBg_space_01Parts_space_00�֐� - ���i(�F��[00])�̍X�V���� -
//========================================
void UpdateBg_space_01Parts_space_00(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_00];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_00));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���łȂ����A
			// �e�N�X�`���̈ʒu���X�V
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fHeight;
		}

		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_00].texturePos);
	}
}

//========================================
// UpdateBg_space_01Parts_space_01�֐� - ���i(�F��[01])�̍X�V���� -
//========================================
void UpdateBg_space_01Parts_space_01(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_01];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_01));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���łȂ����A
			// �e�N�X�`���̈ʒu���X�V
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fHeight;
		}

		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_01].texturePos);
	}
}

//========================================
// UpdateBg_space_01Parts_space_02�֐� - ���i(�F��[02])�̍X�V���� -
//========================================
void UpdateBg_space_01Parts_space_02(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_02];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_02));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���łȂ����A
			// �e�N�X�`���̈ʒu���X�V
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fHeight;
		}

		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_02].texturePos);
	}
}

//========================================
// UpdateBg_space_01Parts_space_03�֐� - ���i(�F��[03])�̍X�V���� -
//========================================
void UpdateBg_space_01Parts_space_03(void)
{
	Bg_space_01Parts	*pParts							// 
						= &g_aBg_space_01Parts			//
						[BG_SPACE_01_PARTS_SPACE_03];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_SPACE_03));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
			&&
			(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���łȂ����A
			// �e�N�X�`���̈ʒu���X�V
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].texturePos.x
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].textureMove.x / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fWidth;
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].texturePos.y
				+= g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].textureMove.y / g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fHeight;
		}

		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fWidth,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtn,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMin,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMax,
				&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nCounterAnim,
				g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMaxX,
			g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].nPtnMaxY,
			&g_aBg_space_01Parts[BG_SPACE_01_PARTS_SPACE_03].texturePos);
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumBg_space_01(int nParts) 
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aBg_space_01Parts[nCntPolygon].nPolygonNum;
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
// InitBg_space_01�֐� - �F��[01] �̏��������� -
//========================================
void InitBg_space_01(void)
{
	// �p�����[�^�[�̏���������
	InitParameterBg_space_01();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumBg_space_01(BG_SPACE_01_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_space_01,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_space_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++)
	{
		// �ʒu��������
		g_aBg_space_01Parts[nCntParts].pos = BG_SPACE_01_START_POS;

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aBg_space_01Parts[nCntParts].aTexturePath, &g_aTextureBg_space_01[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aBg_space_01Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aBg_space_01Parts[nCntParts].fWidth,
				g_aBg_space_01Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx, 
				0, 
				g_aBg_space_01Parts[nCntParts].nPtnMaxX, 
				g_aBg_space_01Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffBg_space_01->Unlock();
}

//========================================
// UninitBg_space_01�֐� - �F��[01] �̏I������ -
//========================================
void UninitBg_space_01(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureBg_space_01[nCntParts] != NULL)
		{
			g_aTextureBg_space_01[nCntParts]->Release();
			g_aTextureBg_space_01[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg_space_01 != NULL) 
	{
		g_pVtxBuffBg_space_01->Release();
		g_pVtxBuffBg_space_01 = NULL;
	}
}

//========================================
// UpdateBg_space_01�֐� - �F��[01] �̍X�V���� -
//========================================
void UpdateBg_space_01(void) 
{
	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++) 
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** �F��[00] ***
		case BG_SPACE_01_PARTS_SPACE_00:
		{
			// ���i(�F��[00])�̍X�V����
			UpdateBg_space_01Parts_space_00();
		}
			break;
			//========== *** �F��[01] ***
		case BG_SPACE_01_PARTS_SPACE_01:
		{
			// ���i(�F��[01])�̍X�V����
			UpdateBg_space_01Parts_space_01();
		}
		break;
			//========== *** �F��[02] ***
		case BG_SPACE_01_PARTS_SPACE_02:
		{
			// ���i(�F��[02])�̍X�V����
			UpdateBg_space_01Parts_space_02();
		}
		break;
			//========== *** �F��[03] ***
		case BG_SPACE_01_PARTS_SPACE_03:
		{
			// ���i(�F��[03])�̍X�V����
			UpdateBg_space_01Parts_space_03();
		}
		break;
		}
	}
}

//========================================
// DrawBg_space_01�֐� - �F��[01] �̕`�揈�� -
//========================================
void DrawBg_space_01(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg_space_01, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < BG_SPACE_01_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureBg_space_01[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aBg_space_01Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �F��[02] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}