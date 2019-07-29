#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 30

struct person{
	char name[SIZE];
	char surname[SIZE];
	char musical_Work[SIZE];
	int age;
	struct person *next;
} *top;

const int SIZE_PERSON_S = (SIZE*3) + (sizeof(int)*2);

void menu();
void get_person();
void addNode(char[], char[], char[], int);
void printNode(struct person *);
void printStack();
void delete_node();
void sort_by_age();
void sort_by_name();
void swap(struct person *, struct person *);
void add_test();

void sort_by_name(){
	struct person *p1, *p2;
	p1 = top;
	for( ;p1->next != NULL; p1 = p1->next)
	{
		for(p2 = p1->next; p2 != NULL; p2 = p2->next)
		{
			//Sort by name.
			//Sort by surname when name is same.
			//Sort by creation when name and surname is same.
			if(strcmp(p1->name, p2->name)>0 ||
			(strcmp(p1->name, p2->name) == 0 && strcmp(p1->surname, p2->surname)>0) ||
			(strcmp(p1->name, p2->name) == 0 && strcmp(p1->surname, p2->surname) == 0 && strcmp(p1->musical_Work, p2->musical_Work)>0))
			{
				swap(p1, p2);
			}
		}
	}
	printStack();
}

void sort_by_age(){
	struct person *p1, *p2;
	p1 = top;
	for( ;p1->next != NULL; p1 = p1->next)
	{
		for(p2 = p1->next; p2 != NULL; p2 = p2->next)
		{
			if(p1->age > p2->age)
			{
				swap(p1, p2);
			}
		}
	}
	printStack();
}

void swap(struct person *p1, struct person *p2){
	struct person *temp = (struct person*) malloc(SIZE_PERSON_S);

	strcpy(temp->name, p1->name);
	strcpy(temp->surname, p1->surname);
	strcpy(temp->musical_Work, p1->musical_Work);
	temp->age = p1->age;

	strcpy(p1->name, p2->name);
	strcpy(p1->surname, p2->surname);
	strcpy(p1->musical_Work, p2->musical_Work);
	p1->age = p2->age;

	strcpy(p2->name, temp->name);
	strcpy(p2->surname, temp->surname);
	strcpy(p2->musical_Work, temp->musical_Work);
	p2->age = temp->age;

	free(temp);
}

void delete_node(){
	struct person *temp;
	if(top != NULL){
		if(top->next == NULL) {
    	top == NULL;
    	printf("\nThe last element has deleted. Stack is empty now.\n");
    	return;
  	}
    temp = top;
		top = top->next;
		printStack();
    free(temp);
	} else {
		printf("\nStack is already empty.");
	}
}

void addNode(char name[], char surname [], char creation [], int age){
	struct person * newPerson;
	newPerson = (struct person*) malloc(SIZE_PERSON_S);
	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	strcpy(newPerson->musical_Work, creation);
	newPerson->age = age;
	newPerson->next = NULL;
	if(top == NULL){
		top = newPerson;
		top->next = NULL;
		printStack();
	} else {
		newPerson->next = top;
		top = newPerson;
		printStack();
	}
}

void printStack(){
	struct person * ptr = (struct person*) malloc(SIZE_PERSON_S);
	ptr = top;
	int i = 0;
		while(ptr != NULL){
			printf("\n%d)", ++i);
			printNode(ptr);
			ptr = ptr->next;
		}
}

void printNode(struct person * ptr){
	//printf(" %p | %s %s %s %d - next:%p (size:%ld)\n", ptr, ptr->name, ptr->surname, ptr->musical_Work, ptr->age, ptr->next, sizeof(*ptr));
	printf(" %s\n%s\n%s\n%d\n", ptr->name, ptr->surname, ptr->musical_Work, ptr->age);
}

void get_person(){
	int state = 0;
	char * name = (char*) malloc(sizeof(char)*SIZE);
	char * surname = (char*) malloc(sizeof(char)*SIZE);
	char * creation = (char*) malloc(sizeof(char)*SIZE);
	int age;
	printf("Name: ");
	state = scanf(" %99[^\n]s", name);
	printf("Surname: ");
	state = scanf(" %99[^\n]s", surname) && state;
	printf("Creation: ");
	state = scanf(" %99[^\n]s", creation) && state;
	printf("Age: ");
	state = scanf("%d", &age) && state;
	printf("------------");
	fflush(stdout);
	if(state){
		addNode(name, surname, creation, age);
	} else {
		printf("\nWrong input!");
	}
    free(name);
    free(surname);
    free(creation);
}

void menu(){
	fflush(stdout);
	int choice=0, state=0;
	printf("\n****MENU****");
	printf("\n1- Add a Person to the Stack");
	printf("\n2- Pop a Person from the Stack");
	printf("\n3- Sort in Alphabetical Order");
	printf("\n4- Sort Age in Increasing Order");
	printf("\n5- Exit menu");
	printf("\n************");
	printf("\nSelect your choice: ");
	state = scanf("%d", &choice);

	if(state == 1){
		switch(choice){
			case 1:
				//add_test();
				get_person();
				break;
			case 2:
				delete_node();
				break;
			case 3:
				sort_by_name();
				break;
			case 4:
				sort_by_age();
				break;
			case 5:
        return;
				break;
			default:
				printf("Wrong input! Please try again.\n");
				break;
		}
	} else {
		printf("Wrong input! Exit the program.\n");
		return;
	}
    menu();
}

int main(){
	top = NULL;
	menu();
	return 0;
}

void add_test(){
	char * name = (char*) malloc(sizeof(char)*SIZE);
	char * surname = (char*) malloc(sizeof(char)*SIZE);
	char * creation = (char*) malloc(sizeof(char)*SIZE);
	int age;

	name = "Cemal Reşit";
	surname = "Rey";
	creation = "Lüküs Hayat";
	age = 80;
	addNode(name, surname, creation, age);

	name = "Ahmed Adnan";
	surname = "Saygun";
	creation = "Yunus Emre";
	age = 83;
	addNode(name, surname, creation, age);

	name = "Ulvi Cemal";
	surname = "Erkin";
	creation = "Keloğlan";
	age = 66;
	addNode(name, surname, creation, age);

	name = "Hasan Ferit";
	surname = "Alnar";
	creation = "Sarı Zeybek";
	age = 72;
	addNode(name, surname, creation, age);

	name = "Necil Kazım";
	surname = "Akses";
	creation = "İkinci Senfoni";
	age = 90;
	addNode(name, surname, creation, age);

	name = "Cemal Reşit";
	surname = "Zey";
	creation = "Lüküs Hayat";
	age = 80;
	addNode(name, surname, creation, age);
}
