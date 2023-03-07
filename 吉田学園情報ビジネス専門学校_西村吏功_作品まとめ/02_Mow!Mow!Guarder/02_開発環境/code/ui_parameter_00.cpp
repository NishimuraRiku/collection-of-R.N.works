//========================================
// 
// UI:�p�����[�^�[[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_parameter_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "ui_parameter_00.h"	// UI :�p�����[�^�[	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�p�����[�^�[[00] �̈ʒu
#define UI_PARAMETER_00_POS	D3DXVECTOR3(INSIDE_SCREEN_LEFTMOST+(PIXEL*32),BUFFER_HEIGHT-(PIXEL*30),0.0f)

// UI:�p�����[�^�[[00] ��LV�e�L�X�g�̑��Έʒu
// UI:�p�����[�^�[[00] ��LV�e�L�X�g�̊ԊuY
// UI:�p�����[�^�[[00] ��LV�e�L�X�g�̐F
#define UI_PARAMETER_00_LV_TEXT_RELATIVE_POS	D3DXVECTOR3(PIXEL*16,PIXEL*-12,0.0f)
#define UI_PARAMETER_00_LV_TEXT_SPACE_Y			(PIXEL*8)
#define UI_PARAMETER_00_LV_TEXT_COLOR			Color{255,255,255,255}

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:�p�����[�^�[[00] �̃|���S��
typedef enum 
{
	UI_PARAMETER_00_POLYGON_FRAME,	// �t���[��
	UI_PARAMETER_00_POLYGON_MAX,
}UI_PARAMETER_00_POLYGON;

// UI:�p�����[�^�[[00] ��LV�e�L�X�g
typedef enum
{
	UI_PARAMETER_00_LV_TEXT_SWORD_ATK,	// ��ATK
	UI_PARAMETER_00_LV_TEXT_BULLET_ATK,	// �eATK
	UI_PARAMETER_00_LV_TEXT_PLAYER_HP,	// �v���C���[HP
	UI_PARAMETER_00_LV_TEXT_UNIT_HP,	// ���j�b�gHP
	UI_PARAMETER_00_LV_TEXT_MAX,
}UI_PARAMETER_00_LV_TEXT;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_parameter_00	// �e�N�X�`���ւ̃|�C���^
						[UI_PARAMETER_00_POLYGON_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_parameter_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;

// UI:�p�����[�^�[[00] �̃|���S�����̏��
const Polygon2D g_aUi_parameter_00Polygon[UI_PARAMETER_00_POLYGON_MAX] = 
{
	{ "data\\TEXTURE\\UserInterface\\ui_parameter-frame_000.png",PIXEL * 64,PIXEL * 64,1 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_parameter_00�֐� - UI:�p�����[�^�[[00] �̏��������� -
//========================================
void InitUi_parameter_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	LoadPolygon2DTexture(pDevice, g_aTextureUi_parameter_00, g_aUi_parameter_00Polygon, UI_PARAMETER_00_POLYGON_MAX);
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GetPolygon2DNum(g_aUi_parameter_00Polygon, UI_PARAMETER_00_POLYGON_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_parameter_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_parameter_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// �|���S�����̏��
						= &g_aUi_parameter_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPos2D(pVtx,
				UI_PARAMETER_00_POS,
				INITD3DXVECTOR3,
				false,
				pPoly->fWidth,
				pPoly->fHeight,
				ANGLE_TYPE_FIXED);

			// rhw�̐ݒ�
			SetRHW2D(pVtx);

			switch (nCntPoly)
			{
			case/*/ �t���[�� /*/ UI_PARAMETER_00_POLYGON_FRAME:
				// �e�N�X�`�����W�̐ݒ�
				SetTexturePos2D(pVtx, 0, 1, 1, NULL);

				// ���_�J���[�̐ݒ�
				SetVertexColor2D(pVtx, INITCOLOR);

				break;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_parameter_00->Unlock();
}

//========================================
// UninitUi_parameter_00�֐� - UI:�p�����[�^�[[00] �̏I������ -
//========================================
void UninitUi_parameter_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		if (g_aTextureUi_parameter_00[nCntPoly] != NULL)
		{
			g_aTextureUi_parameter_00[nCntPoly]->Release();
			g_aTextureUi_parameter_00[nCntPoly] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_parameter_00 != NULL) 
	{
		g_pVtxBuffUi_parameter_00->Release();
		g_pVtxBuffUi_parameter_00 = NULL;
	}
}

//========================================
// UpdateUi_parameter_00�֐� - UI:�p�����[�^�[[00] �̍X�V���� -
//========================================
void UpdateUi_parameter_00(void)
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_parameter_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly		// �|���S�����̏��
						= &g_aUi_parameter_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			switch (nCntPoly)
			{
			case/*/ �t���[�� /*/ UI_PARAMETER_00_POLYGON_FRAME:

				break;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_parameter_00->Unlock();

	for (int nCntTxt = 0; nCntTxt < UI_PARAMETER_00_LV_TEXT_MAX; nCntTxt++) 
	{
		int nLv = 0;	// LV

		switch (nCntTxt)
		{
		case /*/ ��ATK /*/UI_PARAMETER_00_LV_TEXT_SWORD_ATK:
			nLv = GetChr_player_00()->nSwordAtkUpCount; break;
		case /*/ �eATK /*/UI_PARAMETER_00_LV_TEXT_BULLET_ATK:
			nLv = GetChr_player_00()->nBulletAtkUpCount; break;
		case /*/ �v���C���[HP /*/UI_PARAMETER_00_LV_TEXT_PLAYER_HP:
			nLv = GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS].nCntPriceUp; break;
		case /*/ ���j�b�gHP /*/UI_PARAMETER_00_LV_TEXT_UNIT_HP:
			nLv = GetObj_pedestal_00ItemControl()[OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS].nCntPriceUp; break;
		}

		char aString[TXT_MAX];	// �������ݒ�
		sprintf(aString, "LV%2d", nLv + 1);

		D3DXVECTOR3 setPos	// �ݒ�ʒu��ݒ�
			= UI_PARAMETER_00_POS + UI_PARAMETER_00_LV_TEXT_RELATIVE_POS;
		setPos.y += UI_PARAMETER_00_LV_TEXT_SPACE_Y * nCntTxt;
		SetText2D(
			aString,
			FONT_000,
			DISPLAY_RIGHT,
			setPos,
			UI_PARAMETER_00_LV_TEXT_COLOR, 1.0f, 1.0f);
	}
}

//========================================
// DrawUi_parameter_00�֐� - UI:�p�����[�^�[[00] �̕`�揈�� -
//========================================
void DrawUi_parameter_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_parameter_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoly = 0; nCntPoly < UI_PARAMETER_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// �|���S�����̏��
			= &g_aUi_parameter_00Polygon[nCntPoly];

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_parameter_00[nCntPoly]);

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++)
		{
			// UI:�p�����[�^�[[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (GetPolygon2DNum(g_aUi_parameter_00Polygon, nCntPoly) + nCntPolyIdx), 2);
		}
	}
}