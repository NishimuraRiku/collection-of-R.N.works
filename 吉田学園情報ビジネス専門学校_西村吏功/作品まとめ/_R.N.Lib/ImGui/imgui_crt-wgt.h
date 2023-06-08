//========================================
// 
// ImGuiのウィジェット生成のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** imgui_crt-wgt_basic.h ***
//========================================
#ifndef _IMGUI_CRTWGT_BASIC_H_	// このマクロ定義がされていなかった時
#define _IMGUI_CRTWGT_BASIC_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
//==================== Basic
//========== *** General ***
/* button         */bool ImGui_crtWgt_button(char *pLabelName);
/* checkbox       */void ImGui_crtWgt_checkBox(char *pLabelName, bool *pCheck);
/* radio          */
/* click          */
/* hold to repeat */void ImGui_crtWgt_holdToRepeat(char *pLabelName, int *pData);
/* tool tips      */
//========== *** Inputs ***
/* input text           */
/* input text (w/ hint) */
/* input int            */void ImGui_crtWgt_inputInt(char *pLabelName, int *pData);
/* input float          */void ImGui_crtWgt_inputFloat(char *pLabelName, float *pData);
/* input double         */
/* input scientifc      */
/* input float3         */void ImGui_crtWgt_inputFloat3(char *pLabelName, float *pData1, float *pData2, float *pData3);
//========== *** Drags ***
/* drag int         */
/* drag int 0..100  */
/* drag float       */
/* drag small float */
//========== *** Sliders ***
/* slider int        */void ImGui_crtWgt_sliderInt(char *pLabelName, int *pData, int nMin, int nMax);
/* slider float      */
/* slider float(log) */
/* slider angle      */
/* slider enum       */
//========== *** Selectors/Pickers ***
/* color 1  */
/* color 2  */
/* combo    */int ImGui_crtWgt_combo(char *pLabelName, int *pSelect, char *items[], int nItemCount);
/* list box */int ImGui_crtWgt_listBox(char *pLabelName, int *pSelect, char *items[], int nItemCount);
//==================== Trees
/* tree node set */bool ImGui_SetTreeNode(char *pName);
/* tree node end */bool ImGui_EndTreeNode(void);
//==================== Text
/* text           */void ImGui_crtWgt_text(char *pText);
/* separator text */void ImGui_crtWgt_separatorText(char *pText);

//****************************************
// 生成サンプル
//****************************************
#if 0
{
	//==================== Basic

	//========== General

	// button
	if (ImGui_crtWgt_button(u8" "))
	{

	}

	// combo
	static int nSelect = 0;
	char *items[] = { u8" ", u8" " };
	ImGui_crtWgt_combo(u8" ", &nSelect, items, ARRAY_SIZE(items));

	// list box
	static int nSelect = 0;
	const char *items[] = { u8" ", u8" " };
	ImGui_crtWgt_listBox(u8" ", &nSelect, items, ARRAY_SIZE(items));

	//==================== Text

	// Text
	ImGui_crtWgt_text(u8" ");
}
#endif

#endif