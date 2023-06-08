//========================================
// 
// ���i(3D)�̃N���X�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** parts3D_class.cpp ***
//========================================
#include "../../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CParts3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CParts3D::CParts3D(void) {
	m_pos				= INITD3DXVECTOR3;	// �{�̈ʒu
	m_rot				= INITD3DXVECTOR3;	// �{�̌���
	m_bMotionStop		= false;			// ��~�t���O
	m_bDrawCollision	= false;			// �����蔻��`��t���O
	m_nModelSetUp		= 0;				// ���f���̃Z�b�g�A�b�v(3D)�ԍ���ݒ�
	m_nMotion			= 0;				// ���[�V�����ԍ�
	m_nMotionOld		= 0;				// ���[�V�����ԍ�(�ߋ�)
	m_nMotionCounter	= 0;				// ���[�V�����J�E���^�[
	m_col				= CLEARCOLOR;		// �F
	m_pParts			= NULL;				// ���i���̏��̃|�C���^
}

//========================================
// �f�X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CParts3D::~CParts3D(void) {

}

//========================================
// ModelSetParts3D�֐� - ���i(3D)�̃��f���ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::ModelSetParts3D(Parts3DSet *pPartsSet) {
	// ���i��0�ȉ��̎��A�������I������
	if (pPartsSet->nPartsNum <= 0) {
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();	// �f�o�C�X���擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxSelf;				// �{�̂̃}�g���b�N�X
	D3DXMATRIX *pMtxParts = NULL;	// ���i���̃}�g���b�N�X�̃|�C���^

	// ���i���̃}�g���b�N�X�̃������𕔕i�����m��
	GetMemory((void**)&pMtxParts, sizeof(D3DXMATRIX), pPartsSet->nPartsNum);

	// �{�̂̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxSelf);

	// �{�̂̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxRot);

	// �{�̂̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxSelf);

	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++) {
		Parts3D *pParts = &m_pParts[nCntParts];	// ���i�̏��̃|�C���^
		Parts3DType *pPartsType = &pPartsSet->pPartsType[nCntParts];	// ���i�̎�ޖ��̏��̃|�C���^
		D3DXVECTOR3 posResult;	// �ʒu(����)
		D3DXVECTOR3 rotResult;	// ����(����)

		{//========== *** �ʒu(����)���X�V ***
			if (pParts->bPosAnim)
			{// �ʒu�A�j���[�V�����t���O���^�̎��A�J�E���^�[�̐i�s���ɉ����Ĉʒu���X�V
				float fRate = Easing(pParts->easePos, pParts->nCounterPosAnim, pParts->nPosAnimTime);
				pParts->pos = (pParts->posOld * (1.0f - fRate)) + (pParts->targetPos * fRate);
			}

			// ���ʈʒu�����߂�
			posResult = pParts->pos + pPartsType->fixedRelativePos;

			// �e�ԍ���-1�̎��A���ʈʒu�Ɋg��{������Z
			if (pPartsType->nParent == -1) {
				posResult *= pPartsSet->fScale;
			}
		}

		{//========== *** ����(����)���X�V ***
			if (pParts->bStep) 
			{// ���ރt���O���^�̎��A
				int nCounter = pParts->nCounterRotAnim;
				if (nCounter > pParts->nRotAnimTime) {
					nCounter = pParts->nRotAnimTime;
				}
				float fRate = Easing(EASE_LINEAR, nCounter, pParts->nRotAnimTime);
				pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (INITD3DXVECTOR3 * fRate);
			}
			else if (pParts->bRotAnim)
			{// �����A�j���[�V�����t���O���^�̎��A�J�E���^�[�̐i�s���ɉ����Č������X�V
				float fRate = Easing(pParts->easeRot, pParts->nCounterRotAnim, pParts->nRotAnimTime);
				pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (pParts->targetRot * fRate);
			}

			// ���ʈʒu�����߂�
			rotResult = pParts->rot + pPartsType->fixedRelativeRot;
		}

		{// ���i�̃��[���h�}�g���b�N�X�̎Z�o
			// ���i�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMtxParts[nCntParts]);

			if (pPartsType->nParent == -1)
			{// �e�ԍ���-1�̎��A���f���̃T�C�Y��ύX
				D3DXMatrixScaling(&pMtxParts[nCntParts], pPartsSet->fScale, pPartsSet->fScale, pPartsSet->fScale);
			}

			// ���i�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rotResult.y, rotResult.x, rotResult.z);
			D3DXMatrixMultiply(&pMtxParts[nCntParts], &pMtxParts[nCntParts], &mtxRot);

			// ���i�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, posResult.x, posResult.y, posResult.z);
			D3DXMatrixMultiply(&pMtxParts[nCntParts], &pMtxParts[nCntParts], &mtxTrans);

			// �e�̕��i�̃}�g���b�N�X
			D3DXMATRIX mtxParent;

			// ���i�̐e�ԍ���-1(�e����)�łȂ����A�e�}�g���b�N�X��ݒ�B�e�����ł���Ύ��g�̃}�g���b�N�X��ݒ�
			if (pPartsType->nParent != -1) { mtxParent = pMtxParts[pPartsType->nParent]; }
			else                           { mtxParent = mtxSelf; }

			// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(
				&pMtxParts[nCntParts],
				&pMtxParts[nCntParts],
				&mtxParent);

			if (pParts->bStep) {
				D3DXVECTOR3 pos = ConvMatrixToPos(pMtxParts[nCntParts]);
				D3DXVECTOR3 rot = posResult;
				D3DXMATRIX mtxOld = pMtxParts[nCntParts];
				pMtxParts[nCntParts] = ConvPosRotToMatrix(pos, rot);
				pMtxParts[nCntParts]._11 = mtxOld._11;
				pMtxParts[nCntParts]._22 = mtxOld._22;
				pMtxParts[nCntParts]._33 = mtxOld._33;
			}
		}

		// ���f���̐ݒ菈��
		SetModel(pPartsType->nModelIdx, pMtxParts[nCntParts], m_col, pPartsType->bLighting);

		// �}�g���b�N�X�𕔕i�ɕۑ�
		pParts->mtx = pMtxParts[nCntParts];
	}

	// ���i���̃}�g���b�N�X�̃��������
	ReleaseMemory((void**)&pMtxParts);
}

//========================================
// HitTestSet�֐� - �����蔻��̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::HitTestSet(Parts3DSet *pPartsSet, HitTest3DSet *pHitTestSet) {
	if (pHitTestSet->nHitTestNum <= 0) {
		return;	// �����蔻��̐���0�ȉ��̎��A�������I������
	}

	for (int nCntHitTestParts = 0; nCntHitTestParts < pHitTestSet->pHitTest[0].nHitTestPartsNum; nCntHitTestParts++) {
		D3DXMATRIX setMtx = INITD3DXMATRIX;	// �ݒ�}�g���b�N�X

		if (pHitTestSet->pHitTest[0].pHitTestParts[nCntHitTestParts].nParent == DATANONE)
		{// �e�ԍ����w��Ȃ��̎��A
			// �{�̃}�g���b�N�X��ݒ�}�g���b�N�X�Ƃ���
			setMtx = ConvPosRotToMatrix(m_pos, m_rot);
		}
		else {
			// �e�ԍ��ƈ�v�����ԍ��̕��i�̃}�g���b�N�X��ݒ�}�g���b�N�X�Ƃ���
			for (int nCntParts3D = 0; nCntParts3D < pPartsSet->nPartsNum; nCntParts3D++) {
				if (pHitTestSet->pHitTest[0].pHitTestParts[nCntHitTestParts].nParent == pPartsSet->pPartsType[nCntParts3D].nIdx) {
					setMtx = m_pParts[nCntParts3D].mtx;
				}
			}
		}

		// �`�悷�铖���蔻��(3D)���\����
		DrawHitTest3DSet drawHitTestSet;
		/* �}�g���b�N�X */drawHitTestSet.mtx = setMtx;
		/* ���i���     */drawHitTestSet.hitTestParts = pHitTestSet->pHitTest[0].pHitTestParts[nCntHitTestParts];
		// �`�悷�铖���蔻��(3D)�o�^����
		DrawHitTest3DSignUp(drawHitTestSet);
	}
}

//========================================
// InitParts3DInfo�֐� - ���i(3D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::Init(int nSetUpNum) {
	// ���f���̃Z�b�g�A�b�v(3D)���
	ModelSetUp3D setUp = GetModelSetUpInfo(nSetUpNum);

	// ������
	m_nModelSetUp		= nSetUpNum;		// ���f���̃Z�b�g�A�b�v(3D)�ԍ���ݒ�
	m_pos				= INITD3DXVECTOR3;	// �{�̈ʒu
	m_rot				= INITD3DXVECTOR3;	// �{�̌���
	m_bMotionStop		= false;			// ��~�t���O
	m_bDrawCollision	= false;			// �����蔻��`��t���O
	m_nMotion			= DATANONE;			// ���[�V�����ԍ�
	m_nMotionOld		= DATANONE;			// ���[�V�����ԍ�(�ߋ�)
	m_nMotionCounter	= 0;				// ���[�V�����J�E���^�[
	m_col				= INITCOLOR;		// �F

	if (setUp.partsSet.nPartsNum > 0) 
	{// ���i����0�𒴂��Ă������A
		// ���i���̃|�C���^�̃������𕔕i�����m�ۂ���
		GetMemory((void**)&m_pParts, sizeof(Parts3D), setUp.partsSet.nPartsNum);

		for (int nCntParts = 0; nCntParts < setUp.partsSet.nPartsNum; nCntParts++) {
			Parts3DType *pPartsType = &setUp.partsSet.pPartsType[nCntParts];	// ���i�̎�ޖ��̏��̃|�C���^

			m_pParts[nCntParts] = {};	// ���i�̏���������

			// �}�g���b�N�X�ɑ��Έʒu/������K�p
			m_pParts[nCntParts].mtx = ConvPosRotToMatrix(pPartsType->fixedRelativePos, pPartsType->fixedRelativeRot);
		}
	}
	else 
	{// ���i����0�𒴂��Ă��Ȃ��������A
		if (m_pParts != NULL) {
			delete[] m_pParts;
			m_pParts = NULL;
		}
	}
}

//========================================
// UninitParts3DInfo�֐� - ���i(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::Uninit(void) {
	ReleaseMemory((void**)&m_pParts);	// ���i���̃��������
}

//========================================
// UpdateParts3DInfo�֐� - ���i(3D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::Update(void) {
	// ���f���̃Z�b�g�A�b�v(3D)���
	ModelSetUp3D setUp = GetModelSetUpInfo(m_nModelSetUp);

	if (m_nModelSetUp != DATANONE)
	{// ���f���̃Z�b�g�A�b�v(3D)�ԍ���-1 or �̎��A
		if (!m_bMotionStop)
		{// ��~���Ă��Ȃ����A
			// ���[�V�����ݒ�
			SetMotion3D();

			// ���[�V�����̍X�V����
			UpdateMotion3D(m_nMotion, setUp.partsSet);
		}

		// ���f���̐ݒ菈��
		ModelSetParts3D(&setUp.partsSet);

		if (m_bDrawCollision) 
		{// �����蔻��`��t���O���^�̎��A
			HitTestSet(&setUp.partsSet, &setUp.hitTestSet);	// �����蔻��̐ݒ菈��
		}
	}
}

//========================================
// SetMotion3D�֐� - ���[�V����(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::SetMotion3D(void) {
	if (m_nMotion == m_nMotionOld)
	{// ���ɂ��̃��[�V�����ԍ��ɐݒ肳��Ă��鎞�A
		return;	// �������I������
	}

	m_nMotionOld = m_nMotion;	// �ߋ��̃��[�V�����ԍ��Ƃ��ĕۑ�
	m_nMotionCounter = 0;		// ���[�V�����J�E���^�[��������
}

//========================================
// UpdateMotion3D�֐� - ���[�V����(3D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
bool CParts3D::UpdateMotion3D(int nMotion, Parts3DSet partsSet) {
	if (nMotion == DATANONE) {
		return false;
	}

	Motion3D motion = GetMotion3D()[nMotion];	// ���[�V�������

	if (motion.nLoopTime == DATANONE)
	{// ���[�V�����̃��[�v���Ԃ������̎��A
		// �������I������
		return false;
	}

	// ���[�V�����̃A�j���[�V����(3D)�ǂݍ��ݏ���
	LoadMotionAnim3D(motion, partsSet);

	// ���[�V�����̃A�j���[�V��������
	MotionAnim3D(partsSet);

	// ���[�V���������[�v�������t���O
	bool bMotionLoop = false;

	if (++m_nMotionCounter > motion.nLoopTime)
	{// ���[�V�����J�E���^�[�����Z�������ʃ��[�v���ԂɒB�������A
		bMotionLoop = true;	// ���[�V���������[�v�������t���O��^�ɂ���

		if (motion.bLoop)
		{// ���[�v���邩�t���O���^�̎��A
			// ���[�V�����J�E���^�[�����[�v����
			IntLoopControl(&m_nMotionCounter, motion.nLoopTime, 0);
		}
		else
		{// ���[�v���邩�t���O���U�̎��A
			// ���[�V�����J�E���^�[�𐧌�
			IntControl(&m_nMotionCounter, motion.nLoopTime, 0);
		}
	}

	// ���[�V���������[�v�������t���O��Ԃ�
	return bMotionLoop;
}


//========================================
// LoadMotionAnim3D�֐� - ���[�V����(3D)�̃A�j���[�V�����ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::LoadMotionAnim3D(Motion3D motion, Parts3DSet partsSet) {
	// ���i���̃A�j���[�V�����̓ǂݍ���
	for (int nCntPartsAnim = 0; nCntPartsAnim < motion.nPartsMotionNum; nCntPartsAnim++) {
		// ���i���̃��[�V����(3D)�̏��
		PartsMotion3D partsMotion3D = motion.pPartsMotion[nCntPartsAnim];

		for (int nCntCmd = 0; nCntCmd < partsMotion3D.nCommandNum; nCntCmd++) {
			Motion3DCommand cmd = partsMotion3D.pCommand[nCntCmd];	// �R�}���h���
			Parts3D *pParts = m_pParts;	// ���i(3D)�̏��

			{// ��v�������i�ԍ��܂Ń|�C���^��i�߂�
				bool bFind = false;	// ��v���i�m�F�t���O
				for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++, pParts++)
				{
					if (partsSet.pPartsType[nCntParts].nIdx == nCntPartsAnim)
					{// ���i���̃A�j���J�E���g�ƕ��i�ԍ�����v�������A
						bFind = true;
						break;
					}
				}

				if ((m_nMotionCounter != cmd.nTime) || (!bFind))
				{// �J�E���^�[�����s���Ԃƈ�v���Ă��Ȃ� or ��v���i���m�F�A
					continue;
				}
			}

			// �R�}���h���x���ɉ���������
			switch (cmd.command) {
			case MOTION3D_COMMAND_LABEL_MOVE: {
				// �ʒu
				D3DXVECTOR3 pos = D3DXVECTOR3(cmd.pData[0], cmd.pData[1], cmd.pData[2]);
				// �ړ��ɂ����鎞��
				int nMoveTime = (int)cmd.pData[3];
				// ���
				EASE ease = (EASE)((int)cmd.pData[4]);

				if (nMoveTime == 0)
				{// �ړ��ɂ����鎞�Ԃ�0�̎��A
					pParts->pos = pos;	// �ʒu�𒼐ڑ��
				}
				else
				{// �ړ��ɂ����鎞�Ԃ�0�łȂ����A
					// �ʒu��Ԃ�ݒ�
					pParts->easePos = ease;
					// ���̈ʒu��ݒ�
					pParts->posOld = pParts->pos;
					// �ڕW�ʒu����
					pParts->targetPos = pos;
					// �ʒu�A�j���[�V�����ɂ����鎞�Ԃ���
					pParts->nPosAnimTime = nMoveTime;
					// �ʒu�A�j���[�V�����J�E���^�[��������
					pParts->nCounterPosAnim = 0;
					// �ʒu�A�j���[�V�����t���O��^�ɂ���
					pParts->bPosAnim = true;
				}
			}
			break;
			case MOTION3D_COMMAND_LABEL_SPIN: {
				// ����
				D3DXVECTOR3 rot = D3DXVECTOR3(cmd.pData[0], cmd.pData[1], cmd.pData[2]);
				// ��]�ɂ����鎞��
				int nSpinTime = (int)cmd.pData[3];
				// ���
				EASE ease = (EASE)((int)cmd.pData[4]);

				if (nSpinTime == 0)
				{// ��]�ɂ����鎞�Ԃ�0�̎��A
					pParts->rot = rot;	// �����𒼐ڑ��
				}
				else
				{// ��]�ɂ����鎞�Ԃ�0�łȂ����A
					pParts->easeRot         = ease;			// ������Ԃ�ݒ�
					pParts->rotOld          = pParts->rot;	// ���̌�����ݒ�
					pParts->targetRot       = rot;			// �ڕW��������
					pParts->nRotAnimTime    = nSpinTime;	// �����A�j���[�V�����ɂ����鎞�Ԃ���
					pParts->nCounterRotAnim = 0;			// �����A�j���[�V�����J�E���^�[��������
					pParts->bRotAnim        = true;			// �����A�j���[�V�����t���O��^�ɂ���
				}
			}
			break;
			case MOTION3D_COMMAND_LABEL_STEP: {
				pParts->rotOld          = ConvMatrixToRot(pParts->mtx);	// ���̌�����ݒ�
				pParts->nRotAnimTime    = (int)cmd.pData[0];	// �����A�j���[�V�����ɂ����鎞�Ԃ���
				pParts->nCounterRotAnim = 0;					// �����A�j���[�V�����J�E���^�[��������
				pParts->bRotAnim        = false;				// �����A�j���[�V�����t���O��^�ɂ���
				pParts->bStep           = true;					// ���ރt���O�^
				pParts->nStepTime       = (int)cmd.pData[1];	// ���ގ���
			}
			break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//========================================
// MotionAnim3D�֐� - ���[�V����(3D)�̃A�j���[�V�������� -
// Author:RIKU NISHIMURA
//========================================
void CParts3D::MotionAnim3D(Parts3DSet partsSet) {
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++) {
		// ���i�̏��̃|�C���^
		Parts3D *pParts = &m_pParts[nCntParts];

		if (pParts->bPosAnim)
		{// �ʒu�A�j���[�V�����t���O���^�̎��A
			if (++pParts->nCounterPosAnim >= pParts->nPosAnimTime)
			{// �ʒu�A�j���[�V�����J�E���^�[�����Z�������ʈʒu�A�j���[�V�����ɂ����鎞�Ԃ����������A
				pParts->bPosAnim = false;	// �ʒu�A�j���[�V�����t���O���U�ɂ���
			}
		}

		if (pParts->bStep) 
		{// ���ރt���O���^�̎��A
			if (++pParts->nCounterRotAnim >= pParts->nStepTime)
			{// �����A�j���[�V�����J�E���^�[�����Z�������ʓ��ގ��Ԃ����������A
				pParts->bStep = false;	// ���ރt���O���U�ɂ���
			}
		}
		else if (pParts->bRotAnim)
		{// �p�x�A�j���[�V�����t���O���^�̎��A
			if (++pParts->nCounterRotAnim >= pParts->nRotAnimTime)
			{// �����A�j���[�V�����J�E���^�[�����Z�������ʌ����A�j���[�V�����ɂ����鎞�Ԃ����������A
				pParts->bRotAnim = false;	// �����A�j���[�V�����t���O���U�ɂ���
			}
		}
	}
}