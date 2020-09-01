/*************************************
* Lab 1 Exercise 1
* Name: Wong Choon Wei
* Student No: A0202190X
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

// add in your implementation below to the respective functions
// feel free to add any headers you deem fit (although you do not need to)

// inserts a new node with data value at index (counting from the front
// starting at 0)
void insert_node_from_head_at(list *lst, int index, int data)
{
  if (lst->head == NULL) {
    node* newNode = (NODE*) malloc(sizeof(NODE));
    newNode->data = data;
    newNode-> prev = NULL;
    newNode->next = NULL;
    lst->head = &newNode
    lst->tail = &(lst->head)

  } else {
    if(index == 0) {
    // initialise new node
    node* newNode = (NODE*) malloc(sizeof(NODE));
    newNode->data = data;
    newNode->prev = &(lst->head->prev);
    newNode->next = &(lst->head);

    // change prev node
    lst->head->prev->next = &(newNode);
    
    //change index node
    lst->head->prev = &newNode; 

    } else {
      list newList = (list*) malloc(sizeof(list));
      newList->head = &(lst->head->next);
      insert_node_from_head_at(newList, index - 1, data);
    }
  }
}

// // inserts a new node with data value at index (counting from the back
// // starting at 0)
// void insert_node_from_tail_at(list *lst, int index, int data)
// {

// }

// // deletes node at index counting from the front (starting from 0)
// // note: index is guaranteed to be valid
// void delete_node_from_head_at(list *lst, int index)
// {
// }

// // deletes node at index counting from the back (starting from 0)
// // note: index is guaranteed to be valid
// void delete_node_from_tail_at(list *lst, int index)
// {
// }

// // resets list to an empty state (no nodes) and frees any allocated memory in
// // the process
// void reset_list(list *lst)
// {
// }
