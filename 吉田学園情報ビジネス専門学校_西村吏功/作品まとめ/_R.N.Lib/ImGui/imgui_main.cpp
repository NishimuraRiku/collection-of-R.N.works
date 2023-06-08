//========================================
// 
// ImGuiのメイン処理の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** imgui_main.cpp ***
//========================================
#include "../R.N.Lib.h"

//****************************************
// マクロ定義
//****************************************
// フォントのパス
#define FONT_PATH "data_R.N.App\\FONT\\meiryo.ttc"
// フォントのサイズ
#define FONT_SIZE (14.0f)

//****************************************
// グローバル変数宣言
//****************************************
static bool g_bImGuiSet = false;	// ImGuiを設定したかフラグ
static bool g_bWindowFocused = false;	// ウィンドウにフォーカスが当たっているかフラグ

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// ImGuiInit関数 - ImGuiの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void ImGuiInit(HWND hWnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();	// デバイスの情報

	// DearImGuiコンテキストをセットアップ
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// キーボードの操作を有効化
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// ゲームパッドの操作を有効化
	io.Fonts->AddFontFromFileTTF(FONT_PATH, FONT_SIZE, nullptr, io.Fonts->GetGlyphRangesJapanese());

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);	// 日本語のグリフ範囲を取得
}

//========================================
// ImGuiUninit関数 - ImGuiの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void ImGuiUninit(void)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//========================================
// ImGuiInitFlag関数 - ImGuiのフラグ初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void ImGuiInitFlag(void)
{
	g_bImGuiSet = false;
	g_bWindowFocused = false;
}

//========================================
// ImGuiDraw関数 - ImGuiの描画処理 -
// Author:RIKU NISHIMURA
//========================================
void ImGuiDraw(void)
{
	if (!g_bImGuiSet) 
	{// 設定していない時、
		return;
	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//============================================================
//--------------------| *** 基本接続 *** |--------------------
//============================================================

//========================================
// ImGuiCreateStart関数 - ImGuiの生成開始処理 -
// Author:RIKU NISHIMURA
//========================================
void ImGui_CreateStart(void)
{
	g_bImGuiSet = true;

	// ImGuiフレームを開始する
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//========================================
// ImGuiCreateEnd関数 - ImGuiの生成終了処理 -
// Author:RIKU NISHIMURA
//========================================
void ImGui_CreateEnd(void)
{
	// ImGuiフレームを終了する
	ImGui::EndFrame();
}

//========================================
// ImGui_SetWindow関数 - ImGuiのウィンドウ設定処理 -
// Author:RIKU NISHIMURA
//========================================
bool ImGui_SetWindow(ImGuiWindowSet winSet)
{
	// ウィンドウのサイズ変更
	ImGui::SetNextWindowSize(ImVec2(winSet.fWidth, winSet.fHeight), ImGuiCond_Once);

	// ウィンドウの位置を設定
	ImGui::SetNextWindowPos(ImVec2(winSet.pos.x, winSet.pos.y), ImGuiCond_Always);

	// ウィンドウ名を設定
	ImGui::Begin(winSet.aName, nullptr, ImGuiWindowFlags_NoResize);

	// フォーカスが当たっているか
	g_bWindowFocused |= ImGui::IsWindowFocused();

	return true;
}

//========================================
// ImGui_EndWindow関数 - ImGuiのウィンドウ終了処理 -
// Author:RIKU NISHIMURA
//========================================
bool ImGui_EndWindow(void)
{
	ImGui::End();

	return true;
}

//========================================
// ImGui_SetHeader関数 - ImGuiのヘッダ設定処理 -
// Author:RIKU NISHIMURA
//========================================
bool ImGui_SetHeader(char *pName)
{
	return ImGui::CollapsingHeader(pName, ImGuiTreeNodeFlags_DefaultOpen);
}

//========================================
// ImGui_GetWindowFocused関数 - ImGuiのウィンドウにフォーカスが当たっているか取得 -
// Author:RIKU NISHIMURA
//========================================
bool ImGui_GetWindowFocused(void) 
{
	return g_bWindowFocused;
}