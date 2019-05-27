#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 20

typedef struct stack{
int num;
struct stack *next;
} stack;

typedef struct queue{ 
    struct queue_node *front, *rear; 
}queue;

typedef struct queue_node{
int num;
struct queue_node *next;
} queue_node;

typedef struct bst{
int num;
struct bst *right;
struct bst *left;
} bst;

void fill_structures(stack **, queue **, bst **, int[20]);
void ordered_print(stack *, queue *, bst *);
void search(stack *, queue *, bst *, int);
void special_traverse(stack *, queue *, bst *);

void fill_stack(stack **, int[20]);
void printStack(stack **);
void fill_queue(queue **, int[20]);
void print_queue(queue **);
void fill_bst(bst **, int[20]);
void print_bst(bst **);
void add_num_to_bst(bst **, bst *);

void ordered_bst_print(bst **);
void bst_print(bst *);

//Part-2
void ordered_print(stack *stack_, queue *queue_, bst *bst_){
    ordered_bst_print(&bst_);
}

void ordered_bst_print(bst **bst_){
      bst *temp;
    for(temp = (*bst_) ; temp != NULL; temp=temp->left){
        printf("%10d --- %p --- ->left: --- %p ->right: --- %p\n", temp->num, temp, temp->left, temp->right);
    }
   /// bst_print(bst_); 
}

void bst_print(bst *node){
     bst *temp = node;
    printf("bst_print %d\n", node->num);
    
    if (node == NULL){
        printf("NODE is null return.");
        return; 
    }
     bst_print(temp->left); 
     bst_print(temp->right);
 
}



// Part-1
void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[20]){
    double stack_time, queue_time, bst_time;
    clock_t t; 

    t = clock(); 
    fill_stack(stack_, data);
    t = clock() - t; 
    stack_time = ((double)t*1000)/CLOCKS_PER_SEC;
    printf("fill_stack took %f seconds to execute \n\n", stack_time);

    t = clock(); 
    fill_queue(queue_, data);
     t = clock() - t; 
    queue_time = ((double)t*1000)/CLOCKS_PER_SEC; 
    printf("fill_queue took %f seconds to execute \n\n", queue_time);

    t = clock(); 
    fill_bst(bst_, data);
    t = clock() - t; 
    bst_time = ((double)t*1000)/CLOCKS_PER_SEC;
    printf("fill_bst took %f seconds to execute \n\n", bst_time);
    
    printf("-------------------------------------------\n");
    printf("Part-1 Fill Structers:\n");
    printf("%10s %10s %10s %10s\n", "Structures", "Stack", "Queue", "Bst");
    printf("%10s %10.2f %10.2f %10.2f\n", "Exec. Time", stack_time, queue_time, bst_time);
    printf("-------------------------------------------\n");

}

void fill_bst(bst ** bst_, int data[20]){
    int i=0;
    bst nodes[SIZE];
    for (i=0; i<20; i++) {
        nodes[i].num = data[i];
        nodes[i].right = NULL;
        nodes[i].left = NULL;
        add_num_to_bst(bst_, &nodes[i]);
    }
}

void add_num_to_bst(bst ** bst_, bst *bst_node){
    bst *temp = *bst_;
    if(*bst_ == NULL){
        printf("ADD ROOT: %d\n", bst_node->num);
        *bst_ = bst_node;
    } else if (temp->num < bst_node->num) {
        if (temp->right == NULL) {
            printf("ADD RIGHT: %d of %d\n", bst_node->num, temp->num);
            temp->right = bst_node;
        }  
        else add_num_to_bst(&(temp->right), bst_node);
    } else if (temp->num > bst_node->num) {
        if(temp->left == NULL){
            printf("ADD LEFT: %d of %d\n", bst_node->num, temp->num);
            temp->left = bst_node;
        } 
        else add_num_to_bst(&(temp->left), bst_node);
    }
}

void fill_queue(queue ** queue_, int data[20]){
    int i;
    queue q;
    queue_node nodes[SIZE]; 

     nodes[0].num = data[0]; 
     nodes[0].next = NULL; 
     q.rear = &nodes[0];
     q.front = &nodes[0];
     *queue_ = &q;
  
    for (i=1; i<20; i++) {
        nodes[i].num = data[i];
        (*queue_)->rear->next = &nodes[i];
        (*queue_)->rear = &nodes[i];
    }
    q.rear->next = NULL;

   print_queue(queue_);
} 

void print_queue(queue ** q){
    queue_node *temp = (*q)->front;
    for( ; temp != NULL; temp=temp->next){
        printf("%3d --- %p --- ->next: --- %p\n", temp->num, temp, temp->next);
    }
}

void fill_stack(stack ** top, int data[20]){
    int i=0;
    stack nodes[SIZE];
   // stack *temp = *top;
    
    for (i=0; i<20; i++) {
        nodes[i].num = data[i];
        if (*top == NULL) {
            nodes[i].next = NULL;
            *top = &nodes[i];
        } else {
            nodes[i].next = *top;
            *top = &nodes[i];
        }
    }
    printStack(top);
}

void printStack(stack ** top){
    stack *temp = *top;
    for(temp = *top; temp != NULL; temp=temp->next){
        printf("%3d --- %p --- ->next: --- %p\n", temp->num, temp, temp->next);
    }
}

int main(){
int data[20]={5, 2, 7, 8, 11, 3, 21, 7, 6, 15, 19, 35, 24, 1, 8, 12, 17, 8, 23, 4};
bst *bst_ = NULL;
queue *queue_ = NULL;
stack *stack_ = NULL;
fill_structures(&stack_, &queue_, &bst_, data);
ordered_print(stack_, queue_, bst_);
// search(stack_, queue_, bst_, 5);
// special_traverse(stack_, queue_, bst_);
return 0;
}