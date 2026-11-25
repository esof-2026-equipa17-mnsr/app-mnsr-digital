#ifndef GAMIFICACAO_H
#define GAMIFICACAO_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#define FICHEIRO_PROGRESSO "progresso_tesouro.dat"
#define TOTAL_PISTAS 8
#define MAX_CODIGO_QR 30
#define MAX_OBRA_LOCAL 100
#define MAX_PISTA_TEXTO 200
#define MAX_ENTRADA_QR 50

typedef enum {
    ESTADO_INATIVO = 0,
    ESTADO_ATIVO = 1
} EstadoFlag;

typedef enum {
    ACAO_CANCELAR = 0,
    ACAO_CONFIRMAR = 1
} ConfirmacaoAcao;

typedef enum {
    OPCAO_VOLTAR = 0,
    OPCAO_LER_QR = 1,
    OPCAO_ALTERNAR_CAMARA = 2,
    OPCAO_VER_EMBLEMA = 3,
    OPCAO_REINICIAR = 4
} OpcaoMenuTesouro;

typedef struct {
    int numero;
    char codigo_qr[MAX_CODIGO_QR];
    char obra_local[MAX_OBRA_LOCAL];
    char pista_seguinte[MAX_PISTA_TEXTO];
} Pista;

static void limparBufferEntrada(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

static int lerOpcaoNumerica(void) {
    int valor = 0;
    while (scanf("%d", &valor) != 1) {
        limparBufferEntrada();
        printf("Entrada invalida. Introduza um numero: ");
    }
    limparBufferEntrada();
    return valor;
}

// Guarda apenas o estado anonimo local (conformidade RGPD / RNF-04 - sem dados pessoais)
static void guardarProgressoLocal(const int perfil_criado, const int progresso, const int emblema) {
    FILE *ficheiro = fopen(FICHEIRO_PROGRESSO, "w");
    if (ficheiro != NULL) {
        fprintf(ficheiro, "%d %d %d\n", perfil_criado, progresso, emblema);
        fclose(ficheiro);
    }
}

static void carregarProgressoLocal(int *perfil_criado, int *progresso, int *emblema) {
    *perfil_criado = ESTADO_INATIVO;
    *progresso = 0;
    *emblema = ESTADO_INATIVO;

    FILE *ficheiro = fopen(FICHEIRO_PROGRESSO, "r");
    if (ficheiro != NULL) {
        if (fscanf(ficheiro, "%d %d %d", perfil_criado, progresso, emblema) != 3) {
            *perfil_criado = ESTADO_INATIVO;
            *progresso = 0;
            *emblema = ESTADO_INATIVO;
        }
        fclose(ficheiro);
    }
}

static void mostrarEmblemaVirtual(void) {
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

static int inicializarSessaoAnonima(int *perfil_criado, int *progresso, int *emblema) {
    if (*perfil_criado == ESTADO_ATIVO) {
        printf("\n[Sessao Anonima Recuperada do Dispositivo]\n");
        printf("Progresso mantido: %d/%d pistas desbloqueadas.\n", *progresso, TOTAL_PISTAS);
        if (*emblema == ESTADO_ATIVO) {
            printf("Estado da recompensa: Emblema 'Explorador Mestre do MNSR' ja conquistado!\n");
        }
        return ESTADO_ATIVO;
    }

    printf("\nBem-vindo a Caca ao Tesouro do MNSR!\n");
    printf("- Joga sem criar conta e sem dados pessoais (conformidade RGPD / RNF-04).\n");
    printf("- Sem geolocalizacao continua (apenas leitura pontual de QR Codes).\n");
    printf("\n1 - Comecar\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Opcao: ");

    const int iniciar = lerOpcaoNumerica();
    if (iniciar != ACAO_CONFIRMAR) {
        return ESTADO_INATIVO;
    }

    *perfil_criado = ESTADO_ATIVO;
    *progresso = 0;
    *emblema = ESTADO_INATIVO;
    guardarProgressoLocal(*perfil_criado, *progresso, *emblema);

    printf("\n[Perfil Anonimo Criado]\n");
    printf("Sessao local iniciada sem recolha de nome, e-mail, data de nascimento ou telefone.\n");
    printf("Geolocalizacao continua: DESATIVADA.\n");
    return ESTADO_ATIVO;
}

static void exibirMenuTesouro(const int progresso, const int permissao_camara) {
    printf("\nProgresso atual: %d/%d | Camara: %s | Perfil: Anonimo Local\n",
           progresso, TOTAL_PISTAS,
           permissao_camara == ESTADO_ATIVO ? "PERMITIDA" : "NEGADA");
    printf("1 - Ler QR Code\n");
    printf("2 - Simular/Alternar permissao da camara\n");
    printf("3 - Ver Emblema Virtual / Estado da Sessao\n");
    printf("4 - Reiniciar progresso (Novo jogo anonimo)\n");
    printf("0 - Voltar ao menu principal\n");
    printf("Opcao: ");
}

static int garantirPermissaoCamara(int *permissao_camara) {
    if (*permissao_camara == ESTADO_ATIVO) {
        return ESTADO_ATIVO;
    }

    printf("\n[Permissao da Camara Negada]\n");
    printf("A app precisa de acesso a camara exclusivamente para ler os QR Codes junto as esculturas (sem geolocalizacao continua).\n");
    printf("1 - [Abrir definicoes] (Conceder acesso a camara)\n");
    printf("0 - Cancelar\n");
    printf("Opcao: ");

    const int acao_definicoes = lerOpcaoNumerica();
    if (acao_definicoes == ACAO_CONFIRMAR) {
        *permissao_camara = ESTADO_ATIVO;
        printf("Permissao da camara concedida com sucesso!\n");
    }
    return ESTADO_INATIVO;
}

static void registarDescobertaPista(const Pista *pista, const int perfil_criado,
                                   int *progresso, int *emblema, const double tempo_resposta) {
    printf("\n[QR Code Valido - %.3f segundos (< 2s)]\n", tempo_resposta);
    printf("Obra identificada: %s\n", pista->obra_local);
    printf("Nova pista desbloqueada -> %s\n", pista->pista_seguinte);

    (*progresso)++;
    printf("Progresso atualizado: %d/%d\n", *progresso, TOTAL_PISTAS);

    if (*progresso == TOTAL_PISTAS) {
        *emblema = ESTADO_ATIVO;
        guardarProgressoLocal(perfil_criado, *progresso, *emblema);
        mostrarEmblemaVirtual();
    } else {
        guardarProgressoLocal(perfil_criado, *progresso, *emblema);
    }
}

static void processarLeituraQrCode(const Pista pistas[], const int perfil_criado,
                                   int *progresso, int *emblema) {
    if (*progresso >= TOTAL_PISTAS) {
        printf("\nJa concluiste todas as pistas (%d/%d)!\n", *progresso, TOTAL_PISTAS);
        mostrarEmblemaVirtual();
        return;
    }

    char qr_lido[MAX_ENTRADA_QR];
    const Pista *pista_atual = &pistas[*progresso];

    printf("\n(Dica de teste - proximo QR esperado: %s)\n", pista_atual->codigo_qr);
    printf("Aponte a camara / Introduza o codigo do QR Code: ");
    scanf("%49s", qr_lido);
    limparBufferEntrada();

    const clock_t inicio = clock();
    if (strcmp(qr_lido, pista_atual->codigo_qr) == 0) {
        const clock_t fim = clock();
        const double tempo_resposta = (double)(fim - inicio) / CLOCKS_PER_SEC;
        registarDescobertaPista(pista_atual, perfil_criado, progresso, emblema, tempo_resposta);
    } else {
        printf("\nQR Code invalido\n");
        printf("Nenhuma pista foi desbloqueada. Progresso mantido: %d/%d\n",
               *progresso, TOTAL_PISTAS);
    }
}

static void consultarEstadoEmblema(const int progresso, const int emblema) {
    if (emblema == ESTADO_ATIVO) {
        mostrarEmblemaVirtual();
    } else {
        printf("\nAinda nao desbloqueaste o emblema virtual (progresso: %d/%d).\n",
               progresso, TOTAL_PISTAS);
    }
}

static void reiniciarProgressoJogo(const int perfil_criado, int *progresso, int *emblema) {
    *progresso = 0;
    *emblema = ESTADO_INATIVO;
    guardarProgressoLocal(perfil_criado, *progresso, *emblema);
    printf("\nProgresso reiniciado para 0/%d (perfil anonimo mantido).\n", TOTAL_PISTAS);
}

void iniciarCacaAoTesouro() {
    const Pista pistas[TOTAL_PISTAS] = {
        {.numero = 1, .codigo_qr = "MNSR-QR-01", .obra_local = "O Desterro (Soares dos Reis)", .pista_seguinte = "Pista 1: Procura o retrato pintado por Columbano Bordalo Pinheiro."},
        {.numero = 2, .codigo_qr = "MNSR-QR-02", .obra_local = "Retrato de Henrique Pousao", .pista_seguinte = "Pista 2: Segue ate a sala de Ourivesaria e encontra a Custodia real."},
        {.numero = 3, .codigo_qr = "MNSR-QR-03", .obra_local = "Custodia de Bemposta", .pista_seguinte = "Pista 3: Encontra a escultura medieval da Senhora do O."},
        {.numero = 4, .codigo_qr = "MNSR-QR-04", .obra_local = "Senhora do O", .pista_seguinte = "Pista 4: Procura o celebre auto-retrato de Aurelia de Sousa."},
        {.numero = 5, .codigo_qr = "MNSR-QR-05", .obra_local = "Auto-retrato (Aurelia de Sousa)", .pista_seguinte = "Pista 5: Viaja ate ao Antigo Egipto e encontra o caixao da mumia."},
        {.numero = 6, .codigo_qr = "MNSR-QR-06", .obra_local = "Caixao de Mumia Egipcia", .pista_seguinte = "Pista 6: Procura os biombos japoneses da arte Namban."},
        {.numero = 7, .codigo_qr = "MNSR-QR-07", .obra_local = "Biombo Namban", .pista_seguinte = "Pista 7: Encontra o busto em bronze de Camilo Castelo Branco."},
        {.numero = 8, .codigo_qr = "MNSR-QR-08", .obra_local = "Busto de Camilo Castelo Branco", .pista_seguinte = "Desvendaste todos os misterios do Museu Nacional Soares dos Reis!"}
    };

    int perfil_anonimo_criado = ESTADO_INATIVO;
    int progresso_atual = 0;
    int emblema_conquistado = ESTADO_INATIVO;
    int permissao_camara = ESTADO_ATIVO;
    int opcao = -1;

    carregarProgressoLocal(&perfil_anonimo_criado, &progresso_atual, &emblema_conquistado);

    printf("\n=== MNSR Digital - Caca ao Tesouro (Modo QR Code / Sem Conta) ===\n");
    if (inicializarSessaoAnonima(&perfil_anonimo_criado, &progresso_atual, &emblema_conquistado) == ESTADO_INATIVO) {
        return;
    }

    while (opcao != OPCAO_VOLTAR) {
        exibirMenuTesouro(progresso_atual, permissao_camara);
        opcao = lerOpcaoNumerica();

        if (opcao == OPCAO_LER_QR) {
            if (garantirPermissaoCamara(&permissao_camara) == ESTADO_ATIVO) {
                processarLeituraQrCode(pistas, perfil_anonimo_criado, &progresso_atual, &emblema_conquistado);
            }
        } else if (opcao == OPCAO_ALTERNAR_CAMARA) {
            permissao_camara = (permissao_camara == ESTADO_ATIVO) ? ESTADO_INATIVO : ESTADO_ATIVO;
            printf("Estado da permissao da camara alterado para: %s\n",
                   permissao_camara == ESTADO_ATIVO ? "PERMITIDA" : "NEGADA");
        } else if (opcao == OPCAO_VER_EMBLEMA) {
            consultarEstadoEmblema(progresso_atual, emblema_conquistado);
        } else if (opcao == OPCAO_REINICIAR) {
            reiniciarProgressoJogo(perfil_anonimo_criado, &progresso_atual, &emblema_conquistado);
        } else if (opcao != OPCAO_VOLTAR) {
            printf("Opcao invalida.\n");
        }
    }
}

#endif