#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dataStruct
{
    char code[20];
    char name[50];
    double balance;
    int months;
};
typedef struct dataStruct var;

int main()
{
    FILE *fp;
    var data;


    if((fp=fopen("client.bin", "rb"))==NULL)
    {
        printf("Error!\n");
        exit(1);
    }

    while(fread(&data, sizeof(var), 1, fp))
    {
        printf("\nCode: %s strlen: %d\n", data.code, strlen(data.code));
        printf("Name: %s strlen: %d\n", data.name, strlen(data.name));
        printf("Balance: %g\n", data.balance);
        printf("Months: %d\n", data.months);
    }

/*
    if((fp=fopen("client.bin", "ab"))==NULL)
    {
        printf("Error!\n");
        exit(1);
    }

    printf("Enter code: ");
    fgets(data.code, 20, stdin);
    if ((strlen(data.code) > 0) && (data.code[strlen(data.code) - 1] == '\n'))
        data.code[strlen(data.code) - 1] = '\0';


    printf("Enter name: ");
    fgets(data.name, 50, stdin);
    if ((strlen(data.name) > 0) && (data.name[strlen(data.name) - 1] == '\n'))
        data.name[strlen(data.name) - 1] = '\0';

    printf("Enter balance: ");
    scanf("%lf", &data.balance);
    printf("Enter months: ");
    scanf("%d", &data.months);

    fwrite(&data, sizeof(var), 1, fp);

    printf("\nCode: %s %d\n", data.code, strlen(data.code));
    printf("Name: %s %d\n", data.name, strlen(data.name));
    printf("Balance: %g\n", data.balance);
    printf("Months: %d\n", data.months);

*/


    fclose(fp);
    return 0;
}
