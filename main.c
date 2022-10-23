#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <time.h>

#define QUESTIONS_PATH "questions.vd"
#define QUANTITY_QUESTIONS 21

typedef struct
{

    char question[200];
    char alternative1[100];
    char alternative2[100];
    char alternative3[100];
    char correct;

} tQuestions;

int aux = 0;

int randomQuestion(int limite, int *val_used)
{
    srand(time(NULL));

    int val;

    val = rand() % limite;

    if(aux == 0)
    {
        for(int i = 0; i < QUANTITY_QUESTIONS; i++)
        {
            //deixando todos os valores como nulos para que nao haja erro.
            val_used[i] = -1;
        }
    }

    for(int j = 0; j < QUANTITY_QUESTIONS; j++)
    {
        if(val == val_used[j])
        {
            val = rand() % limite;
            j = 0;
        }
    }

    val_used[aux] = val;

    aux++;

    return val;
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
            break;
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

char transformAnswer(char answer)
{
    switch(answer)
    {
    case 'a':
        return '1';
    case 'A':
        return '1';
    case 'b':
        return '2';
    case 'B':
        return '2';
    case 'c':
        return '3';
    case 'C':
        return '3';
    default:
        return '0';
    }
}

int answerValidator(tQuestions *qs, int question, char answer)
{

    char auxAnswer = transformAnswer(answer);

    return qs[question].correct == auxAnswer ? 1 : 0;
}

int main()
{

    setlocale(LC_ALL, "Portuguese");

    int jogando = 1;
    int rounds = 1;

    int val_used[QUANTITY_QUESTIONS];

    tQuestions qs[QUANTITY_QUESTIONS];
    setUpQuestions(qs, QUANTITY_QUESTIONS - 1);

    int chosenDoor = 0;
    int question = 0;
    int questionDoor1;// = randomQuestion(QUANTITY_QUESTIONS - 1);
    int questionDoor2;// = randomQuestion(QUANTITY_QUESTIONS - 1);
    int questionDoor3;// = randomQuestion(QUANTITY_QUESTIONS - 1);

    char answer;

    do
    {

        questionDoor1 = randomQuestion(QUANTITY_QUESTIONS - 1, val_used);
        questionDoor2 = randomQuestion(QUANTITY_QUESTIONS - 1, val_used);
        questionDoor3 = randomQuestion(QUANTITY_QUESTIONS - 1, val_used);

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

        printf("\n\n%d\n\n\n", question);

        printf("\n\n%s\n", qs[question].question);
        printf("\n%s\n", qs[question].alternative1);
        printf("%s\n", qs[question].alternative2);
        printf("%s\n", qs[question].alternative3);

        printf("Escolha sua resposta (a, b ou c): ");
        scanf("%c", &answer);

        if(!answerValidator(qs, question, answer))
        {
            printf("\nResposta errada!\n");
            //jogando = 0;

            if(rounds > QUANTITY_QUESTIONS / 3)
            {
                jogando = 0;
            }

            rounds++;
        }
        else
        {
            printf("\nResposta Correta!");

            if(rounds >= QUANTITY_QUESTIONS / 3)
            {
                jogando = 0;
            }

            rounds++;

            getch();
        }

    }while(jogando);


    return 0;
}
