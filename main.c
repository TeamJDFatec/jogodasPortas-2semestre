#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <time.h>

#define QUESTIONS_PATH "questions.vd"
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

void menu();

int sair();

void tutorial();

void tela();

void entraPorta();

void fullScreen();

int main()
{
    setlocale(LC_ALL, "Portuguese");

    FILE *mapa;

    int jogando = 1;
    int rounds = 1;

    int val_used[QUANTITY_QUESTIONS];

    tQuestions qs[QUANTITY_QUESTIONS];
    setUpQuestions(qs, QUANTITY_QUESTIONS - 1);

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

        questionDoor1 = randomQuestion(QUANTITY_QUESTIONS - 1, val_used);
        questionDoor2 = randomQuestion(QUANTITY_QUESTIONS - 1, val_used);
        questionDoor3 = randomQuestion(QUANTITY_QUESTIONS - 1, val_used);

        do

        {
            system("cls");
            mapa = fopen("mapa.txt", "r");
            if(mapa == NULL)
            {
                printf("Erro ao abrir o mapa \n\n");
            }
            else
            {
                tela(mapa, TAM_PORTAS);
                fclose(mapa);
            }
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
            entraPorta(chosenDoor);
            question = questionDoor1;
            break;
        case(2):
            entraPorta(chosenDoor);
            question = questionDoor2;
            break;
        case(3):
            entraPorta(chosenDoor);
            question = questionDoor3;
            break;
        default:
            printf("Erro!");
            break;
        }

        //printf("\n\n%d\n\n\n", question);

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

void menu()
{
    int escolha;
    int continua = 1;

    do
    {
        system("cls");

        printf("****** Jogo das Portas ******\n\n\n");

        printf("\t1 - Jogar\n");
        printf("\t2 - Tutorial\n");
        printf("\t3 - Sair\n");

        fflush(stdin);
        scanf("%d", &escolha);

        switch(escolha)
        {
            case 1:
                continua = 0;
                break;
            case 2:
                tutorial();
                continua = 0;
                break;
            case 3:
                continua = sair();
                break;
            default:
                continua = 1;
                break;
        }
    }while(continua == 1);
}

int sair()
{
    char confirma;
    int continua;

    do
    {
        system("cls");
        printf("\nTem certeza que deseja sair? (s/n)");

        confirma = tolower(getch());

        if(confirma == 's')
        {
            exit(1);
        }
        else if(confirma == 'n')
        {
            continua = 0;
            return 1;
        }
        else
        {
            printf("\nDigite um valor válido por favor!\n");
            system("pause");
            continua = 1;
        }
    }while(continua == 1);
}

void tutorial()
{
    printf("Tutorial...");
}

void tela(FILE *mapa, int tam)
{
    char portas[tam];

    system("cls");

    fgets(portas, tam, mapa);
    while(!feof(mapa))
    {
        printf("%s", portas);
        fgets(portas, tam, mapa);
    }
    printf("%s", portas);
    printf("\n\n");
}

void entraPorta(int chosenDoor)
{
    FILE *portas;

    switch(chosenDoor)
    {
        case 1:
            //abre o arquivo onde esta desenhada a porta 1 e assim vai com os demais.
            portas = fopen("porta_aberta1.txt", "r");
            if(portas == NULL)
            {
                printf("Erro ao abrir a imagem da porta aberta!");
            }
            else
            {
                tela(portas, TAM_PORTAS_ABERTA);
            }
            //system("pause");
            break;
        case 2:
            portas = fopen("porta_aberta2.txt", "r");
            if(portas == NULL)
            {
                printf("Erro ao abrir a imagem da porta aberta!");
            }
            else
            {
                tela(portas, TAM_PORTAS_ABERTA);
            }
            break;
        case 3:
            portas = fopen("porta_aberta3.txt", "r");
            if(portas == NULL)
            {
                printf("Erro ao abrir a imagem da porta aberta!");
            }
            else
            {
                tela(portas, TAM_PORTAS_ABERTA);
            }
            break;
        default:
            break;
    }

    fclose(portas);
}

void fullScreen()
{
    system("mode con cols=200 lines=40");
}
