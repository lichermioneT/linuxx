#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int NodeDataType;
typedef struct Node 
{
    NodeDataType data;
    struct Node* next;
}Node;

Node* CreateNode(NodeDataType x)
{
  Node* newnode = (Node*)malloc(sizeof(Node));
  if(newnode == NULL)
  {
    perror("malloc failed");
    exit(1);
  }
  newnode->next = NULL;
  newnode->data = x;
  return newnode;
}

void print(Node* phead)
{
  while(phead != NULL)
  {
    printf("%d ", phead->data);
    phead = phead->next;
  }
  printf("\n");
}

void NodePushBack(Node** phead, NodeDataType x)
{
  assert(phead);
    
  Node* newnode = CreateNode(x);

  if((*phead) == NULL)
  {
    *phead = newnode;
  }
  else 
  {
    Node* tail = *phead;
    while(tail->next != NULL)
    {
        tail = tail->next;
    }
    tail->next = newnode;
  }
}

void NodePopBack(Node** phead)
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
    Node* tail = *phead;
    Node* tailpre = NULL;

    while(tail->next != NULL)
    {
      tailpre = tail;
      tail = tail->next;
    }

    free(tail);
    tailpre->next = NULL;
  }
}

void NodePushFront(Node** phead, NodeDataType x)
{
  assert(phead);
  Node* newnode = CreateNode(x);
  
  newnode->next = *phead;

  *phead = newnode;
}

void NodePopFront(Node** phead)
{
  assert(phead);
  
  if(*phead == NULL)
  {
    return;
  }
  else 
  {
    // 保存第二个节点
    Node* second = (*phead)->next;
    free(*phead);
    *phead = second;
  }
}







int main()
{

  Node* phead = NULL;

  for(int i = 0; i < 20; i++)
  {
    NodePushBack(&phead, i); 
  } 
  print(phead);

  for(int i = 0; i < 20; i++)
  {
    NodePopBack(&phead); 
    print(phead);
  } 

  for(int i = 0; i < 20; i++)
  {
    NodePushFront(&phead, i);
    print(phead);
  }








  return 0;
}
