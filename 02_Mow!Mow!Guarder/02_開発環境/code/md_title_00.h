//========================================
// 
// MD:タイトル画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.h ***
//========================================
#ifndef _MD_TITLE_00_H_	// このマクロ定義がされていなかった時
#define _MD_TITLE_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
// オブジェクトの配置間隔
#define OBJECT_SET_SPACE	(8)

// MD:タイトル画面[00] のカメラ(3D)の距離
// MD:タイトル画面[00] の視点の高さ
// MD:タイトル画面[00] の注視点の高さ
// MD:タイトル画面[00] の開始向きY
#define MD_TITLE_00_CAMERA3D_LENGTH			(120.0f)
#define MD_TITLE_00_CAMERA3D_POS_R_HEIGHT	(-0.2f)
#define MD_TITLE_00_CAMERA3D_POS_V_HEIGHT	(40.0f)
#define MD_TITLE_00_CAMERA3D_BG_SPIN		D3DXVECTOR3(0.0f,0.001f,0.0f)

//****************************************
// 列挙型の定義
//****************************************
// MD:タイトル画面[00] の状態
typedef enum 
{
	MD_TITLE_00_STATE_NONE,		// 無し
	MD_TITLE_00_STATE_NORMAL,	// 通常
	MD_TITLE_00_STATE_USERGUID,	// ユーザーガイド
	MD_TITLE_00_STATE_RANKING,	// ランキング
	MD_TITLE_00_STATE_SETTINGS,	// 設定
	MD_TITLE_00_STATE_MAX,
}MD_TITLE_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// MD:タイトル画面[00] の情報構造体
typedef struct 
{
	// 状態関連
	MD_TITLE_00_STATE	state;				// MD:タイトル画面[00] の状態
	int					nCounterState;		// 状態のカウンター
	bool				bShowCopyRight;		// 著作権表示フラグ
	int					nCounterCopyRight;	// 著作権表示ガウンター
}Md_title_00;

//****************************************
// プロトタイプ宣言
//****************************************
// MD:タイトル画面[00] の情報を取得
Md_title_00 *GetMd_title_00(void);

// MD:タイトル画面[00] の初期化処理
// MD:タイトル画面[00] の終了処理
// MD:タイトル画面[00] の更新処理
// MD:タイトル画面[00] の描画処理
void InitMd_title_00(void);
void UninitMd_title_00(void);
void UpdateMd_title_00(void);
void DrawMd_title_00(void);

// MD:タイトル画面[00] の状態を設定
void SetMd_title_00State(MD_TITLE_00_STATE state);

#endif