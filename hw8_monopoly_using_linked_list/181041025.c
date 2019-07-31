#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INT_MIN 214748364
#define MAX_INT -214748364

#define TRUE 1
#define FALSE 0

typedef enum {NOONE, CAP, CAR} player_type;
typedef enum {PROPERTY, TAX, PUNISH, START, FORTUNE, NONE} block_type;

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

struct node {
  struct block payload;
  struct node * next;
};

int rolled = 0, location = 0;

struct block board[24];
struct player cap = {CAP, 0, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 100000, 0, "CAP"};
struct player car = {CAR, 0, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 100000, 0, "CAR"};
const struct player noone = {NONE, -1, {}, 0, -1, "No One"};

void init_the_board(struct node**);
void show_board(struct node*, struct player, struct player);
void drawTop(struct node*, struct player, struct player);
void drawMiddle(struct node*, struct player, struct player);
void drawBottom(struct node*, struct player, struct player);
int getDigitNumber(int);
void drawBlankArea();
void drawLine();
void drawLongLine();
void show_properties(struct node*);
void drawFieldAndPriceLine(char *, int);
void buy_property(struct node*, struct block*, struct player*);
void printBlock(struct block*);
void printPlayer(struct player*);
void addBlockToPlayer(struct block*, struct player*);
void sell_property(struct node*, struct player*);
void gameplay (struct node*, struct player, struct player);
int showMenu();
int rollTheDice();
void showMyProperties(struct node*, struct player*);
int getPropertySize(struct player*);
void print_list(struct node*);
void append(struct node**, struct block);
struct node* get_block(int, struct node*);

int main(){
  // struct node HEAD;
  srand ( time(NULL) );
  struct node *listHead = NULL;
  init_the_board(&listHead);
//  print_list(listHead);
  printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", cap.name);
  gameplay(listHead, cap, car);
}

//Part 6
void gameplay (struct node* node, struct player player_one, struct player player_two){
  struct player p1 = player_one, p2 = player_two;
  int rent = 0, giveUp = 0, temp, state, tempDice;
  struct node* boardptr = (struct node*) malloc(sizeof(struct node));


  switch(showMenu()){
    case 1: // Roll the Dice

    if(p1.turn_to_wait == 0){

      if(!rolled){
        rolled = 1;
        location = (p1.current_block_id + rollTheDice()) % 20;
        if(p1.current_block_id > location) {
          printf("\nPlayer %s pass the start block. Gain 10000&.\n", p1.name);
          p1.account += 10000;
        }
        p1.current_block_id = location;
        show_board(node, p1, p2);
        boardptr = get_block(location, node);
        switch(boardptr->payload.type){
          case PROPERTY:
          if(boardptr->payload.owner.type == p2.type){
            switch(boardptr->payload.house_count) {
              case 0:
                rent = boardptr->payload.rent;
                break;
              case 1:
                rent = boardptr->payload.rent_1;
                break;
              case 2:
                rent = boardptr->payload.rent_2;
                break;
              case 3:
                rent = boardptr->payload.rent_3;
                break;
              default:
                printf("\nERROR! Unexpected house count: %d. Exit the game.\n", boardptr->payload.house_count);
                break;
            }

            if(p1.account>=rent){
              printf("\nPlayer %s pay the rent (%d$) to player %s.\n", p1.name, rent, p2.name);
              p1.account -= rent;
              printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
              rolled=0;
              gameplay(node, p2, p1);
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
                    if(getPropertySize(&p1) > 0) {
                      sell_property(&(boardptr->payload), &p1);
                    } else {
                      printf("\n\n There is no any property to sell. Player %s has bakrupted! \n", p1.name);
                      return;
                    }
                  }
                } else {
                    printf("\nWrong Input! \n");
                  }
                }
                gameplay(node, p1, p2);
            }

          } else {
            buy_property(&(boardptr->payload), &board[location], &p1);
            printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
            rolled=0;
            gameplay(node, p2, p1);
          }
            break;

        case TAX:
          if(p1.account >= boardptr->payload.rent){
            p1.account -= boardptr->payload.rent;
            printf("\nPlayer %s payed the taxes: %d\n", p1.name, boardptr->payload.rent);
            printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
            rolled=0;
            gameplay(node, p2, p1);
          } else {
              printf("\nYou have no enough money for paying the tax: %d\n", boardptr->payload.rent);
              while(p1.account < boardptr->payload.rent && giveUp == 0){
                printf("\nSell property to pay tax or give up!\n1) Sell\n 2) Give Up\n");
                state = scanf("%d", &temp);
                if(state){
                  if(temp == 2) {
                    giveUp = 1;
                    printf("\n\n Player %s has bakrupted! \n", p1.name);
                    return;
                  } else {
                    if(getPropertySize(&p1) > 0) {
                      sell_property(&(boardptr->payload), &p1);
                    } else {
                      printf("\n\n There is no any property to sell. Player %s has bakrupted! \n", p1.name);
                      return;
                    }
                  }
                } else {
                    printf("\nWrong Input! \n");
                  }
                }
                gameplay(node, p1, p2);
              }
          break;
        case PUNISH:
          p1.turn_to_wait = boardptr->payload.rent;
          printf("\nPlayer %s punished for %d turn.\n", p1.name, p1.turn_to_wait);
          printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
          rolled = 0;
          gameplay(node, p2, p1);
          break;
        case START:
          printf("\nPlayer %s pass the start block. Gain 10000&.\n", p1.name);
          p1.account += 10000;
          break;
        case FORTUNE:
            switch(pickFortune()){
              case 1:
                printf("You pick Free House card\n"); //Build a house on one of your properties for free.
                //showMyProperties();
                printf("Choose one of your property to build an house.\n");
                break;
              case 2:
                printf("You pick Time Travel card.\n"); // Roll a dice. If result is [1-3], move forward by 2 blocks. Else, move backward by 2 blocks.
                tempDice = rollTheDice();
                if(tempDice>=1 && tempDice<=3){
                  printf("Moved to 2 block beyond.\n");
                  p1.current_block_id += 2;
                } else {
                  printf("Moved to 2 block back.\n");
                  p1.current_block_id -= 2;
                }
                break;
              case 3:
                printf("You pick Garnishment card. Payed 5000$ to the bank.\n"); // Pay 5.000 to the bank.
                p1.account -= 5000;
                break;
              case 4:
                printf("You pick Generosity card. Payed 10000$ to %s.\n", p2.name); // Give 10.000 to the opponent.
                p1.account -= 10000;
                p2.account += 10000;
                break;
              case 5:
                printf("You pick Treasure Hunter card. Taken 20000$ from bank.\n"); //Take 20.000 from the bank.
                p1.account += 20000;
                break;
            }
            rolled=0;
            gameplay(node, p2, p1);
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
      gameplay(node, p1, p2);
    }

  } else {
    printf("\nPlayer %s has %d more punishment\n", p1.name, p1.turn_to_wait);

    p1.turn_to_wait--;
    rolled=0;
    gameplay(node, p2, p1);
  }
      break;
    case 2: //Show my account
        printf("\n%s's Account: %d$\n", p1.name, p1.account);
        gameplay(node, p1, p2);
      break;
    case 3: //Show properties
      showMyProperties(node, &p1);
      gameplay(node, p1, p2);
      break;
    case 4: //Show property deeds
      show_properties(board);
      gameplay(node, p1, p2);
      break;
    case 5: //Buy property
      if(boardptr->payload.type == PROPERTY){
        buy_property(&(boardptr->payload), &boardptr->payload, &p1);
        printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
        rolled = 0;
        gameplay(node, p2, p1);
      } else {
        printf("\n%s is not a property. \n", boardptr->payload.name);
        gameplay(node, p1, p2);
      }
      break;
    case 6: // Buy house
      if(boardptr->payload.type == PROPERTY){
        buy_property(&(boardptr->payload), &boardptr->payload, &p1);
        printf("\n\n-------------------------------It's %s's turn!-----------------------------------\n\n", p2.name);
        rolled = 0;
        gameplay(node, p2, p1);
      } else {
        printf("\n%s is not a property. \n", boardptr->payload.name);
        gameplay(node, p1, p2);
      }
      break;
    case 7: // Sell property
      if(getPropertySize(&p1) > 0) {
        sell_property(&(boardptr->payload), &p1);
      }
      else {
        printf("\nYou have no any property to show.\n");
      }
      gameplay(node, p1, p2);
      break;

  }

}

int getPropertySize(struct player* p){
  int i, n = 0;
  for(i = 0; i<11; i++){
    if(p->owned_block_ids[i] != -1){
      n++;
    }
  }
  return n;
}

void showMyProperties(struct node *node, struct player* p){
  int length, i, j, n = 0;
  struct node *tempNode = node;

  for(i = 1; i<11; i++){
    tempNode = tempNode->next;
    if(p->owned_block_ids[i] != -1){
      n++;
      printf("\n");
      drawLongLine();
      printf("\n");
      length = strlen(tempNode->payload.name);
      printf("|       ");
      printf("%s", tempNode->payload.name);
      for(j = 0; j< 25 - length; j++){
        printf(" ");
      }
      printf("|\n");
      drawLongLine();
      printf("\n");
      drawFieldAndPriceLine("Rent", tempNode->payload.rent);
      drawFieldAndPriceLine("Rent 1 H", tempNode->payload.rent_1);
      drawFieldAndPriceLine("Rent 2 H", tempNode->payload.rent_2);
      drawFieldAndPriceLine("Rent 3 H", tempNode->payload.rent_3);
      drawLongLine();
      printf("\n");
      drawFieldAndPriceLine("House Price", tempNode->payload.house_price);
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

int pickFortune(){
  int n = 1 +rand()%5;
  return n;
}

//Part 5
void sell_property(struct node *node, struct player* current_player){
/*
  while (node != NULL)
  {
    if(node->payload.block_id == id) return &(node->payload);
     node = node->next;
   }
*/
  struct node *tempNode = node;
  int i, id, select, cost, state=0;
  printf("\n-------------sell_property--------------------\n");
  for(i=0; i<11; i++){
    if(current_player->owned_block_ids[i] != -1){
      id = current_player->owned_block_ids[i];
      tempNode = tempNode->next;
      cost = (tempNode->payload).price / 2;
      cost += ((tempNode->payload).house_count * (tempNode->payload).house_price) / 2;
      printf("Do you want to sell your property on %s? Cost: %d\n", (tempNode->payload).name, cost);
      printf("1) Yes \n2) No\n3) Exit\n");
      state = scanf("%d", &select);
      if(state == 1 && (select == 1 || select == 2 || select == 3)){
        if(select == 1){
          current_player->account += cost;
          current_player->owned_block_ids[i] = -1;
          (tempNode->payload).house_count=0;
          (tempNode->payload).owner = noone;
          printPlayer(current_player);
          printBlock(&(tempNode->payload));
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
void buy_property(struct node* current_block, struct block* crrnt_blck, struct player* current_player){

  int state, constructedHouseNumber, enoughMoneyFor, maxHouse, i, buy;
  if(current_player->current_block_id == current_block->payload.block_id){
    if(current_block->payload.owner.type == noone.type){
      if(current_player->account >= current_block->payload.price){
        printf("Do you want to buy the property %s(%d$)\n1) Yes\n2) No\n", current_block->payload.name, current_block->payload.price);
        state = scanf("%d", &buy);
        if(state == 1){
          if(buy == 1){
            enoughMoneyFor = (current_player->account - current_block->payload.price) / current_block->payload.house_price;
            if(enoughMoneyFor>3) enoughMoneyFor = 3;
            printf("How many house do you want to build on %s? (Up to %d)\n", current_block->payload.name, enoughMoneyFor);
            state = scanf("%d", &constructedHouseNumber);

            if(state && constructedHouseNumber>0 && constructedHouseNumber<=enoughMoneyFor){
              current_player->account -= current_block->payload.price;
              printf("Player %s has buy %s.\n", current_player->name, current_block->payload.name);
              if(current_player->account > constructedHouseNumber * current_block->payload.house_price){
                current_player->account -= constructedHouseNumber * current_block->payload.house_price;
                current_block->payload.owner = *current_player;
                current_block->payload.house_count = constructedHouseNumber;
                // printBlock(current_block);
                printf("Player %s has built %d house on %s.\n", current_player->name, constructedHouseNumber, current_block->payload.name);
                addBlockToPlayer(current_block, current_player);
              } else {
                printf("You don't have enough money for build %d house.\n", constructedHouseNumber);
              }
            } else {
              printf("Wrong input! Try again.\n");
              buy_property(current_block, crrnt_blck, current_player);
            }
          }
        } else {
            printf("Wrong Input! Try again.\n");
        }
      } else {
          printf("You have no enough money to buy the block %s\n",  current_block->payload.name);
      }

    } else if(current_player->type == current_block->payload.owner.type){
        if(current_block->payload.house_count < 3){
          enoughMoneyFor = current_player->account / current_block->payload.house_price;
          if(enoughMoneyFor>3) enoughMoneyFor = 3;
          maxHouse = enoughMoneyFor - current_block->payload.house_count;
          printf("Now you have %d house on %s. \n", current_block->payload.house_count, current_block->payload.name);
          if(maxHouse>0 && maxHouse<4){
            printf("How many house do you want to build? (Up to %d)\n", maxHouse);
            state = scanf("%d", &constructedHouseNumber);

            if(state && constructedHouseNumber>0 && constructedHouseNumber<=enoughMoneyFor){
              current_player->account -= current_block->payload.price;
              if(current_player->account > constructedHouseNumber * current_block->payload.house_price){
                current_player->account -= constructedHouseNumber * current_block->payload.house_price;
                current_block->payload.house_count = constructedHouseNumber;
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
      printf("ERROR! Player %s is not on the block %s\n", current_player->name, current_block->payload.name);
      printBlock(crrnt_blck);
      printPlayer(crrnt_blck);

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
void show_properties(struct node *node){
  int i, j, length, state;

  while (node != NULL)
  {
     printf("\n%d- %s", node->payload.block_id, node->payload.name);
     node = node->next;
   }

  printf("\nPlease select a property to see details:");
  /*
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
  */
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
void show_board(struct node* node, struct player player_one, struct player player_two){
  drawTop(node, player_one, player_two);
  drawMiddle(node, player_one, player_two);
  drawBottom(node, player_one, player_two);
}

void drawTop(struct node* node, struct player player_one, struct player player_two){
  int i, j, length, sameBlock = FALSE;
  struct node* boardptr = (struct node*) malloc(sizeof(struct node));

  //begin line
  for(i = 0; i<(25*6)+1; i++){
    printf("-");
  }
  printf("\n");

  //name
  for(i=0; i<6; i++){
    boardptr = get_block(i, node);
    length = strlen(boardptr->payload.name);
    printf("|       ");
    printf("%s", boardptr->payload.name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");
  //price
  for(i=0; i<6; i++){
    printf("|       ");
    boardptr = get_block(i, node);
    if(boardptr->payload.type == PROPERTY){
      length = getDigitNumber(boardptr->payload.price) + 1;
      printf("%d$", boardptr->payload.price);
    } else if(boardptr->payload.type == TAX) {
      length = getDigitNumber(boardptr->payload.rent) + 1;
      printf("%d$", boardptr->payload.rent);
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
    boardptr = get_block(i, node);
    sameBlock=FALSE;
    length = 0;
    printf("|       ");
    if(boardptr->payload.block_id == player_one.current_block_id ){
      length += strlen(player_one.name);
      printf("%s", player_one.name);
      sameBlock = TRUE;
    }
    if(boardptr->payload.block_id == player_two.current_block_id){
      length += strlen(player_two.name);
      if(sameBlock){
        printf(", %s", player_two.name);
        length += 2;
      } else {
        printf("%s", player_two.name);
      }
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

void drawMiddle(struct node* node, struct player player_one, struct player player_two){
  int i, j, k, m = 6, length, sameBlock = FALSE;
  struct node* boardptr_i = (struct node*) malloc(sizeof(struct node));
  struct node* boardptr_m = (struct node*) malloc(sizeof(struct node));

  for(i=23; i>17; i--, m++){

    boardptr_i = get_block(i, node);
    boardptr_m = get_block(m, node);

    //name
    length = strlen(boardptr_i->payload.name);
    printf("|       ");
    printf("%s", boardptr_i->payload.name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
    printf("|");

    drawBlankArea();

    length = strlen(boardptr_m->payload.name);
    printf("|       ");
    printf("%s", boardptr_m->payload.name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
    printf("|\n");

    //price
    printf("|       ");
    if(boardptr_i->payload.type == PROPERTY){
      length = getDigitNumber(boardptr_i->payload.price) + 1;
      printf("%d$", boardptr_i->payload.price);
    } else if(boardptr_i->payload.type == TAX) {
      length = getDigitNumber(boardptr_i->payload.rent) + 1;
      printf("%d$", boardptr_i->payload.rent);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
      printf("|");

    drawBlankArea();

    printf("|       ");
    if(boardptr_m->payload.type == PROPERTY){
      length = getDigitNumber(boardptr_m->payload.price) + 1;
      printf("%d$", boardptr_m->payload.price);
    } else if(board[m].type == TAX) {
      length = getDigitNumber(boardptr_m->payload.rent) + 1;
      printf("%d$", boardptr_m->payload.rent);
    } else {
      length = 0;
    }
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
      printf("|\n");

      //player
      printf("|       ");
      sameBlock=FALSE;
      length = 0;
      if(boardptr_i->payload.block_id == player_one.current_block_id ){
        length += strlen(player_one.name);
        printf("%s", player_one.name);
        sameBlock = TRUE;
      }
      if(boardptr_i->payload.block_id == player_two.current_block_id){
        length += strlen(player_two.name);
        if(sameBlock){
          printf(", %s", player_two.name);
          length -= 2;
        } else {
          printf("%s", player_two.name);
        }
      }

      for(j = 0; j< 17 - length; j++){
        printf(" ");
      }
        printf("|");

        drawBlankArea();

        printf("|       ");
        sameBlock=FALSE;
        length = 0;
        if(boardptr_m->payload.block_id == player_one.current_block_id ){
          length += strlen(player_one.name);
          printf("%s", player_one.name);
          sameBlock = TRUE;
        }
        if(boardptr_m->payload.block_id == player_two.current_block_id){
          length += strlen(player_two.name);
          if(sameBlock){
            printf(", %s", player_two.name);
            length -= 2;
          } else {
            printf("%s", player_two.name);
          }
        }

        for(j = 0; j< 17 - length; j++){
          printf(" ");
        }
          printf("|\n");

  if(i>18){
    drawLine();
    drawBlankArea();
    drawLine();
    printf("\n");
  }
  }
}

void drawBottom(struct node* node, struct player player_one, struct player player_two){
  int i, j, length, sameBlock = FALSE;
  struct node* boardptr = (struct node*) malloc(sizeof(struct node));

  //begin line
  for(i = 0; i<(25*6)+1; i++){
    printf("-");
  }
  printf("\n");

  //name
  for(i=17; i>11; i--){
    boardptr = get_block(i, node);
    length = strlen(boardptr->payload.name);
    printf("|       ");
    printf("%s", boardptr->payload.name);
    for(j = 0; j< 17 - length; j++){
      printf(" ");
    }
  }
  printf("|\n");

  //price
  for(i=15; i>9; i--){
    boardptr = get_block(i, node);
    printf("|       ");
    if(board[i].type == PROPERTY){
      length = getDigitNumber(boardptr->payload.price) + 1;
      printf("%d$", boardptr->payload.price);
    } else if(boardptr->payload.type == TAX) {
      length = getDigitNumber(board[i].rent) + 1;
      printf("%d$", boardptr->payload.rent);
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
    boardptr = get_block(i, node);
    sameBlock=FALSE;
    length = 0;
    printf("|       ");
    if(boardptr->payload.block_id == player_one.current_block_id ){
      length += strlen(player_one.name);
      printf("%s", player_one.name);
      sameBlock = TRUE;
    }
    if(boardptr->payload.block_id == player_two.current_block_id){
      length += strlen(player_two.name);
      if(sameBlock){
        printf(", %s", player_two.name);
        length += 2;
      } else {
        printf("%s", player_two.name);
      }
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

void print_list(struct node *node)
{
  while (node != NULL)
  {
     printf("%s\n", node->payload.name);
     node = node->next;
   }
}

struct node* get_block(int id, struct node *node){
  while (node != NULL)
  {
    if(node->payload.block_id == id){
       return &(node->payload);
    }
     node = node->next;
   }
   return NULL;
}

void append(struct node** head_ref, struct block new_current)
{
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->payload = new_current;
    new_node->next = NULL;
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }
    while (last->next != NULL) last = last->next;
    last->next = new_node;
    return;
}


void init_the_board(struct node** root)
{
  //root = NULL;
  struct block temp = { .block_id = 0, .price = 0, .name = "Start", .rent=0, .rent_1=0, .rent_2=0, .rent_3=0, .house_price= 0, .type= START, .house_count = 0, .owner=noone};
  append(root, temp);

  temp.block_id = 1;
  temp.name = "Esenyurt";
  temp.price = 16000;
  temp.rent = 800;
  temp.rent_1 = 4000;
  temp.rent_2 = 9000;
  temp.rent_3 = 25000;
  temp.house_price = 10000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 2;
  temp.name = "Car Park";
  temp.price = 0;
  temp.rent = 1500;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = TAX;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 3;
  temp.name = "Fortune Card";
  temp.price = 0;
  temp.rent = 0;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = FORTUNE;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 4;
  temp.name = "Tuzla";
  temp.price = 16500;
  temp.rent = 850;
  temp.rent_1 = 4250;
  temp.rent_2 = 9500;
  temp.rent_3 = 26000;
  temp.house_price = 12000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 5;
  temp.name = "Arnavutkoy";
  temp.price = 17000;
  temp.rent = 875;
  temp.rent_1 = 4500;
  temp.rent_2 = 10000;
  temp.rent_3 = 28000;
  temp.house_price = 12000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 6;
  temp.name = "Wait 2 Turn";
  temp.price = 0;
  temp.rent = 2;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = PUNISH;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 7;
  temp.name = "Catalca";
  temp.price = 20000;
  temp.rent = 1000;
  temp.rent_1 = 5000;
  temp.rent_2 = 12000;
  temp.rent_3 = 30000;
  temp.house_price = 13000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 8;
  temp.name = "Beykoz";
  temp.price = 23000;
  temp.rent = 1100;
  temp.rent_1 = 5500;
  temp.rent_2 = 12500;
  temp.rent_3 = 33000;
  temp.house_price = 13000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 9;
  temp.name = "Fortune Card";
  temp.price = 0;
  temp.rent = 0;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = FORTUNE;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 10;
  temp.name = "Car Fix";
  temp.price = 0;
  temp.rent = 1750;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = TAX;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 11;
  temp.name = "Maltepe";
  temp.price = 30000;
  temp.rent = 1350;
  temp.rent_1 = 7000;
  temp.rent_2 = 15000;
  temp.rent_3 = 40000;
  temp.house_price = 15000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 12;
  temp.name = "Bills";
  temp.price = 0;
  temp.rent = 2000;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = TAX;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 13;
  temp.name = "Sisli";
  temp.price = 33000;
  temp.rent = 1500;
  temp.rent_1 = 8000;
  temp.rent_2 = 16000;
  temp.rent_3 = 42000;
  temp.house_price = 16000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 14;
  temp.name = "Oil";
  temp.price = 0;
  temp.rent = 2250;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = TAX;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 15;
  temp.name = "Fortune Card";
  temp.price = 0;
  temp.rent = 0;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = FORTUNE;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 16;
  temp.name = "Atasehir";
  temp.price = 35000;
  temp.rent = 1600;
  temp.rent_1 = 8500;
  temp.rent_2 = 17000;
  temp.rent_3 = 45000;
  temp.house_price = 17000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 17;
  temp.name = "Sariyer";
  temp.price = 40000;
  temp.rent = 1750;
  temp.rent_1 = 9500;
  temp.rent_2 = 19000;
  temp.rent_3 = 48000;
  temp.house_price = 19000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 18;
  temp.name = "Wait 1 Turn";
  temp.price = 0;
  temp.rent = 1;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = PUNISH;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 19;
  temp.name = "Kadikoy";
  temp.price = 43000;
  temp.rent = 1900;
  temp.rent_1 = 11000;
  temp.rent_2 = 21500;
  temp.rent_3 = 55000;
  temp.house_price = 23000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 20;
  temp.name = "Besiktas";
  temp.price = 60000;
  temp.rent = 2500;
  temp.rent_1 = 15000;
  temp.rent_2 = 28000;
  temp.rent_3 = 60000;
  temp.house_price = 30000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 21;
  temp.name = "Fortune Card";
  temp.price = 0;
  temp.rent = 0;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = FORTUNE;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 22;
  temp.name = "Vocation";
  temp.price = 0;
  temp.rent = 5000;
  temp.rent_1 = 0;
  temp.rent_2 = 0;
  temp.rent_3 = 0;
  temp.house_price = 0;
  temp.type = TAX;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

  temp.block_id = 23;
  temp.name = "Bebek";
  temp.price = 70000;
  temp.rent = 3500;
  temp.rent_1 = 20000;
  temp.rent_2 = 35500;
  temp.rent_3 = 65000;
  temp.house_price = 35000;
  temp.type = PROPERTY;
  temp.house_count = 0;
  temp.owner = noone;
  append(root, temp);

//  print_list(root);
}
