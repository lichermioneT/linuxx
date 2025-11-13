#include <iostream>
#include <ctime>
#include <stdlib.h>
using namespace std;

template<class K, class V>
struct AVLTreeNode
{
  AVLTreeNode<K, V>* left;
  AVLTreeNode<K, V>* right;
  AVLTreeNode<K, V>* parent;
  int bf;
  pair<K, V> KV;
  AVLTreeNode(const pair<K,V>& kv)
    :left(nullptr)
    ,right(nullptr)
    ,parent(nullptr)
    ,bf(0)
    ,KV(kv)
    {}
};

template<class K, class V>
class AVLTree 
{
  typedef AVLTreeNode<K,V> Node;
public: 
    bool insert(const pair<K, V>& kv)
    {
      if(root == nullptr)
      {
        root = new Node(kv); 
        return true;
      }

      Node* parent  = nullptr;
      Node* cur = root;

      while(cur != NULL)
      {
        if(cur->KV.first > kv.first)
        {
          parent = cur;
          cur = cur->left;
        }
        else if(cur->KV.first < kv.first) 
        {
          parent = cur;
          cur = cur->right;
        }
        else 
        {
          return false;
        }
      }

      cur = new Node(kv);
      if(parent->KV.first > kv.first)
      {
        parent->left= cur;
        cur->parent = parent;
      }
      else 
      {
        parent->right = cur;
        cur->parent = parent;
      }

// 更新平衡因子的
// 插入右边++
// 插入左边--
      while(parent)
      {
        if(cur == parent->right)
        {
          parent->bf++;
        }
        else 
        {
          parent->bf--;
        }

        if(parent->bf == 0)
        {
          break;
        }
        else if(parent->bf == 1 || parent->bf == -1)
        {
          cur = parent;
          parent = parent->parent;
        }
        else if(parent->bf == 2 || parent->bf == -2)
        {
          if(parent->bf == 2)
          {
            if(cur->bf == 1)
            {
              RotateL(parent);
            }
            else if(cur->bf == -1)
            {
              RotateRL(parent);
            }
          }
          else if(parent->bf == -2)
          {
            if(cur->bf == -1)
            {
              RotateR(parent);
            }
            else if (cur->bf == 1)
            {
              RotateLR(parent);    
            }
          }
        break;
        }
      }
      return true;
    }
// 2             parent 
//   1           cur
//     0
  void RotateL(Node* parent)
  {
    Node* subR = parent->right;
    Node* subRL = subR->left;
    Node* ppnode = parent->parent;

// 旋转，左边高的拉下来
    parent->right = subRL;
    if(subRL != nullptr)
    {
      subRL->parent = parent;
    }

    subR->left = parent;
    parent->parent = subR;

// 判断是不是成为根节点
    // parent sf
    // 2
    //   1
    //     0
    if(root == parent)
    {
      root = subR;
      subR->parent = nullptr;
    }
    else 
    {
// 处理两个平行线的问题
//     Z
//   2   2 
//     1  1 
//       0  0
      if(ppnode->left == parent)
      {
        ppnode->left = subR;
      }
      else 
      {
        ppnode->right = subR;
      }

      subR->parent = ppnode;
    }
// 拉下来 subR->bf等于零了，parent->bf等于零了
    subR->bf = 0;
    parent->bf = 0;
  }

//     2
//   1
// 0
//
  void RotateR(Node* parent)
  {
    Node* subL =  parent->left;
    Node* subLR = subL->right;
    Node* ppnode = parent->parent;

// 旋转右边高的拉下去
    parent->left = subLR;
    if(subLR != nullptr)
    {
      subLR->parent = parent;
    }
    
    subL->right= parent;
    parent->parent = subL;

// 新的高节点是不是，跟节点
    if(root == parent)
    {
      root = subL;
      subL->parent = nullptr;
    }
    else 
    {
// 处理两个平行的情况
      if(ppnode->left == parent)
      {
        ppnode->left = subL;
      }
      else 
      {
        ppnode->right = subL;
      }
      subL->parent = ppnode;
    }
// 处理过后就是 parent的bf原来是2，现在下来了就是零，subL左边-1，也是零了。
    subL->bf = 0;
    parent->bf = 0;
  }

//  5          
//    7       
//  6
//
//  5  
//    6
//      7
//
//  6
// 5 7
  void RotateRL(Node* parent)
  {
      Node* subR = parent->right;
      Node* subRL = subR->left;
      int bf = subRL->bf;

      RotateR(parent->right);
      RotateL(parent);

      if(bf == -1)
      {
        parent->bf = 0;
        subR->bf  = 1;
        subRL->bf = 0;
      }
      else if(bf == 1)
      {
        subR->bf = 0;
        parent->bf = -1;
        subRL->bf = 0;
      }
      else if(bf == 0)
      {
        subR->bf = 0;
        parent->bf = 0;
        subRL->bf = 0;
      }
  }

  void RotateLR(Node* parent)
  {
    Node* subL = parent->left;
    Node* subRL = subL->right;
    int bf = subRL->bf;
    
    RotateL(subL);
    RotateR(parent);
    
    if(bf == 1)
    {
      parent->bf = 0;
      subL->bf  = -1;
      subRL->bf = 0;
    }
    else if(bf == -1)
    {
      parent->bf = 0;
      subL->bf = 1;
      subRL->bf = 0;
    }
    else if(bf == 0)
    {
      parent->bf = 0;
      subL->bf  = 0;
      subRL->bf = 0;
    }
  }

// 中序遍历 (递归)
void InOrder()
{
    cout << "InOrder: ";
    _InOrder(root);
    cout << endl;
}

void _InOrder(Node* root)
{
    if (root == nullptr)
        return;

    _InOrder(root->left);
    cout << "(" << root->KV.first << ", " << root->KV.second 
         << ", bf=" << root->bf << ") " <<endl;
    _InOrder(root->right);
}
private: 
  Node* root = nullptr;
};

void  test()
{
  int a[] = {23,43,6,7,8,9,5,77};
  AVLTree<int, int> t;
  for(auto e : a)
  {
      t.insert(make_pair(e,e));
  }
  t.InOrder();
} 

void test1()
{
  srand((unsigned long)time(nullptr));
  
  AVLTree<int, int> t;
  for(int i = 0; i < 100; i++)
  { 
    int r = rand() % 200;
    t.insert(make_pair(r, r));
  }

  t.InOrder();
}

int main()
{

//  test();
 test1(); 
  return 0;
}
