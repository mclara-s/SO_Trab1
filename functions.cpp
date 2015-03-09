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

