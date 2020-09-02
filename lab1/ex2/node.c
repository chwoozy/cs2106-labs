/*************************************
* Lab 1 Exercise 2
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include <stdbool.h>

// add in your implementation below to the respective functions
// feel free to add any headers you deem fit (although you do not need to)

node* search_node(list *lst, int index, bool front) {
  // initialise search node
  node* searchedNode = (node*) malloc(sizeof(node));
  searchedNode = front ? lst->head : lst->tail;

  for(int i = 0; i < index; i++) {
    if (front) {
      searchedNode = searchedNode->next;
    } else {
      searchedNode = searchedNode->prev;
    }
  }
  return searchedNode;
}

// inserts a new node with data value at index (counting from the front
// starting at 0)
void insert_node_from_head_at(list *lst, int index, int data)
{
  node* nextNode = (node*) malloc(sizeof(node));
  node* prevNode = (node*) malloc(sizeof(node));

  node* newNode = (node*) malloc(sizeof(node));

  if (index != 0) {
    prevNode = search_node(lst, index - 1, true);
  } else {
    prevNode = NULL;
  }
  nextNode = search_node(lst, index, true);

  // set newNode
  newNode->data = data;
  newNode->prev = prevNode;
  newNode->next = nextNode;

  // set prevNode
  if (prevNode != NULL) {
    prevNode->next = newNode;
  }

  // set nextNode
  if (nextNode != NULL) {
    nextNode->prev = newNode;
  }

  if (prevNode == NULL && nextNode == NULL) {
    lst->head = newNode;
    lst->tail = newNode;
  }

  // reorder list
  if (index == 0) {
    lst->head = newNode;
  }

  if (index != 0 && nextNode == NULL) {
    lst->tail = newNode;
  }

}

// inserts a new node with data value at index (counting from the back
// starting at 0)
void insert_node_from_tail_at(list *lst, int index, int data)
{
  node* nextNode = (node*) malloc(sizeof(node));
  node* prevNode = (node*) malloc(sizeof(node));
  node* newNode = (node*) malloc(sizeof(node));

  if (index != 0) {
    nextNode = search_node(lst, index - 1, false);
  } else {
    nextNode = NULL;
  }
  prevNode = search_node(lst, index, false);

  // set newNode
  newNode->data = data;
  newNode->prev = prevNode;
  newNode->next = nextNode;

  // set prevNode
  if (prevNode != NULL) {
    prevNode->next = newNode;
  }

  // set nextNode
  if (nextNode != NULL) {
    nextNode->prev = newNode;
  }

  if (prevNode == NULL && nextNode == NULL) {
    lst->head = newNode;
    lst->tail = newNode;
  }

  // reorder list
  if (index == 0) {
    lst->tail = newNode;
  }

  if (index != 0 && prevNode == NULL) {
    lst->head = newNode;
  }

}

// deletes node at index counting from the front (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_head_at(list *lst, int index)
{
  node* nextNode = (node*) malloc(sizeof(node));
  node* prevNode = (node*) malloc(sizeof(node));
  node* deleteNode = (node*) malloc(sizeof(node));

  deleteNode = search_node(lst, index, true);

  // get prevNode
  if (index != 0) {
    prevNode = search_node(lst, index - 1, true);
  } else {
    prevNode = NULL;
  }

  // get nextNode
  if (deleteNode != NULL) {
    nextNode = deleteNode->next;
  } else {
    nextNode = NULL;
  }

  //set nodes
  if (prevNode != NULL) {
     prevNode->next = nextNode;
  }

  if (nextNode!= NULL) {
    nextNode->prev = prevNode;
  }

  // set list
  if (index == 0) {
    lst->head = nextNode;
  } else {
    if (deleteNode == NULL || nextNode == NULL) {
      lst->tail = prevNode;
    }
  }

  // free deleteNode
  free(deleteNode);


}

// deletes node at index counting from the back (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_tail_at(list *lst, int index)
{
  node* nextNode = (node*) malloc(sizeof(node));
  node* prevNode = (node*) malloc(sizeof(node));
  node* deleteNode = (node*) malloc(sizeof(node));

  deleteNode = search_node(lst, index, false);

  // get nextNode
  if (index != 0) {
    nextNode = search_node(lst, index - 1, false);
  } else {
    nextNode = NULL;
  }

  // get prevNode
  if (deleteNode != NULL) {
    prevNode = deleteNode->prev;
  } else {
    prevNode = NULL;
  }

  //set nodes
  if (prevNode != NULL) {
     prevNode->next = nextNode;
  }

  if (nextNode!= NULL) {
    nextNode->prev = prevNode;
  }

  // set list
  if (index == 0) {
    lst->tail = prevNode;
  } else {
    if (deleteNode == NULL || prevNode == NULL) {
      lst->head = nextNode;
    }
  }

  // free deleteNode
  free(deleteNode);
}

// resets list to an empty state (no nodes) and frees any allocated memory in
// the process  
void reset_list(list *lst)
{
  node *curr = lst->head;
  while (curr != NULL)
    {
        node *releaseNode = curr;
        curr = curr->next;
        free(releaseNode);
    }
  
  lst->head = NULL;
  lst->tail = NULL;
}


// traverses list and applies func on data values of all elements in the list
void map(list *lst, int (*func)(int))
{
    node *curr = lst->head;

    while (curr != NULL)
    {
        curr->data = func(curr->data);
        curr = curr->next;
    }
}

// traverses list and returns the sum of the data values of every node
// in the list
long sum_list(list *lst)
{
    node *curr = lst->head;
    long sum = 0;

    while (curr != NULL)
    {
        sum = sum + ((long) curr->data);
        curr = curr->next;
    }
    return sum;
}
