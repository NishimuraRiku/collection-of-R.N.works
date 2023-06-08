#if 0
//========================================
// 
// �}�b�v����(3D)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** map-generator3D.h ***
//========================================
#ifndef _MAPGENERATOR3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MAPGENERATOR3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// R.N.Lib
#include "../Space/fog.h"
#include "map-chip3D.h"

//****************************************
// �\���̒�`
//****************************************
//========== *** �}�b�v��� ***
// �}�b�v�`�b�v(3D)�ݒ���\����
typedef struct
{
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// �ʒu
	D3DXVECTOR3 rot = INITD3DXVECTOR3;	// ����
	int nType = 0;	// ���
}MapChip3DSet;

// �}�b�v�ݒ���\����
typedef struct
{
	// �}�b�v�`�b�v(3D)�ݒ���
	MapChip3DSet aMapChip3DSet[MAPCHIP3D_MAX];
	int nMapChip3DNum = 0;	// �}�b�v�`�b�v(3D)�̐ݒ萔
	
	bool bUseFog = false;	// �t�H�O�̎g�p�t���O
	FogSet fogSet;	// �t�H�O�̐ݒ���
}MapSet3D;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �|�C���^�擾 ***
// �}�b�v���̃|�C���^�擾
MapSet3D *GetMapSet(void);
//========== *** ���o�� ***
// �}�b�v���̓ǂݍ��ݏ���
void LoadMapData(char *pPath);
// �}�b�v���̏������ݏ���
void SaveMapData(char *pPath);

#endif
#endif