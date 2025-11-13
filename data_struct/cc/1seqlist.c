#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int SeqListDataType;

typedef struct seqlist 
{ 
  SeqListDataType* _data;
  size_t _size;
  size_t _capacity;
}seqlist;

void seqlistInit(seqlist* ps, size_t capacity)
{
  assert(ps);
  assert(capacity > 0);
  SeqListDataType* temp = (SeqListDataType*)malloc(sizeof(SeqListDataType) * capacity);
  
  if(temp == NULL)
  {
    perror("malloc failed");
    return;
  }

  ps->_data = temp;
  ps->_size = 0;
  ps->_capacity = capacity;
}

void seqlistDestory(seqlist* ps)
{
  assert(ps);
  free(ps->_data);
  ps->_data = NULL;
  ps->_size = ps->_capacity = 0;
}

void seqlistPrint(seqlist* ps)
{
  assert(ps);
  for(size_t i = 0; i < ps->_size; i++)
  {
    printf("%d ", ps->_data[i]);
  }
  printf("\n");
}

void seqlistCheckCapacity(seqlist* ps)
{
  assert(ps);
  if(ps->_size == ps->_capacity)
  {
    size_t newCapacity = ps->_capacity == 0 ? 4 : ps->_capacity * 2;
    SeqListDataType* temp = (SeqListDataType*)realloc(ps->_data, sizeof(SeqListDataType) * newCapacity);

    if(temp == NULL)
    {
      perror("realloc failed");
      exit(1);
    }
    
    ps->_capacity = newCapacity;
    ps->_data = temp;
  }
}

void seqlistPushBack(seqlist* ps, SeqListDataType x)
{
  assert(ps);
  seqlistCheckCapacity(ps);
  ps->_data[ps->_size] = x;
  ps->_size++;
}

void seqlistPopBack(seqlist* ps)
{
  assert(ps != NULL);
  assert(ps->_size > 0);
  ps->_size--;
}

void seqlistPusFront(seqlist* ps, SeqListDataType x)
{
  assert(ps != NULL);
  seqlistCheckCapacity(ps);
// pushfront 
// [1, size]
//
  for(size_t i = ps->_size; i > 0; i--)
  {
    ps->_data[i] = ps->_data[i - 1];
  }
  
  ps->_data[0] = x;
  ps->_size++;
}

void seqlistPopFront(seqlist* ps)
{
  assert(ps != NULL);
  assert(ps->_size > 0);

// 
  for(size_t i = 1; i <  ps->_size; i++)
  {
    ps->_data[i-1] = ps->_data[i];
  }

  ps->_size--;
}

int seqlistFind(seqlist* ps, SeqListDataType x)
{
  assert(ps);
  for(size_t i = 0; i < ps->_size; i++)
  {
    if(ps->_data[i] == x)
      return i;
  }

  return -1; 
}

void seqlistInsert(seqlist* ps, size_t pos, SeqListDataType x)
{
  assert(ps != NULL);
  assert(pos <= ps->_size);
  seqlistCheckCapacity(ps);
  
  for(size_t i = ps->_size; i > pos; i--)
  {
    ps->_data[i] = ps->_data[i-1];
  }

  ps->_data[pos] = x;
  ps->_size++;
}

void seqlistErase(seqlist* ps, size_t pos)
{
  assert(ps != NULL);
  assert(pos < ps->_size);
  
  for(size_t i = pos; i < ps->_size - 1; i++)
  {
    ps->_data[i] = ps->_data[i+1];
  }

  ps->_size--;
}

int main()
{

  seqlist s;                  // 创建一个结构体，如果一个函数需要拿到结构体，就需要用指针接收

  seqlistInit(&s, 4);

  seqlistPushBack(&s, 1);
  seqlistPushBack(&s, 2);
  seqlistPushBack(&s, 3);
  seqlistPushBack(&s, 4);
  seqlistPushBack(&s, 5);
  seqlistPushBack(&s, 6);
  seqlistPrint(&s);

  seqlistPopBack(&s);
  seqlistPrint(&s);
  seqlistPopBack(&s);
  seqlistPrint(&s);
  seqlistPopBack(&s);
  seqlistPrint(&s);
  seqlistPopBack(&s);
  seqlistPrint(&s);
  seqlistPopBack(&s);
  seqlistPrint(&s);
  seqlistPopBack(&s);
  
  seqlistPrint(&s);

  for(int i = 0; i <10; i++)
  {
    seqlistPusFront(&s, i);
  }
  
  for(int i = 0; i < 10; i++)
  {
    seqlistPopFront(&s);
    seqlistPrint(&s);
  }
  
  for(int i = 0; i < 20; i++)
  {
    seqlistPushBack(&s, i);
  }
  
  seqlistPrint(&s);
  int i = seqlistFind(&s, 22);
  printf("%d \n", i);
  i = seqlistFind(&s, 3);
  printf("%d \n", i);

  seqlistInsert(&s, 20, 1000);
  seqlistPrint(&s);
  seqlistInsert(&s, 0, 100);
  seqlistPrint(&s);

  seqlistErase(&s, 21);
  seqlistPrint(&s);


  seqlistDestory(&s);
  return 0;
}

