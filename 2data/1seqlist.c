#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int seqlistdatatype;
typedef struct seqlist 
{
  seqlistdatatype* _data;
  size_t _size;
  size_t _capacity;
}seqlist;

void seqlistInit(seqlist* ps, size_t capacity)
{
  assert(ps != NULL);
  
  seqlistdatatype* temp = (seqlistdatatype*)malloc(sizeof(seqlistdatatype) * capacity);
  if(temp == NULL)
  {
    perror("malloc failed");
    return; 
  }
  
  ps->_data = temp;
  ps->_capacity = capacity;
  ps->_size = 0;
}

void seqlistDestory(seqlist* ps)
{
  assert(ps != NULL);
  free(ps->_data);
  ps->_data = NULL;
  ps->_size = ps->_capacity = 0;
}

void seqlistPrint(seqlist* ps)
{
  assert(ps != NULL);
  for(size_t i = 0; i < ps->_size; i++)
  {
    printf("%d ", ps->_data[i]);
  }
  printf("\n");
}


void seqlistCheckCapacity(seqlist* ps)
{
  assert(ps != NULL);
  if(ps->_size == ps->_capacity)
  {
    size_t newcapacity = ps->_capacity == 0 ? 4 : ps->_capacity * 2;
    seqlistdatatype* temp = (seqlistdatatype*)realloc(ps->_data, sizeof(seqlistdatatype)* newcapacity);
    if(temp == NULL)
    {
      perror("realloc failed ");
      exit(1);
    }

    ps->_data = temp;
    ps->_capacity =  newcapacity;
  }
}

/* 指针是
 * [pos + 1, _size];
 *  i = i - 1
 *
 *
 */ 
void seqlistInsert(seqlist* ps, size_t pos, seqlistdatatype x)
{
  assert(ps != NULL);
  assert(pos <= ps->_size);
  seqlistCheckCapacity(ps);
  for(size_t i = ps->_size; i > pos; i--)
  {
    ps->_data[i] = ps->_data[i - 1];
  }
  
  ps->_data[pos] = x;
  ps->_size++;
}


void seqlistErase(seqlist* ps, size_t pos)
{
  assert(ps != NULL);
  assert(ps->_size > 0);
  assert(pos <ps->_size);
  for(size_t i = pos; i < ps->_size - 1; i++)
  {
    ps->_data[i] = ps->_data[i+1];
  }

  ps->_size--;

}


void seqlistPushBack(seqlist* ps, seqlistdatatype x)
{
  seqlistInsert(ps,ps->_size, x);
}

void seqlistPopBack(seqlist* ps)
{
  seqlistErase(ps, ps->_size-1);
}

void seqlistPushFront(seqlist* ps, seqlistdatatype x)
{
  seqlistInsert(ps, 0, x);
}

void seqlistPopFront(seqlist* ps)
{
  seqlistErase(ps,0);
}


int main()
{





  return 0;
}
