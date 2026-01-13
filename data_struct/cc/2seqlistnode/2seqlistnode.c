#include "2seqlistnode.h"

SListNode* BuySListNode(SLTDataType x)
{
  SListNode* newNode = (SListNode*)malloc(sizeof(SListNode));
  if(newNode == NULL)
  {
    perror("malloc failed");
    return NULL;
  }
  
  newNode->data = x;
  newNode->next = NULL;
  return newNode;
}

void SListNodePrint(SListNode* phead)
{
  SListNode* cur = phead;
  while(cur != NULL)
  {
    printf("%d ", cur->data);
    cur = cur->next;
  }
  printf("\n");
}

void SListNodePushBack(SListNode** phead, SLTDataType x)
{
  assert(phead);
  SListNode* newNode = BuySListNode(x);
  
  if(*phead == NULL)
  {
    *phead = newNode;
  }
  else 
  {
    SListNode* tail= *phead;
    while(tail->next != NULL)
    {
      tail = tail->next;
    }
    tail->next = newNode;
  }
}

void SListNodePopBack(SListNode** phead)
{
  assert(phead);
  
  if(*phead == NULL)
  {
    return;
  }
  else if ((*phead)->next == NULL)
  {
    free(*phead);
    *phead = NULL;
  }
  else 
  {
    SListNode* tail = *phead;
    SListNode* tailPre = NULL;

    while(tail->next != NULL)
    {
      tailPre = tail;
      tail = tail->next;
    }
    free(tail);
    tailPre->next = NULL;
  }
}

void SListNodePushFront(SListNode** phead, SLTDataType x)
{
  assert(phead);
  SListNode* newNode = BuySListNode(x);

  if(*phead == NULL)
  {
    *phead = newNode;
  }
  else  
  {
    newNode->next = *phead;
    *phead = newNode;
  }
}

void SListNodePopFront(SListNode** phead)
{
  assert(phead);
  if(*phead == NULL)
  {
    return;
  }
  else if((*phead)->next == NULL)
  {
      free(*phead);
      *phead = NULL;
  }
  else 
  {
    SListNode* newHead = (*phead)->next;
    free(*phead);
    *phead = newHead;
  }
}


SListNode* SListNodeFind(SListNode* phead, SLTDataType x)
{
  assert(phead);
  SListNode* cur = phead;
  while(cur != NULL)
  {
      if(cur->data == x)
        return cur;

      cur = cur->next;
  }
  return NULL;
}

void SListNodeInsertfter(SListNode* pos, SLTDataType x)
{
  assert(pos);

  SListNode* newNode = BuySListNode(x);

  newNode->next = pos->next;
  pos->next = newNode;
}

void SListNodeEraseAfter(SListNode* pos)
{
  assert(pos);
  
  SListNode* del = pos->next;
  if(del == NULL)
  {
    return;
  }
  else 
  {
    pos->next = del->next;
    free(del);
  }
}
