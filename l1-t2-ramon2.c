#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "teclado.h"

#define HEIGHT 4
#define WIDTH 4

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
    printf("---------------------");
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


void numeros() // Função que define as cores para os números que não são o 0
{
    cor_fundo(255, 255, 255); // Deixa o fundo branco
    cor_letra(0, 0, 0); // Deixa a letra preta por contraste
}

void zero() // Função que define a cor do quadrado para o valor zero. Faz parecer um quadrado vazio
{
    cor_fundo(0,0,0); // Define o fundo como preto
    cor_letra(0,0,0); // Define a letra como preta
}

void printaMatriz(int u[][4], int tam, int col, int linha){
    divisoria_horizontal();
    for(int i = 0 ; i < tam ; i++){
        for( int j = 0 ; j < tam ; j++){
            divisoria_vertical_usual(); // Ajeita as cores das separações verticais dos números que não estão à direita
            if(j < tam-1){
                
                if(u[i][j] == 0){
                    zero(); // Corrije a cor da posição do número 0
                    printf("%2d ", u[i][j]);
                } else {
                    numeros(); // Ajeita as cores dos números que não são zero
                    printf("%2d ", u[i][j]);
                }
                cor_normal();
            } else {
                if(u[i][j] == 0){
                    zero(); // Corrije a cor da posição do número 0
                    printf("%2d ", u[i][j]);
                } else {
                    numeros(); // Ajeita as cores dos números que não são zero
                    printf("%2d ", u[i][j]);
                }
                divisoria_vertical_final_2(); // Ajeita as cores das separações verticais dos números que estão à direita dos valores na direita
                divisoria_horizontal();
            }
        }
    }
    printf("linha: %d  Coluna: %d", linha, col);
    putchar('\n');
}

int moveCima(int u[][4], int numero) // Função que move o slide para cima
{
    int temp;
    for( int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 4 ; j++){
            if(u[i][j] == numero){
                if(i+1 < 4){
                    temp = u[i+1][j];
                    u[i+1][j] = numero;
                    u[i][j] = temp;
                    j=4;
                    i=4;
                } else {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int moveBaixo(int u[][4], int numero) // Função que move o slide para baixo
{
    int temp;
    for( int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 4 ; j++){
            if(u[i][j] == numero){
                if(i-1 >= 0){
                    temp = u[i-1][j];
                    u[i-1][j] = numero;
                    u[i][j] = temp; 
                } else {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int moveDireita(int u[][4], int numero) // Função que move o slide para a direita
{
    int temp;
    for( int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 4 ; j++){
            if(u[i][j] == numero){
                if(j-1 >= 0){
                    temp = u[i][j-1];
                    u[i][j-1] = numero;
                    u[i][j] = temp; 
                } else {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int moveEsquerda(int u[][4], int numero) // Função que move o slide para a esquerda
{
    int temp;
    for( int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 4 ; j++){
            if(u[i][j] == numero){
                if(j+1 < 4){
                    temp = u[i][j+1];
                    u[i][j+1] = numero;
                    u[i][j] = temp; 
                    j =4;
                    i =4;
                } else {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void embaralha(int u[][4], int numero) // Função que embaralha a matriz. Funciona fazendo 50 jogadas válidas.
{
    int i = 0; // Contador de jogadas válidas. Serve para garantir que sejam executadas 50 jogadas válidas
    srand(time(NULL));
    while( i < 51 ){
        int random = rand() % 4+1;
        if(random == 1){
            if(moveCima(u, numero) == 1) // Esses if's chamam a função. Se retornar 1, significa que o movimento foi executado pois era válido e soma no contador, senão, ela não soma no contador.
            {
                i++;
            }
        } else if(random == 2){
            if(moveDireita(u, numero) == 1){
                i++;
            }
        } else if(random == 3){
            if(moveBaixo(u, numero) == 1){
                i++;
            }
        } else if(random == 4){
            if(moveEsquerda(u, numero) == 1){
                i++;
            }
        }
    }
}


void inicializa(int u[][4], int tam) // Inicializa a função
{
    for( int i = 0 ; i<tam ; i++){
        for( int j = 0 ; j<4 ; j++){
            u[i][j] = j+1;
            
        }
    }
}

int compMat(int u[][4], int v[][4]){
    for( int i = 0 ; i < 4 ; i++ ){
        for( int j = 0 ; j < 4 ; j++ ){
            if( u[i][j] != v[i][j] ){
                return 0;
            }
        }
    }
    return 1;
}

bool vitoria(int u[][4]){
    int ideal[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
    if( compMat(u, ideal) == 1 ){
        return true;
    }
    return false;
}

int move_across(int u[][4], int linha, int lado)
{
    int final = 3;
    int inicio = 0;
    if( lado == -1)
    {
        int temp = u[linha][0];
        for( int i = 0 ; i < 3 ; i++){
            u[linha][i] = u[linha][i+1];
        }
        u[linha][3] = temp;
    } else {
        int temp = u[linha][3];
        for(int i = 3 ; i > 0 ; i--){
            u[linha][i] = u[linha][i-1];
        }
        u[linha][0] = temp;
    }
}
 

int main(){
    char x = '0';
    int v = 0;
    int u[4][4];

    inicializa(u, 4);

    embaralha(u, 4);

    puts("Bem-vindo ao jogo de deslizar números");
    
    int linha = 0;
    int col = 0;

    printaMatriz(u, 4, col, linha);

    
    do{
        scanf("%d", &x);
        if(x == 'a'){
            move_across(u, linha, -1);
            printaMatriz(u, 4, col, linha);
        } else if(x == 's'){
            if(linha-1 >= 0){
                linha--;
            }
        } else if(x == 'd'){
            move_across(u, linha, 1);
            printaMatriz(u, 4, col, linha);
        } else if(x == 'w'){
            if(linha+1 < HEIGHT){
                linha++;
            }
            printaMatriz(u, 4, col, linha);
        } else if(x == 'c') {
            if(col+1 < WIDTH){
                col++;
            } else {
                col = 0;
            }
            printaMatriz(u, 4, col, linha);
        }
        else if(x == 'f') break;
    }while(vitoria(u) == false);
    printaMatriz(u, 4, col, linha);
    printf("\n```Parabens!! Voce ganhou!!´´´");
}