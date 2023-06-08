//========================================
// 
// ウィンドウの処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** window.cpp ***
//========================================
// R.N.Lib
#include "../../R.N.Lib.h"

//****************************************
// 構造体定義
//****************************************
// ウインドウの管理情報構造体
typedef struct
{
	D3DXVECTOR2 pos;	// 位置
}WindowInfo;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 設定 ***
// Direct3Dデバイスの生成
int CreateDirect3DDevice(CreateWindowInfo cwi);

//****************************************
// グローバル変数定義
//****************************************
// ウィンドウの管理情報
static WindowInfo g_windowInfo;
// ウィンドウの記憶情報
static WindowMemory g_windowMemory;
// Direct3Dオブジェクトへのポインタ
static LPDIRECT3D9 g_pD3D;
// Direct3Dデバイスへのポインタ
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
static HWND g_hWnd;	// ウインドウハンドル(識別子)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// CreateDirect3DDevice関数 - Direct3Dデバイスの生成 -
// Author:RIKU NISHIMURA
//========================================
int CreateDirect3DDevice(CreateWindowInfo cwi)
{
	// ウィンドウの情報を記憶
	g_windowMemory.hInstance = cwi.hInstance;	// インスタンスハンドル
	g_windowMemory.pClassName = cwi.pClassName;	// ウィンドウクラスの名前
	g_windowMemory.fWindowWidth = cwi.fWindowWidth;		// ウィンドウの幅
	g_windowMemory.fWindowHeight = cwi.fWindowHeight;	// ウィンドウの高さ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{// Direct3Dオブジェクトの生成に失敗
		return -1;
	}

	D3DDISPLAYMODE d3ddm;	// ディスプレイモード

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// デバイスのプレゼンテーションパラメータを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // パラメータのゼロクリア

	d3dpp.BackBufferWidth = cwi.fWindowWidth * IMGUI_WINDOW_CORRECT_X;		// ゲームの画面サイズ(幅)
	d3dpp.BackBufferHeight = cwi.fWindowHeight * IMGUI_WINDOW_CORRECT_Y;	// ゲームの画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = cwi.bWindow;								// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			g_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				g_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return -1;
			}
		}
	}

	return 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// UpdateWindow関数 - ウインドウの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateWindowRN(void)
{
	RECT rec;	// 大きさを受け取るRECT構造体

	// ウインドウの情報を取得
	GetWindowRect(g_hWnd, &rec);

	// ウインドウの位置を設定
	g_windowInfo.pos = D3DXVECTOR2(rec.left, rec.top);
}

//========================================
// UninitWindow関数 - ウインドウの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitWindow(void)
{
	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// ウインドウクラスの登録を解除
	UnregisterClass(g_windowMemory.pClassName, g_windowMemory.hInstance);
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================

//========================================
// GetWindowMemory関数 - ウィンドウの記憶情報を取得 -
//========================================
WindowMemory *GetWindowMemory(void)
{
	return &g_windowMemory;
}

//========================================
// GetD3DDevice関数 - Direct3Dデバイスを取得 -
// Author:RIKU NISHIMURA
//========================================
LPDIRECT3DDEVICE9 GetD3DDevice(void) 
{
	return g_pD3DDevice;
}

//========================================
// GetWindowHandle関数 - ウインドウハンドルを取得 -
// Author:RIKU NISHIMURA
//========================================
HWND *GetWindowHandle(void) 
{
	return &g_hWnd;
}

//========================================
// GetWindowPos関数 - ウインドウの位置を取得 -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR2 GetWindowPos(void)
{
	return g_windowInfo.pos;
}

//============================================================
//--------------------| *** 設定 *** |------------------------
//============================================================

//========================================
// CreateWindowRN関数 - ウインドウ作成 -
// Author:RIKU NISHIMURA
//========================================
int CreateWindowRN(CreateWindowInfo cwi)
{
	// ウインドウクラスの構造体
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウインドウのスタイル
		cwi.lpfnWndProc,				// ウインドウプロシージャ
		0,								// 0にする
		0,								// 0にする
		cwi.hInstance,					// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		cwi.pClassName,					// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// ウインドウを生成
	g_hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		cwi.pClassName,					// ウインドウクラスの名前
		cwi.pWindowName,				// ウインドウの名前
		WS_SYSMENU | WS_MINIMIZEBOX,	// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// ウインドウの左上Y座標
		cwi.fWindowWidth,				// ウインドウの幅
		cwi.fWindowHeight,				// ウインドウの高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは
		cwi.hInstance,					// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// Direct3Dデバイスの生成
	if (CreateDirect3DDevice(cwi) == -1) 
	{// 生成に失敗
		return -1;
	}

	// ウィンドウのモードを設定する
	WindowModeChange(cwi.bWindow ^ 1);

	return 0;
}

//============================================================
//--------------------| *** 状態 *** |------------------------
//============================================================

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
// Author:RIKU NISHIMURA
// 参考: https://dixq.net/forum/viewtopic.php?t=12505
//========================================
void ShowTaskBar(bool bShow)
{
	HWND hTask;	// タスクバーのハンドル

	// タスクバーのハンドルを取得
	hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// 表示
		// タスクバーを表示
		ShowWindow(hTask, SW_SHOW);

		// ウインドウをアクティブにする
		SetFocus(g_hWnd);
	}
	else
	{// 非表示
		// タスクバーを非表示
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowModeChange関数 - ウインドウのモードを切り替える -
// Author:RIKU NISHIMURA
//========================================
void WindowModeChange(bool bFullScreen)
{
	HDC hDC = GetDC(GetDesktopWindow());			// デスクトップのハンドル
	int nScrSizeX = GetDeviceCaps(hDC, HORZRES);	// デスクトップの画面サイズX
	int nScrSizeY = GetDeviceCaps(hDC, VERTRES);	// デスクトップの画面サイズY

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen)
	{// 全画面フラグが真の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (g_windowMemory.fWindowWidth * 0.5f),	// 位置を中央に合わせる
			(nScrSizeY * 0.5f) - (g_windowMemory.fWindowHeight * 0.5f),
			g_windowMemory.fWindowWidth,
			g_windowMemory.fWindowHeight,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_hWnd);

	// タスクバーを表示/非表示
	ShowTaskBar(bFullScreen ^ 1);
}

//============================================================
//--------------------| *** その他 *** |----------------------
//============================================================

//========================================
// Message関数 - メッセージ処理 -
// Author:RIKU NISHIMURA
//========================================
void Message(char *pText, char *pCaption) {
	MessageBox(*GetWindowHandle(), pText, pCaption, MB_OK);
}

//========================================
// IsWindowFocused関数 - ウィンドウにフォーカスが当たっているか調べる -
// Author:RIKU NISHIMURA
//========================================
bool IsWindowFocused(HWND hwnd)
{
	HRESULT hr = g_pD3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
	{
		// デバイスが失われている場合はフォーカスが当たっていないとみなす
		return false;
	}

	HWND activeWindow = GetForegroundWindow();

	return (activeWindow == hwnd);
}