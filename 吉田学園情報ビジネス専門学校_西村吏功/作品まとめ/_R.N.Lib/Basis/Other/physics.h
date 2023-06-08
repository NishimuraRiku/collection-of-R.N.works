//========================================
// 
// 物理関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_	// このマクロ定義がされていなかった時
#define _PHYSICS_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型定義
//****************************************
//========== *** 当たり判定 ***
// 衝突チェックのモード
typedef enum {
	COLLCHK_MODE_NORMAL,	// 通常
	COLLCHK_MODE_LOOKDOWN,	// 見下ろし
}COLLCHK_MODE;

// 当たり判定の形
typedef enum {
	HIT_TEST_FORM_SPHERE,	// 球
	HIT_TEST_FORM_SQUARE,	// 四角
	HIT_TEST_FORM_CYLINDER,	// 円筒
	HIT_TEST_FORM_MAX,
}HIT_TEST_FORM;

//========== *** その他 ***
// 衝突方向
typedef enum {
	HITDIRECTION_TOP,		// 上
	HITDIRECTION_BOTTOM,	// 下
	HITDIRECTION_LEFT,		// 左
	HITDIRECTION_RIGHT,		// 右
	HITDIRECTION_BACK,		// 奥
	HITDIRECTION_FRONT,		// 手前
	HITDIRECTION_MAX,
}HITDIRECTION;

// 方向
typedef enum {
	DIRECTION_UP,			// 上
	DIRECTION_DOWN,			// 下
	DIRECTION_LEFT,			// 左
	DIRECTION_RIGHT,		// 右
	DIRECTION_BACK,			// 奥
	DIRECTION_FRONT,		// 手前
	DIRECTION_LEFT_BACK,	// 左奥
	DIRECTION_LEFT_FRONT,	// 左手前
	DIRECTION_RIGHT_BACK,	// 右奥
	DIRECTION_RIGHT_FRONT,	// 右手前
	DIRECTION_MAX,
}DIRECTION;

// ベクトルの種類
typedef enum {
	VECTOR_X,		// X方向
	VECTOR_Y,		// Y方向
	VECTOR_Z,		// Z方向
	VECTOR_NONE,	// 無し
	VECTOR_MAX
}VECTOR;

// 頂点
typedef enum {
	VERTEX_X0Y0,	// 左上
	VERTEX_X1Y0,	// 右上
	VERTEX_X0Y1,	// 左下
	VERTEX_X1Y1,	// 右下
	VERTEX_MAX,
}VERTEX;

//****************************************
// 構造体定義
//****************************************
//========== *** 衝突情報 ***
// 衝突情報構造体
typedef struct {
	bool bHit = false;	// 当たりフラグ
	bool bColl = false;	// 衝突フラグ
	bool bOvl = false;	// 範囲内フラグ
}Collision;

// 当たり判定(3D)の部品情報構造体
typedef struct {
	HIT_TEST_FORM hitTestForm = HIT_TEST_FORM_SPHERE;	// 当たり判定の形
	int nParent = -1;		// 親番号
	float fWidth = 8.0f;	// 幅
	float fHeight = 8.0f;	// 高さ
	float fDepth = 8.0f;	// 奥行き
	D3DXVECTOR3 fixedRelativePos = INITD3DXVECTOR3;	// 相対位置
	D3DXVECTOR3 fixedRelativeRot = INITD3DXVECTOR3;	// 相対向き
	bool aGhost[HITDIRECTION_MAX] = {};	// すり抜け情報
}HitTest3DParts;

//========== *** 設定情報 ***
// 当たり判定(3D)の情報構造体
typedef struct {
	int nHitTestPartsNum = 0;	// 当たり判定(3D)の部品の数
	// 当たり判定(3D)の部品の情報
	HitTest3DParts *pHitTestParts = NULL;
}HitTest3D;

// 当たり判定(3D)の設定情報構造体
typedef struct {
	int nHitTestNum = 0;	// 当たり判定の数
	HitTest3D *pHitTest = NULL;	// 当たり判定の情報
}HitTest3DSet;

//========== *** 管理情報 ***
// 当たり判定(3D)管理構造体
typedef struct {
	int	nHitTest = 0;	// 当たり判定番号
}HitTest3DInfo;

//========== *** 引渡情報 ***
// 衝突判定に必要な情報構造体
typedef struct {
	D3DXVECTOR3 *pPos	= NULL;					// 現在の位置のポインタ
	D3DXVECTOR3 posOld	= INITD3DXVECTOR3;		// 過去の位置のポインタ
	D3DXVECTOR3 *pMove	= NULL;					// 移動量のポインタ
	D3DXVECTOR3 *pRot	= NULL;					// 現在の向きのポインタ
	D3DXVECTOR3 rotOld	= INITD3DXVECTOR3;		// 過去の向きのポインタ
	HitTest3D *pHitTest	= NULL;					// 当たり判定の情報
	COLLCHK_MODE mode	= COLLCHK_MODE_NORMAL;	// 衝突チェックのモード
#if /* 設定フォーマット */0
	// 衝突判定に必要な情報
	CollisionInfo myCollInfo;
	/* 現在の位置のポインタ */myCollInfo.pPos = ;
	/* 過去の位置のポインタ */myCollInfo.posOld = ;
	/* 移動量のポインタ     */myCollInfo.pMove = ;
	/* 現在の向きのポインタ */myCollInfo.pRot = ;
	/* 過去の向きのポインタ */myCollInfo.rotOld = ;
	/* 当たり判定の情報     */myCollInfo.pHitTest = ;
	/* 衝突チェックのモード */myCollInfo.mode = ;
#endif
}CollisionInfo;

// 描画する当たり判定(3D)情報構造体
typedef struct {
	D3DXMATRIX mtx;	// マトリックス
	HitTest3DParts hitTestParts;	// 当たり判定の部品情報
#if /* 設定フォーマット */0
	// 描画する当たり判定(3D)情報構造体
	DrawHitTest3DSet drawHitTestSet;
	/* マトリックス */drawHitTestSet.mtx = ;
	/* 部品情報     */drawHitTestSet.hitTestParts = ;
	// 描画する当たり判定(3D)登録処理
	DrawHitTest3DSignUp(drawHitTestSet);
#endif
}DrawHitTest3DSet;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// 物理関連の初期化処理
void InitPhysics(void);
//========== *** 入出力 ***
// 描画する当たり判定(3D)設定情報の読み込み処理
void LoadHitTest3DSet(FILE *pFile, HitTest3DSet *pHitTest3DSet);
//========== *** 当たり判定 ***
// 描画する当たり判定(3D)登録処理
void DrawHitTest3DSignUp(DrawHitTest3DSet drawHitTestSet);
// 当たり判定登録解除処理
void DrawHitTestSignOut(void);
// 衝突チェック
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode);
// 衝突情報の上書き
void CollisionOverwrite(Collision *pOvwCollision, Collision collision);
//========== *** 当たり判定描画関連 ***
// 当たり判定(3D)の描画準備処理
void DrawHitTest3DSetUp(void);
// 当たり判定(3D)の描画処理
void DrawHitTest3D(void);
// 当たり判定(3D)の線の数を初期化
void InitHitTest3DLineNum(void);
// 当たり判定の表示設定
void ShowHitTest(bool bShow);

#endif