//========================================
// 
// �Q�[���G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game-editor.cpp ***
//========================================
#include "mode_game-editor.h"
#include "../process.h"

//****************************************
// �}�N����`
//****************************************
#define TESTEYE_TEXPATH "data_R.N.App\\TEXTURE\\MATERIAL\\eye00.png"	// �e�X�g���e�N�X�`���p�X
#define GRID_SCALE (100.0f)	// �O���b�h�̑傫��
#define GRID_DIV   (10)		// �O���b�h�̕�����

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ���o�� *** |----------------------
//============================================================

//========================================
// LoadPartsDataGameEditor�֐� - �Q�[���G�f�B�^�[�̕��i�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
bool Mode_GameEditor::LoadPartsDataGameEditor(ModelSetUp3D *pModelSetUp, int nParts) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (OpenFile(fileName, "data\\MODEL\\", ".x"))
	{// �J�����t�@�C�������擾�o�������A
		// data���N�_�Ƃ����p�X������ɕϊ�
		sprintf(fileName, ConvertToDataPath(fileName));
		// ���f���̓ǂݍ��ݏ���
		pModelSetUp->partsSet.pPartsType[nParts].nModelIdx = LoadModel(fileName);

		return true;
	}
	else 
	{// �J�����t�@�C�������擾�o���Ȃ��������A
		return false;
	}
}

//========================================
// LoadModelDataGameEditor�֐� - �Q�[���G�f�B�^�[�̃��f�����̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::LoadModelDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (OpenFile(fileName, "data\\GAMEOBJECT\\3D\\MODELSETUP", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// data���N�_�Ƃ����p�X������ɕϊ�
		sprintf(fileName, ConvertToDataPath(fileName));
		// ���f���̃Z�b�g�A�b�v(3D)��ǂݍ���
		m_nModelSetUpIdx = LoadModelSetUp(fileName, true, 0);
		// �ǂݍ��݃t���O��^�ɂ���
		m_bLoadModel = true;
		// ���i�Ǘ�����������
		m_parts3D.Init(m_nModelSetUpIdx);
		// ���i(3D)�̃��[�V�������Đݒ�
		m_parts3D.SetMotion(0);
		// ���݂̃t�@�C������ݒ�
		sprintf(m_aNowSetUpPath, fileName);
	}
}

//========================================
// SaveModelDataGameEditor�֐� - �Q�[���G�f�B�^�[�̃��f�����̏������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::SaveModelDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (SaveFile(fileName, "data\\GAMEOBJECT\\3D\\MODELSETUP", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// ���f���̃Z�b�g�A�b�v(3D)�̏������ݏ���
		SaveModelSetUp(fileName, 0);
	}
}

//========================================
// LoadMotionDataGameEditor�֐� - �Q�[���G�f�B�^�[�̃��[�V�������̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::LoadMotionDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (OpenFile(fileName, "data\\GAMEOBJECT\\3D\\MOTION", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// ���[�V����(3D)�̓ǂݍ��ݏ��
		Motion3DLoad motion3DLoad;
		/* ���[�V�����̃p�X */motion3DLoad.pMotionPath = fileName;
		/* �ēǂݍ��݃t���O */motion3DLoad.bReload = true;
		/* ���[�V�����ԍ�   */motion3DLoad.nMotionIdx = 0;
		// ���[�V����(3D)��ǂݍ���
		LoadMotion3D(motion3DLoad);
	}
}

//========================================
// SaveMotionDataGameEditor�֐� - �Q�[���G�f�B�^�[�̃��[�V�������̏������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::SaveMotionDataGameEditor(void) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (SaveFile(fileName, "data\\GAMEOBJECT\\3D\\MOTION", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// ���[�V����(3D)�̏������ݏ���
		SaveMotion3D(fileName, 0);
	}
}

//========================================
// LoadHumanBaseGameEditor�֐� - �Q�[���G�f�B�^�[�̃��[�V�������̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::LoadHumanBaseGameEditor(void) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (OpenFile(fileName, "data\\GAMEOBJECT\\3D\\HUMANBASE", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// �q���[�}��(3D)�x�[�X��ǂݍ���
		LoadHuman3DBase(fileName, true, 0, false);
	}
}

//========================================
// SaveHumanBaseGameEditor�֐� - �Q�[���G�f�B�^�[�̃q���[�}��(3D)�x�[�X���̏������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::SaveHumanBaseGameEditor(void) {
	char fileName[TXT_MAX] = "";	// �t�@�C�����i�[

	if (SaveFile(fileName, "data\\GAMEOBJECT\\3D\\HUMANBASE", ".txt"))
	{// �J�����t�@�C�������擾�o�������A
		// �q���[�}��(3D)�x�[�X���̏������ݏ���
		SaveHuman3DBase(fileName, 0);
	}
}

//============================================================
//--------------------| *** ImGui *** |-----------------------
//============================================================

//========================================
// ImGuiSetGameEditor�֐� - �Q�[���G�f�B�^�[��ImGui�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::ImGuiSetGameEditor(void) {
	// ImGui�̐����J�n
	ImGui_CreateStart();

	// ImGui�̃E�B���h�E�ݒ���
	ImGuiWindowSet winSet;
	/* ���O */sprintf(winSet.aName, u8"�G�f�B�b�g");
	/* ��   */winSet.fWidth  = 320.0f;
	/* ���� */winSet.fHeight = SCREEN_HEIGHT;
	/* �ʒu */winSet.pos     = D3DXVECTOR3(SCREEN_WIDTH - winSet.fWidth, 0.0f, 0.0f);
	// ImGui�̃E�B���h�E�ݒ菈��
	if (ImGui_SetWindow(winSet)) {
		do {
			{//==========���[�h1==========
				int nSelect = m_mode;
				char *items[] = { u8"���f���G�f�B�b�g",u8"�L�����N�^�G�f�B�b�g" };
				m_mode = (MODE)ImGui_crtWgt_combo(u8"���[�h1", &nSelect, items, ARRAY_SIZE(items));
			}

			// ���[�h�ɉ���������
			switch (m_mode) {
			case MODE_00_MODELEDIT:ImGuiSetGameEditor_MODE00_MODELEDIT(); break;			
			}
		} while (!ImGui_EndWindow());
	}

	/* ���O */sprintf(winSet.aName, u8"��{���");
	/* ��   */winSet.fWidth  = 320.0f;
	/* ���� */winSet.fHeight = SCREEN_HEIGHT * 0.25f;
	/* �ʒu */winSet.pos     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ImGui�̃E�B���h�E�ݒ菈��
	if (ImGui_SetWindow(winSet)) {
		do {
			//--------------------���f��--------------------
			ImGui_crtWgt_separatorText(u8"���f��");
			
			//===========�t���[��==========
			ImGui_crtWgt_text(CreateText(u8"�t���[�� %d / %d", m_parts3D.GetMotionCounter(), m_parts3D.GetMotionInfo().nLoopTime));

			//==========���[�V�����Đ�==========
			static bool bPlay = true;
			ImGui_crtWgt_checkBox(u8"���[�V�����Đ�", &bPlay);
			m_parts3D.SetMotionStop(!bPlay);
			
			//==========�R�}����==========
			if (!bPlay) {
				m_bFrameByFrame = ImGui_crtWgt_button(u8"�R�}����");
			}

			//--------------------�\��--------------------
			ImGui_crtWgt_separatorText(u8"�\��");

			//==========���[�V�����Đ�==========
			ImGui_crtWgt_checkBox(u8"���i�̔ԍ�", &m_bShowPartsIdx);

			//==========�����蔻��==========
			ImGui_crtWgt_checkBox(u8"�����蔻��", &m_bShowHitTest);
		} while (!ImGui_EndWindow());
	}

	// ImGui�̐����I��
	ImGui_CreateEnd();
}

//========================================
// �Q�[���G�f�B�^�[��ImGui�̐ݒ菈��(���f���G�f�B�b�g)
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::ImGuiSetGameEditor_MODE00_MODELEDIT(void) {
	// ���f���̃Z�b�g�A�b�v(3D)���
	ModelSetUp3D *pModelSetUp = GetModelSetUp()[0];

	{//==========���[�h2==========
		int nSelect = m_mode00;
		char *items[] = { u8"���f���Z�b�g�A�b�v", u8"�����蔻��쐬", u8"���[�V�����쐬", u8"�q���[�}���x�[�X�쐬" };
		m_mode00 = (MODE00)ImGui_crtWgt_combo(u8"���[�h2", &nSelect, items, ARRAY_SIZE(items));
	}

	// ���[�h�ɉ���������
	switch (m_mode00) {
	case MODE00_MODELSETUP: {
		if (ImGui_SetHeader(u8"���f���Z�b�g�A�b�v")) {// ��
			// �ҏW���i�����ԍ�
			static int nEditParts = 0;
			// ���i���̃|�C���^
			int *pPartsNum = &pModelSetUp->partsSet.nPartsNum;

			//==========���f���Z�b�g�A�b�v�ǂݍ���/�����o��==========
			if (ImGui_crtWgt_button(u8"���f���Z�b�g�A�b�v�ǂݍ���")) { LoadModelDataGameEditor(); }
			if (ImGui_crtWgt_button(u8"���f���Z�b�g�A�b�v�����o��")) { SaveModelDataGameEditor(); }

			//--------------------���i�Ǘ����--------------------
			ImGui_crtWgt_separatorText(u8"���i�Ǘ����");

			{//==========���f���\��==========
				int nSelect = (int)pModelSetUp->platform;
				char *items[] = { u8"����", u8"����2�����s00", u8"����2�����s01" };
				pModelSetUp->platform = (MODELPLATFORM)ImGui_crtWgt_combo(u8"���f���\��", &nSelect, items, ARRAY_SIZE(items));

				if (nSelect != MODELPLATFORM_NONE)
				{// �|���S��(2D)�̐ݒ菈��
					Polygon2DSet polySet;
					/* �e�N�X�`���ԍ� */polySet.nTex = m_aModelPlatformTex[nSelect - 1];
					/* �p�^�[���ԍ�   */polySet.nPtn = 0;
					/* �p�^�[�����X  */polySet.nPtnX = 1;
					/* �p�^�[�����Y  */polySet.nPtnY = 1;
					/* ��             */polySet.fWidth = 128.0f;
					/* ����           */polySet.fHeight = 128.0f;
					/* �ʒu           */polySet.pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_HEIGHT - 64.0f, 0.0f);
					/* ����           */polySet.rot = INITD3DXVECTOR3;
					/* �F             */polySet.col = INITCOLOR;
					/* �J�������킹   */polySet.bMatchCamera = false;
					SetPolygon2D(polySet);
				}
			}

			//==========�g��{��(����)==========
			ImGui_crtWgt_inputFloat(u8"�g��{��(����)", &pModelSetUp->partsSet.fScale);

			//==========�ҏW���i�����ԍ�==========
			ImGui_crtWgt_sliderInt(CreateText(u8"�ҏW���i�����ԍ�", *pPartsNum - 1), &nEditParts, 0, *pPartsNum);
			
			//==========���i[%02d]�ҏW==========
			ImGui_crtWgt_separatorText(CreateText(u8"���i[%02d]�ҏW", nEditParts));

			if (nEditParts >= *pPartsNum)
			{// �ҏW���i�ԍ������݂̍ő啔�i�ԍ����z���Ă��鎞�A
				//==========�ǉ�+==========
				if (ImGui_crtWgt_button(u8"�ǉ�+")) { PartsAddGameEditor(pPartsNum, pModelSetUp); }
			}
			else
			{// �ҏW���i�ԍ������݂̍ő啔�i�ԍ����z���Ă��Ȃ�(�p�ӂ��ꂽ�͈͂̕��i�I��)���A
				Parts3DType *pParts = &pModelSetUp->partsSet.pPartsType[nEditParts];

				//==========���f���p�X==========
				ImGui_crtWgt_text(u8"���f���p�X");
				if (ImGui_crtWgt_button(CreateText(u8"\"%s\"", GetModelInfo()->aLoadModelPath[pParts->nModelIdx]))) {
					LoadPartsDataGameEditor(pModelSetUp, nEditParts);	// ���i��ǂݍ���
				}

				{//==========�ԍ�==========
					ImGui_crtWgt_inputInt(u8"�ԍ�", &pParts->nIdx);
					IntControl(&pParts->nIdx, INT_MAX, 0);	// ����
				}

				{//==========�e�ԍ�==========
					ImGui_crtWgt_inputInt(u8"�e�ԍ�", &pParts->nParent);
					IntControl(&pParts->nParent, INT_MAX, -1);	// ����
				}
				
				//==========���Έʒu==========
				ImGui_crtWgt_inputFloat3(u8"���Έʒu", &pParts->fixedRelativePos.x, &pParts->fixedRelativePos.y, &pParts->fixedRelativePos.z);

				{//==========���Ό���==========
					D3DXVECTOR3 rot = pParts->fixedRelativeRot / D3DX_PI;	// �~�����P�ʂŐݒ肷��ׂɈꎞ�ۑ�
					ImGui_crtWgt_inputFloat3(u8"���Ό���", &rot.x, &rot.y, &rot.z);
					pParts->fixedRelativeRot = rot * D3DX_PI;	// �ϊ����đ��
				}

				//==========���C�e�B���O==========
				ImGui_crtWgt_checkBox(u8"���C�e�B���O", &pParts->bLighting);
				
				//==========�폜==========
				if (ImGui_crtWgt_button(u8"�폜")) { PartsSubGameEditor(pPartsNum, nEditParts, pParts, pModelSetUp); }
			}
		}

		break;
	}
	case MODE00_HITTEST: {
		if (ImGui_SetHeader(u8"�����蔻��쐬")) {// ��
			static int nEditHitTest = 0;	// �ҏW���铖���蔻��̔ԍ�
			static int nEditParts   = 0;	// �ҏW���铖���蔻��̕��i�ԍ�

			//==========���f���ǂݍ���/�����o��==========
			if (ImGui_crtWgt_button(u8"���f���Z�b�g�A�b�v�ǂݍ���")) { LoadModelDataGameEditor(); }
			if (ImGui_crtWgt_button(u8"���f���Z�b�g�A�b�v�����o��")) { SaveModelDataGameEditor(); }

			//--------------------���i�Ǘ����--------------------
			ImGui_crtWgt_separatorText(u8"���i�Ǘ����");

			//==========�g��{��(����)==========
			ImGui_crtWgt_inputFloat(u8"�g��{��(����)", &pModelSetUp->partsSet.fScale);
			
			//==========�ҏW�����蔻��ԍ�==========
			ImGui_crtWgt_sliderInt(CreateText(u8"�ҏW�����蔻��ԍ�"), &nEditHitTest, 0, pModelSetUp->hitTestSet.nHitTestNum);
			
			//==========�����蔻��ҏW==========
			ImGui_crtWgt_separatorText(CreateText(u8"�����蔻��[%02d]�ҏW", nEditHitTest));

			if (nEditHitTest > pModelSetUp->hitTestSet.nHitTestNum - 1)
			{// �ҏW�����蔻��ԍ������݂̍ő哖���蔻��ԍ����z���Ă��鎞�A
				//==========�ǉ�+==========
				if (ImGui_crtWgt_button(u8"�ǉ�+")) { HitTestAddGameEditor(&pModelSetUp->hitTestSet.nHitTestNum, pModelSetUp); }
			}
			else
			{// �ҏW�����蔻��ԍ������݂̍ő哖���蔻��ԍ����z���Ă��Ȃ����A
				// (���p�ӂ��ꂽ�͈͂̓����蔻��I�����Ă��鎞)
				//==========�폜-==========
				if (ImGui_crtWgt_button(u8"�폜-")) {
					HitTestSubGameEditor(&pModelSetUp->hitTestSet.nHitTestNum, nEditHitTest, &pModelSetUp->hitTestSet.pHitTest[nEditHitTest], pModelSetUp);
					break;
				}
				else
				{// �폜��������Ȃ��������A
					// ���i���̃|�C���^
					int *pPartsNum = &pModelSetUp->hitTestSet.pHitTest[nEditHitTest].nHitTestPartsNum;

					//==========�ҏW���蕔�i�ԍ�==========
					ImGui_crtWgt_sliderInt(u8"�ҏW���蕔�i�ԍ�", &nEditParts, 0, *pPartsNum);

					//--------------------�����蔻��̕��i�ҏW--------------------
					ImGui_crtWgt_separatorText(CreateText(u8"�����蔻��̕��i[%02d]�ҏW", nEditParts));

					if (nEditParts > *pPartsNum - 1)
					{// �ҏW���i�ԍ������݂̍ő啔�i�ԍ����z���Ă��鎞�A
						//==========�ǉ�+==========
						if (ImGui_crtWgt_button(u8"�ǉ�+")) { HitTestPartsAddGameEditor(pPartsNum, nEditHitTest, pModelSetUp); }
					}
					else
					{// �ҏW���i�ԍ������݂̍ő啔�i�ԍ����z���Ă��Ȃ�(�p�ӂ��ꂽ�͈͂̕��i�I��)���A
						// �����蔻��̕��i�̃|�C���^
						HitTest3DParts *pParts = &pModelSetUp->hitTestSet.pHitTest[nEditHitTest].pHitTestParts[nEditParts];

						{//==========�`==========
							int nSelect = (int)pParts->hitTestForm;
							char *items[] = { u8"��", u8"�l�p", u8"�~��" };
							pParts->hitTestForm = (HIT_TEST_FORM)ImGui_crtWgt_combo(u8"�`", &nSelect, items, ARRAY_SIZE(items));
						}

						{//==========�e�ԍ�==========
							ImGui_crtWgt_inputInt(u8"�e�ԍ�", &pParts->nParent);
							IntControl(&pParts->nParent, INT_MAX, -1);
						}

						//==========�傫��==========
						ImGui_crtWgt_inputFloat3(u8"�傫��", &pParts->fWidth, &pParts->fHeight, &pParts->fDepth);

						//==========���Έʒu==========
						ImGui_crtWgt_inputFloat3(u8"���Έʒu", &pParts->fixedRelativePos.x, &pParts->fixedRelativePos.y, &pParts->fixedRelativePos.z);

						{//==========���Ό���==========
							D3DXVECTOR3 rot = pParts->fixedRelativeRot / D3DX_PI;	// �~�����P�ʂŐݒ肷��ׂɈꎞ�ۑ�
							ImGui_crtWgt_inputFloat3(u8"���Ό���", &rot.x, &rot.y, &rot.z);
							pParts->fixedRelativeRot = rot * D3DX_PI;	// �ϊ����đ��
						}

						//==========�폜==========
						if (ImGui_crtWgt_button(u8"�폜")) { HitTestPartsSubGameEditor(pPartsNum, nEditParts, pParts, pModelSetUp); }
					}
				}
			}
		}

		break;
	}
	case MODE00_MOTION: {
		if (ImGui_SetHeader(u8"���[�V�����쐬")) {// ��
			Motion3D *pMotion = GetMotion3D();	// ���[�V����(3D)�̏��̃|�C���^

			//==========���[�V�����ǂݍ���/�����o��==========
			if (ImGui_crtWgt_button(u8"���[�V�����ǂݍ���")) { LoadMotionDataGameEditor(); }
			if (ImGui_crtWgt_button(u8"���[�V���������o��")) { SaveMotionDataGameEditor(); }

			//--------------------���[�V�����Ǘ����--------------------
			ImGui_crtWgt_separatorText(u8"���[�V�����Ǘ����");

			//==========����==========
			ImGui_crtWgt_inputInt(u8"����", &pMotion->nLoopTime);
			
			//==========���[�v==========
			ImGui_crtWgt_checkBox(u8"���[�v", &pMotion->bLoop);

			{// ���[�V�������i���ɉ������������m�ۏ���
				int nPartsMotionNumTemp = pMotion->nPartsMotionNum;	// ���[�V�������i����ۑ�
				ImGui_crtWgt_holdToRepeat(CreateText(u8"���[�V�������i��:%d", pMotion->nPartsMotionNum), &pMotion->nPartsMotionNum);
				IntControl(&pMotion->nPartsMotionNum, INT_MAX, 0);

				if (pMotion->nPartsMotionNum > nPartsMotionNumTemp)
				{// ���[�V�������i�������Z���ꂽ���A
					// �������Ċm��
					ReGetMemory((void**)&pMotion->pPartsMotion, sizeof(PartsMotion3D), nPartsMotionNumTemp, pMotion->nPartsMotionNum);

					// �ǉ���̕��i��������������
					pMotion->pPartsMotion[pMotion->nPartsMotionNum - 1] = {};
				}
				else if (pMotion->nPartsMotionNum > nPartsMotionNumTemp)
				{// ���[�V�������i�������Z���ꂽ���A
					// �������Ċm��
					ReGetMemory((void**)&pMotion->pPartsMotion, sizeof(PartsMotion3D), pMotion->nPartsMotionNum, pMotion->nPartsMotionNum);
				}
			}

			{//==========�ҏW���i�ԍ�==========
				int nMax = pMotion->nPartsMotionNum - 1;
				IntControl(&nMax, INT_MAX, 0);
				ImGui_crtWgt_sliderInt(u8"�ҏW���i�ԍ�", &m_nSelectParts, 0, nMax);
			}

			if (pMotion->nPartsMotionNum > 0)
			{// ���i���̃��[�V������0�������Ă��鎞�A
				//--------------------���i�̃��[�V�����ҏW--------------------
				ImGui_crtWgt_separatorText(CreateText(u8"���i[%02d]�̃��[�V�����ҏW", m_nSelectParts));

				// �ҏW���镔�i�̃��[�V�������
				PartsMotion3D *pPartsMotion = &GetMotion3D()->pPartsMotion[m_nSelectParts];
				static int nEditCommand = 0;	// �ҏW�R�}���h�ԍ�

				//==========�R�}���h�ǉ�+==========
				if ((ImGui_crtWgt_button(u8"�R�}���h�ǉ�+"))) {
					pPartsMotion->nCommandNum++;	// �R�}���h�������Z

					// �R�}���h���̃������Ċm��
					ReGetMemory((void**)&pPartsMotion->pCommand, sizeof(Motion3DCommand), pPartsMotion->nCommandNum - 1, pPartsMotion->nCommandNum);

					// �R�}���h������
					pPartsMotion->pCommand[pPartsMotion->nCommandNum - 1] = {};

					// �R�}���h�̃|�C���^
					Motion3DCommand *pCmd = &pPartsMotion->pCommand[pPartsMotion->nCommandNum - 1];

					// �f�[�^�̃��������m�ۂ���
					pCmd->pData = NULL;
					GetMemory((void**)&pCmd->pData, sizeof(float), GetMotion3DCommandInfo(pCmd->command).nDataNum);

					// �S�Ẵf�[�^��������
					for (int nCntData = 0; nCntData < GetMotion3DCommandInfo(pCmd->command).nDataNum; pCmd->pData[nCntData] = 0.0f, nCntData++);
				}

				if(pPartsMotion->nCommandNum > 0)
				{//==========�R�}���h���X�g==========
					char **ppItems = NULL;	// ���X�g�̃A�C�e��
					GetMemory((void**)&ppItems, sizeof(char*), pPartsMotion->nCommandNum);	// ������_�u���|�C���^�̃������m��

					for (int nCntCmd = 0; nCntCmd < pPartsMotion->nCommandNum; nCntCmd++) {
						ppItems[nCntCmd] = NULL;
						GetMemory((void**)&ppItems[nCntCmd], sizeof(char)*TXT_MAX, 1);	// ������|�C���^�̃������m��

						// ���[�V�����R�}���h
						Motion3DCommand cmd = pPartsMotion->pCommand[nCntCmd];

						// ��ԕ\���e�L�X�g
						static const char aEaseText[EASE_MAX][TXT_MAX] = {
							u8"����",
							u8"���X�ɉ���",
							u8"���X�Ɍ���",
							u8"�������Č���",
						};

						switch (cmd.command) {
						case MOTION3D_COMMAND_LABEL_MOVE:
							sprintf(ppItems[nCntCmd], u8"%02d_TIME %d �ړ� (%.2f %.2f %.2f) %d %s",
									nCntCmd,
									pPartsMotion->pCommand[nCntCmd].nTime,
									cmd.pData[0],
									cmd.pData[1],
									cmd.pData[2],
									(int)cmd.pData[3],
									aEaseText[(int)cmd.pData[4]]);
							break;
						case MOTION3D_COMMAND_LABEL_SPIN:
							sprintf(ppItems[nCntCmd], u8"%02d_TIME %d ��] (%.2f %.2f %.2f) %d %s",
									nCntCmd,
									pPartsMotion->pCommand[nCntCmd].nTime,
									cmd.pData[0] / D3DX_PI,
									cmd.pData[1] / D3DX_PI,
									cmd.pData[2] / D3DX_PI,
									(int)cmd.pData[3],
									aEaseText[(int)cmd.pData[4]]);
							break;
						case MOTION3D_COMMAND_LABEL_STEP:
							sprintf(ppItems[nCntCmd], u8"%02d_TIME %d ���� >%d %d %d>",
									nCntCmd,
									pPartsMotion->pCommand[nCntCmd].nTime,
									(int)cmd.pData[0],
									(int)cmd.pData[1]);
							break;
						}
					}

					int nSelect = nEditCommand;
					nEditCommand = ImGui_crtWgt_listBox(u8"�R�}���h���X�g", &nSelect, ppItems, pPartsMotion->nCommandNum);
					IntControl(&nEditCommand, pPartsMotion->nCommandNum - 1, 0);

					// ���I�m�ۂ����������̉��
					for (int i = 0; i < pPartsMotion->nCommandNum; i++) {
						ReleaseMemory((void**)&ppItems[i]);
					}
					ReleaseMemory((void**)&ppItems);
				}

				//==========��ƕ��ёւ�==========
				if (ImGui_crtWgt_button(u8"��ƕ��ёւ�")) {
					if (nEditCommand > 0)
					{// �ҏW�R�}���h��0�������Ă��鎞�A
						Motion3DCommand cmdTemp = pPartsMotion->pCommand[nEditCommand];
						pPartsMotion->pCommand[nEditCommand] = pPartsMotion->pCommand[nEditCommand - 1];
						pPartsMotion->pCommand[nEditCommand - 1] = cmdTemp;

						// �ҏW�R�}���h�����Z
						nEditCommand--;
					}
				}

				//==========���ƕ��ёւ�==========
				if (ImGui_crtWgt_button(u8"���ƕ��ёւ�")) {
					if (nEditCommand < pPartsMotion->nCommandNum - 1)
					{// �ҏW�R�}���h��0�������Ă��鎞�A
						Motion3DCommand cmdTemp = pPartsMotion->pCommand[nEditCommand];
						pPartsMotion->pCommand[nEditCommand] = pPartsMotion->pCommand[nEditCommand + 1];
						pPartsMotion->pCommand[nEditCommand + 1] = cmdTemp;

						// �ҏW�R�}���h�����Z
						nEditCommand++;
					}
				}

				if (pPartsMotion->nCommandNum > 0)
				{// �R�}���h����0�������Ă��鎞�A
					//--------------------�R�}���h--------------------
					ImGui_crtWgt_separatorText(CreateText(u8"�R�}���h[%02d]", nEditCommand));

					// �R�}���h�̃|�C���^
					Motion3DCommand *pCmd = &pPartsMotion->pCommand[nEditCommand];

					{// �R�}���h���ҏW
						{//==========�R�}���h���x��==========
							int nSelectCmd  = (int)pCmd->command;
							int nSelectTemp = nSelectCmd;
							char *items[] = { u8"�ړ�", u8"��]", u8"����" };
							ImGui_crtWgt_combo(u8"�R�}���h���x��", &nSelectCmd, items, MOTION3D_COMMAND_LABEL_MAX);
							pCmd->command = (MOTION3D_COMMAND_LABEL)nSelectCmd;

							if (nSelectCmd != nSelectTemp)
							{// ���̃R�}���h���x����I���������A
								// �f�[�^�̃��������m�ۂ���
								GetMemory((void**)&pCmd->pData, sizeof(float), GetMotion3DCommandInfo(pCmd->command).nDataNum);

								// �S�Ẵf�[�^��������
								for (int nCntData = 0; nCntData < GetMotion3DCommandInfo(pCmd->command).nDataNum; pCmd->pData[nCntData] = 0.0f, nCntData++);
							}
						}

						//==========�ʒu==========
						ImGui_crtWgt_inputInt(u8"���s�^�C�~���O", &pCmd->nTime);

						// �R�}���h���x���ɉ��������͏���
						switch (pCmd->command) {
						case MOTION3D_COMMAND_LABEL_MOVE: {
							//==========�ʒu==========
							ImGui_crtWgt_inputFloat3(u8"�ʒu", &pCmd->pData[0], &pCmd->pData[1], &pCmd->pData[2]);

							{//==========�ړ��ɂ����鎞��==========
								int nData = (int)pCmd->pData[3];
								ImGui_crtWgt_inputInt(u8"�ړ��ɂ����鎞��", &nData);
								pCmd->pData[3] = nData;
							}

							{//==========���==========
								int nSelect = (int)pCmd->pData[4];
								char *items[] = { u8"����",u8"���X�ɉ���",u8"���X�Ɍ���",u8"�������Č���" };
								pCmd->pData[4] = (EASE)ImGui_crtWgt_combo(u8"���", &nSelect, items, ARRAY_SIZE(items));
							}

							break;
						}// MOTION3D_COMMAND_LABEL_MOVE
						case MOTION3D_COMMAND_LABEL_SPIN: {
							{//==========����==========
								D3DXVECTOR3 rot = D3DXVECTOR3(pCmd->pData[0], pCmd->pData[1], pCmd->pData[2]) / D3DX_PI;
								ImGui_crtWgt_inputFloat3(u8"����", &rot.x, &rot.y, &rot.z);
								pCmd->pData[0] = rot.x * D3DX_PI;
								pCmd->pData[1] = rot.y * D3DX_PI;
								pCmd->pData[2] = rot.z * D3DX_PI;
							}

							{//==========��]�ɂ����鎞��==========
								int nData = (int)pCmd->pData[3];
								ImGui_crtWgt_inputInt(u8"��]�ɂ����鎞��", &nData);
								pCmd->pData[3] = nData;
							}

							{//==========���==========
								int nSelect = (int)pCmd->pData[4];
								char *items[] = { u8"����",u8"���X�ɉ���",u8"���X�Ɍ���",u8"�������Č���" };
								pCmd->pData[4] = (EASE)ImGui_crtWgt_combo(u8"���", &nSelect, items, ARRAY_SIZE(items));
							}

							break;
						}// MOTION3D_COMMAND_LABEL_SPIN
						case MOTION3D_COMMAND_LABEL_STEP: {
							{//==========���ڎ���==========
								int nData = (int)pCmd->pData[0];
								ImGui_crtWgt_inputInt(u8"���ڎ���", &nData);
								pCmd->pData[0] = nData;
							}

							{//==========���v����==========
								int nData = (int)pCmd->pData[1];
								ImGui_crtWgt_inputInt(u8"���v����", &nData);
								pCmd->pData[1] = nData;
							}

							{//==========�߂鎞��==========
								int nData = (int)pCmd->pData[2];
								ImGui_crtWgt_inputInt(u8"�߂鎞��", &nData);
								pCmd->pData[2] = nData;
							}

							break;
						}// MOTION3D_COMMAND_LABEL_STEP
						}
					}

					//==========�폜==========
					if (ImGui_crtWgt_button(u8"�폜")) {
						// �R�}���h�������炷
						pPartsMotion->nCommandNum--;

						// ��̕��i�S�Ă̏������̕��i�̏��ŏ㏑�����Ă���
						for (int nCntCmd = 0; nCntCmd < pPartsMotion->nCommandNum - nEditCommand; nCntCmd++) {
							pCmd[nCntCmd] = pCmd[nCntCmd + 1];
						}

						// �������Ċm��
						ReGetMemory((void**)&pPartsMotion->pCommand, sizeof(Motion3DCommand), pPartsMotion->nCommandNum, pPartsMotion->nCommandNum);

						// �ҏW�R�}���h�����Z
						nEditCommand--;

						if (nEditCommand < 0) {// 0��������
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
// PartsAddGameEditor�֐� - �Q�[���G�f�B�^�[�̕��i�ǉ����� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::PartsAddGameEditor(int *pPartsNum, ModelSetUp3D *pModelSetUp) {
	(*pPartsNum)++;	// ���i�������Z

	// ���i�̃_�u���|�C���^���擾
	CParts3D::Parts3D **ppParts = m_parts3D.GetParts3Dpp();

	// �������Ċm��
	ReGetMemory((void**)&pModelSetUp->partsSet.pPartsType,
				sizeof(Parts3DType), 
				*pPartsNum - 1, 
				*pPartsNum);
	ReGetMemory((void**)ppParts,
				sizeof(CParts3D::Parts3D),
				*pPartsNum - 1,
				*pPartsNum);

	// �ǉ���̕��i��������������
	pModelSetUp->partsSet.pPartsType[*pPartsNum - 1] = {};
	pModelSetUp->partsSet.pPartsType[*pPartsNum - 1].nIdx = *pPartsNum - 1;
	m_parts3D.InitParts3D(*pPartsNum - 1);

	if (!LoadPartsDataGameEditor(pModelSetUp, *pPartsNum - 1))
	{// ���i��ǂݍ��߂Ȃ��������A
		(*pPartsNum)--;	// ���i�������Z

		// �������Ċm��
		ReGetMemory((void**)&pModelSetUp->partsSet.pPartsType, sizeof(Parts3DType), *pPartsNum, *pPartsNum);
		ReGetMemory((void**)ppParts, sizeof(CParts3D::Parts3D), *pPartsNum, *pPartsNum);
	}
}

//========================================
// PartsSubGameEditor�֐� - �Q�[���G�f�B�^�[�̕��i�폜���� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::PartsSubGameEditor(int *pPartsNum, int nEditParts, Parts3DType *pParts, ModelSetUp3D *pModelSetUp) {
	(*pPartsNum)--;	// ���i�������炷

	// ��̕��i�S�Ă̏������̕��i�̏��ŏ㏑�����Ă���
	for (int nCntTip = 0; nCntTip < *pPartsNum - nEditParts; nCntTip++) {
		pParts[nCntTip] = pParts[nCntTip + 1];
	}

	// �������Ċm��
	ReGetMemory((void**)&pModelSetUp->partsSet.pPartsType, sizeof(Parts3DType), *pPartsNum + 1, *pPartsNum);
}

//========================================
// HitTestAddGameEditor�֐� - �Q�[���G�f�B�^�[�̓����蔻��ǉ����� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestAddGameEditor(int *pHitTestNum, ModelSetUp3D *pModelSetUp) {
	(*pHitTestNum)++;	// �����蔻�萔���Z

	// �������Ċm��
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest, sizeof(HitTest3D), *pHitTestNum - 1, *pHitTestNum);

	// �ǉ���̕��i��������������
	pModelSetUp->hitTestSet.pHitTest[*pHitTestNum - 1] = {};
}

//========================================
// HitTestSubGameEditor�֐� - �Q�[���G�f�B�^�[�̓����蔻��폜���� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestSubGameEditor(int *pHitTestNum, int nEditHitTest, HitTest3D *pHitTest, ModelSetUp3D *pModelSetUp) {
	(*pHitTestNum)--;	// �����蔻�萔���Z

	// ��̕��i�S�Ă̏������̕��i�̏��ŏ㏑�����Ă���
	for (int nCntTip = 0; nCntTip < *pHitTestNum - nEditHitTest; nCntTip++) {
		pHitTest[nCntTip] = pHitTest[nCntTip + 1];
	}

	// �������Ċm��
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest, sizeof(HitTest3D), *pHitTestNum, *pHitTestNum);
}

//========================================
// HitTestPartsAddGameEditor�֐� - �Q�[���G�f�B�^�[�̓����蔻��̕��i�ǉ����� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestPartsAddGameEditor(int *pPartsNum, int nEditHitTest, ModelSetUp3D *pModelSetUp) {
	(*pPartsNum)++;	// ���i�������Z

	// �������Ċm��
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest[nEditHitTest].pHitTestParts, sizeof(HitTest3DParts), *pPartsNum - 1, *pPartsNum);

	// �ǉ���̕��i��������������
	pModelSetUp->hitTestSet.pHitTest[nEditHitTest].pHitTestParts[*pPartsNum - 1] = {};
}

//========================================
// HitTestPartsSubGameEditor�֐� - �Q�[���G�f�B�^�[�̓����蔻��̕��i�폜���� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::HitTestPartsSubGameEditor(int *pPartsNum, int nEditParts, HitTest3DParts *pParts, ModelSetUp3D *pModelSetUp) {
	// ���i�������炷
	(*pPartsNum)--;

	// ��̕��i�S�Ă̏������̕��i�̏��ŏ㏑�����Ă���
	for (int nCntTip = 0; nCntTip < *pPartsNum - nEditParts; nCntTip++) {
		pParts[nCntTip] = pParts[nCntTip + 1];
	}

	// �������Ċm��
	ReGetMemory((void**)&pModelSetUp->hitTestSet.pHitTest->pHitTestParts, sizeof(HitTest3DParts), *pPartsNum, *pPartsNum);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** ��{�ڑ� *** |--------------------
//============================================================

//========================================
// InitGameEditor�֐� - �Q�[���G�f�B�^�[�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::Init(void) {
	// �J����(3D)�̏�Ԃ�ݒ�(�}�E�X�Œ���͂�)
	SetCamera3DState(CAMERA3D_STATE_GRAB_AIR_MOUSE);

	InitModelSetUpMemory(1);	// �ҏW���̃��f���̃Z�b�g�A�b�v(3D)�̃������m��
	InitMotion3DMemory(1);		// �ҏW���̃��[�V�����̃������m��
	InitHuman3DBaseMemory(1);	// �ҏW���̃q���[�}��(3D)�x�[�X�̃������m��

	m_parts3D.Init(0);		// ���i(3D)�̏���������
	m_parts3D.SetMotion(0);	// ���i(3D)�̃��[�V������ݒ�

	{// �ϐ��̏�����
		// ����
		m_bShowPartsIdx = true;					// ���i�̔ԍ��\��
		m_bShowHitTest  = true;					// �����蔻��\���t���O
		m_bFrameByFrame = false;				// �R�}����t���O
		m_mode          = MODE_00_MODELEDIT;	// ���[�h

		// ���f���G�f�B�b�g
		m_mode00         = MODE00_MODELSETUP;	// ���[�h
		sprintf(m_aNowSetUpPath, "");			// ���݂̃Z�b�g�A�b�v�t�@�C���p�X
		m_nModelSetUpIdx = 0;					// ���f���̃Z�b�g�A�b�v(3D)�ԍ�
		m_nSelectParts   = 0;					// �I�����Ă��镔�i
		m_bLoadModel     = false;				// ���f����ǂݍ��߂����t���O
	}

	// �e�X�g���̃e�N�X�`���ǂݍ���
	m_nTestEyeTex = LoadTexture(TESTEYE_TEXPATH);

	// ���f���\���e�N�X�`���̓ǂݍ���
	for (int nCntPlatform = 1; nCntPlatform < MODELPLATFORM_MAX; nCntPlatform++) {
		char aLoadName[TXT_MAX] = "";	// �ǂݍ��ݖ�

		// �J�E���g�ɉ��������f���\���e�N�X�`����ǂݍ���
		switch (nCntPlatform) {
		case MODELPLATFORM_BIPEDALISM_00:sprintf(aLoadName, "data_R.N.App\\TEXTURE\\MODELPLATFORM\\BIPEDALISM_00.png"); break;
		case MODELPLATFORM_BIPEDALISM_01:sprintf(aLoadName, "data_R.N.App\\TEXTURE\\MODELPLATFORM\\BIPEDALISM_01.png"); break;
		default:
			assert(false);
			break;
		}

		// �e�N�X�`���ǂݍ���
		m_aModelPlatformTex[nCntPlatform - 1] = LoadTexture(aLoadName);
	}
}

//========================================
// UninitGameEditor�֐� - �Q�[���G�f�B�^�[�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::Uninit(void) {
	// ���i(3D)�̏I������
	m_parts3D.Uninit();
}

//========================================
// UodateGameEditor�֐� - �Q�[���G�f�B�^�[�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void Mode_GameEditor::Update(void) {
	// ImGui�̐ݒ菈��
	ImGuiSetGameEditor();

	// �����蔻��`��
	m_parts3D.DrawCollision(m_bShowHitTest);
	// �����蔻��̕\����Ԑ؂�ւ�
	ShowHitTest(m_bShowHitTest);

	// �R�}����t���O���^�̎��A���[�V������~����
	if (m_bFrameByFrame) {
		m_parts3D.SetMotionStop(false);
	}

	// ���[�h�����f���G�f�B�b�g�̎��A���i(3D)�̍X�V����
	if (m_mode == MODE_00_MODELEDIT) { m_parts3D.Update(); }

	// �R�}����t���O���^�̎��A���[�V������~
	if (m_bFrameByFrame) { 
		m_parts3D.SetMotionStop(true); 
		m_bFrameByFrame = false;
	}

	// �E�B���h�E�̃t�H�[�J�X��Ԃɉ����Ĕw�i�F�ݒ�
	SetBGColor(ImGui_GetWindowFocused() ? Color{ 0, 0, 0, 255 } : DEFAULTBGCOLOR);

	if (m_bShowPartsIdx) 
	{// ���i�̔ԍ��\�����^�̎��A
		// ���f���̃Z�b�g�A�b�v(3D)���
		ModelSetUp3D *pModelSetUp = GetModelSetUp()[0];

		for (int nCntParts = 0; nCntParts < pModelSetUp->partsSet.nPartsNum; nCntParts++) {
			Parts3DType *pPartsType = &pModelSetUp->partsSet.pPartsType[nCntParts];

			// �e�L�X�g(3D)�̐ݒ菈��
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

	{// �O���b�h�̐ݒ�
		// ��(3D)�̐ݒ���
		Line3DSet lineSet;

		// �ݒ�F�𔒂ɂ���
		lineSet.col1 = INITCOLOR;
		lineSet.col2 = INITCOLOR;
		
		// Z�e�X�g���I���ɂ���
		lineSet.bZTest = true;

		// �[�܂ł̔�����
		float fEdge = GRID_SCALE * 0.5f;

		for (int nCntGrid = 0; nCntGrid < GRID_DIV + 1; nCntGrid++) {
			float fPosValue = -fEdge + (nCntGrid * (GRID_SCALE / GRID_DIV));

			if (nCntGrid == GRID_DIV / 2) {
				// ��������
				/* �_1 */lineSet.pos1 = D3DXVECTOR3(fPosValue, 0.0f, 0.0f);
				/* �_2 */lineSet.pos2 = D3DXVECTOR3(fPosValue, 0.0f, -fEdge);
				// ��(3D)�̐ݒ菈��
				SetLine3D(lineSet);

				// �ݒ�F��Ԃɂ���
				lineSet.col1 = Color{ 255,0,0,255 };
				lineSet.col2 = Color{ 255,0,0,255 };

				// �����c���p��������
				/* �_1 */lineSet.pos1 = D3DXVECTOR3(fPosValue, 0.0f, 0.0f);
				/* �_2 */lineSet.pos2 = D3DXVECTOR3(fPosValue, 0.0f, fEdge);
				// ��(3D)�̐ݒ菈��
				SetLine3D(lineSet);

				// �ݒ�F�𔒂ɂ���
				lineSet.col1 = INITCOLOR;
				lineSet.col2 = INITCOLOR;
			}
			else {
				// ��������
				/* �_1 */lineSet.pos1 = D3DXVECTOR3(fPosValue, 0.0f, fEdge);
				/* �_2 */lineSet.pos2 = D3DXVECTOR3(fPosValue, 0.0f, -fEdge);
				// ��(3D)�̐ݒ菈��
				SetLine3D(lineSet);
			}

			// ��������
			/* �_1 */lineSet.pos1 = D3DXVECTOR3(fEdge, 0.0f, fPosValue);
			/* �_2 */lineSet.pos2 = D3DXVECTOR3(-fEdge, 0.0f, fPosValue);
			// ��(3D)�̐ݒ菈��
			SetLine3D(lineSet);
		}
	}
}