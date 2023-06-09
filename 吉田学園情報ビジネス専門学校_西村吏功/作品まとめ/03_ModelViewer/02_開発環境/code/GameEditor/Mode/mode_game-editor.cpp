//========================================
// 
// ゲームエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game-editor.cpp ***
//========================================
#include "mode_game-editor.h"
#include "../process.h"

//****************************************
// マクロ定義
//****************************************
#define TESTEYE_TEXPATH "data_R.N.App\\TEXTURE\\MATERIAL\\eye00.png"	// テスト瞳テクスチャパス
#define GRID_SCALE (100.0f)	// グリッドの大きさ
#define GRID_DIV   (10)		// グリッドの分割数

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 入出力 *** |----------------------
//============================================================

//========================================
// LoadPartsDataGameEditor関数 - ゲームエディターの部品の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
bool Mode_GameEditor::LoadPartsDataGameEditor(ModelSetUp3D *pModelSetUp, int nParts) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (OpenFile(fileName, "data\\MODEL\\", ".x"))
	{// 開いたファイル名を取得出来た時、
		// dataを起点としたパス文字列に変換
		sprintf(fileName, ConvertToDataPath(fileName));
		// モデルの読み込み処理
		pModelSetUp->partsSet.pPartsType[nParts].nModelIdx = LoadModel(fileName);

		return true;
	}
	else 
	{// 開いたファイル名を取得出来なかった時、
		return false;
	}
}

//========================================
// LoadModelDataGameEditor関数 - ゲームエディターのモデル情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::LoadModelDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (OpenFile(fileName, "data\\GAMEOBJECT\\3D\\MODELSETUP", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// dataを起点としたパス文字列に変換
		sprintf(fileName, ConvertToDataPath(fileName));
		// モデルのセットアップ(3D)を読み込み
		m_nModelSetUpIdx = LoadModelSetUp(fileName, true, 0);
		// 読み込みフラグを真にする
		m_bLoadModel = true;
		// 部品管理初期化処理
		m_parts3D.Init(m_nModelSetUpIdx);
		// 部品(3D)のモーションを再設定
		m_parts3D.SetMotion(0);
		// 現在のファイル名を設定
		sprintf(m_aNowSetUpPath, fileName);
	}
}

//========================================
// SaveModelDataGameEditor関数 - ゲームエディターのモデル情報の書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::SaveModelDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (SaveFile(fileName, "data\\GAMEOBJECT\\3D\\MODELSETUP", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// モデルのセットアップ(3D)の書き込み処理
		SaveModelSetUp(fileName, 0);
	}
}

//========================================
// LoadMotionDataGameEditor関数 - ゲームエディターのモーション情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::LoadMotionDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (OpenFile(fileName, "data\\GAMEOBJECT\\3D\\MOTION", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// モーション(3D)の読み込み情報
		Motion3DLoad motion3DLoad;
		/* モーションのパス */motion3DLoad.pMotionPath = fileName;
		/* 再読み込みフラグ */motion3DLoad.bReload = true;
		/* モーション番号   */motion3DLoad.nMotionIdx = 0;
		// モーション(3D)を読み込み
		LoadMotion3D(motion3DLoad);
	}
}

//========================================
// SaveMotionDataGameEditor関数 - ゲームエディターのモーション情報の書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::SaveMotionDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (SaveFile(fileName, "data\\GAMEOBJECT\\3D\\MOTION", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// モーション(3D)の書き込み処理
		SaveMotion3D(fileName, 0);
	}
}

//========================================
// LoadHumanBaseGameEditor関数 - ゲームエディターのモーション情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::LoadHumanBaseGameEditor(void) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (OpenFile(fileName, "data\\GAMEOBJECT\\3D\\HUMANBASE", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// ヒューマン(3D)ベースを読み込み
		LoadHuman3DBase(fileName, true, 0, false);
	}
}

//========================================
// SaveHumanBaseGameEditor関数 - ゲームエディターのヒューマン(3D)ベース情報の書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::SaveHumanBaseGameEditor(void) {
	char fileName[TXT_MAX] = "";	// ファイル名格納

	if (SaveFile(fileName, "data\\GAMEOBJECT\\3D\\HUMANBASE", ".txt"))
	{// 開いたファイル名を取得出来た時、
		// ヒューマン(3D)ベース情報の書き込み処理
		SaveHuman3DBase(fileName, 0);
	}
}

//============================================================
//--------------------| *** ImGui *** |-----------------------
//============================================================

//========================================
// ImGuiSetGameEditor関数 - ゲームエディターのImGuiの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::ImGuiSetGameEditor(void) {
	// ImGuiの生成開始
	ImGui_CreateStart();

	// ImGuiのウィンドウ設定情報
	ImGuiWindowSet winSet;
	/* 名前 */sprintf(winSet.aName, u8"エディット");
	/* 幅   */winSet.fWidth  = 320.0f;
	/* 高さ */winSet.fHeight = SCREEN_HEIGHT;
	/* 位置 */winSet.pos     = D3DXVECTOR3(SCREEN_WIDTH - winSet.fWidth, 0.0f, 0.0f);
	// ImGuiのウィンドウ設定処理
	if (ImGui_SetWindow(winSet)) {
		do {
			{//==========モード1==========
				int nSelect = m_mode;
				char *items[] = { u8"モデルエディット",u8"キャラクタエディット" };
				m_mode = (MODE)ImGui_crtWgt_combo(u8"モード1", &nSelect, items, ARRAY_SIZE(items));
			}

			// モードに応じた処理
			switch (m_mode) {
			case MODE_00_MODELEDIT:ImGuiSetGameEditor_MODE00_MODELEDIT(); break;			
			}
		} while (!ImGui_EndWindow());
	}

	/* 名前 */sprintf(winSet.aName, u8"基本情報");
	/* 幅   */winSet.fWidth  = 320.0f;
	/* 高さ */winSet.fHeight = SCREEN_HEIGHT * 0.25f;
	/* 位置 */winSet.pos     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ImGuiのウィンドウ設定処理
	if (ImGui_SetWindow(winSet)) {
		do {
			//--------------------モデル--------------------
			ImGui_crtWgt_separatorText(u8"モデル");
			
			//===========フレーム==========
			ImGui_crtWgt_text(CreateText(u8"フレーム %d / %d", m_parts3D.GetMotionCounter(), m_parts3D.GetMotionInfo().nLoopTime));

			//==========モーション再生==========
			static bool bPlay = true;
			ImGui_crtWgt_checkBox(u8"モーション再生", &bPlay);
			m_parts3D.SetMotionStop(!bPlay);
			
			//==========コマ送り==========
			if (!bPlay) {
				m_bFrameByFrame = ImGui_crtWgt_button(u8"コマ送り");
			}

			//--------------------表示--------------------
			ImGui_crtWgt_separatorText(u8"表示");

			//==========モーション再生==========
			ImGui_crtWgt_checkBox(u8"部品の番号", &m_bShowPartsIdx);

			//==========当たり判定==========
			ImGui_crtWgt_checkBox(u8"当たり判定", &m_bShowHitTest);
		} while (!ImGui_EndWindow());
	}

	// ImGuiの生成終了
	ImGui_CreateEnd();
}

//========================================
// ゲームエディターのImGuiの設定処理(モデルエディット)
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::ImGuiSetGameEditor_MODE00_MODELEDIT(void) {
	// モデルのセットアップ(3D)情報
	ModelSetUp3D *pModelSetUp = GetModelSetUp()[0];

	{//==========モード2==========
		int nSelect = m_mode00;
		char *items[] = { u8"モデルセットアップ", u8"当たり判定作成", u8"モーション作成", u8"ヒューマンベース作成" };
		m_mode00 = (MODE00)ImGui_crtWgt_combo(u8"モード2", &nSelect, items, ARRAY_SIZE(items));
	}

	// モードに応じた処理
	switch (m_mode00) {
	case MODE00_MODELSETUP: {
		if (ImGui_SetHeader(u8"モデルセットアップ")) {// ▼
			// 編集部品内部番号
			static int nEditParts = 0;
			// 部品数のポインタ
			int *pPartsNum = &pModelSetUp->partsSet.nPartsNum;

			//==========モデルセットアップ読み込み/書き出し==========
			if (ImGui_crtWgt_button(u8"モデルセットアップ読み込み")) { LoadModelDataGameEditor(); }
			if (ImGui_crtWgt_button(u8"モデルセットアップ書き出し")) { SaveModelDataGameEditor(); }

			//--------------------部品管理情報--------------------
			ImGui_crtWgt_separatorText(u8"部品管理情報");

			{//==========モデル構成==========
				int nSelect = (int)pModelSetUp->platform;
				char *items[] = { u8"無し", u8"直立2足歩行00", u8"直立2足歩行01" };
				pModelSetUp->platform = (MODELPLATFORM)ImGui_crtWgt_combo(u8"モデル構成", &nSelect, items, ARRAY_SIZE(items));

				if (nSelect != MODELPLATFORM_NONE)
				{// ポリゴン(2D)の設定処理
					Polygon2DSet polySet;
					/* テクスチャ番号 */polySet.nTex = m_aModelPlatformTex[nSelect - 1];
					/* パターン番号   */polySet.nPtn = 0;
					/* パターン上限X  */polySet.nPtnX = 1;
					/* パターン上限Y  */polySet.nPtnY = 1;
					/* 幅             */polySet.fWidth = 128.0f;
					/* 高さ           */polySet.fHeight = 128.0f;
					/* 位置           */polySet.pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_HEIGHT - 64.0f, 0.0f);
					/* 向き           */polySet.rot = INITD3DXVECTOR3;
					/* 色             */polySet.col = INITCOLOR;
					/* カメラ合わせ   */polySet.bMatchCamera = false;
					SetPolygon2D(polySet);
				}
			}

			//==========拡大倍率(共通)==========
			ImGui_crtWgt_inputFloat(u8"拡大倍率(共通)", &pModelSetUp->partsSet.fScale);

			//==========編集部品内部番号==========
			ImGui_crtWgt_sliderInt(CreateText(u8"編集部品内部番号", *pPartsNum - 1), &nEditParts, 0, *pPartsNum);
			
			//==========部品[%02d]編集==========
			ImGui_crtWgt_separatorText(CreateText(u8"部品[%02d]編集", nEditParts));

			if (nEditParts >= *pPartsNum)
			{// 編集部品番号が現在の最大部品番号を越えている時、
				//==========追加+==========
				if (ImGui_crtWgt_button(u8"追加+")) { PartsAddGameEditor(pPartsNum, pModelSetUp); }
			}
			else
			{// 編集部品番号が現在の最大部品番号を越えていない(用意された範囲の部品選択)時、
				Parts3DType *pParts = &pModelSetUp->partsSet.pPartsType[nEditParts];

				//==========モデルパス==========
				ImGui_crtWgt_text(u8"モデルパス");
				if (ImGui_crtWgt_button(CreateText(u8"\"%s\"", GetModelInfo()->aLoadModelPath[pParts->nModelIdx]))) {
					LoadPartsDataGameEditor(pModelSetUp, nEditParts);	// 部品を読み込む
				}

				{//==========番号==========
					ImGui_crtWgt_inputInt(u8"番号", &pParts->nIdx);
					IntControl(&pParts->nIdx, INT_MAX, 0);	// 制御
				}

				{//==========親番号==========
					ImGui_crtWgt_inputInt(u8"親番号", &pParts->nParent);
					IntControl(&pParts->nParent, INT_MAX, -1);	// 制御
				}
				
				//==========相対位置==========
				ImGui_crtWgt_inputFloat3(u8"相対位置", &pParts->fixedRelativePos.x, &pParts->fixedRelativePos.y, &pParts->fixedRelativePos.z);

				{//==========相対向き==========
					D3DXVECTOR3 rot = pParts->fixedRelativeRot / D3DX_PI;	// 円周率単位で設定する為に一時保存
					ImGui_crtWgt_inputFloat3(u8"相対向き", &rot.x, &rot.y, &rot.z);
					pParts->fixedRelativeRot = rot * D3DX_PI;	// 変換して代入
				}

				//==========ライティング==========
				ImGui_crtWgt_checkBox(u8"ライティング", &pParts->bLighting);
				
				//==========削除==========
				if (ImGui_crtWgt_button(u8"削除")) { PartsSubGameEditor(pPartsNum, nEditParts, pParts, pModelSetUp); }
			}
		}

		break;
	}
	case MODE00_HITTEST: {
		if (ImGui_SetHeader(u8"当たり判定作成")) {// ▼
			static int nEditHitTest = 0;	// 編集する当たり判定の番号
			static int nEditParts   = 0;	// 編集する当たり判定の部品番号

			//==========モデル読み込み/書き出し==========
			if (ImGui_crtWgt_button(u8"モデルセットアップ読み込み")) { LoadModelDataGameEditor(); }
			if (ImGui_crtWgt_button(u8"モデルセットアップ書き出し")) { SaveModelDataGameEditor(); }

			//--------------------部品管理情報--------------------
			ImGui_crtWgt_separatorText(u8"部品管理情報");

			//==========拡大倍率(共通)==========
			ImGui_crtWgt_inputFloat(u8"拡大倍率(共通)", &pModelSetUp->partsSet.fScale);
			
			//==========編集当たり判定番号==========
			ImGui_crtWgt_sliderInt(CreateText(u8"編集当たり判定番号"), &nEditHitTest, 0, pModelSetUp->hitTestSet.nHitTestNum);
			
			//==========当たり判定編集==========
			ImGui_crtWgt_separatorText(CreateText(u8"当たり判定[%02d]編集", nEditHitTest));

			if (nEditHitTest > pModelSetUp->hitTestSet.nHitTestNum - 1)
			{// 編集当たり判定番号が現在の最大当たり判定番号を越えている時、
				//==========追加+==========
				if (ImGui_crtWgt_button(u8"追加+")) { HitTestAddGameEditor(&pModelSetUp->hitTestSet.nHitTestNum, pModelSetUp); }
			}
			else
			{// 編集当たり判定番号が現在の最大当たり判定番号を越えていない時、
				// (※用意された範囲の当たり判定選択している時)
				//==========削除-==========
				if (ImGui_crtWgt_button(u8"削除-")) {
					HitTestSubGameEditor(&pModelSetUp->hitTestSet.nHitTestNum, nEditHitTest, &pModelSetUp->hitTestSet.pHitTest[nEditHitTest], pModelSetUp);
					break;
				}
				else
				{// 削除が押されなかった時、
					// 部品数のポインタ
					int *pPartsNum = &pModelSetUp->hitTestSet.pHitTest[nEditHitTest].nHitTestPartsNum;

					//==========編集判定部品番号==========
					ImGui_crtWgt_sliderInt(u8"編集判定部品番号", &nEditParts, 0, *pPartsNum);

					//--------------------当たり判定の部品編集--------------------
					ImGui_crtWgt_separatorText(CreateText(u8"当たり判定の部品[%02d]編集", nEditParts));

					if (nEditParts > *pPartsNum - 1)
					{// 編集部品番号が現在の最大部品番号を越えている時、
						//==========追加+==========
						if (ImGui_crtWgt_button(u8"追加+")) { HitTestPartsAddGameEditor(pPartsNum, nEditHitTest, pModelSetUp); }
					}
					else
					{// 編集部品番号が現在の最大部品番号を越えていない(用意された範囲の部品選択)時、
						// 当たり判定の部品のポインタ
						HitTest3DParts *pParts = &pModelSetUp->hitTestSet.pHitTest[nEditHitTest].pHitTestParts[nEditParts];

						{//==========形==========
							int nSelect = (int)pParts->hitTestForm;
							char *items[] = { u8"球", u8"四角", u8"円筒" };
							pParts->hitTestForm = (HIT_TEST_FORM)ImGui_crtWgt_combo(u8"形", &nSelect, items, ARRAY_SIZE(items));
						}

						{//==========親番号==========
							ImGui_crtWgt_inputInt(u8"親番号", &pParts->nParent);
							IntControl(&pParts->nParent, INT_MAX, -1);
						}

						//==========大きさ==========
						ImGui_crtWgt_inputFloat3(u8"大きさ", &pParts->fWidth, &pParts->fHeight, &pParts->fDepth);

						//==========相対位置==========
						ImGui_crtWgt_inputFloat3(u8"相対位置", &pParts->fixedRelativePos.x, &pParts->fixedRelativePos.y, &pParts->fixedRelativePos.z);

						{//==========相対向き==========
							D3DXVECTOR3 rot = pParts->fixedRelativeRot / D3DX_PI;	// 円周率単位で設定する為に一時保存
							ImGui_crtWgt_inputFloat3(u8"相対向き", &rot.x, &rot.y, &rot.z);
							pParts->fixedRelativeRot = rot * D3DX_PI;	// 変換して代入
						}

						//==========削除==========
						if (ImGui_crtWgt_button(u8"削除")) { HitTestPartsSubGameEditor(pPartsNum, nEditParts, pParts, pModelSetUp); }
					}
				}
			}
		}

		break;
	}
	case MODE00_MOTION: {
		if (ImGui_SetHeader(u8"モーション作成")) {// ▼
			Motion3D *pMotion = GetMotion3D();	// モーション(3D)の情報のポインタ

			//==========モーション読み込み/書き出し==========
			if (ImGui_crtWgt_button(u8"モーション読み込み")) { LoadMotionDataGameEditor(); }
			if (ImGui_crtWgt_button(u8"モーション書き出し")) { SaveMotionDataGameEditor(); }

			//--------------------モーション管理情報--------------------
			ImGui_crtWgt_separatorText(u8"モーション管理情報");

			//==========時間==========
			ImGui_crtWgt_inputInt(u8"時間", &pMotion->nLoopTime);
			
			//==========ループ==========
			ImGui_crtWgt_checkBox(u8"ループ", &pMotion->bLoop);

			{// モーション部品数に応じたメモリ確保処理
				int nPartsMotionNumTemp = pMotion->nPartsMotionNum;	// モーション部品数を保存
				ImGui_crtWgt_holdToRepeat(CreateText(u8"モーション部品数:%d", pMotion->nPartsMotionNum), &pMotion->nPartsMotionNum);
				IntControl(&pMotion->nPartsMotionNum, INT_MAX, 0);

				if (pMotion->nPartsMotionNum > nPartsMotionNumTemp)
				{// モーション部品数が加算された時、
					// メモリ再確保
					ReGetMemory((void**)&pMotion->pPartsMotion, sizeof(PartsMotion3D), nPartsMotionNumTemp, pMotion->nPartsMotionNum);

					// 追加先の部品情報を初期化する
					pMotion->pPartsMotion[pMotion->nPartsMotionNum - 1] = {};
				}
				else if (pMotion->nPartsMotionNum > nPartsMotionNumTemp)
				{// モーション部品数が減算された時、
					// メモリ再確保
					ReGetMemory((void**)&pMotion->pPartsMotion, sizeof(PartsMotion3D), pMotion->nPartsMotionNum, pMotion->nPartsMotionNum);
				}
			}

			{//==========編集部品番号==========
				int nMax = pMotion->nPartsMotionNum - 1;
				IntControl(&nMax, INT_MAX, 0);
				ImGui_crtWgt_sliderInt(u8"編集部品番号", &m_nSelectParts, 0, nMax);
			}

			if (pMotion->nPartsMotionNum > 0)
			{// 部品毎のモーションが0を上回っている時、
				//--------------------部品のモーション編集--------------------
				ImGui_crtWgt_separatorText(CreateText(u8"部品[%02d]のモーション編集", m_nSelectParts));

				// 編集する部品のモーション情報
				PartsMotion3D *pPartsMotion = &GetMotion3D()->pPartsMotion[m_nSelectParts];
				static int nEditCommand = 0;	// 編集コマンド番号

				//==========コマンド追加+==========
				if ((ImGui_crtWgt_button(u8"コマンド追加+"))) {
					pPartsMotion->nCommandNum++;	// コマンド数を加算

					// コマンド情報のメモリ再確保
					ReGetMemory((void**)&pPartsMotion->pCommand, sizeof(Motion3DCommand), pPartsMotion->nCommandNum - 1, pPartsMotion->nCommandNum);

					// コマンド初期化
					pPartsMotion->pCommand[pPartsMotion->nCommandNum - 1] = {};

					// コマンドのポインタ
					Motion3DCommand *pCmd = &pPartsMotion->pCommand[pPartsMotion->nCommandNum - 1];

					// データのメモリを確保する
					pCmd->pData = NULL;
					GetMemory((void**)&pCmd->pData, sizeof(float), GetMotion3DCommandInfo(pCmd->command).nDataNum);

					// 全てのデータを初期化
					for (int nCntData = 0; nCntData < GetMotion3DCommandInfo(pCmd->command).nDataNum; pCmd->pData[nCntData] = 0.0f, nCntData++);
				}

				if(pPartsMotion->nCommandNum > 0)
				{//==========コマンドリスト==========
					char **ppItems = NULL;	// リストのアイテム
					GetMemory((void**)&ppItems, sizeof(char*), pPartsMotion->nCommandNum);	// 文字列ダブルポインタのメモリ確保

					for (int nCntCmd = 0; nCntCmd < pPartsMotion->nCommandNum; nCntCmd++) {
						ppItems[nCntCmd] = NULL;
						GetMemory((void**)&ppItems[nCntCmd], sizeof(char)*TXT_MAX, 1);	// 文字列ポインタのメモリ確保

						// モーションコマンド
						Motion3DCommand cmd = pPartsMotion->pCommand[nCntCmd];

						// 補間表示テキスト
						static const char aEaseText[EASE_MAX][TXT_MAX] = {
							u8"等速",
							u8"徐々に加速",
							u8"徐々に減速",
							u8"加速して減速",
						};

						switch (cmd.command) {
						case MOTION3D_COMMAND_LABEL_MOVE:
							sprintf(ppItems[nCntCmd], u8"%02d_TIME %d 移動 (%.2f %.2f %.2f) %d %s",
									nCntCmd,
									pPartsMotion->pCommand[nCntCmd].nTime,
									cmd.pData[0],
									cmd.pData[1],
									cmd.pData[2],
									(int)cmd.pData[3],
									aEaseText[(int)cmd.pData[4]]);
							break;
						case MOTION3D_COMMAND_LABEL_SPIN:
							sprintf(ppItems[nCntCmd], u8"%02d_TIME %d 回転 (%.2f %.2f %.2f) %d %s",
									nCntCmd,
									pPartsMotion->pCommand[nCntCmd].nTime,
									cmd.pData[0] / D3DX_PI,
									cmd.pData[1] / D3DX_PI,
									cmd.pData[2] / D3DX_PI,
									(int)cmd.pData[3],
									aEaseText[(int)cmd.pData[4]]);
							break;
						case MOTION3D_COMMAND_LABEL_STEP:
							sprintf(ppItems[nCntCmd], u8"%02d_TIME %d 踏む >%d %d %d>",
									nCntCmd,
									pPartsMotion->pCommand[nCntCmd].nTime,
									(int)cmd.pData[0],
									(int)cmd.pData[1]);
							break;
						}
					}

					int nSelect = nEditCommand;
					nEditCommand = ImGui_crtWgt_listBox(u8"コマンドリスト", &nSelect, ppItems, pPartsMotion->nCommandNum);
					IntControl(&nEditCommand, pPartsMotion->nCommandNum - 1, 0);

					// 動的確保したメモリの解放
					for (int i = 0; i < pPartsMotion->nCommandNum; i++) {
						ReleaseMemory((void**)&ppItems[i]);
					}
					ReleaseMemory((void**)&ppItems);
				}

				//==========上と並び替え==========
				if (ImGui_crtWgt_button(u8"上と並び替え")) {
					if (nEditCommand > 0)
					{// 編集コマンドが0を上回っている時、
						Motion3DCommand cmdTemp = pPartsMotion->pCommand[nEditCommand];
						pPartsMotion->pCommand[nEditCommand] = pPartsMotion->pCommand[nEditCommand - 1];
						pPartsMotion->pCommand[nEditCommand - 1] = cmdTemp;

						// 編集コマンドを減算
						nEditCommand--;
					}
				}

				//==========下と並び替え==========
				if (ImGui_crtWgt_button(u8"下と並び替え")) {
					if (nEditCommand < pPartsMotion->nCommandNum - 1)
					{// 編集コマンドが0を上回っている時、
						Motion3DCommand cmdTemp = pPartsMotion->pCommand[nEditCommand];
						pPartsMotion->pCommand[nEditCommand] = pPartsMotion->pCommand[nEditCommand + 1];
						pPartsMotion->pCommand[nEditCommand + 1] = cmdTemp;

						// 編集コマンドを加算
						nEditCommand++;
					}
				}

				if (pPartsMotion->nCommandNum > 0)
				{// コマンド数が0を上回っている時、
					//--------------------コマンド--------------------
					ImGui_crtWgt_separatorText(CreateText(u8"コマンド[%02d]", nEditCommand));

					// コマンドのポインタ
					Motion3DCommand *pCmd = &pPartsMotion->pCommand[nEditCommand];

					{// コマンド情報編集
						{//==========コマンドラベル==========
							int nSelectCmd  = (int)pCmd->command;
							int nSelectTemp = nSelectCmd;
							char *items[] = { u8"移動", u8"回転", u8"踏む" };
							ImGui_crtWgt_combo(u8"コマンドラベル", &nSelectCmd, items, MOTION3D_COMMAND_LABEL_MAX);
							pCmd->command = (MOTION3D_COMMAND_LABEL)nSelectCmd;

							if (nSelectCmd != nSelectTemp)
							{// 他のコマンドラベルを選択した時、
								// データのメモリを確保する
								GetMemory((void**)&pCmd->pData, sizeof(float), GetMotion3DCommandInfo(pCmd->command).nDataNum);

								// 全てのデータを初期化
								for (int nCntData = 0; nCntData < GetMotion3DCommandInfo(pCmd->command).nDataNum; pCmd->pData[nCntData] = 0.0f, nCntData++);
							}
						}

						//==========位置==========
						ImGui_crtWgt_inputInt(u8"実行タイミング", &pCmd->nTime);

						// コマンドラベルに応じた入力処理
						switch (pCmd->command) {
						case MOTION3D_COMMAND_LABEL_MOVE: {
							//==========位置==========
							ImGui_crtWgt_inputFloat3(u8"位置", &pCmd->pData[0], &pCmd->pData[1], &pCmd->pData[2]);

							{//==========移動にかかる時間==========
								int nData = (int)pCmd->pData[3];
								ImGui_crtWgt_inputInt(u8"移動にかかる時間", &nData);
								pCmd->pData[3] = nData;
							}

							{//==========補間==========
								int nSelect = (int)pCmd->pData[4];
								char *items[] = { u8"等速",u8"徐々に加速",u8"徐々に減速",u8"加速して減速" };
								pCmd->pData[4] = (EASE)ImGui_crtWgt_combo(u8"補間", &nSelect, items, ARRAY_SIZE(items));
							}

							break;
						}// MOTION3D_COMMAND_LABEL_MOVE
						case MOTION3D_COMMAND_LABEL_SPIN: {
							{//==========向き==========
								D3DXVECTOR3 rot = D3DXVECTOR3(pCmd->pData[0], pCmd->pData[1], pCmd->pData[2]) / D3DX_PI;
								ImGui_crtWgt_inputFloat3(u8"向き", &rot.x, &rot.y, &rot.z);
								pCmd->pData[0] = rot.x * D3DX_PI;
								pCmd->pData[1] = rot.y * D3DX_PI;
								pCmd->pData[2] = rot.z * D3DX_PI;
							}

							{//==========回転にかかる時間==========
								int nData = (int)pCmd->pData[3];
								ImGui_crtWgt_inputInt(u8"回転にかかる時間", &nData);
								pCmd->pData[3] = nData;
							}

							{//==========補間==========
								int nSelect = (int)pCmd->pData[4];
								char *items[] = { u8"等速",u8"徐々に加速",u8"徐々に減速",u8"加速して減速" };
								pCmd->pData[4] = (EASE)ImGui_crtWgt_combo(u8"補間", &nSelect, items, ARRAY_SIZE(items));
							}

							break;
						}// MOTION3D_COMMAND_LABEL_SPIN
						case MOTION3D_COMMAND_LABEL_STEP: {
							{//==========推移時間==========
								int nData = (int)pCmd->pData[0];
								ImGui_crtWgt_inputInt(u8"推移時間", &nData);
								pCmd->pData[0] = nData;
							}

							{//==========合計時間==========
								int nData = (int)pCmd->pData[1];
								ImGui_crtWgt_inputInt(u8"合計時間", &nData);
								pCmd->pData[1] = nData;
							}

							{//==========戻る時間==========
								int nData = (int)pCmd->pData[2];
								ImGui_crtWgt_inputInt(u8"戻る時間", &nData);
								pCmd->pData[2] = nData;
							}

							break;
						}// MOTION3D_COMMAND_LABEL_STEP
						}
					}

					//==========削除==========
					if (ImGui_crtWgt_button(u8"削除")) {
						// コマンド数を減らす
						pPartsMotion->nCommandNum--;

						// 先の部品全ての情報を次の部品の情報で上書きしていく
						for (int nCntCmd = 0; nCntCmd < pPartsMotion->nCommandNum - nEditCommand; nCntCmd++) {
							pCmd[nCntCmd] = pCmd[nCntCmd + 1];
						}

						// メモリ再確保
						ReGetMemory((void**)&pPartsMotion->pCommand, sizeof(Motion3DCommand), pPartsMotion->nCommandNum, pPartsMotion->nCommandNum);

						// 編集コマンドを減算
						nEditCommand--;

						if (nEditCommand < 0) {// 0未満制御
							nEditCommand = 0;
						}
					}
				}
			}
		}

		break;
	}
	}
}

//========================================
// PartsAddGameEditor関数 - ゲームエディターの部品追加処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::PartsAddGameEditor(int *pPartsNum, ModelSetUp3D *pModelSetUp) {
	(*pPartsNum)++;	// 部品数を加算

	// 部品のダブルポインタを取得
	CParts3D::Parts3D **ppParts = m_parts3D.GetParts3Dpp();

	// メモリ再確保
	ReGetMemory((void**)&pModelSetUp->partsSet.pPartsType,
				sizeof(Parts3DType), 
				*pPartsNum - 1, 
				*pPartsNum);
	ReGetMemory((void**)ppParts,
				sizeof(CParts3D::Parts3D),
				*pPartsNum - 1,
				*pPartsNum);

	// 追加先の部品情報を初期化する
	pModelSetUp->partsSet.pPartsType[*pPartsNum - 1] = {};
	pModelSetUp->partsSet.pPartsType[*pPartsNum - 1].nIdx = *pPartsNum - 1;
	m_parts3D.InitParts3D(*pPartsNum - 1);

	if (!LoadPartsDataGameEditor(pModelSetUp, *pPartsNum - 1))
	{// 部品を読み込めなかった時、
		(*pPartsNum)--;	// 部品数を減算

		// メモリ再確保
		ReGetMemory((void**)&pModelSetUp->partsSet.pPartsType, sizeof(Parts3DType), *pPartsNum, *pPartsNum);
		ReGetMemory((void**)ppParts, sizeof(CParts3D::Parts3D), *pPartsNum, *pPartsNum);
	}
}

//========================================
// PartsSubGameEditor関数 - ゲームエディターの部品削除処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::PartsSubGameEditor(int *pPartsNum, int nEditParts, Parts3DType *pParts, ModelSetUp3D *pModelSetUp) {
	(*pPartsNum)--;	// 部品数を減らす

	// 先の部品全ての情報を次の部品の情報で上書きしていく
	for (int nCntTip = 0; nCntTip < *pPartsNum - nEditParts; nCntTip++) {
		pParts[nCntTip] = pParts[nCntTip + 1];
	}

	// メモリ再確保
	ReGetMemory((void**)&pModelSetUp->partsSet.pPartsType, sizeof(Parts3DType), *pPartsNum + 1, *pPartsNum);
}

//========================================
// HitTestAddGameEditor関数 - ゲームエディターの当たり判定追加処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestAddGameEditor(int *pHitTestNum, ModelSetUp3D *pModelSetUp) {
	(*pHitTestNum)++;	// 当たり判定数加算

	// メモリ再確保
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest, sizeof(HitTest3D), *pHitTestNum - 1, *pHitTestNum);

	// 追加先の部品情報を初期化する
	pModelSetUp->hitTestSet.pHitTest[*pHitTestNum - 1] = {};
}

//========================================
// HitTestSubGameEditor関数 - ゲームエディターの当たり判定削除処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestSubGameEditor(int *pHitTestNum, int nEditHitTest, HitTest3D *pHitTest, ModelSetUp3D *pModelSetUp) {
	(*pHitTestNum)--;	// 当たり判定数減算

	// 先の部品全ての情報を次の部品の情報で上書きしていく
	for (int nCntTip = 0; nCntTip < *pHitTestNum - nEditHitTest; nCntTip++) {
		pHitTest[nCntTip] = pHitTest[nCntTip + 1];
	}

	// メモリ再確保
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest, sizeof(HitTest3D), *pHitTestNum, *pHitTestNum);
}

//========================================
// HitTestPartsAddGameEditor関数 - ゲームエディターの当たり判定の部品追加処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestPartsAddGameEditor(int *pPartsNum, int nEditHitTest, ModelSetUp3D *pModelSetUp) {
	(*pPartsNum)++;	// 部品数を加算

	// メモリ再確保
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest[nEditHitTest].pHitTestParts, sizeof(HitTest3DParts), *pPartsNum - 1, *pPartsNum);

	// 追加先の部品情報を初期化する
	pModelSetUp->hitTestSet.pHitTest[nEditHitTest].pHitTestParts[*pPartsNum - 1] = {};
}

//========================================
// HitTestPartsSubGameEditor関数 - ゲームエディターの当たり判定の部品削除処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestPartsSubGameEditor(int *pPartsNum, int nEditParts, HitTest3DParts *pParts, ModelSetUp3D *pModelSetUp) {
	// 部品数を減らす
	(*pPartsNum)--;

	// 先の部品全ての情報を次の部品の情報で上書きしていく
	for (int nCntTip = 0; nCntTip < *pPartsNum - nEditParts; nCntTip++) {
		pParts[nCntTip] = pParts[nCntTip + 1];
	}

	// メモリ再確保
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest->pHitTestParts, sizeof(HitTest3DParts), *pPartsNum, *pPartsNum);
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
// InitGameEditor関数 - ゲームエディターの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::Init(void) {
	// カメラ(3D)の状態を設定(マウスで宙を掴む)
	SetCamera3DState(CAMERA3D_STATE_GRAB_AIR_MOUSE);

	InitModelSetUpMemory(1);	// 編集分のモデルのセットアップ(3D)のメモリ確保
	InitMotion3DMemory(1);		// 編集分のモーションのメモリ確保
	InitHuman3DBaseMemory(1);	// 編集分のヒューマン(3D)ベースのメモリ確保

	m_parts3D.Init(0);		// 部品(3D)の初期化処理
	m_parts3D.SetMotion(0);	// 部品(3D)のモーションを設定

	{// 変数の初期化
		// 共通
		m_bShowPartsIdx = true;					// 部品の番号表示
		m_bShowHitTest  = true;					// 当たり判定表示フラグ
		m_bFrameByFrame = false;				// コマ送りフラグ
		m_mode          = MODE_00_MODELEDIT;	// モード

		// モデルエディット
		m_mode00         = MODE00_MODELSETUP;	// モード
		sprintf(m_aNowSetUpPath, "");			// 現在のセットアップファイルパス
		m_nModelSetUpIdx = 0;					// モデルのセットアップ(3D)番号
		m_nSelectParts   = 0;					// 選択している部品
		m_bLoadModel     = false;				// モデルを読み込めたかフラグ
	}

	// テスト瞳のテクスチャ読み込み
	m_nTestEyeTex = LoadTexture(TESTEYE_TEXPATH);

	// モデル構成テクスチャの読み込み
	for (int nCntPlatform = 1; nCntPlatform < MODELPLATFORM_MAX; nCntPlatform++) {
		char aLoadName[TXT_MAX] = "";	// 読み込み名

		// カウントに応じたモデル構成テクスチャを読み込む
		switch (nCntPlatform) {
		case MODELPLATFORM_BIPEDALISM_00:sprintf(aLoadName, "data_R.N.App\\TEXTURE\\MODELPLATFORM\\BIPEDALISM_00.png"); break;
		case MODELPLATFORM_BIPEDALISM_01:sprintf(aLoadName, "data_R.N.App\\TEXTURE\\MODELPLATFORM\\BIPEDALISM_01.png"); break;
		default:
			assert(false);
			break;
		}

		// テクスチャ読み込み
		m_aModelPlatformTex[nCntPlatform - 1] = LoadTexture(aLoadName);
	}
}

//========================================
// UninitGameEditor関数 - ゲームエディターの終了処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::Uninit(void) {
	// 部品(3D)の終了処理
	m_parts3D.Uninit();
}

//========================================
// UodateGameEditor関数 - ゲームエディターの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::Update(void) {
	// ImGuiの設定処理
	ImGuiSetGameEditor();

	// 当たり判定描画
	m_parts3D.DrawCollision(m_bShowHitTest);
	// 当たり判定の表示状態切り替え
	ShowHitTest(m_bShowHitTest);

	// コマ送りフラグが真の時、モーション停止解除
	if (m_bFrameByFrame) {
		m_parts3D.SetMotionStop(false);
	}

	// モードがモデルエディットの時、部品(3D)の更新処理
	if (m_mode == MODE_00_MODELEDIT) { m_parts3D.Update(); }

	// コマ送りフラグが真の時、モーション停止
	if (m_bFrameByFrame) { 
		m_parts3D.SetMotionStop(true); 
		m_bFrameByFrame = false;
	}

	// ウィンドウのフォーカス状態に応じて背景色設定
	SetBGColor(ImGui_GetWindowFocused() ? Color{ 0, 0, 0, 255 } : DEFAULTBGCOLOR);

	if (m_bShowPartsIdx) 
	{// 部品の番号表示が真の時、
		// モデルのセットアップ(3D)情報
		ModelSetUp3D *pModelSetUp = GetModelSetUp()[0];

		for (int nCntParts = 0; nCntParts < pModelSetUp->partsSet.nPartsNum; nCntParts++) {
			Parts3DType *pPartsType = &pModelSetUp->partsSet.pPartsType[nCntParts];

			// テキスト(3D)の設定処理
			SetText3D(
				CreateText("%d", pPartsType->nIdx),
				0,
				TEXT_DISP_CENTER,
				m_parts3D.GetParts3D(nCntParts).mtx,
				(m_mode00 == MODE00_MOTION) && (m_nSelectParts == pPartsType->nIdx) ? Color{ 243,191,63,255 } : INITCOLOR,
				1.0f,
				1.0f,
				false,
				false,
				true);
		}
	}

	{// グリッドの設定
		// 線(3D)の設定情報
		Line3DSet lineSet;

		// 設定色を白にする
		lineSet.col1 = INITCOLOR;
		lineSet.col2 = INITCOLOR;
		
		// Zテストをオンにする
		lineSet.bZTest = true;

		// 端までの半長さ
		float fEdge = GRID_SCALE * 0.5f;

		for (int nCntGrid = 0; nCntGrid < GRID_DIV + 1; nCntGrid++) {
			float fPosValue = -fEdge + (nCntGrid * (GRID_SCALE / GRID_DIV));

			if (nCntGrid == GRID_DIV / 2) {
				// 奥向き線
				/* 点1 */lineSet.pos1 = D3DXVECTOR3(fPosValue, 0.0f, 0.0f);
				/* 点2 */lineSet.pos2 = D3DXVECTOR3(fPosValue, 0.0f, -fEdge);
				// 線(3D)の設定処理
				SetLine3D(lineSet);

				// 設定色を赤にする
				lineSet.col1 = Color{ 255,0,0,255 };
				lineSet.col2 = Color{ 255,0,0,255 };

				// 向き把握用奥向き線
				/* 点1 */lineSet.pos1 = D3DXVECTOR3(fPosValue, 0.0f, 0.0f);
				/* 点2 */lineSet.pos2 = D3DXVECTOR3(fPosValue, 0.0f, fEdge);
				// 線(3D)の設定処理
				SetLine3D(lineSet);

				// 設定色を白にする
				lineSet.col1 = INITCOLOR;
				lineSet.col2 = INITCOLOR;
			}
			else {
				// 奥向き線
				/* 点1 */lineSet.pos1 = D3DXVECTOR3(fPosValue, 0.0f, fEdge);
				/* 点2 */lineSet.pos2 = D3DXVECTOR3(fPosValue, 0.0f, -fEdge);
				// 線(3D)の設定処理
				SetLine3D(lineSet);
			}

			// 横向き線
			/* 点1 */lineSet.pos1 = D3DXVECTOR3(fEdge, 0.0f, fPosValue);
			/* 点2 */lineSet.pos2 = D3DXVECTOR3(-fEdge, 0.0f, fPosValue);
			// 線(3D)の設定処理
			SetLine3D(lineSet);
		}
	}
}