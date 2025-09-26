/*
--------------------------------------------------------
Arquivo: infixa_para_posfixa.c
Autores: Thiago Amaral e Christian Toleto
Data de criação: 19/09/2025
Última modificação: 26/09/2025
Descrição: Programa em C que converte expressões da 
           notação infixa para a notação pós-fixa 
           (notação polonesa reversa), utilizando pilha.
Versão: 1.2
--------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struct que representa os "nós" da pilha
typedef struct celula{
    char valor;
    struct celula *proximo;
}CELULA;

// Struct que representa a pilha, sempre apontando para a última célula que entrou
typedef struct pilha{
    CELULA *topo;
}PILHA;

// Função que cria a pilha dinamicamente na Heap e retorna um endereço para a função main na Stack
PILHA *criar_pilha(){
    PILHA *endereço_pilha_na_heap;
    endereço_pilha_na_heap = (PILHA*)malloc(sizeof(PILHA));
    endereço_pilha_na_heap->topo = NULL; // começa vazia, apontando para nada
    return endereço_pilha_na_heap; // retorna o endereço que localiza o topo da pilha na Heap
}

// Função que empilha uma nova célula na pilha; recebe como parâmetro o endereço da pilha e um caractere
void empilhar(PILHA *pilha,char caracter){
    CELULA *elemento = (CELULA*) malloc(sizeof(CELULA));
    elemento->proximo = pilha->topo;
    pilha->topo = elemento;
    elemento->valor = caracter;
    return;
}

// Função que desempilha a célula da pilha para onde o topo aponta e retorna o valor armazenado nela
char desempilhar(PILHA *pilha){ // Único parâmetro é a pilha (topo)
    if(pilha->topo == NULL){
        // printf("A pilha está vazia"); // verificação de pilha vazia
        return -1;
    }
    CELULA *aux = pilha->topo;
    pilha->topo = pilha->topo->proximo;
    char salvar_caractere = aux->valor;
    free(aux); // libera a célula para não ficar perdida na Heap
    return salvar_caractere;
}

// Função que libera as células da pilha. Ela percorre até a última célula e vai removendo de baixo para cima
void liberar_celulas(CELULA *celula){
    if(celula == NULL){ // caso base: para quando a célula apontar para NULL
        return;
    }
    liberar_celulas(celula->proximo);
    free(celula);
}

// Função que libera a pilha da Heap
void liberar_pilha(PILHA *pilha){
    liberar_celulas(pilha->topo);
    free(pilha);
    return;
}

/* 
Função que transforma uma expressão infixa para a notação polonesa (pós-fixa).

Parâmetros:
    - infixa: string contendo a expressão em notação infixa.

Retorno:
    - Ponteiro para string alocada dinamicamente contendo a expressão
      convertida para a notação pós-fixa.
*/
char *Transformar_infixa_para_posfixa(char *infixa){
    char *posfixa = (char*) malloc((strlen(infixa) + 1) * sizeof(char)); // Aloca string do mesmo tamanho da entrada para saída em pós-fixa
    PILHA *p = criar_pilha(); // Criação da pilha

    int k = 0; // variável que controla a posição da string pós-fixa
    for(int i=0; i < strlen(infixa); i++){
        char aux; // variável auxiliar que armazena os operadores para fins de comparação

        // Caso 1: se o caractere for '(', empilha o caractere
        if(infixa[i] == '('){
            empilhar(p,infixa[i]);
        }

        // Caso 2: operadores '+' ou '-'
        else if(infixa[i] == '-' || infixa[i] == '+'){
            aux = desempilhar(p); // Desempilha o operador do topo da pilha para verificar sua precedência
            while(aux != -1 && aux != '('){ // Desempilha operadores da pilha que têm precedência maior ou igual a '+' ou '-'
                posfixa[k] = aux;
                k++;                                                                    
                aux = desempilhar(p);
            }

            if(aux == -1){ // se a pilha estiver vazia,basta empilhar
                empilhar(p,infixa[i]);  
            }
            /* Caso o operador que estiver no topo da pilha tenha precedência menor que o caractere atual da infixa,
            empilha novamente o operador (aux) e em seguida empilha o caractere da expressão infixa.*/
            else{ 
                empilhar(p,aux);
                empilhar(p,infixa[i]);
            }     
        }

        // Caso 3: operadores '*' ou '/'
        else if(infixa[i] == '*' || infixa[i] == '/'){
            aux = desempilhar(p); // Desempilha o operador do topo da pilha para verificar sua precedência
            while(aux != -1  && aux != '(' && aux != '+' && aux != '-'){ // Desempilha operadores da pilha que têm precedência maior ou igual a '*' ou '/'
                posfixa[k] = aux;
                k++;
                aux = desempilhar(p);
            }
            if(aux == -1){ // se a pilha estiver vazia,basta empilhar
                empilhar(p,infixa[i]);  
            }
            /* Caso o operador que estiver no topo da pilha tenha precedência menor que o caractere atual da infixa,
            empilha novamente o operador (aux) e em seguida empilha o caractere da expressão infixa.*/
            else{
                empilhar(p,aux);
                empilhar(p,infixa[i]);
            }     
        }

        // Caso 4: operador '^'
        else if(infixa[i] == '^'){ // como '^' é o operador de maior precedência, basta empilhar
            aux = desempilhar(p); // Desempilha o operador do topo da pilha para verificar sua precedência
            if(aux != -1){ 
                empilhar(p, aux);
            }
            empilhar(p, infixa[i]);
        }

        // Caso 5: se for um parêntese de fechamento ')', desempilha todos os operadores até encontrar '('
        else if(infixa[i] == ')'){ 
            aux = desempilhar(p);
            while(aux != '(' ){
                posfixa[k] = aux;
                k++;
                aux = desempilhar(p);
            }
        }
        
        // Caso 6: se for um operando, adiciona diretamente à saída (na string pós-fixa)
        else{
            posfixa[k] = infixa[i];
            k++;
        }
    }

    // Desempilha o que ainda restar na pilha
    while(p->topo != NULL){
        posfixa[k]= desempilhar(p); 
        k++;
    }

    posfixa[k] = '\0'; // adiciona o caractere nulo no final da string
    liberar_pilha(p); // libera a pilha para não deixar memória órfã
    return posfixa;
}

int main(){
    char infixa[1000];
    scanf(" %[^\n]", infixa);
    char *posfixa = Transformar_infixa_para_posfixa(infixa);
    printf("%s\n",posfixa);
    free(posfixa);
    return 0;
}
