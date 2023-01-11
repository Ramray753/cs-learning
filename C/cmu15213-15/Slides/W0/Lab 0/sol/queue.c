/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL){
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL){
        return;
    }
    list_ele_t *tmp;
    list_ele_t *head = q -> head;
    while(head != NULL){
        tmp = head;
        head = head -> next;
        free(tmp -> value);
        free(tmp);
    }
    free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    if (q == NULL){
        return false;
    }
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL){
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    newh -> value = malloc(strlen(s) + 1);
    if (newh -> value == NULL){
        free(newh);
        return false;
    }
    strcpy(newh -> value, s);
    /* What if either call to malloc returns NULL? */
    if (q->tail == NULL){
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    q->size++;
    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL){
        return false;
    }
    if (q -> size == 0){
        return queue_insert_head(q, s);
    }
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL){
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    newh -> value = malloc(strlen(s) + 1);
    if (newh -> value == NULL){
        free(newh);
        return false;
    }
    strcpy(newh -> value, s);
    newh->next = NULL;
    q->tail->next = newh;
    q->tail = newh;
    q->size++;
    return false;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */
    if (q == NULL || buf == NULL){
        return false;
    }
    list_ele_t *head = q -> head;
    if (head == NULL || strlen(head -> value) > (bufsize - 1)){
        return false;
    }
    strcpy(buf, q -> head -> value);
    q->head = head->next;
    if(q->size == 1){
        q->tail = NULL;
    }
    free(head->value);
    free(head);
    q->size--;
    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    return q->size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */
    if (q == NULL || q -> head == NULL){
        return;
    }
    list_ele_t *record[q -> size];
    list_ele_t *head = q -> head;
    for (int i = 0; i < q -> size; i++){
        record[i] = head;
        head = head -> next;
    }
    for (int i = q -> size - 1; i >= 1; i--){
        record[i] -> next = record[i - 1];
    }
    q -> head = record[(q -> size) - 1];
    q -> tail = record[0];
    record[0] -> next = NULL;
}

int main(void){
    queue_t *q = queue_new();
    queue_insert_head(q, "Apple");
    queue_insert_head(q, "Pen");
    queue_insert_head(q, "Pineapple");
    queue_insert_tail(q, "Banana");
    queue_insert_tail(q, "Pear");
    queue_reverse(q);
    char buf[64];
    queue_remove_head(q, buf, 64);
    printf("%s\n", buf);
    queue_remove_head(q, buf, 64);
    printf("%s\n", buf);
    queue_remove_head(q, buf, 64);
    printf("%s\n", buf);
    queue_remove_head(q, buf, 64);
    printf("%s\n", buf);
    queue_remove_head(q, buf, 64);
    printf("%s\n", buf);
    queue_insert_head(q, "Pen");
    queue_insert_tail(q, "Pear");
    printf("%ld\n", queue_size(q));
    queue_free(q);
}