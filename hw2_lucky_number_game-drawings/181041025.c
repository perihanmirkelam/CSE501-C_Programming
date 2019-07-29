#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define VALID 1

#define PLAY_LUCKY_NUMBER
#define DRAW_HOURGLASS
#define DRAW_MOUNTAIN_ROAD
#define EXIT

#define MIN 0
#define MAX 1024

#define LEFT 0
#define RIGHT 1


void menu();
int getChoice();
void playLuckyNumber();
int make_a_guess (int, int, int);
void show_scores(int, int);
void printDistance(int, int);
void draw_hourglass (int);
int getHeight();
void draw_mountain_road (int, int);
int getLength();
int getMaxRadius();
void drawSpaces(int);
void drawRight(int);
void drawStraight();
void drawLeft(int);

int score_human = 0, score_program = 0;
int isFirst = 1;
int mid;

int main ()
{
  srand(time(NULL));
  menu();
  return 0;
}

void menu (){
  isFirst=1;
  switch (getChoice()){
    case 1:
      playLuckyNumber();
      break;
    case 2:
      draw_hourglass(getHeight());
      break;
    case 3:
      printf("\n***PLAY DRAW MOUNTAIN ROAD GAME***\n");
      draw_mountain_road(getLength(), getMaxRadius());
      break;
    case 4:
      break;
    default:
      getChoice();
      break;
}
}

int getChoice(){
  int choice;
  do {
    if(!isFirst) printf("This is an invalid choice. Try again!\n\n");
    isFirst = 0;
    printf("***** MENU *****\n1. Play Lucky Number\n2. Draw Hourglass\n3. Draw Mountain Road\n4. Exit\nChoice: ");
    scanf("%d", &choice);
  }
  while(choice < 1 || choice > 4);
  return choice;
}

void playLuckyNumber(){
  printf("\n***PLAY LUCKY NUMBER GAME***\n");
  int trial;
  int inf = MIN;
  int sup = MAX;
  int guess;
  int luckyNumber = rand() % MAX + 1;
  //printf("luckyNumber: %d\n", luckyNumber);
  printf("Enter number of trial: ");
  scanf("%d", &trial);
  while(trial > 0){
    guess = make_a_guess(trial, inf, sup);
    printDistance(guess, luckyNumber);
    if(guess < luckyNumber) inf = guess;
    else if(guess > luckyNumber) sup = guess;
    trial--;
  }
  if (guess != luckyNumber) score_program++;
  printf("***LUCKY NUMBER: %d***\n", luckyNumber);
  show_scores(score_human, score_program);
}

int make_a_guess (int trial, int inf, int sup){
  int guess;
  do {
    printf("\n(Trial: %d) Make a guess between %d and %d: ", trial, inf, sup);
    scanf("%d", &guess);
  }
  while(guess < inf || guess > sup);
  return guess;
}

void show_scores(int score_human, int score_program){
printf("\nHUMAN: %d - PROGRAM: %d\n\n", score_human, score_program);
menu();
}

void printDistance(int guess, int luckyNumber){
  int distance = abs(luckyNumber - guess);
  if(distance >= 512 && distance <= 1024){
    printf("Distance: 10 \n");
  } else if(distance >= 256 && distance <= 511){
    printf("Distance: 9 \n");
  } else if(distance >= 128 && distance <= 255){
   printf("Distance: 8 \n");
 } else if(distance >= 64 && distance <= 127){
    printf("Distance: 7 \n");
  } else if(distance >= 32 && distance <= 63){
    printf("Distance: 6 \n");
  } else if(distance >= 16 && distance <= 31){
    printf("Distance: 5 \n");
  } else if(distance >= 8 && distance <= 15){
    printf("Distance: 4 \n");
  } else if(distance >= 4 && distance <= 7){
    printf("Distance: 3 \n");
  } else if(distance >= 2 && distance <= 3){
    printf("Distance: 2 \n");
  } else if(distance == 1){
    printf("Distance: 1 \n");
  } else if(distance == 0){
    printf("Distance: 0. You win!\n");
    score_human++;
  } else {
      printf("Unexpected values. Guess: %d, LuckyNumber: %d", guess, luckyNumber);
  }
}

int getHeight(){
  printf("\n***PLAY DRAW HOURGLASS***\n");
  int height;
  do {
    printf("Enter an odd number as height of hourglass: ");
    scanf("%d", &height);
  }
  while(height % 2 == 0);
  return height;
}

void draw_hourglass (int height){
  int mid = height/2;
  int spaces;
  for(int i = 0; i<height; i++){
    if(i<mid) spaces = i;
    else spaces = height-i-1;
    printf("\n");
    for(int j = 0; j<spaces; j++){
      printf(" ");
    }
    for(int j = 0; j< height-(2*spaces); j++){
      printf("*");
    }
    for(int j = 0; j< spaces; j++){
      printf(" ");
    }
  }
  printf("\n***HERE'S YOUR HOURGLASS***\n\n");
  menu();
}

void draw_mountain_road(int length, int max_radius){
  int radius [length];

  mid = max_radius*length*2;
  for(int i=0;i<length;i++){
    radius[i] = rand()%max_radius + 1;
  }

  int direction = 0, temp = 2;
  for(int i = 0; i<length; i++){
    direction==temp%2;
    temp++;
    if(direction == LEFT){
      drawLeft(radius[i]);
      drawStraight();
      drawRight(radius[i]);
    } else if(direction == RIGHT){
      drawRight(radius[i]);
      drawStraight();
      drawLeft(radius[i]);
    }
}
printf("\n***HERE'S YOUR MOUNTAIN***\n\n");
menu();
}


void drawRight(int rad){
  for(int j = 0; j<rad; j++){
    mid++;
    drawSpaces(mid);
    printf("\\");
    drawSpaces(mid);
    printf("\n");
  }
}

void drawLeft(int rad){
  for(int j = 0; j<rad; j++){
    drawSpaces(mid);
    printf("/");
    mid--;
    drawSpaces(mid);
    printf("\n");
  }
}

void drawStraight(){
  drawSpaces(mid);
  printf("|");
  drawSpaces(mid);
  printf("\n");
}

void drawSpaces(int spaces){
  for(int i = 0; i<spaces; i++){
    printf(" ");
  }
}

int getLength(){
  int length;
  do {
    printf("Enter length: ");
    scanf("%d", &length);
  }
  while(length < 0);
  return length;
}

int getMaxRadius(){
  int maxRadius;
  do {
    printf("Enter max radius: ");
    scanf("%d", &maxRadius);
  }
  while(maxRadius < 0);
  return maxRadius;
}
