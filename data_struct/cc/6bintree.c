#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef char BTDataType;
typedef struct BinaryTreeNode
{
  struct BinaryTreeNode* left;
  struct BinaryTreeNode* right;
  BTDataType dta;
}BinaryTreeNode;

BinaryTreeNode* BuyNode(BTDataType x)
{
  BinaryTreeNode* Node = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
  if(Node == NULL)
  {
    perror("malloc faile");
    exit(1);
  }
  Node->dta = x;
  Node->left = NULL;
  Node->right = NULL;
  return Node;
}


void PreOrder(BinaryTreeNode* root)
{
  if(root == NULL)
  {
    return;
  }

  printf("%c \n", root->dta);
  PreOrder(root->left);
  PreOrder(root->right);
}


void InOrder(BinaryTreeNode* root)
{
  if(root == NULL)
  {
    return;
  }

  PreOrder(root->left);
  printf("%c \n", root->dta);
  PreOrder(root->right);
}

void PostOrder(BinaryTreeNode* root)
{
  if(root == NULL)
  {
    return;
  }

  PreOrder(root->left);
  PreOrder(root->right);
  printf("%c \n", root->dta);
}

void BinaryTreeDestory(BinaryTreeNode* root)
{
  if(root == NULL)
    return;

  BinaryTreeDestory(root->left);
  BinaryTreeDestory(root->right);
  
  printf("%c \n", root->dta);
  free(root);
}




int main()
{
  
  BinaryTreeNode* A = BuyNode('a'); 
  BinaryTreeNode* B = BuyNode('b'); 
  BinaryTreeNode* C = BuyNode('c'); 
  BinaryTreeNode* D = BuyNode('d'); 
  BinaryTreeNode* E = BuyNode('e'); 

  A->left = B;
  A->right = C;

  B->left = D;
  B->right = E;
  PreOrder(A);


  BinaryTreeDestory(A);



  return 0;
}
