//========================================
// 
// ITM:�R�C��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** itm_coin_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "chr_player_00.h"		// CHR:�v���C���[	[00]
#include "itm_coin_00.h"		// ITM:�R�C��		[00]
#include "obj_block_00.h"		// OBJ:�u���b�N		[00]
#include "obj_pedestal_00.h"	// OBJ:���			[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// ITM:�R�C��[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define ITM_COIN_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ITEM\\ITM_COIN_00_TYPE_DATA.txt"

// ITM:�R�C��[00] �̍ő吔
// ITM:�R�C��[00] �̎�ސ�
#define ITM_COIN_00_MAX			(256)
#define ITM_COIN_00_TYPE_NUM	(1)

// ITM:�R�C��[00] �̖@���x�N�g��
#define ITM_COIN_00_NOR	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

// ITM:�R�C��[00] �̈ړ��͂̉���
// ITM:�R�C��[00] �̈ړ��͂̏��
#define ITM_COIN_00_MOVE_FORCE_MIN	(1.0f)
#define ITM_COIN_00_MOVE_FORCE_MAX	(2.0f)

// ITM:�R�C��[00] �̈ړ��͂̌����{��
#define ITM_COIN_00_MOVE_FORCE_DAMP	(0.9f)

// ITM:�R�C��[00] �̉�]��
#define ITM_COIN_00_SPIN	D3DXVECTOR3(0.0f,0.1f,0.0f)

// ITM:�R�C��[00] �̒ǔ�����
// ITM:�R�C��[00] �̒ǔ���
#define ITM_COIN_00_HOMING_DISTANCE	(80.0f)
#define ITM_COIN_00_HOMING_POWER	(2.0f)

// ITM:�R�C��[00] �̓_�ł�������̃��C��
// ITM:�R�C��[00] �̓_�łɂ����鎞��
#define ITM_COIN_00_BLINK_LIFE_LINE	(120)
#define ITM_COIN_00_BLINK_TIME		(2)

//****************************************
// �񋓌^�̒�`
//****************************************
// ITM:�R�C��[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum
{
	ITM_COIN_00_HITOBJ_OBJ_BLOCK_00,	// OBJ:�u���b�N	[00]
	ITM_COIN_00_HITOBJ_OBJ_PEDESTAL_00,	// OBJ:���		[00]
	ITM_COIN_00_HITOBJ_OBJ_STAGE_00,	// OBJ:�X�e�[�W	[00]
	ITM_COIN_00_HITOBJ_MAX,
}ITM_COIN_00_HITOBJ;

//****************************************
// �\���̂̒�`
//****************************************
// ITM:�R�C��[00] �̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 posOld;		// �ߋ��̈ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 dispRot;	// �\����̌���

	// ���ފ֘A
	int nType;	// ���

	// ��Ԋ֘A
	bool	bUse;		// �g�p����Ă��邩�t���O
	int		nLife;		// ����
	float	fMoveForce;	// �ړ���

	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
	Collision	cmnCollision; 	// ���ʂ̏Փˏ��
	Collision	aObjCollision	// �I�u�W�F�N�g���̏Փˏ��
				[ITM_COIN_00_HITOBJ_MAX];
}Itm_coin_00;

// ITM:�R�C��[00] �̎�ޖ��̏��\����
typedef struct
{
	int			nLife;			// ����
	float		fWidth;			// ��
	float		fHeight;		// ����
	char		aTexturePath	// �e�N�X�`���̑��΃p�X
				[TXT_MAX];
	SOUND_LABEL	destroySE;		// �j��SE
	HitTestSet	hitTestSet;		// �����蔻��ݒ���
}Itm_coin_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ITM:�R�C��[00] �̃p�����[�^�[�̏���������
void InitParameterItm_coin_00(Itm_coin_00 *pItm);

// ITM:�R�C��[00] �̈ʒu�X�V����
// ITM:�R�C��[00] �̏Փ˃`�F�b�N����
void UpdatePosItm_coin_00(Itm_coin_00 *pItm);
void CheckCollisionItm_coin_00(VECTOR vector, Itm_coin_00 *pItm);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureItm_coin_00	// ITM:�R�C��[00] �̃e�N�X�`���ւ̃|�C���^
						[ITM_COIN_00_TYPE_NUM]
						= {};
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffItm_coin_00	// ITM:�R�C��[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldItm_coin_00;	// ITM:�R�C��[00] �̃��[���h�}�g���b�N�X
Itm_coin_00				g_aItm_coin_00			// ITM:�R�C��[00] �̏��
						[ITM_COIN_00_MAX];
Itm_coin_00Type			g_aItm_coin_00Type		// ITM:�R�C��[00] �̎�ޖ��̏��
						[ITM_COIN_00_TYPE_NUM];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterItm_coin_00�֐� - ITM:�R�C��[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterItm_coin_00(Itm_coin_00 *pItm)
{
	pItm->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̈ʒu
	pItm->posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ߋ��̈ʒu
	pItm->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pItm->dispRot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �\����̌���
	pItm->nType			= 0;								// ���
	pItm->bUse			= false;							// �g�p����Ă��邩�t���O
	pItm->nLife			= 0;								// ����
	pItm->fMoveForce	= 0.0f;								// �ړ���
	pItm->hitTestInfo	= {};								// �����蔻��̊Ǘ����
	pItm->cmnCollision	= {};								// ���ʂ̏Փˏ��
	for (int nCntHitObj = 0; nCntHitObj < ITM_COIN_00_HITOBJ_MAX; nCntHitObj++)
	{
		pItm->aObjCollision[nCntHitObj] = {};				// �I�u�W�F�N�g���̏Փˏ��
	}
}

//========================================
// UpdatePosItm_coin_00�֐� - ITM:�R�C��[00] �̈ʒu�X�V���� -
//========================================
void UpdatePosItm_coin_00(Itm_coin_00 *pItm)
{
	// ���ʂ̏Փˏ���������
	pItm->cmnCollision = {};

	// �I�u�W�F�N�g���̏Փˏ���������
	for (int nCntHitObj = 0; nCntHitObj < ITM_COIN_00_HITOBJ_MAX; nCntHitObj++)
	{
		pItm->aObjCollision[nCntHitObj] = {};
	}

	pItm->posOld = pItm->pos;	// �ߋ��̈ʒu��ۑ�

	pItm->pos.x += sinf(pItm->rot.y) * pItm->fMoveForce;	// X���W�X�V
	CheckCollisionItm_coin_00(VECTOR_X, pItm);				// X�����̏Փ˃`�F�b�N����

	pItm->pos.z += cosf(pItm->rot.y) * pItm->fMoveForce;	// Z���W�X�V
	CheckCollisionItm_coin_00(VECTOR_Z, pItm);				// Z�����̏Փ˃`�F�b�N����
}


//========================================
// CheckCollisionItm_coin_00�֐� - ITM:�v���C���[[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionItm_coin_00(VECTOR vector, Itm_coin_00 *pItm)
{
	Itm_coin_00Type	*pType	// ITM:�v���C���[[00] �̎�ޖ��̏��̃|�C���^
					= &g_aItm_coin_00Type[pItm->nType];

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&pItm->pos,
		pItm->posOld,
		NULL,
		&pItm->rot,
		pItm->rot,
		pType->hitTestSet.aHitTest[pItm->hitTestInfo.nHitTest]
	};

	// �Փ˔���
	/*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00	(vector, &pItm->aObjCollision[ITM_COIN_00_HITOBJ_OBJ_BLOCK_00]		, &pItm->cmnCollision, myCollInfo);
	/*/ OBJ:���		[00] /*/CollisionObj_pedestal_00(vector, &pItm->aObjCollision[ITM_COIN_00_HITOBJ_OBJ_PEDESTAL_00]	, &pItm->cmnCollision, myCollInfo);
	/*/ OBJ:�X�e�[�W	[00] /*/CollisionObj_stage_00	(vector, &pItm->aObjCollision[ITM_COIN_00_HITOBJ_OBJ_STAGE_00]		, &pItm->cmnCollision, myCollInfo);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadItm_coin_00�֐� - ITM:�R�C��[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadItm_coin_00(void)
{
	FILE			*pFile;		// �t�@�C���|�C���^
	char			aDataSearch	// �f�[�^�����p
					[TXT_MAX];
	Itm_coin_00Type	*pItmType	// ITM:�R�C��[00] �̎�ޖ��̏��
					= g_aItm_coin_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(ITM_COIN_00_TYPE_DATA_FILE_PATH, "r");

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
			// ITM:�R�C��[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pItmType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LIFE:"))			{ fscanf(pFile, "%d", &pItmType->nLife); }			// ����
				else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pItmType->fWidth); }			// ��
				else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pItmType->fHeight); }		// ����
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:")) { fscanf(pFile, "%s", &pItmType->aTexturePath); }	// �e�N�X�`���̑��΃p�X
				else if (!strcmp(aDataSearch, "DESTROY_SE:"))	{ fscanf(pFile, "%d", &pItmType->destroySE); }		// �j��SE
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pItmType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitItm_coin_00�֐� - ITM:�R�C��[00] �̏��������� -
//========================================
void InitItm_coin_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < ITM_COIN_00_TYPE_NUM; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aItm_coin_00Type[nCntType].aTexturePath, &g_pTextureItm_coin_00[nCntType]);
	}
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * ITM_COIN_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItm_coin_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItm_coin_00->Lock(0, 0, (void**)&pVtx, 0);

	Itm_coin_00	*pItm	// ITM:�R�C��[00] �̏��̃|�C���^
				= g_aItm_coin_00;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pItm++, pVtx += 4)
	{
		// �p�����[�^�[�̏���������
		InitParameterItm_coin_00(&g_aItm_coin_00[nCntItm]);

		// �@���x�N�g���̐ݒ�
		SetNormalLine3D(pVtx, ITM_COIN_00_NOR);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx, 0, 1, 1, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItm_coin_00->Unlock();
}

//========================================
// UninitItm_coin_00�֐� - ITM:�R�C��[00] �̏I������ -
//========================================
void UninitItm_coin_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < ITM_COIN_00_TYPE_NUM; nCntType++)
	{
		if (g_pTextureItm_coin_00[nCntType] != NULL)
		{
			g_pTextureItm_coin_00[nCntType]->Release();
			g_pTextureItm_coin_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffItm_coin_00 != NULL)
	{
		g_pVtxBuffItm_coin_00->Release();
		g_pVtxBuffItm_coin_00 = NULL;
	}
}

//========================================
// UpdateItm_coin_00�֐� - ITM:�R�C��[00] �̍X�V���� -
//========================================
void UpdateItm_coin_00(void)
{
	Itm_coin_00	*pItm	// ITM:�R�C��[00] �̏��̃|�C���^
				= g_aItm_coin_00;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pItm++)
	{
		if (!pItm->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}
		else if (--pItm->nLife <= 0) 
		{// ���������Z��������0�ȉ��̎��A
			pItm->bUse = false;	// �g�p���Ă��Ȃ���Ԃɂ���
			continue;			// ������܂�Ԃ�
		}

		// �ʒu�X�V����
		UpdatePosItm_coin_00(pItm);

		if (GetChr_player_00()->state != CHR_PLAYER_00_STATE_DOWN)
		{// �v���C���[�̏�Ԃ��_�E���łȂ����A
			float	fDistance	// �v���C���[�Ƃ̋���
					= FindDistanceLookDown(pItm->pos, GetChr_player_00()->pos);
			float	fAngle		// �v���C���[�܂ł̌���
					= FindAngleLookDown(pItm->pos, GetChr_player_00()->pos);

			// �����ɉ����ăv���C���[�܂ł̌����Ɉʒu�����Z
			pItm->pos.x += sinf(fAngle) * Prus(1.0f - ((float)fDistance / (float)ITM_COIN_00_HOMING_DISTANCE)) * ITM_COIN_00_HOMING_POWER;
			pItm->pos.z += cosf(fAngle) * Prus(1.0f - ((float)fDistance / (float)ITM_COIN_00_HOMING_DISTANCE)) * ITM_COIN_00_HOMING_POWER;
		}

		// �ړ��͂�����
		pItm->fMoveForce *= ITM_COIN_00_MOVE_FORCE_DAMP;

		// �\�������ɉ�]�ʂ����Z
		pItm->dispRot += ITM_COIN_00_SPIN;

		// �\�������𐧌�
		RotControl(&pItm->dispRot);
	}
}

//========================================
// DrawItm_coin_00�֐� - ITM:�R�C��[00] �̕`�揈�� -
//========================================
void DrawItm_coin_00(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot,		// �v�Z�p�}�g���b�N�X
						mtxTrans;
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p
	Itm_coin_00			*pItm		// ITM:�R�C��[00] �̏��̃|�C���^
						= g_aItm_coin_00;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItm_coin_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++,pItm++)
	{
		if ((!pItm->bUse)
			||
			(pItm->nLife <= ITM_COIN_00_BLINK_LIFE_LINE) && (pItm->nLife % (ITM_COIN_00_BLINK_TIME * 2) < ITM_COIN_00_BLINK_TIME))
		{// �g�p����Ă����ԂłȂ����A
			// ������܂�Ԃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldItm_coin_00);

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		{
			D3DXVECTOR3 setRot = pItm->dispRot;	// �ݒ����
			float fAngleDifference				// �ݒ�����Ǝ��_���猻�݈ʒu�܂ł̌����̍�
				= AngleDifference(
					FindAngleLookDown(GetCamera3D()->posV, pItm->pos),
					setRot.y);
			if ((fAngleDifference >= D3DX_PI * 0.5f)
				||
				(fAngleDifference <= D3DX_PI * -0.5f))
			{// �ݒ�����Ǝ��_���猻�݈ʒu�܂ł̌����̍���90�x�ȏ�̎��A
				setRot.y += D3DX_PI;	// �����ɉ~����(������)�����Z
			}

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, setRot.y, setRot.x, setRot.z);
			D3DXMatrixMultiply(&g_mtxWorldItm_coin_00, &g_mtxWorldItm_coin_00, &mtxRot);
		}
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pItm->pos.x, pItm->pos.y, pItm->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldItm_coin_00, &g_mtxWorldItm_coin_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItm_coin_00);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureItm_coin_00[pItm->nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItm, 2);
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//========================================
// SetItm_coin_00�֐� - ITM:�R�C��[00] �̐ݒ菈�� -
//========================================
void SetItm_coin_00(D3DXVECTOR3 pos, int nSetNum)
{
	if (nSetNum <= 0) 
	{// �ݒ萔��0�ȉ��̎��A
		return;	// �������I������
	}

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItm_coin_00->Lock(0, 0, (void**)&pVtx, 0);

	Itm_coin_00	*pItm	// ITM:�R�C��[00] �̏��̃|�C���^
				= g_aItm_coin_00;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pVtx += 4, pItm++)
	{
		if (pItm->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		Itm_coin_00Type *pType	// ITM:�R�C��[00] �̎�ޖ��̏��
						= &g_aItm_coin_00Type[pItm->nType];

		pItm->pos = pos;	// �ʒu����
		pItm->rot.y =		// �����𗐐��Őݒ�
			-D3DX_PI + fRand(D3DX_PI * 2);
		pItm->dispRot.y =	// �\����̌����𗐐��Őݒ�
			-D3DX_PI + fRand(D3DX_PI * 2);
		pItm->bUse = true;	// �g�p����Ă����Ԃɂ���
		pItm->fMoveForce =	// �ړ��͂𗐐��Őݒ�
			ITM_COIN_00_MOVE_FORCE_MIN + 
			fRand(ITM_COIN_00_MOVE_FORCE_MAX - ITM_COIN_00_MOVE_FORCE_MIN);
		pItm->nLife = pType->nLife;	// ������ݒ肷��

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f), pType->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (pType->fWidth * 0.5f), pType->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(pType->fWidth * 0.5f), 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (pType->fWidth * 0.5f), 0.0f, 0.0f);

		if (--nSetNum <= 0) 
		{// �ݒ萔�����Z��������0�ȉ��̎��A
			break;	// �J��Ԃ������𔲂���
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItm_coin_00->Unlock();
}

//========================================
// CollisionItm_coin_00�֐� - ITM:�R�C��[00] �Ƃ̓����蔻�菈�� -
//========================================
int CollisionItm_coin_00(VECTOR vector, Collision *pItmCollision, Collision *pCmnCollision, CollisionInfo myCollInfo)
{
	bool		bGhostTemp	// ������t���O��ۑ�
				= pItmCollision->bGhost;
	Itm_coin_00	*pItm		// ITM:�R�C��[00] �̏��̃|�C���^
				= g_aItm_coin_00;
	int			nIndex		// CHR:�G[00] �̕Ԃ��̔ԍ�
				= -1;

	for (int nCntItm = 0; nCntItm < ITM_COIN_00_MAX; nCntItm++, pItm++)
	{
		if (!pItm->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		// �Փ˔���ɕK�v�ȏ��
		CollisionCheck(
			vector,
			pItmCollision,
			myCollInfo,
			{
				&pItm->pos,
				pItm->pos,
				NULL,
				&pItm->rot,
				pItm->rot,
				g_aItm_coin_00Type[pItm->nType].hitTestSet.aHitTest[pItm->hitTestInfo.nHitTest]
			},
			COLLCHK_MODE_LOOKDOWN);

		if ((pItmCollision->bGhost) && (!bGhostTemp))
		{// ������t���O���^�̎��A
			nIndex = nCntItm;	// �Ԃ��̔ԍ���ݒ�
			bGhostTemp = true;	// ������t���O�̕ۑ���^�ɂ���
		}
	}

	// ���ʂ̏Փˏ����㏑��
	CollisionOverwrite(pCmnCollision, *pItmCollision);

	// �ԍ���Ԃ�
	return nIndex;
}

//========================================
// DestroyItm_coin_00�֐� - ITM:�R�C��[00] �̔j�󏈗� -
//========================================
void DestroyItm_coin_00(int nIndex) 
{
	Itm_coin_00		*pItm	// ITM:�R�C��[00] �̏��̃|�C���^
					= &g_aItm_coin_00[nIndex];
	Itm_coin_00Type *pType	// ITM:�R�C��[00] �̎�ޖ��̏��
					= &g_aItm_coin_00Type[pItm->nType];

	pItm->bUse = false;				// �g�p����Ă��Ȃ���Ԃɂ���
	PlaySound(pType->destroySE);	// �j��SE
}