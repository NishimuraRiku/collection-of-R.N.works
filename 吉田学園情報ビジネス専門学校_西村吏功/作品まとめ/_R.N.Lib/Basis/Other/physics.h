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

//****************************************
// �񋓌^��`
//****************************************
//========== *** �����蔻�� ***
// �Փ˃`�F�b�N�̃��[�h
typedef enum {
	COLLCHK_MODE_NORMAL,	// �ʏ�
	COLLCHK_MODE_LOOKDOWN,	// �����낵
}COLLCHK_MODE;

// �����蔻��̌`
typedef enum {
	HIT_TEST_FORM_SPHERE,	// ��
	HIT_TEST_FORM_SQUARE,	// �l�p
	HIT_TEST_FORM_CYLINDER,	// �~��
	HIT_TEST_FORM_MAX,
}HIT_TEST_FORM;

//========== *** ���̑� ***
// �Փ˕���
typedef enum {
	HITDIRECTION_TOP,		// ��
	HITDIRECTION_BOTTOM,	// ��
	HITDIRECTION_LEFT,		// ��
	HITDIRECTION_RIGHT,		// �E
	HITDIRECTION_BACK,		// ��
	HITDIRECTION_FRONT,		// ��O
	HITDIRECTION_MAX,
}HITDIRECTION;

// ����
typedef enum {
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
typedef enum {
	VECTOR_X,		// X����
	VECTOR_Y,		// Y����
	VECTOR_Z,		// Z����
	VECTOR_NONE,	// ����
	VECTOR_MAX
}VECTOR;

// ���_
typedef enum {
	VERTEX_X0Y0,	// ����
	VERTEX_X1Y0,	// �E��
	VERTEX_X0Y1,	// ����
	VERTEX_X1Y1,	// �E��
	VERTEX_MAX,
}VERTEX;

//****************************************
// �\���̒�`
//****************************************
//========== *** �Փˏ�� ***
// �Փˏ��\����
typedef struct {
	bool bHit = false;	// ������t���O
	bool bColl = false;	// �Փ˃t���O
	bool bOvl = false;	// �͈͓��t���O
}Collision;

// �����蔻��(3D)�̕��i���\����
typedef struct {
	HIT_TEST_FORM hitTestForm = HIT_TEST_FORM_SPHERE;	// �����蔻��̌`
	int nParent = -1;		// �e�ԍ�
	float fWidth = 8.0f;	// ��
	float fHeight = 8.0f;	// ����
	float fDepth = 8.0f;	// ���s��
	D3DXVECTOR3 fixedRelativePos = INITD3DXVECTOR3;	// ���Έʒu
	D3DXVECTOR3 fixedRelativeRot = INITD3DXVECTOR3;	// ���Ό���
	bool aGhost[HITDIRECTION_MAX] = {};	// ���蔲�����
}HitTest3DParts;

//========== *** �ݒ��� ***
// �����蔻��(3D)�̏��\����
typedef struct {
	int nHitTestPartsNum = 0;	// �����蔻��(3D)�̕��i�̐�
	// �����蔻��(3D)�̕��i�̏��
	HitTest3DParts *pHitTestParts = NULL;
}HitTest3D;

// �����蔻��(3D)�̐ݒ���\����
typedef struct {
	int nHitTestNum = 0;	// �����蔻��̐�
	HitTest3D *pHitTest = NULL;	// �����蔻��̏��
}HitTest3DSet;

//========== *** �Ǘ���� ***
// �����蔻��(3D)�Ǘ��\����
typedef struct {
	int	nHitTest = 0;	// �����蔻��ԍ�
}HitTest3DInfo;

//========== *** ���n��� ***
// �Փ˔���ɕK�v�ȏ��\����
typedef struct {
	D3DXVECTOR3 *pPos	= NULL;					// ���݂̈ʒu�̃|�C���^
	D3DXVECTOR3 posOld	= INITD3DXVECTOR3;		// �ߋ��̈ʒu�̃|�C���^
	D3DXVECTOR3 *pMove	= NULL;					// �ړ��ʂ̃|�C���^
	D3DXVECTOR3 *pRot	= NULL;					// ���݂̌����̃|�C���^
	D3DXVECTOR3 rotOld	= INITD3DXVECTOR3;		// �ߋ��̌����̃|�C���^
	HitTest3D *pHitTest	= NULL;					// �����蔻��̏��
	COLLCHK_MODE mode	= COLLCHK_MODE_NORMAL;	// �Փ˃`�F�b�N�̃��[�h
#if /* �ݒ�t�H�[�}�b�g */0
	// �Փ˔���ɕK�v�ȏ��
	CollisionInfo myCollInfo;
	/* ���݂̈ʒu�̃|�C���^ */myCollInfo.pPos = ;
	/* �ߋ��̈ʒu�̃|�C���^ */myCollInfo.posOld = ;
	/* �ړ��ʂ̃|�C���^     */myCollInfo.pMove = ;
	/* ���݂̌����̃|�C���^ */myCollInfo.pRot = ;
	/* �ߋ��̌����̃|�C���^ */myCollInfo.rotOld = ;
	/* �����蔻��̏��     */myCollInfo.pHitTest = ;
	/* �Փ˃`�F�b�N�̃��[�h */myCollInfo.mode = ;
#endif
}CollisionInfo;

// �`�悷�铖���蔻��(3D)���\����
typedef struct {
	D3DXMATRIX mtx;	// �}�g���b�N�X
	HitTest3DParts hitTestParts;	// �����蔻��̕��i���
#if /* �ݒ�t�H�[�}�b�g */0
	// �`�悷�铖���蔻��(3D)���\����
	DrawHitTest3DSet drawHitTestSet;
	/* �}�g���b�N�X */drawHitTestSet.mtx = ;
	/* ���i���     */drawHitTestSet.hitTestParts = ;
	// �`�悷�铖���蔻��(3D)�o�^����
	DrawHitTest3DSignUp(drawHitTestSet);
#endif
}DrawHitTest3DSet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �����֘A�̏���������
void InitPhysics(void);
//========== *** ���o�� ***
// �`�悷�铖���蔻��(3D)�ݒ���̓ǂݍ��ݏ���
void LoadHitTest3DSet(FILE *pFile, HitTest3DSet *pHitTest3DSet);
//========== *** �����蔻�� ***
// �`�悷�铖���蔻��(3D)�o�^����
void DrawHitTest3DSignUp(DrawHitTest3DSet drawHitTestSet);
// �����蔻��o�^��������
void DrawHitTestSignOut(void);
// �Փ˃`�F�b�N
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode);
// �Փˏ��̏㏑��
void CollisionOverwrite(Collision *pOvwCollision, Collision collision);
//========== *** �����蔻��`��֘A ***
// �����蔻��(3D)�̕`�揀������
void DrawHitTest3DSetUp(void);
// �����蔻��(3D)�̕`�揈��
void DrawHitTest3D(void);
// �����蔻��(3D)�̐��̐���������
void InitHitTest3DLineNum(void);
// �����蔻��̕\���ݒ�
void ShowHitTest(bool bShow);

#endif