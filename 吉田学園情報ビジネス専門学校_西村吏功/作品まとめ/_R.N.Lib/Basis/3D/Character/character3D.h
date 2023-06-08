//========================================
// 
// �L�����N�^(3D)�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** character3D.h ***
//========================================
#ifndef _CHARACTER3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHARACTER3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../Body/parts3D_class.h"

//****************************************
// �N���X��`
//****************************************
// �L�����N�^(3D)�N���X
class CChr3D {
public:
	//========== *** �񋓌^��` ***
	// ���
	typedef enum {
		TYPE_STANDERD,	// �X�^���_�[�h�^
		TYPE_HUMAN,		// �q���[�}���^
		TYPE_MAX,
	}TYPE;

	//========== *** �֐��錾 ***
	/* �R���X�g���N�^ */CChr3D();
	/* �f�X�g���N�^   */virtual ~CChr3D();
	/* ����������     */virtual void Init(char *pBodyPath) = 0;

protected:
	//========== *** �ϐ��錾 ***
	int m_nModelSetUpIdx;	// ���f���̃Z�b�g�A�b�v�ԍ�
	CParts3D m_parts3D;		// ���i(3D)�̃C���X�^���X
};

// �X�^���_�[�h�^�L�����N�^(3D)�N���X
class CChr3D_Standard :public CChr3D {
public:
	//========== *** �֐��錾 ***
	/* �R���X�g���N�^ */CChr3D_Standard();
	/* �f�X�g���N�^   */~CChr3D_Standard();
	/* ����������     */void Init(char *pBodyPath);

};

// �q���[�}���^�L�����N�^(3D)�N���X
class CChr3D_Human :public CChr3D {
public:
	//========== *** �֐��錾 ***
	/* �R���X�g���N�^ */CChr3D_Human();
	/* �f�X�g���N�^   */~CChr3D_Human();
	/* ����������     */void Init(char *pBodyPath);

	//========== *** �ϐ��錾 ***
	int m_nIdxHumanBase;	// �q���[�}���x�[�X�ԍ�
};

#endif