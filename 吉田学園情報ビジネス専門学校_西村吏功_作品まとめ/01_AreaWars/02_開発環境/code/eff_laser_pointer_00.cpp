//========================================
// 
// ���[�U�[�|�C���^�\[00] �̏���
// Author:���� ����
// 
//========================================
//  *** eff_laser_pointer_00.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "setting.h"
#include "sound.h"
#include "polygon.h"
#include "chr_fighter_00.h"
#include "md_title_00.h"
#include "eff_laser_pointer_00.h"

//****************************************
// �}�N����`
//****************************************

// ���[�U�[�|�C���^�\[00] �̏�����̂ɂ����鎞��
#define EFF_LASER_POINTER_00_CLEAR_TIME	(15)

//****************************************
// �񋓌^�̒�`
//****************************************

// ���[�U�[�|�C���^�\[00] �̕��i�ԍ�
typedef enum
{
	EFF_LASER_POINTER_00_PARTS_LASER_POINTER = 0,	// ���[�U�[�|�C���^�\
	EFF_LASER_POINTER_00_PARTS_MAX,
}EFF_LASER_POINTER_00_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// ���[�U�[�|�C���^�\[00] �̃|���S�����̏��\����
typedef struct
{
	char	aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
			[TXT_MAX];		// 
	int		nPtnMaxX;		// �p�^�[���̏��(��)
	int		nPtnMaxY;		// �p�^�[���̏��(�c)
	int		nPtnMin;		// �p�^�[��No.�̉���
	int		nPtnMax;		// �p�^�[��No.�̏��
	int		nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	float	fWidth;			// ��
	float	fHeight;		// ����
	int		nPolygonNum;	// �K�v�|���S����

	int		nPtn;			// �p�^�[��No.
	int		nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Eff_laser_pointer_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterEff_laser_pointer_00(void);

// ���i(���[�U�[�|�C���^�\)�̍X�V����
void UpdateEff_laser_pointer_00Parts_top_arrow(void);

// ��Ԃɉ������J�n����
void StartStateProcessEff_laser_pointer_00(void);

// ��Ԃɉ������I������
void EndStateProcessEff_laser_pointer_00(void);

// ��Ԃɉ������X�V����
void UpdateStateProcessEff_laser_pointer_00(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumEff_laser_pointer_00(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_laser_pointer_00		// �e�N�X�`���ւ̃|�C���^
						[EFF_LASER_POINTER_00_PARTS_MAX]	//
						= {};								//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_laser_pointer_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;								//
Eff_laser_pointer_00	g_eff_laser_pointer_00;				// ���[�U�[�|�C���^�\[00] �̏��

// ���[�U�[�|�C���^�\[00] �̕��i���̏��
Eff_laser_pointer_00Parts g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_MAX] =
{
	{ "data\\TEXTURE\\eff_laser_pointer_000_red.jpg" , 1 , 1 , 0 , 0 , -1 , PIXEL * 2 , PIXEL * 128 , 1 },
};

//========== *** ���[�U�[�|�C���^�\[00] �����擾 ***
Eff_laser_pointer_00 *GetEff_laser_pointer_00(void)
{
	return &g_eff_laser_pointer_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_laser_pointer_00�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterEff_laser_pointer_00(void) 
{
	g_eff_laser_pointer_00.rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	g_eff_laser_pointer_00.state			= EFF_LASER_POINTER_00_STATE_CLEAR;	// ���
	g_eff_laser_pointer_00.nCounterState	= 0;								// ��ԃJ�E���^�[
	g_eff_laser_pointer_00.fAlpha			= 0.0f;								// �����x
}

//========================================
// UpdateEff_laser_pointer_00Parts_top_arrow�֐� - ���i(���[�U�[�|�C���^�\)�̍X�V���� -
//========================================
void UpdateEff_laser_pointer_00Parts_top_arrow(void)
{
	Eff_laser_pointer_00Parts	*pParts										// 
								= &g_aEff_laser_pointer_00Parts				//
								[EFF_LASER_POINTER_00_PARTS_LASER_POINTER];	// ���i�̏��̃|�C���^
	VERTEX_2D					*pVtx;										// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_laser_pointer_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumEff_laser_pointer_00(EFF_LASER_POINTER_00_PARTS_LASER_POINTER));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		D3DXVECTOR3 setPos						// 
					= GetChr_fighter_00()->pos;	// �ݒ�ʒu

		// �ݒ�ʒu�������ɔ����������Z
		setPos.x += sinf(g_eff_laser_pointer_00.rot.z)
			* (g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fHeight * 0.5f);
		setPos.y += cosf(g_eff_laser_pointer_00.rot.z)
			* (g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fHeight * 0.5f);

		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			setPos,
			g_eff_laser_pointer_00.rot,
			true,
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fWidth,
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].fHeight,
			ANGLE_TYPE_FREE);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtn,
				g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMin,
				g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMax,
				&g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nCounterAnim,
				g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMaxX,
			g_aEff_laser_pointer_00Parts[EFF_LASER_POINTER_00_PARTS_LASER_POINTER].nPtnMaxY,
			NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_eff_laser_pointer_00.fAlpha) });
	}
}

//========================================
// StartStateProcessEff_laser_pointer_00�֐� - ��Ԃɉ������J�n���� -
//========================================
void StartStateProcessEff_laser_pointer_00(void) 
{
	Eff_laser_pointer_00	*pEff						// 
							= &g_eff_laser_pointer_00;	// ���[�U�[�|�C���^�\[00] �̏��̃|�C���^ 

	// ��ԃJ�E���^�[��������
	pEff->nCounterState = 0;

	switch (pEff->state)
	{
		//========== *** �o�� ***
	case EFF_LASER_POINTER_00_STATE_POP:
	{
		// �����x��ݒ�
		pEff->fAlpha = 1.0f;
	}
		break;
		//========== *** ������ ***
	case EFF_LASER_POINTER_00_STATE_IN_CLEAR:
	{
		// �ڕW�������@��[00] ������
		pEff->rot = GetChr_fighter_00()->targetRot;
	}
		break;
		//========== *** �����Ă��� ***
	case EFF_LASER_POINTER_00_STATE_CLEAR:
	{
		// �����x��ݒ�
		pEff->fAlpha = 0.0f;
	}
		break;
	}
}

//========================================
// EndStateProcessEff_laser_pointer_00�֐� - ��Ԃɉ������I������ -
//========================================
void EndStateProcessEff_laser_pointer_00(void) 
{
	Eff_laser_pointer_00	*pEff						// 
							= &g_eff_laser_pointer_00;	// ���[�U�[�|�C���^�\[00] �̏��̃|�C���^ 

	switch (pEff->state)
	{
		//========== *** �o�� ***
	case EFF_LASER_POINTER_00_STATE_POP:
	{

	}
	break;
		//========== *** ������ ***
	case EFF_LASER_POINTER_00_STATE_IN_CLEAR:
	{

	}
	break;
		//========== *** �����Ă��� ***
	case EFF_LASER_POINTER_00_STATE_CLEAR:
	{

	}
	break;
	}
}

//========================================
// UpdateStateProcessEff_laser_pointer_00�֐� - ��Ԃɉ������X�V���� -
//========================================
void UpdateStateProcessEff_laser_pointer_00(void) 
{
	Eff_laser_pointer_00	*pEff						// 
							= &g_eff_laser_pointer_00;	// ���[�U�[�|�C���^�\[00] �̏��̃|�C���^ 

	switch (pEff->state)
	{
		//========== *** �o�� ***
	case EFF_LASER_POINTER_00_STATE_POP:
	{

	}
	break;
		//========== *** ������ ***
	case EFF_LASER_POINTER_00_STATE_IN_CLEAR:
	{
		// �����x���X�V
		pEff->fAlpha = (1.0f - ((float)pEff->nCounterState / (float)EFF_LASER_POINTER_00_CLEAR_TIME));

		if (++pEff->nCounterState >= EFF_LASER_POINTER_00_CLEAR_TIME) 
		{// ��ԃJ�E���^�[�����Z�������ʏ�����̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE_CLEAR);
		}
	}
	break;
		//========== *** �����Ă��� ***
	case EFF_LASER_POINTER_00_STATE_CLEAR:
	{

	}
	break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumEff_laser_pointer_00(int nParts)
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aEff_laser_pointer_00Parts[nCntPolygon].nPolygonNum;
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
// InitEff_laser_pointer_00�֐� - ���[�U�[�|�C���^�\[00] �̏��������� -
//========================================
void InitEff_laser_pointer_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumEff_laser_pointer_00(EFF_LASER_POINTER_00_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_laser_pointer_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_laser_pointer_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aEff_laser_pointer_00Parts[nCntParts].aTexturePath, &g_aTextureEff_laser_pointer_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aEff_laser_pointer_00Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aEff_laser_pointer_00Parts[nCntParts].fWidth,
				g_aEff_laser_pointer_00Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx,
				0,
				g_aEff_laser_pointer_00Parts[nCntParts].nPtnMaxX,
				g_aEff_laser_pointer_00Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_laser_pointer_00->Unlock();
}

//========================================
// UninitEff_laser_pointer_00�֐� - ���[�U�[�|�C���^�\[00] �̏I������ -
//========================================
void UninitEff_laser_pointer_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureEff_laser_pointer_00[nCntParts] != NULL)
		{
			g_aTextureEff_laser_pointer_00[nCntParts]->Release();
			g_aTextureEff_laser_pointer_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_laser_pointer_00 != NULL)
	{
		g_pVtxBuffEff_laser_pointer_00->Release();
		g_pVtxBuffEff_laser_pointer_00 = NULL;
	}
}

//========================================
// UpdateEff_laser_pointer_00�֐� - ���[�U�[�|�C���^�\[00] �̍X�V���� -
//========================================
void UpdateEff_laser_pointer_00(void)
{
	Eff_laser_pointer_00	*pUi						//
							= &g_eff_laser_pointer_00;	// ���[�U�[�|�C���^�\[00] �̏��̃|�C���^

	// ��Ԃɉ������X�V����
	UpdateStateProcessEff_laser_pointer_00();

	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** ���[�U�[�|�C���^�\ ***
		case EFF_LASER_POINTER_00_PARTS_LASER_POINTER:
		{
			// ���i(���[�U�[�|�C���^�\)�̍X�V����
			UpdateEff_laser_pointer_00Parts_top_arrow();
		}
		break;
		}
	}
}

//========================================
// DrawEff_laser_pointer_00�֐� - ���[�U�[�|�C���^�\[00] �̕`�揈�� -
//========================================
void DrawEff_laser_pointer_00(void)
{
	if (!GetSetting()->bLaser) 
	{// �ݒ�̌����t���O���U�̎��A
		// �������I������
		return;
	}

	Eff_laser_pointer_00	*pUi						//
							= &g_eff_laser_pointer_00;	// ���[�U�[�|�C���^�\[00] �̏��̃|�C���^

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_laser_pointer_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < EFF_LASER_POINTER_00_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureEff_laser_pointer_00[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aEff_laser_pointer_00Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// ���[�U�[�|�C���^�\[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}

	// �A���t�@�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// SetStateEff_laser_pointer_00�֐� - ���[�U�[�|�C���^�\[00] �̏�Ԑݒ菈�� -
//========================================
void SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE state)
{
	Eff_laser_pointer_00	*pUi						//
							= &g_eff_laser_pointer_00;	// ���[�U�[�|�C���^�\[00] �̏��̃|�C���^ 

	// ��Ԃɉ������I������
	EndStateProcessEff_laser_pointer_00();

	// ��Ԃ���
	pUi->state = state;

	// ��Ԃɉ������J�n����
	StartStateProcessEff_laser_pointer_00();

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}