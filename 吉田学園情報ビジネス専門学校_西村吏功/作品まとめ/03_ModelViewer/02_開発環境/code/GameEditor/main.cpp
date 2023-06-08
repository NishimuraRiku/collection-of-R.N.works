//========================================
// 
// メイン処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "process.h"
#include "../../../../_R.N.Lib/RNmain.h"

//========================================
// WinMain関数
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	CProcess process;	// メイン処理のインスタンスを生成

	// メインループ
	return process.MainLoop(hInstance, hInstancePrev, IpCmdLine, nCmdShow);
}

//========================================
// WindowProc関数
// Author:RIKU NISHIMURA
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int nID;	// 返り値を格納

	// R.N.Lib
	RNLib::WindowProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg) {
	case WM_DESTROY: // ウインドウ破棄のメッセージ

		PostQuitMessage(0); // WM_QUITメッセージを送る

		break;
	case WM_KEYDOWN: // キー押下のメッセージ

		switch (wParam) {
		case VK_ESCAPE: // [ESC]キーが押された

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO); // ウインドウを破棄する(WM_DESTROYメッセージを送る)

			if (nID == IDYES) {
				DestroyWindow(hWnd); // ウインドウを破棄する
			}

			break;
		}

		break;
	case WM_CLOSE: // 終了ボタンが押された時

		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO); // ウインドウを破棄する(WM_DESTROYメッセージを送る)

		if (nID == IDYES) {
			DestroyWindow(hWnd); // ウインドウを破棄する
		}
		else {
			return 0; // (※0を返さないと終了してしまう)
		}

		break;
	case WM_LBUTTONDOWN: // マウスクリックのメッセージ

		// 対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd); // マウスを左クリックしたウインドウをアクティブにする

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を返す
}