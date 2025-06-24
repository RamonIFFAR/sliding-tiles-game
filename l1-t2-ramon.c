#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "teclado.h"

void cor_letra(int r, int g, int b) // Troca a cor da letra
{
    printf("%c[38;2;%d;%d;%dm", 27, r, g, b);
}

void cor_fundo(int r, int g, int b) // Troca a cor do fundo
{
    printf("%c[48;2;%d;%d;%dm", 27, r, g, b);
}

void cor_normal() // Volta a cor do texto e do fundo pra branco
{
    printf("%c[0m", 27);
}

void divisoria_horizontal() // Função que ajeita a divisória horizontal que separa as linhas
{
    putchar('\n');
    cor_fundo(120, 120, 150);
    cor_letra(120, 120, 150);
    printf("-------------------------------");
    cor_normal();
    putchar('\n');
}

/*
 *
 * Essas duas funções à seguir são um fiasco. Ambas fazem praticamente a mesma coisa, mas da um leve BO quando eu tento transformar elas em uma.
 * Portanto, tendo isso em vista, deixarei assim.
 * Posso mudar se necessário.
 *
*/
void divisoria_vertical_usual() // Função que define a cor da vertical antes do número
{
    cor_fundo(120, 120, 150);
    cor_letra(120, 120, 150);
    printf("| ");
    cor_normal();
}

void divisoria_vertical_final_2() // Função que define a cor da vertical após o número
{
    cor_fundo(120, 120, 150);
    cor_letra(120, 120, 150);
    printf("|");
    cor_normal();
}
/*
 * Aqui morre o fiasco
 *
*/


void numeros(int nQ) // Função que define as cores para os números que não são o 0
{
    switch(nQ){
        case 0: // Cor preta
            cor_fundo(0, 0, 0); 
            break;
        case 1: // Cor azul
            cor_fundo(0, 0, 255); 
            break;
        case 2: // Cor vermelha
            cor_fundo(255, 0, 0); 
            break;
        case 3: // Cor verde
            cor_fundo(0, 255, 0); 
            break;
        case 4: // Cor amarela
            cor_fundo(255, 255, 0); 
            break;
        case 5: // Cor roxa
            cor_fundo(255, 0, 255);
            break;
        case 6: // Cor Ciano
            cor_fundo(0, 255, 255); 
            break;
        case 7: // Cor laranja
            cor_fundo(125, 250, 0); 
            break;
        case 8: // Cor branca
            cor_fundo(255, 255, 255); 
            break;
    }
}

void printaMatriz(int *u, int col, int linha, int HEIGHT, int WIDTH){
    int pos;
    for(int i = 0 ; i < HEIGHT ; i++){
        divisoria_horizontal();
        for( int j = 0 ; j < WIDTH ; j++){
            pos = i*WIDTH + j;
            divisoria_vertical_usual(); // Ajeita as cores das separações verticais dos números que não estão à direita
            if(j < WIDTH-1){
                numeros(*(u+pos));
                printf("%2d ", *(u+pos));
                cor_normal();
            } else {
                numeros(*(u+pos)); 
                printf("%2d ", *(u+pos));
                divisoria_vertical_final_2(); // Ajeita as cores das separações verticais dos números que estão à direita dos valores na direita
            }
        }
    }
    divisoria_horizontal();
    printf("linha: %d  Coluna: %d", linha, col);
    putchar('\n');
}

int moveBaixo(int *u, int i, int j, int HEIGHT, int WIDTH) // Função que move o slide para baixo
{
    int temp, pos;
    if(i-1 >= 0 && i <= HEIGHT-1 && *(u+(i*WIDTH+j)) == 0){
        pos = i*WIDTH+j;
        temp = *(u+pos-WIDTH);
        *(u+pos-WIDTH) = *(u+pos);
        *(u+pos) = temp;
        moveBaixo(u, i-1, j, HEIGHT, WIDTH);
    }
    return 0;
}

void inicializa(int *u, int altura, int largura, int numCores) // Inicializa a função
{
    int cor = 1, pos;
    for( int i = 0 ; i<altura ; i++){
        for( int j = 0 ; j<largura ; j++){
            pos = i*largura + j;
            *(u+pos) = cor;
            cor++;
            printf("numCores: %d", numCores);
            if(cor > numCores){
                cor = 1;
            }
        }
    }
}

void quedaBloco(int *u, int HEIGHT, int WIDTH)
{
    int pos;
    for( int i = 0 ; i < HEIGHT ; i++){
        for( int j = 0 ; j < WIDTH ; j++){
            pos = i*WIDTH + j;
            if(*(u+pos) == 0){
                moveBaixo(u, i, j, HEIGHT, WIDTH);
            }
        }
    }
}

int move_across(int *u, int linha, int WIDTH, int lado) // Move as peças da linha para a direita ou para a esquerda com padrão circular
{
    

    int inicio = 0, pos;
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

void restauraLinha(int linha, int WIDTH, int *u)
{
    int pos;
    srand(time(NULL));
    for(int i = 0 ; i < WIDTH ; i++){
        pos = linha*WIDTH + i;
        *(u+pos) = rand() % 4+1;
    }
}

int buscaColunas(int *u, int WIDTH, int HEIGHT) // Função que busca por colunas com números iguais e retorna o índice da primeira coluna que encontrar. Retorna -1 se não encontrar
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

void detonaColuna(int *u, int WIDTH, int HEIGHT) // Função recursiva que continua a chamar a buscaColunas e apagar colunas até que buscaColunas retorne -1
{
    int col = buscaColunas(u, WIDTH, HEIGHT), pos;
    if( col != -1){
        for( int i = 0 ; i < HEIGHT ; i++){
            pos = i*WIDTH + col;
            *(u+pos) = 0;
        }
        detonaColuna(u, WIDTH, HEIGHT);
    }
}

void detonaLinha(int linha, int *u, int WIDTH)
{
    int pos;
    for( int i = 0 ; i < WIDTH ; i++){
        pos = linha*WIDTH + i;
        *(u+pos) = 0;
    }
    restauraLinha(linha, WIDTH, u);
}

int buscaTopo(int *u, int HEIGHT, int WIDTH, int col, int busca)
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

int buscaEspaco(int *u, int *nC, int Ca, int WIDTH, int HEIGHT, int lado)
{
    int posE = Ca-1, posD = Ca+1;
    int teste;
    if(posD >= WIDTH){
        posD -= Ca+1;
        printf("\nsoma da pos na direita: %d", posD);
    }
    if(posE < 0){
        posE = WIDTH-1;
        printf("\nsoma da pos na esquerda: %d", posE);
    }
    if(lado == 1){
        teste = buscaTopo(u, HEIGHT, WIDTH, posD*-1, 0);
        if( teste != -1){
            *nC = posD;
            printf("\nResultado da busca: %d", *nC);
            return 1;
        }
    } else {
        teste = buscaTopo(u, HEIGHT, WIDTH, posE, 0);
        printf("Return do busca topo: %d", teste);
        if( teste != -1){
            *nC = posE;
            printf("\nResultado da busca: %d", *nC);
            return 1;
        }
    }
    return -1;
    
}

void movePeca(int *u, int linha, int col, int WIDTH, int HEIGHT, int lado)
{
        if(linha == -1){
            return;
        }
    
        int nP, enc = buscaEspaco(u, &nP, col, WIDTH, HEIGHT, lado);
        int posAtual = linha*WIDTH+col;
        printf("\nposAtual: %d", posAtual);
        if( enc != -1){
            printf("\nValor na posição de NP: %d", *(u+nP));
            printf("\nValor na posição de anterior: %d", *(u+posAtual));
            *(u+nP) = *(u+posAtual);
            *(u+posAtual) = 0;
        }
        
}

void aumentacores(int *u, int HEIGHT, int WIDTH, int *numCores){
    *numCores += 1;
    inicializa(u, HEIGHT, WIDTH, *numCores);
}


int main(){
    int x = 0;

    int HEIGHT = 5;
    int WIDTH = 6;
    int numCores = 4;
    int *u = malloc(HEIGHT*WIDTH*sizeof(int));
    inicializa(u, HEIGHT, WIDTH, numCores);

    puts("Bem-vindo ao jogo de deslizar números");
    
    int linha = 0;
    int col = 0;

    printaMatriz(u, col, linha, HEIGHT, WIDTH);

    
    do{
        scanf("%d", &x);
        if(x == 1){

            move_across(u, linha, WIDTH, -1);

        } else if(x == 2){

            if(linha-1 >= 0){
                linha--;
            }

        } else if(x == 3){

            move_across(u, linha, WIDTH, 1);

        } else if(x == 4){

            if(linha+1 < HEIGHT){
                linha++;
            }

        } else if(x == 5) {

            if(col+1 < WIDTH){
                col++;
            } else {
                col = 0;
            }

        } else if(x == 6){

            detonaLinha(linha, u, WIDTH);

        } else if(x == 8){
            
            movePeca(u, buscaTopo(u, HEIGHT, WIDTH, col, 1), col, WIDTH, HEIGHT, 1);

        } else if(x == 7){
            movePeca(u, buscaTopo(u, HEIGHT, WIDTH, col, 1), col, WIDTH, HEIGHT, -1);

        } else if(x == 9){
            aumentacores(u, HEIGHT, WIDTH, &numCores);
        }
        //detonaColuna(u, WIDTH, HEIGHT);
        quedaBloco(u, HEIGHT, WIDTH);
        printaMatriz(u, col, linha, HEIGHT, WIDTH);
    }while(x != 0);
    printaMatriz(u, col, linha, HEIGHT, WIDTH);
    printf("\n```Parabens!! Voce ganhou!!´´´");
}
