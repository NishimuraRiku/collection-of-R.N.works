//========================================
// 
// ImGuiのウィジェット生成
// Author:RIKU NISHIMURA
// 
//========================================
// *** imgui_crt-wgt.cpp ***
//========================================
#include "../R.N.Lib.h"

//****************************************
// マクロ定義
//****************************************
// リストのデフォルトの高さ
#define DEFAULT_LIST_HEIGHT (80.0f)
// スクロールバーのデフォルトの幅
#define DEFAULT_SCROLLBAR_WIDTH (16.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Basic
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** General *** |---------------------
//============================================================

bool ImGui_crtWgt_button(char *pLabelName) 
{
	return ImGui::Button(pLabelName);
}

void ImGui_crtWgt_checkBox(char *pLabelName, bool *pCheck) 
{
	ImGui::Checkbox(pLabelName, pCheck);
}

void ImGui_crtWgt_holdToRepeat(char *pLabelName, int *pData) 
{
	ImGui::Text(pLabelName);
	ImGui::SameLine();

	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::PushButtonRepeat(true);
	
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
		*pData -= 1;
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
		*pData += 1;
	}

	ImGui::PopButtonRepeat();
}

//============================================================
//--------------------| *** Inputs *** |----------------------
//============================================================

void ImGui_crtWgt_inputInt(char *pLabelName, int *pData)
{
	ImGui::InputInt(pLabelName, pData);
}

void ImGui_crtWgt_inputFloat(char *pLabelName, float *pData) 
{
	ImGui::InputFloat(pLabelName, pData);
}

void ImGui_crtWgt_inputFloat3(char *pLabelName, float *pData1, float *pData2, float *pData3) 
{
	float *aData[3] = { pData1,pData2,pData3 };
	ImGui::InputFloat3(pLabelName, *aData);
}

//============================================================
//--------------------| *** Drags *** |-----------------------
//============================================================

//============================================================
//--------------------| *** Sliders *** |---------------------
//============================================================

void ImGui_crtWgt_sliderInt(char *pLabelName, int *pData, int nMin, int nMax)
{
	ImGui::SliderInt(pLabelName, pData, nMin, nMax);
}

//============================================================
//--------------------| *** Selectors/Pickers *** |-----------
//============================================================

int ImGui_crtWgt_combo(char *pLabelName, int *pSelect, char *items[], int nItemCount) 
{
	ImGui::Combo(pLabelName, pSelect, items, nItemCount);
	return *pSelect;
}

int ImGui_crtWgt_listBox(char *pLabelName, int *pSelect, char *items[], int nItemCount)
{
	// リストの高さ
	float fListHeight = DEFAULT_LIST_HEIGHT;

	// スクロールバーのサイズを設定する
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, DEFAULT_SCROLLBAR_WIDTH);

	if (ImGui::BeginListBox(pLabelName, ImVec2(208.0f, fListHeight)))
	{
		for (int n = 0; n < nItemCount; n++)
		{
			const bool is_selected = (*pSelect == n);
			if (ImGui::Selectable(items[n], is_selected))
				*pSelect = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	// スタイル変数をポップする
	ImGui::PopStyleVar();

	return *pSelect;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Trees
//----------|---------------------------------------------------------------------
//================================================================================

bool ImGui_SetTreeNode(char *pName)
{
	// ツリーノードを最初から開いた状態で設定する
	ImGui::SetNextItemOpen(true);

	return ImGui::TreeNode(pName);
}

bool ImGui_EndTreeNode(void)
{
	ImGui::TreePop();

	return true;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Text
//----------|---------------------------------------------------------------------
//================================================================================

void ImGui_crtWgt_text(char *pText) 
{
	ImGui::Text(pText);
}

void ImGui_crtWgt_separatorText(char *pText)
{
	ImGui::SeparatorText(pText);
}