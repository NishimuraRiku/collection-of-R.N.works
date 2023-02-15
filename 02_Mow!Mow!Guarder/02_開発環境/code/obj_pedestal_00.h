//========================================
// 
// OBJ:台座[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_pedestal_00.h ***
//========================================
#ifndef _OBJ_PEDESTAL_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_PEDESTAL_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "physics.h"

//****************************************
// マクロ定義
//****************************************
// OBJ:台座[00] の最大数
#define OBJ_PEDESTAL_00_MAX	(64)

// OBJ:台座[00] の到着にかかる時間
// OBJ:台座[00] のユニットのHPの上昇倍率
// OBJ:台座[00] のプレイヤーのHPの上昇倍率
#define OBJ_PEDESTAL_00_ARRIVAL_TIME		(60)
#define OBJ_PEDESTAL_00_UNIT_HP_ADDRATE		(0.2f)
#define OBJ_PEDESTAL_00_PLAYER_HP_ADDRATE	(0.2f)

//****************************************
// 列挙型の定義
//****************************************
// OBJ:台座[00] の商品番号
typedef enum 
{
	OBJ_PEDESTAL_00_ITEM_BLOCK,				// ブロック
	OBJ_PEDESTAL_00_ITEM_TURRET,			// タレット
	OBJ_PEDESTAL_00_ITEM_REFLECTOR,			// 反射装置
	OBJ_PEDESTAL_00_ITEM_DISCHARGER,		// 放電装置
	OBJ_PEDESTAL_00_ITEM_PLAYER_HP_PRUS,	// プレイヤーのHPを回復
	OBJ_PEDESTAL_00_ITEM_UNIT_HP_PRUS,		// ユニットのHPを回復
	OBJ_PEDESTAL_00_ITEM_SWORD_ATK_UP,		// 剣の攻撃力アップ
	OBJ_PEDESTAL_00_ITEM_BULLET_ATK_UP,		// 弾の攻撃力アップ
	OBJ_PEDESTAL_00_ITEM_MAX,
}OBJ_PEDESTAL_00_ITEM;

//****************************************
// 構造体の定義
//****************************************
// OBJ:台座[00] の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 basePos;	// 基準位置
	D3DXVECTOR3 rot;		// 向き

	// 分類情報
	int nType;	// 種類

	// 状態関連
	bool	bUse;			// 使用されているかフラグ
	int		nCounterNotice;	// 注目カウンター
}Obj_pedestal_00;

// OBJ:台座[00] の管理情報構造体
typedef struct
{
	int	nCntItems;			// 品揃えカウント
	int nCounterArrival;	// 到着カウンター
}Obj_pedestal_00Control;

// OBJ:台座[00] の商品毎の情報構造体
typedef struct
{
	int	nPrice;	// 値段
}Obj_pedestal_00Item;

// OBJ:台座[00] の商品毎の管理情報構造体
typedef struct
{
	int	nCntPriceUp;	// 値上げカウント
}Obj_pedestal_00ItemControl;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:台座[00] の情報を取得
// OBJ:台座[00] の管理情報を取得
// OBJ:台座[00] の商品情報を取得
Obj_pedestal_00 *GetObj_pedestal_00(void);
Obj_pedestal_00Control *GetObj_pedestal_00Control(void);
const Obj_pedestal_00Item *GetObj_pedestal_00Item(void);
Obj_pedestal_00ItemControl *GetObj_pedestal_00ItemControl(void);

// OBJ:台座[00] の初期化処理
// OBJ:台座[00] の終了処理
// OBJ:台座[00] の更新処理
// OBJ:台座[00] の描画処理
void InitObj_pedestal_00(void);
void UninitObj_pedestal_00(void);
void UpdateObj_pedestal_00(void);
void DrawObj_pedestal_00(void);

// OBJ:台座[00] の読み込み処理
void LoadObj_pedestal_00(void);

// OBJ:台座[00] の設定処理
void SetObj_pedestal_00(D3DXVECTOR3 pos, int nType);

// OBJ:台座[00] との当たり判定処理
int CollisionObj_pedestal_00(VECTOR vector, Collision *pObjCollision, Collision *pCmnCollision, CollisionInfo myCollInfo);

// OBJ:台座[00] の購入処理
// OBJ:台座[00] の品揃え変更処理
// OBJ:台座[00] の商品値上げ処理
// OBJ:台座[00] の商品値下げ処理
bool PurchaseObj_pedestal_00(void);
void ItemsChangeObj_pedestal_00(void);
void ItemPriceUpObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm);
void ItemPriceDownObj_pedestal_00(OBJ_PEDESTAL_00_ITEM itm);

#endif