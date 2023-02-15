//========================================
// 
// UI:���Ӄ}�[�N[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_attention-mark_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "color.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "ui_attention-mark_00.h"	// UI :���Ӄ}�[�N	[00]
#include "obj_stage_00.h"			// OBJ:�X�e�[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// UI:���Ӄ}�[�N[00] �̍ő吔
#define UI_ATTENTIONMARK_00_MAX	(OBJ_STAGE_00_GATE_MAX)

// UI:���Ӄ}�[�N[00] �̕�
// UI:���Ӄ}�[�N[00] �̍���
#define UI_ATTENTIONMARK_00_WIDTH			(64)
#define UI_ATTENTIONMARK_00_HEIGHT			(64)

// UI:���Ӄ}�[�N[00] �̖��̕�
// UI:���Ӄ}�[�N[00] �̖��̉��s��
// UI:���Ӄ}�[�N[00] �̖��̉�������
// UI:���Ӄ}�[�N[00] �̖���Y�␳
// UI:���Ӄ}�[�N[00] �̖��̈ړ��ɂ����鎞��
#define UI_ATTENTIONMARK_00_ARROW_WIDTH				(64)
#define UI_ATTENTIONMARK_00_ARROW_DEPTH				(128)
#define UI_ATTENTIONMARK_00_ARROW_DIVISION_DEPTH	(8)
#define UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION		(0.1f)
#define UI_ATTENTIONMARK_00_ARROW_MOVE_TIME			(60)

// UI:���Ӄ}�[�N[00] �̖@���x�N�g��
#define UI_ATTENTIONMARK_00_NOR	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

// UI:���Ӄ}�[�N[00] �̐F
// UI:���Ӄ}�[�N[00] �̒��ӎ��̐F
#define UI_ATTENTIONMARK_00_COLOR			Color{255,43,0,255}
#define UI_ATTENTIONMARK_00_WARNING_COLOR	Color{131,0,243,255}

// UI:���Ӄ}�[�N[00] �̓_�łɂ����鎞��
#define UI_ATTENTIONMARK_00_BRINK_TIME	(40)

//****************************************
// �񋓌^�̒�`
//****************************************
// UI:���Ӄ}�[�N[00] �̃|���S���ԍ�
typedef enum
{
	UI_ATTENTIONMARK_00_POLYGON_BASE,	// ��{
	UI_ATTENTIONMARK_00_POLYGON_ARROW,	// ���
	UI_ATTENTIONMARK_00_POLYGON_MAX,
}UI_ATTENTIONMARK_00_POLYGON;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:���Ӄ}�[�N[00] �̃p�����[�^�[�̏���������
void InitParameterUi_attentionMark_00(Ui_attentionMark_00 *pUi);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9			g_pTextureUi_attentionMark_00	// UI:���Ӄ}�[�N[00] �̃e�N�X�`���ւ̃|�C���^
							[UI_ATTENTIONMARK_00_POLYGON_MAX]
							= {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffUi_attentionMark_00	// UI:���Ӄ}�[�N[00] �̒��_�o�b�t�@�ւ̃|�C���^
							= NULL;
D3DXMATRIX					g_mtxWorldUi_attentionMark_00;	// UI:���Ӄ}�[�N[00] �̃��[���h�}�g���b�N�X
Ui_attentionMark_00			g_aUi_attentionMark_00			// UI:���Ӄ}�[�N[00] �̏��
							[UI_ATTENTIONMARK_00_MAX];
Ui_attentionMark_00Control	g_ui_attentionMark_00Control;	// UI:���Ӄ}�[�N[00] �̊Ǘ����

// UI:���Ӄ}�[�N[00] �̃e�N�X�`���̑��΃p�X
const char c_aUi_attentionMark_00Texture[UI_ATTENTIONMARK_00_POLYGON_MAX][TXT_MAX] = 
{
	"data\\TEXTURE\\UserInterface\\ui_attention-mark_000.png",
	"data\\TEXTURE\\UserInterface\\ui_attention-arrow_000.png",
};

//========== *** UI:���Ӄ}�[�N[00] �̏����擾 ***
Ui_attentionMark_00 *GetUi_attentionMark_00(void) 
{
	return g_aUi_attentionMark_00;
}

//========== *** UI:���Ӄ}�[�N[00] �̊Ǘ������擾 ***
Ui_attentionMark_00Control *GetUi_attentionMark_00Control(void)
{
	return &g_ui_attentionMark_00Control;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_attentionMark_00�֐� - UI:���Ӄ}�[�N[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_attentionMark_00(Ui_attentionMark_00 *pUi)
{
	pUi->pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pUi->rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pUi->bUse	= false;							// �g�p����Ă��邩�t���O
	pUi->bShow	= false;							// ���t���O
}

//========================================
// InitParameterUi_attentionMark_00Control�֐� - UI:���Ӄ}�[�N[00] �̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_attentionMark_00Control(void)
{
	Ui_attentionMark_00Control	*pUiCtl	// UI:���Ӄ}�[�N[00] �̊Ǘ����̃|�C���^
								= &g_ui_attentionMark_00Control;

	pUiCtl->nCounterBrink		= 0;	// �_�ŃJ�E���^�[
	pUiCtl->nCounterArrowMove	= 0;	// ���ړ��J�E���^�[
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_attentionMark_00�֐� - UI:���Ӄ}�[�N[00] �̏��������� -
//========================================
void InitUi_attentionMark_00(void)
{
	// �Ǘ����̏���������
	InitParameterUi_attentionMark_00Control();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++)
	{
		D3DXCreateTextureFromFile(pDevice, c_aUi_attentionMark_00Texture[nCntPoly], &g_pTextureUi_attentionMark_00[nCntPoly]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * UI_ATTENTIONMARK_00_MAX * UI_ATTENTIONMARK_00_POLYGON_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_attentionMark_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_attentionMark_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_attentionMark_00	*pUi	// UI:���Ӄ}�[�N[00] �̏��̃|�C���^
						= g_aUi_attentionMark_00;

	for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++, pUi++)
	{
		// �p�����[�^�[�̏���������
		InitParameterUi_attentionMark_00(&g_aUi_attentionMark_00[nCntUi]);

		for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++, pVtx += 4)
		{
			switch (nCntPoly)
			{
			case /*/ ��{ /*/UI_ATTENTIONMARK_00_POLYGON_BASE:
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), 0.0f, 0.0f);

				// ���_�J���[�̐ݒ�
				SetVertexColor3D(pVtx, { 255,255,255,255 });

				// �e�N�X�`�����W�̐ݒ�
				SetTexturePos3D(pVtx, 0, 1, 1, NULL);

				break;
			case /*/ ��� /*/UI_ATTENTIONMARK_00_POLYGON_ARROW:
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-(UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, -UI_ATTENTIONMARK_00_ARROW_DEPTH);
				pVtx[3].pos = D3DXVECTOR3( (UI_ATTENTIONMARK_00_WIDTH * 0.5f), UI_ATTENTIONMARK_00_ARROW_Y_CORRECTION, -UI_ATTENTIONMARK_00_ARROW_DEPTH);

				// �F��ݒ�
				Color col = GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
					UI_ATTENTIONMARK_00_WARNING_COLOR :
					UI_ATTENTIONMARK_00_COLOR;

				// ���_�J���[�̐ݒ�
				pVtx[0].col =
				pVtx[1].col = ColorToD3DCOLOR(col);
				col.a = 0;
				pVtx[2].col =
				pVtx[3].col = ColorToD3DCOLOR(col);

				// �e�N�X�`�����W�̐ݒ�
				SetTexturePos3DCut(pVtx, { 1.0f,1.0f / (float)UI_ATTENTIONMARK_00_ARROW_DIVISION_DEPTH,0.0f }, INITD3DXVECTOR3);

				break;
			}
			
			// �@���x�N�g���̐ݒ�
			SetNormalLine3D(pVtx, UI_ATTENTIONMARK_00_NOR);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_attentionMark_00->Unlock();
}

//========================================
// UninitUi_attentionMark_00�֐� - UI:���Ӄ}�[�N[00] �̏I������ -
//========================================
void UninitUi_attentionMark_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++)
	{
		if (g_pTextureUi_attentionMark_00[nCntPoly] != NULL)
		{
			g_pTextureUi_attentionMark_00[nCntPoly]->Release();
			g_pTextureUi_attentionMark_00[nCntPoly] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_attentionMark_00 != NULL)
	{
		g_pVtxBuffUi_attentionMark_00->Release();
		g_pVtxBuffUi_attentionMark_00 = NULL;
	}
}

//========================================
// UpdateUi_attentionMark_00�֐� - UI:���Ӄ}�[�N[00] �̍X�V���� -
//========================================
void UpdateUi_attentionMark_00(void)
{
	if (GetObj_stage_00()->state == OBJ_STAGE_00_STATE_INTERVAL)
	{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̎��A
		Ui_attentionMark_00Control	*pUiCtl	// UI:���Ӄ}�[�N[00] �̊Ǘ����̃|�C���^
									= &g_ui_attentionMark_00Control;

		float fAlpha =	// �s�����x�����߂A�_�ŃJ�E���^�[���J�E���g
			(float)Count(&pUiCtl->nCounterBrink, 0, UI_ATTENTIONMARK_00_BRINK_TIME, NULL, 0, COUNT_TYPE_TURN) /
			(float)UI_ATTENTIONMARK_00_BRINK_TIME;

		// ���̈ړ��J�E���^�[�����Z����
		++pUiCtl->nCounterArrowMove %= UI_ATTENTIONMARK_00_ARROW_MOVE_TIME;

		// ���̈ړ��J�E���^�[�̐i�s��
		float fArwMvRate = (float)pUiCtl->nCounterArrowMove / (float)UI_ATTENTIONMARK_00_ARROW_MOVE_TIME;

		VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffUi_attentionMark_00->Lock(0, 0, (void**)&pVtx, 0);

		Ui_attentionMark_00	*pUi	// UI:���Ӄ}�[�N[00] �̏��̃|�C���^
							= g_aUi_attentionMark_00;

		// �F��ݒ�
		Color col = GetObj_stage_00Type()->waveSet.aWave[GetObj_stage_00()->nWave].bWarningText ?
			UI_ATTENTIONMARK_00_WARNING_COLOR :
			UI_ATTENTIONMARK_00_COLOR;

		// �s�����x�𔽉f
		col.a *= fAlpha;

		for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++, pUi++)
		{
			if (!pUi->bUse)
			{// �g�p����Ă����ԂłȂ����A
				pVtx += 4 * UI_ATTENTIONMARK_00_POLYGON_MAX;	// ���_�̃|�C���^�����Z
				continue;	// �J��Ԃ�������܂�Ԃ�
			}

			for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++, pVtx += 4)
			{
				switch (nCntPoly)
				{
				case /*/ ��{ /*/UI_ATTENTIONMARK_00_POLYGON_BASE:
					// ���_�J���[��ݒ�
					SetVertexColor3D(pVtx, col);

					break;
				case /*/ ��� /*/UI_ATTENTIONMARK_00_POLYGON_ARROW:
					// ���_�J���[�̐ݒ�
					pVtx[0].col =
					pVtx[1].col = ColorToD3DCOLOR(col);
					Color colTemp = col;
					colTemp.a = 0;
					pVtx[2].col =
					pVtx[3].col = ColorToD3DCOLOR(colTemp);

					// �e�N�X�`�����W�̐ݒ�
					SetTexturePos3DCut(pVtx, { 1.0f,UI_ATTENTIONMARK_00_ARROW_DIVISION_DEPTH,0.0f }, D3DXVECTOR3(0.0f, -fArwMvRate, 0.0f));

					break;
				}
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffUi_attentionMark_00->Unlock();
	}
}

//========================================
// DrawUi_attentionMark_00�֐� - UI:���Ӄ}�[�N[00] �̕`�揈�� -
//========================================
void DrawUi_attentionMark_00(void)
{
	if (GetObj_stage_00()->state != OBJ_STAGE_00_STATE_INTERVAL)
	{// OBJ:�X�e�[�W[00] �̏�Ԃ��C���^�[�o���̂łȂ����A
		return;	// �������I������
	}

	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot,		// �v�Z�p�}�g���b�N�X
						mtxTrans;
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p
	Ui_attentionMark_00	*pUi		// UI:���Ӄ}�[�N[00] �̏��̃|�C���^
						= g_aUi_attentionMark_00;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_attentionMark_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++,pUi++)
	{
		if ((!pUi->bUse) || (!pUi->bShow))
		{// �g�p����Ă����ԂłȂ� or ���t���O���U�̎��A
			// ������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldUi_attentionMark_00);

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pUi->rot.y, pUi->rot.x, pUi->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldUi_attentionMark_00, &g_mtxWorldUi_attentionMark_00, &mtxRot);
		
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pUi->pos.x, pUi->pos.y, pUi->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldUi_attentionMark_00, &g_mtxWorldUi_attentionMark_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldUi_attentionMark_00);

		for (int nCntPoly = 0; nCntPoly < UI_ATTENTIONMARK_00_POLYGON_MAX; nCntPoly++)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureUi_attentionMark_00[nCntPoly]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((4 * UI_ATTENTIONMARK_00_POLYGON_MAX) * nCntUi) + (nCntPoly * 4), 2);
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//========================================
// SetUi_attentionMark_00�֐� - UI:���Ӄ}�[�N[00] �̐ݒ菈�� -
//========================================
void SetUi_attentionMark_00(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	Ui_attentionMark_00	*pUi	// UI:���Ӄ}�[�N[00] �̏��̃|�C���^
						= g_aUi_attentionMark_00;

	for (int nCntUi = 0; nCntUi < UI_ATTENTIONMARK_00_MAX; nCntUi++, pUi++)
	{
		if (pUi->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pUi->pos = pos;		// �ʒu����
		pUi->rot = rot;		// ��������
		pUi->bUse = true;	// �g�p����Ă����Ԃɂ���

		// �J��Ԃ������𔲂���
		break;
	}
}