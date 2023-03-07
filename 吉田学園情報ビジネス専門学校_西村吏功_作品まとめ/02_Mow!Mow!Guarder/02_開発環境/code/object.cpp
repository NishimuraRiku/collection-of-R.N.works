//========================================
// 
// �I�u�W�F�N�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** object.cpp ***
//========================================
#include "main.h"
#include "obj_block_00.h"		// OBJ:�u���b�N	[00]
#include "obj_core_00.h"		// OBJ:�R�A		[00]
#include "obj_discharger_00.h"	// OBJ:���d���u	[00]
#include "obj_mirror_00.h"		// OBJ:�~���[	[00]
#include "obj_pedestal_00.h"	// OBJ:���		[00]
#include "obj_signboard_00.h"	// OBJ:�Ŕ�		[00]
#include "obj_stage_00.h"		// OBJ:�X�e�[�W	[00]
#include "obj_switch_00.h"		// OBJ:�X�C�b�`	[00]
#include "obj_turret_00.h"		// OBJ:�^���b�g	[00]
#include "object.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadObject�֐� - �I�u�W�F�N�g�̓ǂݍ��ݏ��� -
//========================================
void LoadObject(void) 
{
	LoadObj_block_00();			// OBJ:�u���b�N	[00]
	LoadObj_core_00();			// OBJ:�R�A		[00]
	LoadObj_discharger_00();	// OBJ:���d���u	[00]
	LoadObj_mirror_00();		// OBJ:�~���[	[00]
	LoadObj_pedestal_00();		// OBJ:���		[00]
	LoadObj_signboard_00();		// OBJ:�Ŕ�		[00]
	LoadObj_stage_00();			// OBJ:�X�e�[�W	[00]
	LoadObj_switch_00();		// OBJ:�X�C�b�`	[00]
	LoadObj_turret_00();		// OBJ:�^���b�g	[00]
}