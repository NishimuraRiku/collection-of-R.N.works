//========================================
// 
// UI:HP�o�[[01] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_hp-bar_01.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "obj_core_00.h"	// OBJ:�R�A		[00]
#include "ui_hp-bar_01.h"	// UI :HP�o�[	[01]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// UI:HP�o�[[01] �̖@���x�N�g��
// UI:HP�o�[[01] �̑��Έʒu
#define UI_HPBAR_01_NOR				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define UI_HPBAR_01_RELATIVE_POS	(D3DXVECTOR3(0.0f, 20.0f, 0.0f))

// UI:HP�o�[[01] �̃o�[�����̑��Έʒu
// UI:HP�o�[[01] �̃o�[�����̐F
#define UI_HPBAR_01_BAR_RELATIVE_POS	(D3DXVECTOR3(PIXEL3D * -27, PIXEL3D * 3, 0.0f))
#define UI_HPBAR_01_BAR_COLOR			{243,191,63,255}

//****************************************
// �񋓌^�̒�`
//****************************************
// HP�o�[[01] �̕��i�ԍ�
typedef enum
{
	UI_HPBAR_01_PARTS_FRAME,	// �t���[��
	UI_HPBAR_01_PARTS_BAR,		// �o�[
	UI_HPBAR_01_PARTS_MAX,
}UI_HPBAR_01_PARTS;

// HP�o�[[01] �̃e�N�X�`���ԍ�
typedef enum
{
	UI_HPBAR_01_TEXTURE_FRAME,	// �t���[��
	UI_HPBAR_01_TEXTURE_BAR,	// �o�[
	UI_HPBAR_01_TEXTURE_MAX,
}UI_HPBAR_01_TEXTURE;

//****************************************
// �\���̂̒�`
//****************************************
// HP�o�[[01] �̕��i���̏��\����
typedef struct
{
	UI_HPBAR_01_TEXTURE texture;	// �e�N�X�`��
	int					nPattern;	// �p�^�[��No.
	float				fWidth;		// ��
	float				fHeight;	// ����
}Ui_hpBar_01Parts;

// HP�o�[[01] �̃e�N�X�`�����̏��\����
typedef struct
{
	char	aTexturePath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	int		nPtnWidth;				// �p�^�[����
	int		nPtnHeight;				// �p�^�[������
}Ui_hpBar_01Texture;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// HP�o�[[01] �̕��i���̒��_���W�ݒ菈��
void UpdatePartsUi_hpBar_01(VERTEX_3D *pVtx, UI_HPBAR_01_PARTS parts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_hpBar_01	// UI:HP�o�[[01] �̃e�N�X�`���ւ̃|�C���^
						[UI_HPBAR_01_TEXTURE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_hpBar_01	// UI:HP�o�[[01] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldUi_hpBar_01;	// UI:HP�o�[[01] �̃��[���h�}�g���b�N�X

// HP�o�[[01] �̃e�N�X�`�����̏��
const Ui_hpBar_01Texture g_aUi_hpBar_01Texture[UI_HPBAR_01_TEXTURE_MAX] =
{
	{ "data\\TEXTURE\\UserInterface\\ui_hp-frame_000.png" , 1, 1 },	// �t���[��
	{ ""                                                  , 2, 1 },	// �o�[
};

// HP�o�[[01] �̕��i���̏��
const Ui_hpBar_01Parts g_aUi_hpBar_01Parts[UI_HPBAR_01_PARTS_MAX] =
{
	{ UI_HPBAR_01_TEXTURE_FRAME, 0, PIXEL3D * 64 , PIXEL3D * 16 },	// �t���[��
	{ UI_HPBAR_01_TEXTURE_BAR  , 0, PIXEL3D * 54 , PIXEL3D * 4  },	// �o�[
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetVertexPositionUi_hpBar_01�֐� - HP�o�[[01] �̕��i���̒��_���W�ݒ菈�� -
//========================================
void UpdatePartsUi_hpBar_01(VERTEX_3D *pVtx, UI_HPBAR_01_PARTS parts)
{
	float					fBarLengthDiameter	// �o�[�̒����̔{��
							= ((float)GetObj_core_00()->nHP / (float)GetObj_core_00()->nHPMax);
	const Ui_hpBar_01Parts	*pParts				// ���i���̏��̃|�C���^
							= &g_aUi_hpBar_01Parts[parts];

	switch (parts)
	{
		//========== *** �t���[�� ***
	case UI_HPBAR_01_PARTS_FRAME:
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(pParts->fWidth * 0.5f),  (pParts->fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (pParts->fWidth * 0.5f),  (pParts->fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(pParts->fWidth * 0.5f), -(pParts->fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (pParts->fWidth * 0.5f), -(pParts->fHeight * 0.5f), 0.0f);
	}
	break;
		//========== *** �o�[ ***
	case UI_HPBAR_01_PARTS_BAR:
	{
		D3DXVECTOR3 posR = UI_HPBAR_01_BAR_RELATIVE_POS;	// ���Έʒu

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(posR.x											, posR.y + (pParts->fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posR.x + (pParts->fWidth * fBarLengthDiameter)	, posR.y + (pParts->fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posR.x											, posR.y - (pParts->fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posR.x + (pParts->fWidth * fBarLengthDiameter)	, posR.y - (pParts->fHeight * 0.5f), 0.0f);
	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_hpBar_01�֐� - UI:HP�o�[[01] �̏��������� -
//========================================
void InitUi_hpBar_01(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < UI_HPBAR_01_TEXTURE_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aUi_hpBar_01Texture[nCntTexture].aTexturePath, &g_aTextureUi_hpBar_01[nCntTexture]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * UI_HPBAR_01_PARTS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_hpBar_01,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_hpBar_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_01_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, UI_HPBAR_01_NOR);

		// ���_�J���[�̐ݒ�
		if (nCntParts == UI_HPBAR_01_PARTS_BAR) {
			SetVertexColor3D(pVtx, UI_HPBAR_01_BAR_COLOR);
		}
		else {
			SetVertexColor3D(pVtx, { 255,255,255,255 });
		}

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx,
			g_aUi_hpBar_01Parts[nCntParts].nPattern,
			g_aUi_hpBar_01Texture[g_aUi_hpBar_01Parts[nCntParts].texture].nPtnWidth,
			g_aUi_hpBar_01Texture[g_aUi_hpBar_01Parts[nCntParts].texture].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_hpBar_01->Unlock();
}

//========================================
// UninitUi_hpBar_01�֐� - UI:HP�o�[[01] �̏I������ -
//========================================
void UninitUi_hpBar_01(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < UI_HPBAR_01_TEXTURE_MAX; nCntType++)
	{
		if (g_aTextureUi_hpBar_01[nCntType] != NULL)
		{
			g_aTextureUi_hpBar_01[nCntType]->Release();
			g_aTextureUi_hpBar_01[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_hpBar_01 != NULL)
	{
		g_pVtxBuffUi_hpBar_01->Release();
		g_pVtxBuffUi_hpBar_01 = NULL;
	}
}

//========================================
// UpdateUi_hpBar_01�֐� - UI:HP�o�[[01] �̍X�V���� -
//========================================
void UpdateUi_hpBar_01(void)
{
	VERTEX_3D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_hpBar_01->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_01_PARTS_MAX; nCntParts++, pVtx += 4)
	{
		// ���i���̍X�V����
		UpdatePartsUi_hpBar_01(pVtx, (UI_HPBAR_01_PARTS)nCntParts);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_hpBar_01->Unlock();
}

//========================================
// DrawUi_hpBar_01�֐� - UI:HP�o�[[01] �̕`�揈�� -
//========================================
void DrawUi_hpBar_01(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_hpBar_01, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntParts = 0; nCntParts < UI_HPBAR_01_PARTS_MAX; nCntParts++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldUi_hpBar_01);

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_mtxWorldUi_hpBar_01, NULL, &mtxView);
		g_mtxWorldUi_hpBar_01._41 = 0.0f;
		g_mtxWorldUi_hpBar_01._42 = 0.0f;
		g_mtxWorldUi_hpBar_01._43 = 0.0f;
		{
			// �ʒu�𔽉f
			D3DXVECTOR3 setPos = GetObj_core_00()->pos + UI_HPBAR_01_RELATIVE_POS;
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldUi_hpBar_01, &g_mtxWorldUi_hpBar_01, &mtxTrans);
		}
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldUi_hpBar_01);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_hpBar_01[g_aUi_hpBar_01Parts[nCntParts].texture]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParts, 2);
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}