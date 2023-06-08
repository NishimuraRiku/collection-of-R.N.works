//========================================
// 
// ウィンドウのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** window.h ***
//========================================
#ifndef _WINDOW_H_	// このマクロ定義がされていなかった時
#define _WINDOW_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define IMGUI_WINDOW_CORRECT_X (0.98775f)	// ImGui用のウィンドウ領域補正値X
#define IMGUI_WINDOW_CORRECT_Y (0.94735f)	// ImGui用のウィンドウ領域補正値Y
// (※ImGuiが当たり判定の際にDirect3Dデバイスの画面サイズを参照しており、誤差を埋める為の数値)

//****************************************
// 構造体定義
//****************************************
// ウィンドウ作成情報構造体
typedef struct 
{
	WNDPROC lpfnWndProc;	// ウィンドウプロシージャ
	HINSTANCE hInstance;	// インスタンスハンドル
	char *pClassName;		// ウィンドウクラスの名前
	char *pWindowName;		// ウィンドウの名前
	float fWindowWidth;		// ウィンドウの幅
	float fWindowHeight;	// ウィンドウの高さ
	bool bWindow;			// ウィンドウモード
	// (※TRUEならウィンドウモード、FALSEならフルスクリーンモードで初期化)
}CreateWindowInfo;

// ウィンドウの記憶情報構造体
typedef struct
{
	HINSTANCE hInstance;	// インスタンスハンドル
	char *pClassName;		// ウィンドウクラスの名前
	float fWindowWidth;		// ウィンドウの幅
	float fWindowHeight;	// ウィンドウの高さ
}WindowMemory;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// ウィンドウの更新処理
void UpdateWindowRN(void);
// ウィンドウの終了処理
void UninitWindow(void);
//========== *** 取得 ***
// ウィンドウの記憶情報を取得
WindowMemory *GetWindowMemory(void);
// Direct3Dデバイスを取得
LPDIRECT3DDEVICE9 GetD3DDevice(void);
// ウインドウハンドルを取得
HWND *GetWindowHandle(void);
// ウインドウの位置を取得
D3DXVECTOR2 GetWindowPos(void);
//========== *** 設定 ***
// ウインドウ作成
int CreateWindowRN(CreateWindowInfo cwi);
//========== *** 状態 ***
// タスクバーの表示/非表示を切り替える
void ShowTaskBar(bool bShow);
// ウインドウのモードを切り替える(true:Full false:Normal)
void WindowModeChange(bool bFullScreen);
// メッセージ処理
void Message(char *pText, char *pCaption);
// ウィンドウにフォーカスが当たっているか調べる
bool IsWindowFocused(HWND hwnd);

#endif