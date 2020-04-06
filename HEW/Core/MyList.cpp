/**********************************************************************
[[���X�g�v���O����(MyList.cpp)]]
	��ҁF���c�@�^�K

	���X�g�쐬�Ɋւ���v���O����
***********************************************************************/
#include "MyList.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------


// a��16�̔{���Ɏ��܂�悤�ɂ���
#define NUM_STR(a)		((a) + (16 - ((a) % 16)))			

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------

// ���X�gObjheader
typedef struct MyListHead {
	MyListHead *next_pt;		// ���̃��X�g�ւ̃A�h���X
	MyListHead *back_pt;		// �O�́@�@����
	size_t		DynamicSize;	// �I�u�W�F�N�g�ɂ���ĈقȂ�ӏ��̃T�C�Y(Object_Size������)
}MyListHead;

// ���X�g�Ǘ��\����
typedef struct ListManager {
	LIST		ListData;			// ���J���郊�X�g�f�[�^
	size_t		Obj_Size;			// ���X�g�������̃T�C�Y(�w�b�_������)

	MyListHead	*Top_pt;			// �擪���X�g�I�u�W�F�N�g
	MyListHead	*Bot_pt;			// �I�[���X�g�I�u�W�F�N�g

	MyListHead	*work_pt;			// ���X�g�C�e���[�^�i���݉񂵂Ă���A�h���X�j
	bool		isLoopFromTop;		// �C�e���[�^�̕���(true:�擪����)

	MyListHead	*PoolTop_pt;		// �ꎞ�ۊǏ�(�擪)�imalloc�񐔌y���j
	MyListHead	*PoolBot_pt;		// �ꎞ�ۊǏ��i�I�[�j�imalloc�񐔌y���j
	DWORD		numPoolObj;			// �ꎞ�ۊǂ���I�u�W�F�N�g��
	DWORD		maxPoolObj;			// ����ő吔
}ListManager;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
MyListHead *GetObjectFromPool(ListManager* manager, size_t DynamicSize);
void DelObjAndPool(ListManager *manager, MyListHead *del_pt);

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------

//----------------------------------------------------------------
// ���X�g�@�\�쐬�A�폜�֐�
//----------------------------------------------------------------

/*=====================================================================
���X�g�@�\�쐬�֐�
	���X�g���g�p���邽�߂ɏ�������֐��B���X�g���Ƃɗp�ӂ���
	�߂�l:MyList 
	�����G
	size_t ObjectSize,	: ���X�g1�v�f������̑傫��
						�i�I�u�W�F�N�g���Ƃɕψق���T�C�Y�������j
	DWORD numPool		: �I�u�W�F�N�g�v�[����(�g��Ȃ��Ȃ����I�u�W�F�N�g���ꎞ�ۑ�����)
							(�K�{�łȂ���32�ɂȂ�)
	size_t PublicSize	: ���L�f�[�^�̑傫���i�K�{�łȂ��j
=====================================================================*/
MyList MyListCreate(size_t ObjectSize, DWORD numPool, size_t PublicSize)
{
	ListManager *list = NULL;

	// �������m�ہi�[���N���A�j
	list = (ListManager*)calloc(1, sizeof(ListManager) + PublicSize);

	if (PublicSize <= 0)
		list->ListData.Public_pt = &list[1];		// ���L�f�[�^�Q�̓������̍Ō�Ɏ����Ă���

	list->Obj_Size = ObjectSize;					// 1�I�u�W�F�N�g�̃T�C�Y
	list->maxPoolObj = numPool;						// �v�[���ő吔

	return &list->ListData;
}

/*=====================================================================
���X�g�@�\�폜�֐�
	�S�Ẵ��X�g�I�u�W�F�N�g���J�����ă��X�g�@�\���폜����֐�
	�߂�l:void
	�����G
	MyList* DelList : �폜���郊�X�g�i�֐����s��NULL������j
=====================================================================*/
void MyListDelete(MyList *DelList)
{
	ListManager *manager = (ListManager*)*DelList;
	MyListHead *work_pt, *del_pt;

	// �{���X�g�̊J��
	work_pt = manager->Top_pt;
	while (work_pt != NULL)
	{
		del_pt = work_pt;
		work_pt = work_pt->next_pt;
		free(del_pt);
	}

	// �v�[�����X�g�̊J��
	work_pt = manager->PoolTop_pt;
	while (work_pt != NULL)
	{
		del_pt = work_pt;
		work_pt = work_pt->next_pt;
		free(del_pt);
	}

	// ���X�g�Ǘ��\���̂̊J��
	free(*DelList);
	*DelList = NULL;
}


//----------------------------------------------------------------
// ���X�g�I�u�W�F�N�g�ǉ��A�r���֐�
//----------------------------------------------------------------

/*=====================================================================
���X�g�I�u�W�F�N�g�ψكT�C�Y�ύX�֐�
	�I�u�W�F�N�g���Ƃɂ���ψك������T�C�Y��ύX����֐�

	�߂�l:void*  : �V���ɕς�����\���̂���I�u�W�F�N�g�A�h���X
	�����G
	MyList list,			: ���X�g�@�\
	void *work_pt,			�F�I�u�W�F�N�g�A�h���X
	size_t DynamicSize		: �V���ȕψكT�C�Y
=====================================================================*/
void *MyListChangeDynamicSize(MyList list, void *work_pt, size_t nSize)
{
	if (!work_pt)return work_pt;

	ListManager *manager = (ListManager*)list;

	// �Ă΂ꂽ�I�u�W�F�N�g����header�A�h���X�����߂�
	MyListHead *Idx_Head = &((MyListHead*)work_pt)[-1];
	MyListHead *keep_pt;

	// �傫���̋��e���ł����ꍇ�͍�蒼���Ȃ�
	if (Idx_Head->DynamicSize >= nSize)return work_pt;
	
	// �V���ɃI�u�W�F�N�g��p�ӂ��ăR�s�[
	keep_pt = GetObjectFromPool(manager, nSize);
	memcpy(&keep_pt[1], &Idx_Head[1], sizeof(MyListHead) + manager->Obj_Size + Idx_Head->DynamicSize);
	keep_pt->back_pt = Idx_Head->back_pt;
	keep_pt->next_pt = Idx_Head->next_pt;


	// ���̃I�u�W�F�N�g���폜
	DelObjAndPool(manager, Idx_Head);

	// ��������
	if (keep_pt->back_pt)	keep_pt->back_pt->next_pt = keep_pt;
	else					manager->Top_pt = keep_pt;

	if (keep_pt->next_pt)	keep_pt->next_pt->back_pt = keep_pt;
	else					manager->Bot_pt = keep_pt;

	return &keep_pt[1];
}

/*=====================================================================
���X�g�I�u�W�F�N�g�쐬�֐�
	�w��I�u�W�F�N�g�̑O��Ƀ��X�g�I�u�W�F�N�g���쐬����֐�
	�߂�l:void*  : �쐬�������X�g�̐擪�A�h���X
	�����G
	MyList list,			: ���X�g�@�\
	void *work_pt,			�F�O��Ƀ��X�g����邽�߂̌��ƂȂ�I�u�W�F�N�g�A�h���X
	bool isNext,			: (true)������̎��I�u�W�F�N�g�Ƃ��č쐬����
							(false) ����O�I�u�W�F�N�g�Ƃ��č쐬����
	size_t DynamicSize		: �I�u�W�F�N�g���ƂɃT�C�Y���قȂ�m�ۃT�C�Y(�K�{�łȂ�)
=====================================================================*/
void *MyListCreateObject(MyList list, void *work_pt, bool isNext, size_t DynamicSize)
{
	if (!work_pt)
		return MyListCreateObjectTop(list, DynamicSize);

	ListManager *manager = (ListManager*)list;
	MyListHead	*keep_pt = NULL;					// ��ԃ|�C���^
	// �Ă΂ꂽ�I�u�W�F�N�g����header�A�h���X�����߂�
	MyListHead *Idx_Head = &((MyListHead*)work_pt)[-1];

	// �V�I�u�W�F�N�g
	MyListHead	*new_pt = GetObjectFromPool(manager, DynamicSize);

	if (isNext)
	{	// �I�u�W�F�N�g�̎��ɐݒu
		keep_pt = Idx_Head->next_pt;

		// �l�N�X�g���̐���
		Idx_Head->next_pt = new_pt;		
		new_pt->next_pt = keep_pt;	

		// �o�b�N���̐���
		if(keep_pt)	keep_pt->back_pt = new_pt;
		else		manager->Bot_pt = new_pt;

		new_pt->back_pt = Idx_Head;
	}
	else
	{	// �I�u�W�F�N�g�̑O�ɐݒu
		keep_pt = Idx_Head->back_pt;

		// �o�b�N���̐���
		Idx_Head->back_pt = new_pt;
		new_pt->back_pt = keep_pt;

		// �l�N�X�g���̐���
		if (keep_pt)keep_pt->next_pt = new_pt;
		else		manager->Top_pt	= new_pt;

		new_pt->next_pt = Idx_Head;
	}
	return &new_pt[1];
}

/*=====================================================================
���X�g�I�u�W�F�N�g�擪�ӏ��쐬�֐�
	���X�g�̐擪�ɃI�u�W�F�N�g���쐬����֐�
	�߂�l:void*  : �쐬�������X�g�̐擪�A�h���X
	�����G
	MyList list,			: ���X�g�@�\
	size_t DynamicSize		: �I�u�W�F�N�g���ƂɃT�C�Y���قȂ�m�ۃT�C�Y(�K�{�łȂ�)
=====================================================================*/
void *MyListCreateObjectTop(MyList list, size_t DynamicSize)
{
	ListManager *manager = (ListManager*)list;
	MyListHead	*keep_pt = manager->Top_pt;					// ��ԃ|�C���^

	// �V�I�u�W�F�N�g
	MyListHead	*new_pt = GetObjectFromPool(manager, DynamicSize);

	// �l�N�X�g���̐���
	manager->Top_pt = new_pt;
	if (!keep_pt)manager->Bot_pt = new_pt;
	else			keep_pt->back_pt = new_pt;

	new_pt->next_pt = keep_pt;

	// �o�b�N���̐���
	new_pt->back_pt = NULL;

	return &new_pt[1];
}

/*=====================================================================
���X�g�I�u�W�F�N�g�I�[�ӏ��쐬�֐�
	���X�g�̏I�[�ɃI�u�W�F�N�g���쐬����֐�
	�߂�l:void*  : �쐬�������X�g�̐擪�A�h���X
	�����G
	MyList list,			: ���X�g�@�\
	size_t DynamicSize		: �I�u�W�F�N�g���ƂɃT�C�Y���قȂ�m�ۃT�C�Y(�K�{�łȂ�)
=====================================================================*/
void *MyListCreateObjectBottom(MyList list, size_t DynamicSize)
{
	ListManager *manager = (ListManager*)list;
	MyListHead	*keep_pt = manager->Bot_pt;		// ��ԃ|�C���^

	// �V�I�u�W�F�N�g
	MyListHead	*new_pt = GetObjectFromPool(manager, DynamicSize);

	// �o�b�N���̐���
	manager->Bot_pt = new_pt;
	if (!keep_pt)manager->Top_pt = new_pt;
	else			keep_pt->next_pt = new_pt;

	new_pt->back_pt = keep_pt;

	// �l�N�X�g���̐���
	new_pt->next_pt = NULL;

	return &new_pt[1];
}

/*=====================================================================
���X�g�I�u�W�F�N�g�폜�֐�
	�w��I�u�W�F�N�g���폜����֐�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
	void **del_pt			: �폜����Ώۂ̃I�u�W�F���Ɓi���s��NULL������j
=====================================================================*/
void MyListDeleteObject(MyList list, void **del_pt)
{
	ListManager *manager = (ListManager*)list;

	MyListHead *DelHead_pt = &((MyListHead*)*del_pt)[-1];

	DelObjAndPool(manager, DelHead_pt);
	*del_pt = NULL;
}

/*=====================================================================
���X�g�I�u�W�F�N�g�擪�ӏ��폜�֐�
	���X�g�̐擪�I�u�W�F�N�g���폜����֐�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
=====================================================================*/
void MyListDeleteObjectTop(MyList list)
{
	ListManager *manager = (ListManager*)list;

	DelObjAndPool(manager, manager->Top_pt);
}

/*=====================================================================
���X�g�I�u�W�F�N�g�I�[�ӏ��폜�֐�
	���X�g�̏I�[�I�u�W�F�N�g���폜����֐�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
=====================================================================*/
void MyListDeleteObjectBottom(MyList list)
{
	ListManager *manager = (ListManager*)list;

	DelObjAndPool(manager, manager->Bot_pt);
}

/*=====================================================================
���X�g�I�u�W�F�N�g�S���폜�֐�
	���X�g�̃I�u�W�F�N�g��S�č폜����֐�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
=====================================================================*/
void MyListDeleteObjectAll(MyList list)
{
	ListManager *manager = (ListManager*)list;

	while(manager->ListData.numObj != 0)
	{
		DelObjAndPool(manager, manager->Top_pt);
	}
}

//----------------------------------------------------------------
// ���X�g�I�u�W�F�N�g����@�\�֐�
//----------------------------------------------------------------

/*=====================================================================
���X�gIterator�ݒu�w��֐�
	���X�g�����񂷂�ׂ̃C�e���[�^��ݒu�w�肵����������w�肷��֐�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
	void *pStart,			: �擪�̃A�h���X
	bool isFromTop			: (true)�擪����I�[�̏��ŏ��񂷂�t���O�𗧂Ă�
							(false) �I�[����擪�̏��ŏ��񂷂�t���O�𗧂Ă�
=====================================================================*/
void MyListSetIterator(MyList list, void *pStart, bool isFromTop)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *Head_pt = NULL;

	manager->isLoopFromTop = isFromTop;

	// �|���̂Ńk���`�F�b�N
	if (pStart)Head_pt = &((MyListHead*)pStart)[-1];

	manager->work_pt = Head_pt;
}

/*=====================================================================
���X�gIterator�ݒu�֐�
	���X�g�����񂷂�ׂ̃C�e���[�^��ݒu������������w�肷��֐�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
	bool isSetTop			: (true)���X�g�̐擪�ɐݒu����
									�擪����I�[�̏��ŏ��񂷂�t���O�𗧂Ă�
							(false) �I�[�ɐݒu����
									�I�[����擪�̏��ŏ��񂷂�t���O�𗧂Ă�
=====================================================================*/
void MyListResetIterator(MyList list, bool isSetTop)
{
	ListManager *manager = (ListManager*)list;

	manager->isLoopFromTop = isSetTop;

	if (isSetTop)
		manager->work_pt = manager->Top_pt;
	else
		manager->work_pt = manager->Bot_pt;
}

/*=====================================================================
���X�g����֐�
	���X�g������d�؂�܂�true��Ԃ��A�C�e���[�^��i�߂�֐�
	while���̏������ɓ���邱�ƂŃ��X�g�����ׂď��񂷂邱�Ƃ��\
	�߂�l:bool
					true:���X�g�̏���͂܂��I����Ă��Ȃ�
					false;���X�g�̏���͏I��
	�����G
	MyList list,			: ���X�g�@�\
	void** work_pt			: ���݉񂵂Ă���Iterator�̃I�u�W�F�N�g���Ԃ�
=====================================================================*/
bool MyListLoop(MyList list, void** work_pt)
{
	ListManager *manager = (ListManager*)list;

	// �����Ȃ��ꍇ��false��Ԃ�
	if (manager->work_pt == NULL)
	{
		if(work_pt)*work_pt = NULL;
		return false;
	}

	if (work_pt)*work_pt = (void*)&manager->work_pt[1];

	// �o�������X�g�̐i�ޏ��Ԃ�����
	if (manager->isLoopFromTop)
		manager->work_pt = manager->work_pt->next_pt;
	else
		manager->work_pt = manager->work_pt->back_pt;

	return true;
}

/*=====================================================================
���X�g�擪�I�u�W�F�N�g�擾�֐�
	�߂�l: void * �擪�I�u�W�F�N�g�A�h���X
	�����G
	MyList list,			: ���X�g�@�\
=====================================================================*/
void *MyListGetTopObject(MyList list)
{
	ListManager *manager = (ListManager*)list;
	if (!manager->Top_pt)return NULL;

	return &manager->Top_pt[1];
}

/*=====================================================================
���X�g�I�[�I�u�W�F�N�g�擾�֐�
	�߂�l: void * �I�[�I�u�W�F�N�g�A�h���X
	�����G
	MyList list,			: ���X�g�@�\
=====================================================================*/
void *MyListGetBottomObject(MyList list)
{
	ListManager *manager = (ListManager*)list;
	if (!manager->Bot_pt)return NULL;

	return &manager->Bot_pt[1];
}

/*=====================================================================
���X�g�I�u�W�F�N�g�X���b�v�֐�
	�Q�̃I�u�W�F�N�g�����ԏ����ւ���֐�
	��������ł͓���ւ��Ȃ�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
	void* pObj1,			: 1�ԃI�u�W�F�N�g
	void* pObj2				: 2�ԃI�u�W�F�N�g
=====================================================================*/
void MyListSwap(MyList list, void* pObj1, void* pObj2)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *work1_pt = &((MyListHead*)pObj1)[-1];
	MyListHead *work2_pt = &((MyListHead*)pObj2)[-1];
	MyListHead keep;

	// �C�e���[�^�̐�������
	if (manager->work_pt == work1_pt)
	{
		manager->work_pt = work2_pt;
	}
	else if (manager->work_pt == work2_pt)
	{
		manager->work_pt = work1_pt;
	}

	// adr�X���b�v
	keep.back_pt = work1_pt->back_pt;
	keep.next_pt = work1_pt->next_pt;
	work1_pt->back_pt = work2_pt->back_pt;
	work1_pt->next_pt = work2_pt->next_pt;
	work2_pt->back_pt = keep.back_pt;
	work2_pt->next_pt = keep.next_pt;

	// 1�Ԃ�swap���
	if (manager->Top_pt == work1_pt)
	{
		// 2�Ԃ̑O��null
		manager->Top_pt = work2_pt;	
	}
	else
	{	
		work2_pt->back_pt->next_pt = work2_pt;
	}

	if (manager->Bot_pt == work1_pt)
	{
		manager->Bot_pt = work2_pt;
	}
	else
	{
		work2_pt->next_pt->back_pt = work2_pt;
	}

	// 2�Ԃ�swap���
	if (manager->Top_pt == work2_pt)
	{
		manager->Top_pt = work1_pt;
	}
	else
	{
		work1_pt->back_pt->next_pt = work1_pt;
	}

	if (manager->Bot_pt == work2_pt)
	{
		manager->Bot_pt = work1_pt;
	}
	else
	{
		work1_pt->next_pt->back_pt = work1_pt;
	}
}

/*=====================================================================
���X�g�I�u�W�F�N�g�擪�ݒu�֐�
	���X�g��ɂ���I�u�W�F�N�g���擪�Ɏ����Ă���
	��������ł͓���ւ��Ȃ�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
	void* pObject,			: �擪�Ɏ����Ă��������X�g��ɑ��݂���I�u�W�F�N�g
=====================================================================*/
void MyListSetTopObject(MyList list, void* pObject)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *work_pt = &((MyListHead*)pObject)[-1];

	if (manager->Top_pt == work_pt)return;

	// Iterator�̐�������
	if (manager->work_pt == work_pt)
	{
		if (manager->isLoopFromTop)
		{
			manager->work_pt = work_pt->next_pt;
		}
		else
		{
			manager->work_pt = work_pt->back_pt;
		}
	}

	// ������ꏊ�̒u������
	if (manager->Bot_pt == work_pt)
	{
		work_pt->back_pt->next_pt = NULL;
		manager->Bot_pt = work_pt->back_pt;
	}
	else
	{
		work_pt->back_pt->next_pt = work_pt->next_pt;
		work_pt->next_pt->back_pt = work_pt->back_pt;
	}

	// �擪�̓���ւ�
	manager->Top_pt->back_pt = work_pt;
	work_pt->next_pt = manager->Top_pt;
	work_pt->back_pt = NULL;
	manager->Top_pt = work_pt;	
}

/*=====================================================================
���X�g�I�u�W�F�N�g�I�[�ݒu�֐�
	���X�g��ɂ���I�u�W�F�N�g����I�[�Ɏ����Ă���
	��������ł͓���ւ��Ȃ�
	�߂�l:void
	�����G
	MyList list,			: ���X�g�@�\
	void* pObject,			: �I�[�Ɏ����Ă��������X�g��ɑ��݂���I�u�W�F�N�g
=====================================================================*/
void MyListSetBottomObject(MyList list, void* pObject)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *work_pt = &((MyListHead*)pObject)[-1];

	if (manager->Bot_pt == work_pt)return;

	// Iterator�̐�������
	if (manager->work_pt == work_pt)
	{
		if (manager->isLoopFromTop)
		{
			manager->work_pt = work_pt->next_pt;
		}
		else
		{
			manager->work_pt = work_pt->back_pt;
		}
	}

	// ������ꏊ�̒u������
	if (manager->Top_pt == work_pt)
	{
		work_pt->next_pt->back_pt = NULL;
		manager->Top_pt = work_pt->next_pt;
	}
	else
	{
		work_pt->back_pt->next_pt = work_pt->next_pt;
		work_pt->next_pt->back_pt = work_pt->back_pt;
	}

	// �I�[�̓���ւ�
	manager->Bot_pt->next_pt = work_pt;
	work_pt->back_pt = manager->Bot_pt;
	work_pt->next_pt = NULL;
	manager->Bot_pt = work_pt;
}

/*=====================================================================
���X�g�v�[�����擾�֐�
�v�[�����Ă��鐔������Ƃ�����֐�
	�߂�l:DWORD
	�����G
	MyList list,			: ���X�g�@�\
=====================================================================*/
DWORD MyListGetNumPoolObject(MyList list)
{
	return ((ListManager*)list)->numPoolObj;
}

//----------------------------------------------------------------
// cpp���֐�
//----------------------------------------------------------------

/*=====================================================================
[cpp���֐�]���X�g�I�u�W�F�N�g�����Ȃ����쐬�֐�
	�v�[������Ă��郊�X�g�I�u�W�F�N�g��T���ĊY���I�u�W�F�N�g������΂����
	������ΐV�K�쐬���s���֐�

	�߂�l:MyListHead * ���������쐬�����I�u�W�F�N�g��header�A�h���X
	�����G
	ListManager* manager,	:���X�g�@�\��private���������́F
	size_t DynamicSize		:�I�u�W�F�N�g���ƂɈقȂ�֐�
=====================================================================*/
MyListHead *GetObjectFromPool(ListManager* manager, size_t DynamicSize)
{
	MyListHead *work_pt = manager->PoolTop_pt;

	while (work_pt != NULL)
	{
		if (work_pt->DynamicSize >= DynamicSize)
			break;
		
		work_pt = work_pt->next_pt;
	}

	// �v�[����������o�����ꍇ�ƈ����o���Ȃ��ꍇ
	if (work_pt)
	{	// �����o������
		manager->numPoolObj--;
		ZeroMemory(&work_pt[1], manager->Obj_Size + work_pt->DynamicSize);

		if (work_pt->back_pt)	work_pt->back_pt->next_pt	= work_pt->next_pt;
		else					manager->PoolTop_pt			= work_pt->next_pt;

		if (work_pt->next_pt)	work_pt->next_pt->back_pt	= work_pt->back_pt;
		else					manager->PoolBot_pt			= work_pt->back_pt;

		work_pt->back_pt = NULL;
		work_pt->next_pt = NULL;
	}
	else
	{	// �쐬����
		DynamicSize = NUM_STR(DynamicSize);
		work_pt = (MyListHead*)calloc(1, sizeof(MyListHead) + manager->Obj_Size + DynamicSize);
		work_pt->DynamicSize = DynamicSize;
	}

	manager->ListData.numObj++;	// �I�u�W�F�N�g�J�E���g�C���N�������g
	return work_pt;
}


/*=====================================================================
[cpp���֐�]���X�g�I�u�W�F�N�g���O�y�уv�[���֐�
	���X�g�I�u�W�F�N�g�����X�g���珜�O���ăv�[�����X�g�ւƈړ�������֐�
	�v�[���̏���𒴂����ꍇ�͈�ԃA�h���X�l���傫�����̂��J������
	�߂�l:void
	�����G
	ListManager* manager,	:���X�g�@�\��private���������́F
	MyListHead *del_pt		: �{���X�g�ɑ��݂���
=====================================================================*/
void DelObjAndPool(ListManager *manager, MyListHead *del_pt)
{
	MyListHead *work_pt = manager->PoolTop_pt;

	if (!del_pt)return;	// �k���`�F�b�N

	// Iterator�̐�������
	if (del_pt == manager->work_pt)
	{
		if (manager->isLoopFromTop)manager->work_pt = del_pt->next_pt;
		else						manager->work_pt = del_pt->back_pt;
	}

	// ��������
	if (del_pt->back_pt)	del_pt->back_pt->next_pt = del_pt->next_pt;
	else					manager->Top_pt = del_pt->next_pt;

	if (del_pt->next_pt)	del_pt->next_pt->back_pt = del_pt->back_pt;
	else					manager->Bot_pt = del_pt->back_pt;

	del_pt->back_pt = del_pt->next_pt = 0;

	manager->ListData.numObj--;	// �I�u�W�F�N�g�J�E���g�̌���

	// �k���Ȃ�Ώ��߂Ă�����
	if (!work_pt)
	{
		manager->PoolBot_pt =
			manager->PoolTop_pt = del_pt;

		manager->numPoolObj = 1;

		return;
	}

	// �v�[�����X�g���񂵂ăA�h���X���ɕ��ׂ�
	while (work_pt != NULL)
	{
		// �A�h���X�Ԓn���o��|�C���^����ɂ���ꍇ
		if (work_pt > del_pt)
			break;
		
		work_pt = work_pt->next_pt;
	}

	// �A�h���X���ɕ��ׂ�ۂ̐�������
	if (!work_pt)
	{
		// �I�[������pt => del_pt�ɕ��ׂ�
		MyListHead *keep_pt = manager->PoolBot_pt;

		manager->PoolBot_pt = del_pt;
		del_pt->back_pt = keep_pt;
		del_pt->next_pt = NULL;
		keep_pt->next_pt = del_pt;
	}
	else
	{
		// del_pt => work_pt�ɕ��ׂ�

		MyListHead *keep_pt = work_pt->back_pt;

		// �o�b�N���̐���
		work_pt->back_pt = del_pt;
		del_pt->back_pt = keep_pt;

		// �l�N�X�g���̐���
		if (keep_pt)keep_pt->next_pt = del_pt;
		else		manager->PoolTop_pt = del_pt;

		del_pt->next_pt = work_pt;
	}
	
	// �v�[������ő吔�𒴂����ꍇ
	if (++manager->numPoolObj > manager->maxPoolObj)
	{
		MyListHead *keep_pt = manager->PoolBot_pt;

		manager->numPoolObj--;

		// �I�[�̐�������
		manager->PoolBot_pt = manager->PoolBot_pt->back_pt;
		manager->PoolBot_pt->next_pt = NULL;

		// �J��
		free(keep_pt);
	}
	return;
}