//========================================
// 
// R.N.Libの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** R.N.Lib.cpp ***
//========================================
#include <time.h>
#include "R.N.Lib.h"

//****************************************
// 前方宣言
//****************************************
// imgui_impl_win32.cppのメッセージハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNLibクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
RNLib::RNLib(void) {

}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
RNLib::~RNLib(void) {

}

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::Init(HINSTANCE hInstance) {
	// レンダーステートの設定処理
	SetRenderStateRN();

	// 乱数の種を取得
	srand((unsigned int)time(0));

	// 3D
	InitCamera3D();	// カメラ(3D)
	InitLight();	// 光
	InitLine3D();	// 線(3D)
	InitModel();	// モデル
	InitPolygon3D();// ポリゴン(3D)
	// 2D
	InitPolygon2D();// ポリゴン(2D)
	// Graphic
	InitText();		// テキスト
	// Other
	InitInput(		// 入力関連の初期化処理
		hInstance,
		*GetWindowHandle());
	InitPhysics();	// 物理関連
	InitSound(		// サウンド
		*GetWindowHandle());
	InitSetting();	// 設定

	// ImGuiの初期化処理
	ImGuiInit(*GetWindowHandle());
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::Uninit(void) {
	// ImGuiの終了処理
	ImGuiUninit();

	// 3D
	UninitHuman3DBase();// ヒューマン(3D)ベース
	UninitModelSetUp();	// モデルセットアップ
	UninitCamera3D();	// カメラ(3D)
	UninitLight();		// 光
	UninitLine3D();		// 線(3D)
	UninitModel();		// モデル
	UninitPolygon3D();	// ポリゴン(3D)
	// 2D
	UninitPolygon2D();	// ポリゴン(2D)
	// Other
	UninitInput();		// 入力関連
	UninitSetting();	// 設定
	UninitSound();		// サウンド
	UninitWindow();		// ウィンドウ
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::Update(void) {
	// 当たり判定の線の数を初期化
	InitHitTest3DLineNum();

	UpdateSetting();	// 設定
	UpdateCamera3D();	// カメラ(3D)
	UpdateLight();		// 光
	UpdateSound();		// サウンド
	UpdateWindowRN();	// ウィンドウ
	DrawHitTest3DSetUp();	// 当たり判定の描画準備処理
	DrawHitTestSignOut();	// 当たり判定の登録解除処理
	UpdateInput();		// 入力関連

	// ImGuiのフラグ初期化処理
	ImGuiInitFlag();
}

//========================================
// 描画処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::Draw(void) {
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 画面クリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DXCOLOR(GetColorInfo()->bgCol), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{// 描画開始が成功した場合、
		SetCamera3D();		// カメラ(3D)の設定処理

		Draw3D();			// 3D:3Dオブジェクト全般
		DrawLine3D();		// 3D:線(3D)
		DrawHitTest3D();	// 3D:当たり判定
		DrawPolygon2D();	// 2D:ポリゴン(2D)

		// ImGuiの描画処理
		ImGuiDraw();

		// 描画終了
		pDevice->EndScene();
	}

	// ポリゴン(2D & 3D)の設定数を初期化
	GetPolygon2DControl()->nPolygonNum = 0;
	GetPolygon3DControl()->nPolygonNum = 0;
	// 線(3D)の数を初期化
	GetLine3DControl()->nLineNum = 0;
	// モデルの設定数を初期化
	GetModelInfo()->nModelSetNum = 0;

	// バックバッファをフロントバッファの入れ替え
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// ウィンドウプロシージャの延長先
// Author:RIKU NISHIMURA
//========================================
void RNLib::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{// ImGuiのウィンドウプロシージャの延長先
		return;
	}

	switch (uMsg)
	{
	case WM_MOUSEWHEEL: // マウスホイールが回された時のメッセージ
		if (HIWORD(wParam) == WHEEL_DELTA)
		{// マウスホイールの回転状態を前回転に設定
			GetInputInfo()->wheelSpin = WHEELSPIN_FRONT;
		}
		else
		{// マウスホイールの回転状態を後回転に設定
			GetInputInfo()->wheelSpin = WHEELSPIN_BACK;
		}

		break;
	}
}

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// 読み込み処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::Load(void) {
	LoadFont();	// フォント
}

//========================================
// 書き込み処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::Save(void) {
	
}

//========================================
// レンダーステートの設定処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::SetRenderStateRN(void) {
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 片面カリング設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// アルファブレンドの設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステート設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}