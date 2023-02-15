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

#include "main.h"

//****************************************
// �}�N����`
//****************************************
// �e�L�X�g(2D)�̍ő吔
#define TEXT2D_MAX	(128)

// �e�L�X�g(3D)�̍ő吔
#define TEXT3D_MAX	(128)

//****************************************
// �񋓌^�̒�`
//****************************************
// �t�H���g
typedef enum
{
	FONT_000,
	FONT_001,
	FONT_002,
	FONT_003,
	FONT_004,
	FONT_MAX
}FONT;

// �\���`��
typedef enum
{
	DISPLAY_CENTER,	// ��������
	DISPLAY_LEFT,	// ���l��
	DISPLAY_RIGHT,	// �E�l��
}DISPLAY;

// �t�H���g�̎�ޖ��̏��\����
typedef struct
{
	const char	c_aPath			// �e�N�X�`���t�@�C���̑��΃p�X
				[TXT_MAX];
	const float	c_fWidth;		// ��
	const float	c_fHeight;		// ����
	const float	c_fSpaceX;		// �z�u�ԊuX
	const float	c_fSpaceY;		// �z�u�ԊuY
	const char	c_cStartChar;	// �J�n����
	const int	c_nPtnMaxX;		// �p�^�[����(��)
	const int	c_nPtnMaxY;		// �p�^�[����(�c)
}Font;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �t�H���g�̏����擾
Font *GetFont(void);

// �e�L�X�g�̏���������
// �e�L�X�g�̏I������
// �e�L�X�g�̕`�揈��
void InitText(void);
void UninitText(void);
void DrawText_(void);

// �e�L�X�g(2D)�̕`�揈��
void DrawText2D(void);

// �e�L�X�g(3D)�̕`�揈��
void DrawText3D(void);

// �e�L�X�g(2D)�̐ݒ菈��
void SetText2D(char *pString, FONT font, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight);

// �e�L�X�g(3D)�̐ݒ菈��
void SetText3D(char *pString, FONT font, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard);

#endif