#if 0
//========================================
// 
// �}�b�v����(3D)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** map-generator3D.cpp ***
//========================================
// R.N.Lib
#include "../../../R.N.Lib.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** �}�b�v�Ǘ� ***
// �}�b�v�N���A����
void MapClear(void);

//****************************************
// �O���[�o���ϐ��錾
//****************************************
MapSet3D g_mapSet3D;	// �}�b�v�ݒ���(3D)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** �}�b�v�Ǘ� *** |------------------
//============================================================

//========================================
// MapClear�֐� - �}�b�v�N���A���� -
// Author:RIKU NISHIMURA
//========================================
void MapClear(void) 
{
	// �N���A
	ClearMapChip3D();	// �}�b�v�`�b�v(3D)

	// �t�H�O���g�p���Ă��Ȃ���Ԃɂ���
	GetFog()->bUse = false;
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
// InitMapGenerator3D�֐� - �}�b�v����(3D)�̏����� -
// Author:RIKU NISHIMURA
//========================================
void InitMapGenerator3D(void)
{
	// �}�b�v�ݒ���(3D)�̏�����
	g_mapSet3D = {};
}

//============================================================
//--------------------| *** �擾 *** |------------------------
//============================================================

//========================================
// GetMapSet�֐� - �}�b�v�ݒ���(3D)�̃|�C���^�擾 -
// Author:RIKU NISHIMURA
//========================================
MapSet3D *GetMapSet(void)
{
	return &g_mapSet3D;
}

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// LoadMapData�֐� - �}�b�v���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMapData(char *pPath)
{
	FILE *pFile;				// �t�@�C���̃|�C���^
	char aDataSearch[TXT_MAX] = "";	// �����p������
	
	// �t�@�C�����J��
	pFile = fopen(pPath, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ�A
		// �}�b�v���N���A
		MapClear();

		// �}�b�v�ݒ���(3D)�̏�����
		g_mapSet3D = {};

		while (DataSearch(pFile,aDataSearch, "END"))
		{
			if (!strcmp(aDataSearch, "MAPCHIP3DSET"))	// �}�b�v�`�b�v(3D)�ݒ���ǂݍ���
			{
				// �}�b�v�`�b�v(3D)�ݒ���̃|�C���^
				MapChip3DSet *pMapChip3DSet = g_mapSet3D.aMapChip3DSet;

				// �}�b�v�`�b�v(3D)�ݒ���
				while (DataSearch(pFile, aDataSearch, "MAPCHIP3DSET_END"))
				{
					if (!strcmp(aDataSearch, "SET")) 
					{
						while (DataSearch(pFile, aDataSearch, "SET_END"))
						{
							if (false) {}
							else if (!strcmp(aDataSearch, "POS"))	{ fscanf_D3DXVECTOR3(pFile, &pMapChip3DSet->pos); }	// �F
							else if (!strcmp(aDataSearch, "ROT"))	{ fscanf_D3DXVECTOR3(pFile, &pMapChip3DSet->rot); }	// �J�n����
							else if (!strcmp(aDataSearch, "TYPE"))	{ fscanf_int(pFile, &pMapChip3DSet->nType); }		// �I������
						}

						SetMapChip3D(	// �}�b�v�`�b�v(3D)��ݒ�
							pMapChip3DSet->pos,
							pMapChip3DSet->rot,
							pMapChip3DSet->nType,
							0);
						pMapChip3DSet++;
						g_mapSet3D.nMapChip3DNum++;
					}
				}
			}
			else if (!strcmp(aDataSearch, "FOGSET"))	// �t�H�O�ݒ���ǂݍ���
			{
				// �t�H�O���g�p���Ă����Ԃɂ���
				GetFog()->bUse = true;

				FogSet fogSet;	// �t�H�O�̐ݒ���

				while (DataSearch(pFile, aDataSearch, "FODSET_END"))
				{
					if (false) {}
					else if (!strcmp(aDataSearch, "COLOR")) { fscanf_Color(pFile, &fogSet.col); }	// �F
					else if (!strcmp(aDataSearch, "START")) { fscanf_int(pFile, &fogSet.nStart); }	// �J�n����
					else if (!strcmp(aDataSearch, "END"))	{ fscanf_int(pFile, &fogSet.nEnd); }	// �I������
				}

				// �t�H�O��ݒ�
				SetFog(fogSet);

				// �t�H�O���g�p���Ă����Ԃɂ���
				GetFog()->bUse = true;

				// �t�H�O�̐ݒ�����擾
				g_mapSet3D.fogSet = fogSet;
			}
		}
		
		 // �t�@�C�������
		fclose(pFile);
	}
}

//========================================
// SaveMapData�֐� - �}�b�v���̏������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void SaveMapData(char *pPath)
{
	FILE *pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen(pPath, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ�A
		{// �}�b�v�`�b�v(3D)�ݒ������������
			MapChip3DSet *pMapChip3DSet = g_mapSet3D.aMapChip3DSet;

			fprintf(pFile, "MAPCHIP3DSET\n");
			for (int nCntMapChip3D = 0; nCntMapChip3D < g_mapSet3D.nMapChip3DNum; nCntMapChip3D++, pMapChip3DSet++)
			{
				fprintf(pFile, "	SET\n");
				fprintf(pFile, "		POS %f %f %f\n", pMapChip3DSet->pos.x, pMapChip3DSet->pos.y, pMapChip3DSet->pos.z);
				fprintf(pFile, "		ROT %f %f %f\n", pMapChip3DSet->rot.x, pMapChip3DSet->rot.y, pMapChip3DSet->rot.z);
				fprintf(pFile, "		TYPE %d\n", pMapChip3DSet->nType);
				fprintf(pFile, "	SET_END\n");
			}
			fprintf(pFile, "MAPCHIP3DSET_END\n");

			fprintf(pFile, "END\n");
		}

		// �t�@�C�������
		fclose(pFile);
	}
}
#endif