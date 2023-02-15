//========================================
// 
// �|���S��(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon3D.h ***
//========================================
#ifndef _POLYGON3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _POLYGON3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include <stdio.h>
// _R.N.Lib
#include "RNmain.h"

//****************************************
// �}�N����`
//****************************************
// �}�e���A��(3D)�̏��
#define MATERIAL_3D_MAX (10)
// ���i(3D)�̍ő吔
#define PARTS_3D_MAX (8)
// ���[�V����(3D)�̍ő吔
#define MOTION_3D_MAX (16)
// ���[�V����(3D)�̈ʒu�̃A�j���[�V�������̍ő吔
#define MOTION_3D_POSITION_ANIM_MAX (8)
// ���[�V����(3D)�̊p�x�̃A�j���[�V�������̍ő吔
#define MOTION_3D_ROTATION_ANIM_MAX	(8)
// ���[�V����(3D)�̃T�E���̃A�j���[�V�������̍ő吔
#define MOTION_3D_SOUND_ANIM_MAX (8)

//****************************************
// �\���̂̒�`
//****************************************
//========== *** ���i(3D)�֘A ***
// ���i(3D)�\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 posResult;	// �ʒu(����)
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rotResult;	// ����(����)
	D3DXVECTOR3 rot;		// ����

	// ���[�V�����֘A
	bool bPosAnim;			// �ʒu�A�j���[�V�����t���O
	int nCounterPosAnim;	// �ʒu�A�j���[�V�����J�E���^�[
	int nPosAnimTime;		// �ʒu�A�j���[�V�����ɂ����鎞��
	D3DXVECTOR3 posOld;		// ���̈ʒu
	D3DXVECTOR3 targetPos;	// �ڕW�ʒu
	bool bRotAnim;			// �p�x�A�j���[�V�����t���O
	int nCounterRotAnim;	// �p�x�A�j���[�V�����J�E���^�[
	int nRotAnimTime;		// �p�x�A�j���[�V�����ɂ����鎞��
	D3DXVECTOR3 rotOld;		// ���̊p�x
	D3DXVECTOR3 targetRot;	// �ڕW�p�x
}Parts3D;
// ���i(3D)�Ǘ��\����
typedef struct
{
	int nMotion;		// ���[�V�����ԍ�
	int nCounterMotion;	// ���[�V�����J�E���^�[
	// ���i���
	Parts3D aParts[PARTS_3D_MAX];
}Parts3DInfo;
// ���i(3D)�̎�ޖ��̏��\����
typedef struct
{
	// �ǂݍ��ݏ��
	char aModelPath[TXT_MAX];		// ���f���̑��΃p�X
	int nParent;					// �e�ԍ�
	D3DXVECTOR3 fixedRelativePos;	// �Œ�̑��Έʒu
	D3DXVECTOR3 fixedRelativeRot;	// �Œ�̑��Ίp�x
	D3DXVECTOR3 rotRelativePos;		// ��]���̑��Έʒu
}Parts3DType;
// ���i�ݒ���\����
typedef struct
{
	int nPartsNum;	// ���i��
	// ���i�̎�ޖ��̏��
	Parts3DType aPartsType[PARTS_3D_MAX];
}Parts3DSet;
//========== *** ���[�V�����֘A(3D) ***
// �ʒu�̃A�j���[�V����(3D)���\����
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nMoveTime;		// ���B�ɂ����鎞��
	int nTime;			// �؂�ւ�����
}PositionAnim3D;
// �p�x�̃A�j���[�V����(3D)���\����
typedef struct
{
	D3DXVECTOR3 rot;		// �p�x
	int			nMoveTime;	// ���B�ɂ����鎞��
	int			nTime;		// �؂�ւ�����
}RotationAnim3D;
// �T�E���h�̃A�j���[�V����(3D)���\����
typedef struct
{
	int	nSound;	// �T�E���h�ԍ�
	int	nTime;	// �؂�ւ�����
}SoundAnim3D;
// ���i���̃��[�V����(3D)���\����
typedef struct
{
	int nPosAnimNum;	// �ʒu(����)�̃A�j���[�V������
	// �ʒu(����)�̃A�j���[�V�������
	PositionAnim3D aPosAnim[MOTION_3D_POSITION_ANIM_MAX];
		
	int nRotAnimNum;	// �p�x(����)�̃A�j���[�V������
	// �p�x(����)�̃A�j���[�V�������
	RotationAnim3D aRotAnim[MOTION_3D_ROTATION_ANIM_MAX];
}PartsMotion3D;
// ���[�V����(3D)���\����
typedef struct
{
	int nLoopTime;	// ���[�v����
	bool bLoop;		// ���[�v���邩�t���O
	// ���i���̃��[�V�������
	PartsMotion3D aPartsMotion[PARTS_3D_MAX];
					
	int nSoundAnimNum;	// �T�E���h�̃A�j���[�V������
	// �T�E���h�̃A�j���[�V�������
	SoundAnim3D aSoundAnim[MOTION_3D_SOUND_ANIM_MAX];
}Motion3D;
// ���[�V����(3D)�ݒ���\����
typedef struct
{
	int nMotionNum = 1;					// ���[�V������
	Motion3D aMotion[MOTION_3D_MAX];	// ���[�V�����̏��
}Motion3DSet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ���i(3D)�֘A ***
// ���i�ݒ���(3D)�̓ǂݍ��ݏ���
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);
// ���i(3D)�̏���������
void InitParts3DInfo(Parts3DInfo *pPartsInfo);
// ���i(3D)�̃g�����X�t�H�[���̏���������
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
// ���i(3D)�̃g�����X�t�H�[���̎擾����
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
// ���i(3D)�Ǘ����̃p�����[�^�[�����l��Ԃ�
Parts3DInfo InitParameterParts3DInfo(void);
// ���i(3D)�̃}�g���b�N�X���擾
D3DXMATRIX GetMatrixParts3D(int nParts, D3DXVECTOR3 pos, D3DXVECTOR3 rot, Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
//========== *** ���[�V����(3D)�֘A ***
// ���[�V����(3D)�ݒ���̓ǂݍ��ݏ���
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet);
// ���[�V����(3D)�̍X�V����
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
// ���[�V����(3D)�̐ݒ菈��
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion);
//========== *** �ݒ� ***
// ���_���W�̐ݒ菈��(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
// �@���x�N�g���̐ݒ菈��(3D)
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
// ���_�F�̐ݒ菈��(3D)
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
// �e�N�X�`�����W�̐ݒ菈��(3D)
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
// �}�e���A���̐ݒ�
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col);

#endif