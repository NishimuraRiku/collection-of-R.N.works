//========================================
// 
// EFF:��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_light_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "eff_light_00.h"	// EFF:��	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// EFF:��[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define EFF_LIGHT_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\EFFECT\\EFF_LIGHT_00_TYPE_DATA.txt"

// EFF:��[00] �̍ő吔
// EFF:��[00] �̎�ނ̍ő吔
#define EFF_LIGHT_00_MAX		(2048)
#define EFF_LIGHT_00_TYPE_MAX	(8)

// EFF:��[00] �̖@���x�N�g��
// EFF:��[00] �̑��Έʒu
#define EFF_LIGHT_00_NOR			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define EFF_LIGHT_00_RELATIVE_POS	(D3DXVECTOR3(0.0f, 4.0f, 0.0f))

//****************************************
// �\���̂̒�`
//****************************************
// EFF:��[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;	// �ʒu
	
	// ���ފ֘A
	int	nType;	// ���
	
	// �|���S���֘A
	int nPtn;			// �p�^�[��No.
	int nCounterAnim;	// �A�j���[�V�����J�E���^�[

	// ��Ԋ֘A
	bool	bUse;	// �g�p����Ă��邩�t���O
	int		nLife;	// ����
}Eff_light_00;

// EFF:��[00] �̎�ޖ��̏��\����
typedef struct
{
	float		fWidth;			// ��
	float		fHeight;		// ����
	char		aTexturePath	// �e�N�X�`���̑��΃p�X
				[TXT_MAX];
	int			nPtnWidth;		// �p�^�[����
	int			nPtnHeight;		// �p�^�[������
	int			nPtnMax;		// �p�^�[�����
	int			nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	int			nLife;			// ����
	Color		col;			// �F
}Eff_light_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:��[00] �̃p�����[�^�[�̏���������
void InitParameterEff_light_00(Eff_light_00 *pEff);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_light_00	// EFF:��[00] �̃e�N�X�`���ւ̃|�C���^
						[EFF_LIGHT_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_light_00	// EFF:��[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldEff_light_00;	// EFF:��[00] �̃��[���h�}�g���b�N�X
Eff_light_00			g_aEff_light_00			// EFF:��[00] �̏��
						[EFF_LIGHT_00_MAX];
Eff_light_00Type		g_aEff_light_00Type		// EFF:��[00] �̎�ޖ��̏��
						[EFF_LIGHT_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_light_00�֐� - EFF:��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterEff_light_00(Eff_light_00 *pEff)
{
	pEff->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pEff->nType			= 0;								// ���
	pEff->nPtn			= 0;								// �p�^�[��No.
	pEff->nCounterAnim	= 0;								// �A�j���[�V�����J�E���^�[
	pEff->bUse			= false;							// �g�p����Ă��邩�t���O
	pEff->nLife			= 0;
}

//========================================
// DestroyEff_light_00�֐� - EFF:��[00] �̔j�󏈗� -
//========================================
void DestroyEff_light_00(Eff_light_00 *pEff) 
{
	pEff->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadEff_light_00�֐� - EFF:��[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadEff_light_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Eff_light_00Type	*pEffType	// EFF:��[00] �̎�ޖ��̏��
						= g_aEff_light_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(EFF_LIGHT_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// �ǂݍ��݂��I��
		if (aDataSearch[0] == '#')			{ continue; }				// �܂�Ԃ�

		if (!strcmp(aDataSearch, "TYPE")) {
			// EFF:��[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pEffType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pEffType->fWidth); }			// ��
				else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pEffType->fHeight); }		// ����
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))	{ fscanf(pFile, "%s", &pEffType->aTexturePath); }	// �e�N�X�`���̑��΃p�X
				else if (!strcmp(aDataSearch, "PTN_WIDTH:"))	{ fscanf(pFile, "%d", &pEffType->nPtnWidth); }		// �p�^�[����
				else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))	{ fscanf(pFile, "%d", &pEffType->nPtnHeight); }		// �p�^�[������
				else if (!strcmp(aDataSearch, "PTN_MAX:"))		{ fscanf(pFile, "%d", &pEffType->nPtnMax); }		// �p�^�[�����
				else if (!strcmp(aDataSearch, "ANIM_TIME:"))	{ fscanf(pFile, "%d", &pEffType->nAnimTime); }		// �A�j���[�V�����ɂ����鎞��
				else if (!strcmp(aDataSearch, "LIFE:"))			{ fscanf(pFile, "%d", &pEffType->nLife); }			// ����
				else if (!strcmp(aDataSearch, "COLOR:"))		{	// �F
					fscanf(pFile, "%d", &pEffType->col.r);			// R�l
					fscanf(pFile, "%d", &pEffType->col.g);			// G�l
					fscanf(pFile, "%d", &pEffType->col.b);			// B�l
					fscanf(pFile, "%d", &pEffType->col.a);			// A�l
				}
			}
		}
	}
}

//========================================
// InitEff_light_00�֐� - EFF:��[00] �̏��������� -
//========================================
void InitEff_light_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < EFF_LIGHT_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aEff_light_00Type[nCntType].aTexturePath, &g_aTextureEff_light_00[nCntType]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFF_LIGHT_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_light_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_light_00	*pEff	// EFF:��[00] �̏��̃|�C���^
					= g_aEff_light_00;

	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++, pEff++, pVtx += 4)
	{
		Eff_light_00Type	*pType	// EFF:��[00] �̎�ޖ��̏��̃|�C���^
							= &g_aEff_light_00Type[pEff->nType];

		// �p�����[�^�[�̏���������
		InitParameterEff_light_00(&g_aEff_light_00[nCntEff]);

		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, EFF_LIGHT_00_NOR);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, pType->col);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx, 0, pType->nPtnWidth, pType->nPtnHeight, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// UninitEff_light_00�֐� - EFF:��[00] �̏I������ -
//========================================
void UninitEff_light_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < EFF_LIGHT_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureEff_light_00[nCntType] != NULL)
		{
			g_aTextureEff_light_00[nCntType]->Release();
			g_aTextureEff_light_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_light_00 != NULL)
	{
		g_pVtxBuffEff_light_00->Release();
		g_pVtxBuffEff_light_00 = NULL;
	}
}

//========================================
// UpdateEff_light_00�֐� - EFF:��[00] �̍X�V���� -
//========================================
void UpdateEff_light_00(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_light_00	*pEff	// EFF:��[00] �̏��̃|�C���^
					= g_aEff_light_00;
	
	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++, pEff++, pVtx += 4)
	{
		if (!pEff->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}
		else if (--pEff->nLife <= 0) 
		{// ���������Z��������0�ȉ��̎��A
			pEff->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
			continue;			// �J��Ԃ�������܂�Ԃ�
		}

		Eff_light_00Type	*pType	// EFF:��[00] �̎�ޖ��̏��̃|�C���^
							= &g_aEff_light_00Type[pEff->nType];

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx,
			Count(
				&pEff->nPtn,
				0,
				g_aEff_light_00Type[pEff->nType].nPtnMax,
				&pEff->nCounterAnim,
				g_aEff_light_00Type[pEff->nType].nAnimTime,
				COUNT_TYPE_NORMAL),
			g_aEff_light_00Type[pEff->nType].nPtnWidth,
			g_aEff_light_00Type[pEff->nType].nPtnHeight, NULL);

		// ���_�J���[�̐ݒ�
		{
			Color col = pType->col;	// �F
			col.a *= ((float)pEff->nLife / (float)pType->nLife);
			SetVertexColor3D(pVtx, col);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// DrawEff_light_00�֐� - EFF:��[00] �̕`�揈�� -
//========================================
void DrawEff_light_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p
	Eff_light_00		*pEff		// EFF:��[00] �̏��̃|�C���^
						= g_aEff_light_00;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_light_00, 0, sizeof(VERTEX_3D));

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

	// �A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++,pEff++)
	{
		if (!pEff->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldEff_light_00);

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_mtxWorldEff_light_00, NULL, &mtxView);
		g_mtxWorldEff_light_00._41 = 0.0f;
		g_mtxWorldEff_light_00._42 = 0.0f;
		g_mtxWorldEff_light_00._43 = 0.0f;
		{
			D3DXVECTOR3 setPos = pEff->pos + EFF_LIGHT_00_RELATIVE_POS;	// �ݒ�ʒu��ݒ�

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);
			D3DXMatrixMultiply(&g_mtxWorldEff_light_00, &g_mtxWorldEff_light_00, &mtxTrans);
		}
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEff_light_00);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureEff_light_00[pEff->nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEff, 2);
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	// �A���t�@�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// SetEff_light_00�֐� - EFF:��[00] �̐ݒ菈�� -
//========================================
void SetEff_light_00(D3DXVECTOR3 pos,int nType)
{
	if (nType == -1)
	{// ��ނ�-1�̎��A
		return;	// �������I��
	}

	Eff_light_00	*pEff	// EFF:��[00] �̏��̃|�C���^
					= g_aEff_light_00;
	VERTEX_3D		*pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff = 0; nCntEff < EFF_LIGHT_00_MAX; nCntEff++, pEff++, pVtx += 4)
	{
		if (pEff->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Eff_light_00Type	*pType	// ��ޖ��̏��̃|�C���^
							= &g_aEff_light_00Type[nType];

		pEff->pos = pos;			// �ʒu����
		pEff->nType = nType;		// ��ނ���
		pEff->nPtn = 0;				// �p�^�[��No.��������
		pEff->nCounterAnim = 0;		// �A�j���[�V�����J�E���^�[��������
		pEff->bUse = true;			// �g�p����Ă����Ԃɂ���
		pEff->nLife = pType->nLife;	// ������ݒ肷��

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f),  (pType->fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (pType->fWidth * 0.5f),  (pType->fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f), -(pType->fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (pType->fWidth * 0.5f), -(pType->fHeight * 0.5f), 0.0f);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, pType->col);

		// �J��Ԃ������𔲂���
		break;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_light_00->Unlock();
}