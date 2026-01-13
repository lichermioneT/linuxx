#include "seqlist.h"
void seqlistInit(seqlist* ps, size_t capacity) // 需要操作结构体，所有传入指针。这里结构体是变量s，所以是一级指针。
{
  assert(ps);
  assert(capacity > 0);
  SeqListDataType* temp = (SeqListDataType*)malloc(sizeof(SeqListDataType) * capacity);
  
  if(temp == NULL)
  {
    perror("malloc failed");
    return;
  }

// 初始化三个数据
  ps->_data = temp;
  ps->_size = 0;
  ps->_capacity = capacity;
}

void seqlistDestory(seqlist* ps)
{
  assert(ps);
  free(ps->_data);
  ps->_data = NULL; // 防止指针变量里面还存储原来空间的大小
  ps->_size = ps->_capacity = 0; // 归零
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

// 数据结构里面涉及到扩容 就要 扩容！
void seqlistCheckCapacity(seqlist* ps)
{
  assert(ps);
  if(ps->_size == ps->_capacity)
  {
    size_t newCapacity = ps->_capacity == 0 ? 4 : ps->_capacity * 2;
    SeqListDataType* temp = (SeqListDataType*)realloc(ps->_data, sizeof(SeqListDataType) * newCapacity); // realloc会释放原来的空间大小的！

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
// [0, size-1]
  for(size_t i = ps->_size; i > 0; i--) // 数据全部往后面移动一个，留出第一个位置。
  {
    ps->_data[i] = ps->_data[i - 1];    // [size-1, 1];
  }
  
  ps->_data[0] = x;
  ps->_size++;
}

void seqlistPopFront(seqlist* ps)
{
  assert(ps != NULL);
  assert(ps->_size > 0);

//[1,szie-1] 
  for(size_t i = 1; i <  ps->_size; i++)
  {
    ps->_data[i-1] = ps->_data[i];//[1, size-1];
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

