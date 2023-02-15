//========================================
// 
// スコア[00]の処理
// Author:西村 吏功
// 
//========================================
// *** ui_score_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chr_fighter_00.h"
#include "md_game_00.h"
#include "ui_score_00.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************
#define UI_SCORE_00_LIFE			(40)				// スコア[00] の寿命
#define UI_SCORE_00_POP_TIME		(20)				// スコア[00] の出現にかかる時間
#define UI_SCORE_00_CLEAR_TIME		(20)				// スコア[00] のクリアにかかる時間
#define UI_SCORE_00_SCALE_DIAMETER	(0.005f)			// スコア[00] の大きさにかかる倍率
#define UI_SCORE_00_SCALE_MIN		(1.0f)				// スコア[00] の大きさの最小
#define UI_SCORE_00_SCALE_MAX		(4.0f)				// スコア[00] の大きさの最大
#define UI_SCORE_00_BRINK_COLOR		{243, 191, 63, 255}	// スコア[00] の点滅時の色
#define UI_SCORE_00_BRINK_TIME		(4)					// スコア[00] の点滅時にかかる時間
#define BLINK_SCORE_LINE			(1000)				// スコア[00] の点滅ライン

//****************************************
// グローバル宣言
//****************************************
Ui_score_00 g_aUi_score_00[UI_SCORE_00_MAX];	// スコア[00] の情報

//========== *** スコア[00] の情報を取得 ***
Ui_score_00 *GetUi_score_00(void) 
{
	return g_aUi_score_00;
}

//========================================
// InitUi_score_00関数 - スコア[00]の初期化処理 -
//========================================
void InitUi_score_00(void)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++) 
	{
		g_aUi_score_00[nCntUi_score_00].bUse = false; // 使用されているかフラグ を初期化する
		g_aUi_score_00[nCntUi_score_00].rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);	// 向きを初期化
	}
}

//========================================
// UninitUi_score_00関数 - スコア[00]の終了処理 -
//========================================
void UninitUi_score_00(void)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++)
	{
		// 非表示にする
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, false);
	}
}

//========================================
// UpdateUi_score_00関数 - スコア[00]の更新処理 -
//========================================
void UpdateUi_score_00(void)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++)
	{
		if (g_aUi_score_00[nCntUi_score_00].bUse)
		{// スコア[00] が使用されている時、
			// 表示する
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, true);
		}
		else 
		{// スコア[00] が使用されていない時、
			// 非表示にする
			ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, false);

			continue;
		}

		// 寿命を減算する
		g_aUi_score_00[nCntUi_score_00].nLife--;

		if (g_aUi_score_00[nCntUi_score_00].nLife <= 0)
		{// 寿命が尽きた時、
			// 使用していない状態にする
			g_aUi_score_00[nCntUi_score_00].bUse = false;
		}

		D3DXVECTOR3 *move	= &g_aUi_score_00[nCntUi_score_00].move;	// 移動量
		float fWidth		= g_aUi_score_00[nCntUi_score_00].fWidth;	// 幅
		float fHeight		= g_aUi_score_00[nCntUi_score_00].fHeight;	// 高さ

		// 位置 を更新
		g_aUi_score_00[nCntUi_score_00].pos.x += move->x;
		g_aUi_score_00[nCntUi_score_00].pos.y += move->y;

		// 移動量を更新(減衰)
		move->x += (0.0f - move->x) * 0.1f;
		move->y += (0.0f - move->y) * 0.1f;
		move->z += (0.0f - move->z) * 0.1f;

		D3DXVECTOR3 pos = g_aUi_score_00[nCntUi_score_00].pos;	// 位置

		// 位置を合わせる
		MatchCameraPosition(&pos);

		if (g_aUi_score_00[nCntUi_score_00].nScore >= BLINK_SCORE_LINE)
		{// スコアが点滅ラインを越えている時、
			// 点滅カウンターを加算
			g_aUi_score_00[nCntUi_score_00].nCounterBrink++;

			if (g_aUi_score_00[nCntUi_score_00].nCounterBrink >= UI_SCORE_00_BRINK_TIME) 
			{// 点滅カウンターが点滅にかかる時間に達した時、
				// 点滅フラグを初期化
				g_aUi_score_00[nCntUi_score_00].nCounterBrink = 0;

				// 点滅フラグを切り替える
				g_aUi_score_00[nCntUi_score_00].bBrink ^= 1;
			}
		}
		
		{
			char aString[TXT_MAX];	// 文字列

			// 色を設定
			Color col = { 255,255,255,255 };

			if (g_aUi_score_00[nCntUi_score_00].bBrink) 
			{
				col = UI_SCORE_00_BRINK_COLOR;
			}

			float fScore = g_aUi_score_00[nCntUi_score_00].nScore;	// 拡大率の基準となるスコアの値
			float fScale = (fScore * ((float)g_aUi_score_00[nCntUi_score_00].nLife / (float)UI_SCORE_00_LIFE)) * UI_SCORE_00_SCALE_DIAMETER;

			// 大きさを制御
			if (fScale < UI_SCORE_00_SCALE_MIN) 
			{
				fScale = UI_SCORE_00_SCALE_MIN;
			}
			else if (fScale > UI_SCORE_00_SCALE_MAX)
			{
				fScale = UI_SCORE_00_SCALE_MAX;
			}

			sprintf(aString, "%d", g_aUi_score_00[nCntUi_score_00].nScore);
			UpdateTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE_DISP + nCntUi_score_00, aString, pos, col, TEXT_MODE_CENTER, FONT_002, fScale, fScale);
		}
		
	}
}

//========================================
// SetUi_score_00関数 - スコア[00]の設定処理 -
//========================================
void SetUi_score_00(D3DXVECTOR3 pos, int nScore)
{
	for (int nCntUi_score_00 = 0; nCntUi_score_00 < UI_SCORE_00_MAX; nCntUi_score_00++)
	{
		if (!g_aUi_score_00[nCntUi_score_00].bUse)
		{// スコア[00]が使用されていない時

			// スコアを代入
			g_aUi_score_00[nCntUi_score_00].nScore = nScore;

			// 位置を代入
			g_aUi_score_00[nCntUi_score_00].pos = pos;

			// 寿命を設定する
			g_aUi_score_00[nCntUi_score_00].nLife = UI_SCORE_00_LIFE;

			// 点滅カウンター/点滅フラグを初期化する
			g_aUi_score_00[nCntUi_score_00].nCounterBrink = 0;
			g_aUi_score_00[nCntUi_score_00].bBrink = false;

			// 使用している状態にする
			g_aUi_score_00[nCntUi_score_00].bUse = true;

			break;
		}
	}
}