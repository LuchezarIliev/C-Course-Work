//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <ctype.h>

int fileToFileFunction();
int fileToScreenFunction();
int keyToFileFunction();
int keyToScreenFunction();

int resultFunction(char *textArray, char *identifier,
	int *flagMultiLine, int *flagString, int *flagIdentifier, int *flagOther,
	int *countSingleComments, int *countMultiLineComments, int *countStrings,
	int *countSymbols);
int isLetter(char textArray);
int isDigit(char textArray);
int validFileName(char *fileNameRead);
int validFileName2(char *fileNameWrite);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choice;
	int k;

	do {
		printf("Меню с опции:\n\n");
		printf("\t|1| Четене на програма от файл и запис на резултата във файл.\n");
		printf("\t|2| Четене на програма от файл и извеждане резултата на екрана.\n");
		printf("\t|3| Четене на програма от клавиатура и извеждане във файл.\n");
		printf("\t|4| Четене на програма от клавиатура и извеждане на екрана.\n");
		printf("\t|5| Изход.\n");
		printf("Въведете числото [1-5] на избраната опция: ");
		do {
			k = scanf("%d", &choice);
			while (getchar() != '\n');
			if (choice < 1 || choice > 5)
				printf("\tГрешка! Въведете ново число:\n");
		} while (k != 1);

		switch (choice)
		{
		case 1:
			fileToFileFunction();
			break;
		case 2:
			fileToScreenFunction();
			break;
		case 3:
			keyToFileFunction();
			break;
		case 4:
			keyToScreenFunction();
			break;
		}
	} while (choice != 5);

	return 0;
}

int fileToFileFunction()
{
	char textArray[256], identifier[256];
	int countSingleComments = 0, countMultiLineComments = 0;
	int commentsTotalCount = 0;
	int countStrings = 0, countSymbols = 0;
	int flagMultiLine = 0, flagString = 0, flagIdentifier = 0, flagOther = 0;

	char fileNameRead[256], fileNameWrite[256];
	FILE *fpR, *fpW;

	do
	{
		printf("Въведете името на файла за четене (трябва да завършва с \".C\"):\n");
		scanf("%s", fileNameRead);
		while (getchar() != '\n');
	} while (!validFileName(fileNameRead));
	if ((fpR = fopen(fileNameRead, "r")) == NULL)
	{
		printf("\tГрешка при отварянето на файла!\n");
		return 1;
	}

	do
	{
		printf("Въведете името на файла за запис (трябва да завършва с \".C\" или \".txt\"):\n");
		scanf("%s", fileNameWrite);
		while (getchar() != '\n');
	} while (!validFileName2(fileNameWrite));
	if ((fpW = fopen(fileNameWrite, "w+")) == NULL)
	{
		printf("\tГрешка при отварянето на файла!\n");
		return 1;
	}

	while (fgets(textArray, 255, fpR) != NULL)
	{
		commentsTotalCount = resultFunction(textArray, identifier,
			&flagMultiLine, &flagString, &flagIdentifier, &flagOther,
			&countSingleComments, &countMultiLineComments, &countStrings,
			&countSymbols);
	}
	if (ferror(fpR))
	{
		perror("Грешка!");
		exit(1);
	}

	fprintf(fpW, "Броят на коментарите в програмата е: %d\n", commentsTotalCount);
	fprintf(fpW, "Общият брой на символите в идентификаторите на програмата е: %d\n\n", countSymbols);
	if (ferror(fpW))
	{
		perror("Грешка!");
		exit(1);
	}
	_fcloseall();
	return 0;
}

int fileToScreenFunction()
{
	char textArray[256], identifier[256];
	int countSingleComments = 0, countMultiLineComments = 0;
	int commentsTotalCount = 0;
	int countStrings = 0, countSymbols = 0;
	int flagMultiLine = 0, flagString = 0, flagIdentifier = 0, flagOther = 0;

	char fileNameRead[256];
	FILE *fpR;

	do
	{
		printf("Въведете името на файла за четене (трябва да завършва с \".C\"):\n");
		scanf("%s", fileNameRead);
		while (getchar() != '\n');
	} while (!validFileName(fileNameRead));

	if ((fpR = fopen(fileNameRead, "r")) == NULL)
	{
		printf("\tГрешка при отварянето на файла!\n");
		return 1;
	}

	while (fgets(textArray, 255, fpR) != NULL)
	{
		commentsTotalCount = resultFunction(textArray, identifier,
			&flagMultiLine, &flagString, &flagIdentifier, &flagOther,
			&countSingleComments, &countMultiLineComments, &countStrings,
			&countSymbols);
	}
	if (ferror(fpR))
	{
		perror("Грешка!");
		exit(1);
	}
	fclose(fpR);

	printf("Броят на коментарите в програмата е: %d\n", commentsTotalCount);
	printf("Общият брой на символите в идентификаторите на програмата е: %d\n\n", countSymbols);
	return 0;
}

int keyToFileFunction()
{
	char textArray[256], identifier[256];
	int countSingleComments = 0, countMultiLineComments = 0;
	int commentsTotalCount = 0;
	int countStrings = 0, countSymbols = 0;
	int flagMultiLine = 0, flagString = 0, flagIdentifier = 0, flagOther = 0;

	char fileNameWrite[256];
	FILE *fpW;

	do
	{
		printf("Въведете името на файла за запис (трябва да завършва с \".C\" или \".txt\"):\n");
		scanf("%s", fileNameWrite);
		while (getchar() != '\n');
	} while (!validFileName2(fileNameWrite));
	if ((fpW = fopen(fileNameWrite, "w+")) == NULL)
	{
		printf("\tГрешка при отварянето на файла!\n");
		return 1;
	}

	printf("Напишете програмата за четене от вашата клавиатура. За край CTR/Z!\n");
	while (fgets(textArray, 255, stdin) != NULL)
	{
		commentsTotalCount = resultFunction(textArray, identifier,
			&flagMultiLine, &flagString, &flagIdentifier, &flagOther,
			&countSingleComments, &countMultiLineComments, &countStrings,
			&countSymbols);
	}
	if (ferror(stdin))
	{
		perror("Грешка!");
		exit(1);
	}

	fprintf(fpW, "Броят на коментарите в програмата е: %d\n", commentsTotalCount);
	fprintf(fpW, "Общият брой на символите в идентификаторите на програмата е: %d\n\n", countSymbols);
	if (ferror(fpW))
	{
		perror("Грешка!");
		exit(1);
	}
	fclose(fpW);
	return 0;
}

int keyToScreenFunction()
{
	char textArray[256], identifier[256];
	int countSingleComments = 0, countMultiLineComments = 0;
	int commentsTotalCount = 0;
	int countStrings = 0, countSymbols = 0;
	int flagMultiLine = 0, flagString = 0, flagIdentifier = 0, flagOther = 0;

	printf("Напишете програмата за четене от вашата клавиатура. За край CTR/Z!\n");
	while (fgets(textArray, 255, stdin) != NULL)
	{
		commentsTotalCount = resultFunction(textArray, identifier,
			&flagMultiLine, &flagString, &flagIdentifier, &flagOther,
			&countSingleComments, &countMultiLineComments, &countStrings,
			&countSymbols);
	}
	if (ferror(stdin))
	{
		perror("Грешка!");
		exit(1);
	}

	printf("Броят на коментарите в програмата е: %d\n", commentsTotalCount);
	printf("Общият брой на символите в идентификаторите на програмата е: %d\n\n", countSymbols);

	return 0;
}

int resultFunction(char *textArray, char *identifier,
	int *flagMultiLine, int *flagString, int *flagIdentifier, int *flagOther,
	int *countSingleComments, int *countMultiLineComments, int *countStrings,
	int *countSymbols)
{
    int i, j;
	for (i = 0, j = 0; textArray[i]; i++)
	{
		if (*flagMultiLine == 0 && *flagString == 0 && *flagIdentifier == 0 && *flagOther == 0)
		{
			if (textArray[i] == '/')
			{
				if (textArray[i + 1] == '/')
				{
					(*countSingleComments)++;
					break;
				}
				if (textArray[i + 1] == '*')
				{
					*flagMultiLine = 1;
					(*countMultiLineComments)++;
				}
			}
			if (textArray[i] == '\"')
			{
				*flagString = 1;
			}

			if (textArray[i] == '_' || isLetter(textArray[i]))
			{
				*flagIdentifier = 1;
				identifier[j++] = textArray[i];
				(*countSymbols)++;
			}
			else
			{
				if (!isspace(textArray[i]))
					*flagOther = 1;
			}
		}
		else if (*flagMultiLine == 1)
		{
			if (textArray[i] == '*' && textArray[i + 1] == '/')
			{
				*flagMultiLine = 0;
			}
		}
		else if (*flagString == 1)
		{
			if (textArray[i] == '\"' && textArray[i - 1] != '\\')
			{
				(*countStrings)++;
				*flagString = 0;
			}
		}
		else if (*flagOther == 1)
		{
			if (isspace(textArray[i]))
				*flagOther = 0;
		}

		else
		{
			if (*flagMultiLine == 0 && *flagString == 0)
			{
				if (textArray[i] == '_' || isLetter(textArray[i]) || isDigit(textArray[i]))
				{
					identifier[j++] = textArray[i];
					(*countSymbols)++;
				}
				else
				{
					*flagIdentifier = 0;
					identifier[j] = '\0';
					j = 0;
					//printf("%s\n", identifier);
				}
			}
		}
	}
	return *countSingleComments + *countMultiLineComments;
}

int isLetter(char textArray)
{
	short k;
	k = textArray;
	if ((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z'))
		return 1;
	if ((k >= 'а' && k <= 'я') || (k >= 'А' && k <= 'Я'))
		return 1;
	return 0;
}

int isDigit(char textArray)
{
	if (textArray >= '0' && textArray <= '9')
		return 1;
	return 0;
}

int validFileName(char *fileNameRead)
{
	if (strlen(fileNameRead) < 3)
	{
		printf("\tИмето на файла е прекалено кратко!\n");
		return 0;
	}
	if ((fileNameRead[strlen(fileNameRead) - 2] == '.') && (fileNameRead[strlen(fileNameRead) - 1] == 'C' || fileNameRead[strlen(fileNameRead) - 1] == 'c'))
		return 1;
	else
		printf("\tФайлът не завършва с \".C\"!\n");
	return 0;
}

int validFileName2(char *fileNameWrite)
{
	if (strlen(fileNameWrite) < 3)
	{
		printf("\tИмето на файла е прекалено кратко!\n");
		return 0;
	}
	if ((fileNameWrite[strlen(fileNameWrite) - 2] == '.') && (fileNameWrite[strlen(fileNameWrite) - 1] == 'C' || fileNameWrite[strlen(fileNameWrite) - 1] == 'c'))
		return 1;
	else if (fileNameWrite[strlen(fileNameWrite) - 4] == '.' && fileNameWrite[strlen(fileNameWrite) - 3] == 't' && fileNameWrite[strlen(fileNameWrite) - 2] == 'x' && fileNameWrite[strlen(fileNameWrite) - 1] == 't')
		return 1;
	else if (fileNameWrite[strlen(fileNameWrite) - 4] == '.' && fileNameWrite[strlen(fileNameWrite) - 3] == 'T' && fileNameWrite[strlen(fileNameWrite) - 2] == 'X' && fileNameWrite[strlen(fileNameWrite) - 1] == 'T')
		return 1;
	else
		printf("\tФайлът не завършва с \".C\" или \".txt\"!\n");
	return 0;
}
