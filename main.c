#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100

/**
@brief: structure representing the nodes of the doubly linked list
@param1: pointer to structure to previous node
@param2: int count to represent the number of visits to a specific web page
@param4: pointer to structure to next node
*/
struct NODE
{
    struct NODE *prev;
    int count;
    char *name;
    struct NODE* next;

};

/**
@brief: structure representing the doubly linked list
@param1: pointer to structure to head node
@param2: int length to represent the number web pages opened
*/
struct doubleLinkedList
{
    struct NODE* head;
    int length;
};

/**
@brief:function to search for a node in the doubly linked list
@param1: pointer to structure to doubly linked list
@param2: char array name to compare to the elements of the list
@returns: pointer to structure to the node with the same name
*/
struct NODE *searchNode(struct doubleLinkedList *list, char* name);

/**
@brief:function to create a doubly linked list
@returns: pointer to structure to newly created doubly linked list
*/
struct doubleLinkedList* createLinkedList();

/**
@brief: function to add a node in the doubly linked list
@param1: pointer to structure to doubly linked list
@param2: pointer to char array representing the name of the node
@param3: int that shows the max number of calls that can be made to a website
@param4: int that shows the max size of the buffer (doubly linked list)
@returns: nothing. void function.
*/
void addNode(struct doubleLinkedList *list, char *name, int maxCall, int maxSize);

/**
@brief:function to make a node the new head of the doubly linked list
@param1: pointer to structure to doubly linked list
@param2: pointer to structure to node that is going to be the new head
@returns: nothing. void function.
*/
void headify(struct doubleLinkedList *list, struct NODE* newHead);

/**
@brief:function to delete the last node of the doubly linked list after the # of web pages exceeds maxSize
@param1: pointer to structure to doubly linked list
@returns: nothing. void function.
*/
void deleteNode(struct doubleLinkedList *list);

/**
@brief:function to print the doubly linked list after every new addition to it
@param1: pointer to structure to doubly linked list
@returns: nothing. void function.
*/
void printList(struct doubleLinkedList *list);

/**
@brief:function to empty the doubly linked list after the user finishes browsing the net
@param1: pointer to structure to doubly linked list
@returns: nothing. void function.
*/
void emptyList(struct doubleLinkedList *list);

int main()
{
    int maxSize, maxCall; //max number of calls to a website and of buffer size
    char choice;

    struct doubleLinkedList *list = createLinkedList(); //creation of new doubly linked list

    printf("Enter max size of buffer: ");
    scanf("%d", &maxSize);
    if(maxSize <= 0){
        printf("ERROR: size must be bigger than 0!\n");
        exit(1);
    }

    printf("Enter max call that can be made to a website: ");
    scanf("%d", &maxCall);
    if(maxCall <= 0){
        printf("ERROR: call must be bigger than 0!\n");
        exit(1);
    }

    char *website = (char*)malloc(SIZE * sizeof(char));
    if(website == NULL){
        printf("Memory allocation ERROR: website names string"); //if failing to allocate memory
        exit(1);
    }

    getchar(); // skip past the newline character that scanf left before
    printf("Enter the website's name or '0' if you want to end browsing the Internet: ");

    while(fgets(website, SIZE, stdin) != NULL && strcmp(website, "0\n") != 0){
        website[strcspn(website, "\n")] = '\0'; //removing the end of string character

        addNode(list, website, maxCall, maxSize); //call to addNode function

        printList(list); //call to printList function
        printf("Enter the website's name or '0' if you want to end browsing the Internet: ");
    }

    printf("Do you want to empty the buffer or not?\n");
    printf("(Y)es // (N)o: "); // the user decides whether to empty the list or not
    scanf(" %c", &choice);

    if(choice == 'Y' || choice == 'y'){
        emptyList(list);
    }
    free(website); // freeing the allocated memory for website

    return 0;
}

struct doubleLinkedList* createLinkedList()
{
    struct doubleLinkedList *list = (struct doubleLinkedList*)malloc(sizeof(struct doubleLinkedList)); // new linked list

    if(list == NULL){
        printf("Memory allocation ERROR: double linked list"); //failing to allocate memory
    }
    list->head = NULL; // initialize head to null
    list->length = 0; // initialize length to 0
    return list;
}

void addNode(struct doubleLinkedList *list, char *name, int maxCall, int maxSize)
{
    struct NODE *newNode = (struct NODE*)malloc(sizeof(struct NODE));
    if(newNode == NULL){
        printf("Memory allocation ERROR: new node creation while adding it to the list"); //failing to allocate memory
        exit(0);
    }

    struct NODE *curr;

    if((curr = searchNode(list, name)) != NULL){ //if node is already present in the buffer, increment its count
        curr->count++;
        if(curr->count > maxCall){
            headify(list, curr); // call to headify function
        }
    }
    else{
        newNode->prev = NULL; //creating a new node with the given data
        newNode->count = 1;
        newNode->name = strdup(name);
        newNode->next = NULL;

        if(list->head == NULL){
            list->head = newNode; //if list is empty make the newNode the new head
        }
        else{
            list->head->prev = newNode;
            newNode->next = list->head;
            list->head = newNode; // head of list becomes newNode
        }
        list->length++; // incrementing the buffer size
    }
    if(list->length > maxSize){
        deleteNode(list); // if buffer size gets bigger than the allowed max, delete the last node of the doubly linked list
    }
}

struct NODE *searchNode(struct doubleLinkedList *list, char* name)
{
    struct NODE *curr = list->head;

    while(curr != NULL && strcmp(curr->name, name) != 0){
        curr = curr->next; // simple function to search for a node in the list
    }

    return curr;
}

void headify(struct doubleLinkedList *list, struct NODE* newHead)
{
    struct NODE *prev = newHead->prev; // node previous to the newHead
    struct NODE *next = newHead->next; // node after the newHead

    if(newHead == NULL || list == NULL){
        printf("Invalid arguments!\n");
        return;
    }

    if(list->head == newHead || newHead->prev == NULL){ // if the list is empty or newHead is already the head no need to update
        return;
    }


    if(next != NULL){
        next->prev = prev; // remove newHead from its current position
    }

    if(prev != NULL){
        prev->next = next;
    }

    newHead->prev = NULL;  // Set newHead as the new head
    newHead->next = list->head;

    if(list->head != NULL){
        list->head->prev = newHead;
    }

    list->head = newHead;
}

void deleteNode(struct doubleLinkedList *list)
{
    if(list == NULL || list->head == NULL){
        printf("List already empty!\n");
        return;
    }

    struct NODE *curr = list->head;

    while(curr->next != NULL){
        curr = curr->next; // iterating through the linked list until the last element in encountered
    }

    if(curr->prev != NULL){
        curr->prev->next = NULL;
    }

    free(curr->name); // free the memory allocated for name
    free(curr);
    list->length--; // decrease the size of the buffer

}

void printList(struct doubleLinkedList *list)
{
    struct NODE *curr;

    printf("\nList: ");
    for(curr = list->head; curr != NULL; curr = curr->next){ // for loop to iterate through the doubly linked list
        printf("|%5s |%2d|->", curr->name, curr->count);
    }
    printf("\n");

}

void emptyList(struct doubleLinkedList *list)
{
    if(list == NULL || list->head == NULL){
        printf("List already empty!\n");
        return;
    }

    struct NODE *curr = list->head;
    struct NODE *nextNode;

    while(curr != NULL){
        nextNode = curr->next;
        free(curr->name); // free the memory for the allocated name
        free(curr); // free the current node
        curr = nextNode;
    }

    list->head = NULL; // assign null to the head of the list
    list->length = 0; // make the size of the list 0

    printf("List emptied successfully!\n");
    printList(list); // call to printList to show that the buffer is really empty or not
}
