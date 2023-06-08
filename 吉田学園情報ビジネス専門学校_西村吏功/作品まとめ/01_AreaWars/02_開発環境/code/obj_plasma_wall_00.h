//========================================
//  *** obj_plasma_wall_00.h ***
//========================================
#ifndef _OBJ_PLASMA_WALL_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_PLASMA_WALL_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
#define OBJ_PLASMA_WALL_00_WIDTH	(PIXEL * 16)	// �v���Y�}�E�H�[��[00] �̕�

//****************************************
// �񋓌^�̒�`
//****************************************

// �v���Y�}�E�H�[��[00] �̔ԍ�
typedef enum
{
	OBJ_PLASMA_WALL_00_TOP = 0,
	OBJ_PLASMA_WALL_00_BOTTOM,
	OBJ_PLASMA_WALL_00_LEFT,
	OBJ_PLASMA_WALL_00_RIGHT,
	OBJ_PLASMA_WALL_00_MAX
}OBJ_PLASMA_WALL_00;

//****************************************
// �\���̂̒�`
//****************************************

// �v���Y�}�E�H�[��[00]�\���̂̒�`
typedef struct
{
						//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
	float fHeight;		// ����
						//========== *** �e�N�X�`���֘A ***
	int nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
	int nPattern;		// �p�^�[��No.
	D3DXVECTOR3 tex;	// �e�N�X�`�����W�̊J�n�ʒu(UV�l)
}Obj_plasma_wall_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitObj_plasma_wall_00					(void);	// �v���Y�}�E�H�[��[00] �̏���������
void UninitObj_plasma_wall_00				(void);	// �v���Y�}�E�H�[��[00] �̏I������
void UpdateObj_plasma_wall_00				(void);	// �v���Y�}�E�H�[��[00] �̍X�V����
void DrawObj_plasma_wall_00					(void);	// �v���Y�}�E�H�[��[00] �̕`�揈��
Obj_plasma_wall_00 *GetObj_plasma_wall_00	(void);	// �v���Y�}�E�H�[��[00] �̏����擾

#endif