//========================================
// 
// UI:�}�b�v[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_map_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "text.h"
#include "chr_player_00.h"			// CHR:�v���C���[	[00]
#include "chr_enemy_00.h"			// CHR:�G			[00]
#include "obj_core_00.h"			// OBJ:�R�A			[00]
#include "obj_stage_00.h"			// OBJ:�X�e�[�W		[00]
#include "ui_attention-mark_00.h"	// UI :���Ӄ}�[�N	[00]
#include "ui_map_00.h"				// UI :�}�b�v		[00]

//****************************************
// �}�N����`
//****************************************
// UI:�}�b�v[00] �̈ʒu
// UI:�}�b�v[00] �̐؂���X�P�[��
// UI:�}�b�v[00] �̃e�N�X�`���̈ʒu�ϊ��l
// UI:�}�b�v[00] �̈ʒu�ϊ��l
// UI:�}�b�v[00] �̃}�b�v�̐F
#define UI_MAP_00_POS				D3DXVECTOR3(INSIDE_SCREEN_RIGHTMOST-(PIXEL*32),SCREEN_HEIGHT-(PIXEL*30),0.0f)
#define UI_MAP_00_CUT_SCALE			Scale{0.5f,0.5f,0.0f}
#define UI_MAP_00_TEXTURE_POS_CONV	(1.0f/8.0f/64.0f)
#define UI_MAP_00_POS_CONV			((UI_MAP_00_TEXTURE_POS_CONV*PIXEL*48)/0.5f)
#define UI_MAP_00_MAP_COLOR			Color{100,100,100,255}

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:�}�b�v[00] �̃|���S��
typedef enum 
{
	UI_MAP_00_POLYGON_MAP,			// �}�b�v
	UI_MAP_00_POLYGON_ICON_CORE,	// �A�C�R��(�R�A)
	UI_MAP_00_POLYGON_ICON_ENEMY,	// �A�C�R��(�G)
	UI_MAP_00_POLYGON_ICON_PLAYER,	// �A�C�R��(�v���C���[)
	UI_MAP_00_POLYGON_ICON_WARNING,	// �A�C�R��(�x��)
	UI_MAP_00_POLYGON_FRAME,		// �t���[��
	UI_MAP_00_POLYGON_MAX,
}UI_MAP_00_POLYGON;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_map_00	// �e�N�X�`���ւ̃|�C���^
						[UI_MAP_00_POLYGON_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_map_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;

// UI:�}�b�v[00] �̃|���S�����̏��
const Polygon2D g_aUi_map_00Polygon[UI_MAP_00_POLYGON_MAX] = 
{
	{ "data\\TEXTURE\\UserInterface\\ui_stage-map_000.png"    ,PIXEL * 48,PIXEL * 48,1                     },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\core.png"   ,PIXEL * 4 ,PIXEL * 4 ,1                     },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\enemy.png"  ,PIXEL * 2 ,PIXEL * 2 ,CHR_ENEMY_00_MAX      },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\player.png" ,PIXEL * 4 ,PIXEL * 4 ,1                     },
	{ "data\\TEXTURE\\UserInterface\\ui_icon_000\\warning.png",PIXEL * 8 ,PIXEL * 8 ,OBJ_STAGE_00_GATE_MAX },
	{ "data\\TEXTURE\\UserInterface\\ui_map-frame_000.png"    ,PIXEL * 64,PIXEL * 64,1                     },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_map_00�֐� - UI:�}�b�v[00] �̏��������� -
//========================================
void InitUi_map_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	LoadPolygon2DTexture(pDevice, g_aTextureUi_map_00, g_aUi_map_00Polygon, UI_MAP_00_POLYGON_MAX);
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GetPolygon2DNum(g_aUi_map_00Polygon, UI_MAP_00_POLYGON_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_map_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_map_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// �|���S�����̏��
						= &g_aUi_map_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPos2D(pVtx,
				UI_MAP_00_POS,
				INITD3DXVECTOR3,
				false,
				pPoly->fWidth,
				pPoly->fHeight,
				ANGLE_TYPE_FIXED);

			// rhw�̐ݒ�
			SetRHW2D(pVtx);

			switch (nCntPoly)
			{
			case /*/ �}�b�v /*/UI_MAP_00_POLYGON_MAP:
				// �e�N�X�`�����W�̐ݒ�
				SetTexturePos2DCut(pVtx, UI_MAP_00_CUT_SCALE, ConvPosToBirdsEyeView(GetChr_player_00()->pos * UI_MAP_00_TEXTURE_POS_CONV));
				
				// ���_�J���[�̐ݒ�
				SetVertexColor2D(pVtx, UI_MAP_00_MAP_COLOR);

				break;
			case/*/ �A�C�R��(�R�A)			/*/ UI_MAP_00_POLYGON_ICON_CORE:
			case/*/ �A�C�R��(�G)			/*/ UI_MAP_00_POLYGON_ICON_ENEMY:
			case/*/ �A�C�R��(�v���C���[)	/*/ UI_MAP_00_POLYGON_ICON_PLAYER:
			case/*/ �A�C�R��(�x��)			/*/ UI_MAP_00_POLYGON_ICON_WARNING:
			case/*/ �t���[��				/*/ UI_MAP_00_POLYGON_FRAME:
				// �e�N�X�`�����W�̐ݒ�
				SetTexturePos2D(pVtx, 0, 1, 1, NULL);

				// ���_�J���[�̐ݒ�
				SetVertexColor2D(pVtx, INITCOLOR);

				break;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_map_00->Unlock();
}

//========================================
// UninitUi_map_00�֐� - UI:�}�b�v[00] �̏I������ -
//========================================
void UninitUi_map_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		if (g_aTextureUi_map_00[nCntPoly] != NULL)
		{
			g_aTextureUi_map_00[nCntPoly]->Release();
			g_aTextureUi_map_00[nCntPoly] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_map_00 != NULL) 
	{
		g_pVtxBuffUi_map_00->Release();
		g_pVtxBuffUi_map_00 = NULL;
	}
}

//========================================
// UpdateUi_map_00�֐� - UI:�}�b�v[00] �̍X�V���� -
//========================================
void UpdateUi_map_00(void)
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_map_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly		// �|���S�����̏��
						= &g_aUi_map_00Polygon[nCntPoly];

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++, pVtx += 4)
		{
			switch (nCntPoly)
			{
			case /*/ �}�b�v /*/UI_MAP_00_POLYGON_MAP:
				// �e�N�X�`�����W�̐ݒ�
				SetTexturePos2DCut(pVtx, UI_MAP_00_CUT_SCALE, ConvPosToBirdsEyeView(GetChr_player_00()->pos * UI_MAP_00_TEXTURE_POS_CONV));

				break;
			case/*/ �A�C�R��(�G) /*/ UI_MAP_00_POLYGON_ICON_ENEMY:
				if ((!GetChr_enemy_00()[nCntPolyIdx].bUse) || (GetChr_enemy_00()[nCntPolyIdx].bRecognitionOff))
				{// �J�E���g��CHR:�G[00] ���g�p����Ă��Ȃ���� or �F��OFF�t���O���^�̎��A
					break;	// Switch���𔲂���
				}

				// ���_���W��ݒ�
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS +
					ConvPosToBirdsEyeView((GetChr_enemy_00()[nCntPolyIdx].pos - GetChr_player_00()->pos) * UI_MAP_00_POS_CONV),
					INITD3DXVECTOR3,
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FIXED);

				break;
			case/*/ �A�C�R��(�R�A) /*/ UI_MAP_00_POLYGON_ICON_CORE:
				// ���_���W��ݒ�
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS +
					ConvPosToBirdsEyeView((GetObj_core_00()->pos - GetChr_player_00()->pos) * UI_MAP_00_POS_CONV),
					INITD3DXVECTOR3,
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FIXED);

				break;
			case/*/ �A�C�R��(�v���C���[) /*/ UI_MAP_00_POLYGON_ICON_PLAYER:
				// ���_���W��ݒ�
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS,
					ConvRotToBirdsEyeView(GetChr_player_00()->rot),
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FREE);

				break;
			case/*/ �A�C�R��(�x��)	/*/ UI_MAP_00_POLYGON_ICON_WARNING:
				if (!GetUi_attentionMark_00()[nCntPolyIdx].bUse)
				{// �J�E���g��UI:���Ӄ}�[�N[00] ���g�p����Ă��Ȃ���Ԃ̎��A
					break;	// Switch���𔲂���
				}

				// ���_���W��ݒ�
				SetVertexPos2D(pVtx,
					UI_MAP_00_POS +
					ConvPosToBirdsEyeView((GetUi_attentionMark_00()[nCntPolyIdx].pos - GetChr_player_00()->pos) * UI_MAP_00_POS_CONV),
					INITD3DXVECTOR3,
					false,
					pPoly->fWidth,
					pPoly->fHeight,
					ANGLE_TYPE_FIXED);

				break;
			case/*/ �t���[�� /*/ UI_MAP_00_POLYGON_FRAME:

				break;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_map_00->Unlock();
}

//========================================
// DrawUi_map_00�֐� - UI:�}�b�v[00] �̕`�揈�� -
//========================================
void DrawUi_map_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_map_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoly = 0; nCntPoly < UI_MAP_00_POLYGON_MAX; nCntPoly++)
	{
		const Polygon2D *pPoly	// �|���S�����̏��
			= &g_aUi_map_00Polygon[nCntPoly];

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_map_00[nCntPoly]);

		for (int nCntPolyIdx = 0; nCntPolyIdx < pPoly->nMax; nCntPolyIdx++)
		{
			switch (nCntPoly)
			{
			case/*/ �A�C�R��(�G) /*/ UI_MAP_00_POLYGON_ICON_ENEMY:
				if (!GetChr_enemy_00()[nCntPolyIdx].bUse)
				{// �J�E���g��CHR:�G[00] ���g�p����Ă��Ȃ���Ԃ̎��A
					continue;	// �J��Ԃ�������܂�Ԃ�
				}
				break;
			case/*/ �A�C�R��(�x��)	/*/ UI_MAP_00_POLYGON_ICON_WARNING:
				if ((!GetUi_attentionMark_00()[nCntPolyIdx].bUse) || (!GetUi_attentionMark_00()[nCntPolyIdx].bShow) || (GetObj_stage_00()->state != OBJ_STAGE_00_STATE_INTERVAL))
				{// �J�E���g��UI:���Ӄ}�[�N[00] ���g�p����Ă��Ȃ� or ���t���O���U or  OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̂łȂ����A
					continue;	// �J��Ԃ�������܂�Ԃ�
				}
				break;
			}

			// UI:�}�b�v[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (GetPolygon2DNum(g_aUi_map_00Polygon, nCntPoly) + nCntPolyIdx), 2);
		}
	}
}