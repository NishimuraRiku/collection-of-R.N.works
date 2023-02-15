//========================================
// 
// �@��[00] �̏���
// Author:���� ����
// 
//========================================
// *** chr_fighter_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "physics.h"
#include "sound.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "atk_explosion_01.h"
#include "chr_enemy_00.h"
#include "chr_fighter_00.h"
#include "eff_laser_pointer_00.h"
#include "eff_screen_00.h"
#include "md_game_00.h"
#include "md_title_00.h"
#include "obj_plasma_block_00.h"
#include "obj_plasma_wall_00.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************

// �@��[00] �̏����̔��e������
// �@��[00] �̏o���ɂ����鎞��
// �@��[00] �̑傫���Ɋ֗^����o���̃J�E���^�[�ɂ�����{��
// �@��[00] �̃v���C���[�������E������
// �@��[00] �̉�]�ʂɂ�����{��
// �@��[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
// �@��[00] �̖��G����
// �@��[00] �̃_���[�W��Ԃ̎���
// �@��[00] �̔�_���[�W���̐U��
// �@��[00] �̔�_���[�W���̉�ʌ��ʂ̐F
#define CHR_FIGHTER_00_INIT_BOMB_NUM		(2)
#define CHR_FIGHTER_00_POP_TIME				(40)
#define CHR_FIGHTER_00_POP_SIZE				(0.08f)
#define CHR_FIGHTER_00_PLAYER_RANGE_SOUND	(800.0f)
#define CHR_FIGHTER_00_ROTATION_DIAMETER	(0.3f)
#define CHR_FIGHTER_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\CHR_FIGHTER_00_TYPE_DATA.txt"
#define CHR_FIGHTER_00_INVINCIBLE_TIME		(30)
#define CHR_FIGHTER_00_DAMAGE_TIME			(30)
#define CHR_FIGHTER_00_DAMAGE_VIBRATION		(64.0f)
#define CHR_FIGHTER_00_DAMAGE_COLOR			{167,0,0,0}

//****************************************
// �񋓌^�̒�`
//****************************************

// �@��[00] �̃|���S���ԍ�
typedef enum 
{
	CHR_FIGHTER_00_POLYGON_NORMAL = 0,		// �ʏ�
	CHR_FIGHTER_00_POLYGON_NUM				// �|���S����
}CHR_FIGHTER_00_POLYGON;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �@��[00] �̃L�[���͏���
void KeyInputChr_fighter_00(void);

// �@��[00] �̏�ԏ���
void StateProcessChr_fighter_00(void);

// �@��[00] �̃p�����[�^�[����ނɉ����Đݒ�
void SetParameterChr_fighter_00(void);

// �v���Y�}�E�H�[��[00] �Ƃ̓����蔻��
void CollisionObj_plasma_wall_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_fighter_00	[CHR_FIGHTER_00_TYPE_MAX]	= {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChr_fighter_00								= NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Chr_fighter_00			g_chr_fighter_00;												// �@��[00] �̏��
Chr_fighter_00Type		g_aChr_fighter_00Type		[CHR_FIGHTER_00_TYPE_MAX];			// �@��[00] �̎�ޖ��̏��

//========== *** �@��[00] �̏����擾 ***
Chr_fighter_00 *GetChr_fighter_00(void)
{
	return &g_chr_fighter_00;
}

//========== *** �@��[00] �̎�ޖ��̏����擾 ***
Chr_fighter_00Type *GetChr_fighter_00Type(void)
{
	return g_aChr_fighter_00Type;
}

//========================================
// InitChr_fighter_00�֐� - �@��[00] �̏��������� -
//========================================
void InitChr_fighter_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntChr_fighter_00Type = 0; nCntChr_fighter_00Type < CHR_FIGHTER_00_TYPE_MAX; nCntChr_fighter_00Type++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aChr_fighter_00Type[nCntChr_fighter_00Type].aTexturPath, &g_aTextureChr_fighter_00[nCntChr_fighter_00Type]);
	}
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_FIGHTER_00_POLYGON_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChr_fighter_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	pChr->nCounterAnim	= 0;								// �A�j���[�V�����J�E���^�[ ������������
	pChr->nPattern		= 0;								// �p�^�[��No.				������������
	pChr->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu						������������
	pChr->move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���					������������
	pChr->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����						������������
	pChr->bUse			= false;							// �g�p����Ă��邩�t���O	������������
	pChr->nScore		= 0;								// �X�R�A					������������
	pChr->nType			= 0;								// ���						������������
	pChr->nShotCounter	= 0;								// �e�̔��˃J�E���^�[		������������
	pChr->nBomb			= CHR_FIGHTER_00_INIT_BOMB_NUM;		// ���e������				������������
	pChr->nCntDamage	= 0;								// ��e�J�E���g				������������

	// �X�e�[�^�X����ނɉ����Đݒ�
	SetParameterChr_fighter_00();

	// ��Ԋ֘A�̏�����
	for (int nCntState = 0; nCntState < CHR_FIGHTER_00_STATE_MAX; nCntState++)
	{
		pChr->aState[nCntState]			= false;	// ��� ������������
		pChr->aCounterState[nCntState]	= 0;		// ��ԊǗ��̃J�E���^�[ ������������
	}

	// �|���S�����̐ݒ�
	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++, pVtx += 4)
	{
		{
			D3DXVECTOR3 drawPos = pChr->pos;												// �`��ʒu
			D3DXVECTOR3 rot		= pChr->rot;												// ����
			float fWidth		= g_aChr_fighter_00Type[pChr->nType].fWidth;				// ��
			float fHeight		= g_aChr_fighter_00Type[pChr->nType].fHeight;				// ����
			float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle		= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= pChr->nPattern;								// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxY;	// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffChr_fighter_00->Unlock();
}

//========================================
// UninitChr_fighter_00�֐� - �@��[00] �̏I������ -
//========================================
void UninitChr_fighter_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntChr_fighter_00 = 0; nCntChr_fighter_00 < CHR_FIGHTER_00_TYPE_MAX; nCntChr_fighter_00++)
	{
		if (g_aTextureChr_fighter_00[nCntChr_fighter_00] != NULL)
		{
			g_aTextureChr_fighter_00[nCntChr_fighter_00]->Release();
			g_aTextureChr_fighter_00[nCntChr_fighter_00] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffChr_fighter_00 != NULL) 
	{
		g_pVtxBuffChr_fighter_00->Release();
		g_pVtxBuffChr_fighter_00 = NULL;
	}
}

//========================================
// UpdateChr_fighter_00�֐� - �@��[00] �̍X�V���� -
//========================================
void UpdateChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	// �J�����̈ʒu���@��[00] �̍��W�Ƃ���
	GetCamera()->pos = pChr->pos;

	if (!pChr->bUse)
	{// �g�p����Ă����ԂłȂ����A
		// �������I������
		return;
	}

	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	// ��ԏ���
	StateProcessChr_fighter_00();

	if (pChr->nShotCounter < pChr->nShotTime)
	{// �e�̔��˃J�E���^�[���e�̔��˂ɂ����鎞�ԂɒB���Ă��Ȃ����A
		// �e�̔��˃J�E���^�[�����Z
		pChr->nShotCounter++;
	}

	// �L�[���͏���
	KeyInputChr_fighter_00();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu/��]���X�V
	pChr->pos.x += pChr->move.x;
	pChr->pos.y += pChr->move.y;
	
	// �ړ��ʂ��X�V(����)
	pChr->move.x += (0.0f - pChr->move.x) * 0.1f;
	pChr->move.y += (0.0f - pChr->move.y) * 0.1f;

	// �v���Y�}�E�H�[��[00] �Ƃ̓����蔻��
	CollisionObj_plasma_wall_00();

	// �p�x��ڕW�p�x�Ɍ����Đ��ڂ���
	pChr->rot.z += AngleDifference(pChr->rot.z, pChr->targetRot.z) * CHR_FIGHTER_00_ROTATION_DIAMETER;

	// �p�x�𐧌䂷��
	ControlAngle(&pChr->rot.z);

	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++, pVtx += 4)
	{
		int nPtn		= pChr->nPattern;								// �p�^�[��No.
		int nPtnMaxX	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxX;	// �e�N�X�`���̃p�^�[����(��)
		int nPtnMaxY	= g_aChr_fighter_00Type[pChr->nType].nPtnMaxY;	// �e�N�X�`���̃p�^�[����(�c)
		int nR, nG, nB;													// �F(RGB)

		{
			D3DXVECTOR3 drawPos = pChr->pos;																							// �`��ʒu
			float fWidth	= pChr->fWidth * (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));		// ��
			float fHeight	= pChr->fHeight * (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));	// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;													// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);																					// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((pChr->rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((pChr->rot.z + D3DX_PI) + fAngle) * fLength;
		}

		{
			float fRed		= 1.0f - ((float)pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE] / (float)CHR_FIGHTER_00_DAMAGE_TIME);	// �ԓx
			float fAlpha	= 1.0f - ((float)pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] / (float)CHR_FIGHTER_00_POP_TIME);	// �����x

			// ���_�J���[�̐ݒ�																						   
			pVtx[0].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
			pVtx[1].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
			pVtx[2].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
			pVtx[3].col = D3DCOLOR_RGBA(255, (int)(255 * fRed), (int)(255 * fRed), (int)(255 * fAlpha));
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffChr_fighter_00->Unlock();
}

//========================================
// LoadChr_fighter_00�֐� - �@��[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadChr_fighter_00(void)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX];	// �f�[�^�����p

	Chr_fighter_00Type *pChrType
		= &g_aChr_fighter_00Type[0];	// �@��[00] �̎�ޖ��̏��

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(CHR_FIGHTER_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// �������ʎq��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		// �����p�̕�����Ɍ��݂̍s�̕������ǂݍ���
		fscanf(pFile, "%s", aDataSearch);

		if (strcmp(aDataSearch, "END") == 0)
		{// �����p�̕�����[����] ���ʎq�ƈ�v�������A
			// �t�@�C�������
			fclose(pFile);

			break;
		}

		if (aDataSearch[0] == '#')
		{// �����p�̕�����̍ŏ��̕�����[�R�����g] ���ʎq�ƈ�v�������A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (strcmp(aDataSearch, "FIGHTER_TYPE") == 0)
		{// �����p�̕�����[�@�̂̎��] ���ʎq�ƈ�v�������A
			// �@�̂̎�ޖ��̏���ǂݍ���
			while (1)
			{
				// �����p�̕�����Ɍ��݂̍s�̕������ǂݍ���
				fscanf(pFile, "%s", aDataSearch);
				if (strcmp(aDataSearch, "FIGHTER_TYPE_END") == 0)
				{// �����p�̕�����[����] ���ʎq�ƈ�v�������A
					pChrType++;
					break;
				}
				else if (strcmp(aDataSearch, "����:") == 0)
				{// �����p�̕�����[����] ���ʎq�ƈ�v�������A
					// ���̂�ǂݍ���
					fscanf(pFile, "%s", pChrType->aName);
				}
				else if (strcmp(aDataSearch, "�e�N�X�`���̑��΃p�X:") == 0)
				{// �����p�̕�����[�e�N�X�`���̑��΃p�X] ���ʎq�ƈ�v�������A
					// �e�N�X�`���̑��΃p�X��ǂݍ���
					fscanf(pFile, "%s", pChrType->aTexturPath);
				}
				else if (strcmp(aDataSearch, "�p�^�[���̏��X:") == 0)
				{// �����p�̕�����[�p�^�[�����X] ���ʎq�ƈ�v�������A
					// �p�^�[�����X��ǂݍ���
					fscanf(pFile, "%d", &pChrType->nPtnMaxX);
				}
				else if (strcmp(aDataSearch, "�p�^�[���̏��Y:") == 0)
				{// �����p�̕�����[�p�^�[�����Y] ���ʎq�ƈ�v�������A
					// �p�^�[�����Y��ǂݍ���
					fscanf(pFile, "%d", &pChrType->nPtnMaxY);
				}
				else if (strcmp(aDataSearch, "�ϐF���̒i:") == 0)
				{// �����p�̕�����[�ϐF���̒i] ���ʎq�ƈ�v�������A
					// �ϐF���̒i��ǂݍ���
					fscanf(pFile, "%d", &pChrType->nFillStep);
				}
				else if (strcmp(aDataSearch, "��:") == 0)
				{// �����p�̕�����[��] ���ʎq�ƈ�v�������A
					// ����ǂݍ���
					fscanf(pFile, "%f", &pChrType->fWidth);

					// �����s�N�Z���P�ʂŏC��
					pChrType->fWidth *= PIXEL;
				}
				else if (strcmp(aDataSearch, "����:") == 0)
				{// �����p�̕�����[����] ���ʎq�ƈ�v�������A
					// ������ǂݍ���
					fscanf(pFile, "%f", &pChrType->fHeight);

					// �������s�N�Z���P�ʂŏC��
					pChrType->fHeight *= PIXEL;
				}
				else if (strcmp(aDataSearch, "�����蔻��̕�:") == 0)
				{// �����蔻��̕���ǂݍ���
					fscanf(pFile, "%f", &pChrType->fHitTestWidth);

					// �����蔻����s�N�Z���P�ʂŏC��
					pChrType->fHitTestWidth *= PIXEL;
				}
				else if (strcmp(aDataSearch, "�����蔻��̍���:") == 0)
				{// �����蔻��̕���ǂݍ���
					fscanf(pFile, "%f", &pChrType->fHitTestHeight);

					// �����蔻����s�N�Z���P�ʂŏC��
					pChrType->fHitTestHeight *= PIXEL;
				}
				else if (strcmp(aDataSearch, "�����蔻��̎��:") == 0)
				{// �����蔻��̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �����蔻��̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadHitTestType(aDataSearch, &pChrType->hitTestType);
				}
				else if (strcmp(aDataSearch, "�̗�:") == 0)
				{// �����p�̕�����[�̗�] ���ʎq�ƈ�v�������A
					// �̗͂�ǂݍ���
					fscanf(pFile, "%d", &pChrType->nLife);
				}
				else if (strcmp(aDataSearch, "�ړ���:") == 0)
				{// �����p�̕�����[�ړ���] ���ʎq�ƈ�v�������A
					// �ړ��͂�ǂݍ���
					fscanf(pFile, "%f", &pChrType->fMove);
				}
				else if (strcmp(aDataSearch, "�e�̔��˂ɂ����鎞��:") == 0)
				{// �����p�̕�����[�e�̔��˂ɂ����鎞��] ���ʎq�ƈ�v�������A
					// �e�̔��˂ɂ����鎞�Ԃ�ǂݍ���
					fscanf(pFile, "%d", &pChrType->nShotTime);
				}
			}
		}			
	}
}

//========================================
// StateProcess�֐� - �@��[00] �̏�ԏ��� -
//========================================
void StateProcessChr_fighter_00(void) 
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	// ��ԏ���
	for (int nCntState = 0; nCntState < CHR_FIGHTER_00_STATE_MAX; nCntState++)
	{
		if (pChr->aState[nCntState])
		{// ���̏�Ԃ��^�̎�

			switch (nCntState)
			{
			case CHR_FIGHTER_00_STATE_IN_POP: // *** �o���� ***

				if (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] > 0)
				{// ��ԊǗ��J�E���^�[[�o����]��0�������Ă��鎞

				 // ��ԊǗ��J�E���^�[[�o����]�����Z
					pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP]--;
				}
				else
				{// ��ԊǗ��J�E���^�[[�o����]��0�ȉ��̎�

				 // ���[�o����]���U�ɂ���
					pChr->aState[CHR_FIGHTER_00_STATE_IN_POP] = false;
				}

				break;
			case CHR_FIGHTER_00_STATE_DAMAGE: // *** �_���[�W ***

				if (pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE] > 0)
				{// ��ԊǗ��J�E���^�[[�_���[�W]��0�������Ă��鎞

					// ��ԊǗ��J�E���^�[[�_���[�W]�����Z
					pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE]--;
				}
				else
				{// ��ԊǗ��J�E���^�[[�_���[�W]��0�ȉ��̎�

					// ���[�_���[�W]���U�ɂ���
					pChr->aState[CHR_FIGHTER_00_STATE_DAMAGE] = false;
				}

				break;
			case CHR_FIGHTER_00_STATE_INVINCIBLE: // *** ���G ***

				if (pChr->aCounterState[CHR_FIGHTER_00_STATE_INVINCIBLE] > 0)
				{// ��ԊǗ��J�E���^�[[���G]��0�������Ă��鎞

					// ��ԊǗ��J�E���^�[[���G]�����Z
					pChr->aCounterState[CHR_FIGHTER_00_STATE_INVINCIBLE]--;
				}
				else
				{// ��ԊǗ��J�E���^�[[���G]��0�ȉ��̎�

					// ���[���G]���U�ɂ���
					pChr->aState[CHR_FIGHTER_00_STATE_INVINCIBLE] = false;
				}

				break;
			}
		}
	}
}

//========================================
// KeyInput�֐� - �@��[00] �̃L�[���͏��� -
//========================================
void KeyInputChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	if ((pChr->aState[CHR_FIGHTER_00_STATE_IN_POP])
		||
		(GetGameState() == MD_GAME_00_STATE_RESULT)
		||
		(GetGameState() == MD_GAME_00_STATE_GAME_CLEAR)
		||
		(GetGameState() == MD_GAME_00_STATE_GAME_OVER))
	{// �o�����A�������̓��U���g/�Q�[���N���A/�Q�[���I�[�o�[�̎��A
		// �����𔲂���
		return;
	}

	bool	bShotInput	//
			= false;	// �V���b�g�̓��̓t���O

	if ((GetMousePress(MOUSE_LEFT))
		||
		(GetStick().aTplDiameter[STICK_TYPE_RIGHT] > 0))
	{// �V���b�g�̓��͂�����Ă��鎞�A
		// �V���b�g�̓��̓t���O��^�ɂ���
		bShotInput = true;

		if (GetMousePress(MOUSE_LEFT))
		{// �}�E�X�̍���������Ă��鎞�A
			D3DXVECTOR3 drawPos			//
						= pChr->pos;	// �`��ʒu

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// �ڕW�p�x��`��ʒu����J�[�\���܂ł̊p�x�ɐݒ�
			pChr->targetRot.z = FindAngle(drawPos, GetCursorPosOnScreen());
		}
		else if (GetStick().aTplDiameter[STICK_TYPE_RIGHT] > 0)
		{// �E�X�e�B�b�N�����͂���Ă��鎞�A
			// �@��[00] �̖ڕW�p�x��ݒ�
			pChr->targetRot.z = GetStick().aAngle[STICK_TYPE_RIGHT];
		}

		// ���[�U�[�|�C���^�\[00] �̏�Ԃ��������ɂ���
		SetStateEff_laser_pointer_00(EFF_LASER_POINTER_00_STATE_IN_CLEAR);

		if (pChr->nShotCounter >= pChr->nShotTime)
		{// �e�̔��˃J�E���^�[���e�̔��˂ɂ����鎞�ԂɒB�������A
			// �e�̔��˃J�E���^�[��������
			pChr->nShotCounter = 0;

			// �e�̐ݒ菈��
			SetAtk_bullet_00(
				D3DXVECTOR3((sinf(pChr->targetRot.z) * pChr->fHitTestWidth) + pChr->pos.x,
				(cosf(pChr->targetRot.z) * pChr->fHitTestWidth) + pChr->pos.y, 0.0f),
				pChr->targetRot,
				0,
				ATK_PARENT_TYPE_PLAYER);
		}
	}

	{
		D3DXVECTOR3 moveRot;	// �ړ��p�x
		bool		bMoveInput	//
					= true;		// �ړ��̓��̓t���O

		if (GetKeyboardPress(DIK_W))
		{// ��L�[��������Ă��鎞�A
			
			if (GetKeyboardPress(DIK_A))
			{// ���L�[��������Ă��鎞�A
				// �@��[00] �̈ړ��p�x��ݒ�
				moveRot.z = D3DX_PI * -0.75f;
			}
			else if (GetKeyboardPress(DIK_D))
			{// �E�L�[��������Ă��鎞�A
				// �@��[00] �̈ړ��p�x��ݒ�
				moveRot.z = D3DX_PI * 0.75f;
			}
			else
			{// ��L�[�̂݉�����Ă��鎞
				// �@��[00] �̈ړ��p�x��ݒ�
				moveRot.z = D3DX_PI * 1.0f;
			}

			// �ړ��ʂ��X�V
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetKeyboardPress(DIK_S))
		{// ���L�[��������Ă��鎞�A
			if (GetKeyboardPress(DIK_A))
			{// ���L�[��������Ă��鎞�A
				// �@��[00] �̈ړ��p�x��ݒ�
				moveRot.z = D3DX_PI * -0.25f;
			}
			else if (GetKeyboardPress(DIK_D))
			{// �E�L�[��������Ă��鎞�A
				// �@��[00] �̈ړ��p�x��ݒ�
				moveRot.z = D3DX_PI * 0.25f;
			}
			else
			{// ���L�[�̂݉�����Ă��鎞�A
				// �@��[00] �̈ړ��p�x��ݒ�
				moveRot.z = D3DX_PI * 0.0f;
			}

			// �ړ��ʂ��X�V
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetKeyboardPress(DIK_A))
		{// ���L�[��������Ă��鎞�A
			// �@��[00] �̈ړ��p�x��ݒ�
			moveRot.z = D3DX_PI * -0.5f;

			// �ړ��ʂ��X�V
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetKeyboardPress(DIK_D))
		{// �E�L�[��������Ă��鎞�A
			// �@��[00] �̈ړ��p�x��ݒ�
			moveRot.z = D3DX_PI * 0.5f;

			// �ړ��ʂ��X�V
			pChr->move.x += sinf(moveRot.z) * pChr->fMove;
			pChr->move.y += cosf(moveRot.z) * pChr->fMove;
		}
		else if (GetStick().aTplDiameter[STICK_TYPE_LEFT] > 0)
		{// ���X�e�B�b�N�����͂���Ă��鎞�A
			// �@��[00] �̈ړ��p�x��ݒ�
			moveRot.z = GetStick().aAngle[STICK_TYPE_LEFT];

			// �ړ��ʂ��X�V
			pChr->move.x += sinf(moveRot.z) * pChr->fMove * GetStick().aTplDiameter[STICK_TYPE_LEFT];
			pChr->move.y += cosf(moveRot.z) * pChr->fMove * GetStick().aTplDiameter[STICK_TYPE_LEFT];
		}
		else
		{// �ǂ̕����L�[��������Ă��Ȃ����A
			// �ړ��̓��̓t���O���U�ɂ���
			bMoveInput = false;

			// �ړ��p�x�����݂̊p�x�ɐݒ�
			moveRot.z = pChr->rot.z;
		}

		if ((!bShotInput)
			&&
			(bMoveInput))
		{// �V���b�g�̓��̓t���O���U�A���ړ��̓��̓t���O���^�̎��A
			// �ڕW�����̐ݒ�
			pChr->targetRot = moveRot;
		}
	}

	if (GetMouseTrigger(MOUSE_RIGHT)
		|| GetButtonTrigger(BUTTON_RIGHT_SHOULDER)
		|| GetButtonTrigger(BUTTON_RIGHT_TRIGGER))
	{// ���e�����͂��ꂽ���A
		if ((pChr->nBomb > 0)
			&& GetGameState() == MD_GAME_00_STATE_NORMAL)
		{// ���e���������Ă��āA���Q�[���̏�Ԃ��ʏ�̎��A
			// ���e�̏����������Z
			pChr->nBomb--;

			// ����[01] ��ݒ�
			SetAtk_explosion_01(pChr->pos, pChr->rot, ATK_EXPLOSION_01_TYPE_00);
		}
		else 
		{// ���e���������Ă��Ȃ��������A

		}
	}
}

//========================================
// DrawChr_fighter_00�֐� - �@��[00] �̕`�揈�� -
//========================================
void DrawChr_fighter_00(void)
{
	if (!g_chr_fighter_00.bUse)
	{// �g�p����Ă��Ȃ����A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffChr_fighter_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureChr_fighter_00[g_chr_fighter_00.nType]);

	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++)
	{
		// �@��[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
	}
}

//========================================
// SetChr_fighter_00�֐� - �@��[00] �̐ݒ菈�� -
//========================================
void SetChr_fighter_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_chr_fighter_00.bUse)
	{// �g�p����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_fighter_00->Lock(0, 0, (void**)&pVtx, 0);

	// �X�e�[�^�X����ނɉ����Đݒ�
	SetParameterChr_fighter_00();

	// �ʒu/�p�x����
	pChr->pos		= pos;
	pChr->rot		=
	pChr->targetRot = rot;

	// �o�����̏�Ԃɂ���
	pChr->aState		[CHR_FIGHTER_00_STATE_IN_POP]	= true;
	pChr->aCounterState	[CHR_FIGHTER_00_STATE_IN_POP]	= CHR_FIGHTER_00_POP_TIME;

	for (int nCntPolygon = 0; nCntPolygon < CHR_FIGHTER_00_POLYGON_NUM; nCntPolygon++, pVtx += 4)
	{
		D3DXVECTOR3 drawPos = pChr->pos;												// �`��ʒu
		D3DXVECTOR3 rot		= pChr->rot;												// ����
		float fWidth		= pChr->fWidth;												// ��
		float fHeight		= pChr->fHeight;											// ����
		float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
		float fAngle		= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

		// �`��ʒu��ݒ�
		MatchCameraPosition(&drawPos);

		// �o�����̃J�E���^�[�ɉ����Ċg��
		fWidth	*= (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));
		fHeight *= (1.0f + (pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP] * CHR_FIGHTER_00_POP_SIZE));

		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;

		{
			int nRed	= pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE];	// �ԓx
			int nAlpha	= pChr->aCounterState[CHR_FIGHTER_00_STATE_IN_POP];	// �����x

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
			pVtx[1].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
			pVtx[2].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
			pVtx[3].col = D3DCOLOR_RGBA(255, 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nRed * (255 / CHR_FIGHTER_00_DAMAGE_TIME)), 255 - (nAlpha * (255 / CHR_FIGHTER_00_POP_TIME)));
		}
	}

	// �ړ��ʂ�������
	pChr->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�p���Ă����Ԃɂ���
	pChr->bUse = true;

	// ���_���W���A�����b�N����
	g_pVtxBuffChr_fighter_00->Unlock();
}

//========================================
// BreakChr_fighter_00�֐� - �@��[00] �̔j�󏈗� -
//========================================
void BreakChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	// �Q�[���̏�Ԃ����S��̗]�C�ɂ���
	SetMd_game_00State(MD_GAME_00_STATE_DIED_AFTERGLOW);

	// �@��[00]���g�p���Ă��Ȃ���Ԃɂ���
	pChr->bUse = false;

	// ����[00] �̐ݒ菈��
	SetAtk_explosion_00(pChr->pos, pChr->rot, ATK_EXPLOSION_00_TYPE_06,0);

	// �S�Ă̓G[00] ����������
	RemoveAllChr_enemy_00();

	// �e[00] �̑S�j�󏈗�
	AllDestructionAtk_bullet_00(ATK_PARENT_TYPE_ENEMY);
	AllDestructionAtk_bullet_00(ATK_PARENT_TYPE_PLAYER);
}

//========================================
// SetParameter�֐� - �@��[00] �̃X�e�[�^�X����ނɉ����Đݒ� -
//========================================
void SetParameterChr_fighter_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	pChr->fWidth			= g_aChr_fighter_00Type[pChr->nType].fWidth;			// ��
	pChr->fHeight			= g_aChr_fighter_00Type[pChr->nType].fHeight;			// ����
	pChr->fHitTestWidth		= g_aChr_fighter_00Type[pChr->nType].fHitTestWidth;		// �����蔻��̕�
	pChr->fHitTestHeight	= g_aChr_fighter_00Type[pChr->nType].fHitTestHeight;	// �����蔻��̍���
	pChr->fMove				= g_aChr_fighter_00Type[pChr->nType].fMove;				// �ړ���
	pChr->nLife				= g_aChr_fighter_00Type[pChr->nType].nLife;				// �̗�
	pChr->nShotTime			= g_aChr_fighter_00Type[pChr->nType].nShotTime;			// �e�̔��˂ɂ����鎞��
}

//========================================
// HitChr_fighter_00�֐� - �@��[00] �̃q�b�g���� -
//========================================
void HitChr_fighter_00(int nDamage)
{
	if (nDamage <= 0) 
	{// �_���[�W��0�ȉ��̎��A
		// �������I��
		return;
	}

	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	// �̗͂���_���[�W�����Z
	pChr->nLife -= nDamage;

	// ��e�J�E���g�����Z
	pChr->nCntDamage++;

	// �J�����̐U����ݒ�
	SetCameraVibration(CHR_FIGHTER_00_DAMAGE_VIBRATION);

	// �X�N���[��[00] �̐ݒ菈��
	SetEff_screen_00(CHR_FIGHTER_00_DAMAGE_TIME, CHR_FIGHTER_00_DAMAGE_COLOR);

	if (pChr->nLife <= 0)
	{// �̗͂�0�ȉ��̎��A
		// �̗͂�0�ɂ���
		pChr->nLife = 0;

		// �@��[00] �̔j�󏈗�
		BreakChr_fighter_00();
	}
	else
	{// �̗͂�0�������Ă��鎞�A
		// �_���[�W��Ԃɂ���
		pChr->aState[CHR_FIGHTER_00_STATE_DAMAGE] = true;
		pChr->aCounterState[CHR_FIGHTER_00_STATE_DAMAGE] = CHR_FIGHTER_00_DAMAGE_TIME;

		// ���G��Ԃɂ���
		pChr->aState[CHR_FIGHTER_00_STATE_INVINCIBLE] = true;
		pChr->aCounterState[CHR_FIGHTER_00_STATE_INVINCIBLE] = CHR_FIGHTER_00_INVINCIBLE_TIME;

		// �_���[�WSE[000] ��炷
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);
	}
}

//========================================
// CollisionObj_plasma_wall_00�֐� - �v���Y�}�E�H�[��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionObj_plasma_wall_00(void)
{
	Chr_fighter_00 *pChr = &g_chr_fighter_00;	// �@��[00] �̏��̃|�C���^

	if (!pChr->bUse)
	{// �@��[00] ���g�p����Ă��Ȃ����A
		// �������I������
		return;
	}

	Obj_plasma_wall_00	*pObj = GetObj_plasma_wall_00();					// �v���Y�}�E�H�[��[00] �̏��
	Obj_plasma_block_00_square	*pSquare = GetObj_plasma_block_00_square();	// �v���Y�}�u���b�N[00] �̕`���l�p�`�̏��

	for (int nCntObj = 0; nCntObj < OBJ_PLASMA_WALL_00_MAX; nCntObj++, pObj++)
	{
		float fWidth, fHeight;	// ���ƍ���

		if ((int)((pObj->rot.z / D3DX_PI) * 10) % 10 == 0)
		{// �v���Y�}�E�H�[���̌������c�̎��A
			// ���ƍ�����ݒ�
			fWidth = OBJ_PLASMA_WALL_00_WIDTH;
			fHeight = pObj->fHeight;
		}
		else
		{// �v���Y�}�E�H�[���̌��������̎��A
			// ���ƍ�����ݒ�
			fHeight = OBJ_PLASMA_WALL_00_WIDTH;
			fWidth = pObj->fHeight;
		}

		if ((nCntObj == OBJ_PLASMA_WALL_00_TOP) && (pChr->pos.y - pChr->fHitTestWidth < pSquare->fTop + (fHeight * 0.5f)))
		{// ��̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pChr->pos.y = pSquare->fTop + (fHeight * 0.5f) + pChr->fHitTestWidth;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_BOTTOM) && (pChr->pos.y + pChr->fHitTestWidth > pSquare->fBottom - (fHeight * 0.5f)))
		{// ���̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pChr->pos.y = pSquare->fBottom - (fHeight * 0.5f) - pChr->fHitTestWidth;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_LEFT) && (pChr->pos.x - pChr->fHitTestWidth < pSquare->fLeft + (fWidth * 0.5f)))
		{// ���̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pChr->pos.x = pSquare->fLeft + (fWidth * 0.5f) + pChr->fHitTestWidth;
		}
		else if ((nCntObj == OBJ_PLASMA_WALL_00_RIGHT) && (pChr->pos.x + pChr->fHitTestWidth > pObj->pos.x - (fWidth * 0.5f)))
		{// �E�̃v���Y�}�E�H�[�����z���Ă������A
			// �ʒu���C��
			pChr->pos.x = pObj->pos.x - (fWidth * 0.5f) - pChr->fHitTestWidth;
		}
	}
}