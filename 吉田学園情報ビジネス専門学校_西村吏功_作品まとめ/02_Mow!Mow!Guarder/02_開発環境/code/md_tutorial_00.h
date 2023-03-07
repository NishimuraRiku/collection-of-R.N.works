//========================================
// 
// MD:チュートリアル画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_tutorial_00.h ***
//========================================
#ifndef _MD_TUTORIAL_00_H_	// このマクロ定義がされていなかった時
#define _MD_TUTORIAL_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
// オブジェクトの配置間隔
#define OBJECT_SET_SPACE	(8)

// MD:チュートリアル画面[00] のチュートリアル[00] のカメラ(3D)の開始距離
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH	(280.0f)
// MD:チュートリアル画面[00] のチュートリアル[00] のカメラ(3D)の終了距離
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_END_LENGTH	(240.0f)
// MD:チュートリアル画面[00] のチュートリアル[00] のカメラ(3D)の拡大にかかる時間
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME		(60)
// MD:チュートリアル画面[00] のチュートリアル[00] のカメラ(3D)の注視点の高さ
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_POS_R_HEIGHT	(4.0f)
// MD:チュートリアル画面[00] のチュートリアル[00] のカメラ(3D)の視点の高さ
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_POS_V_HEIGHT	(0.35f)
// MD:チュートリアル画面[00] のチュートリアル[00] のカメラ(3D)の向き
#define MD_TUTORIAL_00_STATE_TUTORIAL_00_CAMERA3D_ROT			(D3DX_PI * -0.85f)

// MD:チュートリアル画面[00] の位置Y
#define MD_TUTORIAL_00_BG_POS_Y	(-40.0f)

//****************************************
// 列挙型の定義
//****************************************
// MD:チュートリアル画面[00] の状態
typedef enum 
{
	MD_TUTORIAL_00_STATE_NONE,			// 無し
	MD_TUTORIAL_00_STATE_NORMAL,		// 通常
	MD_TUTORIAL_00_STATE_PAUSE,			// ポーズ
	MD_TUTORIAL_00_STATE_USERGUID,		// ユーザーガイド
	MD_TUTORIAL_00_STATE_SETTING,		// 設定
	MD_TUTORIAL_00_STATE_RANKING,		// ランキング
	MD_TUTORIAL_00_STATE_RESULT,		// リザルト
	MD_TUTORIAL_00_STATE_TIPS,			// TIPS待ち
	MD_TUTORIAL_00_STATE_TUTORIAL_00,	// チュートリアル[00]
	MD_TUTORIAL_00_STATE_MAX,
}MD_TUTORIAL_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// MD:チュートリアル画面[00] の情報構造体
typedef struct 
{
	// 状態関連
	MD_TUTORIAL_00_STATE	state;			// MD:チュートリアル画面[00] の状態
	int					nCounterState;	// 状態のカウンター
	D3DXVECTOR3			previousPosR;	// 前の注視点の位置
}Md_tutorial_00;

//****************************************
// プロトタイプ宣言
//****************************************
// MD:チュートリアル画面[00] の情報を取得
Md_tutorial_00 *GetMd_tutorial_00(void);

// MD:チュートリアル画面[00] の初期化処理
// MD:チュートリアル画面[00] の終了処理
// MD:チュートリアル画面[00] の更新処理
// MD:チュートリアル画面[00] の描画処理
void InitMd_tutorial_00(void);
void UninitMd_tutorial_00(void);
void UpdateMd_tutorial_00(void);
void DrawMd_tutorial_00(void);

// MD:チュートリアル画面[00] の状態を設定
void SetMd_tutorial_00State(MD_TUTORIAL_00_STATE state);

#endif