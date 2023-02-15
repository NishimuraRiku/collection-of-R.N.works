//========================================
// 
// OBJ:�^�[�Q�b�g[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_target_00.cpp ***
//========================================
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "main.h"
#include "physics.h"
#include "polygon3D.h"
#include "sound.h"
#include "chr_player_00.h"	// CHR:�v���C���[	[00]
#include "obj_target_00.h"	// OBJ:�^�[�Q�b�g	[00]
#include "obj_block_00.h"	// OBJ:�u���b�N		[00]
#include "obj_stage_00.h"	// OBJ:�X�e�[�W		[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// OBJ:�^�[�Q�b�g[00] �̃��f���̑��΃p�X
// OBJ:�^�[�Q�b�g[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define OBJ_TARGET_00_MODEL_PATH			"data\\MODEL\\Object\\obj_target_000\\Model\\body_x0.1.x"
#define OBJ_TARGET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\OBJ_TARGET_00_TYPE_DATA.txt"

// OBJ:�^�[�Q�b�g[00] �̎�ނ̍ő吔
#define OBJ_TARGET_00_TYPE_MAX	(1)

//****************************************
// �񋓌^�̒�`
//****************************************
// OBJ:�^�[�Q�b�g[00] �̏Փ˃I�u�W�F�N�g�ԍ�
typedef enum
{
	OBJ_TARGET_00_HITOBJ_BLOCK_00,	// OBJ:�u���b�N	[00]
	OBJ_TARGET_00_HITOBJ_STAGE_00,	// OBJ:�X�e�[�W	[00]
	OBJ_TARGET_00_HITOBJ_MAX,
}OBJ_TARGET_00_HITOBJ;

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:�^�[�Q�b�g[00] �̏��\����
typedef struct
{
	// �^���֘A
	HitTestInfo hitTestInfo;	// �����蔻��̊Ǘ����
	Collision	cmnCollision; 	// ���ʂ̏Փˏ��
	Collision	aObjCollision	// �I�u�W�F�N�g���̏Փˏ��
				[OBJ_TARGET_00_HITOBJ_MAX];
}Obj_target_00;

// OBJ:�^�[�Q�b�g[00] �̎�ޖ��̏��\����
typedef struct
{
	HitTestSet	hitTestSet;	// �����蔻��ݒ���
}Obj_target_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:�^�[�Q�b�g[00] �̃p�����[�^�[�̏���������
void InitParameterObj_target_00(void);

// OBJ:�^�[�Q�b�g[00] �̏Փ˃`�F�b�N����
void CheckCollisionObj_target_00(VECTOR vector);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9	g_pTextureObj_target_00		// OBJ:�^�[�Q�b�g[00] �̃e�N�X�`���ւ̃|�C���^
					[MATERIAL_3D_MAX]
					= {};
LPD3DXMESH			g_pMeshObj_target_00		// OBJ:�^�[�Q�b�g[00] �̃��b�V��(���_���)�ւ̃|�C���^
					= NULL;
LPD3DXBUFFER		g_pBuffMatObj_target_00		// OBJ:�^�[�Q�b�g[00] �̃}�e���A���ւ̃|�C���^
					= NULL;
DWORD				g_dwNumMatObj_target_00		// OBJ:�^�[�Q�b�g[00] �̃}�e���A���̐�
					= 0;
D3DXMATRIX			g_mtxWorldObj_target_00;	// OBJ:�^�[�Q�b�g[00] �̃��[���h�}�g���b�N�X
Obj_target_00		g_obj_target_00;			// OBJ:�^�[�Q�b�g[00] �̏��
Obj_target_00Type	g_aObj_target_00Type		// OBJ:�^�[�Q�b�g[00] �̎�ޖ��̏��
					[OBJ_TARGET_00_TYPE_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̃p�����[�^�[�̏��������� -
//========================================
void InitParameterObj_target_00(void) 
{
	Obj_target_00	*pObj	// OBJ:�^�[�Q�b�g[00] �̏��̃|�C���^
					= &g_obj_target_00;

	pObj->hitTestInfo					= {};	// �����蔻��̊Ǘ����
	pObj->cmnCollision					= {};	// ���ʂ̏Փˏ��
	for (int nCntHitObj = 0; nCntHitObj < OBJ_TARGET_00_HITOBJ_MAX; nCntHitObj++) 
	{
		pObj->aObjCollision[nCntHitObj]	= {};	// �I�u�W�F�N�g���̏Փˏ��
	}
}

//========================================
// CheckCollisionObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̏Փ˃`�F�b�N���� -
//========================================
void CheckCollisionObj_target_00(VECTOR vector)
{
	Obj_target_00	*pObj	// OBJ:�^�[�Q�b�g[00] �̏��̃|�C���^
					= &g_obj_target_00;

	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo	myCollInfo =
	{
		&GetChr_player_00()->setPos,
		GetChr_player_00()->pos,
		NULL,
		&GetChr_player_00()->rot,
		GetChr_player_00()->rot,
		g_aObj_target_00Type[0].hitTestSet.aHitTest[pObj->hitTestInfo.nHitTest]
	};

	// �Փ˔���
	/*/ OBJ:�u���b�N	[00] /*/CollisionObj_block_00(vector, &pObj->aObjCollision[OBJ_TARGET_00_HITOBJ_BLOCK_00], &pObj->cmnCollision, myCollInfo);
	/*/ OBJ:�X�e�[�W	[00] /*/CollisionObj_stage_00(vector, &pObj->aObjCollision[OBJ_TARGET_00_HITOBJ_BLOCK_00], &pObj->cmnCollision, myCollInfo);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadObj_target_00(void)
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Obj_target_00Type	*pObjType	// OBJ:�^�[�Q�b�g[00] �̎�ޖ��̏��
						= g_aObj_target_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(OBJ_TARGET_00_TYPE_DATA_FILE_PATH, "r");

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
			// OBJ:�^�[�Q�b�g[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "TYPE_END"))		{ pObjType++; break; }								// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "HITTESTSET"))	{ LoadHitTestSet(pFile, &pObjType->hitTestSet); }	// �����蔻��ݒ���
			}
		}
	}
}

//========================================
// InitObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̏��������� -
//========================================
void InitObj_target_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterObj_target_00();

	Obj_target_00		*pObj	// OBJ:�^�[�Q�b�g[00] �̏��̃|�C���^
						= &g_obj_target_00;
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		OBJ_TARGET_00_MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObj_target_00,
		NULL,
		&g_dwNumMatObj_target_00,
		&g_pMeshObj_target_00
	);

	D3DXMATERIAL *pMat;	// �}�e���A���ւ̃|�C���^

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_target_00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObj_target_00; nCntMat++) 
	{
		if (pMat[nCntMat].pTextureFilename != NULL) 
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureObj_target_00[nCntMat]);
		}
	}
}

//========================================
// UninitObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̏I������ -
//========================================
void UninitObj_target_00(void)
{
	// ���b�V���̔j��
	if (g_pMeshObj_target_00 != NULL)
	{
		g_pMeshObj_target_00->Release();
		g_pMeshObj_target_00 = NULL;
	}

	// �}�e���A���̔j��
	if (g_pBuffMatObj_target_00 != NULL)
	{
		g_pBuffMatObj_target_00->Release();
		g_pBuffMatObj_target_00 = NULL;
	}
}

//========================================
// UpdateObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̍X�V���� -
//========================================
void UpdateObj_target_00(void)
{
	Obj_target_00	*pObj	// OBJ:�^�[�Q�b�g[00] �̏��̃|�C���^
					= &g_obj_target_00;
}

//========================================
// DrawObj_target_00�֐� - OBJ:�^�[�Q�b�g[00] �̕`�揈�� -
//========================================
void DrawObj_target_00(void)
{
	if ((!GetChr_player_00Item()[GetChr_player_00()->nSelectItem].bSet)
		||
		(GetChr_player_00()->setPosCmnCollision.bInside)) 
	{// �I������Ă���A�C�e���̐ݒu�t���O���U or �ݒ�ʒu�̏d�Ȃ�t���O���^�̎��A
		return;	// �������I������
	}

	Obj_target_00		*pObj				// OBJ:�^�[�Q�b�g[00] �̏��̃|�C���^
						= &g_obj_target_00;
	LPDIRECT3DDEVICE9	pDevice				// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL		*pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldObj_target_00);
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_mtxWorldObj_target_00, &g_mtxWorldObj_target_00, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetChr_player_00()->setPos.x, GetChr_player_00()->setPos.y, GetChr_player_00()->setPos.z);
	D3DXMatrixMultiply(&g_mtxWorldObj_target_00, &g_mtxWorldObj_target_00, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObj_target_00);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObj_target_00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObj_target_00; nCntMat++) 
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureObj_target_00[nCntMat]);

		// �|���S��(�p�[�c)�̕`��
		g_pMeshObj_target_00->DrawSubset(nCntMat);
	}
}