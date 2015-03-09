#include "functions.h"
#include "mainwindow.h"
#include <cstdlib>


void criarVetor(int tamanhoVetor, int *vetor){
  srand (time(NULL));
  for (int i=0;i<tamanhoVetor;i++){
    vetor[i] = rand()%10000;
  }
}

void bubbleSort(int *vetor, int tamanho) {
  int aux;
  for (int i = 0; i < tamanho-1; i++) {
     for (int j = 0; j < tamanho-1; j++) {
        if (vetor[j] > vetor[j+1]) {
          aux = vetor[j];
          vetor[j] = vetor[j+1];
          vetor[j+1] = aux;
        }
     }
  }
}

//Funcao usada pelo qsort para comparar dois numeros
int compare_ints( const void* a, const void* b ) {
     int* arg1 = (int*) a;
     int* arg2 = (int*) b;
     if( *arg1 < *arg2 ) return -1;
     else if( *arg1 == *arg2 ) return 0;
     else return 1;
}

