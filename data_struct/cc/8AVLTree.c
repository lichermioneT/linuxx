#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int AVLTreeDataType;
typedef struct AVLTreeNode
{
  AVLTreeDataType data;
  struct AVLTreeNode* left;
  struct AVLTreeNode* right;
  struct AVLTreeNode* parent;
  int heigt;
}AVLTreeNode;


void AVLTreeInsert(AVLTreeNode** root, AVLTreeDataType x)
{
  assert(root);
  AVLTreeNode* newNode = (AVLTreeNode*)malloc(sizeof(AVLTreeNode));
  if(newNode == NULL)
  {
    perror("malloc failed");
    exit(1);
  }
  newNode->data = x;
  newNode->left = NULL;
  newNode->parent = NULL;
  newNode->parent = NULL;

  if(*root == NULL)
  {
    *root = newNode;
    return;
  }
  
  AVLTreeNode* cur = *root;
  AVLTreeNode* parent = NULL;
  while(cur != NULL)
  {
    parent = cur;
    if(cur->data > x)
    {
      cur = cur->left;
    }
    else if(cur->data < x)
    {
      cur = cur->right;
    }
    else 
    {
      return; // 直接退出
    }
  }
  
  if(parent->data > x)
  {
    parent->left = newNode;
    newNode->parent = parent; // 和父亲链接起来
  }
  else 
  {
    parent->right = newNode;
    newNode->parent = parent; // 和父亲链接起来
  }

  // 更新平衡因子
  // 


}







int main()
{

  AVLTreeNode* root = NULL;















  return 0;
}
