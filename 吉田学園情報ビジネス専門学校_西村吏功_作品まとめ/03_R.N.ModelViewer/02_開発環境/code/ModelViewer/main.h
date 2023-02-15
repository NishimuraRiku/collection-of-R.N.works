//========================================
// 
// ���C���̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.h ***
//========================================
#ifndef _MAIN_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						// ���͏����ɕK�v
#include "Xinput.h"						// ���͏����ɕK�v
#include "xaudio2.h"					// �T�E���h�����ɕK�v

//****************************************
// ���C�u�����̃����N
//****************************************
#pragma comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g���g�p����̂ɕK�v
#pragma comment(lib,"dsound.lib")	// DirectSound���g�p����̂ɕK�v
#pragma comment(lib,"winmm.lib")	// �V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	// ���͏����ɕK�v
#pragma warning(disable:4091)		// �x��:C4091�𖳎�����
#pragma warning(disable:4244)		// �x��:C4244�𖳎�����

//****************************************
// �}�N����`
//****************************************
// �E�C���h�E�N���X
#define CLASS_NAME "WindowClass"
// �E�C���h�E�̕�
#define SCREEN_WIDTH (960)	// NORMAL=1280 4:3=960
// �E�C���h�E�̍���
#define SCREEN_HEIGHT (720)
// ���_�t�H�[�}�b�g(2D)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
// ���_�t�H�[�}�b�g(3D)
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
// ������̍ő吔(�ėp)
#define TXT_MAX (256)

// �E�C���h�E�̒��S���WX
#define SCREEN_CENTER_X	(SCREEN_WIDTH*0.5f)
// �E�C���h�E�̒��S���WY
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT*0.5f)

// �����X�N���[���̍��[
#define INSIDE_SCREEN_LEFTMOST (960)
// �����X�N���[���̉E�[
#define INSIDE_SCREEN_RIGHTMOST (720)

// D3DXVECTOR3�̏������l
#define INITD3DXVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)
// Color�̏������l
#define INITCOLOR Color{255,255,255,255}
// �w�i�F�̏����l
#define INITBGCOLOR Color{58,58,58,255}

//****************************************
// �\���̂̒�`
//****************************************
// ���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_2D;
// ���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_3D;
// �g��{���\����
typedef struct 
{
	float fWidth;	// ��
	float fHeight;	// ����
	float fDepth;	// ���s��
}Scale;
// �F�\����
typedef struct 
{
	int r;
	int g;
	int b;
	int a;
}Color;
// ���C�������̏��\����
typedef struct 
{
	// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3D9 pD3D = NULL;
	// Direct3D�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	HWND hWnd;				// �E�C���h�E�n���h��(���ʎq)
	bool bPause = false;	// �|�[�Y�t���O
	Color bgCol;			// �w�i�F
	int nCountFPS = 0;		// FPS�̃J�E���^�[
	D3DXVECTOR3 windowPos;	// �E�C���h�E�̈ʒu
}Main;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���C�������̏��̎擾
Main *GetMain(void);
// �f�o�C�X���擾
LPDIRECT3DDEVICE9 GetDevice(void);
// �E�C���h�E�n���h�����擾
HWND *GetWindowHandle(void);
// �E�C���h�E�̈ʒu���擾
D3DXVECTOR3 GetWindowPos(void);
// �E�C���h�E�̃��[�h��؂�ւ���
void WindowModeChange(bool bFullScreen);
// ���b�Z�[�W����
void Message(char *pText, char *pCaption);

#endif