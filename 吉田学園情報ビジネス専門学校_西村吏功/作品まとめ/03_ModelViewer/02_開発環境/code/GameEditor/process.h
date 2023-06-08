//========================================
// 
// ��{�����N���X�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** process.h ***
//========================================
#ifndef _PROCESS_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _PROCESS_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "../../../../_R.N.Lib/R.N.Lib.h"

// Mode
#include "Mode\mode_game-editor.h"

//****************************************
// �}�N����`
//****************************************
#define SCREEN_WIDTH            (1280)					// �E�C���h�E�̕�(NORMAL=1280 4:3=960)
#define SCREEN_HEIGHT           (720)					// �E�C���h�E�̍���
#define SCREEN_CENTER_X         (SCREEN_WIDTH*0.5f)		// �E�C���h�E�̒��S���WX
#define SCREEN_CENTER_Y         (SCREEN_HEIGHT*0.5f)	// �E�C���h�E�̒��S���WY
#define INSIDE_SCREEN_LEFTMOST  (960)					// �����X�N���[���̍��[
#define INSIDE_SCREEN_RIGHTMOST (720)					// �����X�N���[���̉E�[
#define DEFAULTBGCOLOR          Color{58,58,58,255}		// �w�i�F

//****************************************
// �N���X��`
//****************************************
// ���C�������N���X
class CProcess {
public:
	//========== *** �֐��錾 ***
	/* �R���X�g���N�^ */CProcess();
	/* �f�X�g���N�^   */~CProcess();
	/* ���C�����[�v   */int MainLoop(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow);

private:
	//========== *** �񋓌^��` ***
	// ���[�h(���)�̎��
	typedef enum {
		MODENONE = -1,	// ����
		MODE_MAIN,
		MODE_MAX,
	}MODE;

	//========== *** �֐��錾 ***
	/* ���[�h�ɉ��������������� */void ModeInit(void);
	/* ���[�h�ɉ������I������   */void ModeUninit(void);
	/* ���[�h�ɉ������X�V����   */void ModeUpdate(void);
	/* ����������               */HRESULT Init(HINSTANCE hInstance);
	/* �I������                 */void Uninit(void);
	/* �X�V����                 */void Update(void);
	/* �`�揈��                 */void Draw(void);
	/* �ǂݍ��ݏ���             */void Load(void);
	/* �������ݏ���             */void Save(void);
	/* ���[�h�̐ݒ菈��         */void SetMode(MODE newMode);

	//========== *** �ϐ���` ***
	int m_nCountFPS;	// FPS�̃J�E���^�[
	MODE m_mode;		// ���[�h

	//========== *** ���[�h�C���X�^���X ***
	Mode_GameEditor mode_gameEditor;
};

#endif