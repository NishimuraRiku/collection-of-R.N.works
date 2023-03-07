//========================================
// 
// オブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** object.cpp ***
//========================================
#include "main.h"
#include "obj_block_00.h"		// OBJ:ブロック	[00]
#include "obj_core_00.h"		// OBJ:コア		[00]
#include "obj_discharger_00.h"	// OBJ:放電装置	[00]
#include "obj_mirror_00.h"		// OBJ:ミラー	[00]
#include "obj_pedestal_00.h"	// OBJ:台座		[00]
#include "obj_signboard_00.h"	// OBJ:看板		[00]
#include "obj_stage_00.h"		// OBJ:ステージ	[00]
#include "obj_switch_00.h"		// OBJ:スイッチ	[00]
#include "obj_turret_00.h"		// OBJ:タレット	[00]
#include "object.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObject関数 - オブジェクトの読み込み処理 -
//========================================
void LoadObject(void) 
{
	LoadObj_block_00();			// OBJ:ブロック	[00]
	LoadObj_core_00();			// OBJ:コア		[00]
	LoadObj_discharger_00();	// OBJ:放電装置	[00]
	LoadObj_mirror_00();		// OBJ:ミラー	[00]
	LoadObj_pedestal_00();		// OBJ:台座		[00]
	LoadObj_signboard_00();		// OBJ:看板		[00]
	LoadObj_stage_00();			// OBJ:ステージ	[00]
	LoadObj_switch_00();		// OBJ:スイッチ	[00]
	LoadObj_turret_00();		// OBJ:タレット	[00]
}