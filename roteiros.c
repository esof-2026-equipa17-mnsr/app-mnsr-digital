#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char nome[100];
    char tema[50];
    int tempo_estimado;
} Obra;

void gerarRoteiroPorTempo(Obra acervo[], int total_obras, int tempo_disponivel) {
    int tempo_acumulado = 0;

    printf("--- Roteiro Sugerido (Tempo max: %d minutos) ---\n", tempo_disponivel);
    for (int i = 0; i < total_obras; i++) {
        if (tempo_acumulado + acervo[i].tempo_estimado <= tempo_disponivel) {
            printf("-> %s (%s) | Demora: %d min\n",
                   acervo[i].nome, acervo[i].tema, acervo[i].tempo_estimado);
            tempo_acumulado += acervo[i].tempo_estimado;
        }
    }
    printf("\nTempo total preenchido: %d minutos\n", tempo_acumulado);
}

void gerarRoteiroPorTema(Obra acervo[], int total_obras, int tempo_disponivel, const char *tema_escolhido) {
    int tempo_acumulado = 0;
    int obras_encontradas = 0;

    printf("--- Roteiro Sugerido | Tema: %s | Tempo max: %d minutos ---\n",
           tema_escolhido, tempo_disponivel);

    for (int i = 0; i < total_obras; i++) {
        int corresponde_tema = (strcmp(tema_escolhido, "Todos") == 0) ||
                               (strcmp(acervo[i].tema, tema_escolhido) == 0);

        if (corresponde_tema && (tempo_acumulado + acervo[i].tempo_estimado <= tempo_disponivel)) {
            printf("-> %s (%s) | Demora: %d min\n",
                   acervo[i].nome, acervo[i].tema, acervo[i].tempo_estimado);
            tempo_acumulado += acervo[i].tempo_estimado;
            obras_encontradas++;
        }
    }

    if (obras_encontradas == 0) {
        printf("Nenhuma obra encontrada para o tema '%s' dentro do tempo disponivel.\n", tema_escolhido);
    } else {
        printf("\nTotal de obras: %d | Tempo total preenchido: %d minutos\n",
               obras_encontradas, tempo_acumulado);
    }
}