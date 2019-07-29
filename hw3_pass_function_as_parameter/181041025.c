#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int isFirst = 1;

void menu();
int getChoice();
void calculator(int);
void weightedAverage();
void drawTetragon();

void doit(int (*f)(int, int), int, int);

int divide(int, int);
int add(int, int);
int sub(int, int);
int mult(int, int);
int power(int, int);
int mod(int, int);

int take_grades(int [] );
int take_exam_grades(int[]);
double calculate_homework(int[]);
double calculate_lab(int[]);
double calculate_all(int ,int[],int);

void main ()
{
  menu();
}

void menu (){
  isFirst=1;
  switch (getChoice()){
    case 1:
      fflush(stdin);
      calculator(0);
      break;
    case 2:
      fflush(stdin);
      weightedAverage();
      break;
    case 3:
     fflush(stdin);
     drawTetragon();
     break;
    case 4:
     fflush(stdin);
      break;
    default:
      fflush(stdin);
      getChoice();
      break;
}
}

int getChoice(){
  int choice;
  do {
    if(!isFirst) printf("This is an invalid choice. Try again!\n\n");
    isFirst = 0;
    printf("\n***** MENU *****\n1. Calculator\n2. Calculate Weighted Average\n3. Draw Tetragon\n4. Exit\nChoice: ");
    scanf("%d", &choice);
  }
  while(choice < 1 || choice > 4);
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { }
  return choice;
}

void calculator(int result){
  char operator, operators[2], temp, temp1;
  int i=0, k=0,size,operand[2];
  int c;

  printf("\nWrite what you want to calculate ( write: + 3 4 -> output: 7)\n");

  operators[1] = ' ';
  while ((c = getchar()) != ' ' && k<2) {
    operators[k] = c;
    k++;
  }

  if(operators[1] == '*'){
    operator = '&';
  } else {
    operator = operators[0];
  }

  do{
    scanf("%d%c", &operand[i], &temp);
    i++;
  } while(temp!= '\n' && i<2);

  if(i == 1) operand[1] = result;

  if(operator == '+'){
    doit(add, operand[0], operand[1]);
    return;
  } else if(operator == '-'){
    doit(sub, operand[0], operand[1]);
    return;
  } else if(operator == '*'){
    doit(mult, operand[0], operand[1]);
  } else if(operator == '/'){
    if(operand[1] == 0){
      printf("Wrong input! Denominator can not be zero! Try again...");
      calculator(result);
      return;
    }
    doit(divide, operand[0], operand[1]);
    return;
  } else if(operator == '&'){ // todo convert to **
    doit(power, operand[0], operand[1]);
    return;
  } else if(operator == '%'){
    if(operand[1] == 0){
      printf("Wrong input! Denominator can not be zero!  Try again...");
      calculator(result);
    }
    doit(mod, operand[0], operand[1]);
    return;
  } else {
    printf("Wrong input as operator! Try again... %c", operator);
    calculator(0);
    return;
  }
}

void doit(int (*f)(int, int), int first_number, int second_number){
  int final = (*f)(first_number, second_number);
  printf("%d\n", final);
  menu();
}

int add(int number1, int number2){
  return number1 + number2;
}

int sub(int number1, int number2){
  return number2 - number1;
}

int mult(int number1, int number2){
  return number1 * number2;
}

int divide(int number1, int number2){
  return number2 / number1;
}

int power(int number1, int number2){
  int product = 1;
  if(number2 == 0) return 1;
  for(int i = 0; i < number2; i++){
    product *= number1;
  }
  return product;
}

int mod(int number1, int number2){
  return number2 % number1;
}

void weightedAverage(){
  int arrayHw[10], arrayLab[10], arrayExams[2]; //hw, lab, final

  printf("\nEnter 10 homework grades:");
  take_grades(arrayHw);

  printf("\nEnter 10 lab grades:");
  take_grades(arrayLab);

  printf("\nEnter midterm and final grades:");
  take_exam_grades(arrayExams);

  printf("\nWeighted Average: %f\n\n",calculate_all(calculate_lab(arrayLab) , arrayExams, calculate_homework(arrayHw)));
  menu();
  }

int take_grades(int array[10]){
  char temp = ' ';
  int i = 0;
  do{
    scanf("%d%c", &array[i], &temp);
    i++;
  } while(temp!= '\n' && i<10);
  return *array;
}

 int take_exam_grades(int array[2]){
   char temp = ' ';
   int k = 0;
   do{
     scanf("%d%c", &array[k], &temp);
     k++;
   } while(temp!= '\n' && k<2);
   return *array;
 }

double calculate_homework(int array[10]){
  double avHw = 0.0;
  for(int m=0; m< 10; m++){
    avHw += array[m];
  }
  return avHw /= 10;
}

double calculate_lab(int array[10]){
    double avLab = 0.0;
    for(int m=0; m< 10; m++){
      avLab += array[m];
    }
    return avLab /= 10;
  }

 double calculate_all(int avLab ,int exams[2], int avHw){
   return (avHw * 0.1) + (avLab * 0.2) + (exams[0] * 0.3) + (exams[1] * 0.4);
 }


 void drawTetragon(){
   int h = 0;
   printf("\nEnter heihgt:");
   scanf("%d", &h);
   char array[h][h];
   int mid = h/2 +1;
   for(int i=0; i < h; i++){
     for(int j=0; j < h; j++){
       if(i+j < mid){
         array[i][j]=' ';
       } else if(i+j == mid){
         array[i][j]='/';
       } else if(i+j > mid){
         array[i][j]='*';
       }
       printf("%c", array[i][j]);
     }
     for(int j=0; j < h; j++){
       if(i<j){
         array[i][j]=' ';
       } else if(i==j){
         array[i][j]='\\';
       } else if(i > j){
         array[i][j]='*';
       }
       printf("%c", array[i][j]);
     }
     printf("\n");
   }

   for(int i=0; i < h; i++){
     for(int j=0; j < h; j++){
       if(i<j){
         array[i][j]='*';
       } else if(i == j){
         array[i][j]='\\';
       } else if(i>j){
         array[i][j]=' ';
       }
       printf("%c", array[i][j]);
     }
     for(int j=0; j < h; j++){
       if(i+j < mid){
         array[i][j]='*';
       } else if(i+j == mid){
         array[i][j]='/';
       } else if(i+j > mid){
         array[i][j]=' ';
       }
       printf("%c", array[i][j]);
     }
     printf("\n");
   }
menu();

}
