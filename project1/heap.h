#ifndef __HEAP__
#define __HEAP__

typedef struct HeapNode{
  struct HeapNode* right;
  struct HeapNode* left;
  struct HeapNode* parent;
  double score;
  int id;
} HeapNode;

typedef struct Heap{
  HeapNode* root;
  int numOfNodes;
}Heap;

void InsertHeap(Heap* heap, double score, int id);
Heap* CreateHeap();
void cleanHeap();
int printtLeftHeap(HeapNode *node,int k);
int printRightHeap(HeapNode *node,int k);


#endif
