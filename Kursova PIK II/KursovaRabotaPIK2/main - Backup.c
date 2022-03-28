#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct var
{
    char code[20];
    char name[50];
    double balance;
    int months;
    struct var *next;
};
typedef struct var var;

var *createStack(double *averageBalance);
void addNewClient();
void writeToBin(var *head);
void newItem(var **currElement, var *data);
void printData(var *head);
void printByCode(var *head, char *passedCode);
var *deleteByCode(var *head, char *passedCode);
void printByBalance(var *head, double averageBalance);
void freeMemory(var *head);

int main()
{
    int choice, k;
    char passedCode[20];
    double averageBalance = 0;
    var *head = NULL;

	do {
		printf("\t\t\tMenu:\n\n");
		printf("\t|1| Enter data for new client \n");
		printf("\t|2| Find client by code \n");
		printf("\t|3| Delete client by code\n");
		printf("\t|4| Show clients with balance > averageBalance \n");
		printf("\t|5| Exit\n\n");
		printf("Choose [1-5]: ");
		do {
			k = scanf("%d", &choice);
			while (getchar() != '\n');
			if (choice < 1 || choice > 5)
				printf("\tError! Try again:\n");
		} while (k != 1);
        system("CLS");
		switch (choice)
		{
		case 1:
		    printf("|1| Enter data for new client:\n");
		    addNewClient();
			break;
		case 2:
		    printf("|2| Find client by code: ");
            head = createStack(&averageBalance);
            fgets(passedCode, 20, stdin);
            if ((strlen(passedCode) > 0) && (passedCode[strlen (passedCode) - 1] == '\n'))
                passedCode[strlen (passedCode) - 1] = '\0';
            printByCode(head, passedCode);
			break;
		case 3:
		    printf("|3| Delete client by code: ");
            head = createStack(&averageBalance);
            fgets(passedCode, 20, stdin);
            if ((strlen(passedCode) > 0) && (passedCode[strlen (passedCode) - 1] == '\n'))
                passedCode[strlen (passedCode) - 1] = '\0';
            printf("\n\tBefore delete:\n");
            printData(head);
            head = deleteByCode(head, passedCode);
            printf("\n\tAfter delete:\n");
            printData(head);
            writeToBin(head);
			break;
		case 4:
            averageBalance = 0;
            head = createStack(&averageBalance);
		    printf("|4| Show clients with balance > averageBalance[%g]:\n", averageBalance);
		    printByBalance(head, averageBalance);
			break;
		}
	} while (choice != 5);
    freeMemory(head);
    head = NULL;
    return 0;
}

var *createStack(double *averageBalance)
{
    var *head = NULL;
    var *currentItem = NULL;
    var data;
    int count = 0;
    double sum = 0;

    FILE *fp;
    if((fp = fopen("clients.bin", "rb")) == NULL)
    {
        printf("Error! File not found/does not exist...\n");
        exit(1);
    }
    while(fread(&data, sizeof(var), 1, fp))
    {
        sum += data.balance;
        count++;
        newItem(&currentItem, &data);
        currentItem->next = head;
        head = currentItem;
    }
    if(count > 0)
        *averageBalance += (sum/count);
    fclose(fp);
    return head;
}

void newItem(var **currElement, var *data)
{
    *currElement = (var *)malloc(sizeof(var));
    if(*currElement == NULL)
    {
        printf("Error! Exiting program...\n");
        exit(1);
    }
    strcpy((*currElement)->code, data->code);
    strcpy((*currElement)->name, data->name);
    (*currElement)->balance = data->balance;
    (*currElement)->months = data->months;

    (*currElement)->next = NULL;
}

void addNewClient()
{
    var client;
    FILE *fp;
    if((fp=fopen("clients.bin", "ab"))==NULL)
    {
        printf("Error! Can not open/create file for write...\n");
        exit(1);
    }
    printf("Enter code: ");
    fgets(client.code, 20, stdin);
    if ((strlen(client.code) > 0) && (client.code[strlen(client.code) - 1] == '\n'))
        client.code[strlen(client.code) - 1] = '\0';
    printf("Enter name: ");
    fgets(client.name, 50, stdin);
    if ((strlen(client.name) > 0) && (client.name[strlen(client.name) - 1] == '\n'))
        client.name[strlen(client.name) - 1] = '\0';
    printf("Enter balance: ");
    scanf("%lf", &client.balance);
    printf("Enter months: ");
    scanf("%d", &client.months);
    if(fwrite(&client, sizeof(var), 1, fp)!=1)
    {
        printf("Error writing new client!\nExiting...");
        exit(1);
    }
    fclose(fp);
}

void writeToBin(var *head)
{
    FILE *fp;
    if((fp=fopen("clients.bin", "wb"))==NULL)
    {
        printf("Error! Can not open/create file for write...\n");
        exit(1);
    }
    var *currElement = NULL;
    for(currElement = head; currElement != NULL; currElement = currElement->next)
    {
        if(fwrite(&(*currElement), sizeof(var), 1, fp)!=1)
        {
            printf("Error writing new client!\nExiting...");
            exit(1);
        }
    }
    fclose(fp);
}

void printData(var *head)
{
    var *currElement = NULL;
    for(currElement = head; currElement != NULL; currElement = currElement->next)
    {
        printf("\nCode: %s\n", currElement->code);
        printf("Name: %s\n", currElement->name);
        printf("Balance: %g\n", currElement->balance);
        printf("Months: %d\n", currElement->months);
    }
}

void printByCode(var *head, char *passedCode)
{
    var *currElement = NULL;
    for(currElement = head; currElement != NULL; currElement = currElement->next)
    {
        if(!strcmp((currElement->code), passedCode))
        {
            printf("\nCode: %s\n", currElement->code);
            printf("Name: %s\n", currElement->name);
            printf("Balance: %g\n", currElement->balance);
            printf("Months: %d\n", currElement->months);
        }
    }
}

var *deleteByCode(var *head, char *passedCode)
{
    var *prevElement=head;
    var *currElement=head;
    int flag = 0;
    while(currElement!=NULL)
    {
        if(!strcmp((currElement->code), passedCode))
        {
            if(currElement == head)
            {
                head = head->next;
                prevElement = head;
                flag = 1;
            }
            else
                prevElement->next = currElement->next;
            free(currElement);
            currElement = prevElement;
        }
        prevElement = currElement;
        if(flag == 0)
            currElement = currElement->next;
        flag = 0;
    }
    return head;
}


void printByBalance(var *head, double averageBalance)
{
    var *currElement = NULL;
    for(currElement = head; currElement != NULL; currElement = currElement->next)
    {
        if(currElement->balance > averageBalance)
        {
            printf("\nCode: %s\n", currElement->code);
            printf("Name: %s\n", currElement->name);
            printf("Balance: %g\n", currElement->balance);
            printf("Months: %d\n", currElement->months);
        }
    }
}

void freeMemory(var *head)
{
    var *currElement = head;
    while(head != NULL)
    {
        head = head->next;
        free(currElement);
        currElement = head;
    }
}
