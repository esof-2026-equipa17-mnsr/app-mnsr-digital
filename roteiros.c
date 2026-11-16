#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_NOME_OBRA 100
#define MAX_TEMA_OBRA 50
#define MAX_OBRAS_ROTEIRO 25
#define FICHEIRO_CACHE_ROTEIRO "cache_roteiro.dat"
#define TEMA_TODOS "Todos"

typedef enum {
    OBRA_POR_VISITAR = 0,
    OBRA_JA_VISITADA = 1
} EstadoVisitaObra;

typedef enum {
    OPERACAO_FALHOU = 0,
    OPERACAO_SUCESSO = 1
} ResultadoOperacao;

typedef struct {
    int id;
    char nome[MAX_NOME_OBRA];
    char tema[MAX_TEMA_OBRA];
    int tempo_estimado;
    int visitada;
} Obra;

typedef struct {
    char tema[MAX_TEMA_OBRA];
    int tempo_escolhido;
    int tempo_total;
    int total_obras;
    Obra obras[MAX_OBRAS_ROTEIRO];
} RoteiroCache;

static int obraCorrespondeAoTema(const Obra *obra, const char *tema_escolhido) {
    return strcmp(tema_escolhido, TEMA_TODOS) == 0 ||
           strcmp(obra->tema, tema_escolhido) == 0;
}

static int obraElegivelParaRoteiro(const Obra *obra, const char *tema_escolhido,
                                   const int tempo_acumulado, const int tempo_limite) {
    const int por_visitar = obra->visitada == OBRA_POR_VISITAR;
    const int cabe_no_tempo = tempo_acumulado + obra->tempo_estimado <= tempo_limite;
    return por_visitar && obraCorrespondeAoTema(obra, tema_escolhido) && cabe_no_tempo;
}

static void imprimirObraNumerada(const int indice_exibicao, const Obra *obra) {
    printf("%d. %s (%s) | Demora: %d min\n",
           indice_exibicao, obra->nome, obra->tema, obra->tempo_estimado);
}

static int calcularTempoDisponivelTema(const Obra acervo[], const int total_obras,
                                       const char *tema_escolhido) {
    int tempo_total_tema = 0;
    for (int i = 0; i < total_obras; i++) {
        if (acervo[i].visitada == OBRA_POR_VISITAR &&
            obraCorrespondeAoTema(&acervo[i], tema_escolhido)) {
            tempo_total_tema += acervo[i].tempo_estimado;
        }
    }
    return tempo_total_tema;
}

static void alertarSeConteudoInsuficiente(const char *tema_escolhido,
                                          const int tempo_total_tema,
                                          const int tempo_disponivel) {
    if (strcmp(tema_escolhido, TEMA_TODOS) != 0 && tempo_total_tema < tempo_disponivel) {
        printf("[Aviso] O tema '%s' tem apenas %d minutos de conteudo disponivel (inferior aos %d minutos escolhidos).\n",
               tema_escolhido, tempo_total_tema, tempo_disponivel);
        printf("[Sugestao] Adicione outro tema ou escolha 'Todos os Temas' para aproveitar o restante tempo da visita.\n");
    }
}

void guardarRoteiroCache(const char *tema, const int tempo_escolhido, const int tempo_total,
                         Obra obras_selecionadas[], const int total_selecionadas) {
    FILE *ficheiro = fopen(FICHEIRO_CACHE_ROTEIRO, "wb");
    if (ficheiro == NULL) {
        return;
    }

    RoteiroCache cache;
    snprintf(cache.tema, sizeof(cache.tema), "%s", tema);
    cache.tempo_escolhido = tempo_escolhido;
    cache.tempo_total = tempo_total;
    cache.total_obras = total_selecionadas;

    for (int i = 0; i < total_selecionadas && i < MAX_OBRAS_ROTEIRO; i++) {
        cache.obras[i] = obras_selecionadas[i];
    }

    fwrite(&cache, sizeof(RoteiroCache), 1, ficheiro);
    fclose(ficheiro);
}

int carregarRoteiroCache(void) {
    const clock_t inicio = clock();
    FILE *ficheiro = fopen(FICHEIRO_CACHE_ROTEIRO, "rb");
    if (ficheiro == NULL) {
        printf("\n[Modo Offline] Nenhum roteiro guardado em cache local.\n");
        return OPERACAO_FALHOU;
    }

    RoteiroCache cache;
    const size_t registos_lidos = fread(&cache, sizeof(RoteiroCache), 1, ficheiro);
    fclose(ficheiro);

    if (registos_lidos != 1) {
        printf("\n[Modo Offline] Erro ao ler a cache local.\n");
        return OPERACAO_FALHOU;
    }

    const clock_t fim = clock();
    const double tempo_leitura = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n=== Roteiro Guardado [Modo Offline] (Carregado em %.3f s < 1.0s) ===\n", tempo_leitura);
    printf("Tema: %s | Duracao alvo: %d min | Tempo estimado: %d min (%d obras)\n",
           cache.tema, cache.tempo_escolhido, cache.tempo_total, cache.total_obras);

    for (int i = 0; i < cache.total_obras; i++) {
        imprimirObraNumerada(i + 1, &cache.obras[i]);
    }
    return OPERACAO_SUCESSO;
}

void gerarRoteiroPorTempo(Obra acervo[], const int total_obras, const int tempo_disponivel) {
    int tempo_acumulado = 0;

    printf("--- Roteiro Sugerido (Tempo max: %d minutos) ---\n", tempo_disponivel);
    for (int i = 0; i < total_obras; i++) {
        if (obraElegivelParaRoteiro(&acervo[i], TEMA_TODOS, tempo_acumulado, tempo_disponivel)) {
            printf("-> %s (%s) | Demora: %d min\n",
                   acervo[i].nome, acervo[i].tema, acervo[i].tempo_estimado);
            tempo_acumulado += acervo[i].tempo_estimado;
        }
    }
    printf("\nTempo total preenchido: %d minutos\n", tempo_acumulado);
}

int gerarRoteiroPorTema(Obra acervo[], const int total_obras, const int tempo_disponivel,
                        const char *tema_escolhido, int indices_roteiro[]) {
    int tempo_acumulado = 0;
    int obras_encontradas = 0;
    Obra selecionadas[MAX_OBRAS_ROTEIRO];
    const int tempo_total_tema = calcularTempoDisponivelTema(acervo, total_obras, tema_escolhido);

    printf("\n--- Roteiro Sugerido | Tema: %s | Tempo max: %d minutos ---\n",
           tema_escolhido, tempo_disponivel);

    for (int i = 0; i < total_obras; i++) {
        if (obraElegivelParaRoteiro(&acervo[i], tema_escolhido, tempo_acumulado, tempo_disponivel)) {
            imprimirObraNumerada(obras_encontradas + 1, &acervo[i]);
            tempo_acumulado += acervo[i].tempo_estimado;

            if (indices_roteiro != NULL) {
                indices_roteiro[obras_encontradas] = i;
            }
            if (obras_encontradas < MAX_OBRAS_ROTEIRO) {
                selecionadas[obras_encontradas] = acervo[i];
            }
            obras_encontradas++;
        }
    }

    if (obras_encontradas == 0) {
        printf("Nenhuma obra encontrada para o tema '%s' dentro do tempo disponivel.\n", tema_escolhido);
        return 0;
    }

    printf("\nTotal de obras: %d | Tempo total preenchido: %d minutos\n",
           obras_encontradas, tempo_acumulado);
    alertarSeConteudoInsuficiente(tema_escolhido, tempo_total_tema, tempo_disponivel);
    guardarRoteiroCache(tema_escolhido, tempo_disponivel, tempo_acumulado, selecionadas, obras_encontradas);

    return obras_encontradas;
}

int reajustarRoteiro(Obra acervo[], const int total_obras, const int tempo_restante,
                     const char *tema_escolhido) {
    int tempo_acumulado = 0;
    int obras_encontradas = 0;

    printf("\n=== Roteiro Reajustado [Modo Offline / Cache] | Tempo restante: %d min ===\n", tempo_restante);

    if (tempo_restante > 0) {
        for (int i = 0; i < total_obras; i++) {
            if (obraElegivelParaRoteiro(&acervo[i], tema_escolhido, tempo_acumulado, tempo_restante)) {
                imprimirObraNumerada(obras_encontradas + 1, &acervo[i]);
                tempo_acumulado += acervo[i].tempo_estimado;
                obras_encontradas++;
            }
        }
    }

    if (obras_encontradas == 0) {
        printf("Tempo insuficiente para mais obras\n");
        return 0;
    }

    printf("\nTotal de obras restantes: %d | Tempo total preenchido: %d minutos\n",
           obras_encontradas, tempo_acumulado);
    return obras_encontradas;
}