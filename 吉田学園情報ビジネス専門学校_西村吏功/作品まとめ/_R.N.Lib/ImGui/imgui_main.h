//========================================
// 
// ImGuiのメイン処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** imgui_main.h ***
//========================================
#ifndef _IMGUI_MAIN_H_	// このマクロ定義がされていなかった時
#define _IMGUI_MAIN_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 構造体の宣言
//****************************************
// ImGuiのウィンドウ設定情報
typedef struct 
{
	char aName[TXT_MAX] = "";	// 名前
	float fWidth = 0.0f;	// 幅
	float fHeight = 0.0f;	// 高さ
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// 位置
#if /* 設定フォーマット */0
	// ImGuiのウィンドウ設定情報
	ImGuiWindowSet winSet;
	/* 名前 */sprintf(winSet.aName, "");
	/* 幅   */winSet.fWidth = ;
	/* 高さ */winSet.fHeight = ;
	/* 位置 */winSet.pos = ;
	// ImGuiのウィンドウ設定処理
	if (ImGui_SetWindow(winSet))
#endif
}ImGuiWindowSet;

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 基本接続 ***
// ImGuiの初期化処理
void ImGuiInit(HWND hWnd);
// ImGuiの終了処理
void ImGuiUninit(void);
// ImGuiのフラグ初期化処理
void ImGuiInitFlag(void);
// ImGuiの描画処理
void ImGuiDraw(void);
//========== *** 生成関連 ***
// ImGuiの生成開始処理
void ImGui_CreateStart(void);
// ImGuiの生成終了処理
void ImGui_CreateEnd(void);
// ImGuiのウィンドウ設定処理
bool ImGui_SetWindow(ImGuiWindowSet winSet);
// ImGuiのウィンドウ終了処理
bool ImGui_EndWindow(void);
// ImGuiのヘッダ設定処理
bool ImGui_SetHeader(char *pName);
//========== *** 取得 ***
// ImGuiのウィンドウにフォーカスが当たっているか取得
bool ImGui_GetWindowFocused(void);

//****************************************
// 生成サンプル
//****************************************
#if 0
{
	// ImGuiの生成開始
	ImGui_CreateStart();

	// ImGuiのウィンドウ設定情報
	ImGuiWindowSet winSet;
	/* 名前 */sprintf(winSet.aName, u8" ");
	/* 幅   */winSet.fWidth = ;
	/* 高さ */winSet.fHeight = ;
	/* 位置 */winSet.pos = ;
	// ImGuiのウィンドウ設定処理
	if (ImGui_SetWindow(winSet)) {
		do {
			if (ImGui_SetHeader(u8" ")) 
			{// ▼
				if (ImGui_SetTreeNode(u8" ")) {
					do {// ▽

					} while (!ImGui_EndTreeNode());
				}
			}
		} while (!ImGui_EndWindow());
	}

	// ImGuiの生成終了
	ImGui_CreateEnd();
}
#endif

#endif