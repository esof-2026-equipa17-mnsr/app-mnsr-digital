#include <stdio.h>
#include "roteiros.c"
#include "gamificacao.c"
#include "assistente.c"

void menuRoteiros() {
    Obra acervo[] = {
        {1, "O Desterro (Soares dos Reis)", "Escultura", 5, 0},
        {2, "Retrato de Henrique Pousao (Columbano)", "Pintura", 5, 0},
        {3, "Custodia de Bemposta", "Ourivesaria", 4, 0},
        {4, "Senhora do O", "Escultura", 5, 0},
        {5, "Auto-retrato (Aurelia de Sousa)", "Pintura", 5, 0},
        {6, "Caixao de Mumia Egipcia", "Arqueologia", 5, 0},
        {7, "Biombo Namban", "Artes Decorativas", 6, 0},
        {8, "Cristo Crucificado (Frei Cipriano)", "Escultura", 5, 0},
        {9, "Paisagem do Douro (Silva Porto)", "Pintura", 5, 0},
        {10, "Salva de Prata Dourada", "Ourivesaria", 5, 0},
        {11, "Busto de Camilo Castelo Branco", "Escultura", 4, 0},
        {12, "Sagrada Familia (Vasco Fernandes)", "Pintura", 5, 0}
    };
    int total_obras = 12;
    int indices_roteiro[12];
    int opcao_tempo = 0;
    int opcao_tema = 0;
    int tempo_visitante = 0;
    const char *tema_escolhido = "Todos";

    printf("\n=== MNSR Digital - Roteiros Inteligentes ===\n");
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
    int qtd_sugeridas = gerarRoteiroPorTema(acervo, total_obras, tempo_visitante, tema_escolhido, indices_roteiro);

    if (qtd_sugeridas > 0) {
        int reajustar = 0;
        printf("\nMenu do Roteiro - A meio da visita:\n");
        printf("1 - Reajustar tempo restante ('Tenho X minutos')\n");
        printf("0 - Continuar roteiro atual\n");
        printf("Opcao: ");
        scanf("%d", &reajustar);

        if (reajustar == 1) {
            int ja_visitadas = 0;
            int tempo_restante = 0;

            printf("Quantas obras deste roteiro ja visitou? (0 a %d): ", qtd_sugeridas);
            scanf("%d", &ja_visitadas);

            if (ja_visitadas < 0) ja_visitadas = 0;
            if (ja_visitadas > qtd_sugeridas) ja_visitadas = qtd_sugeridas;

            for (int i = 0; i < ja_visitadas; i++) {
                acervo[indices_roteiro[i]].visitada = 1;
            }

            printf("Tenho quantos minutos restantes? ");
            scanf("%d", &tempo_restante);

            int restantes = reajustarRoteiro(acervo, total_obras, tempo_restante, tema_escolhido);

            if (restantes == 0) {
                int acao_fim = 0;
                printf("\nAcao disponivel:\n");
                printf("1 - Terminar visita\n");
                printf("Opcao: ");
                scanf("%d", &acao_fim);
                printf("\nVisita terminada. Obrigado por utilizar o MNSR Digital!\n");
            }
        }
    }
}

int main() {
    int menu_principal = -1;

    while (menu_principal != 0) {
        printf("\n========================================\n");
        printf("       MNSR Digital - Menu Principal    \n");
        printf("========================================\n");
        printf("1 - Roteiros Inteligentes\n");
        printf("2 - Caca ao Tesouro (QR Codes)\n");
        printf("3 - Assistente IA / RAG (Acervo MNSR)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        menu_principal = lerOpcaoNumerica();

        switch (menu_principal) {
            case 1:
                menuRoteiros();
                break;
            case 2:
                iniciarCacaAoTesouro();
                break;
            case 3:
                iniciarAssistenteIA();
                break;
            case 0:
                printf("\nAte breve no Museu Nacional Soares dos Reis!\n");
                break;
            default:
                printf("\nOpcao invalida.\n");
                break;
        }
    }

    return 0;
}