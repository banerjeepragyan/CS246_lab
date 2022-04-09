#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
int MAX;
int DATAMAX;
int MX = max(MAX, DATAMAX);
int in = 0, dn = 0;
class Node 
{
  bool IS_LEAF;
  int *key, size;
  Node **ptr;
  friend class BPTree;
   public:
  Node();
};
class BPTree 
{
  Node *root;
  void insertInternal(int, Node *, Node *);
  Node *findParent(Node *, Node *);
   public:
  BPTree();
  void insert(int);
  void display(Node *);
  void display_root(Node *);
  Node *getRoot();
};
Node::Node() 
{
  key = new int[MX];
  ptr = new Node *[MX + 1];
}
BPTree::BPTree() 
{
  root = NULL;
}
void BPTree::insert(int x) 
{
  if (root == NULL) 
  {
    root = new Node;
    root->key[0] = x;
    root->IS_LEAF = true;
    root->size = 1;
  } 
  else 
  {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false) 
    {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) 
      {
        if (x < cursor->key[i]) 
        {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) 
        {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < DATAMAX) 
    {
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size)
      {
        i++;
      }
      for (int j = cursor->size; j > i; j--) 
      {
        cursor->key[j] = cursor->key[j - 1];
      }
      cursor->key[i] = x;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } 
    else 
    {
      Node *newLeaf = new Node;
      int virtualNode[DATAMAX + 1];
      for (int i = 0; i < DATAMAX; i++) 
      {
        virtualNode[i] = cursor->key[i];
      }
      int i = 0, j;
      while (x > virtualNode[i] && i < DATAMAX)
      {
        i++;
      }
      for (int j = DATAMAX + 1; j > i; j--) 
      {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i] = x;
      newLeaf->IS_LEAF = true;
      cursor->size = (DATAMAX + 1) / 2;
      newLeaf->size = DATAMAX + 1 - (DATAMAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[DATAMAX];
      cursor->ptr[DATAMAX] = NULL;
      for (i = 0; i < cursor->size; i++) 
      {
        cursor->key[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) 
      {
        newLeaf->key[i] = virtualNode[j];
      }
      if (cursor == root) 
      {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } 
      else 
      {
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}
void BPTree::insertInternal(int x, Node *cursor, Node *child) 
{
  int up = 0;
  if (cursor->size < MAX) 
  {
    int i = 0;
    while (x > cursor->key[i] && i < cursor->size)
    {
      i++;
    }
    for (int j = cursor->size; j > i; j--) 
    {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) 
    {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i] = x;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } 
  else 
  {
    Node *newInternal = new Node;
    int virtualKey[MAX + 1];
    Node *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++) 
    {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) 
    {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < MAX)
    {
      i++;
    }
    for (int j = MAX + 1; j > i; j--) 
    {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    up = virtualKey[((MAX+1)/2)-1];
    for (int j = MAX + 2; j > i + 1; j--) 
    {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = MAX - (MAX + 1) / 2;
    newInternal->size = (MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) 
    {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) 
    {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) 
    {
      Node *newRoot = new Node;
      newRoot->key[0] = up;
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } 
    else 
    {
      insertInternal(up, findParent(root, cursor), newInternal);
    }
  }
}
Node *BPTree::findParent(Node *cursor, Node *child) 
{
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) 
  {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) 
  {
    if (cursor->ptr[i] == child) 
    {
      parent = cursor;
      return parent;
    } 
    else 
    {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
      {
        return parent;
      }
    }
  }
  return parent;
}
void BPTree::display(Node *cursor) 
{
  if (cursor != NULL) 
  {
    if (cursor->IS_LEAF != true) 
    {
        in++;
      for (int i = 0; i < cursor->size + 1; i++) 
      {
        display(cursor->ptr[i]);
      }      
    }
    else
    {
      dn++;
    }
  }
}
void BPTree::display_root(Node *cursor) 
{
  if (cursor != NULL) 
  {
    for (int i = 0; i < cursor->size; i++) 
    {
      cout << cursor->key[i] << " ";
    }
    cout << "\n";
  }
}
Node *BPTree::getRoot() 
{
  return root;
}
int main() 
{
  BPTree node;
  #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
  #endif
  int d, t;
  cin >> d >> t;
  DATAMAX = (2*d);
  MAX = (2*t)+1;
  MX = max(DATAMAX, MAX);
  int ch, key;
  while (true)
  {
    cin >> ch;
    if (ch == 1)
    {
      cin >> key;
      node.insert(key);
    }
    else if (ch == 2 )
    {
      in = 0;
      dn = 0;
      node.display(node.getRoot());
      cout << in << " " << dn << " ";
      node.display_root(node.getRoot());
    }
    else if (ch == 3)
    {
      break;
    }
    else
    {
      cout << "WRONG CHOICE!" << endl;
    }
  }
  return 0;
}