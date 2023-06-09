//========================================
// 
// �Q�[���G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** mode_game-editor.h ***
//========================================
#ifndef _MODE_GAMEEDITOR_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MODE_GAMEEDITOR_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../../../../../_R.N.Lib/R.N.Lib.h"

//****************************************
// �N���X��`
//****************************************
// �Q�[���G�f�B�^�[�̏��\����
class Mode_GameEditor{
public:
	//========== *** �֐��錾 ***
	/* ���������� */void Init(void);
	/* �I������   */void Uninit(void);
	/* �X�V����   */void Update(void);

private:
	//========== *** �񋓌^��` ***
	// �Q�[���G�f�B�^�[�̃��[�h
	typedef enum {
		MODE_00_MODELEDIT,		// ���f���G�f�B�b�g
		MODE_MAX,
	}MODE;

	// ���f���G�f�B�b�g�̃��[�h
	typedef enum {
		MODE00_MODELSETUP,	// ���f���̃Z�b�g�A�b�v
		MODE00_HITTEST,		// �����蔻��쐬
		MODE00_MOTION,		// ���[�V�����쐬
		MODE00_MAX,
	}MODE00;

	//========== *** �֐��錾 ***
	/* ���i�̓ǂݍ��ݏ���                  */bool LoadPartsDataGameEditor(ModelSetUp3D *pModelSetUp, int nParts);
	/* ���f���̓ǂݍ��ݏ���                */void LoadModelDataGameEditor(void);
	/* ���f���̏������ݏ���                */void SaveModelDataGameEditor(void);
	/* ���f���̓ǂݍ��ݏ���                */void LoadMotionDataGameEditor(void);
	/* ���f���̏������ݏ���                */void SaveMotionDataGameEditor(void);
	/* �q���[�}���x�[�X���̓ǂݍ��ݏ���  */void LoadHumanBaseGameEditor(void);
	/* �q���[�}���x�[�X���̏������ݏ���  */void SaveHumanBaseGameEditor(void);
	/* ImGui�ݒ菈��                       */void ImGuiSetGameEditor(void);
	/* ImGui�ݒ菈��(���f���G�f�B�b�g)     */void ImGuiSetGameEditor_MODE00_MODELEDIT(void);
	/* ���i�ǉ�����                        */void PartsAddGameEditor(int *pPartsNum, ModelSetUp3D *pModelSetUp);
	/* ���i�폜����                        */void PartsSubGameEditor(int *pPartsNum, int nEditParts, Parts3DType *pParts, ModelSetUp3D *pModelSetUp);
	/* �����蔻��ǉ�����                  */void HitTestAddGameEditor(int *pHitTestNum, ModelSetUp3D *pModelSetUp);
	/* �����蔻��폜����                  */void HitTestSubGameEditor(int *pHitTestNum, int nEditHitTest, HitTest3D *pHitTest, ModelSetUp3D *pModelSetUp);
	/* �����蔻��̕��i�ǉ�����            */void HitTestPartsAddGameEditor(int *pPartsNum, int nEditHitTest, ModelSetUp3D *pModelSetUp);
	/* �����蔻��̕��i�폜����            */void HitTestPartsSubGameEditor(int *pPartsNum, int nEditParts, HitTest3DParts *pParts, ModelSetUp3D *pModelSetUp);

	//========== *** �ϐ��錾 ***
	bool m_bShowPartsIdx;	// ���i�̔ԍ��\��
	bool m_bShowHitTest;	// �����蔻��\���t���O
	bool m_bFrameByFrame;	// �R�}����t���O
	MODE m_mode;			// ���[�h

	// ���f���G�f�B�b�g
	MODE00 m_mode00;				// ���[�h
	char m_aNowSetUpPath[TXT_MAX] = "";	// ���݂̃Z�b�g�A�b�v�t�@�C���̃p�X
	unsigned int m_nModelSetUpIdx;	// ���f���̃Z�b�g�A�b�v(3D)�ԍ�
	int m_nSelectParts;				// �I�����Ă��镔�i
	int m_nTestEyeTex;				// �e�X�g���e�N�X�`��
									// ���f���\���e�N�X�`��
	int m_aModelPlatformTex[MODELPLATFORM_MAX - 1];
	bool m_bLoadModel;				// ���f����ǂݍ��߂����t���O
	CParts3D m_parts3D;				// ���i(3D)
};

#endif