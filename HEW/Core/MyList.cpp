/**********************************************************************
[[リストプログラム(MyList.cpp)]]
	作者：奥田　真規

	リスト作成に関するプログラム
***********************************************************************/
#include "MyList.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------


// aを16の倍数に収まるようにする
#define NUM_STR(a)		((a) + (16 - ((a) % 16)))			

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

// リストObjheader
typedef struct MyListHead {
	MyListHead *next_pt;		// 次のリストへのアドレス
	MyListHead *back_pt;		// 前の　　同上
	size_t		DynamicSize;	// オブジェクトによって異なる箇所のサイズ(Object_Sizeを除く)
}MyListHead;

// リスト管理構造体
typedef struct ListManager {
	LIST		ListData;			// 公開するリストデータ
	size_t		Obj_Size;			// リスト一つ当たりのサイズ(ヘッダを除く)

	MyListHead	*Top_pt;			// 先頭リストオブジェクト
	MyListHead	*Bot_pt;			// 終端リストオブジェクト

	MyListHead	*work_pt;			// リストイテレータ（現在回しているアドレス）
	bool		isLoopFromTop;		// イテレータの方向(true:先頭から)

	MyListHead	*PoolTop_pt;		// 一時保管所(先頭)（malloc回数軽減）
	MyListHead	*PoolBot_pt;		// 一時保管所（終端）（malloc回数軽減）
	DWORD		numPoolObj;			// 一時保管するオブジェクト個数
	DWORD		maxPoolObj;			// 同上最大数
}ListManager;

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
MyListHead *GetObjectFromPool(ListManager* manager, size_t DynamicSize);
void DelObjAndPool(ListManager *manager, MyListHead *del_pt);

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

//----------------------------------------------------------------
// リスト機能作成、削除関数
//----------------------------------------------------------------

/*=====================================================================
リスト機能作成関数
	リストを使用するために準備する関数。リストごとに用意する
	戻り値:MyList 
	引数；
	size_t ObjectSize,	: リスト1要素当たりの大きさ
						（オブジェクトごとに変異するサイズを除く）
	DWORD numPool		: オブジェクトプール数(使わなくなったオブジェクトを一時保存する)
							(必須でないが32になる)
	size_t PublicSize	: 共有データの大きさ（必須でない）
=====================================================================*/
MyList MyListCreate(size_t ObjectSize, DWORD numPool, size_t PublicSize)
{
	ListManager *list = NULL;

	// メモリ確保（ゼロクリア）
	list = (ListManager*)calloc(1, sizeof(ListManager) + PublicSize);

	if (PublicSize <= 0)
		list->ListData.Public_pt = &list[1];		// 共有データ群はメモリの最後に持ってくる

	list->Obj_Size = ObjectSize;					// 1オブジェクトのサイズ
	list->maxPoolObj = numPool;						// プール最大数

	return &list->ListData;
}

/*=====================================================================
リスト機能削除関数
	全てのリストオブジェクトを開放してリスト機能を削除する関数
	戻り値:void
	引数；
	MyList* DelList : 削除するリスト（関数実行後NULLが入る）
=====================================================================*/
void MyListDelete(MyList *DelList)
{
	ListManager *manager = (ListManager*)*DelList;
	MyListHead *work_pt, *del_pt;

	// 本リストの開放
	work_pt = manager->Top_pt;
	while (work_pt != NULL)
	{
		del_pt = work_pt;
		work_pt = work_pt->next_pt;
		free(del_pt);
	}

	// プールリストの開放
	work_pt = manager->PoolTop_pt;
	while (work_pt != NULL)
	{
		del_pt = work_pt;
		work_pt = work_pt->next_pt;
		free(del_pt);
	}

	// リスト管理構造体の開放
	free(*DelList);
	*DelList = NULL;
}


//----------------------------------------------------------------
// リストオブジェクト追加、排除関数
//----------------------------------------------------------------

/*=====================================================================
リストオブジェクト変異サイズ変更関数
	オブジェクトごとにある変異メモリサイズを変更する関数

	戻り値:void*  : 新たに変わった可能性のあるオブジェクトアドレス
	引数；
	MyList list,			: リスト機能
	void *work_pt,			：オブジェクトアドレス
	size_t DynamicSize		: 新たな変異サイズ
=====================================================================*/
void *MyListChangeDynamicSize(MyList list, void *work_pt, size_t nSize)
{
	if (!work_pt)return work_pt;

	ListManager *manager = (ListManager*)list;

	// 呼ばれたオブジェクトからheaderアドレスを求める
	MyListHead *Idx_Head = &((MyListHead*)work_pt)[-1];
	MyListHead *keep_pt;

	// 大きさの許容ができた場合は作り直さない
	if (Idx_Head->DynamicSize >= nSize)return work_pt;
	
	// 新たにオブジェクトを用意してコピー
	keep_pt = GetObjectFromPool(manager, nSize);
	memcpy(&keep_pt[1], &Idx_Head[1], sizeof(MyListHead) + manager->Obj_Size + Idx_Head->DynamicSize);
	keep_pt->back_pt = Idx_Head->back_pt;
	keep_pt->next_pt = Idx_Head->next_pt;


	// 元のオブジェクトを削除
	DelObjAndPool(manager, Idx_Head);

	// 整合処理
	if (keep_pt->back_pt)	keep_pt->back_pt->next_pt = keep_pt;
	else					manager->Top_pt = keep_pt;

	if (keep_pt->next_pt)	keep_pt->next_pt->back_pt = keep_pt;
	else					manager->Bot_pt = keep_pt;

	return &keep_pt[1];
}

/*=====================================================================
リストオブジェクト作成関数
	指定オブジェクトの前後にリストオブジェクトを作成する関数
	戻り値:void*  : 作成したリストの先頭アドレス
	引数；
	MyList list,			: リスト機能
	void *work_pt,			：前後にリストを作るための元となるオブジェクトアドレス
	bool isNext,			: (true)上引数の次オブジェクトとして作成する
							(false) 同上前オブジェクトとして作成する
	size_t DynamicSize		: オブジェクトごとにサイズが異なる確保サイズ(必須でない)
=====================================================================*/
void *MyListCreateObject(MyList list, void *work_pt, bool isNext, size_t DynamicSize)
{
	if (!work_pt)
		return MyListCreateObjectTop(list, DynamicSize);

	ListManager *manager = (ListManager*)list;
	MyListHead	*keep_pt = NULL;					// 補間ポインタ
	// 呼ばれたオブジェクトからheaderアドレスを求める
	MyListHead *Idx_Head = &((MyListHead*)work_pt)[-1];

	// 新オブジェクト
	MyListHead	*new_pt = GetObjectFromPool(manager, DynamicSize);

	if (isNext)
	{	// オブジェクトの次に設置
		keep_pt = Idx_Head->next_pt;

		// ネクスト順の整合
		Idx_Head->next_pt = new_pt;		
		new_pt->next_pt = keep_pt;	

		// バック順の整合
		if(keep_pt)	keep_pt->back_pt = new_pt;
		else		manager->Bot_pt = new_pt;

		new_pt->back_pt = Idx_Head;
	}
	else
	{	// オブジェクトの前に設置
		keep_pt = Idx_Head->back_pt;

		// バック順の整合
		Idx_Head->back_pt = new_pt;
		new_pt->back_pt = keep_pt;

		// ネクスト順の整合
		if (keep_pt)keep_pt->next_pt = new_pt;
		else		manager->Top_pt	= new_pt;

		new_pt->next_pt = Idx_Head;
	}
	return &new_pt[1];
}

/*=====================================================================
リストオブジェクト先頭箇所作成関数
	リストの先頭にオブジェクトを作成する関数
	戻り値:void*  : 作成したリストの先頭アドレス
	引数；
	MyList list,			: リスト機能
	size_t DynamicSize		: オブジェクトごとにサイズが異なる確保サイズ(必須でない)
=====================================================================*/
void *MyListCreateObjectTop(MyList list, size_t DynamicSize)
{
	ListManager *manager = (ListManager*)list;
	MyListHead	*keep_pt = manager->Top_pt;					// 補間ポインタ

	// 新オブジェクト
	MyListHead	*new_pt = GetObjectFromPool(manager, DynamicSize);

	// ネクスト順の整合
	manager->Top_pt = new_pt;
	if (!keep_pt)manager->Bot_pt = new_pt;
	else			keep_pt->back_pt = new_pt;

	new_pt->next_pt = keep_pt;

	// バック順の整合
	new_pt->back_pt = NULL;

	return &new_pt[1];
}

/*=====================================================================
リストオブジェクト終端箇所作成関数
	リストの終端にオブジェクトを作成する関数
	戻り値:void*  : 作成したリストの先頭アドレス
	引数；
	MyList list,			: リスト機能
	size_t DynamicSize		: オブジェクトごとにサイズが異なる確保サイズ(必須でない)
=====================================================================*/
void *MyListCreateObjectBottom(MyList list, size_t DynamicSize)
{
	ListManager *manager = (ListManager*)list;
	MyListHead	*keep_pt = manager->Bot_pt;		// 補間ポインタ

	// 新オブジェクト
	MyListHead	*new_pt = GetObjectFromPool(manager, DynamicSize);

	// バック順の整合
	manager->Bot_pt = new_pt;
	if (!keep_pt)manager->Top_pt = new_pt;
	else			keep_pt->next_pt = new_pt;

	new_pt->back_pt = keep_pt;

	// ネクスト順の整合
	new_pt->next_pt = NULL;

	return &new_pt[1];
}

/*=====================================================================
リストオブジェクト削除関数
	指定オブジェクトを削除する関数
	戻り値:void
	引数；
	MyList list,			: リスト機能
	void **del_pt			: 削除する対象のオブジェくと（実行後NULLが入る）
=====================================================================*/
void MyListDeleteObject(MyList list, void **del_pt)
{
	ListManager *manager = (ListManager*)list;

	MyListHead *DelHead_pt = &((MyListHead*)*del_pt)[-1];

	DelObjAndPool(manager, DelHead_pt);
	*del_pt = NULL;
}

/*=====================================================================
リストオブジェクト先頭箇所削除関数
	リストの先頭オブジェクトを削除する関数
	戻り値:void
	引数；
	MyList list,			: リスト機能
=====================================================================*/
void MyListDeleteObjectTop(MyList list)
{
	ListManager *manager = (ListManager*)list;

	DelObjAndPool(manager, manager->Top_pt);
}

/*=====================================================================
リストオブジェクト終端箇所削除関数
	リストの終端オブジェクトを削除する関数
	戻り値:void
	引数；
	MyList list,			: リスト機能
=====================================================================*/
void MyListDeleteObjectBottom(MyList list)
{
	ListManager *manager = (ListManager*)list;

	DelObjAndPool(manager, manager->Bot_pt);
}

/*=====================================================================
リストオブジェクト全部削除関数
	リストのオブジェクトを全て削除する関数
	戻り値:void
	引数；
	MyList list,			: リスト機能
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
// リストオブジェクト巡回機能関数
//----------------------------------------------------------------

/*=====================================================================
リストIterator設置指定関数
	リストを巡回する為のイテレータを設置指定し巡回方向を指定する関数
	戻り値:void
	引数；
	MyList list,			: リスト機能
	void *pStart,			: 先頭のアドレス
	bool isFromTop			: (true)先頭から終端の順で巡回するフラグを立てる
							(false) 終端から先頭の順で巡回するフラグを立てる
=====================================================================*/
void MyListSetIterator(MyList list, void *pStart, bool isFromTop)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *Head_pt = NULL;

	manager->isLoopFromTop = isFromTop;

	// 怖いのでヌルチェック
	if (pStart)Head_pt = &((MyListHead*)pStart)[-1];

	manager->work_pt = Head_pt;
}

/*=====================================================================
リストIterator設置関数
	リストを巡回する為のイテレータを設置し巡回方向を指定する関数
	戻り値:void
	引数；
	MyList list,			: リスト機能
	bool isSetTop			: (true)リストの先頭に設置する
									先頭から終端の順で巡回するフラグを立てる
							(false) 終端に設置する
									終端から先頭の順で巡回するフラグを立てる
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
リスト巡回関数
	リストを巡回仕切るまでtrueを返し、イテレータを進める関数
	while分の条件部に入れることでリストをすべて巡回することが可能
	戻り値:bool
					true:リストの巡回はまだ終わっていない
					false;リストの巡回は終了
	引数；
	MyList list,			: リスト機能
	void** work_pt			: 現在回しているIteratorのオブジェクトが返る
=====================================================================*/
bool MyListLoop(MyList list, void** work_pt)
{
	ListManager *manager = (ListManager*)list;

	// 次がない場合はfalseを返す
	if (manager->work_pt == NULL)
	{
		if(work_pt)*work_pt = NULL;
		return false;
	}

	if (work_pt)*work_pt = (void*)&manager->work_pt[1];

	// 双方向リストの進む順番を決定
	if (manager->isLoopFromTop)
		manager->work_pt = manager->work_pt->next_pt;
	else
		manager->work_pt = manager->work_pt->back_pt;

	return true;
}

/*=====================================================================
リスト先頭オブジェクト取得関数
	戻り値: void * 先頭オブジェクトアドレス
	引数；
	MyList list,			: リスト機能
=====================================================================*/
void *MyListGetTopObject(MyList list)
{
	ListManager *manager = (ListManager*)list;
	if (!manager->Top_pt)return NULL;

	return &manager->Top_pt[1];
}

/*=====================================================================
リスト終端オブジェクト取得関数
	戻り値: void * 終端オブジェクトアドレス
	引数；
	MyList list,			: リスト機能
=====================================================================*/
void *MyListGetBottomObject(MyList list)
{
	ListManager *manager = (ListManager*)list;
	if (!manager->Bot_pt)return NULL;

	return &manager->Bot_pt[1];
}

/*=====================================================================
リストオブジェクトスワップ関数
	２つのオブジェクトを順番上入れ替える関数
	メモリ上では入れ替えない
	戻り値:void
	引数；
	MyList list,			: リスト機能
	void* pObj1,			: 1番オブジェクト
	void* pObj2				: 2番オブジェクト
=====================================================================*/
void MyListSwap(MyList list, void* pObj1, void* pObj2)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *work1_pt = &((MyListHead*)pObj1)[-1];
	MyListHead *work2_pt = &((MyListHead*)pObj2)[-1];
	MyListHead keep;

	// イテレータの整合処理
	if (manager->work_pt == work1_pt)
	{
		manager->work_pt = work2_pt;
	}
	else if (manager->work_pt == work2_pt)
	{
		manager->work_pt = work1_pt;
	}

	// adrスワップ
	keep.back_pt = work1_pt->back_pt;
	keep.next_pt = work1_pt->next_pt;
	work1_pt->back_pt = work2_pt->back_pt;
	work1_pt->next_pt = work2_pt->next_pt;
	work2_pt->back_pt = keep.back_pt;
	work2_pt->next_pt = keep.next_pt;

	// 1番のswap作業
	if (manager->Top_pt == work1_pt)
	{
		// 2番の前はnull
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

	// 2番のswap作業
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
リストオブジェクト先頭設置関数
	リスト上にあるオブジェクト一つを先頭に持ってくる
	メモリ上では入れ替えない
	戻り値:void
	引数；
	MyList list,			: リスト機能
	void* pObject,			: 先頭に持ってきたいリスト上に存在するオブジェクト
=====================================================================*/
void MyListSetTopObject(MyList list, void* pObject)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *work_pt = &((MyListHead*)pObject)[-1];

	if (manager->Top_pt == work_pt)return;

	// Iteratorの整合処理
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

	// 今ある場所の置き換え
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

	// 先頭の入れ替え
	manager->Top_pt->back_pt = work_pt;
	work_pt->next_pt = manager->Top_pt;
	work_pt->back_pt = NULL;
	manager->Top_pt = work_pt;	
}

/*=====================================================================
リストオブジェクト終端設置関数
	リスト上にあるオブジェクト一つを終端に持ってくる
	メモリ上では入れ替えない
	戻り値:void
	引数；
	MyList list,			: リスト機能
	void* pObject,			: 終端に持ってきたいリスト上に存在するオブジェクト
=====================================================================*/
void MyListSetBottomObject(MyList list, void* pObject)
{
	ListManager *manager = (ListManager*)list;
	MyListHead *work_pt = &((MyListHead*)pObject)[-1];

	if (manager->Bot_pt == work_pt)return;

	// Iteratorの整合処理
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

	// 今ある場所の置き換え
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

	// 終端の入れ替え
	manager->Bot_pt->next_pt = work_pt;
	work_pt->back_pt = manager->Bot_pt;
	work_pt->next_pt = NULL;
	manager->Bot_pt = work_pt;
}

/*=====================================================================
リストプール数取得関数
プールしている数をしゅとくする関数
	戻り値:DWORD
	引数；
	MyList list,			: リスト機能
=====================================================================*/
DWORD MyListGetNumPoolObject(MyList list)
{
	return ((ListManager*)list)->numPoolObj;
}

//----------------------------------------------------------------
// cpp内関数
//----------------------------------------------------------------

/*=====================================================================
[cpp内関数]リストオブジェクト発見ないし作成関数
	プールされているリストオブジェクトを探して該当オブジェクトがあればそれを
	無ければ新規作成を行う関数

	戻り値:MyListHead * 見つけたか作成したオブジェクトのheaderアドレス
	引数；
	ListManager* manager,	:リスト機能をprivate化したもの：
	size_t DynamicSize		:オブジェクトごとに異なる関数
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

	// プールから引き出せた場合と引き出せない場合
	if (work_pt)
	{	// 引き出し処理
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
	{	// 作成処理
		DynamicSize = NUM_STR(DynamicSize);
		work_pt = (MyListHead*)calloc(1, sizeof(MyListHead) + manager->Obj_Size + DynamicSize);
		work_pt->DynamicSize = DynamicSize;
	}

	manager->ListData.numObj++;	// オブジェクトカウントインクリメント
	return work_pt;
}


/*=====================================================================
[cpp内関数]リストオブジェクト除外及びプール関数
	リストオブジェクトをリストから除外してプールリストへと移動させる関数
	プールの上限を超えた場合は一番アドレス値が大きいものを開放する
	戻り値:void
	引数；
	ListManager* manager,	:リスト機能をprivate化したもの：
	MyListHead *del_pt		: 本リストに存在する
=====================================================================*/
void DelObjAndPool(ListManager *manager, MyListHead *del_pt)
{
	MyListHead *work_pt = manager->PoolTop_pt;

	if (!del_pt)return;	// ヌルチェック

	// Iteratorの整合処理
	if (del_pt == manager->work_pt)
	{
		if (manager->isLoopFromTop)manager->work_pt = del_pt->next_pt;
		else						manager->work_pt = del_pt->back_pt;
	}

	// 整合処理
	if (del_pt->back_pt)	del_pt->back_pt->next_pt = del_pt->next_pt;
	else					manager->Top_pt = del_pt->next_pt;

	if (del_pt->next_pt)	del_pt->next_pt->back_pt = del_pt->back_pt;
	else					manager->Bot_pt = del_pt->back_pt;

	del_pt->back_pt = del_pt->next_pt = 0;

	manager->ListData.numObj--;	// オブジェクトカウントの減少

	// ヌルならば初めてを入れる
	if (!work_pt)
	{
		manager->PoolBot_pt =
			manager->PoolTop_pt = del_pt;

		manager->numPoolObj = 1;

		return;
	}

	// プールリストを回してアドレス順に並べる
	while (work_pt != NULL)
	{
		// アドレス番地が出るポインタより後にある場合
		if (work_pt > del_pt)
			break;
		
		work_pt = work_pt->next_pt;
	}

	// アドレス順に並べる際の整合処理
	if (!work_pt)
	{
		// 終端だったpt => del_ptに並べる
		MyListHead *keep_pt = manager->PoolBot_pt;

		manager->PoolBot_pt = del_pt;
		del_pt->back_pt = keep_pt;
		del_pt->next_pt = NULL;
		keep_pt->next_pt = del_pt;
	}
	else
	{
		// del_pt => work_ptに並べる

		MyListHead *keep_pt = work_pt->back_pt;

		// バック順の整合
		work_pt->back_pt = del_pt;
		del_pt->back_pt = keep_pt;

		// ネクスト順の整合
		if (keep_pt)keep_pt->next_pt = del_pt;
		else		manager->PoolTop_pt = del_pt;

		del_pt->next_pt = work_pt;
	}
	
	// プールする最大数を超えた場合
	if (++manager->numPoolObj > manager->maxPoolObj)
	{
		MyListHead *keep_pt = manager->PoolBot_pt;

		manager->numPoolObj--;

		// 終端の整合処理
		manager->PoolBot_pt = manager->PoolBot_pt->back_pt;
		manager->PoolBot_pt->next_pt = NULL;

		// 開放
		free(keep_pt);
	}
	return;
}