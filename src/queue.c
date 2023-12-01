#include "utils.h"
#include <stdio.h>



QueueNode* create_queue_node(Node* tree_value) {
    QueueNode* new_node = malloc(sizeof(QueueNode));

    new_node->next = NULL;
    new_node->value = tree_value;

    return new_node;
}


Queue* create_queue() {
    Queue* new_queue = malloc(sizeof(Queue));

    new_queue->front = NULL;
    new_queue->rear = NULL;


    return new_queue;
}

void enqueue(Queue* q, Node* tree_value) {

    if (q == NULL) {
        printf("queue address is NULL");
        return;
    }

    QueueNode* node_to_add = malloc(sizeof(QueueNode));

    node_to_add->value = tree_value;
    node_to_add->next = NULL;

    // facem next la rear sa fie new node si dupa setam rear sa fie new node
    if (q->rear == NULL && q->front == NULL) {
        
        q->rear = node_to_add;
        q->front = node_to_add;
    }
    q->rear->next = node_to_add;
    q->rear = node_to_add;
}
 
QueueNode* dequeue(Queue* q) {

    if (q == NULL) {
        printf("queue address is NULL");
        return NULL;
    }
    if (q->front == NULL) {
        printf("Queue is empty");
        return NULL;
    }
    
    QueueNode* dequeued_node;
    dequeued_node = q->front;

    if (q->front == q->rear) {
        q->front = q->rear = NULL;
    } else {
        q->front = q->front->next;
    }

    return dequeued_node;
}

int is_queue_empty(Queue* q) {
    if (q == NULL) {
        printf("queue address is NULL");
        return 1;
    }
    if (q->front == NULL) {
        return 1; 
    } else {
        return 0;
    }
    
}











