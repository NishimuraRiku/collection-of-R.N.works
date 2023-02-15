//========================================
// 
// �|���S��(2D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon2D.h ***
//========================================
#ifndef _POLYGON2D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _POLYGON2D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// _R.N.Lib
#include "RNmain.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// �p�x�̎��
typedef enum
{
	ANGLE_TYPE_FREE,	// ���R
	ANGLE_TYPE_FIXED,	// �Œ�
	ANGLE_TYPE_MAX,
}ANGLE_TYPE;

//****************************************
// �\���̂̒�`
//****************************************
// �|���S�����̏��(2D)
typedef struct 
{
	// �e�N�X�`���̑��΃p�X
	char aTexturePath[TXT_MAX];
	float fWidth;	// ��
	float fHeight;	// ����
	int nMax;		// �ő吔
}Polygon2D;
//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �擾 ***
// �|���S��(2D)�̕K�v����Ԃ�
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax);
// ���݂̃J�E���g�̃|���S��(2D)�̔ԍ���Ԃ�
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount);
//========== *** ���o�� ***
// �|���S��(2D)�̃e�N�X�`���̓ǂݍ���
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax);
//========== *** �ݒ� ***
// ���_���W�̐ݒ菈��(2D)
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType);
// rhw�̐ݒ菈��(2D)
void SetRHW2D(VERTEX_2D *pVtx);
// ���_�F�̐ݒ菈��(2D)
void SetVertexColor2D(VERTEX_2D *pVtx, Color col);
// �e�N�X�`�����W�̐ݒ菈��(2D)
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
// �e�N�X�`�����W�̐ݒ菈��(2D)(�؂���ver)
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos);
//========== *** ���̑� ***
// �`��ʒu����ɍ��킹��
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);

#endif