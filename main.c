#include <stdio.h>
#include "roteiros.c"
#include "gamificacao.c"
#include "assistente.c"

void menuRoteiros() {
    Obra acervo[] = {
        {1, "O Desterro (Soares dos Reis)", "Escultura", 5, 0},
        {2, "Retrato de Henrique Pousao (Columbano)", "Pintura", 5, 0},
        {3, "Custodia de Bemposta", "Ourivesaria", 4, 0},
        {4, "Senhora do O (Mestre de Pero)", "Escultura", 5, 0},
        {5, "Auto-retrato (Aurelia de Sousa)", "Pintura", 5, 0},
        {6, "Caixao de Mumia Egipcia", "Arqueologia", 5, 0},
        {7, "Biombo Namban", "Artes Decorativas", 5, 0},
        {8, "Cristo Crucificado (Frei Cipriano)", "Escultura", 5, 0},
        {9, "Paisagem do Douro (Silva Porto)", "Pintura", 4, 0},
        {10, "Salva de Prata Dourada", "Ourivesaria", 5, 0},
        {11, "Busto de Camilo Castelo Branco", "Escultura", 5, 0},
        {12, "Sagrada Familia (Vasco Fernandes)", "Pintura", 5, 0},
        {13, "A Morte de Adonis (Soares dos Reis)", "Escultura", 5, 0},
        {14, "O Negro (Henrique Pousao)", "Pintura", 5, 0},
        {15, "Contador Indo-Portugues", "Artes Decorativas", 4, 0},
        {16, "A Flor Agreste (Soares dos Reis)", "Escultura", 5, 0},
        {17, "Cecilia (Henrique Pousao)", "Pintura", 5, 0},
        {18, "Estela Funeraria Romana", "Arqueologia", 5, 0}
    };
    int total_obras = 18;
    int indices_roteiro[18];
    int opcao_tempo = 0;
    int opcao_tema = 0;
    int tempo_visitante = 0;
    const char *tema_escolhido = "Todos";

    printf("\n=== MNSR Digital - Roteiros Inteligentes ===\n");
    printf("1 - Gerar novo roteiro personalizado\n");
    printf("2 - Aceder ao ultimo roteiro guardado em cache [Modo Offline]\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Opcao: ");
    int acao_inicial = lerOpcaoNumerica();

    if (acao_inicial == 2) {
        carregarRoteiroCache();
        return;
    } else if (acao_inicial != 1) {
        return;
    }

    printf("\nSelecione a duracao da visita:\n");
    printf("1 - 30 minutos (5 a 7 obras)\n");
    printf("2 - 60 minutos (10 a 14 obras)\n");
    printf("3 - 90 minutos (15 a 21 obras)\n");
    printf("Opcao: ");
    opcao_tempo = lerOpcaoNumerica();

    if (opcao_tempo == 1) tempo_visitante = 30;
    else if (opcao_tempo == 2) tempo_visitante = 60;
    else if (opcao_tempo == 3) tempo_visitante = 90;
    else {
        printf("Duracao invalida. A assumir 30 minutos.\n");
        tempo_visitante = 30;
    }

    printf("\nSelecione o tema da visita:\n");
    printf("0 - Todos os Temas\n");
    printf("1 - Escultura\n");
    printf("2 - Pintura\n");
    printf("3 - Ourivesaria\n");
    printf("4 - Arqueologia\n");
    printf("5 - Artes Decorativas\n");
    printf("Opcao: ");
    opcao_tema = lerOpcaoNumerica();

    if (opcao_tema == 1) tema_escolhido = "Escultura";
    else if (opcao_tema == 2) tema_escolhido = "Pintura";
    else if (opcao_tema == 3) tema_escolhido = "Ourivesaria";
    else if (opcao_tema == 4) tema_escolhido = "Arqueologia";
    else if (opcao_tema == 5) tema_escolhido = "Artes Decorativas";
    else tema_escolhido = "Todos";

    int qtd = gerarRoteiroPorTema(acervo, total_obras, tempo_visitante, tema_escolhido, indices_roteiro);

    if (qtd > 0) {
        int sub_opcao = -1;
        while (sub_opcao != 0) {
            printf("\n1 - Marcar obra visitada e reajustar roteiro\n");
            printf("2 - Simular perda de rede e abrir roteiro em cache [Modo Offline]\n");
            printf("0 - Voltar ao menu principal\n");
            printf("Opcao: ");
            sub_opcao = lerOpcaoNumerica();

            if (sub_opcao == 1) {
                printf("Numero da obra visitada (1 a %d): ", qtd);
                int num_obra = lerOpcaoNumerica();
                if (num_obra >= 1 && num_obra <= qtd) {
                    acervo[indices_roteiro[num_obra - 1]].visitada = 1;
                    printf("Obra '%s' marcada como visitada!\n", acervo[indices_roteiro[num_obra - 1]].nome);
                }
                printf("Tempo restante disponivel (minutos): ");
                int tempo_restante = lerOpcaoNumerica();
                reajustarRoteiro(acervo, total_obras, tempo_restante, tema_escolhido);
            } else if (sub_opcao == 2) {
                carregarRoteiroCache();
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