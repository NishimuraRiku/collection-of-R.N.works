//========================================
// 
// �����֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _PHYSICS_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include <stdio.h>
// _R.N.Lib
#include "RNmain.h"
#include "polygon3D.h"

//****************************************
// �}�N����`
//****************************************
// �����蔻��̍ő吔
#define HIT_TEST_MAX (16)
// �����蔻��̕��i�̍ő吔
#define HIT_TEST_PARTS_MAX (4)

//****************************************
// �񋓌^�̒�`
//****************************************
//========== *** �����蔻�� ***
// �Փ˃`�F�b�N�̃��[�h
typedef enum 
{
	COLLCHK_MODE_NORMAL,	// �ʏ�
	COLLCHK_MODE_LOOKDOWN,	// �����낵
}COLLCHK_MODE;
// �����蔻��̌`
typedef enum
{
	HIT_TEST_FORM_CIRCLE,	// �~
	HIT_TEST_FORM_SQUARE,	// �l�p
	HIT_TEST_FORM_MAX,
}HIT_TEST_FORM;
//========== *** ���̑� ***
// �Փ˕���
typedef enum 
{
	HITDIRECTION_TOP,		// ��
	HITDIRECTION_BOTTOM,	// ��
	HITDIRECTION_LEFT,		// ��
	HITDIRECTION_RIGHT,		// �E
	HITDIRECTION_BACK,		// ��
	HITDIRECTION_FRONT,		// ��O
	HITDIRECTION_MAX,
}HITDIRECTION;
// ����
typedef enum
{
	DIRECTION_UP,			// ��
	DIRECTION_DOWN,			// ��
	DIRECTION_LEFT,			// ��
	DIRECTION_RIGHT,		// �E
	DIRECTION_BACK,			// ��
	DIRECTION_FRONT,		// ��O
	DIRECTION_LEFT_BACK,	// ����
	DIRECTION_LEFT_FRONT,	// ����O
	DIRECTION_RIGHT_BACK,	// �E��
	DIRECTION_RIGHT_FRONT,	// �E��O
	DIRECTION_MAX,
}DIRECTION;
// �x�N�g���̎��
typedef enum 
{
	VECTOR_X,		// X����
	VECTOR_Y,		// Y����
	VECTOR_Z,		// Z����
	VECTOR_NONE,	// ����
	VECTOR_MAX
}VECTOR;
// ���_
typedef enum 
{
	VERTEX_X0Y0,	// ����
	VERTEX_X1Y0,	// �E��
	VERTEX_X0Y1,	// ����
	VERTEX_X1Y1,	// �E��
	VERTEX_MAX,
}VERTEX;

//****************************************
// �\���̂̒�`
//****************************************
//========== *** �Փˏ�� ***
// �Փˏ��\����
typedef struct
{
	bool bHit;	// ������t���O
	bool bColl;	// �Փ˃t���O
	bool bOvl;	// �͈͓��t���O
}Collision;
// �����蔻��̕��i���\����
typedef struct
{
	HIT_TEST_FORM hitTestForm;		// �����蔻��̌`
	int nParent;					// �e�ԍ�
	float fWidth;					// ��
	float fHeight;					// ����
	float fDepth;					// ���s��
	D3DXVECTOR3 fixedRelativePos;	// ���Έʒu
	D3DXVECTOR3 fixedRelativeRot;	// ���Ό���
	bool aGhost[HITDIRECTION_MAX];	// ���蔲�����
}HitTestParts;
//========== *** �ݒ��� ***
// �����蔻��̏��\����
typedef struct 
{
	int nHitTestPartsNum;	// �����蔻��̕��i�̐�
	// �����蔻��̕��i�̏��
	HitTestParts aHitTestParts[HIT_TEST_PARTS_MAX];
}HitTest;
// �����蔻��̐ݒ���\����
typedef struct
{
	int nHitTestNum;				// �����蔻��̐�
	HitTest aHitTest[HIT_TEST_MAX];	// �����蔻��̏��
}HitTestSet;
//========== *** �Ǘ���� ***
// �����蔻��Ǘ��\����
typedef struct
{
	int	nHitTest;	// �����蔻��ԍ�
}HitTestInfo;
//========== *** ���n��� ***
// �Փ˔���ɕK�v�ȏ��\����
typedef struct
{
	D3DXVECTOR3 *pPos;			// ���݂̈ʒu�̃|�C���^
	D3DXVECTOR3 posOld;			// �ߋ��̈ʒu�̃|�C���^
	D3DXVECTOR3 *pMove;			// �ړ��ʂ̃|�C���^
	D3DXVECTOR3 *pRot;			// ���݂̌����̃|�C���^
	D3DXVECTOR3 rotOld;			// �ߋ��̌����̃|�C���^
	Parts3DInfo *pPartsInfo;	// ���i�Ǘ����
	Parts3DSet *pPartsSet;		// ���i�ݒ���
	HitTest *pHitTest;			// �����蔻��̏��
	COLLCHK_MODE mode;			// �Փ˃`�F�b�N�̃��[�h
}CollisionInfo;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �����֘A�̏���������
void InitPhysics(void);
//========== *** ���o�� ***
// �����蔻��ݒ���̓ǂݍ��ݏ���
void LoadHitTestSet(FILE *pFile, HitTestSet *pHitTestSet);
//========== *** �����蔻�� ***
// �����蔻��o�^����
void HitTestSignUp(CollisionInfo myCollInfo);
// �����蔻��o�^��������
void HitTestSignOut(void);
// �Փ˃`�F�b�N
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode);
// �Փˏ��̏㏑��
void CollisionOverwrite(Collision *pOvwCollision, Collision collision);
//========== *** �����蔻��`��֘A ***
// �����蔻��̕`�揀������
void DrawHitTestSetUp(void);
// �����蔻��̕`�揈��
void DrawHitTest(void);
// �����蔻��̐��̐���������
void InitHitTestLineNum(void);
// �����蔻��̕\���ݒ�
void ShowHitTest(bool bShow);
//========== *** ���̑����� ***
// 2�_�̋��������߂�
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
// 2�_�̋��������߂�(�����낵�)
float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
// 2�_�̊p�x�����߂�
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
// 2�_�̊p�x�����߂�(�����낵�)
float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
// ��_�̍��W�����߂�(�����낵�)
D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2);
// �p�x�̍������߂�
float AngleDifference(float fAngle, float fTargetAngle);
// �p�x�𐧌䂷��
void ControlAngle(float *pAngle);
// �p�x���w�肳�ꂽ�������̕����ɕϊ�����
float ConvertAngleToDirection(float fAngle, int nDiv);
// �ʒu�������낵��ɕϊ�����(Z��Y)
D3DXVECTOR3 ConvPosLookDown(D3DXVECTOR3 pos);
// �����������낵��ɕϊ�����(Z��Y)
D3DXVECTOR3 ConvRotLookDown(D3DXVECTOR3 rot);
// �����𐧌�
void RotControl(D3DXVECTOR3 *pRot);
//========== *** �}�g���b�N�X ***
// �ʒu�ƌ������}�g���b�N�X�ɕϊ����ĕԂ�
D3DXMATRIX ConvPosRotToMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
// �}�g���b�N�X���|�����킹��
D3DXMATRIX MultiplyMatrix(D3DXMATRIX mtxParent, D3DXMATRIX mtxChild);
// �}�g���b�N�X�̈ʒu�����𕪉����ĕԂ�
D3DXVECTOR3 ConvMatrixToPos(D3DXMATRIX mtx);
// �}�g���b�N�X�̌��������𕪉����ĕԂ�
D3DXVECTOR3 ConvMatrixToRot(D3DXMATRIX mtx);

#endif