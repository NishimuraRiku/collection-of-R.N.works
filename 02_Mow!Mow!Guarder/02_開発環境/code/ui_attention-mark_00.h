//========================================
// 
// UI:注意マーク[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_attention-mark_00.h ***
//========================================
#ifndef _UI_ATTENTIONMARK_00_H_	// このマクロ定義がされていなかった時
#define _UI_ATTENTIONMARK_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 構造体の定義
//****************************************
// UI:注意マーク[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き

	// 状態関連
	bool	bUse;			// 使用されているかフラグ
	bool	bShow;			// 可視フラグ
}Ui_attentionMark_00;

// UI:注意マーク[00] の管理情報構造体
typedef struct 
{
	int	nCounterBrink;		// 点滅カウンター
	int	nCounterArrowMove;	// 矢印移動カウンター
}Ui_attentionMark_00Control;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:注意マーク[00] の情報を取得
// UI:注意マーク[00] の管理情報を取得
Ui_attentionMark_00 *GetUi_attentionMark_00(void);
Ui_attentionMark_00Control *GetUi_attentionMark_00Control(void);

// UI:注意マーク[00] の初期化処理
// UI:注意マーク[00] の終了処理
// UI:注意マーク[00] の更新処理
// UI:注意マーク[00] の描画処理
void InitUi_attentionMark_00(void);
void UninitUi_attentionMark_00(void);
void UpdateUi_attentionMark_00(void);
void DrawUi_attentionMark_00(void);

// UI:注意マーク[00] の設定処理
void SetUi_attentionMark_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif