//========================================
// 
// MD:ゲーム画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game_00.h ***
//========================================
#ifndef _MD_GAME_00_H_	// このマクロ定義がされていなかった時
#define _MD_GAME_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
// オブジェクトの配置間隔
#define OBJECT_SET_SPACE	(8)

// MD:ゲーム画面[00] のチュートリアル[00] のカメラ(3D)の開始距離
// MD:ゲーム画面[00] のチュートリアル[00] のカメラ(3D)の終了距離
// MD:ゲーム画面[00] のチュートリアル[00] のカメラ(3D)の拡大にかかる時間
// MD:ゲーム画面[00] のチュートリアル[00] のカメラ(3D)の注視点の高さ
// MD:ゲーム画面[00] のチュートリアル[00] のカメラ(3D)の視点の高さ
// MD:ゲーム画面[00] のチュートリアル[00] のカメラ(3D)の向き
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_START_LENGTH	(160.0f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_END_LENGTH	(120.0f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ZOOM_TIME		(60)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_R_HEIGHT	(4.0f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_POS_V_HEIGHT	(0.25f)
#define MD_GAME_00_STATE_TUTORIAL_00_CAMERA3D_ROT			(D3DX_PI)

// MD:ゲーム画面[00] のチュートリアル[01] にかかる時間
// MD:ゲーム画面[00] のチュートリアル[01] のカメラ(3D)の開始距離
// MD:ゲーム画面[00] のチュートリアル[01] のカメラ(3D)の終了距離
// MD:ゲーム画面[00] のチュートリアル[01] のカメラ(3D)の注視点の高さ
// MD:ゲーム画面[00] のチュートリアル[01] のカメラ(3D)の視点の高さ
// MD:ゲーム画面[00] のチュートリアル[01] のカメラ(3D)の開始向きY
// MD:ゲーム画面[00] のチュートリアル[01] のカメラ(3D)の終了向きY
#define MD_GAME_00_STATE_TUTORIAL_01_TIME					(3000)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_LENGTH		(160.0f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_R_HEIGHT	(4.0f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_POS_V_HEIGHT	(0.25f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_START_ROT_Y	(D3DX_PI*-0.5f)
#define MD_GAME_00_STATE_TUTORIAL_01_CAMERA3D_END_ROT_Y		(D3DX_PI*0.5f)

// MD:ゲーム画面[00] のコア破壊ムービー(拡大)にかかる時間
// MD:ゲーム画面[00] のコア破壊ムービー(拡大)のカメラ(3D)の開始距離
// MD:ゲーム画面[00] のコア破壊ムービー(拡大)のカメラ(3D)の終了距離
// MD:ゲーム画面[00] のコア破壊ムービー(拡大)のカメラ(3D)の注視点の高さ
// MD:ゲーム画面[00] のコア破壊ムービー(拡大)のカメラ(3D)の視点の高さ
// MD:ゲーム画面[00] のコア破壊ムービー(拡大)のカメラ(3D)の開始向きY
// MD:ゲーム画面[00] のコア破壊ムービー(拡大)のカメラ(3D)の終了向きY
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_TIME					(180)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_START_LENGTH	(200.0f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_END_LENGTH	(80.0f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_R_HEIGHT	(4.0f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_POS_V_HEIGHT	(0.5f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_START_ROT_Y	(D3DX_PI * -1.25f)
#define MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM_CAMERA3D_END_ROT_Y		(D3DX_PI * -0.75f)

// MD:ゲーム画面[00] の位置Y
#define MD_GAME_00_BG_POS_Y	(-40.0f)

//****************************************
// 列挙型の定義
//****************************************
// MD:ゲーム画面[00] の状態
typedef enum 
{
	MD_GAME_00_STATE_NONE,								// 無し
	MD_GAME_00_STATE_NORMAL,							// 通常
	MD_GAME_00_STATE_PAUSE,								// ポーズ
	MD_GAME_00_STATE_USERGUID,							// ユーザーガイド
	MD_GAME_00_STATE_SETTING,							// 設定
	MD_GAME_00_STATE_CLEAR,								// クリア演出
	MD_GAME_00_STATE_RANKING,							// ランキング
	MD_GAME_00_STATE_RESULT,							// リザルト
	MD_GAME_00_STATE_TUTORIAL_00,						// チュートリアル[00]
	MD_GAME_00_STATE_TUTORIAL_01,						// チュートリアル[01]
	MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_MOVE,		// ボス出現ムービー(視点移動)
	MD_GAME_00_STATE_BOSS_POP_MOVIE_POP,				// ボス出現ムービー(出現)
	MD_GAME_00_STATE_BOSS_POP_MOVIE_VIEWPOINT_RESET,	// ボス出現ムービー(視点リセット)
	MD_GAME_00_STATE_CORE_DESTROY_MOVIE_ZOOM,			// コア破壊ムービー(拡大)
	MD_GAME_00_STATE_CORE_DESTROY_MOVIE_EXPLOSION,		// コア破壊ムービー(爆発)
	MD_GAME_00_STATE_MAX,
}MD_GAME_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// MD:ゲーム画面[00] の情報構造体
typedef struct 
{
	// 状態関連
	MD_GAME_00_STATE	state;			// MD:ゲーム画面[00] の状態
	int					nCounterState;	// 状態のカウンター
	D3DXVECTOR3			previousPosR;	// 前の注視点の位置
}Md_game_00;

//****************************************
// プロトタイプ宣言
//****************************************
// MD:ゲーム画面[00] の情報を取得
Md_game_00 *GetMd_game_00(void);

// MD:ゲーム画面[00] の初期化処理
// MD:ゲーム画面[00] の終了処理
// MD:ゲーム画面[00] の更新処理
// MD:ゲーム画面[00] の描画処理
void InitMd_game_00(void);
void UninitMd_game_00(void);
void UpdateMd_game_00(void);
void DrawMd_game_00(void);

// MD:ゲーム画面[00] の状態を設定
void SetMd_game_00State(MD_GAME_00_STATE state);

#endif