#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

Heap* CreateHeap(){
  Heap* heap;
  heap= malloc(sizeof(Heap));
  heap->numOfNodes=0;
  heap->root = NULL;
  return heap;
}

HeapNode* CreateHeapNode(HeapNode* parent,double score,int id){
    HeapNode* heapNode;
    heapNode = malloc(sizeof(HeapNode));
    heapNode->score = score;
    heapNode->left = NULL;
    heapNode->right = NULL;
    heapNode->parent = parent;
    heapNode->id= id;
    return heapNode;
}

void heapify(HeapNode* node){
  HeapNode* tempParent = node->parent;
  HeapNode* tempNode = node;
  double tempScore;
  int tempid;
  if(tempParent == NULL)return;
  while(tempParent->score < node->score){
    tempScore = node->score;
    node->score = tempParent->score;
    tempParent->score = tempScore;

    tempid = node->id;
    node->id = tempParent->id;
    tempParent->id = tempid;

    tempNode = tempParent;
    tempParent = tempParent->parent;
    if(tempParent == NULL)break;
  }
}

void InsertHeap(Heap* heap, double score,int id){
  if ( heap->numOfNodes==0){
      heap->root = CreateHeapNode(NULL,score,id);
      heap->numOfNodes++;
      return;
  }
  heap->numOfNodes++;
  int path[100];
  int count=0,i;
  int current = heap->numOfNodes;
  while(current>1){
    if(current%2==1){
      path[count]=1;
      count++;
    }else{
      path[count]=0;
      count++;
    }
    current=current/2;
  }
  HeapNode* temp = heap->root;
  for(i=0 ; i < count-1 ; i++){
    if(path[i]==0){
      temp = temp->left;
    }else if(path[i]==1){
      temp = temp->right;
    }else{
      printf("error in heap insert\n" );
      return ;
    }
  }
  if(path[count-1] == 0){
    temp -> left =CreateHeapNode(temp,score,id);
    heapify(temp->left);
  }else if(path[count-1]==1){
    temp -> right =CreateHeapNode(temp,score,id);
    heapify(temp->right);
  }else{
    printf("error in heap insert \n");
    return;
  }
}


void cleanHeapNode(HeapNode *node){
  if(node->left!=NULL){
    cleanHeapNode(node->left);
  }
  if(node->right!=NULL){
    cleanHeapNode(node->right);
  }
  free(node);
}

void cleanHeap(Heap *heap){
    cleanHeapNode(heap->root);
    free(heap);
}

int printtLeftHeap(HeapNode *node,int k){
  while (k!=0){
    if (node->left!=NULL){
      k--;
      printRightHeap(node->left,k);
    }
  }
  return node->id;

}
int printRightHeap(HeapNode *node,int k){
  while(k!=0){
    if (node->right!=NULL){
      k--;
      printtLeftHeap(node->right,k);
    }
  }
  return node->id;
}
