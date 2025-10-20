#include <iostream>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
  AVLTreeNode<K, V>* letf;
  AVLTreeNode<K, V>* right;
  AVLTreeNode<K, V>* parent;
  int bf;
  pair<K, V> KV;
  AVLTreeNode(const pair<K,V> kv)
    :letf(nullptr)
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
          cur = cur->letf;
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
        parent->right = cur;
        cur->parent = parent;
      }
      else 
      {
        parent->letf = cur;
        cur->parent = parent;
      }

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
// 2
//   1
//     0
  void RotateL(Node* parent)
  {
    Node* subR = parent->right;
    Node* subRL = subR->letf;

    parent->right = subRL;
    if(subRL != nullptr)
    {
      subRL->parent = parent;
    }

    subR->letf = parent;
    Node* ppnode = parent->parent;
    parent->parent = subR;
    
    // parent sf
    if(root == parent)
    {
      root = subR;
      subR->parent = nullptr;
    }
    else 
    {
      if(ppnode->letf == parent)
      {
        ppnode->letf = subR;
      }
      else 
      {
        ppnode->right = subR;
      }

      subR->parent = ppnode;
    }

    subR->bf = 0;
    parent->bf = 0;
  }

  void RotateR(Node* parent)
  {
    Node* subL =  parent->letf;
    Node* subLR = subL->right;
  
    parent->letf = subLR;
    if(subLR != nullptr)
    {
      subLR->parent = parent;
    }
    
    subL->letf = parent;
    Node* ppnode = parent->parent;
    parent->parent = subL;

    if(root == parent)
    {
      root = subL;
      subL->parent = nullptr;
    }
    else 
    {
      if(ppnode->letf == parent)
      {
        ppnode->letf = subL;
      }
      else 
      {
        ppnode->right = subL;
      }
      subL->parent = ppnode;
    }

    subL->bf = 0;
    parent->bf = 0;
  }

  void RotateRL(Node* parent)
  {
      Node* subR = parent->right;
      Node* subRL = parent->letf;
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
    Node* subL = parent->letf;
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

} 


int main()
{

  test();
  














  return 0;
}
