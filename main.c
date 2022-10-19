#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define NOME_ARQ "perguntas.quiz"
#define QTD_PERGUNTAS 10

typedef struct
{
    int indice;
    char pergunta[200];
    char resposta1[100];
    char resposta2[100];
    char resposta3[100];
    int correta;
}tPergunta;

int geradorAleatorio()
{
    int indice = 0;

    srand(time(NULL));

    indice = rand() % QTD_PERGUNTAS;

    return indice;
}

void limpaVetor(char *pergunta, int tam)
{
    for(int i = 0; i < tam; i++)
    {
        pergunta[i] = '\0';
    }
}

void criaPergunta()
{
    FILE *arq;

    tPergunta p;

    p.indice = geradorAleatorio();
    limpaVetor(&p.pergunta, 200);
    strcpy(p.pergunta, "Qual meu nome?");
    limpaVetor(&p.resposta1, 100);
    strcpy(p.resposta1, "Roberto");
    limpaVetor(&p.resposta2, 100);
    strcpy(p.resposta2, "Luciano");
    limpaVetor(&p.resposta3, 100);
    strcpy(p.resposta3, "Eduardo");
    p.correta = 3;

    arq = fopen(NOME_ARQ, "ab");

    if(arq == NULL)
    {
        printf("Erro ao abrir o arquivo de perguntas para escrita!");
    }
    else
    {

        fwrite(&p, sizeof(tPergunta), 1, arq);
        fputc('\n', arq);

        fclose(arq);
    }
}

void guardaPergunta(tPergunta *perguntas)
{
    FILE *arq;
    arq = fopen(NOME_ARQ, "rb");
    tPergunta linha;
    int i = 0;

    if(arq == NULL)
    {
        printf("Erro ao abrir o arquivo de perguntas para leitura!");
    }
    else
    {
        fread(&linha, sizeof(tPergunta), 1, arq);
        while(!feof(arq))
        {
            perguntas[i] = linha;
            fread(&linha, sizeof(tPergunta), 1, arq);
            i++;
        }

        fclose(arq);
    }
}

void exibePergunta()
{

}

void limpaArquivo()
{
    int retorno = 0;
    char nome[] = NOME_ARQ;

    retorno = remove(nome);

    if(retorno == 0)
    {
        printf("\n\nArquivo foi deletado com sucesso!\n");
    }
    else
    {
        printf("\n\nErro ao deletar o arquivo desejado!\n");
    }
}

int main()
{
    tPergunta perguntas[QTD_PERGUNTAS];
    int resposta;

    criaPergunta();
    guardaPergunta(perguntas);

    printf("\n\n%d\n", perguntas[0].indice);
    printf("%s\n", perguntas[0].pergunta);
    printf("%s\n", perguntas[0].resposta1);
    printf("%s\n", perguntas[0].resposta2);
    printf("%s\n", perguntas[0].resposta3);
    printf("%d\n", perguntas[0].correta);

    printf("\nEscolha sua resposta: ");
    scanf("%d", &resposta);

    if(resposta == perguntas[0].correta)
    {
        printf("Parabens, voce acertou a resposta!");
    }
    else
    {
        printf("Infelizmente voce errou, a resposta correta era %d", perguntas[0].correta);
    }

    limpaArquivo();

    return 0;
}
