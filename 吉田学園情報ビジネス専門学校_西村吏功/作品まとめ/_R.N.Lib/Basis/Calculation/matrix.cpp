//========================================
// 
// �}�g���b�N�X�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** matrix.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ConvPosRotToMatrix�֐� - �ʒu�ƌ������}�g���b�N�X�ɕϊ����ĕԂ� -
// Author:RIKU NISHIMURA
//========================================
D3DXMATRIX ConvPosRotToMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtx;	// �{�̂̃}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	return mtx;
}

//========================================
// MultiplyMatrix�֐� - �}�g���b�N�X���|�����킹�� -
// Author:RIKU NISHIMURA
//========================================
D3DXMATRIX MultiplyMatrix(D3DXMATRIX mtxParent, D3DXMATRIX mtxChild) {
	// �Z�o�������i�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(
		&mtxChild,
		&mtxChild,
		&mtxParent);

	return mtxChild;
}

//========================================
// ConvMatrixToPos�֐� - �}�g���b�N�X�̈ʒu�����𕪉����ĕԂ� -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvMatrixToPos(D3DXMATRIX mtx) {
	return D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

//========================================
// ConvMatrixToRot�֐� - �}�g���b�N�X�̌��������𕪉����ĕԂ� -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvMatrixToRot(D3DXMATRIX mtx) {
	D3DXVECTOR3 rot;

	// �s���3��ڂ��擾���āAD3DXVECTOR3�ɕϊ�
	rot.x = mtx._31;
	rot.y = mtx._32;
	rot.z = mtx._33;

	// �x�N�g���𐳋K��
	D3DXVec3Normalize(&rot, &rot);

	return rot;
}

//========================================
// SetMatrixRot�֐� - �}�g���b�N�X�Ɍ������� -
// Author:RIKU NISHIMURA
//========================================
void SetMatrixRot(D3DXMATRIX *pMtx, D3DXVECTOR3 rot) {
	// ��������������̒l
	float pitch = rot.x;
	float yaw = rot.y;
	float roll = rot.z;

	D3DXMATRIX matrix;
	D3DXMatrixIdentity(pMtx); // �s���P�ʍs��ŏ�����

	// �����̗v�f��������
	pMtx->_11 = cos(yaw) * cos(roll);
	pMtx->_12 = cos(yaw) * sin(roll);
	pMtx->_13 = sin(yaw);
	pMtx->_21 = sin(pitch) * sin(yaw) * cos(roll) - cos(pitch) * sin(roll);
	pMtx->_22 = sin(pitch) * sin(yaw) * sin(roll) + cos(pitch) * cos(roll);
	pMtx->_23 = -sin(pitch) * cos(yaw);
	pMtx->_31 = -cos(pitch) * sin(yaw) * cos(roll) - sin(pitch) * sin(roll);
	pMtx->_32 = -cos(pitch) * sin(yaw) * sin(roll) + sin(pitch) * cos(roll);
	pMtx->_33 = cos(pitch) * cos(yaw);
}