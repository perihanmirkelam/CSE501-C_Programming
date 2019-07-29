#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PI 3

#define RECTENGULAR 1
#define CIRCULAR 2
#define SQUARE 3

#define RED 0
#define YELLOW 1
#define BLUE 2
#define BLACK 3
#define WHITE 4

double CreateBody(int);
int SetColor(int);
double LoadMoves(int, double);
int SetAttackPower(int, int);
void ShowPokemon(int, double, int, double, int);

int main ()
{
  int shape, color, attack_power;
  double size_of_body, move_length;
  shape = CIRCULAR;
  size_of_body = CreateBody(shape);
  color = 798;
  color = SetColor(color);
  move_length = LoadMoves(shape, size_of_body);
  attack_power = SetAttackPower (0, 150);
  ShowPokemon(shape, size_of_body, color, move_length, attack_power);
return 0;
}

double CreateBody (int shape){
  double area;
  if(shape == RECTENGULAR){
    int width, height;
    printf("Enter width: ");
    scanf("%d", &width);
    printf("Enter height: ");
    scanf("%d", &height);
    area = width*height;
  } else if (shape == CIRCULAR){
    int radius;
    printf("Enter radius: ");
    scanf("%d", &radius);
    area = PI*radius*radius;
  } else if (shape == SQUARE){
    int edge;
    printf("Enter edge length: ");
    scanf("%d", &edge);
    area = edge*edge;
  }
  return area;
}

int SetColor(int color){
int revised = 1;
if(color > 0 && color < 1000){
  revised = color%5;
}
  return revised;
}

double LoadMoves(int shape, double body_size){
  double perimeter = 0.0;
  if(shape == RECTENGULAR){
    perimeter = 2 * ((body_size / 5) + 5);
  } else if (shape == CIRCULAR){
    perimeter = 2 * PI * sqrt(body_size / PI);
  } else if (shape == SQUARE){
    perimeter = 2 * sqrt(body_size);
  }
  return perimeter;
}

int SetAttackPower(int lower_bound, int upper_bound) {
  int num = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
  return num;
}

void ShowPokemon(int shape, double body_size, int color,
  double move_length, int attack_power){
    char name[15], colorName[10];
    if(shape == RECTENGULAR){
      int width = body_size / 5;
      for(int i = 0; i < width; i++){
        for(int j = 0; j < 5; j++){
          printf("X");
        }
        printf("\n");
      }
      printf("Name: Rectengular Pokémon\n");

    } else if (shape == CIRCULAR){
      int radius = sqrt(body_size / PI);
      int axis;
      if(radius%2==1){
        axis = (radius - 1) / 2;
        for(int i=0; i < radius; i++){
          for(int j=0; j < radius; j++){
            if( i <= axis && (j == (axis - i) || j == (axis + i)) ){
              printf(" X ");
            } else if(i > axis && (j == (i - axis) || j == (3*axis - i))){
              printf(" X ");
            } else {
              printf("   ");
            }
          }
          printf("\n");
        }

      } else {
        axis = radius / 2;
        for(int i=0; i <= radius; i++){
          for(int j=0; j <= radius; j++){
            if( i <= axis && (j == (axis - i) || j == (axis + i)) ){
              printf("  X  ");
            } else if(i > axis && (j == (i - axis) || j == (3*axis - i))){
              printf("  X  ");
            } else {
              printf("      ");
            }
          }
          printf("\n");
        }
      }
      printf("Name: Circular Pokémon\n");

    } else if (shape == SQUARE){
      int edge = sqrt(body_size);
      for(int i = 0; i < edge; i++){
        for(int j = 0; j < edge; j++){
          printf("X");
        }
        printf("\n");
      }
      printf("Name: Square Pokémon\n");
    }

    printf("Size: %.2f\n", body_size);

    if(color == RED)
      printf("Color: Red\n");
    if(color == YELLOW)
      printf("Color: Yellow\n");
    if(color == BLUE)
      printf("Color: Blue\n");
    if(color == BLACK)
      printf("Color: Black\n");
    if(color == WHITE)
      printf("Color: White\n");

    printf("Move: %.2f\n", move_length);
    printf("Attack Power: %d\n", attack_power);
  }
