#ifndef LIST
#define LIST
#include <stdio.h>
#include <stdlib.h>

void insert(int* tour, int* len, int k, int value) {
  int i;
  
  if(k<0 || k > *len) {
    printf("Error in insert: out of range\n");
    return;
  }
  
  for(i=*len;i>k;i--) {
    tour[i]=tour[i-1];
  }
  tour[k]=value;
  (*len)++;
}

void removeAtIndex(int* arr, int *size, int index) {
    if (index < 0 || index >= *size) {
        // Invalid index
        printf("Error in insert: out of range\n");
        return;
    }

    // Shift elements to the left to fill the gap
    for (int i = index; i < *size - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Decrease the size of the array
    (*size)--;
}

struct cell {
  int data;
  struct cell* prev;
  struct cell* next;
};

struct list {
  struct cell* head;
  struct cell* tail;
};

void initialize(struct list* l) {
  l->head = (struct cell*)malloc(sizeof(struct cell));
  l->tail = (struct cell*)malloc(sizeof(struct cell));
  l->head->prev = NULL;
  l->head->next = l->tail;
  l->tail->prev = l->head;
  l->tail->next = NULL;
}

void insertBefore(struct cell* p, int v) {
  struct cell* newcell = (struct cell*)malloc(sizeof(struct cell));
  newcell->data = v;
  newcell->next = p;
  p->prev->next = newcell;
  newcell->prev = p->prev;
  p->prev = newcell;
}

void insertAfter(struct cell* p, int v) {
  struct cell* newcell = (struct cell*)malloc(sizeof(struct cell));
  newcell->data = v;
  newcell->prev = p;
  p->next->prev = newcell;
  newcell->next = p->next;
  p->next = newcell;
}

void erase(struct cell* p) {
  p->prev->next = p->next;
  p->next->prev = p->prev;
  free(p);
}

int isEmpty(struct list* l) {
  return l->head->next == l->tail;
}

void clear(struct list* l) {
  while (!isEmpty(l))
    erase(l->head->next);
  free(l->head);
  free(l->tail);
}

void printNumbers(struct list* l) {
  for (struct cell* p = l->head->next; p->next != NULL; p = p->next)
    printf("%d ", p->data);
  printf("\n");
}

void printString(struct list* l) {
  for (struct cell* p = l->head->next; p->next != NULL; p = p->next)
    printf("%c ", p->data);
  printf("\n");
}
#endif 
