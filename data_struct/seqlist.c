#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef int SeqListDatetype;

typedef struct SeqList 
{
  SeqListDatetype* data;
  size_t size;
  size_t capacity;
}SeqList;

void SeqListinit(SeqList* ps)
{
  assert(ps);
  SeqListDatetype* temp = (SeqListDatetype*)malloc(sizeof(SeqListDatetype) * 4);



}



int main()
{

  SeqList s;












  return 0;
}
