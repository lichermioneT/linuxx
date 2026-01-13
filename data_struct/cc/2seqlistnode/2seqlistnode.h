#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef int SLTDataType;
typedef struct SListNode
{
  SLTDataType data;
  struct SListNode* next;
}SListNode;

SListNode* BuySListNode(SLTDataType x);
void SListNodePrint(SListNode* phead);
void SListNodePushBack(SListNode** phead, SLTDataType x);
void SListNodePopBack(SListNode** phead);
void SListNodePushFront(SListNode** phead, SLTDataType x);
void SListNodePopFront(SListNode** phead);
SListNode* SListNodeFind(SListNode* phead, SLTDataType x);
void SListNodeInsertfter(SListNode* pos, SLTDataType x);
void SListNodeEraseAfter(SListNode* pos);

