//
// Created by Gabriel Martins on 11/15/2026.
//
#include <stdio.h>
#include <string.h>

// 1. Definir a estrutura de uma Obra de Arte / Ponto de Interesse
typedef struct {
    int id;
    char nome[100];
    char tema[50];
    int tempo_estimado; // tempo médio de visita em minutos
} Obra;

// 2. Função principal da tua User Story: Gerar o roteiro
// Recebe o array com todas as obras do museu e o tempo que o visitante tem disponível
void gerarRoteiroPorTempo(Obra acervo[], int total_obras, int tempo_disponivel) {
    int tempo_acumulado = 0;

    printf("\n=== Roteiro Sugerido (Tempo max: %d minutos) ===\n", tempo_disponivel);

    // Lógica simples (Greedy algorythm): Adiciona obras até o tempo acabar
    for (int i = 0; i < total_obras; i++) {
        if (tempo_acumulado + acervo[i].tempo_estimado <= tempo_disponivel) {
            printf(" -> %s (%s) | Demora: %d min\n",
                   acervo[i].nome,
                   acervo[i].tema,
                   acervo[i].tempo_estimado);
            tempo_acumulado += acervo[i].tempo_estimado;
        }
    }

    printf("--------------------------------------------------\n");
    printf("Tempo total preenchido: %d minutos\n\n", tempo_acumulado);
}