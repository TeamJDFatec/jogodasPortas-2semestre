#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>

#define QUESTIONS_PATH "questions.vd"
#define POINT_RATE 100
#define QUANTITY_QUESTIONS 21

#define TAM_PORTAS 71
#define TAM_PORTAS_ABERTA 55

typedef struct
{

    char question[200];
    char alternative1[100];
    char alternative2[100];
    char alternative3[100];
    char correct;

} tQuestions;

typedef struct
{
    char playerName[100];
    int score;

} tRanking;

int questionAlreadyViewed(int *viwedQuestions, int question, int round)
{

    for(int i = 0; i < round; i++)
    {
        if(question == viwedQuestions[i])
        {
            return 1;
        }
    }

    return 0;
}

int randomQuestion(int limite, int *qs_used, int round)
{
    srand(time(NULL));
    int qs;

    do
    {
        qs = rand() % limite;

    }while(questionAlreadyViewed(qs_used, qs, round));

    return qs;
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

void tutorial()
{
    printf("Tutorial...\n");
    system("pause");
}


void confirmExit()
{
    char confirm;

    do
    {
        system("cls");
        printf("\nTem certeza que deseja sair? (s/n)");

        confirm = tolower(getch());

        if(confirm != 's' && confirm != 'n')
        {
            printf("\nDigite um valor válido por favor!\n");
            system("pause");
        }

    }while(confirm != 's' && confirm != 'n');

    if(confirm == 's')
    {
        exit(1);
    }
}

void menu()
{
    int choice;
    int bContinue = 1;

    do
    {
        system("cls");

        printf("****** Jogo das Portas ******\n\n\n");

        printf("\t1 - Jogar\n");
        printf("\t2 - Tutorial\n");
        printf("\t3 - Sair\n");

        fflush(stdin);
        scanf("%d", &choice);

        if(choice == 3)
        {
            confirmExit();
        }

        switch(choice)
        {
            case 1:
                bContinue = 0;
                break;
            case 2:
                tutorial();
                bContinue = 0;
                break;
            default:
                bContinue = 1;
                break;
        }

    }while(bContinue == 1);
}

void screen(FILE *map, int size)
{
    char doors[size];

    system("cls");

    fgets(doors, size, map);
    while(!feof(map))
    {
        printf("%s", doors);
        fgets(doors, size, map);
    }
    printf("%s", doors);
    printf("\n\n");
}

void openDoor(int chosenDoor)
{
    FILE *doors;

    switch(chosenDoor)
    {
        case 1:
            //abre o arquivo onde esta desenhada a porta 1 e assim vai com os demais.
            doors = fopen("porta_aberta1.txt", "r");
            if(doors == NULL)
            {
                printf("Erro ao abrir a imagem da porta aberta!");
            }
            else
            {
                screen(doors, TAM_PORTAS_ABERTA);
            }
            break;
        case 2:
            doors = fopen("porta_aberta2.txt", "r");
            if(doors == NULL)
            {
                printf("Erro ao abrir a imagem da porta aberta!");
            }
            else
            {
                screen(doors, TAM_PORTAS_ABERTA);
            }
            break;
        case 3:
            doors = fopen("porta_aberta3.txt", "r");
            if(doors == NULL)
            {
                printf("Erro ao abrir a imagem da porta aberta!");
            }
            else
            {
                screen(doors, TAM_PORTAS_ABERTA);
            }
            break;
        default:
            break;
    }

    fclose(doors);
}

void fullScreen()
{
    system("mode con cols=200 lines=40");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    system("title Jogo das Portas");

    FILE *map;
    tQuestions qs[QUANTITY_QUESTIONS];
    setUpQuestions(qs, QUANTITY_QUESTIONS - 1);

    int playing = 1;
    int points = 0;
    int round = 0;

    int qs_used[QUANTITY_QUESTIONS];

    int chosenDoor = 0;
    int question = 0;

    int questionDoor1;
    int questionDoor2;
    int questionDoor3;
    char answer;

    fullScreen();

    menu();

    do
    {

        questionDoor1 = randomQuestion(QUANTITY_QUESTIONS - 1, qs_used, round);
        questionDoor2 = randomQuestion(QUANTITY_QUESTIONS - 1, qs_used, round);
        questionDoor3 = randomQuestion(QUANTITY_QUESTIONS - 1, qs_used, round);

        do

        {

            system("cls");
            map = fopen("mapa.txt", "r");

            if(map == NULL)
            {
                printf("Erro ao abrir o mapa \n\n");
                break;
            }
            else
            {
                screen(map, TAM_PORTAS);
                fclose(map);
            }

            printf("SCORE: %d\n\n", points);
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
            openDoor(chosenDoor);
            question = questionDoor1;
            break;
        case(2):
            openDoor(chosenDoor);
            question = questionDoor2;
            break;
        case(3):
            openDoor(chosenDoor);
            question = questionDoor3;
            break;
        default:
            printf("Erro!");
            break;
        }

        qs_used[round] = question;

        printf("\n\n%s\n", qs[question].question);
        printf("\n%s\n", qs[question].alternative1);
        printf("%s\n", qs[question].alternative2);
        printf("%s\n", qs[question].alternative3);

        printf("\nEscolha sua resposta (a, b ou c): ");
        scanf("%c", &answer);

        if(!answerValidator(qs, question, answer))
        {
            printf("\nResposta errada!\n");
            playing = 0;
        }
        else
        {
            printf("\nResposta Correta!");
            points += POINT_RATE;

            if(round >= QUANTITY_QUESTIONS)
            {
                playing = 0;
            }

            round++;
        }

            getch();


    }while(playing);

    return 0;
}
