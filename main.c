//
// Created by Gabriel Martins on 11/15/2026.
//
#include <stdio.h>
#include "roteiros.c" // Incluímos o ficheiro com a lógica

int main() {
    // 1. Criar um "acervo" de teste (lista de obras do museu)
    Obra acervo[] = {
        {1, "O Desterro", "Escultura", 15},
        {2, "Retrato de Henrique Pousao", "Pintura", 10},
        {3, "Custodia de Bemposta", "Ourivesaria", 20},
        {4, "Estatua de Soares dos Reis", "Escultura", 10}
    };
    int total_obras = 4;

    // 2. Simular a User Story: o visitante diz que tem 30 minutos
    int tempo_visitante = 30;

    // 3. Executar a tua função
    gerarRoteiroPorTempo(acervo, total_obras, tempo_visitante);

    return 0;
}