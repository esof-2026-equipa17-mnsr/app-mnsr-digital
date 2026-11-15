#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char nome[100];
    char tema[50];
    int tempo_estimado;
    int visitada; // 0 = por visitar, 1 = ja visitada
} Obra;

typedef struct {
    char tema[50];
    int tempo_escolhido;
    int tempo_total;
    int total_obras;
    Obra obras[25];
} RoteiroCache;

void guardarRoteiroCache(const char *tema, int tempo_escolhido, int tempo_total, Obra obras_selecionadas[], int total_selecionadas) {
    FILE *f = fopen("cache_roteiro.dat", "wb");
    if (f == NULL) return;

    RoteiroCache cache;
    snprintf(cache.tema, sizeof(cache.tema), "%s", tema);
    cache.tempo_escolhido = tempo_escolhido;
    cache.tempo_total = tempo_total;
    cache.total_obras = total_selecionadas;
    for (int i = 0; i < total_selecionadas && i < 25; i++) {
        cache.obras[i] = obras_selecionadas[i];
    }

    fwrite(&cache, sizeof(RoteiroCache), 1, f);
    fclose(f);
}

int carregarRoteiroCache() {
    clock_t inicio = clock();
    FILE *f = fopen("cache_roteiro.dat", "rb");
    if (f == NULL) {
        printf("\n[Modo Offline] Nenhum roteiro guardado em cache local.\n");
        return 0;
    }

    RoteiroCache cache;
    size_t lidos = fread(&cache, sizeof(RoteiroCache), 1, f);
    fclose(f);

    if (lidos != 1) {
        printf("\n[Modo Offline] Erro ao ler a cache local.\n");
        return 0;
    }

    clock_t fim = clock();
    double tempo_leitura = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n=== Roteiro Guardado [Modo Offline] (Carregado em %.3f s < 1.0s) ===\n", tempo_leitura);
    printf("Tema: %s | Duracao alvo: %d min | Tempo estimado: %d min (%d obras)\n",
           cache.tema, cache.tempo_escolhido, cache.tempo_total, cache.total_obras);
    for (int i = 0; i < cache.total_obras; i++) {
        printf("%d. %s (%s) | Demora: %d min\n",
               i + 1, cache.obras[i].nome, cache.obras[i].tema, cache.obras[i].tempo_estimado);
    }
    return 1;
}

void gerarRoteiroPorTempo(Obra acervo[], int total_obras, int tempo_disponivel) {
    int tempo_acumulado = 0;

    printf("--- Roteiro Sugerido (Tempo max: %d minutos) ---\n", tempo_disponivel);
    for (int i = 0; i < total_obras; i++) {
        if (!acervo[i].visitada && (tempo_acumulado + acervo[i].tempo_estimado <= tempo_disponivel)) {
            printf("-> %s (%s) | Demora: %d min\n",
                   acervo[i].nome, acervo[i].tema, acervo[i].tempo_estimado);
            tempo_acumulado += acervo[i].tempo_estimado;
        }
    }
    printf("\nTempo total preenchido: %d minutos\n", tempo_acumulado);
}

int gerarRoteiroPorTema(Obra acervo[], int total_obras, int tempo_disponivel, const char *tema_escolhido, int indices_roteiro[]) {
    int tempo_acumulado = 0;
    int obras_encontradas = 0;
    int tempo_total_tema = 0;
    Obra selecionadas[25];

    // Calcula o tempo total disponivel do tema selecionado no acervo
    for (int i = 0; i < total_obras; i++) {
        int corresponde_tema = (strcmp(tema_escolhido, "Todos") == 0) ||
                               (strcmp(acervo[i].tema, tema_escolhido) == 0);
        if (!acervo[i].visitada && corresponde_tema) {
            tempo_total_tema += acervo[i].tempo_estimado;
        }
    }

    printf("\n--- Roteiro Sugerido | Tema: %s | Tempo max: %d minutos ---\n",
           tema_escolhido, tempo_disponivel);

    for (int i = 0; i < total_obras; i++) {
        int corresponde_tema = (strcmp(tema_escolhido, "Todos") == 0) ||
                               (strcmp(acervo[i].tema, tema_escolhido) == 0);

        if (!acervo[i].visitada && corresponde_tema && (tempo_acumulado + acervo[i].tempo_estimado <= tempo_disponivel)) {
            printf("%d. %s (%s) | Demora: %d min\n",
                   obras_encontradas + 1, acervo[i].nome, acervo[i].tema, acervo[i].tempo_estimado);
            tempo_acumulado += acervo[i].tempo_estimado;
            if (indices_roteiro != NULL) {
                indices_roteiro[obras_encontradas] = i;
            }
            if (obras_encontradas < 25) {
                selecionadas[obras_encontradas] = acervo[i];
            }
            obras_encontradas++;
        }
    }

    if (obras_encontradas == 0) {
        printf("Nenhuma obra encontrada para o tema '%s' dentro do tempo disponivel.\n", tema_escolhido);
    } else {
        printf("\nTotal de obras: %d | Tempo total preenchido: %d minutos\n",
               obras_encontradas, tempo_acumulado);

        // Cenario 2 (#4): Tema com conteudo insuficiente para a duracao escolhida
        if (strcmp(tema_escolhido, "Todos") != 0 && tempo_total_tema < tempo_disponivel) {
            printf("[Aviso] O tema '%s' tem apenas %d minutos de conteudo disponivel (inferior aos %d minutos escolhidos).\n",
                   tema_escolhido, tempo_total_tema, tempo_disponivel);
            printf("[Sugestao] Adicione outro tema ou escolha 'Todos os Temas' para aproveitar o restante tempo da visita.\n");
        }

        // Guarda em cache local para acesso offline (#3 - Cenario 3)
        guardarRoteiroCache(tema_escolhido, tempo_disponivel, tempo_acumulado, selecionadas, obras_encontradas);
    }

    return obras_encontradas;
}

int reajustarRoteiro(Obra acervo[], int total_obras, int tempo_restante, const char *tema_escolhido) {
    int tempo_acumulado = 0;
    int obras_encontradas = 0;

    printf("\n=== Roteiro Reajustado [Modo Offline / Cache] | Tempo restante: %d min ===\n", tempo_restante);

    if (tempo_restante > 0) {
        for (int i = 0; i < total_obras; i++) {
            int corresponde_tema = (strcmp(tema_escolhido, "Todos") == 0) ||
                                   (strcmp(acervo[i].tema, tema_escolhido) == 0);

            if (!acervo[i].visitada && corresponde_tema && (tempo_acumulado + acervo[i].tempo_estimado <= tempo_restante)) {
                printf("%d. %s (%s) | Demora: %d min\n",
                       obras_encontradas + 1, acervo[i].nome, acervo[i].tema, acervo[i].tempo_estimado);
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