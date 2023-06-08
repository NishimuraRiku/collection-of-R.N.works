//========================================
// 
// 基本処理クラスのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** process.h ***
//========================================
#ifndef _PROCESS_H_	// このマクロ定義がされていなかった時
#define _PROCESS_H_	// 二重インクルード防止のマクロを定義する

#include "../../../../_R.N.Lib/R.N.Lib.h"

// Mode
#include "Mode\mode_game-editor.h"

//****************************************
// マクロ定義
//****************************************
#define SCREEN_WIDTH            (1280)					// ウインドウの幅(NORMAL=1280 4:3=960)
#define SCREEN_HEIGHT           (720)					// ウインドウの高さ
#define SCREEN_CENTER_X         (SCREEN_WIDTH*0.5f)		// ウインドウの中心座標X
#define SCREEN_CENTER_Y         (SCREEN_HEIGHT*0.5f)	// ウインドウの中心座標Y
#define INSIDE_SCREEN_LEFTMOST  (960)					// 内側スクリーンの左端
#define INSIDE_SCREEN_RIGHTMOST (720)					// 内側スクリーンの右端
#define DEFAULTBGCOLOR          Color{58,58,58,255}		// 背景色

//****************************************
// クラス定義
//****************************************
// メイン処理クラス
class CProcess {
public:
	//========== *** 関数宣言 ***
	/* コンストラクタ */CProcess();
	/* デストラクタ   */~CProcess();
	/* メインループ   */int MainLoop(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow);

private:
	//========== *** 列挙型定義 ***
	// モード(画面)の種類
	typedef enum {
		MODENONE = -1,	// 無し
		MODE_MAIN,
		MODE_MAX,
	}MODE;

	//========== *** 関数宣言 ***
	/* モードに応じた初期化処理 */void ModeInit(void);
	/* モードに応じた終了処理   */void ModeUninit(void);
	/* モードに応じた更新処理   */void ModeUpdate(void);
	/* 初期化処理               */HRESULT Init(HINSTANCE hInstance);
	/* 終了処理                 */void Uninit(void);
	/* 更新処理                 */void Update(void);
	/* 描画処理                 */void Draw(void);
	/* 読み込み処理             */void Load(void);
	/* 書き込み処理             */void Save(void);
	/* モードの設定処理         */void SetMode(MODE newMode);

	//========== *** 変数定義 ***
	int m_nCountFPS;	// FPSのカウンター
	MODE m_mode;		// モード

	//========== *** モードインスタンス ***
	Mode_GameEditor mode_gameEditor;
};

#endif