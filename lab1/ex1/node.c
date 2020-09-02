/*************************************
* Lab 1 Exercise 1
* Name: Wong Choon Wei
* Student No: A0202190X
* Lab Group: 11
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
  node* searchedNode;
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
  node* nextNode;
  node* prevNode;
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

  newNode = NULL;
}

// inserts a new node with data value at index (counting from the back
// starting at 0)
void insert_node_from_tail_at(list *lst, int index, int data)
{
  node* nextNode;
  node* prevNode;
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

  newNode = NULL;

}

// deletes node at index counting from the front (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_head_at(list *lst, int index)
{
  node* nextNode;
  node* prevNode;
  node* deleteNode;

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

  if(prevNode == NULL && nextNode == NULL) {
      lst->head = NULL;
      lst->tail = NULL;
  }

  // free deleteNode
  free(deleteNode);


}

// deletes node at index counting from the back (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_tail_at(list *lst, int index)
{
  node* nextNode;
  node* prevNode;
  node* deleteNode;

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

  if(prevNode == NULL && nextNode == NULL) {
      lst->head = NULL;
      lst->tail = NULL;
  }

  // free deleteNode
  free(deleteNode);
}