//========================================
// 
// ���f���֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** model.h ***
//========================================
#ifndef _MODEL_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MODEL_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../../../RNmain.h"
#include "../../Other/color.h"
#include "../../Calculation/matrix.h"

//****************************************
// �}�N����`
//****************************************
#define MODELTYPE_MAX  (256)	// ���f���̎�ނ̍ő吔
#define MODEL_MAX      (256)	// ���f���̍ő吔
#define MATERIAL3D_MAX (16)		// �}�e���A���̍ő吔

//****************************************
// �\���̒�`
//****************************************
// ���f���̐ݒ���\����
typedef struct {
	int nModelIdx  = -1;				// ��ޔԍ�
	D3DXMATRIX mtx = INITD3DXMATRIX;	// �}�g���b�N�X
	Color col      = INITCOLOR;			// �F
	bool bLighting = false;				// ���C�e�B���O�t���O
}ModelSet;

// ���f���̊Ǘ����
typedef struct {
	//========== *** �ǂݍ��݊֘A ***
	// �ǂݍ��ݍς̃��f���p�X
	char aLoadModelPath[MODELTYPE_MAX][TXT_MAX] = {};
	int nModelTypeNum = 0;	// ���f���̎�ސ�

	//========== *** �ݒ�֘A ***
	int nModelSetNum = 0;		// ���f���̐ݒ萔
								// ���f���̐ݒ���
	ModelSet aModelSet[MODEL_MAX] = {};
}ModelInfo;

//****************************************
// �v���g�^�C�v�錾
//****************************************
/* ���f���̏���������   */void InitModel(void);
/* ���f���̏I������     */void UninitModel(void);
/* ���f���̕`�揈��     */void DrawModel(void);
/* ���f���̓ǂݍ��ݏ��� */int LoadModel(char *pPath);
/* ���f���̐ݒ菈��     */void SetModel(int nModelIdx = -1, D3DXMATRIX mtx = INITD3DXMATRIX, Color col = INITCOLOR, bool bLighting = false);
/* ���f���̊Ǘ����擾 */ModelInfo *GetModelInfo(void);

#endif