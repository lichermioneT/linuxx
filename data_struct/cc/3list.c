#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int LDataType;
typedef struct ListNode
{
  LDataType data;
  struct ListNode* next;
  struct ListNode* prev;
}ListNode;


ListNode* ListCreate()
{
  ListNode* phead = (ListNode*)malloc(sizeof(ListNode));
  if(phead == NULL)
  {
    perror("malloc failed");
    exit(1);
  }

  phead->data = 0;
  phead->prev = phead; 
  phead->next = phead;

  return phead;
}

void ListDestory(ListNode* plist)
{
  assert(plist);
  
  ListNode* cur = plist->next;
  ListNode* free_cur = NULL;
  while(cur != plist)
  {
      free_cur = cur;
      cur = cur->next;
      free(free_cur);
  }
}

void ListPrint(ListNode* plist)
{
  assert(plist);

  ListNode* cur = plist->next;
  while(cur != plist)
  {
    printf("%d ", cur->data);
    cur = cur->next;
  }
}

ListNode* BuyListNode(LDataType x)
{
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if(newNode == NULL)
    {
      perror("malloc failed");
      exit(1);
    }

    newNode->data = x;
    newNode->next = newNode;
    newNode->prev = newNode;
    return newNode;
}

void ListPushBack(ListNode* plist, LDataType x)
{
  assert(plist);
  
  ListNode* newNode = BuyListNode(x);
  
  ListNode* tail = plist->prev; // 最后一个节点位置

  tail->next = newNode;  // 尾部链接
  newNode->prev = tail;

  newNode->next = plist; // 头部链接
  plist->prev = newNode;
}


void ListPopBack(ListNode* phead)
{
  assert(phead);
  
  ListNode* tail = phead->prev;
  ListNode* tailPrev = tail->prev;
  
  if(tail != phead)
  {
    free(tail);
  }

  tailPrev->next = phead;
  phead->prev =tailPrev;
}


void ListPushFront(ListNode* phead, LDataType x)
{
  assert(phead);
  ListNode* newNode = BuyListNode(x);

  ListNode* first = phead->next; // 保存头节点
  
  phead->next = newNode;
  newNode->prev = phead;

  newNode->next = first;
  first->prev = newNode;
}

void ListPopFront(ListNode* phead)
{
  assert(phead);
  
  if(phead->next == phead)
  {
    return;
  }

  ListNode* first = phead->next;
  ListNode* second = first->next;

  free(first);

  second->prev = phead;
  phead->next = second;
}

ListNode* listFind(ListNode* phead, LDataType x)
{
  assert(phead);
  
  ListNode* cur = phead->next;
  while(cur != phead)
  {
    if(cur->data == x)
      return cur;

    cur = cur->next;
  }
  
  return NULL;
}


void ListInsert(ListNode* pos, LDataType x)
{
  assert(pos);
  
  ListNode* newNode = BuyListNode(x);
  
  ListNode* posPrev = pos->prev;
  
  posPrev->next = newNode;
  newNode->prev = posPrev;;

  newNode->next = pos;
  pos->prev = newNode;

}

void ListErase(ListNode* pos)
{
  assert(pos);
  
  ListNode* posNext = pos->next;
  ListNode* posPrev = pos->prev;

  free(pos);
  
  posPrev->next = posNext;
  posNext->prev = posPrev;
}



int main()
{

  // 头节点 哨兵
  ListNode* phead = ListCreate(); 
  
  for(int i = 0; i < 10; i++)
  {
    ListPushBack(phead, i);
    ListPrint(phead);
    printf("\n");
  }

  for(int i = 0; i < 10; i++)
  {
    ListPopBack(phead);
    ListPrint(phead);
    printf("\n");
  }

  for(int i = 0; i < 10; i++)
  {
    ListPushFront(phead, i);
    ListPrint(phead);
    printf("\n");
  }

  for(int i = 0; i < 10; i++)
  {
    ListPopFront(phead);
    ListPrint(phead);
    printf("\n");
  }

  for(int i = 0; i < 10; i++)
  {
    ListPushFront(phead, i);
  }
  
  ListNode* pos = listFind(phead, 8);


  ListPrint(phead);
  printf("\n");
  

  ListInsert(pos, 10000000);
  ListPrint(phead);
  printf("\n");

  
  pos = listFind(phead, 9);
  ListErase(pos);
  ListPrint(phead);
  printf("\n");


  pos = listFind(phead, 0);
  ListErase(pos);
  ListPrint(phead);
  printf("\n");

  pos = listFind(phead, 6);
  ListErase(pos);
  ListPrint(phead);
  printf("\n");


  return 0;
}
