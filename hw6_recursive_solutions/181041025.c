#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int common(int, int);
void part1();
int getSize();
void part2();
void compound(int[], int, int, int);
void sort(int[], int, int);
void printArray(int[], int);
void part3();
void collatz(int);
void part4();
int getDigit(int);
void check(int, int, int, int);
void part5();
void printCapital(char*);


int main(){
  part1();
  part2();
  part3();
  part4();
  part5();
}

void part1(){
  int n1, n2, state;
  printf("Enter two number to find the gcd:\n");
  state = scanf("%d %d", &n1, &n2);
  if(state == 2){
    printf("GCD: %d\n",common(n1, n2));
  } else {
    printf("Invalid input! Try again.");
  }
  fflush(stdout);
}

int common(int num1, int num2){
  if(num2 == 0){
    return num1;
  }else{
    common(num2, num1%num2);
  }
}

void part2(){
  int i, state, size = getSize();
  int list[size];
  for(i=0; i < size; i++){
    scanf("%d", &list[i]);
  }
   printf("\nSorted array is: ");
   sort(list, 0, size - 1);
   printArray(list, size);
   fflush(stdout);
}

int getSize(){
  int size, state;
  printf("\nEnter the length of the list:");
  state = scanf("%d", &size);
  if(state == 1){
    return size;
  } else {
    return getSize();
  }
}

void compound(int arr[], int left, int mid, int right){
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 =  right - mid;
    int leftArr[n1], rightArr[n2];
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1+ j];
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2){
        if (leftArr[i] <= rightArr[j]) arr[k] = leftArr[i++];
        else arr[k] = rightArr[j++];
        k++;
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

void sort(int arr[], int left, int right){
    if (left < right){
        int m = left+(right-left)/2;
        sort(arr, left, m);
        sort(arr, m+1, right);
        compound(arr, left, m, right);
    }
}

void printArray(int A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

void part3(){
  int num, state;
  printf("\nEnter a number to print Collatz series:");
  state = scanf("%d", &num);
  if(state == 1){
    collatz(num);
  } else {
    printf("Invalid input! Try again.");
    fflush(stdout);
    main();
  }
}

void collatz(int num){
  printf(" %d", num);
  if(num > 1){
    if(num % 2 == 0){
      num /= 2;
    } else {
      num = (num*3) + 1;
    }
    collatz(num);
  } else {
    fflush(stdout);
  }
}

void part4(){
  int num, state, digit;
  printf("\nEnter a number to check if narcissistic number:");
  state = scanf("%d", &num);
  if(state == 1){
    digit = getDigit(num);
    check(num, 0, digit, digit);
  } else {
    printf("Invalid input! Try again.");
    fflush(stdout);
    part4();
  }
}

void check(int n, int sub, int digitNumber, int currentDigit){
  int divisor = 1, i, j, temp, item = 1;
  if(currentDigit>0){
    for(i=0;i<currentDigit-1; i++) divisor *= 10;
    temp = n/divisor;
    temp = temp %10;
    for(j=0;j<digitNumber;j++) item *= temp;
    check(n, sub + item, digitNumber, --currentDigit);
  } else {
    if(n == sub) printf("\nEqual");
    else printf("\nNot Equal");
  }
}

int getDigit(int n){
  if(n>0 && n< 9999){
    if(n/10000 == 0 && n/1000 != 0){
      return 4;
    } else if(n/1000 == 0 && n/100 != 0){
      return 3;
    } else if(n/100 == 0 && n/10 != 0){
      return 2;
    } else {
      return 1;
    }
  } else {
    printf("Invalid input. Valid interval is 0<x<10000. Try again.");
    fflush(stdout);
    part4();
  }
}

void part5(){
  int state;
  char str[40];
  printf("\nEnter a string to find first capital letter of it: ");
  state = scanf("%s", str);
  if(state == 1){
    printCapital(str);
  } else {
    printf("\nInvalid input! Try again.");
    fflush(stdout);
    part5();
  }
}

void printCapital(char *str){
  if(str[0] == '\0') {
    printf("\nNo capital letter!");
    return;
  }
  if(isupper(str[0])){
      printf("\n%c", str[0]);
      fflush(stdout);
  } else {
    printCapital(&str[1]);
  }
}
