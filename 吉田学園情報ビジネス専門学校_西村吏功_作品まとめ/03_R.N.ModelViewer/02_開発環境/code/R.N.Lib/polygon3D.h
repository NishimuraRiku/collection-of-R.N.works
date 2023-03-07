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
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �|���S��(3D)�֘A�̏���������
void InitPolygon3D(void);
// �|���S��(3D)�֘A�̕`�揈��
void DrawPolygon3D(void);
// ��(3D)�̐���������
void InitLine3DNum(void);
//========== *** �ݒ� ***
// ��(3D)�̐ݒ菈��
void SetLine3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, Color col);
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