/**********************************************************************
[[�X���C�hGIMMICK�w�b�_�[(Gimmick/Slide.h)]]
	��ҁF���c�@�^�K

	Gimmick/Slide.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once


//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

/*=====================================================================
�X���C�h�������֐�
=====================================================================*/
GIMMICK_HEADER *GetSlideMemory(FIELD_CHIP* pChip, Vec3* pos);

// ��{�֐��Q
void InitFieldGimmickSlide();
void UninitFieldGimmickSlide();
void ResetFieldGimmickSlide();
void AllEndFieldGimmickSlide();