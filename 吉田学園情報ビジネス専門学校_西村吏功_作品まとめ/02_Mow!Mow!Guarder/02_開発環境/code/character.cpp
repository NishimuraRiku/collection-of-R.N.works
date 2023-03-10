//========================================
// 
// キャラクターの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** character.cpp ***
//========================================
#include "main.h"
#include "chr_enemy_00.h"	// CHR:敵			[00]
#include "chr_player_00.h"	// CHR:プレイヤー	[00]
#include "character.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadCharacter関数 - キャラクターの読み込み処理 -
//========================================
void LoadCharacter(void) 
{
	LoadChr_enemy_00();		// CHR:敵			[00]
	LoadChr_player_00();	// CHR:プレイヤー	[00]
}