//========================================
// 
// モデルビューワの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** model-viewer.cpp ***
//========================================
#include <stdio.h>
#include "main.h"
// Character
#include "chr_model.h"
// System
#include "model-viewer.h"
// _R.N.Lib
#include "../R.N.Lib\RNmain.h"

//****************************************
// マクロ定義
//****************************************
//========== *** 操作関連 ***
//===== *** カメラ ***
// カメラ(3D)のカーソルの回転力
#define CAMERA3D_ROT_FORCE (D3DXVECTOR3(0.004f,-0.006f,0.0f))
// カメラ(3D)の回転力
#define CAMERA3D_ROT_FORCE_BY_CURSOR (D3DXVECTOR3(0.0005f,0.00075f,0.0f))
// カメラ(3D)の回転の減衰値
#define CAMERA3D_SPIN_DAMP (0.9f)
// カメラ(3D)の初期の高さの上限
#define CAMERA3D_HEIGHT_MAX (0.99f)
// カメラ(3D)の初期の高さの下限
#define CAMERA3D_HEIGHT_MIN (0.0f)

//****************************************
// 列挙型の定義
//****************************************
// モデルビューワのテキスト
typedef enum
{
	MODELVIEWER_TEXT_MODELTYPE,				// モデルの種類
	MODELVIEWER_TEXT_MOTIONNUMBER,			// モーション番号
	MODELVIEWER_TEXT_MOTIONFRAME,			// モーションフレーム
	MODELVIEWER_TEXT_MODELTYPECHANGE,		// モデルの種類変更
	MODELVIEWER_TEXT_MOTIONNUMBERCHANGE,	// モーション番号変更
	MODELVIEWER_TEXT_PAUSE,					// ポーズ
	MODELVIEWER_TEXT_CAMERAUPDOWN,			// カメラ上下
	MODELVIEWER_TEXT_HIDETEXT,				// テキストを隠す
	MODELVIEWER_TEXT_HIDEHITTEST,			// 当たり判定を隠す
	MODELVIEWER_TEXT_RELOAD,				// 再読込
	MODELVIEWER_TEXT_MODELLOAD,				// モデル読み込み
	MODELVIEWER_TEXT_MAX,
}MODELVIEWER_TEXT;

//****************************************
// 構造体の定義
//****************************************
//========== *** その他 ***
// モデルビューワのテキスト設定情報
typedef struct 
{
	// 表示形式
	char aString[TXT_MAX];
	D3DXVECTOR3 pos;	// 位置
	int nFont;			// フォント
	TEXT_DISP textDisp;	// 表示形式
	float fScale;		// 拡大倍率
}TextInfoModelViewer;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 更新 ***
// モデルビューワの入力処理
void InputModelViewer(ModelViewer *pEdtr);
// モデルビューワのテキスト設定処理
void SetTextModelViewer(ModelViewer *pEdtr);
//========== *** 入出力 ***
// モデルビューワのマップ情報の読み込み処理
void LoadModelDataModelViewer(ModelViewer *pEdtr);
// モデルビューワのマップ情報の再読み込み処理
void ReloadModelDataModelViewer(ModelViewer *pEdtr);
//========== *** 取得 ***
// モデルビューワのパラメーター初期値を取得
ModelViewer InitParameterModelViewer(void);

//****************************************
// グローバル変数宣言
//****************************************
ModelViewer g_modelViewer3D;	// モデルビューワの情報

//****************************************
// グローバル定数宣言
//****************************************
// モデルビューワのテキスト情報
const TextInfoModelViewer g_aTextInfoModelViewer[MODELVIEWER_TEXT_MAX] = {
	{ "MODEL TYPE   : %d/%d"       ,D3DXVECTOR3(8,8,0.0f)                 ,0,TEXT_DISP_LEFT ,16.0f },	// モデルの種類
	{ "MOTION NUMBER: %d/%d"       ,D3DXVECTOR3(8,24,0.0f)                ,0,TEXT_DISP_LEFT ,16.0f },	// モーション番号
	{ "MOTION FRAME : %d/%d"       ,D3DXVECTOR3(8,40,0.0f)                ,0,TEXT_DISP_LEFT ,16.0f },	// モーションフレーム
	{ "MODEL TYPE CHANGE:[1][2]KEY",D3DXVECTOR3(SCREEN_WIDTH - 8,8,0.0f)  ,0,TEXT_DISP_RIGHT,16.0f },	// モデルの種類変更
	{ "MOTION CHANGE:[3][4]KEY"    ,D3DXVECTOR3(SCREEN_WIDTH - 8,24,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// モーション番号変更
	{ "PAUSE:   [P]KEY"            ,D3DXVECTOR3(SCREEN_WIDTH - 8,40,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// ポーズ
	{ "CAMERA UP/DOWN:[U][J]KEY"   ,D3DXVECTOR3(SCREEN_WIDTH - 8,56,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// カメラ上下
	{ "HIDE TEXT:  [F1]KEY"        ,D3DXVECTOR3(SCREEN_WIDTH - 8,72,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// テキストを隠す
	{ "HIDE HITTEST:  [F2]KEY"     ,D3DXVECTOR3(SCREEN_WIDTH - 8,88,0.0f) ,0,TEXT_DISP_RIGHT,16.0f },	// テキストを隠す
	{ "RELOAD:  [F3]KEY"           ,D3DXVECTOR3(SCREEN_WIDTH - 8,104,0.0f),0,TEXT_DISP_RIGHT,16.0f },	// 再読込
	{ "MODEL LOAD:  [F4]KEY"       ,D3DXVECTOR3(SCREEN_WIDTH - 8,120,0.0f),0,TEXT_DISP_RIGHT,16.0f },	// モデル読み込み
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 更新 *** |------------------------
//============================================================
//========================================
// InputModelViewer関数 - モデルビューワの入力処理 -
// Author:RIKU NISHIMURA
//========================================
void InputModelViewer(ModelViewer *pEdtr)
{
	// ポーズフラグを切り替える
	pEdtr->bPause ^= GetKeyboardTrigger(DIK_P);

	// テキスト表示フラグを切り替える
	pEdtr->bShowText ^= GetKeyboardTrigger(DIK_F1);

	// 当たり判定表示フラグを切り替える
	pEdtr->bShowHitTest ^= GetKeyboardTrigger(DIK_F2);

	// 当たり判定の表示/非表示
	ShowHitTest(pEdtr->bShowHitTest);

	// カメラの上下
	GetCamera3D()->posR.y += CAMERA3D_Y_MOVE * GetKeyboardPress(DIK_U);
	GetCamera3D()->posR.y -= CAMERA3D_Y_MOVE * GetKeyboardPress(DIK_J);

	if (GetKeyboardTrigger(DIK_F3)) 
	{// 再読み込み処理 
		ReloadModelDataModelViewer(pEdtr);
	}
	
	if (GetKeyboardTrigger(DIK_F4))
	{// 読み込み処理 
		LoadModelDataModelViewer(pEdtr);
	}

	// カメラ操作
	{
		if (GetMousePress(MOUSEBUTTON_RIGHT))
		{// マウスの右ボタンが押されている時、
			// カーソルの移動量に応じて回転
			AxisRotationCamera3D(DIRECTION_UP,		GetCursorMove().y * CAMERA3D_ROT_FORCE_BY_CURSOR.x);
			AxisRotationCamera3D(DIRECTION_LEFT,	GetCursorMove().x * CAMERA3D_ROT_FORCE_BY_CURSOR.y);
		}
		else
		{// いずれも該当しない時、
			if (GetKeyboardPress(DIK_UP		)){ AxisRotationCamera3D(DIRECTION_UP,		CAMERA3D_ROT_FORCE.x); }	// 上軸回転
			if (GetKeyboardPress(DIK_DOWN	)){ AxisRotationCamera3D(DIRECTION_DOWN,	CAMERA3D_ROT_FORCE.x); }	// 下軸回転
			if (GetKeyboardPress(DIK_LEFT	)){ AxisRotationCamera3D(DIRECTION_LEFT,	CAMERA3D_ROT_FORCE.y); }	// 左軸回転
			if (GetKeyboardPress(DIK_RIGHT	)){ AxisRotationCamera3D(DIRECTION_RIGHT,	CAMERA3D_ROT_FORCE.y); }	// 右軸回転
		}

		// カメラ(3D)のポインタ
		Camera3D *pCamera3D = GetCamera3D();

		pCamera3D->rot += pCamera3D->spin;				// 向きを更新
		pCamera3D->spin *= CAMERA3D_SPIN_DAMP;			// 回転量を減衰
		pCamera3D->fHeight += pCamera3D->fVerticalMove;	// 高さを更新
		pCamera3D->fVerticalMove *= CAMERA3D_SPIN_DAMP;	// 縦方向の移動量を減衰

		// 向きを制御
		RotControl(&pCamera3D->rot);

		// 高さを制御
		FloatControl(&pCamera3D->fHeight, CAMERA3D_HEIGHT_MAX, CAMERA3D_HEIGHT_MIN);

		// 視点の位置を設定
		pCamera3D->posV.x = pCamera3D->posR.x + (sinf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
		pCamera3D->posV.y = pCamera3D->posR.y + (pCamera3D->fLength * pCamera3D->fHeight);
		pCamera3D->posV.z = pCamera3D->posR.z + (cosf(pCamera3D->rot.y + D3DX_PI) * (pCamera3D->fLength * (1.0f - fabsf(pCamera3D->fHeight))));
	}
}

//========================================
// SetTextModelViewer関数 - モデルビューワのテキスト設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetTextModelViewer(ModelViewer *pEdtr) 
{
	if (pEdtr->bShowText)
	{// テキスト表示フラグ
		char aString[TXT_MAX];	// 代入用文字列

		for (int nCntText = 0; nCntText < MODELVIEWER_TEXT_MAX; nCntText++)
		{
			const TextInfoModelViewer txt = g_aTextInfoModelViewer[nCntText];
			Chr_model *pChr = GetChr_model();

			// 表示文字列設定
			switch (nCntText)
			{
			case MODELVIEWER_TEXT_MODELTYPE:
				sprintf(aString, txt.aString, pChr->nType, GetChr_modelControl()->nTypeNum - 1);;
				break;
			case MODELVIEWER_TEXT_MOTIONNUMBER:
				sprintf(aString, txt.aString,pChr->nMotion, GetChr_modelType()[pChr->nType].motionSet.nMotionNum - 1);
				break;
			case MODELVIEWER_TEXT_MOTIONFRAME:
				sprintf(aString, txt.aString, pChr->partsInfo.nCounterMotion, GetChr_modelType()[pChr->nType].motionSet.aMotion[pChr->nMotion].nLoopTime);
				break;
			default:
				sprintf(aString, txt.aString);
				break;
			}

			SetText2D(aString, txt.nFont, txt.textDisp, txt.pos, INITCOLOR, txt.fScale, txt.fScale);
		}
	}
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================
//========================================
// LoadModelDataModelViewer関数 - モデルビューワのモデル情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadModelDataModelViewer(ModelViewer *pEdtr)
{
	char fileName[TXT_MAX];	// ファイル名格納

	if (OpenFile(fileName, "data\\MODELDATA", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// CHR:モデルの読み込み処理
		LoadChr_model(fileName);
		// CHR:モデルの初期化処理
		InitChr_model();
		// 現在のファイル名を設定
		sprintf(pEdtr->aNowFile, fileName);
	}
}

//========================================
// ReloadModelDataModelViewer関数 - モデルビューワのモデル情報の再読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void ReloadModelDataModelViewer(ModelViewer *pEdtr)
{
	// CHR:モデルの読み込み処理
	LoadChr_model(pEdtr->aNowFile);
	// CHR:モデルの初期化処理
	InitChr_model();
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// InitParameterModelViewer関数 - モデルビューワのパラメーター初期値を返す -
// Author:RIKU NISHIMURA
//========================================
ModelViewer InitParameterModelViewer(void)
{
	return{
		false,	// ポーズフラグ
		true,	// テキスト表示フラグ
		true,	// 当たり判定表示フラグ
		"",		// 現在のファイル名
	};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================
//========================================
// InitModelViewer関数 - モデルビューワの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitModelViewer(void)
{
	// モデルビューワのパラメーターを初期化
	g_modelViewer3D = InitParameterModelViewer();
}

//========================================
// UodateModelViewer関数 - モデルビューワの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateModelViewer(void)
{
	// モデルビューワの情報のポインタ
	ModelViewer *pEdtr = &g_modelViewer3D;

	// 入力処理
	InputModelViewer(pEdtr);

	// テキスト設定処理
	SetTextModelViewer(pEdtr);
}

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// GetModelViewer関数 - モデルビューワのポインタを取得 -
// Author:RIKU NISHIMURA
//========================================
ModelViewer *GetModelViewer(void)
{
	return &g_modelViewer3D;
}