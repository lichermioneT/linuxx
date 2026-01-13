#include "2seqlistnode.h"

int main()
{
  SListNode* phead = NULL; // 一级指针，存放变量的地址，如果要操作它的话，需要取地址，然后接受他的话需要二级指针的。
  
  for(int i = 0; i < 10; i++)
  {
      SListNodePushBack(&phead, i);
      SListNodePrint(phead);
  }

  for(int i = 0; i < 10; i++)
  {
      SListNodePopBack(&phead);
      SListNodePrint(phead);
  }


  for(int i = 0; i < 10; i++)
  {
      SListNodePushFront(&phead, i);
      SListNodePrint(phead);
  }


  for(int i = 0; i < 10; i++)
  {
      SListNodePopFront(&phead);
      SListNodePrint(phead);
  }
  
  for(int i = 0; i < 10; i++)
  {
    SListNodePushFront(&phead, i);
    SListNode* pos = SListNodeFind(phead, i);
    
    pos->data *= 2;
    SListNodePrint(phead);
  }

  SListNode* pos = SListNodeFind(phead, 6);
  SListNodeInsertfter(pos, 1000);
  SListNodePrint(phead);
  
  pos = SListNodeFind(phead, 18);
  SListNodeEraseAfter(pos);
  SListNodePrint(phead);
  return 0;
}
