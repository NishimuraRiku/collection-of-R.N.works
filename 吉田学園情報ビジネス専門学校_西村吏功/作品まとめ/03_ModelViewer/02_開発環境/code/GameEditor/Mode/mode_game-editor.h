//========================================
// 
// ゲームエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** mode_game-editor.h ***
//========================================
#ifndef _MODE_GAMEEDITOR_H_	// このマクロ定義がされていなかった時
#define _MODE_GAMEEDITOR_H_	// 二重インクルード防止のマクロを定義する

#include "../../../../../_R.N.Lib/R.N.Lib.h"

//****************************************
// クラス定義
//****************************************
// ゲームエディターの情報構造体
class Mode_GameEditor{
public:
	//========== *** 関数宣言 ***
	/* 初期化処理 */void Init(void);
	/* 終了処理   */void Uninit(void);
	/* 更新処理   */void Update(void);

private:
	//========== *** 列挙型定義 ***
	// ゲームエディターのモード
	typedef enum {
		MODE_00_MODELEDIT,		// モデルエディット
		MODE_MAX,
	}MODE;

	// モデルエディットのモード
	typedef enum {
		MODE00_MODELSETUP,	// モデルのセットアップ
		MODE00_HITTEST,		// 当たり判定作成
		MODE00_MOTION,		// モーション作成
		MODE00_MAX,
	}MODE00;

	//========== *** 関数宣言 ***
	/* 部品の読み込み処理                  */bool LoadPartsDataGameEditor(ModelSetUp3D *pModelSetUp, int nParts);
	/* モデルの読み込み処理                */void LoadModelDataGameEditor(void);
	/* モデルの書き込み処理                */void SaveModelDataGameEditor(void);
	/* モデルの読み込み処理                */void LoadMotionDataGameEditor(void);
	/* モデルの書き込み処理                */void SaveMotionDataGameEditor(void);
	/* ヒューマンベース情報の読み込み処理  */void LoadHumanBaseGameEditor(void);
	/* ヒューマンベース情報の書き込み処理  */void SaveHumanBaseGameEditor(void);
	/* ImGui設定処理                       */void ImGuiSetGameEditor(void);
	/* ImGui設定処理(モデルエディット)     */void ImGuiSetGameEditor_MODE00_MODELEDIT(void);
	/* 部品追加処理                        */void PartsAddGameEditor(int *pPartsNum, ModelSetUp3D *pModelSetUp);
	/* 部品削除処理                        */void PartsSubGameEditor(int *pPartsNum, int nEditParts, Parts3DType *pParts, ModelSetUp3D *pModelSetUp);
	/* 当たり判定追加処理                  */void HitTestAddGameEditor(int *pHitTestNum, ModelSetUp3D *pModelSetUp);
	/* 当たり判定削除処理                  */void HitTestSubGameEditor(int *pHitTestNum, int nEditHitTest, HitTest3D *pHitTest, ModelSetUp3D *pModelSetUp);
	/* 当たり判定の部品追加処理            */void HitTestPartsAddGameEditor(int *pPartsNum, int nEditHitTest, ModelSetUp3D *pModelSetUp);
	/* 当たり判定の部品削除処理            */void HitTestPartsSubGameEditor(int *pPartsNum, int nEditParts, HitTest3DParts *pParts, ModelSetUp3D *pModelSetUp);

	//========== *** 変数宣言 ***
	bool m_bShowPartsIdx;	// 部品の番号表示
	bool m_bShowHitTest;	// 当たり判定表示フラグ
	bool m_bFrameByFrame;	// コマ送りフラグ
	MODE m_mode;			// モード

	// モデルエディット
	MODE00 m_mode00;				// モード
	char m_aNowSetUpPath[TXT_MAX] = "";	// 現在のセットアップファイルのパス
	unsigned int m_nModelSetUpIdx;	// モデルのセットアップ(3D)番号
	int m_nSelectParts;				// 選択している部品
	int m_nTestEyeTex;				// テスト瞳テクスチャ
									// モデル構成テクスチャ
	int m_aModelPlatformTex[MODELPLATFORM_MAX - 1];
	bool m_bLoadModel;				// モデルを読み込めたかフラグ
	CParts3D m_parts3D;				// 部品(3D)
};

#endif