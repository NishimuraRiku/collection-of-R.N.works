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

//****************************************
// マクロ定義
//****************************************
//========== *** 基本情報 ***
// キーの最大数
#define NUM_KEY_MAX (256)

//****************************************
// 列挙型定義
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
	MOUSEBUTTON_LEFT,
	MOUSEBUTTON_RIGHT,
	MOUSEBUTTON_MAX,
}MOUSEBUTTON;

// ボタン番号
typedef enum 
{
	BUTTON_UP,				// 上キー
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
	STICK_TYPE_LEFT,
	STICK_TYPE_RIGHT,
	STICK_TYPE_MAX
}STICK_TYPE;

// スティックの方向
typedef enum 
{
	STICK_ANGLE_UP,
	STICK_ANGLE_DOWN,
	STICK_ANGLE_LEFT,
	STICK_ANGLE_RIGHT,
	STICK_ANGLE_MAX,
}STICK_ANGLE;

// マウスホイールの回転状態
typedef enum
{
	WHEELSPIN_NONE,		// 無し
	WHEELSPIN_FRONT,	// 前回転
	WHEELSPIN_BACK,		// 後回転
}WHEELSPIN;

//****************************************
// 構造体定義
//****************************************

// スティックの入力情報構造体
typedef struct 
{
	// スティックの倒し具合
	float aTplDiameter[STICK_TYPE_MAX];
	// スティックの角度
	float aAngle[STICK_TYPE_MAX];
	// スティックの方向プレス情報
	BYTE aAnglePress[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの方向トリガー情報
	BYTE aAngleTrigger[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの方向リピート情報
	BYTE aAngleRepeat[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの方向リリース情報
	BYTE aAngleRelease[STICK_TYPE_MAX][STICK_ANGLE_MAX];
}StickInput;

// 入力情報構造体
typedef struct
{
	// キーボード関連
	BYTE aKeyStatePress[NUM_KEY_MAX];		// キーボードのプレス情報
	BYTE aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
	BYTE aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報
	BYTE aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報
	DWORD aKeyCurrentTime[NUM_KEY_MAX];		// キーボードの現在の時間
	DWORD aKeyExecLastTime[NUM_KEY_MAX];	// キーボードの最後に真を返した時間

	// マウス関連
	BYTE aMouseState[MOUSEBUTTON_MAX];			// マウスのプレス情報
	BYTE aMouseStateTrigger[MOUSEBUTTON_MAX];	// マウスのトリガー情報
	BYTE aMouseStateRelease[MOUSEBUTTON_MAX];	// マウスのリリース情報
	BYTE aMouseStateRepeat[MOUSEBUTTON_MAX];	// マウスのリピート情報
	DWORD aMouseCurrentTime[MOUSEBUTTON_MAX];	// マウスの現在の時間
	DWORD aMouseExecLastTime[MOUSEBUTTON_MAX];	// マウスの最後に真を返した時間
	D3DXVECTOR3 cursorPosTemp;					// カーソルの位置を保存
	D3DXVECTOR3 cursorMove;						// カーソルの移動量
	WHEELSPIN wheelSpin;						// マウスホイールの回転状態

	// コントローラー関連
	BYTE aButtonPress[BUTTON_MAX];			// ボタンのプレス情報
	BYTE aButtonTrigger[BUTTON_MAX];		// ボタンのトリガー情報
	BYTE aButtonRelease[BUTTON_MAX];		// ボタンのリリース情報
	BYTE aButtonRepeat[BUTTON_MAX];			// ボタンのリピート情報
	DWORD aButtonCurrentTime[BUTTON_MAX];	// ボタンの現在の時間
	DWORD aButtonExecLastTime[BUTTON_MAX];	// ボタンの最後に真を返した時間
	StickInput stick;						// スティックの入力情報
	// スティックの現在の時間
	DWORD aStickCurrentTime[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	// スティックの最後に真を返した時間
	DWORD aStickExecLastTime[STICK_TYPE_MAX][STICK_ANGLE_MAX];
	int nCounterVibration;		// コントローラーの振動カウンター
	int nCounterVibrationMax;	// コントローラーの振動カウンターの最大値を保存
	float fVibration;			// 振動倍率
	XINPUT_STATE xInputState;	// XInputの状態
}InputInfo;

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
// 入力情報を取得
InputInfo *GetInputInfo(void);
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
// マウスホイールの回転状態を取得
WHEELSPIN GetWheelSpin(void);
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