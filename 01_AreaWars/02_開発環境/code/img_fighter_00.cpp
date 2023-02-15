//========================================
// 
// �@��[00] �̏���
// Author:���� ����
// 
//========================================
//  *** img_fighter_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_title_00.h"
#include "img_fighter_00.h"

//****************************************
// �}�N����`
//****************************************

// �@��[00] �̏o���ɂ����鎞��
// �@��[00] �̈ʒu
// �@��[00] �̉��̓_�łɂ����鎞��
// �@��[00] �̉e���̐F
#define IMG_FIGHTER_00_IN_POP_TIME		(10)
#define IMG_FIGHTER_00_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 16), 0.0f))
#define IMG_FIGHTER_00_FIRE_BRINK_TIME	(1)
#define IMG_FIGHTER_00_SHADOW_COLOR		{0,0,0,255}

//****************************************
// �񋓌^�̒�`
//****************************************

// �@��[00] �̕��i�ԍ�
typedef enum
{
	IMG_FIGHTER_00_PARTS_FIRE_LEFT= 0,	// ��(��)
	IMG_FIGHTER_00_PARTS_FIRE_RIGHT,	// ��(�E)
	IMG_FIGHTER_00_PARTS_BODY,			// �{��
	IMG_FIGHTER_00_PARTS_MAX,
}IMG_FIGHTER_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �@��[00] �̃|���S�����̏��\����
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
}Img_fighter_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterImg_fighter_00(void);

// ���i(�{��)�̍X�V����
void UpdateImg_fighter_00Parts_body(void);

// ���i(��)�̍X�V����
void UpdateImg_fighter_00Parts_fire_left(void);

// ���i(�E)�̍X�V����
void UpdateImg_fighter_00Parts_fire_right(void);

// ��Ԃɉ������J�n����
void StartStateProcessImg_fighter_00(void);

// ��Ԃɉ������I������
void EndStateProcessImg_fighter_00(void);

// ��Ԃɉ������X�V����
void UpdateStateProcessImg_fighter_00(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumImg_fighter_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureImg_fighter_00			// �e�N�X�`���ւ̃|�C���^
						[IMG_FIGHTER_00_PARTS_MAX]			//
						= {};								//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffImg_fighter_00			// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;								//
Img_fighter_00			g_img_fighter_00;					// �@��[00] �̏��

// �@��[00] �̕��i���̏��
Img_fighter_00Parts g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\img_fighter_000_fire.png" , 1 , 1 , 0 , 0 , -1, PIXEL * 32  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * -2, PIXEL * -74, 0.0f) },
	{ "data\\TEXTURE\\img_fighter_000_fire.png" , 1 , 1 , 0 , 0 , -1, PIXEL * 32  , PIXEL * 32  , 1 , D3DXVECTOR3(PIXEL * 88, PIXEL * -44, 0.0f) },
	{ "data\\TEXTURE\\img_fighter_000.png"      , 4 , 2 , 0 , 5 , 6 , PIXEL * 256 , PIXEL * 256 , 1 , D3DXVECTOR3(PIXEL * 0 , PIXEL * 0  , 0.0f) },
};

//========== *** �@��[00] �����擾 ***
Img_fighter_00 *GetImg_fighter_00(void)
{
	return &g_img_fighter_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterImg_fighter_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterImg_fighter_00(void)
{
	g_img_fighter_00.nCounterState		= 0;	// ��ԃJ�E���^�[
	g_img_fighter_00.fAlpha				= 0.0f;	// �S�̂̓����x
	g_img_fighter_00.nCounterFireBrink	= 0;	// ���̓_�ŃJ�E���^�[
	g_img_fighter_00.bFireBrink			= true;	// ���̓_�Ńt���O
}

//========================================
// UpdateImg_fighter_00Parts_body�֐� - ���i(�{��)�̍X�V���� -
//========================================
void UpdateImg_fighter_00Parts_body(void)
{
	Img_fighter_00Parts	*pParts							// 
						= &g_aImg_fighter_00Parts		//
						[IMG_FIGHTER_00_PARTS_BODY];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_BODY));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_img_fighter_00.pos + g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].fWidth,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtn,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMin,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMax,
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nCounterAnim,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nAnimTime,
				ANIM_TYPE_TURN),
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMaxX,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_BODY].nPtnMaxY,
			NULL);

		if ((GetMd_title_00()->state == MD_TITLE_00_STATE_TEAM_LOGO)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_TITLE_ANIM)
			||
			(GetMd_title_00()->state == MD_TITLE_00_STATE_FLASH))
		{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���̂����ꂩ�ł��鎞�A
			// ���_�J���[���e���̂��̂ɐݒ�
			SetVertexColor(pVtx, IMG_FIGHTER_00_SHADOW_COLOR);
		}
		else 
		{
			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });
		}
	}
}

//========================================
// UpdateImg_fighter_00Parts_fire_left�֐� - ���i(��(��))�̍X�V���� -
//========================================
void UpdateImg_fighter_00Parts_fire_left(void)
{
	Img_fighter_00Parts	*pParts								// 
						= &g_aImg_fighter_00Parts			//
						[IMG_FIGHTER_00_PARTS_FIRE_LEFT];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;								// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_FIRE_LEFT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_img_fighter_00.pos + g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].fWidth,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtn,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMin,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMax,
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nCounterAnim,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nAnimTime,
				ANIM_TYPE_TURN),
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMaxX,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_LEFT].nPtnMaxY,
			NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,255 });
	}
}

//========================================
// UpdateImg_fighter_00Parts_fire_right�֐� - ���i(��(�E))�̍X�V���� -
//========================================
void UpdateImg_fighter_00Parts_fire_right(void)
{
	Img_fighter_00Parts	*pParts								// 
						= &g_aImg_fighter_00Parts			//
						[IMG_FIGHTER_00_PARTS_FIRE_RIGHT];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;								// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_FIRE_RIGHT));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_img_fighter_00.pos + g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].fWidth,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtn,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMin,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMax,
				&g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nCounterAnim,
				g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nAnimTime,
				ANIM_TYPE_TURN),
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMaxX,
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_FIRE_RIGHT].nPtnMaxY,
			NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,255 });
	}
}

//========================================
// StartStateProcessImg_fighter_00�֐� - ��Ԃɉ������J�n���� -
//========================================
void StartStateProcessImg_fighter_00(void)
{
	Img_fighter_00	*pUi					// 
					= &g_img_fighter_00;	// �@��[00] �̏��̃|�C���^ 

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case IMG_FIGHTER_00_STATE_POP:
	{
		// �@��[00] �̈ʒu����
		pUi->pos = IMG_FIGHTER_00_POS;
	}
	break;
		//========== *** �o���� ***
	case IMG_FIGHTER_00_STATE_IN_POP:
	{
		// �S�Ă̕��i�̃A�j���[�V�����̃J�E���^�[��������
		for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
		{
			g_aImg_fighter_00Parts[IMG_FIGHTER_00_PARTS_MAX].nCounterAnim = 0;
		}
	}
	break;
	}
}

//========================================
// EndStateProcessImg_fighter_00�֐� - ��Ԃɉ������I������ -
//========================================
void EndStateProcessImg_fighter_00(void)
{
	Img_fighter_00	*pUi					// 
					= &g_img_fighter_00;	// �@��[00] �̏��̃|�C���^ 

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case IMG_FIGHTER_00_STATE_POP:
	{

	}
	break;
		//========== *** �o���� ***
	case IMG_FIGHTER_00_STATE_IN_POP:
	{

	}
	break;
	}
}

//========================================
// UpdateStateProcessImg_fighter_00�֐� - ��Ԃɉ������X�V���� -
//========================================
void UpdateStateProcessImg_fighter_00(void)
{
	Img_fighter_00	*pUi					// 
					= &g_img_fighter_00;	// �@��[00] �̏��̃|�C���^ 

	if ((GetMd_title_00()->state != MD_TITLE_00_STATE_TEAM_LOGO)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_TITLE_ANIM)
		&&
		(GetMd_title_00()->state != MD_TITLE_00_STATE_FLASH))
	{// �^�C�g�����[00] �̏�Ԃ��`�[�����S/�I�[�v�j���O/�t���b�V���łȂ����A
		if (++pUi->nCounterFireBrink >= IMG_FIGHTER_00_FIRE_BRINK_TIME)
		{// ���̓_�ŃJ�E���^�[�����̓_�łɂ����鎞�ԂɒB�������A
			// ���̓_�ŃJ�E���^�[������������
			pUi->nCounterFireBrink = 0;

			// ���̓_�Ńt���O��؂�ւ���
			pUi->bFireBrink ^= 1;
		}
	}

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case IMG_FIGHTER_00_STATE_POP:
	{

	}
	break;
		//========== *** �o���� ***
	case IMG_FIGHTER_00_STATE_IN_POP:
	{
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		if (pUi->nCounterState >= IMG_FIGHTER_00_IN_POP_TIME)
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateImg_fighter_00(IMG_FIGHTER_00_STATE_POP);
		}
	}
	break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumImg_fighter_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aImg_fighter_00Parts[nCntPolygon].nPolygonNum;
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
// InitImg_fighter_00�֐� - �@��[00] �̏��������� -
//========================================
void InitImg_fighter_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterImg_fighter_00();

	// ��Ԑݒ菈��
	SetStateImg_fighter_00(IMG_FIGHTER_00_STATE_POP);

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumImg_fighter_00(IMG_FIGHTER_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffImg_fighter_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImg_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aImg_fighter_00Parts[nCntParts].aTexturePath, &g_aTextureImg_fighter_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aImg_fighter_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aImg_fighter_00Parts[nCntParts].fWidth,
				g_aImg_fighter_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx,
				0,
				g_aImg_fighter_00Parts[nCntParts].nPtnMaxX,
				g_aImg_fighter_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffImg_fighter_00->Unlock();
}

//========================================
// UninitImg_fighter_00�֐� - �@��[00] �̏I������ -
//========================================
void UninitImg_fighter_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureImg_fighter_00[nCntParts] != NULL)
		{
			g_aTextureImg_fighter_00[nCntParts]->Release();
			g_aTextureImg_fighter_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffImg_fighter_00 != NULL)
	{
		g_pVtxBuffImg_fighter_00->Release();
		g_pVtxBuffImg_fighter_00 = NULL;
	}
}

//========================================
// UpdateImg_fighter_00�֐� - �@��[00] �̍X�V���� -
//========================================
void UpdateImg_fighter_00(void)
{
	Img_fighter_00	*pUi					//
					= &g_img_fighter_00;	// �@��[00] �̏��̃|�C���^

	// ��ԏ���
	UpdateStateProcessImg_fighter_00();

	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** �{�̂̐��� ***
		case IMG_FIGHTER_00_PARTS_BODY:
		{
			// ���i(�{�̂̐���)�̍X�V����
			UpdateImg_fighter_00Parts_body();
		}
		break;
			//========== *** ��(��) ***
		case IMG_FIGHTER_00_PARTS_FIRE_LEFT:
		{
			// ���i(��(��))�̍X�V����
			UpdateImg_fighter_00Parts_fire_left();
		}
		break;
			//========== *** ��(�E) ***
		case IMG_FIGHTER_00_PARTS_FIRE_RIGHT:
		{
			// ���i(��(�E))�̍X�V����
			UpdateImg_fighter_00Parts_fire_right();
		}
		break;
		}
	}
}

//========================================
// DrawImg_fighter_00�֐� - �@��[00] �̕`�揈�� -
//========================================
void DrawImg_fighter_00(void)
{
	Img_fighter_00	*pUi					//
					= &g_img_fighter_00;	// �@��[00] �̏��̃|�C���^

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffImg_fighter_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < IMG_FIGHTER_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureImg_fighter_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aImg_fighter_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			if ((pUi->bFireBrink)
				&&
				((nCntParts == IMG_FIGHTER_00_PARTS_FIRE_LEFT)
				||
				(nCntParts == IMG_FIGHTER_00_PARTS_FIRE_RIGHT)))
			{// ���̓_�Ńt���O���^�ŁA���J�E���g�����̕��i�ɊY�����鎞�A
				// �J��Ԃ�������܂�Ԃ�
				continue;
			}

			// �@��[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState�֐� - �@��[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateImg_fighter_00(IMG_FIGHTER_00_STATE state)
{
	Img_fighter_00	*pUi					//
					= &g_img_fighter_00;	// �@��[00] �̏��̃|�C���^ 

	// ��Ԃɉ������I������
	EndStateProcessImg_fighter_00();

	// ��Ԃ���
	pUi->state = state;

	// ��Ԃɉ������J�n����
	StartStateProcessImg_fighter_00();

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}