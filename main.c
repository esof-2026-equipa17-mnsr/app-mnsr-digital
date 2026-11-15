#include <stdio.h>
#include "roteiros.c"

int main() {
    // Acervo de obras emblematicas (tempo = contemplacao + deslocacao entre salas)
    Obra acervo[] = {
        {1, "O Desterro (Soares dos Reis)", "Escultura", 5},
        {2, "Retrato de Henrique Pousao (Columbano)", "Pintura", 5},
        {3, "Custodia de Bemposta", "Ourivesaria", 4},
        {4, "Senhora do O", "Escultura", 5},
        {5, "Auto-retrato (Aurelia de Sousa)", "Pintura", 5},
        {6, "Caixao de Mumia Egipcia", "Arqueologia", 5},
        {7, "Biombo Namban", "Artes Decorativas", 6},
        {8, "Cristo Crucificado (Frei Cipriano)", "Escultura", 5},
        {9, "Paisagem do Douro (Silva Porto)", "Pintura", 5},
        {10, "Salva de Prata Dourada", "Ourivesaria", 5},
        {11, "Busto de Camilo Castelo Branco", "Escultura", 4},
        {12, "Sagrada Familia (Vasco Fernandes)", "Pintura", 5}
    };
    int total_obras = 12;
    int opcao = 0;
    int tempo_visitante = 0;

    printf("=== MNSR Digital - Roteiros Inteligentes ===\n");
    printf("Selecione a duracao da sua visita:\n");
    printf("1 - 30 minutos (Essencial: 5 a 7 obras)\n");
    printf("2 - 60 minutos (Intermedio)\n");
    printf("3 - 90 minutos (Completo)\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            tempo_visitante = 30;
            break;
        case 2:
            tempo_visitante = 60;
            break;
        case 3:
            tempo_visitante = 90;
            break;
        default:
            printf("\nOpcao invalida. A aplicar duracao minima de 30 minutos.\n");
            tempo_visitante = 30;
            break;
    }

    printf("\n");
    gerarRoteiroPorTempo(acervo, total_obras, tempo_visitante);

    return 0;
}