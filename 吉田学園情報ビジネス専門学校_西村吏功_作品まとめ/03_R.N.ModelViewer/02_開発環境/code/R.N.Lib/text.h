//========================================
// 
// �e�L�X�g�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _TEXT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

// _R.N.Lib
#include "RNmain.h"

//****************************************
// �}�N����`
//****************************************
// �e�L�X�g(2D)�̍ő吔
#define TEXT2D_MAX (128)
// �e�L�X�g(3D)�̍ő吔
#define TEXT3D_MAX (128)

//****************************************
// �񋓌^�̒�`
//****************************************
// �\���`��
typedef enum
{
	TEXT_DISP_CENTER,	// ��������
	TEXT_DISP_LEFT,		// ������
	TEXT_DISP_RIGHT,	// �E����
	TEXT_DISP_MAX,
}TEXT_DISP;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// �e�L�X�g�̏���������
void InitText(void);
// �e�L�X�g�̏I������
void UninitText(void);
// �e�L�X�g�̕`�揈��
void DrawText_(void);
//========== *** ���o�� ***
// �t�H���g�̓ǂݍ��ݏ���
void LoadFont(void);
//========== *** �ݒ� ***
// �e�L�X�g(2D)�̐ݒ菈��
void SetText2D(char *pString, int nFont, TEXT_DISP disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight);
// �e�L�X�g(3D)�̐ݒ菈��
void SetText3D(char *pString, int nFont, TEXT_DISP disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard);

#endif