//========================================
// 
// ImGui�̃��C�������̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** imgui_main.h ***
//========================================
#ifndef _IMGUI_MAIN_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _IMGUI_MAIN_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �\���̂̐錾
//****************************************
// ImGui�̃E�B���h�E�ݒ���
typedef struct 
{
	char aName[TXT_MAX] = "";	// ���O
	float fWidth = 0.0f;	// ��
	float fHeight = 0.0f;	// ����
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// �ʒu
#if /* �ݒ�t�H�[�}�b�g */0
	// ImGui�̃E�B���h�E�ݒ���
	ImGuiWindowSet winSet;
	/* ���O */sprintf(winSet.aName, "");
	/* ��   */winSet.fWidth = ;
	/* ���� */winSet.fHeight = ;
	/* �ʒu */winSet.pos = ;
	// ImGui�̃E�B���h�E�ݒ菈��
	if (ImGui_SetWindow(winSet))
#endif
}ImGuiWindowSet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
//========== *** ��{�ڑ� ***
// ImGui�̏���������
void ImGuiInit(HWND hWnd);
// ImGui�̏I������
void ImGuiUninit(void);
// ImGui�̃t���O����������
void ImGuiInitFlag(void);
// ImGui�̕`�揈��
void ImGuiDraw(void);
//========== *** �����֘A ***
// ImGui�̐����J�n����
void ImGui_CreateStart(void);
// ImGui�̐����I������
void ImGui_CreateEnd(void);
// ImGui�̃E�B���h�E�ݒ菈��
bool ImGui_SetWindow(ImGuiWindowSet winSet);
// ImGui�̃E�B���h�E�I������
bool ImGui_EndWindow(void);
// ImGui�̃w�b�_�ݒ菈��
bool ImGui_SetHeader(char *pName);
//========== *** �擾 ***
// ImGui�̃E�B���h�E�Ƀt�H�[�J�X���������Ă��邩�擾
bool ImGui_GetWindowFocused(void);

//****************************************
// �����T���v��
//****************************************
#if 0
{
	// ImGui�̐����J�n
	ImGui_CreateStart();

	// ImGui�̃E�B���h�E�ݒ���
	ImGuiWindowSet winSet;
	/* ���O */sprintf(winSet.aName, u8" ");
	/* ��   */winSet.fWidth = ;
	/* ���� */winSet.fHeight = ;
	/* �ʒu */winSet.pos = ;
	// ImGui�̃E�B���h�E�ݒ菈��
	if (ImGui_SetWindow(winSet)) {
		do {
			if (ImGui_SetHeader(u8" ")) 
			{// ��
				if (ImGui_SetTreeNode(u8" ")) {
					do {// ��

					} while (!ImGui_EndTreeNode());
				}
			}
		} while (!ImGui_EndWindow());
	}

	// ImGui�̐����I��
	ImGui_CreateEnd();
}
#endif

#endif