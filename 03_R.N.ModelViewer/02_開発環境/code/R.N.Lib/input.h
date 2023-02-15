//========================================
// 
// 入力関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** input.h ***
//========================================
#ifndef _INPUT_H_	// このマクロ定義がされていなかった時
#define _INPUT_H_	// 二重インクルード防止のマクロを定義する

// _R.N.Lib
#include "RNmain.h"

//****************************************
// 列挙型の定義
//****************************************
// 動的なデバイス
typedef enum 
{
	ACTIVE_DEVICE_KEYBOARD,
	ACTIVE_DEVICE_CONTROLLER,
	ACTIVE_DEVICE_MAX,
}ACTIVE_DEVICE_TYPE;
// マウス番号
typedef enum
{
	MOUSEBUTTON_LEFT = 0,
	MOUSEBUTTON_RIGHT,
	MOUSEBUTTON_MAX,
}MOUSEBUTTON;
// ボタン番号
typedef enum 
{
	BUTTON_UP = 0,			// 上キー
	BUTTON_DOWN,			// 下キー
	BUTTON_LEFT,			// 左キー
	BUTTON_RIGHT,			// 右キー
	BUTTON_START,			// スタートキー
	BUTTON_BACK,			// バックキー
	BUTTON_LEFT_THUMB,		// 左スティック
	BUTTON_RIGHT_THUMB,		// 右スティック
	BUTTON_LEFT_SHOULDER,	// 左トリガー
	BUTTON_RIGHT_SHOULDER,	// 右トリガー
	BUTTON_A,				// Aボタン
	BUTTON_B,				// Bボタン
	BUTTON_X,				// Xボタン
	BUTTON_Y,				// Yボタン
	BUTTON_LEFT_TRIGGER,	// 左トリガー
	BUTTON_RIGHT_TRIGGER,	// 右トリガー
	BUTTON_MAX,
}BUTTON;
// スティック番号
typedef enum
{
	STICK_TYPE_LEFT = 0,
	STICK_TYPE_RIGHT,
	STICK_TYPE_MAX
}STICK_TYPE;
// スティックの方向
typedef enum 
{
	STICK_ANGLE_UP = 0,
	STICK_ANGLE_DOWN,
	STICK_ANGLE_LEFT,
	STICK_ANGLE_RIGHT,
	STICK_ANGLE_MAX,
}STICK_ANGLE;

//****************************************
// 構造体の定義
//****************************************
// スティックの入力情報構造体
typedef struct 
{
	// スティックの倒し具合
	float aTplDiameter[STICK_TYPE_MAX];
	// スティックの角度
	float aAngle[STICK_TYPE_MAX];
	// スティックの方向プレス情報
	bool aAnglePress[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの方向トリガー情報
	bool aAngleTrigger[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの方向リピート情報
	bool aAngleRepeat[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの方向リリース情報
	bool aAngleRelease[STICK_TYPE_MAX][STICK_ANGLE_MAX];
}StickInput;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// 入力関連の初期化処理
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
// 入力関連の終了処理
void UninitInput(void);
// 入力関連の更新処理
void UpdateInput(void);
//========== *** 取得 ***
// XInputの状態を取得
XINPUT_STATE *GetXInputState(void);
// 画面上のカーソルの位置を取得
D3DXVECTOR3 GetCursorPosOnScreen(void);
// カーソルの移動量を取得
D3DXVECTOR3 GetCursorMove(void);
// スティックの入力情報を取得
StickInput GetStick(void);
// 動的なデバイスを取得
ACTIVE_DEVICE_TYPE GetActiveInputType(void);
//========== *** キーボード ***
// キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey);
// キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey);
// キーボードのリピート情報を取得
bool GetKeyboardRepeat(int nKey);
// キーボードのリリース情報を取得
bool GetKeyboardRelease(int nKey);
//========== *** マウス ***
// マウスのプレス情報を取得
bool GetMousePress(int nMouse);
// マウスのトリガー情報を取得
bool GetMouseTrigger(int nMouse);
// マウスのリピート情報を取得
bool GetMouseRepeat(int nMouse);
// マウスのリリース情報を取得
bool GetMouseRelease(int nMouse);
//========== *** ボタン ***
// ボタンのプレス情報を取得
bool GetButtonPress(int nButton);
// ボタンのトリガー情報を取得
bool GetButtonTrigger(int nButton);
// ボタンのリピート情報を取得
bool GetButtonRepeat(int nButton);
// ボタンのリリース情報を取得
bool GetButtonRelease(int nButton);
//========== *** コントローラー ***
// コントローラーの振動設定処理
void SetVibration(float fVibration, int nTime);
//========== *** その他 ***
// 入力情報のクリア処理
void ClearInputInfo(void);

#endif