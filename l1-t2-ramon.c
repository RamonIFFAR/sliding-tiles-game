#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "janela.h"

#define TAM_BLOCO 30
#define SEPARACAO 50
#define COORD_INICIAL 100
#define WIDTH 6
#define HEIGHT 5

int compara(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int moveBaixo(int *u, int i, int j) // Função que move o slide para baixo
{
    int temp, pos;
    if(i-1 >= 0 && i <= HEIGHT-1 && *(u+(i*WIDTH+j)) == 0){
        pos = i*WIDTH+j;
        temp = *(u+pos-WIDTH);
        *(u+pos-WIDTH) = *(u+pos);
        *(u+pos) = temp;
        moveBaixo(u, i-1, j);
    }
    return 0;
}


void inicializa(int *u, int numCores) // Inicializa a função
{
    srand(time(NULL));
    int pos;
    for( int i = 0 ; i<HEIGHT ; i++){
        for( int j = 0 ; j<WIDTH ; j++){
            pos = i*WIDTH + j;
            *(u+pos) = rand() % numCores + 1;
        }
    }
}

void quedaBloco(int *u)
{
    int pos;
    for( int i = 0 ; i < HEIGHT ; i++){
        for( int j = 0 ; j < WIDTH ; j++){
            pos = i*WIDTH + j;
            if(*(u+pos) == 0){
                moveBaixo(u, i, j);
            }
        }
    }
}

void move_across(int *u, int linha, int lado) // Move as peças da linha para a direita ou para a esquerda com padrão circular
{
    

    int pos;
    if( lado == -1)
    {
        pos = linha*WIDTH;
        int temp = *(u+pos);
        for( int i = 0 ; i < WIDTH-1 ; i++){
            pos = linha*WIDTH + i;
            *(u+pos) = *(u+pos+1);
            
        }
        pos = linha*WIDTH + WIDTH-1;
        *(u+pos) = temp;
    } else {
        pos = linha*WIDTH +WIDTH-1;
        int temp = *(u+pos);
        for(int i = WIDTH-1 ; i > 0 ; i--){
            pos = linha*WIDTH + i;
            *(u+pos) = *(u+pos-1);
        }
        pos = linha*WIDTH;
        *(u+pos) = temp;
    }
}

void restauraLinha(int linha, int *u, int numCores)
{
    int pos;
    srand(time(NULL));
    for(int i = 0 ; i < WIDTH ; i++){
        pos = linha*WIDTH + i;
        *(u+pos) = rand() % numCores +1;
    }
}

int buscaColunas(int *u) // Função que busca por colunas com números iguais e retorna o índice da primeira coluna que encontrar. Retorna -1 se não encontrar
{
    int cont[2] = {0, -1}, pos;
    for( int i = 0 ; i < WIDTH ; i++){
        for( int j = 0 ; j < HEIGHT ; j++){
            pos = j*WIDTH + i;
            if(*(u+pos) == *(u+i) && *(u+i) != 0){
                
                cont[0]++;
            } else {
                cont[0] = 0;
                break;
            }
        }
        if(cont[0] == HEIGHT){
            cont[1] = i;
            return cont[1];
        } else {
            cont[0] = 0;
            cont[1] = -1;
        }
    }
    return -1;

}

void detonaColuna(int *u, int* pontos) // Função recursiva que continua a chamar a buscaColunas e apagar colunas até que buscaColunas retorne -1
{
    int col = buscaColunas(u), pos;
    if( col != -1){
        for( int i = 0 ; i < HEIGHT ; i++){
            pos = i*WIDTH + col;
            *(u+pos) = 0;
        }
        *pontos += 10;
        detonaColuna(u, pontos);
    }
}

void detonaLinha(int linha, int *u, int *pontos, int numCores)
{
    int pos;
    for( int i = 0 ; i < WIDTH ; i++){
        pos = linha*WIDTH + i;
        *(u+pos) = 0;
    }
    restauraLinha(linha, u, numCores);
    if(*(pontos)-1 >= 0){
        *pontos -= 2;
    }
    
}

int buscaTopo(int *u, int col, int busca)
{
    for( int i = WIDTH-1 ; i>=0 ; i--){
        if(*(u+i*WIDTH+col) == 0 && busca == 0){
            return i;
        } else if (*(u+i*WIDTH+col) == 0 && busca == 1 && i+1 < WIDTH){
            return i+1;
        }
    }
    return -1;
}

int buscaEspaco(int *u, int *nC, int Ca, int lado)
{
    int posE = Ca-1, posD = Ca+1;
    int teste;
    if(posD >= WIDTH){
        posD -= Ca+1;
        //printf("\nsoma da pos na direita: %d", posD);
    }
    if(posE < 0){
        posE = WIDTH-1;
        //printf("\nsoma da pos na esquerda: %d", posE);
    }
    if(lado == 1){
        teste = buscaTopo(u, posD*-1, 0);
        if( teste != -1){
            *nC = posD;
            //printf("\nResultado da busca: %d", *nC);
            return 1;
        }
    } else {
        teste = buscaTopo(u, posE, 0);
        //printf("Return do busca topo: %d", teste);
        if( teste != -1){
            *nC = posE;
            //printf("\nResultado da busca: %d", *nC);
            return 1;
        }
    }
    return -1;
    
}

void movePeca(int *u, int linha, int col, int lado)
{
        if(linha == -1){
            return;
        }
    
        int nP, enc = buscaEspaco(u, &nP, col, lado);
        int posAtual = linha*WIDTH+col;
        //printf("\nposAtual: %d", posAtual);
        if( enc != -1){
            //printf("\nValor na posição de anterior: %d", *(u+posAtual));
            if(*(u+nP) == 0){
                *(u+nP) = *(u+posAtual);
                *(u+posAtual) = 0;
            }
        }
}

void aumentacores(int *u, int *numCores){
    *numCores += 1;
    inicializa(u, *numCores);
}

void addPeca(int *u, int numCores)
{
    srand(time(NULL));
    for( int i = 0 ; i < WIDTH ; i++){
        if(*(u+i) != 0){
            return;
        }
    }
    *u = rand() % numCores+1;
    quedaBloco(u);
    *u = rand() % numCores+1;
    addPeca(u, numCores);
}

int* contaRecordes(int *v, int *qtd)
{
    FILE* fp;
    fp = fopen("score.txt", "r");
    int pos = 0, controle = 0, substitui = *(qtd), tempBuffer[1];
    while(!feof(fp)){
        fscanf(fp, "%d", &tempBuffer[0]);
        if(tempBuffer[0] != 0){
            //printf("Vai atribuir %d ao vetor\n", tempBuffer[0]);
            *(v+pos) = tempBuffer[0];
            //printf("Colocou na posicao %d: %d\n", pos, *(v+pos));
            pos += 1;
            substitui += 1;
            controle += 1;
            v = realloc(v, (1+pos)*sizeof(int));
            if(v == NULL){
                return 0;
            }
        }
    }
    fclose(fp);
    *(qtd) = substitui;
    return v;
}

// Estado jogo
typedef struct{
    tamanho_t tamanho_janela;
    tecla_t tecla;
    int pontos;
    bool terminou;
    double tempo_de_jogo;
    double data_inicio;


}jogo_t;

void processa_teclado(jogo_t *pj)
{
  // altera tecla caso tenha sido digitado algo
  if (j_tem_tecla()) pj->tecla = j_tecla();

}

void inicializa_jogo(jogo_t *pj)
{
    pj->tecla = 'x';
    pj->pontos = 0;
    pj->terminou = false;
}

void processa_tempo(jogo_t *pj)
{
  // termina depois de alguns segundos
  pj->tempo_de_jogo = j_relogio() - pj->data_inicio;
  if (pj->tempo_de_jogo >= 70) pj->terminou = true;
}

cor_t defineCor(int i)
{
    switch(i){
        case 0:
            cor_t preto = { 0.0, 0.0, 0.0, 0.0};
            return preto;
        case 1:
            cor_t azul = { 0.0, 0.0, 1.0, 1.0 };
            return azul;
        case 2:
            cor_t verde = { 0.0, 1.0, 0.0, 1.0 };
            return verde;
        case 3:
            cor_t vermelho = { 1.0, 0.0, 0.0, 1.0 };
            return vermelho;
        case 4:
            cor_t rosa = { 1.0, 0.0, 1.0, 1.0 };
            return rosa;
        case 5:
            cor_t ciano = { 0.0, 1.0, 1.0, 1.0 };
            return ciano;
        case 6:
            cor_t laranja = { 1.0, 0.5, 0.5, 1.0 };
            return laranja;
        case 7:
            cor_t desisto = { 0.3, 0.8, 0.1, 1.0 };
            return desisto;
        default:
            cor_t branco     = { 1.0, 1.0, 1.0, 1.0 };
            return branco;
    }
}

void pintaLinha(int linha, int *vetor)
{
    ponto_t inicio = {0 , 0};
    tamanho_t tamanho = {TAM_BLOCO, TAM_BLOCO};
    retangulo_t retangulo = {inicio, tamanho};
    for(int coluna = 0; coluna < 6 ; coluna++){
        inicio = (ponto_t){SEPARACAO*coluna+COORD_INICIAL, SEPARACAO*linha+COORD_INICIAL};
        retangulo = (retangulo_t){inicio, tamanho};
        j_retangulo(retangulo, 10, defineCor(*(vetor+linha*6+coluna)), defineCor(*(vetor+linha*6+coluna)));
    }
}

void pintaGrid(int *vetor)
{
    for(int i = 0 ; i < 5 ; i++){
        pintaLinha(i, vetor);
    }
}

/**
void mostraPontos()
{
    int qtd = 1;
    FILE* fp;
    fp = fopen("score.txt", "r");
    if( fp != NULL){
        v = contaRecordes(fp, v, &qtd);
        if(v != 0 && *(v) != 0)
        {
            printf("Há %d recorde(s)\n", qtd);
        }
    }

    if( x == 2){
        if(qtd > 1)
        {
            for(int i = 0; i < qtd ; i++){
                printf("Recorde: %d\n", *(v+i));
            }
        } else if(qtd > 0){
            printf("Recorde: %d\n", *(v));
        }
    }
}
*/

void indicaColuna(int col)
{
    ponto_t coluna = {COORD_INICIAL+col*SEPARACAO, COORD_INICIAL-10};
    tamanho_t tamanho = {TAM_BLOCO, TAM_BLOCO*HEIGHT+COORD_INICIAL};
    retangulo_t retangulo = {coluna, tamanho};
    j_retangulo(retangulo, 5, defineCor(8), defineCor(0));

}

void indicaLinha(int lin)
{
    ponto_t linha = {COORD_INICIAL-10, COORD_INICIAL+lin*SEPARACAO};
    tamanho_t tamanho = {TAM_BLOCO*WIDTH+COORD_INICIAL+20, TAM_BLOCO};
    retangulo_t retangulo = {linha, tamanho};
    j_retangulo(retangulo, 5, defineCor(8), defineCor(0));

}

void gravaPontos(int pontos, int* v, int qtd)
{
    //printf("Valor secreto em 0: %d\n", *v);
    //printf("Num posicoes: %d\n", qtd);
    *(v+qtd-1) = pontos;
    //printf("qtd = %d\n", qtd);
    //printf("busca manual na posição 0: %d\n", *(v));
    //printf("busca manual na posição 1: %d\n", *(v+1));
    //printf("busca manual na posição 2: %d\n", *(v+2));
    //printf("Último valor: %d\n", *(v+qtd));
    qsort(v, qtd, sizeof(int), compara);
    FILE* fp;
    fp = fopen("score.txt", "w");
    fprintf(fp, " ");
    fclose(fp);
    fp = fopen("score.txt", "a");
    for( int i = qtd-1 ; i >= 0 ; i--){
        //printf("Vai por no file na posição %d: %d\n",i, *(v+i));
        fprintf(fp, "\n%d", *(v+i));
    }
    fclose(fp);
}

void rodaJogo(jogo_t* jogo, int linha, int col, int* u, int* numCores, int* v, int qtd, int* pontos)
{
    *pontos = 0;
    ponto_t pontuacao;
    pontuacao = (ponto_t){50, 50};
    char SPontos[30];

    char tempo[30];
    ponto_t tempoJogo;
    tempoJogo = (ponto_t){300, 50};

    while(jogo->terminou == false){

        snprintf(SPontos, sizeof(SPontos), "Pontuacao: %d", *pontos);
        snprintf(tempo, sizeof(tempo), "Tempo: %.2f", jogo->tempo_de_jogo);
        j_texto(pontuacao, defineCor(8), SPontos);
        j_texto(tempoJogo, defineCor(8), tempo);

        indicaColuna(col);
        indicaLinha(linha);
        if((int)jogo->tempo_de_jogo >= 20 && (int)jogo->tempo_de_jogo % 20 == 0){
            aumentacores(u, numCores);
            j_cochila(1);
        }
        processa_teclado(jogo);
        if(jogo->tecla == T_DIREITA){
            move_across(u, linha, 1);
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_ESQUERDA){
            move_across(u, linha, -1);
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_BAIXO){
            if(linha+1 < HEIGHT){
                linha++;
            }
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_CIMA){
            if(linha-1 >= 0){
                linha--;
            }
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_DEL){
            detonaLinha(linha, u, pontos, *numCores);
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_ENTER){
            movePeca(u, buscaTopo(u, col, 1), col, -1);
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_BS){
            movePeca(u, buscaTopo(u, col, 1), col, 1);
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_PGDN){
            if(col+1 < WIDTH){
                col++;
            }
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_PGUP){
            if(col-1 >= 0){
                col--;
            }
            jogo->tecla = T_NADA;
        }
        if(jogo->tecla == T_ESC){
            jogo->terminou = true;
        }
        detonaColuna(u, pontos);
        quedaBloco(u);
        addPeca(u, *numCores);
        pintaGrid(u);
        processa_tempo(jogo);
        j_mostra();
    }
    
    jogo->tecla = T_NADA;
    jogo->terminou = false;
}

void telaInicial(int *v, int qtd)
{
    char pontos[30];
    ponto_t pontuacao = {30, 80}, tela = {10, 10};
    tamanho_t area = {480, 480};
    retangulo_t retangulo = {tela, area};
    j_retangulo(retangulo, 3, defineCor(4), defineCor(0));
    int separacao = 0; 
    j_texto(pontuacao, defineCor(3), "10 melhores");   
    if( qtd > 10)
    {
        for( int i = 0 ; i < 10 ; i++){
            pontuacao = (ponto_t){50, 120+separacao*35};
            snprintf(pontos, sizeof(pontos), "%d", *(v+i));
            j_texto(pontuacao, defineCor(8), pontos);
            separacao++;
        }
    } else {
        for( int i = 0 ; i < qtd-1 ; i++){
            pontuacao = (ponto_t){50, 120+separacao*35};
            snprintf(pontos, sizeof(pontos), "%d", *(v+i));
            j_texto(pontuacao, defineCor(8), pontos);
            separacao++;
        }
    }
    j_mostra();
}

int main(){

    jogo_t jogo;
    jogo.tamanho_janela =(tamanho_t){500, 500};

    j_inicializa(jogo.tamanho_janela, "teste");
    inicializa_jogo(&jogo);

    int numCores = 4;
    int *u = malloc(HEIGHT*WIDTH*sizeof(int));
    inicializa(u, numCores);

    int linha = 0;
    int col = 0;

    //FILE *fp;
    int qtd = 1;
    int* v; // Array para guardar as pontuações

    int pontos = 0;

    //int checa = 0;
    v = (int*)malloc(qtd * sizeof(int));
    if(v == NULL){
        printf("Deu BO. Não alocou a memória. Prepare-se para ver horrores além de sua própria humanidade. Aqui, a realidade é apenas uma opinião da qual discordamos");
    }

    while(jogo.terminou == false){
        qtd = 1;
        v = contaRecordes(v, &qtd); // qtd terá o valor de espaços contando um último espaço vazio
        processa_teclado(&jogo);

        
        telaInicial(v, qtd);
        if(jogo.tecla == T_ENTER){
            jogo.tecla = T_NADA;
            rodaJogo(&jogo, linha, col, u, &numCores, v, qtd, &pontos);
            if(pontos > 0){
                gravaPontos(pontos, v, qtd);
            }
        }
        
        if(jogo.tecla == T_ESC){
            jogo.terminou = true;
        }
    }
    j_finaliza();
}