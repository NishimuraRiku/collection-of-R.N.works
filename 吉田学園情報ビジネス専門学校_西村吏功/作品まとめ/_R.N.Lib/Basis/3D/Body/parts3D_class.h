//========================================
// 
// ���i(3D)�̃N���X�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** parts3D_class.h ***
//========================================
#ifndef _PARTS3D_CLASS_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _PARTS3D_CLASS_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "parts3D.h"
#include "motion3D.h"
#include "../../Other/physics.h"
#include "../../Calculation/matrix.h"
#include "../../Calculation/easing.h"

//****************************************
// �N���X��`
//****************************************
// ���i(3D)�N���X
class CParts3D {
public:
	//========== *** �\���̒�` ***
	// ���i(3D)�\����
	typedef struct {
		// �ʒu�֘A
		D3DXVECTOR3 pos = INITD3DXVECTOR3;	// �ʒu
		D3DXVECTOR3 rot = INITD3DXVECTOR3;	// ����
		D3DXMATRIX  mtx = INITD3DXMATRIX;	// �}�g���b�N�X(�ŏI�ʒu)

		//========== *** ���[�V�����֘A ***
		// �ړ��A�j���[�V����
		EASE easePos          = EASE_LINEAR;		// �ʒu���
		bool bPosAnim         = false;				// �ʒu�A�j���[�V�����t���O
		int nCounterPosAnim   = 0;					// �ʒu�A�j���[�V�����J�E���^�[
		int nPosAnimTime      = 0;					// �ʒu�A�j���[�V�����ɂ����鎞��
		D3DXVECTOR3 posOld    = INITD3DXVECTOR3;	// ���̈ʒu
		D3DXVECTOR3 targetPos = INITD3DXVECTOR3;	// �ڕW�ʒu
		// �����A�j���[�V����
		EASE easeRot          = EASE_LINEAR;		// �������
		bool bRotAnim         = false;				// �����A�j���[�V�����t���O
		int nCounterRotAnim   = 0;					// �����A�j���[�V�����J�E���^�[
		int nRotAnimTime      = 0;					// �����A�j���[�V�����ɂ����鎞��
		D3DXVECTOR3 rotOld    = INITD3DXVECTOR3;	// ���̊p�x
		D3DXVECTOR3 targetRot = INITD3DXVECTOR3;	// �ڕW�p�x
		// ����
		bool bStep            = false;				// ���ރt���O(�����A�j���[�V�����ϐ��g�p)
		int nStepTime         = 0;					// ���ގ���
	}Parts3D;

	//========== *** �֐��錾 ***
	/* �R���X�g���N�^         */CParts3D();
	/* �f�X�g���N�^           */~CParts3D();
	/* ����������             */void Init(int nModelSetUp);
	/* �I������               */void Uninit(void);
	/* �X�V����               */void Update(void);
	/*========================*/
	/* ���[�V�����ݒ菈��     */void SetMotion(int nIdx)       { m_nMotion = nIdx; }
	/* ���[�V������~�ݒ�     */void SetMotionStop(bool bStop) { m_bMotionStop = bStop; }
	/* ���[�V������~�ݒ�     */void DrawCollision(bool bDraw) { m_bDrawCollision = bDraw; }
	/*========================*/
	/* ���i��񏉊���         */void InitParts3D(int nParts)   { m_pParts[nParts] = {}; }
	/* ���i�����擾         */Parts3D GetParts3D(int nParts) { return m_pParts[nParts]; }
	/* ���i���擾           */Parts3D **GetParts3Dpp(void)   { return &m_pParts; }
	/* �����[�V�������擾   */Motion3D GetMotionInfo(void)   { return GetMotion3D()[m_nMotion]; }
	/* ���[�V�����J�E���g�擾 */int GetMotionCounter(void)     { return m_nMotionCounter; }
	/*========================*/

private:
	//========== *** �֐��錾 ***
	/* ���i(3D)�̃��f���ݒ菈��   */void ModelSetParts3D(Parts3DSet *pPartsSet);
	/* �����蔻��̐ݒ菈��       */void HitTestSet(Parts3DSet *pPartsSet, HitTest3DSet *pHitTestSet);
	/* ���[�V�����̐ݒ菈��       */void SetMotion3D(void);
	/* ���[�V�����̍X�V����       */bool UpdateMotion3D(int nMotion, Parts3DSet partsSet);
	/* ���[�V�����̃A�j���Ǎ����� */void LoadMotionAnim3D(Motion3D motion, Parts3DSet partsSet);
	/* ���[�V�����̃A�j������     */void MotionAnim3D(Parts3DSet partsSet);

	//========== *** �ϐ��錾 ***
	D3DXVECTOR3 m_pos;		// �ʒu(�{��)
	D3DXVECTOR3 m_rot;		// ����(�{��)
	bool m_bMotionStop;		// ���[�V������~�t���O
	bool m_bDrawCollision;	// �����蔻��`��t���O
	int m_nModelSetUp;		// ���f���̃Z�b�g�A�b�v(3D)�ԍ�
	int m_nMotion;			// ���[�V�����ԍ�
	int m_nMotionOld;		// ���[�V�����ԍ�(�ߋ�)
	int m_nMotionCounter;	// ���[�V�����J�E���^�[
	Color m_col;			// �F
	Parts3D *m_pParts;		// ���i���̏��̃|�C���^
};

#endif