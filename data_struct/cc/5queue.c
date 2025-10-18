#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int QueueDataType;
typedef struct queueNode
{
  QueueDataType data;
  struct queueNode* next;
}queueNode;

typedef struct queue 
{
  queueNode* head;
  queueNode* tail;
}queue;

void QueueInit(queue* pq)
{
  assert(pq);
  pq->head = pq->tail = NULL;
}

void QueueDestory(queue* pq)
{
  assert(pq);
  queueNode* cur = pq->head;
  queueNode* curNext = NULL;
  while(cur != NULL)
  {
    curNext = cur->next;
    free(cur);
    cur = curNext;
  }
  pq->head = pq->tail = NULL;
}

void QueuePrint(queue* pq)
{
  assert(pq);
  queueNode* cur = pq->head;
  
  while(cur != NULL)
  {
    printf("%d ", cur->data);
    cur = cur->next;
  }
}


void QueuePush(queue* pq, QueueDataType x)
{
  assert(pq);
  queueNode* newNode = (queueNode*)malloc(sizeof(queueNode));
  if(newNode == NULL)
  {
    perror("malloc failed");
    exit(1);
  }

  newNode->data = x;
  newNode->next = NULL;

  if(pq->head == NULL)
  {
    pq->head = pq->tail = newNode;
  }
  else 
  {
    pq->tail->next = newNode;
    pq->tail = newNode;
  }
}

// 先进去的元素，出去
void QueuePop(queue* pq)
{
  assert(pq);
  assert(pq->head);
  
  queueNode* del = pq->head;
  pq->head = del->next;
  free(del);
  
// 只有一个节点后的处理
  if(pq->head == NULL)
  {
    pq->tail = NULL;
  }
}

// 获取最后一个元素
QueueDataType QueueBack(queue* pq)
{
  assert(pq);
  assert(pq->head);

  return pq->tail->data;
}

size_t QueueSize(queue* pq)
{
  assert(pq);
  size_t count = 0;

  queueNode* cur = pq->head;
  while(cur != NULL)
  {
    count++;
    cur = cur->next;
  }
  
  return count;
}

int QueueEmpty(queue* pq)
{
  assert(pq);

  return pq->head == NULL ? 1 : 0;
}


int main()
{
  
  queue q;
  QueueInit(&q);
  for(int i = 0; i < 10; i++)
  {
    QueuePush(&q, i);
    QueuePrint(&q);
    printf("\n");
  }

  size_t cnt = QueueSize(&q);
  printf("%zd \n", cnt);






  return 0;
}

