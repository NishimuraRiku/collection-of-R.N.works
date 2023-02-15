//========================================
// 
// メイン処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
// Character
#include "character.h"
// Object
#include "object.h"
// _R.N.Lib
#include "../R.N.Lib\RNmain.h"

//****************************************
// マクロ定義
//****************************************
// ウインドウの名前
#define WINDOW_NAME	"R.N.ModelViewer"

//****************************************
// プロトタイプ宣言
//****************************************
// ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
// 終了処理
void Uninit(void);
// 更新処理
void Update(void);
// 描画処理
void Draw(void);
// 読み込み処理
void Load(void);
// 書き込み処理
void Save(void);
// ウインドウの表示/非表示を切り替える
void ShowTaskBar(bool bShow);

//****************************************
// グローバル変数宣言
//****************************************
Main g_main;	// メイン処理の情報構造体

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// WinMain関数
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	MSG msg;				// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	// 乱数の種を取得
	srand((unsigned int)time(0));

	// ウインドウクラスの構造体
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウインドウのスタイル
		WindowProc,						// ウインドウプロシージャ
		0,								// 0にする
		0,								// 0にする
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		CLASS_NAME,						// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// ウインドウを生成
	g_main.hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		CLASS_NAME,						// ウインドウクラスの名前
		WINDOW_NAME,					// ウインドウの名前
		WS_SYSMENU | WS_MINIMIZEBOX,	// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// ウインドウの左上Y座標
		SCREEN_WIDTH,					// ウインドウの幅
		SCREEN_HEIGHT,					// ウインドウの高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, g_main.hWnd, TRUE)))
	{// 初期化処理が失敗した場合

		return -1;
	}

	// ウインドウの表示
	WindowModeChange(false);

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0; // 現在時刻を初期化する
	dwExecLastTime = timeGetTime(); // 現在時刻を取得(保存)

	dwFrameCount = 0;	// フレームカウントを初期化する
	dwFPSLastTime = timeGetTime();	// 現在時刻を取得(保存)

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取った時、メッセージループを抜ける
				break;
			}
			else {
				// メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime(); // 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				g_main.nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime; // FPSを測定した時刻を保存
				dwFrameCount = 0; // フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60/1秒経過
				dwExecLastTime = dwCurrentTime; // 処理開始の時刻[現在時刻]を保存

				{
					RECT rec;	// 大きさを受け取るRECT構造体

					// ウインドウの情報を取得
					GetWindowRect(g_main.hWnd, &rec);

					// ウインドウの位置を設定
					g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
				}

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++; // フレームカウントを加算
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
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
		SetFocus(g_main.hWnd);
	}
	else
	{// 非表示
		// タスクバーを非表示
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowProc関数
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// 返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY: // ウインドウ破棄のメッセージ

		PostQuitMessage(0); // WM_QUITメッセージを送る

		break;
	case WM_KEYDOWN: // キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]キーが押された

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO); // ウインドウを破棄する(WM_DESTROYメッセージを送る)

			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // ウインドウを破棄する
			}

			break;
		}

		break;
	case WM_CLOSE: // 終了ボタンが押された時

		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO); // ウインドウを破棄する(WM_DESTROYメッセージを送る)

		if (nID == IDYES)
		{
			DestroyWindow(hWnd); // ウインドウを破棄する
		}
		else
		{
			return 0; // (※0を返さないと終了してしまう)
		}

		break;
	case WM_LBUTTONDOWN: // マウスクリックのメッセージ

		// 対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd); // マウスを左クリックしたウインドウをアクティブにする
		break;
	case WM_MOUSEWHEEL: // マウスホイールが回された時のメッセージ
		if (HIWORD(wParam) == WHEEL_DELTA)
		{
			GetCamera3D()->fLength -= CAMERA3D_ZOOM_LENGTH;
		}
		else
		{
			GetCamera3D()->fLength += CAMERA3D_ZOOM_LENGTH;
		}

		break;
	case WM_CREATE: //ウインドウ生成のメッセージ

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を返す
}

//========================================
// Init関数 - 初期化処理 -
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 背景色を初期化
	g_main.bgCol = INITBGCOLOR;

	// Direct3Dオブジェクトの生成
	g_main.pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_main.pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;	// ディスプレイモード

	// 現在のディスプレイモードを取得
	if (FAILED(g_main.pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// デバイスのプレゼンテーションパラメータを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲームの画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲームの画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_main.pD3DDevice)))
	{
		if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_main.pD3DDevice)))
		{
			if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_main.pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_main.pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステート設定
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// アルファブレンドの設定
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 読み込み処理
	Load();

	// R.N.Libの初期化処理
	RNInit(hInstance);

	return S_OK;
}

//========================================
// Uninit関数 - 終了処理 -
//========================================
void Uninit(void)
{
	// 書き込み処理
	Save();

	// R.N.Libの終了処理
	RNUninit();

	// Direct3Dデバイスの破棄
	if (g_main.pD3DDevice != NULL)
	{
		g_main.pD3DDevice->Release();
		g_main.pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_main.pD3D != NULL)
	{
		g_main.pD3D->Release();
		g_main.pD3D = NULL;
	}

	// タスクバーを表示
	ShowTaskBar(true);
}

//========================================
// Update関数 - 更新処理 - 
//========================================
void Update(void)
{
	// 更新処理
	RNUpdate();	// R.N.Lib
}

//========================================
// Draw関数 - 描画処理 -
//========================================
void Draw(void)
{
	// 画面クリア
	g_main.pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DCOLOR(g_main.bgCol), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_main.pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合、
		SetCamera3D();	// カメラ(3D)の設定処理
		RNDraw();		// R.N.Lib

		// 描画終了
		g_main.pD3DDevice->EndScene();
	}

	// バックバッファをフロントバッファの入れ替え
	g_main.pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// Load関数 - 読み込み処理 -
//========================================
void Load(void)
{
	LoadCharacter();	// キャラクター
	LoadObject();		// オブジェクト
	RNLoad();			// R.N.Lib
}

//========================================
// Save関数 - 書き込み処理 -
//========================================
void Save(void)
{
	SaveSetting();	// 設定
	RNSave();		// R.N.Lib
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// GetMain関数 - メイン処理の情報の取得 -
//========================================
Main *GetMain(void)
{
	return &g_main;
}

//========================================
// GetDevice関数 - デバイスの取得 -
//========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_main.pD3DDevice;
}

//========================================
// GetWindowHandle関数 - ウインドウハンドルを取得 -
//========================================
HWND *GetWindowHandle(void)
{
	return &g_main.hWnd;
}

//========================================
// GetWindowPos関数 - ウインドウの位置を取得 -
//========================================
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}

//========================================
// WindowModeChange関数 - ウインドウのモードを切り替える -
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
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_main.hWnd);

	// タスクバーを表示/非表示
	ShowTaskBar(bFullScreen ^ 1);
}

//========================================
// Message関数 - メッセージ処理 -
//========================================
void Message(char *pText, char *pCaption)
{
	static int	bMessage = false;	// メッセージ表示完了フラグ(無限ループ防止)

	if (bMessage)
	{// メッセージ表示完了フラグ
		bMessage = false;	// メッセージ表示完了フラグを偽にする
		return;				// 処理を終了する
	}

	if (MessageBox(*GetWindowHandle(), pText, pCaption, MB_OK) == IDOK)
	{// メッセージボックスの返り値がOKの時、
		// メッセージ表示完了フラグを真にする
		bMessage = true;
	}
}