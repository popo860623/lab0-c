#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2);
list_ele_t *merge_sort(list_ele_t *head);
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *ptr;
    while (q->head != NULL) {
        ptr = q->head;
        q->head = ptr->next;
        if (ptr->value)
            free(ptr->value);
        free(ptr);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *new_node = malloc(sizeof(list_ele_t));
    if (!new_node) {
        free(new_node);
        return false;
    }
    new_node->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!new_node->value) {
        free(new_node);
        return false;
    }
    strncpy(new_node->value, s, strlen(s) + 1);
    if (!q->head) {
        q->tail = new_node;
        q->tail->next = NULL;
    }
    new_node->next = q->head;
    q->head = new_node;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *new_node = malloc(sizeof(list_ele_t));
    if (!new_node)
        return false;
    new_node->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    strncpy(new_node->value, s, strlen(s) + 1);
    if (q->head == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    q->tail->next = NULL;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    list_ele_t *ptr = q->head;
    if (sp) {
        strncpy(sp, ptr->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    q->head = ptr->next;

    if (ptr->value)
        free(ptr->value);

    free(ptr);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (!q) ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;
    if (!q->head->next)
        return;
    list_ele_t *current, *prev, *next;
    prev = NULL;
    current = q->head;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    q->tail = q->head;
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge_sort(list_ele_t *head)
{
    /* merge sort */
    if (!head || !head->next)
        return head;
    list_ele_t *slow = head;
    list_ele_t *fast = head->next;
    /* split list */
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    /* sort each list */
    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);
    /* merge sorted l1 and sorted l2 */
    return merge(l1, l2);
}



list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l1)
        return l2;
    if (!l2)
        return l1;

    list_ele_t *head = NULL; /* pseudo head */
    list_ele_t *tmp = NULL;
    /* decide the first element and use it as pseudo head */
    if (strcasecmp(l1->value, l2->value) < 0) {
        head = l1;
        l1 = l1->next;
    } else {
        head = l2;
        l2 = l2->next;
    }
    /* merge remaining elements to pseudo head */
    tmp = head;
    while (l1 && l2) {
        if (strcasecmp(l1->value, l2->value) < 0) {
            tmp->next = l1;
            l1 = l1->next;

        } else {
            tmp->next = l2;
            l2 = l2->next;
        }
        tmp = tmp->next;
    }
    if (l1)
        tmp->next = l1;
    if (l2)
        tmp->next = l2;
    return head;
}



void q_sort(queue_t *q)
{
    if (!q || !q->head) /* ignore NULL and empty queue */
        return;
    if (!q->head->next)
        return;
    q->head = merge_sort(q->head);
    while (q->tail->next) { /* update the tail pointer */
        q->tail = q->tail->next;
    }
}
