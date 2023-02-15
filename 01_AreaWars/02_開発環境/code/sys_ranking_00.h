//========================================
// *** sys_ranking_00.h ***
//========================================
#ifndef _SYS_RANKING_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SYS_RANKING_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************

// �����L���O�̏��ʐ�
// �����L���O���̕�����
#define RANKING_NUM			(8)
#define RANKING_NAME_NUM	(3)

//****************************************
// �\���̂̒�`
//****************************************

// �����L���O�\����
typedef struct
{
	char	aName					//
			[RANKING_NAME_NUM + 1];	// ���O
	int		nScore;					// �X�R�A
}Sys_ranking_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ���ʂ̕\���`�����擾
char *GetRankText(int nRank);

// �V�X�e��:�����L���O[00] �̏���������
void InitSys_ranking_00(void);

// �V�X�e��:�����L���O[00] �̏I������
void UninitSys_ranking_00(void);

// �V�X�e��:�����L���O[00] �̍X�V����
void UpdateSys_ranking_00(void);

// �V�X�e��:�����L���O[00] �̏����擾
Sys_ranking_00 *GetSys_ranking_00(void);

// �V�X�e��:�����L���O[00] �̓ǂݍ��ݏ���
void LoadSys_ranking_00(void);

// �V�X�e��:�����L���O[00] �̏������ݏ���
void SaveSys_ranking_00(void);

// �����L���O�ɃX�R�A��}������
int SetScore(int nScore);

#endif