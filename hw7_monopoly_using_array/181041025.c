#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INT_MIN 214748364
#define MAX_INT -214748364

typedef enum {NOONE, CAP, CAR} player_type;
typedef enum {PROPERTY, TAX, PUNISH, START, NONE} block_type;

struct player {
player_type type;
int current_block_id;
int owned_block_ids[11];
int account;
int turn_to_wait;
char * name;
};

struct block {
  int block_id;
  char * name;
  int price;
  int rent;
  int rent_1;
  int rent_2;
  int rent_3;
  int house_price;
  int house_count;
  struct player owner;
  block_type type;
};

int rolled = 0, location = 0;

struct block board[20];
struct player cap = {CAP, 0, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 100000, 0, "CAP"};
struct player car = {CAR, 0, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 100000, 0, "CAR"};
const struct player noone = {NONE, -1, {}, 0, -1, "No One"};

void init_the_board(struct block[]);
void show_board(struct block[], struct player, struct player);
void drawTop(struct block[], struct player, struct player);
void drawMiddle(struct block[], struct player, struct player);
void drawBottom(struct block[], struct player, struct player);
int getDigitNumber(int);
void drawBlankArea();
void drawLine();
void drawLongLine();
void show_properties(struct block[]);
void drawFieldAndPriceLine(char *, int);
void buy_property(struct block*, struct player*);
void printBlock(struct block*);
void printPlayer(struct player*);
void addBlockToPlayer(struct block*, struct player*);
void sell_property(struct block[], struct player*);
void gameplay (struct block[], struct player, struct player);
int showMenu();
int rollTheDice();
void showMyProperties(struct block[], struct player*);
int getPropertySize(struct block[], struct player*);

int main(){
  srand ( time(NULL) );
  init_the_board(board);
  printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", cap.name);
  gameplay(board, cap, car);
}

//Part 6
void gameplay (struct block board[20], struct player player_one, struct player player_two){
  struct player p1 = player_one, p2 = player_two;
  int rent = 0, giveUp = 0, temp, state;

  if(p1.turn_to_wait == 0){

  switch(showMenu()){
    case 1: // Roll the Dice
      if(!rolled){
        rolled = 1;
        location = (p1.current_block_id + rollTheDice()) % 20;
        if(p1.current_block_id > location) {
          printf("\nPlayer %s pass the start block. Gain 10000&.\n", p1.name);
          p1.account += 10000;
        }
        p1.current_block_id = location;
        show_board(board, p1, p2);
        switch(board[location].type){
          case PROPERTY:
          if(board[location].owner.type == p2.type){
            switch(board[location].house_count) {
              case 0:
                rent = board[location].rent;
                break;
              case 1:
                rent = board[location].rent_1;
                break;
              case 2:
                rent = board[location].rent_2;
                break;
              case 3:
                rent = board[location].rent_3;
                break;
              default:
                printf("\nERROR! Unexpected house count: %d. Exit the game.\n", board[location].house_count);
                break;
            }

            if(p1.account>=rent){
              printf("\nPlayer %s pay the rent (%d$) to player %s.\n", p1.name, rent, p2.name);
              p1.account -= rent;
              printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
              rolled=0;
              gameplay(board, p2, p1);
            } else {
              printf("\nPlayer %s has no enough money for paying the rent: %d\n", p1.name, rent);
              while(p1.account < rent && giveUp == 0){
                printf("\nSell property to pay rent or give up!\n1) Sell\n 2) Give Up\n");
                state = scanf("%d", &temp);
                if(state){
                  if(temp == 2) {
                    giveUp = 1;
                    printf("\n\n Player %s has bakrupted! \n", p1.name);
                    return;
                  } else {
                    if(getPropertySize(board, &p1) > 0) {
                      sell_property(board, &p1);
                    } else {
                      printf("\n\n There is no any property to sell. Player %s has bakrupted! \n", p1.name);
                      return;
                    }
                  }
                } else {
                    printf("\nWrong Input! \n");
                  }
                }
                gameplay(board, p1, p2);
            }

          } else {
            printf("\nChoose buy property.\n");
            gameplay(board, p1, p2);
          }
            break;

        case TAX:
          if(p1.account >= board[location].rent){
            p1.account -= board[location].rent;
            printf("\nPlayer %s payed the taxes: %d\n", p1.name, board[location].rent);
            printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
            rolled=0;
            gameplay(board, p2, p1);
          } else {
              printf("\nYou have no enough money for paying the tax: %d\n", board[location].rent);
              while(p1.account < board[location].rent && giveUp == 0){
                printf("\nSell property to pay tax or give up!\n1) Sell\n 2) Give Up\n");
                state = scanf("%d", &temp);
                if(state){
                  if(temp == 2) {
                    giveUp = 1;
                    printf("\n\n Player %s has bakrupted! \n", p1.name);
                    return;
                  } else {
                    if(getPropertySize(board, &p1) > 0) {
                      sell_property(board, &p1);
                    } else {
                      printf("\n\n There is no any property to sell. Player %s has bakrupted! \n", p1.name);
                      return;
                    }
                  }
                } else {
                    printf("\nWrong Input! \n");
                  }
                }
                gameplay(board, p1, p2);
              }
          break;
        case PUNISH:
          p1.turn_to_wait = board[location].rent;
          printf("\nPlayer %s punished for %d turn.\n", p1.name, p1.turn_to_wait);
          printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
          rolled = 0;
          gameplay(board, p2, p1);
          break;
        case START:
          printf("\nPlayer %s pass the start block. Gain 10000&.\n", p1.name);
          p1.account += 10000;
          break;
        default:
          printf("\nWrong block type! Exit the game.");
          return;
          break;
      }
    } else {
      printf("\nYou already rolled the dice. Select other options.\n");
      gameplay(board, p1, p2);
    }
      break;
    case 2: //Show my naccount
        printf("\n%s's Account: %d$\n", p1.name, p1.account);
        gameplay(board, p1, p2);
      break;
    case 3: //Show properties
      showMyProperties(board, &p1);
      gameplay(board, p1, p2);
      break;
    case 4: //Show property deeds
      show_properties(board);
      gameplay(board, p1, p2);
      break;
    case 5: //Buy property
      if(board[location].type == PROPERTY){
        buy_property(&board[location], &p1);
        printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
        rolled = 0;
        gameplay(board, p2, p1);
      } else {
        printf("\n%s is not a property. \n", board[location].name);
        gameplay(board, p1, p2);
      }
      break;
    case 6: // Buy house
      if(board[location].type == PROPERTY){
        buy_property(&board[location], &p1);
        printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
        rolled = 0;
        gameplay(board, p2, p1);
      } else {
        printf("\n%s is not a property. \n", board[location].name);
        gameplay(board, p1, p2);
      }
      break;
    case 7: // Sell property
      if(getPropertySize(board, &p1) > 0) {
        sell_property(board, &p1);
      }
      else {
        printf("\nYou have no any property to show.\n");
      }
      gameplay(board, p1, p2);
      break;

  }
} else {
  printf("\nPlayer %s has %d more punishment\n", p1.name, p1.turn_to_wait);

  p1.turn_to_wait--;
  rolled=0;
  gameplay(board, p2, p1);
}
}

int getPropertySize(struct block board[20], struct player* p){
  int i, n = 0;
  for(i = 0; i<11; i++){
    if(p->owned_block_ids[i] != -1){
      n++;
    }
  }
  return n;
}

void showMyProperties(struct block board[20], struct player* p){
  int length, i, j, n = 0;
  for(i = 0; i<11; i++){
    if(p->owned_block_ids[i] != -1){
      n++;
      printf("\n");
      drawLongLine();
      printf("\n");
      length = strlen(board[i].name);
      printf("|       ");
      printf("%s", board[i].name);
      for(j = 0; j< 25 - length; j++){
        printf(" ");
      }
      printf("|\n");
      drawLongLine();
      printf("\n");
      drawFieldAndPriceLine("Rent", board[i].rent);
      drawFieldAndPriceLine("Rent 1 H", board[i].rent_1);
      drawFieldAndPriceLine("Rent 2 H", board[i].rent_2);
      drawFieldAndPriceLine("Rent 3 H", board[i].rent_3);
      drawLongLine();
      printf("\n");
      drawFieldAndPriceLine("House Price", board[i].house_price);
      drawLongLine();
      printf("\n");
    }
  }
  if(n == 0) printf("\nPlayer %s has no preoperty yet.\n", p->name);
}

int showMenu(){
  int select, state;
  printf("\n1 - Roll the dice\n");
  printf("2 - Show my account\n");
  printf("3 - Show my properties\n");
  printf("4 - Show my property deeds\n");
  printf("5 - Buy property\n");
  printf("6 - Buy house\n");
  printf("7 - Sell property\n");
  state = scanf("%d", &select);
  if(state == 1 && state>0 && state<8){
    return select;
  } else {
    fflush(stdout);
    showMenu();
  }
}

int rollTheDice(){
  int n = 1 +rand()%6;
  printf("\nDice: %d\n", n);
  return n;
}

//Part 5
void sell_property(struct block board[20], struct player* current_player){
  int i, id, select, cost, state=0;
  printf("\n-------------sell_property--------------------\n");
  for(i=0; i<11; i++){
    if(current_player->owned_block_ids[i] != -1){
      id = current_player->owned_block_ids[i];
      cost = board[id].price / 2;
      cost += (board[id].house_count * board[id].house_price) / 2;
      printf("Do you want to sell your property on %s? Cost: %d\n", board[id].name, cost);
      printf("1) Yes \n2) No\n3) Exit\n");
      state = scanf("%d", &select);
      if(state == 1 && (select == 1 || select == 2 || select == 3)){
        if(select == 1){
          current_player->account += cost;
          current_player->owned_block_ids[i] = -1;
          board[id].house_count=0;
          board[id].owner = noone;
          printPlayer(current_player);
          printBlock(&board[id]);
        } else if(select == 3){
          return;
        }
        continue;
      } else {
        printf("Wrong Input!");
      }
      return;
    }
  }
}

//Part 4
void buy_property(struct block* current_block, struct player* current_player){
  int state, constructedHouseNumber, enoughMoneyFor, maxHouse, i, buy;
  if(current_player->current_block_id == current_block->block_id){
    if(current_block->owner.type == noone.type){
      if(current_player->account >= current_block->price){
        printf("Do you want to buy the property %s(%d$)\n1) Yes\n2) No\n", current_block->name, current_block->price);
        state = scanf("%d", &buy);
        if(state == 1 && buy == 1){
          enoughMoneyFor = (current_player->account - current_block->price) / current_block->house_price;
          if(enoughMoneyFor>3) enoughMoneyFor = 3;
          printf("How many house do you want to build on %s? (Up to %d)\n", current_block->name, enoughMoneyFor);
          state = scanf("%d", &constructedHouseNumber);

          if(state && constructedHouseNumber>0 && constructedHouseNumber<=enoughMoneyFor){
            current_player->account -= current_block->price;
            if(current_player->account > constructedHouseNumber * current_block->house_price){
              current_player->account -= constructedHouseNumber * current_block->house_price;
              current_block->owner = *current_player;
              current_block->house_count = constructedHouseNumber;
              printBlock(current_block);
              addBlockToPlayer(current_block, current_player);
            } else {
              printf("You don't have enough money for build %d house.\n", constructedHouseNumber);
            }
          } else {
            printf("Wrong input! Try again.\n");
            buy_property(current_block, current_player);
          }
        } else {
            printf("Wrong Input! Try again.\n");
        }
      } else {
          printf("You have no enough money to buy the block %s\n",  current_block->name);
      }

    } else if(current_player->type == current_block->owner.type){
        if(current_block->house_count < 3){
          enoughMoneyFor = current_player->account / current_block->house_price;
          if(enoughMoneyFor>3) enoughMoneyFor = 3;
          maxHouse = enoughMoneyFor - current_block->house_count;
          printf("Now you have %d house on %s. \n", current_block->house_count, current_block->name);
          if(maxHouse>0 && maxHouse<4){
            printf("How many house do you want to build? (Up to %d)\n", maxHouse);
            state = scanf("%d", &constructedHouseNumber);

            if(state && constructedHouseNumber>0 && constructedHouseNumber<=enoughMoneyFor){
              current_player->account -= current_block->price;
              if(current_player->account > constructedHouseNumber * current_block->house_price){
                current_player->account -= constructedHouseNumber * current_block->house_price;
                current_block->house_count = constructedHouseNumber;
              } else {
                printf("You don't have enough money for build %d house.\n", constructedHouseNumber);
              }
            }
          }
        }
    } else {
          printf("ERROR! Other player lands on the block.\n");
      }
    } else {
      printf("ERROR! Player %s is not on the block %s\n", current_player->name, current_block->name);
      printBlock(current_block);
      printPlayer(current_player);

    }
}

void addBlockToPlayer(struct block* current_block, struct player* current_player){
  int i;
  for(i=0; i<11; i++){
    if(current_player->owned_block_ids[i] == -1){
      current_player->owned_block_ids[i] = current_block->block_id;
      return;
    }
  }
}

void printBlock(struct block* b){
  printf("\n---------------\n");
  printf("\nblock id: %d\n", b->block_id);
  printf("name: %s\n", b->name);
  printf("price: %d\n", b->price);
  printf("rent: %d\n", b->rent);
  printf("rent1: %d - rent2: %d - rent3: %d\n", b->rent_1, b->rent_2, b->rent_3);
  printf("house_price: %d\n", b->house_price);
  printf("house_count: %d\n", b->house_count);
  printf("owner.name: %s\n", b->owner.name);
  printf("block_type: %d\n", b->type);
  printf("\n---------------\n");
}

void printPlayer(struct player* p){
  int i;
  size_t n = sizeof(p->owned_block_ids) / sizeof(int);
  printf("\n---------------\n");
  printf("name: %s\n", p->name);
  printf("type: %d\n", p->type);
  printf("current_block_id: %d\n", p->current_block_id);
  for(i = 0; i< 11; i++){
    if(p->owned_block_ids[i] != 0) printf("owned_block_ids: %d\n", p->owned_block_ids[i]); //todo
  }
  printf("account: %d\n", p->account);
  printf("turn_to_wait: %d \n", p->turn_to_wait);
  printf("---------------\n");
}

//Part 3
void show_properties(struct block board[]){
  int i, j, length, state;

  printf("\nPlease select a property to see details:");
  printf("\n%d- %s", board[1].block_id, board[1].name);
  printf("\n%d- %s", board[3].block_id, board[3].name);
  printf("\n%d- %s", board[4].block_id, board[4].name);
  printf("\n%d- %s", board[6].block_id, board[6].name);
  printf("\n%d- %s", board[7].block_id, board[7].name);
  printf("\n%d- %s", board[9].block_id, board[9].name);
  printf("\n%d- %s", board[11].block_id, board[11].name);
  printf("\n%d- %s", board[13].block_id, board[13].name);
  printf("\n%d- %s", board[14].block_id, board[14].name);
  printf("\n%d- %s", board[16].block_id, board[16].name);
  printf("\n%d- %s", board[17].block_id, board[17].name);
  printf("\n%d- %s\n", board[19].block_id, board[19].name);
  state = scanf("%d", &i);
  if(state && i>0 && i<20){
    printf("\n\n\n");
    drawLongLine();
    printf("\n");
    length = strlen(board[i].name);
    printf("|       ");
    printf("%s", board[i].name);
    for(j = 0; j< 25 - length; j++){
      printf(" ");
    }
    printf("|\n");
    drawLongLine();
    printf("\n");
    drawFieldAndPriceLine("Rent", board[i].rent);
    drawFieldAndPriceLine("Rent 1 H", board[i].rent_1);
    drawFieldAndPriceLine("Rent 2 H", board[i].rent_2);
    drawFieldAndPriceLine("Rent 3 H", board[i].rent_3);
    drawLongLine();
    printf("\n");
    drawFieldAndPriceLine("House Price", board[i].house_price);
    drawLongLine();
    printf("\n");
  } else {
    printf("Wrong input! Try again.");
  }
}

void drawFieldAndPriceLine(char * field, int price){
  int i, j, length = 16 - strlen(field);
  printf("|       ");
  printf("%s", field);
  for(j = 0; j< length; j++){
    printf(" ");
  }
  length = 8 - getDigitNumber(price);
  printf("%d$", price);
  for(j = 0; j< length; j++){
    printf(" ");
  }
  printf("|\n");
}

void drawLongLine(){
  int k;
  for(k = 0; k<34; k++){
    printf("-");
  }
}

//Part 2
void show_board(struct block board[20], struct player player_one, struct player player_two){
  drawTop(board, player_one, player_two);
  drawMiddle(board, player_one, player_two);
  drawBottom(board, player_one, player_two);
}

void drawTop(struct block board[], struct player player_one, struct player player_two){
  int i, j, length;

  //begin line
  for(i = 0; i<(25*6)+1; i++){
    printf("-");
  }
  printf("\n");

  //name
  for(i=0; i<6; i++){
    length = strlen(board[i].name);
    printf("|       ");
    printf("%s", board[i].name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");
  //price
  for(i=0; i<6; i++){
    printf("|       ");
    if(board[i].type == PROPERTY){
      length = getDigitNumber(board[i].price) + 1;
      printf("%d$", board[i].price);
    } else if(board[i].type == TAX) {
      length = getDigitNumber(board[i].rent) + 1;
      printf("%d$", board[i].rent);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");

  //player
  for(i=0; i<6; i++){
    printf("|       ");
    if(board[i].block_id == player_one.current_block_id ){
      length = strlen(player_one.name);
      printf("%s", player_one.name);
    } else if(board[i].block_id == player_two.current_block_id){
      length = strlen(player_two.name);
      printf("%s", player_two.name);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");

  //end line
  for(i = 0; i<(25*6)+1; i++){
    printf("-");
  }
  printf("\n");
}

void drawMiddle(struct block board[], struct player player_one, struct player player_two){
  int i, j, k, m = 6, length;

  for(i=19; i>15; i--, m++){

    //name
    length = strlen(board[i].name);
    printf("|       ");
    printf("%s", board[i].name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
    printf("|");

    drawBlankArea();

    length = strlen(board[m].name);
    printf("|       ");
    printf("%s", board[m].name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
    printf("|\n");

    //price
    printf("|       ");
    if(board[i].type == PROPERTY){
      length = getDigitNumber(board[i].price) + 1;
      printf("%d$", board[i].price);
    } else if(board[i].type == TAX) {
      length = getDigitNumber(board[i].rent) + 1;
      printf("%d$", board[i].rent);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
      printf("|");

    drawBlankArea();

    printf("|       ");
    if(board[m].type == PROPERTY){
      length = getDigitNumber(board[m].price) + 1;
      printf("%d$", board[m].price);
    } else if(board[m].type == TAX) {
      length = getDigitNumber(board[m].rent) + 1;
      printf("%d$", board[m].rent);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
      printf("|\n");

      //player
      printf("|       ");
      if(board[i].block_id == player_one.current_block_id){
        length = strlen(player_one.name);
        printf("%s", player_one.name);
      } else if(board[i].block_id == player_two.current_block_id){
        length = strlen(player_two.name);
        printf("%s", player_two.name);
      } else {
        length = 0;
      }
      for(j = 0; j< 17 - length; j++){
        printf(" ");
      }
        printf("|");

        drawBlankArea();

        printf("|       ");
        if(board[m].block_id == player_one.current_block_id){
          length = strlen(player_one.name);
          printf("%s", player_one.name);
        } else if(board[m].block_id == player_two.current_block_id){
          length = strlen(player_two.name);
          printf("%s", player_two.name);
        } else {
          length = 0;
        }
        for(j = 0; j< 17 - length; j++){
          printf(" ");
        }
          printf("|\n");

  if(i>16){
    drawLine();
    drawBlankArea();
    drawLine();
    printf("\n");
  }
  }
}

void drawBottom(struct block board[], struct player player_one, struct player player_two){
  int i, j, length;

  //begin line
  for(i = 0; i<(25*6)+1; i++){
    printf("-");
  }
  printf("\n");

  //name
  for(i=15; i>9; i--){
    length = strlen(board[i].name);
    printf("|       ");
    printf("%s", board[i].name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");

  //price
  for(i=15; i>9; i--){
    printf("|       ");
    if(board[i].type == PROPERTY){
      length = getDigitNumber(board[i].price) + 1;
      printf("%d$", board[i].price);
    } else if(board[i].type == TAX) {
      length = getDigitNumber(board[i].rent) + 1;
      printf("%d$", board[i].rent);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");

  //player
  for(i=15; i>9; i--){
    printf("|       ");
    if(board[i].block_id == player_one.current_block_id){
      length = strlen(player_one.name);
      printf("%s", player_one.name);
    } else if(board[i].block_id == player_two.current_block_id){
      length = strlen(player_two.name);
      printf("%s", player_two.name);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");

  //end line
  for(i = 0; i<(25*6)+1; i++){
    printf("-");
  }
  printf("\n");
}

void drawBlankArea(){
  int k;
  for(k = 0; k<(25*4)-1; k++){
    printf(" ");
  }
}

void drawLine(){
  int k;
  for(k = 0; k<25+1; k++){
    printf("-");
  }
}

int getDigitNumber (int n) {
      if (n < 0) n = (n == INT_MIN) ? MAX_INT : -n;
      if (n < 10) return 1;
      if (n < 100) return 2;
      if (n < 1000) return 3;
      if (n < 10000) return 4;
      if (n < 100000) return 5;
      if (n < 1000000) return 6;
      if (n < 10000000) return 7;
      if (n < 100000000) return 8;
      if (n < 1000000000) return 9;
      return 10;
  }

//Part 1
void init_the_board(struct block board[20]){
  board[0].block_id = 0;
  board[0].name = "Start";
  board[0].price = 0;
  board[0].rent = 0;
  board[0].rent_1 = 0;
  board[0].rent_2 = 0;
  board[0].rent_3 = 0;
  board[0].house_price = 0;
  board[0].type = START;
  board[0].house_count = 0;
  board[0].owner = noone;

  board[1].block_id = 1;
  board[1].name = "Esenyurt";
  board[1].price = 16000;
  board[1].rent = 800;
  board[1].rent_1 = 4000;
  board[1].rent_2 = 9000;
  board[1].rent_3 = 25000;
  board[1].house_price = 10000;
  board[1].type = PROPERTY;
  board[1].house_count = 0;
  board[1].owner = noone;

  board[2].block_id = 2;
  board[2].name = "Car Park";
  board[2].price = 0;
  board[2].rent = 1500;
  board[2].rent_1 = 0;
  board[2].rent_2 = 0;
  board[2].rent_3 = 0;
  board[2].house_price = 0;
  board[2].type = TAX;
  board[2].house_count = 0;
  board[2].owner = noone;

  board[3].block_id = 3;
  board[3].name = "Tuzla";
  board[3].price = 16500;
  board[3].rent = 850;
  board[3].rent_1 = 4250;
  board[3].rent_2 = 9500;
  board[3].rent_3 = 26000;
  board[3].house_price = 12000;
  board[3].type = PROPERTY;
  board[3].house_count = 0;
  board[3].owner = noone;

  board[4].block_id = 4;
  board[4].name = "Arnavutkoy";
  board[4].price = 17000;
  board[4].rent = 875;
  board[4].rent_1 = 4500;
  board[4].rent_2 = 10000;
  board[4].rent_3 = 28000;
  board[4].house_price = 12000;
  board[4].type = PROPERTY;
  board[4].house_count = 0;
  board[4].owner = noone;

  board[5].block_id = 5;
  board[5].name = "Wait 2 Turn";
  board[5].price = 0;
  board[5].rent = 2;
  board[5].rent_1 = 0;
  board[5].rent_2 = 0;
  board[5].rent_3 = 0;
  board[5].house_price = 0;
  board[5].type = PUNISH;
  board[5].house_count = 0;
  board[5].owner = noone;

  board[6].block_id = 6;
  board[6].name = "Catalca";
  board[6].price = 20000;
  board[6].rent = 1000;
  board[6].rent_1 = 5000;
  board[6].rent_2 = 12000;
  board[6].rent_3 = 30000;
  board[6].house_price = 13000;
  board[6].type = PROPERTY;
  board[6].house_count = 0;
  board[6].owner = noone;

  board[7].block_id = 7;
  board[7].name = "Beykoz";
  board[7].price = 23000;
  board[7].rent = 1100;
  board[7].rent_1 = 5500;
  board[7].rent_2 = 12500;
  board[7].rent_3 = 33000;
  board[7].house_price = 13000;
  board[7].type = PROPERTY;
  board[7].house_count = 0;
  board[7].owner = noone;

  board[8].block_id = 8;
  board[8].name = "Car Fix";
  board[8].price = 0;
  board[8].rent = 1750;
  board[8].rent_1 = 0;
  board[8].rent_2 = 0;
  board[8].rent_3 = 0;
  board[8].house_price = 0;
  board[8].type = TAX;
  board[8].house_count = 0;
  board[8].owner = noone;

  board[9].block_id = 9;
  board[9].name = "Maltepe";
  board[9].price = 30000;
  board[9].rent = 1350;
  board[9].rent_1 = 7000;
  board[9].rent_2 = 15000;
  board[9].rent_3 = 40000;
  board[9].house_price = 15000;
  board[9].type = PROPERTY;
  board[9].house_count = 0;
  board[9].owner = noone;

  board[10].block_id = 10;
  board[10].name = "Bills";
  board[10].price = 0;
  board[10].rent = 2000;
  board[10].rent_1 = 0;
  board[10].rent_2 = 0;
  board[10].rent_3 = 0;
  board[10].house_price = 0;
  board[10].type = TAX;
  board[10].house_count = 0;
  board[10].owner = noone;

  board[11].block_id = 11;
  board[11].name = "Sisli";
  board[11].price = 33000;
  board[11].rent = 1500;
  board[11].rent_1 = 8000;
  board[11].rent_2 = 16000;
  board[11].rent_3 = 42000;
  board[11].house_price = 16000;
  board[11].type = PROPERTY;
  board[11].house_count = 0;
  board[11].owner = noone;

  board[12].block_id = 12;
  board[12].name = "Oil";
  board[12].price = 0;
  board[12].rent = 2250;
  board[12].rent_1 = 0;
  board[12].rent_2 = 0;
  board[12].rent_3 = 0;
  board[12].house_price = 0;
  board[12].type = TAX;
  board[12].house_count = 0;
  board[12].owner = noone;

  board[13].block_id = 13;
  board[13].name = "Atasehir";
  board[13].price = 35000;
  board[13].rent = 1600;
  board[13].rent_1 = 8500;
  board[13].rent_2 = 17000;
  board[13].rent_3 = 45000;
  board[13].house_price = 17000;
  board[13].type = PROPERTY;
  board[13].house_count = 0;
  board[13].owner = noone;

  board[14].block_id = 14;
  board[14].name = "Sariyer";
  board[14].price = 40000;
  board[14].rent = 1750;
  board[14].rent_1 = 9500;
  board[14].rent_2 = 19000;
  board[14].rent_3 = 48000;
  board[14].house_price = 19000;
  board[14].type = PROPERTY;
  board[14].house_count = 0;
  board[14].owner = noone;

  board[15].block_id = 15;
  board[15].name = "Wait 1 Turn";
  board[15].price = 0;
  board[15].rent = 1;
  board[15].rent_1 = 0;
  board[15].rent_2 = 0;
  board[15].rent_3 = 0;
  board[15].house_price = 0;
  board[15].type = PUNISH;
  board[15].house_count = 0;
  board[15].owner = noone;

  board[16].block_id = 16;
  board[16].name = "Kadikoy";
  board[16].price = 43000;
  board[16].rent = 1900;
  board[16].rent_1 = 11000;
  board[16].rent_2 = 21500;
  board[16].rent_3 = 55000;
  board[16].house_price = 23000;
  board[16].type = PROPERTY;
  board[16].house_count = 0;
  board[16].owner = noone;

  board[17].block_id = 17;
  board[17].name = "Besiktas";
  board[17].price = 60000;
  board[17].rent = 2500;
  board[17].rent_1 = 15000;
  board[17].rent_2 = 28000;
  board[17].rent_3 = 60000;
  board[17].house_price = 30000;
  board[17].type = PROPERTY;
  board[17].house_count = 0;
  board[17].owner = noone;

  board[18].block_id = 18;
  board[18].name = "Vocation";
  board[18].price = 0;
  board[18].rent = 5000;
  board[18].rent_1 = 0;
  board[18].rent_2 = 0;
  board[18].rent_3 = 0;
  board[18].house_price = 0;
  board[18].type = TAX;
  board[18].house_count = 0;
  board[18].owner = noone;

  board[19].block_id = 19;
  board[19].name = "Bebek";
  board[19].price = 70000;
  board[19].rent = 3500;
  board[19].rent_1 = 20000;
  board[19].rent_2 = 35500;
  board[19].rent_3 = 65000;
  board[19].house_price = 35000;
  board[19].type = PROPERTY;
  board[19].house_count = 0;
  board[19].owner = noone;

}
