//========================================
// 
// UI:ランキングフレーム[00] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_ranking-frame_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "polygon2D.h"
#include "text.h"
#include "md_game_00.h"				// MD :ゲーム				[00]
#include "sys_ranking_00.h"			// SYS:ランキング			[00]
#include "ui_ranking-frame_00.h"	// UI :ランキングフレーム	[00]

//****************************************
// マクロ定義
//****************************************
// UI:ランキングフレーム[00] のテクスチャの相対パス
// UI:ランキングフレーム[00] の幅
// UI:ランキングフレーム[00] の高さ
#define UI_RANKINGFRAME_00_TEXTURE_PATH		"data\\TEXTURE\\UserInterface\\ui_ranking-frame_000.png"
#define UI_RANKINGFRAME_00_TEXTURE_WIDTH	(PIXEL*256)
#define UI_RANKINGFRAME_00_TEXTURE_HEIGHT	(PIXEL*256)

// UI:ランキングフレーム[00] の出現にかかる時間
// UI:ランキングフレーム[00] の消えるのにかかる時間
#define UI_RANKINGFRAME_00_IN_POP_TIME		(10)
#define UI_RANKINGFRAME_00_IN_CLEAR_TIME	(10)

// UI:ランキングフレーム[00] のテキストの開始位置X
// UI:ランキングフレーム[00] のテキストの終了位置X
// UI:ランキングフレーム[00] のテキストの位置Y
// UI:ランキングフレーム[00] のテキストの間隔Y
// UI:ランキングフレーム[00] のテキストセットにかかる時間
// UI:ランキングフレーム[00] の名前の相対位置X
#define UI_RANKINGFRAME_00_TEXT_START_POS_X		(PIXEL*-90)
#define UI_RANKINGFRAME_00_TEXT_END_POS_X		(PIXEL*-50)
#define UI_RANKINGFRAME_00_TEXT_POS_Y			(PIXEL*-56)
#define UI_RANKINGFRAME_00_TEXT_SPACE_Y			(PIXEL*16)
#define UI_RANKINGFRAME_00_TEXT_SET_TIME		(60)
#define UI_RANKINGFRAME_00_NAME_RERATIVE_POS_X	(PIXEL*36)

// UI:ランキングフレーム[00] の点滅にかかる時間
// UI:ランキングフレーム[00] の点滅色
#define UI_RANKINGFRAME_00_BLINK_TIME	(4)
#define UI_RANKINGFRAME_00_BLINK_COLOR	Color{243,189,63,255}

// UI:ランキングフレーム[00] の矢印の相対位置X
#define UI_RANKINGFRAME_00_ARROW_RERATIVE_POS_X	(PIXEL*6*6)

// UI:ランキングフレーム[00] の上矢印の表示形式
// UI:ランキングフレーム[00] の上矢印の相対位置Y
#define UI_RANKINGFRAME_00_UP_ARROW_DISPLAY			"^"
#define UI_RANKINGFRAME_00_UP_ARROW_RERATIVE_POS_Y	(PIXEL*-8)

// UI:ランキングフレーム[00] の下矢印の表示形式
// UI:ランキングフレーム[00] の下矢印の相対位置Y
#define UI_RANKINGFRAME_00_DOWN_ARROW_DISPLAY			"_"
#define UI_RANKINGFRAME_00_DOWN_ARROW_RERATIVE_POS_Y	(PIXEL*8)

// UI:ランキングフレーム[00] の選択SE
// UI:ランキングフレーム[00] の決定SE
#define UI_RANKINGFRAME_00_SELECT_SE		(SOUND_LABEL_SE_SELECT_000)
#define UI_RANKINGFRAME_00_DETERMINATION_SE	(SOUND_LABEL_SE_DETERMINATION_001)

//****************************************
// プロトタイプ宣言
//****************************************
// UI:ランキングフレーム[00] のパラメーターの初期化処理
void InitParameterUi_rankingFrame_00(void);

// UI:ランキングフレーム[00] の状態処理
void StateProcessUi_rankingFrame_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureUi_rankingFrame_00	// テクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_rankingFrame_00	// 頂点バッファへのポインタ
						= NULL;
Ui_rankingFrame_00		g_ui_rankingFrame_00;			// UI:ランキングフレーム[00] の情報

//========== *** UI:ランキングフレーム[00] の情報を取得 ***
Ui_rankingFrame_00 *GetUi_rankingFrame_00(void) 
{
	return &g_ui_rankingFrame_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_rankingFrame_00関数 - UI:ランキングフレーム[00] のパラメーターの初期化処理 -
//========================================
void InitParameterUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ 
						= &g_ui_rankingFrame_00;

	pUi->state			= UI_RANKINGFRAME_00_STATE_CLEAR;	// 状態
	pUi->nCounterState	= 0;								// 状態カウンター
	pUi->fAlpha			= 0.0f;								// 全体の透明度
	pUi->pos			= INITD3DXVECTOR3;					// 位置
	pUi->scale			= { 1.0f,0.0f,0.0f };				// 拡大倍率
	pUi->bNameEntry		= false;							// 名前入力フラグ
	pUi->nUpdateRank	= -1;								// 更新順位
	pUi->nCounterBlink	= 0;								// 点滅カウンター
}

//========================================
// NameEntryUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の名前入力処理 -
//========================================
void NameEntryUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ 
						= &g_ui_rankingFrame_00;

	if ((!pUi->bNameEntry) || (pUi->nCounterState < UI_RANKINGFRAME_00_TEXT_SET_TIME))
	{// 名前入力フラグが偽 or 状態カウンターがテキストセットにかかる時間を下回っている時、
		return;	// 処理を終了する
	}

	if ((GetKeyboardRepeat(DIK_DOWN))
		||
		(GetKeyboardRepeat(DIK_S))
		||
		(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_DOWN])
		||
		(GetButtonRepeat(BUTTON_DOWN)))
	{// 下に入力された時、
		pUi->nCountChar--;	// 文字カウントを減算
		// 選択SEを再生
		PlaySound(UI_RANKINGFRAME_00_SELECT_SE);
	}
	else if (
		(GetKeyboardRepeat(DIK_UP))
		||
		(GetKeyboardRepeat(DIK_W))
		||
		(GetStick().aAngleRepeat[STICK_TYPE_LEFT][STICK_ANGLE_UP])
		||
		(GetButtonRepeat(BUTTON_UP)))
	{// 上に入力された時、
		pUi->nCountChar++;	// 文字カウントを加算
		// 選択SEを再生
		PlaySound(UI_RANKINGFRAME_00_SELECT_SE);
	}

	// 文字カウントをループ制御
	IntLoopControl(&pUi->nCountChar, RANKING_NAME_MAX_CHAR, RANKING_NAME_MIN_CHAR);

	if (((GetKeyboardTrigger(DIK_BACKSPACE))
		||
		(GetButtonTrigger(BUTTON_B)))
		&&
		(pUi->nCountName > 0))
	{// 削除が入力された時、かつ名前カウントが0を上回っている時、
		// 名前入力のカウントを減算
		pUi->nCountName--;

		// 現在のカウント以降の文字を空白にする
		GetSys_ranking_00()[pUi->nUpdateRank].aName[pUi->nCountName + 1] = '\0';

		// 文字カウントを設定
		pUi->nCountChar = GetSys_ranking_00()[pUi->nUpdateRank].aName[pUi->nCountName];
	}
	else if ((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A) || (GetMouseTrigger(MOUSE_LEFT))))
	{// ENTERキー or Aボタン or マウス左ボタンが入力された時、
		// 決定SEを再生
		PlaySound(UI_RANKINGFRAME_00_DETERMINATION_SE);

		if (++pUi->nCountName >= RANKING_NAME_NUM) 
		{// 名前入力のカウントを加算した結果、ランキング名の文字数に達した時、
			pUi->bNameEntry = false;	// 名前入力フラグを偽にする
			return;						// 処理を終了する
		}
		pUi->nCountChar	// 文字カウントを初期化
			= RANKING_NAME_START_CHAR;
	}

	// 現在のカウントの文字を反映する
	GetSys_ranking_00()[pUi->nUpdateRank].aName[pUi->nCountName] = pUi->nCountChar;

	// 矢印の基準位置
	D3DXVECTOR3 arrowPos = pUi->pos +
		D3DXVECTOR3(
			UI_RANKINGFRAME_00_TEXT_END_POS_X + (pUi->nCountName * (GetFont()[FONT_000].c_fSpaceX * PIXEL)) + UI_RANKINGFRAME_00_ARROW_RERATIVE_POS_X,
			UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * pUi->nUpdateRank),
			0.0f);

	// 上矢印
	SetText2D(
		UI_RANKINGFRAME_00_UP_ARROW_DISPLAY,
		FONT_002,
		DISPLAY_CENTER,
		arrowPos +
		D3DXVECTOR3(0.0f, UI_RANKINGFRAME_00_UP_ARROW_RERATIVE_POS_Y, 0.0f),
		INITCOLOR,
		1.0f,
		1.0f);

	// 下矢印
	SetText2D(
		UI_RANKINGFRAME_00_DOWN_ARROW_DISPLAY,
		FONT_002,
		DISPLAY_CENTER,
		arrowPos +
		D3DXVECTOR3(0.0f, UI_RANKINGFRAME_00_DOWN_ARROW_RERATIVE_POS_Y, 0.0f),
		INITCOLOR,
		1.0f,
		1.0f);
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ 
						= &g_ui_rankingFrame_00;

	++pUi->nCounterBlink %= (UI_RANKINGFRAME_00_BLINK_TIME * 2);	// 点滅カウンターを加算制御

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_RANKINGFRAME_00_STATE_POP:
	{
		Sys_ranking_00	*pSys	// ランキング[00] の情報のポインタ
						= GetSys_ranking_00();

		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, " %s  %-3s %8d", GetRankText(nCntRanking), pSys->aName, pSys->nScore);
			Color setColor =
				pUi->nUpdateRank == nCntRanking ?
				!(pUi->nCounterBlink / UI_RANKINGFRAME_00_BLINK_TIME) ?
				INITCOLOR : UI_RANKINGFRAME_00_BLINK_COLOR :
				INITCOLOR;
			SetText2D(
				aString,
				FONT_000,
				DISPLAY_LEFT,
				pUi->pos +
				D3DXVECTOR3(
					UI_RANKINGFRAME_00_TEXT_END_POS_X,
					UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * nCntRanking),
					0.0f),
				setColor,
				1.0f,
				1.0f);
		}
		break;
	}
		//========== *** テキストセット ***
	case UI_RANKINGFRAME_00_STATE_TEXT_SET: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		// 状態カウンターを制御
		IntControl(&pUi->nCounterState, UI_RANKINGFRAME_00_TEXT_SET_TIME, 0);

		Sys_ranking_00	*pSys	// ランキング[00] の情報のポインタ
						= GetSys_ranking_00();

		const int	nTime	// 1つのランキング当たりにかかる時間
					= UI_RANKINGFRAME_00_TEXT_SET_TIME / RANKING_NUM;

		if (pUi->nCounterState % nTime == 0) 
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
		}

		for (int nCntRanking = 0; nCntRanking < IntControlReturn((pUi->nCounterState / nTime) + 1, RANKING_NUM, 0); nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, " %s  %-3s %8d", GetRankText(nCntRanking), pSys->aName, pSys->nScore);
			float fRate =	// 進行割合
				(float)(IntControlReturn(pUi->nCounterState, nTime * (nCntRanking + 1), 0) -
				(nTime * nCntRanking)) / (float)nTime;
			SetText2D(
				aString,
				FONT_000,
				DISPLAY_LEFT,
				pUi->pos +
				D3DXVECTOR3(
					(UI_RANKINGFRAME_00_TEXT_START_POS_X * (1.0f - fRate)) + (UI_RANKINGFRAME_00_TEXT_END_POS_X * fRate),
					UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * nCntRanking),
					0.0f),
					{ 255,255,255,(int)(255 * fRate) },
				1.0f,
				1.0f);

			if (pUi->nUpdateRank == nCntRanking)
			{// 更新順位とカウントが一致している時、
				Color setColor =
					!(pUi->nCounterBlink / UI_RANKINGFRAME_00_BLINK_TIME) ?
					INITCOLOR : UI_RANKINGFRAME_00_BLINK_COLOR;
				SetText2D(
					pSys->aName,
					FONT_000,
					DISPLAY_LEFT,
					pUi->pos +
					D3DXVECTOR3(
					(UI_RANKINGFRAME_00_TEXT_START_POS_X * (1.0f - fRate)) + (UI_RANKINGFRAME_00_TEXT_END_POS_X * fRate) + UI_RANKINGFRAME_00_NAME_RERATIVE_POS_X,
						UI_RANKINGFRAME_00_TEXT_POS_Y + (UI_RANKINGFRAME_00_TEXT_SPACE_Y * nCntRanking),
						0.0f),
						{ setColor.r,setColor.g,setColor.b,(int)(255 * fRate) },
					1.0f,
					1.0f);
			}
		}

		if ((pUi->nCounterState >= UI_RANKINGFRAME_00_TEXT_SET_TIME)
			&&
			(!pUi->bNameEntry))
		{// 状態カウンターがテキストセットにかかる時間に達した & 名前入力フラグが偽の時、
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_POP);	// 状態を出現にする
		}

		break;
	}
		//========== *** 出現中 ***
	case UI_RANKINGFRAME_00_STATE_IN_POP: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_RANKINGFRAME_00_IN_POP_TIME;

		// 透明度と高さの倍率を更新
		pUi->fAlpha			= fRate;
		pUi->scale.fHeight	= fRate;

		if (pUi->nCounterState >= UI_RANKINGFRAME_00_IN_POP_TIME)
		{// 状態カウンターが出現にかかる時間に達した時、
			// 状態をテキストセットにする
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_TEXT_SET);
		}

		break;
	}
		//========== *** 消えている ***
	case UI_RANKINGFRAME_00_STATE_CLEAR: {
		break;
	}
		//========== *** 消え中 ***
	case UI_RANKINGFRAME_00_STATE_IN_CLEAR: {
		// 状態カウンターを加算
		pUi->nCounterState++;

		float	fRate	// 割合
				= (float)pUi->nCounterState / (float)UI_RANKINGFRAME_00_IN_POP_TIME;

		// 透明度と高さの倍率を更新
		pUi->fAlpha			= 1.0f - fRate;
		pUi->scale.fHeight	= 1.0f - fRate;

		if (pUi->nCounterState >= UI_RANKINGFRAME_00_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_CLEAR);
		}

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の初期化処理 -
//========================================
void InitUi_rankingFrame_00(void)
{
	// パラメーターの初期化処理
	InitParameterUi_rankingFrame_00();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_rankingFrame_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_rankingFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, UI_RANKINGFRAME_00_TEXTURE_PATH, &g_pTextureUi_rankingFrame_00);

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		false,
		UI_RANKINGFRAME_00_TEXTURE_WIDTH,
		UI_RANKINGFRAME_00_TEXTURE_HEIGHT,
		ANGLE_TYPE_FREE);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, { 255,255,255,255 });

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_rankingFrame_00->Unlock();
}

//========================================
// UninitUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の終了処理 -
//========================================
void UninitUi_rankingFrame_00(void)
{
	// テクスチャの破棄
	if (g_pTextureUi_rankingFrame_00 != NULL)
	{
		g_pTextureUi_rankingFrame_00->Release();
		g_pTextureUi_rankingFrame_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_rankingFrame_00 != NULL) 
	{
		g_pVtxBuffUi_rankingFrame_00->Release();
		g_pVtxBuffUi_rankingFrame_00 = NULL;
	}
}

//========================================
// UpdateUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の更新処理 -
//========================================
void UpdateUi_rankingFrame_00(void) 
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_rankingFrame_00->Lock(0, 0, (void**)&pVtx, 0);

	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ
						= &g_ui_rankingFrame_00;

	// 名前入力処理
	NameEntryUi_rankingFrame_00();

	// 状態処理
	StateProcessUi_rankingFrame_00();

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		g_ui_rankingFrame_00.pos,
		INITD3DXVECTOR3,
		false,
		UI_RANKINGFRAME_00_TEXTURE_WIDTH
		* pUi->scale.fWidth,
		UI_RANKINGFRAME_00_TEXTURE_HEIGHT
		* pUi->scale.fHeight,
		ANGLE_TYPE_FIXED);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, { 255,255,255,(int)(255 * g_ui_rankingFrame_00.fAlpha) });
}

//========================================
// DrawUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の描画処理 -
//========================================
void DrawUi_rankingFrame_00(void)
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ
						= &g_ui_rankingFrame_00;

	if (pUi->state == UI_RANKINGFRAME_00_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_rankingFrame_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureUi_rankingFrame_00);

	// UI:ランキングフレーム[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetState関数 - UI:ランキングフレーム[00] の状態設定処理 -
//========================================
void SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE state) 
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ
						= &g_ui_rankingFrame_00;

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}

//========================================
// SetUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の設定処理 -
//========================================
void SetUi_rankingFrame_00(D3DXVECTOR3 pos)
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ 
						= &g_ui_rankingFrame_00;

	if (pUi->state != UI_RANKINGFRAME_00_STATE_CLEAR) 
	{// 消えている状態でない時、
		// 処理を終了する
		return;
	}

	// 位置を代入
	pUi->pos = pos;

	// 出現中の状態にする
	SetStateUi_rankingFrame_00(UI_RANKINGFRAME_00_STATE_IN_POP);
}

//========================================
// SetNameEntryUi_rankingFrame_00関数 - UI:ランキングフレーム[00] の名前入力設定処理 -
//========================================
void SetNameEntryUi_rankingFrame_00(int nUpdateRank)
{
	Ui_rankingFrame_00	*pUi	// UI:ランキングフレーム[00] の情報のポインタ 
						= &g_ui_rankingFrame_00;

	pUi->nUpdateRank = nUpdateRank;	// 更新順位を代入

	if (pUi->nUpdateRank != -1) 
	{// 更新順位が-1(更新無し)でない時、
		pUi->bNameEntry = true;	// 名前入力フラグを真にする
		pUi->nCountName = 0;	// 名前カウントを初期化
		pUi->nCountChar			// 文字カウントを初期化
			= RANKING_NAME_START_CHAR;
		pUi->nCounterBlink = 0;	// 点滅カウンターを初期化

		// 更新した順位の名前を初期化
		strinit(GetSys_ranking_00()[pUi->nUpdateRank].aName, RANKING_NAME_NUM);
	}
	else 
	{// 更新処理が-1(更新無し)の時、
		pUi->bNameEntry = false;	// 名前入力フラグを偽にする
	}
}