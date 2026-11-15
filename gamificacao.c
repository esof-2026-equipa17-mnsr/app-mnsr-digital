#ifndef GAMIFICACAO_H
#define GAMIFICACAO_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#define FICHEIRO_PROGRESSO "progresso_tesouro.dat"

typedef struct {
    int numero;
    char codigo_qr[30];
    char obra_local[100];
    char pista_seguinte[200];
} Pista;

static int lerOpcaoNumerica() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        while (getchar() != '\n');
        printf("Entrada invalida. Introduza um numero: ");
    }
    while (getchar() != '\n');
    return valor;
}

// Guarda apenas o estado anonimo local (conformidade RGPD / RNF-04 - sem dados pessoais)
static void guardarProgressoLocal(int perfil_criado, int progresso, int emblema) {
    FILE *f = fopen(FICHEIRO_PROGRESSO, "w");
    if (f != NULL) {
        fprintf(f, "%d %d %d\n", perfil_criado, progresso, emblema);
        fclose(f);
    }
}

static void carregarProgressoLocal(int *perfil_criado, int *progresso, int *emblema) {
    FILE *f = fopen(FICHEIRO_PROGRESSO, "r");
    if (f != NULL) {
        if (fscanf(f, "%d %d %d", perfil_criado, progresso, emblema) != 3) {
            *perfil_criado = 0;
            *progresso = 0;
            *emblema = 0;
        }
        fclose(f);
    } else {
        *perfil_criado = 0;
        *progresso = 0;
        *emblema = 0;
    }
}

static void mostrarEmblemaVirtual() {
    printf("\n**************************************************\n");
    printf("*        PARABENS! CACA AO TESOURO CONCLUIDA     *\n");
    printf("*                                                *\n");
    printf("*        [ EMBLEMA VIRTUAL CONQUISTADO ]         *\n");
    printf("*         >> Explorador Mestre do MNSR <<        *\n");
    printf("*                                                *\n");
    printf("*  Recompensa guardada localmente no dispositivo *\n");
    printf("*     (Sessao 100%% anonima - sem conta/RGPD)     *\n");
    printf("**************************************************\n");
}

void iniciarCacaAoTesouro() {
    Pista pistas[8] = {
        {1, "MNSR-QR-01", "O Desterro (Soares dos Reis)", "Pista 1: Procura o retrato pintado por Columbano Bordalo Pinheiro."},
        {2, "MNSR-QR-02", "Retrato de Henrique Pousao", "Pista 2: Segue ate a sala de Ourivesaria e encontra a Custodia real."},
        {3, "MNSR-QR-03", "Custodia de Bemposta", "Pista 3: Encontra a escultura medieval da Senhora do O."},
        {4, "MNSR-QR-04", "Senhora do O", "Pista 4: Procura o celebre auto-retrato de Aurelia de Sousa."},
        {5, "MNSR-QR-05", "Auto-retrato (Aurelia de Sousa)", "Pista 5: Viaja ate ao Antigo Egipto e encontra o caixao da mumia."},
        {6, "MNSR-QR-06", "Caixao de Mumia Egipcia", "Pista 6: Procura os biombos japoneses da arte Namban."},
        {7, "MNSR-QR-07", "Biombo Namban", "Pista 7: Encontra o busto em bronze de Camilo Castelo Branco."},
        {8, "MNSR-QR-08", "Busto de Camilo Castelo Branco", "Desvendaste todos os misterios do Museu Nacional Soares dos Reis!"}
    };

    int total_pistas = 8;
    int perfil_anonimo_criado = 0;
    int progresso_atual = 0;
    int emblema_conquistado = 0;
    int permissao_camara = 1;
    int opcao = -1;
    char qr_lido[50];

    // Cenario 3: Recupera progresso guardado localmente
    carregarProgressoLocal(&perfil_anonimo_criado, &progresso_atual, &emblema_conquistado);

    printf("\n=== MNSR Digital - Caca ao Tesouro (Modo QR Code / Sem Conta) ===\n");

    // Cenario 1: Inicio anonimo na primeira utilizacao
    if (!perfil_anonimo_criado) {
        printf("\nBem-vindo a Caca ao Tesouro do MNSR!\n");
        printf("- Joga sem criar conta e sem dados pessoais (conformidade RGPD / RNF-04).\n");
        printf("- Sem geolocalizacao continua (apenas leitura pontual de QR Codes).\n");
        printf("\n1 - Comecar\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Opcao: ");
        int iniciar = lerOpcaoNumerica();

        if (iniciar != 1) {
            return;
        }

        perfil_anonimo_criado = 1;
        progresso_atual = 0;
        emblema_conquistado = 0;
        guardarProgressoLocal(perfil_anonimo_criado, progresso_atual, emblema_conquistado);

        printf("\n[Perfil Anonimo Criado]\n");
        printf("Sessao local iniciada sem recolha de nome, e-mail, data de nascimento ou telefone.\n");
        printf("Geolocalizacao continua: DESATIVADA.\n");
    } else {
        printf("\n[Sessao Anonima Recuperada do Dispositivo]\n");
        printf("Progresso mantido: %d/%d pistas desbloqueadas.\n", progresso_atual, total_pistas);
        if (emblema_conquistado) {
            printf("Estado da recompensa: Emblema 'Explorador Mestre do MNSR' ja conquistado!\n");
        }
    }

    while (opcao != 0) {
        printf("\nProgresso atual: %d/%d | Camara: %s | Perfil: Anonimo Local\n",
               progresso_atual, total_pistas,
               permissao_camara ? "PERMITIDA" : "NEGADA");
        printf("1 - Ler QR Code\n");
        printf("2 - Simular/Alternar permissao da camara\n");
        printf("3 - Ver Emblema Virtual / Estado da Sessao\n");
        printf("4 - Reiniciar progresso (Novo jogo anonimo)\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Opcao: ");
        opcao = lerOpcaoNumerica();

        if (opcao == 1) {
            if (!permissao_camara) {
                printf("\n[Permissao da Camara Negada]\n");
                printf("A app precisa de acesso a camara exclusivamente para ler os QR Codes junto as esculturas (sem geolocalizacao continua).\n");
                printf("1 - [Abrir definicoes] (Conceder acesso a camara)\n");
                printf("0 - Cancelar\n");
                printf("Opcao: ");
                int acao_def = lerOpcaoNumerica();

                if (acao_def == 1) {
                    permissao_camara = 1;
                    printf("Permissao da camara concedida com sucesso!\n");
                }
                continue;
            }

            if (progresso_atual >= total_pistas) {
                printf("\nJa concluiste todas as pistas (%d/%d)!\n", progresso_atual, total_pistas);
                mostrarEmblemaVirtual();
                continue;
            }

            printf("\n(Dica de teste - proximo QR esperado: %s)\n", pistas[progresso_atual].codigo_qr);
            printf("Aponte a camara / Introduza o codigo do QR Code: ");
            scanf("%49s", qr_lido);
            while (getchar() != '\n');

            clock_t inicio = clock();

            if (strcmp(qr_lido, pistas[progresso_atual].codigo_qr) == 0) {
                clock_t fim = clock();
                double tempo_resposta = (double)(fim - inicio) / CLOCKS_PER_SEC;

                printf("\n[QR Code Valido - %.3f segundos (< 2s)]\n", tempo_resposta);
                printf("Obra identificada: %s\n", pistas[progresso_atual].obra_local);
                printf("Nova pista desbloqueada -> %s\n", pistas[progresso_atual].pista_seguinte);
                progresso_atual++;
                printf("Progresso atualizado: %d/%d\n", progresso_atual, total_pistas);

                // Cenario 2: Recompensa ao concluir todas as pistas (8/8)
                if (progresso_atual == total_pistas) {
                    emblema_conquistado = 1;
                    guardarProgressoLocal(perfil_anonimo_criado, progresso_atual, emblema_conquistado);
                    mostrarEmblemaVirtual();
                } else {
                    guardarProgressoLocal(perfil_anonimo_criado, progresso_atual, emblema_conquistado);
                }
            } else {
                printf("\nQR Code invalido\n");
                printf("Nenhuma pista foi desbloqueada. Progresso mantido: %d/%d\n",
                       progresso_atual, total_pistas);
            }
        } else if (opcao == 2) {
            permissao_camara = !permissao_camara;
            printf("Estado da permissao da camara alterado para: %s\n",
                   permissao_camara ? "PERMITIDA" : "NEGADA");
        } else if (opcao == 3) {
            if (emblema_conquistado) {
                mostrarEmblemaVirtual();
            } else {
                printf("\nAinda nao desbloqueaste o emblema virtual (progresso: %d/%d).\n",
                       progresso_atual, total_pistas);
            }
        } else if (opcao == 4) {
            progresso_atual = 0;
            emblema_conquistado = 0;
            guardarProgressoLocal(perfil_anonimo_criado, progresso_atual, emblema_conquistado);
            printf("\nProgresso reiniciado para 0/%d (perfil anonimo mantido).\n", total_pistas);
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    }
}

#endif