#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "concurrent_list.h"

 struct node {
    int value;
    pthread_mutex_t mtxNode ;
     node* next ;
  // add more fields
};
 struct list {
  // add fields
    node* head;
    pthread_mutex_t mtxList;
};

void print_node(node* node)
{
  // DO NOT DELETE
  if(node)
  {
    printf("%d ", node->value);
  }
}

list* create_list()
{
  // add code here
    int is_empty = 0;
    list* newList;
    newList = (list*)malloc(sizeof(list));
    if (newList == NULL)
        is_empty = 1;
    if (is_empty == 0) {
        newList->head = NULL;
        pthread_mutex_init(&(newList->mtxList), NULL);
        return newList;
    }
    else
        return NULL; // REPLACE WITH YOUR OWN CODE
}

void delete_list(list* list)
{
  // add code here
    node  *myNode=NULL, *nextNode=NULL;
    int is_empty = 0;
    if ((list == NULL) || (list->head == NULL))
        is_empty = 1;
    if (is_empty == 0) {
        myNode = list->head;
        pthread_mutex_lock(&(list->mtxList));
        pthread_mutex_lock(&(myNode->mtxNode));
        while (myNode != NULL)
        {
            nextNode = myNode->next;
            if(nextNode!=NULL){
                pthread_mutex_lock(&(nextNode->mtxNode));
            }
            pthread_mutex_unlock(&(myNode->mtxNode));
            pthread_mutex_destroy(&(myNode->mtxNode));
            free(myNode);
            myNode = nextNode;
        }
        pthread_mutex_unlock(&(list->mtxList));
        pthread_mutex_destroy(&(list->mtxList));
        free(list);
    }
}

void insert_value(list* list, int value)
{
    node* curr=NULL;
    node* new_node=(node*)malloc(sizeof(node));
    pthread_mutex_init(&(new_node->mtxNode),NULL);
    pthread_mutex_lock(&(new_node->mtxNode));
    new_node->value=value;
    new_node->next=NULL;
    if(list->head==NULL){
        list->head=new_node;
        pthread_mutex_unlock(&(new_node->mtxNode));
        return;
    }
    curr=list->head;
    pthread_mutex_lock(&(curr->mtxNode));
    if(curr->value > value){
        new_node->next=curr;
        list->head=new_node;
        pthread_mutex_unlock(&(new_node->mtxNode));
        pthread_mutex_unlock(&(curr->mtxNode));
        return;
    }
    curr=list->head;
    node* temp=NULL;
    while(curr!=NULL&&curr->value<value){
        if(curr->next!=NULL){
            pthread_mutex_lock(&(curr->next->mtxNode));
        }
        pthread_mutex_unlock(&(curr->mtxNode));
         temp=curr;
        curr=curr->next;
    }

        pthread_mutex_lock(&(temp->mtxNode));
        temp->next=new_node;

        new_node->next=curr;
        pthread_mutex_unlock(&(temp->mtxNode));
        pthread_mutex_unlock(&(new_node->mtxNode));
        if(curr!=NULL) {
            pthread_mutex_unlock(&(curr->mtxNode));
        }
    return;






    // add code here
}

void remove_value(list* list, int value)
{
    node* prev=NULL;
    node* curr=list->head;
    if(curr!=NULL){
        pthread_mutex_lock(&(curr->mtxNode));
    }else{
        return;
    }
    if(curr->value==value){
       list->head=curr->next;
        pthread_mutex_unlock(&(curr->mtxNode));
        pthread_mutex_destroy(&(curr->mtxNode));
        free(curr);
        return;

    }
    while(curr!=NULL&&curr->value<value){
        if(curr->next!=NULL){
            pthread_mutex_lock(&(curr->next->mtxNode));
        }
        prev=curr;
        pthread_mutex_unlock(&(curr->mtxNode));
        curr=curr->next;
    }
    pthread_mutex_lock(&(prev->mtxNode));
    if(curr!=NULL){
        if(curr->value==value){
            prev->next=curr->next;
            pthread_mutex_unlock(&(curr->mtxNode));
            pthread_mutex_destroy(&(curr->mtxNode));
            pthread_mutex_unlock(&(prev->mtxNode));
            free(curr);
            return;
        }else{
            pthread_mutex_unlock(&(curr->mtxNode));
            pthread_mutex_unlock(&(prev->mtxNode));
            return;

        }

    }
    pthread_mutex_unlock(&(prev->mtxNode));
    return;

  // add code here
}

void print_list(list* list)
{
          node* curr;
          if(list==NULL) {
              printf("\n"); // DO NOT DELETE
              return;
          }
    curr = list->head;
          if(curr!=NULL) {
              pthread_mutex_lock(&(curr->mtxNode));
          }
          while(curr!=NULL){
printf("%d ", curr->value);
if (curr->next!=NULL){
    pthread_mutex_lock(&(curr->next->mtxNode));
}
pthread_mutex_unlock(&(curr->mtxNode));
curr=curr->next;

}
    printf("\n"); // DO NOT DELETE
}

void count_list(list* list, int (*predicate)(int))
{
  int count = 0; // DO NOT DELETE
  node* curr=NULL;
  if(list!=NULL) {
      curr = list->head;
  }
  if(curr!=NULL) {
      pthread_mutex_lock(&(curr->mtxNode));
  }
    while(curr!=NULL){
        if((*predicate)(curr->value)){
            count++;
        }
        if (curr->next!=NULL){
            pthread_mutex_lock(&(curr->next->mtxNode));
        }
        pthread_mutex_unlock(&(curr->mtxNode));
        curr=curr->next;

    }

  // add code here

  printf("%d items were counted\n", count); // DO NOT DELETE
}
