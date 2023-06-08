//========================================
// 
// 基本処理クラス
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "process.h"
#include "../../../../_R.N.Lib/RNmain.h"

//****************************************
// マクロ定義
//****************************************
#define WINDOW_NAME	"R.N.ModelViewer"		// ウインドウの名前
#define CLASS_NAME  "WindowClass"			// ウインドウクラス
#define INIT_MODE   (CProcess::MODE_MAIN)	// 初期のモード

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Processクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CProcess::CProcess(void) {
	m_nCountFPS = 0;	// FPSのカウンター
	m_mode = MODENONE;	// モード
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CProcess::~CProcess(void) {

}

//========================================
// ModeInit関数 - モードに応じた初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::ModeInit(void) {
	switch (m_mode) {
	case MODE_MAIN:mode_gameEditor.Init(); break;
	}
}

//========================================
// ModeUninit関数 - モードに応じた終了処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::ModeUninit(void) {
	switch (m_mode) {
	case MODE_MAIN:mode_gameEditor.Uninit(); break;
	}
}

//========================================
// ModeUpdate関数 - モードに応じた更新処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::ModeUpdate(void) {
	switch (m_mode) {
	case MODE_MAIN:mode_gameEditor.Update(); break;
	}
}

//========================================
// MainLoop関数 - メインループ -
// Author:RIKU NISHIMURA
//========================================
int CProcess::MainLoop(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	MSG msg;				// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	// ウィンドウ生成情報
	CreateWindowInfo cwi = {
		WindowProc,		// ウィンドウプロシージャ
		hInstance,		// インスタンスハンドル
		CLASS_NAME,		// ウィンドウクラスの名前
		WINDOW_NAME,	// ウィンドウの名前
		SCREEN_WIDTH,	// ウィンドウの幅
		SCREEN_HEIGHT,	// ウィンドウの高さ
		true,			// ウィンドウモード
	};
	CreateWindowRN(cwi);

	// 初期化処理
	if (FAILED(Init(hInstance))) {
		return -1;	// 初期化処理が失敗した場合
	}

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0; // 現在時刻を初期化する
	dwExecLastTime = timeGetTime(); // 現在時刻を取得(保存)

	dwFrameCount = 0;	// フレームカウントを初期化する
	dwFPSLastTime = timeGetTime();	// 現在時刻を取得(保存)

	// メッセージループ
	while (1) {
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
				m_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;

				// フレームカウントをクリア
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60/1秒経過
				Update();	// 更新処理
				Draw();		// 描画処理

				// 処理開始の時刻(現在時刻)を保存
				dwExecLastTime = dwCurrentTime;

				// フレームカウントを加算
				dwFrameCount++;
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//========================================
// Init関数 - 初期化処理 -
// Author:RIKU NISHIMURA
//========================================
HRESULT CProcess::Init(HINSTANCE hInstance) {
	Load();	// 読み込み処理
	RNLib::Init(hInstance);	// R.N.Libの初期化処理
	SetMode(INIT_MODE);	// モードを設定する
	SetBGColor(DEFAULTBGCOLOR);	// 背景色を設定

	return S_OK;
}

//========================================
// Uninit関数 - 終了処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Uninit(void) {
	Save();	// 書き込み処理
	ShowTaskBar(true);	// タスクバーを表示

	// 終了処理
	RNLib::Uninit();		// R.N.Lib
	ModeUninit();	// モード
}

//========================================
// Update関数 - 更新処理 - 
// Author:RIKU NISHIMURA
//========================================
void CProcess::Update(void) {
	RNLib::Update();		// R.N.Lib
	ModeUpdate();	// モード
}

//========================================
// Draw関数 - 描画処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Draw(void) {
	RNLib::Draw();	// R.N.Lib
}

//========================================
// Load関数 - 読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Load(void) {
	RNLib::Load();	// R.N.Lib
}

//========================================
// Save関数 - 書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::Save(void) {
	RNLib::Save();		// R.N.Lib
	SaveSetting();	// 設定
}

//========================================
// SetMode関数 - モードの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void CProcess::SetMode(MODE newMode) {
	// 現在のモードの終了処理
	ModeUninit();

	// 現在のモードを切り替える
	m_mode = newMode;

	// 新しいモードの初期化処理
	ModeInit();
}
