//========================================
// 
// UI:ダメージ[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_damage_00.h ***
//========================================
#ifndef _UI_DAMAGE_00_H_	// このマクロ定義がされていなかった時
#define _UI_DAMAGE_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************
// UI:ダメージ[00] の色の種類
typedef enum 
{
	UI_DAMAGE_00_COLOR_TYPE_PLAYER,	// プレイヤー
	UI_DAMAGE_00_COLOR_TYPE_ENEMY,	// 敵
	UI_DAMAGE_00_COLOR_TYPE_MAX,
}UI_DAMAGE_00_COLOR_TYPE;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:ダメージ[00] の初期化処理
// UI:ダメージ[00] の終了処理
// UI:ダメージ[00] の更新処理
void InitUi_damage_00(void);
void UninitUi_damage_00(void);
void UpdateUi_damage_00(void);

// UI:ダメージ[00] の設定処理
void SetUi_damage_00(D3DXVECTOR3 pos, int nDamage, UI_DAMAGE_00_COLOR_TYPE colType);

#endif