/**********************************************************************
[[Phase_GameTackle3�w�b�_�[(Phase_GameTackle3.h)]]
	��ҁFGameTackle3GameTackle3�H�H

	Phase_GameTackle3.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
GameTackle3��{�֐��Q�擾�֐�
=====================================================================*/
PHASE_FUNC* GetPhaseGameTackle3Func();

// ��{�֐��Q
void InitGameTackle3(bool isFirst);
void UninitGameTackle3(bool isEnd);
void UpdateGameTackle3();
void DrawGameTackle3();