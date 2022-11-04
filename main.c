#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>

#define QUESTIONS_PATH "questions.vd"
#define RANKING_PATH "ranking.txt"

#define POINT_RATE 100
#define QUANTITY_QUESTIONS 30
#define QUANTITY_ROUNDS QUANTITY_QUESTIONS/3

#define TAM_PORTAS 71
#define TAM_PORTAS_ABERTA 55

#define SIZE_PLAYER_NAME 100

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
    char playerName[SIZE_PLAYER_NAME];
    int score;

} tRanking;

int randomQuestion(int limite, int *val_used, int aux)
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

void tutorial()
{
    int bContinue = 1;
    int chosenDoor = 0;
    char answer;
    FILE *map;

    map = fopen("mapa.txt", "r");
    if(map == NULL)
    {
        printf("Erro ao abrir o mapa!");
    }
    else
    {
        screen(map, TAM_PORTAS);
        fclose(map);
    }

    do
    {
        printf("\nPor nível, há 3 portas, cada uma escondendo uma pergunta!\n\n");
        printf("\nEscolha a porta 1 e pressione ENTER para acessá-la:\n");
        scanf("%d", &chosenDoor);
        fflush(stdin);

        chosenDoor = chosenDoor != 1 ? 0 : chosenDoor;

        if(chosenDoor == 0)
        {
            printf("\nDigite 1 para concluirmos o tutorial! Clique em uma tecla para continuar");
            getch();
        }
    }while(chosenDoor == 0);


    do
    {
        system("cls");
        openDoor(chosenDoor);

        printf("\n\nAo acessar uma porta, é apresentada a pergunta que nela estava escondida!\n\n");
        printf("Responda corretamente para vencer o jogo!\n");

        printf("\nQuem foram os desenvolvedores do jogo? (a/b/c)\n\n");
        printf("a) Eduardo A. Zampieri e Victor M. C. da Silva.\n");
        printf("b) Isaac Newton e Thomas Edison.\n");
        printf("c) Batman e Superman.\n\n");

        printf("Digite sua resposta: \n");
        answer = tolower(getchar());
        fflush(stdin);

        if(answer == 'a')
        {
            bContinue = 0;
        }
        else
        {
            printf("Digite a resposta correta! Dica - a letra 'a' está bem suspeita!\n\n");
            system("pause");
        }
    }while(bContinue);
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

void fullScreen()
{
    system("mode con cols=200 lines=40");
}

int pauseGame()
{
    int choose;
    system("cls");

    printf("1 - Resume\n");
    printf("2 - Exit");
    scanf("%d", &choose);

    switch(choose)
    {
        case 1:
            return 1;
            break;
        case 2:
            return 0;
            break;
    }
}

void fillPlayerRanking(tRanking *ranking, char *playerName, int score)
{
    for(int i = 0; i < SIZE_PLAYER_NAME; i++)
    {

        ranking->playerName[i] = playerName[i];
    }

    ranking->score = score;

}

void showRanking()
{
    tRanking playerRanking;
    FILE *ranking;

    ranking = fopen(RANKING_PATH, "r");

    if(ranking == NULL)
    {
        printf("Nenhum ranking para exibir");
    }
    else
    {

        system("cls");

        printf("\n\n******* RANKING *******\n\n");

        fread(&playerRanking, sizeof(tRanking), 1, ranking);

        if(ferror(ranking) != 0)
        {
            printf("Erro ao gravar ranking");
        }

        while(!feof(ranking))
        {

            printf("\nPlayer: %s", playerRanking.playerName);
            printf("Score: %d\n\n", playerRanking.score);

            fread(&playerRanking, sizeof(tRanking), 1, ranking);
        }

    }

    system("pause");

    fclose(ranking);

}

void savePlayerPoints(char *playerName, int sizeName, int points)
{
    tRanking playerRanking;
    FILE *ranking;

    ranking = fopen(RANKING_PATH, "a");

    if(ranking == NULL)
    {
        printf("Não foi possível salvar o ranking");
    }
    else
    {
        fillPlayerRanking(&playerRanking, playerName, points);

        fwrite(&playerRanking, sizeof(tRanking), 1, ranking);

        if(ferror(ranking) != 0)
        {
            printf("Erro ao gravar ranking");
        }

    }

    fclose(ranking);


}

void play()
{
    FILE *map;
    tQuestions qs[QUANTITY_QUESTIONS];
    setUpQuestions(qs, QUANTITY_QUESTIONS - 1);

    int val_used[QUANTITY_QUESTIONS];
    int aux_para_aleatoriedade = 0;

    char ch;

    char playerName[SIZE_PLAYER_NAME];
    int playing = 1;
    int points = 0;
    int round = 0;

    int chosenDoor = 0;
    int question = 0;

    int questionDoor1;
    int questionDoor2;
    int questionDoor3;
    char answer;

    do
    {
        questionDoor1 = randomQuestion(QUANTITY_QUESTIONS, val_used, aux_para_aleatoriedade);
        aux_para_aleatoriedade ++;
        questionDoor2 = randomQuestion(QUANTITY_QUESTIONS, val_used, aux_para_aleatoriedade);
        aux_para_aleatoriedade ++;
        questionDoor3 = randomQuestion(QUANTITY_QUESTIONS, val_used, aux_para_aleatoriedade);
        aux_para_aleatoriedade ++;

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

        //qs_used[round] = question;

        printf("\n\n%s\n", qs[question].question);
        printf("\n%s\n", qs[question].alternative1);
        printf("%s\n", qs[question].alternative2);
        printf("%s\n", qs[question].alternative3);

        printf("\nEscolha sua resposta (a, b ou c): ");
        scanf("%c", &answer);

        if(!answerValidator(qs, question, answer))
        {
            printf("\nResposta errada!\n");
        }
        else
        {
            printf("\nResposta Correta!");
            points += POINT_RATE;
        }

        round++;

        if(round + 1 == QUANTITY_ROUNDS + 1)
        {
            playing = 0;
        }

        //getch();

        if(!kbhit())
        {
            printf("\n\nPressione uma tecla qualquer para continuar ou ESC para pausar!\n");
            ch = getch();

            if((int)ch == 27)
            {
                playing = pauseGame();
            }
        }


    }while(playing);


    printf("\n\nParabens, você terminou o jogo!\n");
    printf("\nDigite seu nome para o Ranking: ");
    fflush(stdin);
    fgets(playerName, SIZE_PLAYER_NAME, stdin);

    savePlayerPoints(playerName, SIZE_PLAYER_NAME, points);
    showRanking();
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
        printf("\t3 - Ranking\n");
        printf("\t4 - Sair\n");

        fflush(stdin);
        scanf("%d", &choice);

        if(choice == 4)
        {
            confirmExit();
        }

        switch(choice)
        {
            case 1:
                play();
                bContinue = 1;
                break;
            case 2:
                tutorial();
                bContinue = 1;
                break;
            case 3:
                showRanking();
                bContinue = 1;
            default:
                bContinue = 1;
                break;
        }

    }while(bContinue == 1);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    system("title Jogo das Portas");

    fullScreen();

    menu();

    return 0;
}
