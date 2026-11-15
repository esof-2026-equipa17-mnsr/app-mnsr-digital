#include <stdio.h>
#include "roteiros.c"

int main() {
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
    int opcao_tempo = 0;
    int opcao_tema = 0;
    int tempo_visitante = 0;
    const char *tema_escolhido = "Todos";

    printf("=== MNSR Digital - Roteiros Inteligentes ===\n");
    printf("Selecione a duracao da sua visita:\n");
    printf("1 - 30 minutos (Essencial)\n");
    printf("2 - 60 minutos (Intermedio)\n");
    printf("3 - 90 minutos (Completo)\n");
    printf("Opcao: ");
    scanf("%d", &opcao_tempo);

    switch (opcao_tempo) {
        case 1: tempo_visitante = 30; break;
        case 2: tempo_visitante = 60; break;
        case 3: tempo_visitante = 90; break;
        default:
            printf("Opcao invalida. A aplicar duracao minima de 30 minutos.\n");
            tempo_visitante = 30;
            break;
    }

    printf("\nSelecione o tema de interesse:\n");
    printf("0 - Todos os temas\n");
    printf("1 - Pintura\n");
    printf("2 - Escultura\n");
    printf("3 - Ourivesaria\n");
    printf("4 - Arqueologia\n");
    printf("5 - Artes Decorativas\n");
    printf("Opcao: ");
    scanf("%d", &opcao_tema);

    switch (opcao_tema) {
        case 0: tema_escolhido = "Todos"; break;
        case 1: tema_escolhido = "Pintura"; break;
        case 2: tema_escolhido = "Escultura"; break;
        case 3: tema_escolhido = "Ourivesaria"; break;
        case 4: tema_escolhido = "Arqueologia"; break;
        case 5: tema_escolhido = "Artes Decorativas"; break;
        default:
            printf("Tema invalido. A mostrar todos os temas.\n");
            tema_escolhido = "Todos";
            break;
    }

    printf("\n");
    gerarRoteiroPorTema(acervo, total_obras, tempo_visitante, tema_escolhido);

    return 0;
}