#include <stdlib.h>
#include <stdio.h>

// define a struct for node
typedef struct node{
   int val;
   struct node *next;
} node;

// define a struct for singly linked list
typedef struct{
   int size;
   struct node *first;
} singlylinkedlist;

// prototypes
singlylinkedlist* clist();
int add(singlylinkedlist *list, int n);
int addarray(singlylinkedlist *list, int n, int *nums);
void delete(singlylinkedlist *list);
void freelist(singlylinkedlist *list);
void printlist(singlylinkedlist *list);
node* _cnode(int n, node* nextnode);
node* _last(singlylinkedlist *list);

int main(void){
    singlylinkedlist *list = clist();
    add(list, 100);
    printf("size: %d\n", list -> size);
    add(list, 200);
    printf("size: %d\n", list -> size);
    add(list, 300);
    printf("size: %d\n", list -> size);
    int nums[] = {400, 500, 600};
    addarray(list, 3, nums);
    printf("size: %d\n", list -> size);
    delete(list);
    delete(list);
    printlist(list);
    freelist(list);
}

// create a empty singly linked list
singlylinkedlist* clist(){
    singlylinkedlist *list = malloc(sizeof(singlylinkedlist));
    if (list == NULL){
        return NULL;
    }
    list -> size = 0;
    list -> first = NULL;
    return list;
}

// add one node to the end of the slingly linked list
int add(singlylinkedlist *list, int n){
    if (list == NULL){
        return -1;
    }
    node *last = _last(list);
    node *new = _cnode(n, NULL);
    if (new == NULL){
        return 0;
    }
    if (last == NULL){
        list -> first = new;
        (list -> size)++;
        return 1;
    }
    last -> next = new;
    (list -> size)++;
    return 1;
}

// add nodes to the end of the slingly linked list from an array
int addarray(singlylinkedlist *list, int n, int *nums){
    if (n == 0 || list == NULL){
        return -1;
    }
    node *last = _last(list);
    node *tmp = _cnode(nums[0], NULL);
    if (tmp == NULL){
        return 0;
    }
    if (last == NULL){
        list -> first = tmp;
    } else{
        last -> next = tmp;
    }
    (list -> size)++;
    for (int i = 1; i < n; i++){
        tmp -> next = _cnode(nums[i], NULL);
        tmp = tmp -> next;
        if (tmp == NULL){
            return i;
        }
        (list -> size)++;
    }
    return n;
}

// delete the last node of the singly linked list
void delete(singlylinkedlist *list){
    if (list == NULL || list -> size == 0){
        return;
    }
    if (list -> size == 1){
        free(list -> first);
        list -> first = NULL;
        (list -> size)--;
        return;
    }
    node *tmp = list -> first;
    for (int i = 1; i < (list -> size) - 1; i++){
        tmp = tmp -> next;
    }
    free(tmp -> next);
    tmp -> next = NULL;
    (list -> size)--;
    return;
}

// free the allocated memory of a singly linked list
void freelist(singlylinkedlist *list){
    if (list == NULL){
        return;
    }
    node *tmp = list -> first;
    free(list);
    while (tmp != NULL){
        node* cur = tmp;
        tmp = tmp -> next;
        free(cur);
    }
}

// print the value of all nodes
void printlist(singlylinkedlist *list){
    if (list == NULL){
        return;
    }
    printf("[");
    node *tmp = list -> first;
    while (tmp -> next != NULL){
        printf("%d, ", tmp -> val);
        tmp = tmp -> next;
    }
    printf("%d]\n", tmp -> val);
}

// create a new node
node* _cnode(int n, node *nextnode){
    node *tmp = malloc(sizeof(node));
    if (tmp == NULL){
        return NULL;
    }
    tmp -> next = nextnode;
    tmp -> val = n;
    return tmp;
}

// create a new node
node* _last(singlylinkedlist *list){
    if (list == NULL || list -> size == 0){
        return NULL;
    }
    node *last = list -> first;
    for (int i = 1; i < list -> size; i++){
        last = last -> next;
    }
    return last;
}