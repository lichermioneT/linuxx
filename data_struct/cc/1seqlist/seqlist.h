#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int SeqListDataType;

typedef struct seqlist 
{ 
  SeqListDataType* _data; // 开辟一块空间，用来存储数据的地址。指针变量，就是用来存储空间的地址
  size_t _size;           // 当前里面有多少个数据
  size_t _capacity;       // 开辟空间的大小
}seqlist;

void seqlistInit(seqlist* ps, size_t capacit) ;// 需要操作结构体，所有传入指针。这里结构体是变量s，所以是一级指针。
void seqlistDestory(seqlist* ps);
void seqlistPrint(seqlist* ps);
void seqlistCheckCapacity(seqlist* ps);
void seqlistPushBack(seqlist* ps, SeqListDataType x);
void seqlistPopBack(seqlist* ps);
void seqlistPusFront(seqlist* ps, SeqListDataType x);
void seqlistPopFront(seqlist* ps);
int seqlistFind(seqlist* ps, SeqListDataType x);
void seqlistInsert(seqlist* ps, size_t pos, SeqListDataType x);
void seqlistErase(seqlist* ps, size_t pos);

