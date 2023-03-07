//========================================
// 
// �e�L�X�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** text.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//****************************************
// �}�N����`
//****************************************
//========== *** ��{��� ***
// �e�L�X�g�̕������̍ő吔
#define TEXT_DIGIT_MAX (128)
// �t�H���g�̃f�[�^�t�@�C���̑��΃p�X
#define FONT_DATA_FILE "data\\FONT_LIST.txt"
// �t�H���g�̍ő吔
#define FONT_MAX (8)

//****************************************
// �\���̂̒�`
//****************************************
//========== *** ��{��� ***
// �t�H���g�̎�ޖ��̏��\����
typedef struct
{
	char aTexPath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	char cStartChar;		// �J�n����
	int nPtnWidth;			// �p�^�[����
	int nPtnHeight;			// �p�^�[������
	int nSpaceNum;			// �Ԋu�䗦�̕��q
	int nSpaceDenom;		// �Ԋu�䗦�̕���
}Font;
//========== *** �e�L�X�g(2D) ***
// �e�L�X�g(2D)���\���̂��`
typedef struct
{
	char aString[TXT_MAX];	// ������
	int nFont;				// �t�H���g
}Text2D;
// �e�L�X�g(2D)�̊Ǘ����\���̂��`
typedef struct 
{
	int nUseNum;	// �g�p����Ă��鐔
}Text2DControl;
//========== *** �e�L�X�g(3D) ***
// �e�L�X�g(3D)���\���̂��`
typedef struct
{
	char aString[TXT_MAX];	// ������
	int nFont;				// �t�H���g
	D3DXVECTOR3 pos;		// �ʒu
	bool bZtest;			// Z�e�X�g�t���O
	bool bLighting;			// ���C�e�B���O�t���O
	bool bBillboard;		// �r���{�[�h�t���O
}Text3D;
// �e�L�X�g(3D)�̊Ǘ����\���̂��`
typedef struct
{
	int nUseNum;	// �g�p����Ă��鐔
}Text3DControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �e�L�X�g(2D) ***
// �e�L�X�g(2D)�̃p�����[�^�[�̏����l��Ԃ�
Text2D InitParameterText2D(void);
// �e�L�X�g(2D)�̊Ǘ����̃p�����[�^�[�̏����l��Ԃ�
Text2DControl InitParameterText2DControl(void);
// �e�L�X�g(2D)�̏���������
void InitText2D(void);
// �e�L�X�g(2D)�̏I������
void UninitText2D(void);
// �e�L�X�g(2D)�̕`�揈��
void DrawText2D(void);
//========== *** �e�L�X�g(3D) ***
// �e�L�X�g(3D)�̃p�����[�^�[�̏����l��Ԃ�
Text3D InitParameterText3D(void);
// �e�L�X�g(3D)�̊Ǘ����̃p�����[�^�[�̏����l��Ԃ�
Text3DControl InitParameterText3DControl(void);
// �e�L�X�g(3D)�̏���������
void InitText3D(void);
// �e�L�X�g(3D)�̏I������
void UninitText3D(void);
// �e�L�X�g(3D)�̕`�揈��
void DrawText3D(void);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureText[FONT_MAX] = {};
// �e�L�X�g(2D)�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffText2D = NULL;
Text2D g_text2D[TEXT2D_MAX];	// �e�L�X�g(2D)�̏��
Text2DControl g_text2DControl;	// �e�L�X�g(2D)�̊Ǘ����
// �e�L�X�g(3D)�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffText3D = NULL;
Text3D g_text3D[TEXT2D_MAX];	// �e�L�X�g(3D)�̏��
Text3DControl g_text3DControl;	// �e�L�X�g(3D)�̊Ǘ����
D3DXMATRIX g_mtxWorldText3D;	// �e�L�X�g(3D)�̃��[���h�}�g���b�N�X
Font g_aFont[FONT_MAX];			// �t�H���g���̏��

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �e�L�X�g(2D) *** |----------------
//============================================================
//========================================
// InitParameterText2D�֐� - �e�L�X�g(2D)�̃p�����[�^�[�̏����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
Text2D InitParameterText2D(void)
{
	return{
		"",	// ������
		0,	// �t�H���g
	};
}

//========================================
// InitParameterText2DControl�֐� - �e�L�X�g(2D)�̊Ǘ����̃p�����[�^�[�̏����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
Text2DControl InitParameterText2DControl(void)
{
	return{
		0,	// �g�p����Ă��鐔
	};
}

//========================================
// InitText2D�֐� - �e�L�X�g(2D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitText2D(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TEXT2D_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffText2D,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffText2D->Lock(0, 0, (void**)&pVtx, 0);

	// �Ǘ����̃p�����[�^�[�̏���������
	InitParameterText2DControl();
	
	// �e�L�X�g(2D)�̏��̃|�C���^
	Text2D *pTxt = g_text2D;

	for (int nCntTxt = 0; nCntTxt < TEXT2D_MAX; nCntTxt++, pTxt++)
	{
		// �p�����[�^�[�̏���������
		*pTxt = InitParameterText2D();

		for (int nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++, pVtx += 4)
		{
			// rhw�̐ݒ�
			SetRHW2D(pVtx);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffText2D->Unlock();
}

//========================================
// UninitText2D�֐� - �e�L�X�g(2D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitText2D(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffText2D != NULL)
	{
		g_pVtxBuffText2D->Release();
		g_pVtxBuffText2D = NULL;
	}
}

//========================================
// DrawText2D�֐� - �e�L�X�g2D�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawText2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffText2D, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�L�X�g(2D)�̏��̃|�C���^
	Text2D *pTxt = g_text2D;
	// �e�L�X�g(2D)�̊Ǘ����̃|�C���^
	Text2DControl *pTxtCtl = &g_text2DControl;

	for (int nCntTxt = 0; nCntTxt < pTxtCtl->nUseNum; nCntTxt++, pTxt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureText[pTxt->nFont]);

		int nLength = strlen(pTxt->aString);
		for (int nCntDigit = 0; nCntDigit < nLength; nCntDigit++)
		{
			if ((nCntDigit < nLength) && pTxt->aString[nCntDigit] != ' ')
			{// ���̃J�E���g��������̌�����������Ă��� & ���̕������󔒂Ŗ������A
				// �e�L�X�g�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}
	}

	pTxtCtl->nUseNum = 0;	// �g�p���Ă��鐔������������
}

//============================================================
//--------------------| *** �e�L�X�g(3D) *** |----------------
//============================================================
//========================================
// InitParameterText3D�֐� - �e�L�X�g(3D)�̃p�����[�^�[�̏����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
Text3D InitParameterText3D(void)
{
	return{
		{},					// ������
		0,					// �t�H���g
		INITD3DXVECTOR3,	// �ʒu
		false,				// Z�e�X�g�t���O
		false,				// ���C�e�B���O�t���O
		false,				// �r���{�[�h�t���O
	};
}

//========================================
// InitParameterText3DControl�֐� - �e�L�X�g(3D)�̊Ǘ����̃p�����[�^�[�̏����l��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
Text3DControl InitParameterText3DControl(void)
{
	return{
		0,	// �g�p����Ă��鐔
	};
}

//========================================
// InitText3D�֐� - �e�L�X�g(3D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitText3D(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * TEXT3D_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffText3D,
		NULL);

	// �Ǘ����̃p�����[�^�[�̏���������
	InitParameterText3DControl();

	// �e�L�X�g(3D)�̏��̃|�C���^
	Text3D *pTxt = g_text3D;

	for (int nCntText = 0; nCntText < TEXT3D_MAX; nCntText++, pTxt++)
	{
		// �p�����[�^�[�̏���������
		*pTxt = InitParameterText3D();
	}
}

//========================================
// UninitText3D�֐� - �e�L�X�g(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitText3D(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffText3D != NULL)
	{
		g_pVtxBuffText3D->Release();
		g_pVtxBuffText3D = NULL;
	}
}

//========================================
// DrawText3D�֐� - �e�L�X�g(3D)�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawText3D(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				// �r���[�}�g���b�N�X�擾�p

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffText3D, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �e�L�X�g�̏��̃|�C���^
	Text3D *pTxt = g_text3D;
	// �e�L�X�g(3D)�̊Ǘ����̃|�C���^
	Text3DControl *pTxtCtl = &g_text3DControl;

	for (int nCntTxt = 0; nCntTxt < pTxtCtl->nUseNum; nCntTxt++, pTxt++)
	{
		if (!pTxt->bZtest) 
		{// Z�e�X�g�t���O���U�̎��A
			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureText[pTxt->nFont]);

		for (int nCntDigit = 0; nCntDigit < strlen(pTxt->aString); nCntDigit++) 
		{
			if ((nCntDigit < strlen(pTxt->aString)) && pTxt->aString[nCntDigit] != ' ')
			{// ���̃J�E���g��������̌�����������Ă��� & ���̕������󔒂Ŗ������A
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_mtxWorldText3D);

				// �r���[�}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				if (pTxt->bBillboard) 
				{// �r���{�[�h�t���O���^�̎��A
					// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
					D3DXMatrixInverse(&g_mtxWorldText3D, NULL, &mtxView);
					g_mtxWorldText3D._41 = 0.0f;
					g_mtxWorldText3D._42 = 0.0f;
					g_mtxWorldText3D._43 = 0.0f;
				}
				else
				{// �r���{�[�h�t���O���U�̎��A
					// �����𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRot, FindAngleLookDown(GetCamera3D()->posV, GetCamera3D()->posR), 0.0f, 0.0f);
					D3DXMatrixMultiply(&g_mtxWorldText3D, &g_mtxWorldText3D, &mtxRot);
				}
				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, pTxt->pos.x, pTxt->pos.y, pTxt->pos.z);
				D3DXMatrixMultiply(&g_mtxWorldText3D, &g_mtxWorldText3D, &mtxTrans);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldText3D);

				// �e�L�X�g�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}

		if (!pTxt->bZtest)
		{// Z�e�X�g�t���O���U�̎��A
			// Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}

	pTxtCtl->nUseNum = 0;	// �g�p���Ă��鐔������������

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================
//========================================
// InitText�֐� - �e�L�X�g�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitText(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aFont[nCntFont].aTexPath, &g_pTextureText[nCntFont]);
	}

	// ����������
	InitText3D();	// �e�L�X�g(3D)
	InitText2D();	// �e�L�X�g(2D)
}

//========================================
// UninitText�֐� - �e�L�X�g�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitText(void)
{
	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureText[nCntFont] != NULL)
		{
			g_pTextureText[nCntFont]->Release();
			g_pTextureText[nCntFont] = NULL;
		}
	}

	// �I������
	UninitText3D();	// �e�L�X�g(3D)
	UninitText2D();	// �e�L�X�g(2D)
}

//========================================
// DrawText_�֐� - �e�L�X�g�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawText_(void)
{
	// �`�揈��
	DrawText3D();	// �e�L�X�g(3D)
	DrawText2D();	// �e�L�X�g(2D)
}

//========================================
// LoadFont�֐� - �t�H���g�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadFont(void)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX];	// �f�[�^�����p
	Font *pFont = g_aFont;		// �t�H���g���̏��̃|�C���^

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(FONT_DATA_FILE, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))
		{// �ǂݍ��݂��I��
			fclose(pFile); break; 
		}
		else if (!strcmp(aDataSearch, "FONTSET"))
		{// ���i�̐ݒ���̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// ����

				if (!strcmp(aDataSearch, "FONTSET_END")) 
				{// �ǂݍ��݂��I��
					break;
				}
				else if (!strcmp(aDataSearch, "FONT"))
				{// ���i���̏��̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	// ����

						if (!strcmp(aDataSearch, "FONT_END"))
						{// �|�C���^��i�߂�
							pFont++;
							break;
						}
						else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))
						{// �t�@�C����
							fscanf(pFile, "%s", &pFont->aTexPath); 
						}
						else if (!strcmp(aDataSearch, "START_CHAR:"))
						{// �J�n����
							fscanf(pFile, "%s", &pFont->cStartChar);
						}
						else if (!strcmp(aDataSearch, "PTN_WIDTH:"))
						{// �p�^�[����
							fscanf(pFile, "%d", &pFont->nPtnWidth);
						}
						else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))
						{// �p�^�[������
							fscanf(pFile, "%d", &pFont->nPtnHeight);
						}
						else if (!strcmp(aDataSearch, "SPACE_NUM:"))
						{// �Ԋu�䗦�̕��q 
							fscanf(pFile, "%d", &pFont->nSpaceNum);
						}
						else if (!strcmp(aDataSearch, "SPACE_DENOM:"))
						{// �Ԋu�䗦�̕��� 
							fscanf(pFile, "%d", &pFont->nSpaceDenom);
						}
					}
				}
			}
		}
	}
}

//============================================================
//--------------------| *** �ݒ� *** |------------------------
//============================================================
//========================================
// SetText2D�֐� - �e�L�X�g(2D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetText2D(char *pString, int nFont, TEXT_DISP disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffText2D->Lock(0, 0, (void**)&pVtx, 0);

	// �e�L�X�g(2D)�̊Ǘ����̃|�C���^
	Text2DControl *pTxtCtl = &g_text2DControl;
	// �e�L�X�g(2D)�̏��̃|�C���^
	Text2D *pTxt = &g_text2D[pTxtCtl->nUseNum];

	strcpy(pTxt->aString, pString);	// ���������
	pTxt->nFont = nFont;			// �t�H���g����

	// ���_���ւ̃|�C���^��i�߂�
	pVtx += 4 * pTxtCtl->nUseNum * TEXT_DIGIT_MAX;

	for (int nCntDigit = 0; nCntDigit < strlen(pString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 setPos = pos;	// �ݒ�ʒu
		float fSpaceRate =			// �Ԋu�䗦
			(float)g_aFont[nFont].nSpaceNum / (float)g_aFont[nFont].nSpaceDenom;

		// �\���`���ɉ������ݒ�ʒu�̐ݒ�
		switch (disp)
		{
		case TEXT_DISP_CENTER:
			setPos.x -= ((fWidth * fSpaceRate) * strlen(pString)) * 0.5f;	// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ����̔��������Z
			setPos.x += ((float)nCntDigit + 0.5f) * (fWidth * fSpaceRate);	// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		case TEXT_DISP_LEFT:
			setPos.x += nCntDigit * (fWidth * fSpaceRate);					// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		case TEXT_DISP_RIGHT:
			setPos.x -= (fWidth * fSpaceRate) * (strlen(pString) - 1);		// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ��������Z
			setPos.x += nCntDigit * (fWidth * fSpaceRate);					// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		}

		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			setPos,
			{},
			false,
			fWidth,
			fHeight,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, col);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			pString[nCntDigit] - g_aFont[nFont].cStartChar,
			g_aFont[nFont].nPtnWidth,
			g_aFont[nFont].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffText2D->Unlock();

	pTxtCtl->nUseNum++;	// �g�p����Ă��鐔�����Z
}

//========================================
// SetText3D�֐� - �e�L�X�g(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetText3D(char *pString, int nFont, TEXT_DISP disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard)
{
	VERTEX_3D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffText3D->Lock(0, 0, (void**)&pVtx, 0);

	// �e�L�X�g(3D)�̊Ǘ����̃|�C���^
	Text3DControl *pTxtCtl = &g_text3DControl;
	// �e�L�X�g(3D)�̏��̃|�C���^
	Text3D *pTxt = &g_text3D[pTxtCtl->nUseNum];

	strcpy(pTxt->aString, pString);	// ���������
	pTxt->nFont = nFont;			// �t�H���g����
	pTxt->pos = pos;				// �ʒu����
	pTxt->bZtest = bZtest;			// Z�e�X�g�t���O����
	pTxt->bLighting = bLighting;	// ���C�e�B���O�t���O����
	pTxt->bBillboard = bBillboard;	// �r���{�[�h�t���O����

	// ���_���ւ̃|�C���^��i�߂�
	pVtx += 4 * pTxtCtl->nUseNum * TEXT_DIGIT_MAX;

	for (int nCntDigit = 0; nCntDigit < strlen(pString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 setPos = INITD3DXVECTOR3;	// �ݒ�ʒu
		float fSpaceRate =						// �Ԋu�䗦
			(float)g_aFont[nFont].nSpaceNum / (float)g_aFont[nFont].nSpaceDenom;

		// �\���`���ɉ������ݒ�ʒu�̐ݒ�
		switch (disp)
		{
		case TEXT_DISP_CENTER:
			setPos.x -= ((fWidth * fSpaceRate) * strlen(pString)) * 0.5f;	// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ����̔��������Z
			setPos.x += ((float)nCntDigit + 0.5f) * (fWidth * fSpaceRate);	// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		case TEXT_DISP_LEFT:
			setPos.x += nCntDigit * (fWidth * fSpaceRate);					// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		case TEXT_DISP_RIGHT:
			setPos.x -= (fWidth * fSpaceRate) * (strlen(pString) - 1);		// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ��������Z
			setPos.x += nCntDigit * (fWidth * fSpaceRate);					// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(setPos.x - (fWidth * 0.5f), fHeight + (fHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(setPos.x + (fWidth * 0.5f), fHeight + (fHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(setPos.x - (fWidth * 0.5f), fHeight - (fHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(setPos.x + (fWidth * 0.5f), fHeight - (fHeight * 0.5f), 0.0f);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, col);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx,
			pString[nCntDigit] - g_aFont[nFont].cStartChar,
			g_aFont[nFont].nPtnWidth,
			g_aFont[nFont].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffText3D->Unlock();

	pTxtCtl->nUseNum++;	// �g�p����Ă��鐔�����Z
}