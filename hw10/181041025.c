#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 20

typedef struct stack_s
{
    int num;
    struct stack_s *next;
} stack;

typedef struct queue_s
{
    struct queue_node_s *front, *rear;
} queue;

typedef struct queue_node_s
{
    int num;
    struct queue_node_s *next;
} queue_node;

typedef struct bst_s
{
    int num;
    struct bst_s *right;
    struct bst_s *left;
} bst;

stack nodes_stack[SIZE];
bst nodes_bst[SIZE];
queue q;
queue_node nodes_queue[SIZE];

void fill_structures(stack **, queue **, bst **, int[20]);
void ordered_print(stack *, queue *, bst *);
void search(stack *, queue *, bst *, int);
void special_traverse(stack *, queue *, bst *);

void fill_stack(stack **, int[20]);
void print_stack(stack *);
void fill_queue(queue **, int[20]);
void print_queue(queue *);
void fill_bst(bst **, int[20]);
void add_num_to_bst(bst **, bst *);

void ordered_bst_print(bst *);
void bst_print(bst *);
void ordered_stack_print(stack *stack_);
void ordered_queue_print(queue *q);

void search_in_stack(stack *, int);
void search_in_queue(queue *, int);
void search_in_bst(bst*, int);
int search_bst(bst *, int, int*);


//Part-3
void search(stack * stack_, queue *queue_, bst *bst_, int val_to_search)
{
    double stack_time, queue_time, bst_time;
    clock_t t;

    printf("\n\nPART-3: \n");

    t = clock();
    search_in_stack(stack_, val_to_search);    t = clock() - t;
    stack_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    t = clock();
    search_in_queue(queue_, val_to_search);
    t = clock() - t;
    queue_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    t = clock();
    search_in_bst(bst_, val_to_search);
    t = clock() - t;
    bst_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    printf("\n-------------------------------------------\n");
    printf("Part-2 Print Structs in Descending Order:\n");
    printf("%10s %10s %10s %10s\n", "Structures", "Stack", "Queue", "Bst");
    printf("%10s %10.2f %10.2f %10.2f\n", "Exec. Time", stack_time, queue_time, bst_time);
    printf("-------------------------------------------\n");
}

void search_in_bst(bst *bst_, int val){
    int step = 0;
    int count = search_bst(bst_, val, &step);
    printf("\n%d result founded on %2dth step on queue for value %d. \n", count, step, val);
}

int search_bst(bst *bst_, int val, int *step){
    if(bst_ == NULL){
        return 0;
    } 
    (*step)++;
    if(val == bst_->num) return 1 + search_bst(bst_->right, val, step); 
    else if(val > bst_->num) return search_bst(bst_->right, val, step);
    else if(val < bst_->num) return search_bst(bst_->left, val, step);
    
}

void search_in_queue(queue *q, int val){
    queue_node *temp = q->front;
    int count = 0, step = 0, totalStep = 0;
    while(temp != NULL){
        totalStep ++;
        if(temp->num == val){
            count++;
            step = totalStep;
        }
        temp = temp->next;
    }
    printf("\n%d result founded on %2dth step on queue for value %d. \n", count, step, val);
}

void search_in_stack(stack * stack_, int val)
{
    stack *temp = stack_;
    int count = 0, step = 0, totalStep = 0;
    while(temp != NULL){
        totalStep ++;
        if(temp->num == val){
            count++;
            step = totalStep;
        }
        temp = temp->next;
    }
    printf("\n%d result founded on %2dth step on stack for value %d. \n", count, step, val);
}

//Part-2
void ordered_print(stack *stack_, queue *queue_, bst *bst_)
{
    double stack_time, queue_time, bst_time;
    clock_t t;

    printf("\n\nPART-2:\n");

    t = clock();
    ordered_queue_print(queue_);
    t = clock() - t;
    queue_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    t = clock();
    ordered_stack_print(stack_);
    t = clock() - t;
    stack_time = ((double)t * 1000) / CLOCKS_PER_SEC;

      t = clock();
    ordered_bst_print(bst_);
    t = clock() - t;
    bst_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    printf("\n-------------------------------------------\n");
    printf("Part-2 Print Structs in Descending Order:\n");
    printf("%10s %10s %10s %10s\n", "Structures", "Stack", "Queue", "Bst");
    printf("%10s %10.2f %10.2f %10.2f\n", "Exec. Time", stack_time, queue_time, bst_time);
    printf("-------------------------------------------\n");
}

void ordered_queue_print(queue *q)
{
    queue_node *temp = q->front;
    int max_int = 1000, min_int = -1000, count = 1, k;
    int sup = min_int, i, prev = max_int; // set max int min int
    printf("\nQUEUE NODES IN DESCENDING ORDER:\n");

    for (i = 0; i < 20 && temp != NULL; temp = q->front)
    {
        while (temp != NULL)
        {
            if (sup < (temp->num) && prev > (temp->num))
                sup = temp->num;
            else if (prev == temp->num)
                count++;
            temp = temp->next;
        }
        for (k = 0; k < count; k++)
            if (prev != max_int)
                printf("%d->", prev);
        prev = sup;
        sup = min_int;
        count = 0;
        i++;
    }

}

void ordered_stack_print(stack *top)
{
    stack *temp = top;
    int max_int = 1000, min_int = -1000, count = 1, k;
    int sup = min_int, i, prev = max_int; // set max int min int
    printf("\n\nSTACK NODES IN DESCENDING ORDER:\n");

    for (i = 0; i < 20 && temp != NULL; temp = top)
    {
        while (temp != NULL)
        {
            if (sup < (temp->num) && prev > (temp->num))
                sup = temp->num;
            else if (prev == temp->num)
                count++;
            temp = temp->next;
        }
        for (k = 0; k < count; k++)
            if (prev != max_int)
                printf("%d->", prev);
        prev = sup;
        sup = min_int;
        count = 0;
        i++;
    }
}

void ordered_bst_print(bst *bst_)
{
    printf("\n\nBST NODES IN DESCENDING ORDER:\n");
    bst_print(bst_);
    printf("\n");
}

void bst_print(bst *node)
{
    bst *temp = node;
    if (node == NULL)
    {
        return;
    }
    bst_print(temp->right);
    printf("%d->", temp->num);
    bst_print(temp->left);
}

// Part-1
void fill_structures(stack **stack_, queue **queue_, bst **bst_, int data[20])
{
    double stack_time, queue_time, bst_time;
    clock_t t;

    printf("\nPart-1:\n\n");

    t = clock();
    fill_stack(stack_, data);
    t = clock() - t;
    stack_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    t = clock();
    fill_queue(queue_, data);
    t = clock() - t;
    queue_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    t = clock();
    fill_bst(bst_, data);
    t = clock() - t;
    bst_time = ((double)t * 1000) / CLOCKS_PER_SEC;

    printf("-------------------------------------------\n");
    printf("Part-1 Fill Structers:\n");
    printf("%10s %10s %10s %10s\n", "Structures", "Stack", "Queue", "Bst");
    printf("%10s %10.2f %10.2f %10.2f\n", "Exec. Time", stack_time, queue_time, bst_time);
    printf("-------------------------------------------\n");
}

void fill_bst(bst **bst_, int data[20])
{
    int i = 0;

    for (i = 0; i < 20; i++)
    {
        nodes_bst[i].num = data[i];
        nodes_bst[i].right = NULL;
        nodes_bst[i].left = NULL;
        add_num_to_bst(bst_, &nodes_bst[i]);
    }
}

void add_num_to_bst(bst **bst_, bst *bst_node)
{
    bst *temp = *bst_;
    if (*bst_ == NULL)
    {
        //printf("ADD ROOT: %d\n", bst_node->num);
        *bst_ = bst_node;
    }
    else if (temp->num <= bst_node->num)
    {
        if (temp->right == NULL)
        {
            //printf("ADD RIGHT: %d of %d\n", bst_node->num, temp->num);
            temp->right = bst_node;
        }
        else
            add_num_to_bst(&(temp->right), bst_node);
    }
    else if (temp->num > bst_node->num)
    {
        if (temp->left == NULL)
        {
            //printf("ADD LEFT: %d of %d\n", bst_node->num, temp->num);
            temp->left = bst_node;
        }
        else
            add_num_to_bst(&(temp->left), bst_node);
    }
}

void fill_queue(queue **queue_, int data[20])
{
    int i;

    nodes_queue[0].num = data[0];
    nodes_queue[0].next = NULL;
    q.rear = &nodes_queue[0];
    q.front = &nodes_queue[0];
    *queue_ = &q;

    for (i = 1; i < 20; i++)
    {
        nodes_queue[i].num = data[i];
        (*queue_)->rear->next = &nodes_queue[i];
        (*queue_)->rear = &nodes_queue[i];
    }
    q.rear->next = NULL;

    //print_queue(*queue_);
}

void print_queue(queue *q)
{
    queue_node *temp = q->front;
    for (; temp != NULL; temp = temp->next)
    {
        printf("%3d --- %p --- ->next: --- %p\n", temp->num, temp, temp->next);
    }
}

void fill_stack(stack **top, int data[20])
{
    int i;
    stack *temp = *top;
    for (i = 0; i < 20; i++)
    {
        nodes_stack[i].num = data[i];
        if (temp == NULL)
            nodes_stack[i].next = NULL;
        else
            nodes_stack[i].next = temp;
        temp = &nodes_stack[i];
    }
    *top = temp;
    //print_stack(*top);
}

void print_stack(stack *top)
{
    stack *temp;
    for (temp = top; temp != NULL; temp = temp->next)
    {
        printf("%3d --- %p --- ->next: --- %p\n", temp->num, temp, temp->next);
    }
}

int main()
{
    int data[20] = {5, 2, 7, 8, 11, 3, 21, 7, 6, 15, 19, 35, 24, 1, 8, 12, 17, 8, 23, 4};
    bst *bst_ = NULL;
    queue *queue_ = NULL;
    stack *stack_ = NULL;
    fill_structures(&stack_, &queue_, &bst_, data);
    ordered_print(stack_, queue_, bst_);
    search(stack_, queue_, bst_, 5);
    //special_traverse(stack_, queue_, bst_);
    return 0;
}