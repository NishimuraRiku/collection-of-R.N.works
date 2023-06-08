//========================================
// 
// �����֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** physics.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//****************************************
// �}�N����`
//****************************************
// �`�悷�铖���蔻��̍ő吔
#define DRAWHITTEST_MAX (256)
// �����蔻��̐��̍ő吔
#define HITTESTLINE_MAX (1024)
// �����蔻��̐F
#define HITTEST_COLOR Color{255,0,0,255}
// �����蔻��̉~�̕�����
#define HITTEST_CIRCLE_DIVISION (8)

//****************************************
// �\���̒�`
//****************************************
// �`�悷�铖���蔻����\����
typedef struct
{
	HIT_TEST_FORM hitTestForm;		// �����蔻��̌`
	float fWidth;					// ��
	float fHeight;					// ����
	float fDepth;					// ���s��
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	bool aGhost[HITDIRECTION_MAX];	// ���蔲�����
}HitTestDraw;

// �����֘A�����̊Ǘ����\����
typedef struct
{
	// �`�悷�铖���蔻��̃��X�g
	DrawHitTest3DSet aDrawHitTestList[DRAWHITTEST_MAX];
	int nDrawHitTestSet;	// �`�悷�铖���蔻��̐ݒ萔
	bool bDrawHitTest;		// �����蔻��`��t���O
	int nHitTestLineNum;	// �����蔻��̐��̐�
}PhysicsControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �����蔻��`��֘A ***
// �����蔻��̐��̐ݒ菈��
void SetHitTestLine(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// �����蔻��̐��̒��_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHitTestLine = NULL;
static PhysicsControl g_physicsControl;	// �����蔻��̊Ǘ����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �����蔻��`��֘A *** |----------
//============================================================

//========================================
// SetHitTestLine�֐� - �����蔻��̐��̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetHitTestLine(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2) {
	// �����֘A�����̊Ǘ����̃|�C���^
	PhysicsControl *pPhy = &g_physicsControl;

	if (pPhy->nHitTestLineNum >= HITTESTLINE_MAX) 
	{
		return;	// �����蔻��̐��̐����ő吔�ɒB���Ă��鎞�A
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHitTestLine->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^��i�߂�
	pVtx += pPhy->nHitTestLineNum * 2;

	// �ʒu����
	pVtx[0].pos = pos1;
	pVtx[1].pos = pos2;

	// ���_���W���A�����b�N����
	g_pVtxBuffHitTestLine->Unlock();

	// �����蔻��̐��̐������Z
	pPhy->nHitTestLineNum++;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================

//========================================
// InitPhysics�֐� - �����֘A�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitPhysics(void) {
	// �����֘A�����̊Ǘ����̃p�����[�^�[������
	g_physicsControl = {};

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * HITTESTLINE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHitTestLine,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHitTestLine->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLine = 0; nCntLine < HITTESTLINE_MAX; nCntLine++, pVtx += 2)
	{
		// ���_�J���[��ݒ�
		pVtx[0].col = ColorToD3DCOLOR(HITTEST_COLOR);
		pVtx[1].col = ColorToD3DCOLOR(HITTEST_COLOR);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffHitTestLine->Unlock();
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// LoadHitTestSet�֐� - �����蔻��ݒ���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadHitTest3DSet(FILE *pFile, HitTest3DSet *pHitTestSet) {

	// �����蔻��̐ݒ����������
	if (pHitTestSet->pHitTest != NULL) {
		for (int nCntHitTest = 0; nCntHitTest < pHitTestSet->nHitTestNum; nCntHitTest++) {
			if (pHitTestSet->pHitTest[nCntHitTest].pHitTestParts != NULL) {
				delete[] pHitTestSet->pHitTest[nCntHitTest].pHitTestParts;
				pHitTestSet->pHitTest[nCntHitTest].pHitTestParts = NULL;
			}
		}
		delete[] pHitTestSet->pHitTest;
		pHitTestSet->pHitTest = NULL;
	}
	*pHitTestSet = {};

	// �����蔻��̐��擾
	fscanf_int(pFile, &pHitTestSet->nHitTestNum);

	// �����蔻�薈�̏��̃������m��
	GetMemory((void**)&pHitTestSet->pHitTest, sizeof(HitTest3D), pHitTestSet->nHitTestNum);

	int nCntHitTest = 0;	// �����蔻��̃J�E���g
	char aDataSearch[TXT_MAX] = "";	// �����p������

	// �����蔻��ݒ���̓ǂݍ��݂��J�n
	while (DataSearch(pFile, aDataSearch, "HITTESTSET_END")) {
		if (!strcmp(aDataSearch, "HITTEST")) {
			pHitTestSet->pHitTest[nCntHitTest] = {};
			HitTest3D *pHitTest = &pHitTestSet->pHitTest[nCntHitTest];	// �����蔻�薈�̏��̃|�C���^
			
			// ���i���擾
			fscanf_int(pFile, &pHitTest->nHitTestPartsNum);

			// ���i�̎�ޖ��̏��̃������m��
			GetMemory((void**)&pHitTest->pHitTestParts, sizeof(HitTest3DParts), pHitTest->nHitTestPartsNum);

			int nCntHitTestParts = 0;	// �����蔻��̕��i�̃J�E���g

			// �����蔻����̓ǂݍ��݂��J�n
			while (DataSearch(pFile, aDataSearch, "HITTEST_END")) {
				if (false) {}
				else if (!strcmp(aDataSearch, "HITTEST_PARTS")) {
					HitTest3DParts *pHitTestParts = &pHitTest->pHitTestParts[nCntHitTestParts];	// �|�C���^�̃R�s�[

					// �����蔻����̓ǂݍ��݂��J�n
					while (DataSearch(pFile, aDataSearch, "HITTEST_PARTS_END")) {
						if (false) {}
						else if (!strcmp(aDataSearch, "FORM"))
						{// �����蔻��̌`
							int nTemp;
							fscanf_int(pFile, &nTemp);
							pHitTestParts->hitTestForm = (HIT_TEST_FORM)nTemp;
						}
						else if (!strcmp(aDataSearch, "PARENT")) { fscanf_int(pFile, &pHitTestParts->nParent); }	// �e�ԍ�
						else if (!strcmp(aDataSearch, "WIDTH"))  { fscanf_float(pFile, &pHitTestParts->fWidth); }	// ��
						else if (!strcmp(aDataSearch, "HEIGHT")) { fscanf(pFile, "%f", &pHitTestParts->fHeight); }	// ����
						else if (!strcmp(aDataSearch, "DEPTH"))  { fscanf(pFile, "%f", &pHitTestParts->fDepth); }	// ���s��
						else if (!strcmp(aDataSearch, "RELATIVE_POS")) { fscanf_D3DXVECTOR3(pFile, &pHitTestParts->fixedRelativePos); }	// ���Έʒu
						else if (!strcmp(aDataSearch, "RELATIVE_ROT")) { fscanf_D3DXVECTOR3(pFile, &pHitTestParts->fixedRelativeRot); }	// ���Ό���
						else if (!strcmp(aDataSearch, "GHOST"))
						{// ���蔲���t���O
							for (int nCntDir = 0; nCntDir < HITDIRECTION_MAX; nCntDir++) {
								fscanf_bool(pFile, &pHitTestParts->aGhost[nCntDir]);
							}
						}
					}

					// �����蔻��̕��i�̃J�E���g�����Z
					nCntHitTestParts++;
					assert(nCntHitTestParts <= pHitTest->nHitTestPartsNum);
				}
			}

			// �����蔻��̃J�E���g�����Z
			nCntHitTest++;
			assert(nCntHitTest <= pHitTestSet->nHitTestNum);
		}
	}
}

//============================================================
//--------------------| *** �����蔻�� *** |------------------
//============================================================

//========================================
// DrawHitTest3DSignUp�֐� - �`�悷�铖���蔻��(3D)�o�^���� -
// Author:RIKU NISHIMURA
//========================================
void DrawHitTest3DSignUp(DrawHitTest3DSet drawHitTestSet) {
	// �����֘A�����̊Ǘ����̃|�C���^
	PhysicsControl *pPhy = &g_physicsControl;

	if (pPhy->nDrawHitTestSet >= DRAWHITTEST_MAX)
	{// �`�悷�铖���蔻��̐ݒ萔���ő吔�ɒB���Ă��鎞�A
		return;
	}

	// �����蔻��ɕK�v�ȏ���o�^
	pPhy->aDrawHitTestList[pPhy->nDrawHitTestSet] = drawHitTestSet;
	pPhy->nDrawHitTestSet++;
}

//========================================
// HitTestSignOut�֐� - �����蔻��o�^�������� -
// Author:RIKU NISHIMURA
//========================================
void DrawHitTestSignOut(void) {
	// �����֘A�����̊Ǘ����̃|�C���^
	PhysicsControl *pPhy = &g_physicsControl;

	// �`�悷�铖���蔻��̐ݒ萔��������
	pPhy->nDrawHitTestSet = 0;
}

//========================================
// CollisionCheck�֐� - �Փ˃`�F�b�N -
// Author:RIKU NISHIMURA
//========================================
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo pMyCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode) {
	for (int nCntMyParts = 0; nCntMyParts < pMyCollInfo.pHitTest->nHitTestPartsNum; nCntMyParts++)
	{
		for (int nCntPairParts = 0; nCntPairParts < pairCollInfo.pHitTest->nHitTestPartsNum; nCntPairParts++)
		{
			HIT_TEST_FORM	my					= pMyCollInfo.pHitTest->pHitTestParts[nCntMyParts].hitTestForm;		// �����̓����蔻��̎��
			HIT_TEST_FORM	pr					= pairCollInfo.pHitTest->pHitTestParts[nCntPairParts].hitTestForm;	// ����̓����蔻��̎��
			HitTest3DParts	myHitTestParts		= pMyCollInfo.pHitTest->pHitTestParts[nCntMyParts];					// �����̓����蔻��̕��i
			HitTest3DParts	pairHitTestParts	= pairCollInfo.pHitTest->pHitTestParts[nCntPairParts];				// ����̓����蔻��̕��i

			// ���[�h�ɉ���������
			switch (mode)
			{
			case COLLCHK_MODE_NORMAL:
				
				break;
			case COLLCHK_MODE_LOOKDOWN:
				
				break;
			}
		}
	}
}

//========================================
// CollisionOverwrite�֐� - �Փˏ��̏㏑�� -
// Author:RIKU NISHIMURA
//========================================
void CollisionOverwrite(Collision *pOverwriteColl, Collision collision) {
	pOverwriteColl->bHit	|= collision.bHit;	// ������t���O
	pOverwriteColl->bColl	|= collision.bColl;	// �Փ˃t���O
	pOverwriteColl->bOvl	|= collision.bOvl;	// �����t���O
}

//============================================================
//--------------------| *** �����蔻��`��֘A *** |----------
//============================================================

//========================================
// DrawHitTestSetUp�֐� - �����蔻��̕`�揀������ -
// Author:RIKU NISHIMURA
//========================================
void DrawHitTest3DSetUp(void) {
	if (!g_physicsControl.bDrawHitTest) {
		return;	// �����蔻��̕`��t���O���U�̎��A�������I������
	}

	// �����֘A�����̊Ǘ����̃|�C���^
	PhysicsControl *pPhy = &g_physicsControl;
	// �`�悷�铖���蔻����̃|�C���^
	DrawHitTest3DSet *pDrawHitTest = pPhy->aDrawHitTestList;

	for (int nCntHitTest = 0; nCntHitTest < pPhy->nDrawHitTestSet; nCntHitTest++, pDrawHitTest++) {
		// �����蔻��̕��i���̃|�C���^
		HitTest3DParts *pParts = &pDrawHitTest->hitTestParts;
		// ���i�̍ŏI�}�g���b�N�X
		D3DXMATRIX partsMtx = MultiplyMatrix(pDrawHitTest->mtx, ConvPosRotToMatrix(pParts->fixedRelativePos, pParts->fixedRelativeRot));

		switch (pParts->hitTestForm)
		{
		case HIT_TEST_FORM_SQUARE: {
			//   A1-----B1
			//  /      /|
			// C1-----D1B2
			// |      |/
			// C2-----D2
			enum { l_A1, l_B1, l_C1, l_D1, l_A2, l_B2, l_C2, l_D2, l_MAX };
			D3DXVECTOR3 aPos[l_MAX] = {
				D3DXVECTOR3(-1, 1, 1),
				D3DXVECTOR3(1, 1, 1),
				D3DXVECTOR3(-1, 1,-1),
				D3DXVECTOR3(1, 1,-1),
				D3DXVECTOR3(-1,-1, 1),
				D3DXVECTOR3(1,-1, 1),
				D3DXVECTOR3(-1,-1,-1),
				D3DXVECTOR3(1,-1,-1)
			};

			// ���i�̕�/����/���s���𔽉f
			for (int nCntVtx = 0; nCntVtx < l_MAX; nCntVtx++) {
				aPos[nCntVtx] *= 0.5f;
				aPos[nCntVtx].x *= pParts->fWidth;
				aPos[nCntVtx].y *= pParts->fHeight;
				aPos[nCntVtx].z *= pParts->fDepth;

				// �ʒu�𕔕i�}�g���b�N�X�ƒ��_�ʒu�}�g���b�N�X���|�����킹��
				aPos[nCntVtx] = ConvMatrixToPos(MultiplyMatrix(partsMtx, ConvPosRotToMatrix(aPos[nCntVtx], INITD3DXVECTOR3)));
			}

			// ��̕�
			SetHitTestLine(aPos[l_A1], aPos[l_B1]);
			SetHitTestLine(aPos[l_A1], aPos[l_C1]);
			SetHitTestLine(aPos[l_D1], aPos[l_C1]);
			SetHitTestLine(aPos[l_D1], aPos[l_B1]);
			// ���̕�
			SetHitTestLine(aPos[l_A2], aPos[l_B2]);
			SetHitTestLine(aPos[l_A2], aPos[l_C2]);
			SetHitTestLine(aPos[l_D2], aPos[l_C2]);
			SetHitTestLine(aPos[l_D2], aPos[l_B2]);
			// �c�̕�
			SetHitTestLine(aPos[l_A1], aPos[l_A2]);
			SetHitTestLine(aPos[l_B1], aPos[l_B2]);
			SetHitTestLine(aPos[l_C1], aPos[l_C2]);
			SetHitTestLine(aPos[l_D1], aPos[l_D2]);

			break;
		}// HIT_TEST_FORM_SQUARE
		case HIT_TEST_FORM_CYLINDER: {
			enum { l_SIRCLE_UPPER, l_SIRCLE_BOTTOM, l_SIRCLE_MAX };
			D3DXVECTOR3 aCirclePos[l_SIRCLE_MAX][HITTEST_CIRCLE_DIVISION];

			for (int nCntCircle = 0; nCntCircle < l_SIRCLE_MAX; nCntCircle++)
			{
				for (int nCntDiv = 0; nCntDiv < HITTEST_CIRCLE_DIVISION; nCntDiv++)
				{
					// �~�̃J�E���g�ɉ�����Y���W��ݒ�
					switch (nCntCircle)
					{
					case l_SIRCLE_UPPER:
						aCirclePos[nCntCircle][nCntDiv].y = 1;
						break;
					case l_SIRCLE_BOTTOM:
						aCirclePos[nCntCircle][nCntDiv].y = -1;
						break;
					}

					// �����J�E���g�ɉ�����XZ���W��ݒ�
					aCirclePos[nCntCircle][nCntDiv].x = sinf(nCntDiv * ((D3DX_PI * 2) / (float)HITTEST_CIRCLE_DIVISION));
					aCirclePos[nCntCircle][nCntDiv].z = cosf(nCntDiv * ((D3DX_PI * 2) / (float)HITTEST_CIRCLE_DIVISION));

					// ���i�̕�/����/���s���𔽉f
					aCirclePos[nCntCircle][nCntDiv] *= 0.5f;
					aCirclePos[nCntCircle][nCntDiv].x *= pParts->fWidth;
					aCirclePos[nCntCircle][nCntDiv].y *= pParts->fHeight;
					aCirclePos[nCntCircle][nCntDiv].z *= pParts->fDepth;

					// �ʒu�𕔕i�}�g���b�N�X�ƒ��_�ʒu�}�g���b�N�X���|�����킹��
					aCirclePos[nCntCircle][nCntDiv] = ConvMatrixToPos(MultiplyMatrix(partsMtx, ConvPosRotToMatrix(aCirclePos[nCntCircle][nCntDiv], INITD3DXVECTOR3)));
				}
			}

			for (int nCntDiv = 0; nCntDiv < HITTEST_CIRCLE_DIVISION; nCntDiv++)
			{
				// �c�̕�
				SetHitTestLine(
					aCirclePos[l_SIRCLE_UPPER][nCntDiv],
					aCirclePos[l_SIRCLE_BOTTOM][nCntDiv]);

				// ��̎��͕�
				SetHitTestLine(
					aCirclePos[l_SIRCLE_UPPER][nCntDiv],
					aCirclePos[l_SIRCLE_UPPER][(nCntDiv + 1) % HITTEST_CIRCLE_DIVISION]);

				// ���̎��͕�
				SetHitTestLine(
					aCirclePos[l_SIRCLE_BOTTOM][nCntDiv],
					aCirclePos[l_SIRCLE_BOTTOM][(nCntDiv + 1) % HITTEST_CIRCLE_DIVISION]);
			}

			break;
		}// HIT_TEST_FORM_CYLINDER
		}
	}

	// �`�悷�铖���蔻��̐ݒ萔��������
	pPhy->nDrawHitTestSet = 0;
}

//========================================
// DrawHitTest�֐� - �����蔻��̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawHitTest3D(void) {
	if (!g_physicsControl.bDrawHitTest) {
		return;	// �����蔻��̕`��t���O���U�̎��A�������I������
	}

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtx;
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHitTestLine, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// �����_�����O�X�e�[�g��ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �e�N�X�`���̐ݒ�
	SetTexture(pDevice, -1);

	for (int nCntLine = 0; nCntLine < g_physicsControl.nHitTestLineNum; nCntLine++)
	{
		// �|���S�����̕`��
		pDevice->DrawPrimitive(D3DPT_LINELIST, 2 * nCntLine, 1);
	}

	// �����_�����O�X�e�[�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//========================================
// InitHitTestLineNum�֐� - �����蔻��̐��̐��������� -
// Author:RIKU NISHIMURA
//========================================
void InitHitTest3DLineNum(void) {
	// �����蔻��̐��̐���������
	g_physicsControl.nHitTestLineNum = 0;
}

//========================================
// ShowHitTest�֐� - �����蔻��̕\���ݒ� -
// Author:RIKU NISHIMURA
//========================================
void ShowHitTest(bool bShow)  {
	g_physicsControl.bDrawHitTest = bShow;
}