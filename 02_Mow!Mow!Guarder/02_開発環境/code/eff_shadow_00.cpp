//========================================
// 
// EFF:�e[00] �̊֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_shadow_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon3D.h"
#include "sound.h"
#include "eff_shadow_00.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// EFF:�e[00] �̍ő吔
#define EFF_SHADOW_00_MAX	(256)

// EFF:�e[00] �̃e�N�X�`���̑��΃p�X
#define EFF_SHADOW_00_TEXTURE_PATH	"data\\TEXTURE\\Effect\\eff_circle_000.jpg"

// EFF:�e[00] �̖@���x�N�g��
// EFF:�e[00] �̑��Έʒu
#define EFF_SHADOW_00_NOR			(D3DXVECTOR3(0.0f,1.0f,0.0f))
#define EFF_SHADOW_00_RELATIVE_POS	(D3DXVECTOR3(0.0f,0.01f,0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************
// EFF:�e[00] �̏��\����
typedef struct 
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
}Eff_shadow_00;

// EFF:�e[00] �̊Ǘ����\����
typedef struct
{
	int nUseNum;	// �g�p����Ă��鐔
}Eff_shadow_00Control;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:�e[00] �̃p�����[�^�[�̏���������
// EFF:�e[00] �̃p�����[�^�[�̊Ǘ����̏���������
void InitParameterEff_shadow_00(Eff_shadow_00 *pEff);
void InitParameterEff_shadow_00Control(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureEff_shadow_00		// EFF:�e[00] �̃e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_shadow_00		// EFF:�e[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldEff_shadow_00;	// EFF:�e[00] �̃��[���h�}�g���b�N�X
Eff_shadow_00			g_aEff_shadow_00
						[EFF_SHADOW_00_MAX];		// EFF:�e[00] �̏��
Eff_shadow_00Control	g_eff_shadow_00Control;		// EFF:�e[00] �̊Ǘ����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_shadow_00�֐� - EFF:�e[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterEff_shadow_00(Eff_shadow_00 *pEff) 
{
	pEff->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pEff->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
}

//========================================
// InitParameterEff_shadow_00Control�֐� - EFF:�e[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterEff_shadow_00Control(void)
{
	Eff_shadow_00Control	*pEffCtl	// EFF:�e[00] �̊Ǘ����
							= &g_eff_shadow_00Control;

	pEffCtl->nUseNum = 0;	// �g�p����Ă��鐔
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_shadow_00�֐� - EFF:�e[00] �̏��������� -
//========================================
void InitEff_shadow_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EFF_SHADOW_00_TEXTURE_PATH, &g_pTextureEff_shadow_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFF_SHADOW_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_shadow_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_shadow_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_shadow_00	*pEff	// EFF:�e[00] �̏��\����
					= g_aEff_shadow_00;

	for (int nCntEff = 0; nCntEff < EFF_SHADOW_00_MAX; nCntEff++, pVtx += 4)
	{
		// �p�����[�^�[�̏���������
		InitParameterEff_shadow_00(pEff);

		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, EFF_SHADOW_00_NOR);
		
		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_shadow_00->Unlock();
}

//========================================
// UninitEff_shadow_00�֐� - EFF:�e[00] �̏I������ -
//========================================
void UninitEff_shadow_00(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureEff_shadow_00 != NULL)
	{
		g_pTextureEff_shadow_00->Release();
		g_pTextureEff_shadow_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_shadow_00 != NULL)
	{
		g_pVtxBuffEff_shadow_00->Release();
		g_pVtxBuffEff_shadow_00 = NULL;
	}
}

//========================================
// UpdateEff_shadow_00�֐� - EFF:�e[00] �̍X�V���� -
//========================================
void UpdateEff_shadow_00(void)
{

}

//========================================
// DrawEff_shadow_00�֐� - EFF:�e[00] �̕`�揈�� -
//========================================
void DrawEff_shadow_00(void)
{
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	Eff_shadow_00			*pEff		// EFF:�e[00] �̏��\����
							= g_aEff_shadow_00;
	Eff_shadow_00Control	*pEffCtl	// EFF:�e[00] �̊Ǘ����
							= &g_eff_shadow_00Control;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_shadow_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEff_shadow_00);
	
	for (int nCntEff = 0; nCntEff < pEffCtl->nUseNum; nCntEff++, pEff++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldEff_shadow_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pEff->rot.y, pEff->rot.x, pEff->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shadow_00, &g_mtxWorldEff_shadow_00, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pEff->pos.x, pEff->pos.y, pEff->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shadow_00, &g_mtxWorldEff_shadow_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEff_shadow_00);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEff, 2);
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_eff_shadow_00Control.nUseNum = 0;	// �g�p����Ă��鐔��������
}

//========================================
// SetEff_shadow_00�֐� - EFF:�e[00] �̐ݒ菈�� -
//========================================
void SetEff_shadow_00(D3DXVECTOR3 pos, float fRadius) 
{
	Eff_shadow_00			*pEff		// EFF:�e[00] �̏��\����
							= g_aEff_shadow_00;
	Eff_shadow_00Control	*pEffCtl	// EFF:�e[00] �̊Ǘ����
							= &g_eff_shadow_00Control;

	pEff[pEffCtl->nUseNum].pos 	// �ʒu����
		= pos + EFF_SHADOW_00_RELATIVE_POS;

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_shadow_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexPos3D(&pVtx[pEffCtl->nUseNum * 4], fRadius * 2, fRadius * 2);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_shadow_00->Unlock();

	pEffCtl->nUseNum++;			// �g�p����Ă��鐔�����Z
}