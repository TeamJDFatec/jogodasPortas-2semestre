#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>

#define QUESTIONS_PATH "questions.vd"

typedef struct
{

    char question[200];
    char alternative1[100];
    char alternative2[100];
    char alternative3[100];
    int correct;

} tQuestions;


int randomQuestion(int limite)
{
    return rand() % limite;
}

void splitData(char *line, int line_tam, tQuestions *question, int qsLine)
{
    int countDelimiter = 0;
    int auxCount = 0;



    for(int i = 0; i < line_tam; i++)
    {
        if(line[i] == ';')
        {
            auxCount = 0;
            countDelimiter ++;
            continue;
        }

        if(countDelimiter == 0)
        {
            question[qsLine].question[auxCount] = line[i];
        }

        if(countDelimiter == 1)
        {
            question[qsLine].alternative1[auxCount] = line[i];
        }

        if(countDelimiter == 2)
        {
            question[qsLine].alternative2[auxCount] = line[i];
        }

        if(countDelimiter == 3)
        {
            question[qsLine].alternative3[auxCount] = line[i];
        }

        if(countDelimiter == 4)
        {
            question[qsLine].correct = line[i];
        }

        auxCount ++;
    }

}

void cleanQuestions(tQuestions *qs, int lines)
{
    int len = 0;

    for(int i = 0; i < lines; i++)
    {
        len = sizeof(qs->question);
        for(int j = 0; j < len; j++)
        {
            qs[i].question[j] = '\0';
        }

        len = sizeof(qs->alternative1);
        for(int j = 0; j < len; j++)
        {
            qs[i].alternative1[j] = '\0';
        }

        len = sizeof(qs->alternative2);
        for(int j = 0; j < len; j++)
        {
            qs[i].alternative2[j] = '\0';
        }

        len = sizeof(qs->alternative3);
        for(int j = 0; j < len; j++)
        {
            qs[i].alternative3[j] = '\0';
        }

    }

}

void setUpQuestions(tQuestions *qs, int totalQuestions)
{
    FILE *arq;
    int tam = sizeof(tQuestions);
    char arqLine[tam];
    int qsLine = 0;

    arq = fopen(QUESTIONS_PATH, "r");

    if (arq == NULL)
    {
        printf("Erro ao abrir arquivo");
    }
    else
    {
        cleanQuestions(qs, totalQuestions);

        fgets(arqLine, tam, arq);
        splitData(arqLine, tam, qs, qsLine);
        qsLine++;

        if(ferror(arq))
        {
            printf("Erro ao ler arquivo");
        }
        else
        {

            do
            {
                fgets(arqLine, tam, arq);
                splitData(arqLine, tam, qs, qsLine);
                qsLine++;

            }
            while(qsLine <= totalQuestions);

        }

        fclose(arq);
    }
}

int charToInt(char answer)
{
    switch(answer)
    {
    case 'a':
        return 1;
    case 'A':
        return 1;
    case 'b':
        return 2;
    case 'B':
        return 2;
    case 'c':
        return 3;
    case 'C':
        return 3;
    default:
        return 0;
    }
}

int main()
{

    setlocale(LC_ALL, "Portuguese");

    tQuestions qs[10];
    setUpQuestions(qs, 9);

    int chosenDoor = 0;
    int question = 0;
    int questionDoor1 = randomQuestion(9);
    int questionDoor2 = randomQuestion(9);
    int questionDoor3 = randomQuestion(9);

    char answer;

    do
    {
        system("cls");
        printf("\nEscolha uma Porta (1, 2 ou 3):\n");
        scanf("%d", &chosenDoor);
        fflush(stdin);

        chosenDoor = chosenDoor != 1 && chosenDoor != 2 && chosenDoor != 3 ? 0 : chosenDoor;

        if(chosenDoor == 0)
        {
            printf("Porta inválida! Clique em uma tecla para continuar");
            getch();
        }

    }while(chosenDoor == 0);

    switch(chosenDoor)
    {
    case(1):
        question = questionDoor1;
        break;
    case(2):
        question = questionDoor2;
        break;
    case(3):
        question = questionDoor3;
        break;
    default:
        printf("Erro!");
        break;
    }

    printf("\n\n%s\n", qs[question].question);
    printf("\n%s\n", qs[question].alternative1);
    printf("%s\n", qs[question].alternative2);
    printf("%s\n", qs[question].alternative3);

    printf("Escolha sua resposta (a, b ou c): ");
    gets(answer);




    printf("%d\n", qs[question].correct);

    return 0;
}
