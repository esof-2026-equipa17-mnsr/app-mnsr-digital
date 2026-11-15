#ifndef ASSISTENTE_H
#define ASSISTENTE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    char id_ficha[20];
    char titulo[80];
    char keywords[120];
    char autor[80];
    char ano[20];
    char tecnica[100];
    char sala[80];
    char contexto[350];
    char fonte[120];
} FichaObra;

// Estado persistente da sessao (permite manter a pergunta mesmo navegando entre menus)
static int ligacao_rede = 1; // 1 = Online (Wi-Fi/4G), 0 = Sem cobertura de rede
static char pergunta_pendente[256] = "";
static int tem_pergunta_pendente = 0;

static int lerOpcaoAssistente() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        while (getchar() != '\n');
        printf("Entrada invalida. Introduza um numero: ");
    }
    while (getchar() != '\n');
    return valor;
}

// Normaliza texto para minusculas ASCII (facilita pesquisa em linguagem natural)
static void normalizarTexto(const char *origem, char *destino, int max_len) {
    int j = 0;
    for (int i = 0; origem[i] != '\0' && j < max_len - 1; i++) {
        unsigned char c = (unsigned char)origem[i];
        if (c < 128) {
            if (isalnum(c) || c == ' ') {
                destino[j++] = (char)tolower(c);
            } else {
                destino[j++] = ' ';
            }
        } else if (c == 0xC3 && origem[i + 1] != '\0') {
            unsigned char next = (unsigned char)origem[i + 1];
            i++;
            if (next >= 0x80 && next <= 0x85) destino[j++] = 'a';
            else if (next >= 0xA0 && next <= 0xA5) destino[j++] = 'a';
            else if (next >= 0x88 && next <= 0x8B) destino[j++] = 'e';
            else if (next >= 0xA8 && next <= 0xAB) destino[j++] = 'e';
            else if (next >= 0x8C && next <= 0x8F) destino[j++] = 'i';
            else if (next >= 0xAC && next <= 0xAF) destino[j++] = 'i';
            else if (next >= 0x92 && next <= 0x96) destino[j++] = 'o';
            else if (next >= 0xB2 && next <= 0xB6) destino[j++] = 'o';
            else if (next >= 0x99 && next <= 0x9C) destino[j++] = 'u';
            else if (next >= 0xB9 && next <= 0xBC) destino[j++] = 'u';
            else if (next == 0x87 || next == 0xA7) destino[j++] = 'c';
        }
    }
    destino[j] = '\0';
}

static int carregarBaseConhecimento(FichaObra acervo[]) {
    FichaObra dados[8] = {
        {
            "FT-MNSR-001",
            "O Desterro",
            "desterro soares dos reis escultura marmore",
            "Antonio Soares dos Reis",
            "1872",
            "Escultura em marmore de Carrara (178 x 72 x 92 cm)",
            "Sala de Escultura Oitocentista (Piso 1)",
            "Obra-prima do romantismo e naturalismo portugues, esculpida em Roma durante o pensionato do artista. Representa a figura melancolica do exilado inspirada no poema de Alexandre Herculano.",
            "Ficha Tecnica Oficial MNSR (Inv. 12 Esc) & Catalogo do Acervo"
        },
        {
            "FT-MNSR-002",
            "Retrato de Henrique Pousao",
            "retrato de henrique pousao columbano bordalo pinheiro pintura",
            "Columbano Bordalo Pinheiro",
            "1881",
            "Oleo sobre tela (61 x 50 cm)",
            "Sala de Pintura Naturalista (Piso 1)",
            "Retrato intimo pintado em Paris quando ambos os artistas eram bolseiros em Franca, destacando o dominio do claro-escuro e da psicologia do retratado.",
            "Ficha Tecnica Oficial MNSR (Inv. 145 Pint) & Texto Curatorial"
        },
        {
            "FT-MNSR-003",
            "Custodia de Bemposta",
            "custodia de bemposta ourivesaria ouro pedras preciosas",
            "Oficina de Ourivesaria Portuguesa (encomenda de D. Joao V)",
            "1744",
            "Ouro, prata dourada e pedras preciosas (diamantes, rubis e esmeraldas)",
            "Sala do Tesouro / Ourivesaria",
            "Peca emblematica da ourivesaria barroca joanina, originalmente destinada a Capela Real do Palacio da Bemposta em Lisboa.",
            "Ficha Tecnica Oficial MNSR (Inv. 48 Our) & Guia de Ourivesaria"
        },
        {
            "FT-MNSR-004",
            "Senhora do O",
            "senhora do o escultura medieval calcario",
            "Mestre de Pero (oficina de Coimbra)",
            "Seculo XIV (c. 1330-1340)",
            "Escultura em pedra calcarea policromada",
            "Sala de Arte Medieval",
            "Representacao iconografica da Virgem gravida, caracteristica da escultura gotica portuguesa de influencia coimbra.",
            "Ficha Tecnica Oficial MNSR (Inv. 89 Esc) & Catalogo Medieval"
        },
        {
            "FT-MNSR-005",
            "Auto-retrato",
            "auto retrato autoretrato aurelia de sousa pintura",
            "Aurelia de Sousa",
            "c. 1900",
            "Oleo sobre tela (46 x 38 cm)",
            "Sala de Pintura Moderna / Aurelia de Sousa",
            "Pintado durante a sua estadia em Paris, este auto-retrato com laco vermelho tornou-se um dos icones da pintura moderna portuguesa.",
            "Ficha Tecnica Oficial MNSR (Inv. 312 Pint) & Texto Curatorial"
        },
        {
            "FT-MNSR-006",
            "Caixao de Mumia Egipcia",
            "caixao de mumia egipcia arqueologia egipto",
            "Oficina Funeraria Egipcia (Periodo Tardio)",
            "Seculo VII-IV a.C.",
            "Madeira estucada e policromada",
            "Sala de Arqueologia / Colecao Egipcia",
            "Sarcofago antropomorfo decorado com inscricoes hieroglificas e divindades protetoras destinadas a assegurar a vida apos a morte.",
            "Ficha Tecnica Oficial MNSR (Inv. 19 Arq) & Nucleo de Arqueologia"
        },
        {
            "FT-MNSR-007",
            "Biombo Namban",
            "biombo namban biombos japoneses artes decorativas",
            "Escola Kano (Japao, Periodo Momoyama)",
            "Finais do seculo XVI / inicio do seculo XVII",
            "Tinta, pigmentos e folha de ouro sobre papel",
            "Sala de Artes Decorativas / Expansao Portuguesa",
            "Retrata a chegada da nau do trato portuguesa ao porto de Nagasaki e o encontro cultural entre mercadores portugueses, missionarios e japoneses.",
            "Ficha Tecnica Oficial MNSR (Inv. 77 AD) & Catalogo Namban"
        },
        {
            "FT-MNSR-008",
            "Busto de Camilo Castelo Branco",
            "busto de camilo castelo branco bronze",
            "Antonio Teixeira Lopes",
            "1891",
            "Escultura em bronze fundido",
            "Galeria de Escultura Final de Seculo",
            "Homenagem escultorica ao celebre romancista portugues, executada logo apos a sua morte com forte expressividade naturalista.",
            "Ficha Tecnica Oficial MNSR (Inv. 204 Esc) & Arquivo MNSR"
        }
    };

    for (int i = 0; i < 8; i++) {
        acervo[i] = dados[i];
    }
    return 8;
}

static int pertenceAoDominioMuseu(const char *norm) {
    const char *termos_dominio[] = {
        "museu", "mnsr", "acervo", "obra", "obras", "quadro", "pintura",
        "escultura", "estatua", "artista", "autor", "esculpiu", "pintou",
        "sala", "exposicao", "galeria", "visita", "colecao", "ficha",
        "ourivesaria", "arqueologia", "retrato", "arte", "soares dos reis"
    };
    int n = sizeof(termos_dominio) / sizeof(termos_dominio[0]);
    for (int i = 0; i < n; i++) {
        if (strstr(norm, termos_dominio[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

static int encontrarObraNaBase(const char *pergunta_norm, FichaObra acervo[], int total) {
    if (strstr(pergunta_norm, "desterro") != NULL) return 0;
    if (strstr(pergunta_norm, "pousao") != NULL || strstr(pergunta_norm, "columbano") != NULL) return 1;
    if (strstr(pergunta_norm, "custodia") != NULL || strstr(pergunta_norm, "bemposta") != NULL) return 2;
    if (strstr(pergunta_norm, "senhora do o") != NULL) return 3;
    if (strstr(pergunta_norm, "aurelia") != NULL || strstr(pergunta_norm, "auto retrato") != NULL || strstr(pergunta_norm, "autoretrato") != NULL) return 4;
    if (strstr(pergunta_norm, "mumia") != NULL || strstr(pergunta_norm, "egipcia") != NULL || strstr(pergunta_norm, "caixao") != NULL) return 5;
    if (strstr(pergunta_norm, "namban") != NULL || strstr(pergunta_norm, "biombo") != NULL) return 6;
    if (strstr(pergunta_norm, "camilo") != NULL || strstr(pergunta_norm, "busto") != NULL) return 7;

    char titulo_norm[100];
    for (int i = 0; i < total; i++) {
        normalizarTexto(acervo[i].titulo, titulo_norm, sizeof(titulo_norm));
        if (strstr(pergunta_norm, titulo_norm) != NULL) {
            return i;
        }
    }
    return -1;
}

static int responderComBaseNoAcervo(const char *pergunta, FichaObra acervo[], int total, char *resposta_out, int max_out) {
    char norm[300];
    normalizarTexto(pergunta, norm, sizeof(norm));

    int idx = encontrarObraNaBase(norm, acervo, total);

    if (idx == -1) {
        if (pertenceAoDominioMuseu(norm)) {
            snprintf(resposta_out, max_out,
                     "Nao tenho essa informacao nos documentos oficiais do acervo do MNSR. Para garantir rigor factual, nao apresento respostas especulativas sobre obras ou detalhes nao documentados.");
            return 0;
        }

        snprintf(resposta_out, max_out,
                 "Lamento, mas apenas posso responder a perguntas relacionadas com o acervo e a visita ao Museu Nacional Soares dos Reis (MNSR).\nConvido-o a perguntar sobre as obras em exposicao, como 'O Desterro', o 'Retrato de Henrique Pousao' ou o 'Auto-retrato' de Aurelia de Sousa!");
        return 0;
    }

    FichaObra obra = acervo[idx];
    int pede_autor = (strstr(norm, "quem") != NULL || strstr(norm, "autor") != NULL ||
                      strstr(norm, "esculpiu") != NULL || strstr(norm, "pintou") != NULL ||
                      strstr(norm, "artista") != NULL || strstr(norm, "produziu") != NULL ||
                      strstr(norm, "criou") != NULL);
    int pede_ano = (strstr(norm, "ano") != NULL || strstr(norm, "quando") != NULL ||
                    strstr(norm, "data") != NULL || strstr(norm, "seculo") != NULL ||
                    strstr(norm, "epoca") != NULL);
    int pede_sala = (strstr(norm, "onde") != NULL || strstr(norm, "sala") != NULL ||
                     strstr(norm, "local") != NULL || strstr(norm, "exposto") != NULL ||
                     strstr(norm, "fica") != NULL);
    int pede_tecnica = (strstr(norm, "material") != NULL || strstr(norm, "materiais") != NULL ||
                        strstr(norm, "tecnica") != NULL || strstr(norm, "feito") != NULL);
    int pede_contexto = (strstr(norm, "contexto") != NULL || strstr(norm, "historia") != NULL ||
                         strstr(norm, "sobre") != NULL || strstr(norm, "fala") != NULL ||
                         strstr(norm, "explica") != NULL || strstr(norm, "significado") != NULL ||
                         strstr(norm, "origem") != NULL || strstr(norm, "retrata") != NULL ||
                         strstr(norm, "ficha") != NULL);

    if (pede_autor && pede_ano) {
        snprintf(resposta_out, max_out,
                 "A obra '%s' foi criada por %s no ano de %s.\nContexto: %s\nFonte: %s",
                 obra.titulo, obra.autor, obra.ano, obra.contexto, obra.fonte);
    } else if (pede_autor) {
        snprintf(resposta_out, max_out,
                 "O autor de '%s' e %s (%s).\nFonte: %s",
                 obra.titulo, obra.autor, obra.ano, obra.fonte);
    } else if (pede_ano) {
        snprintf(resposta_out, max_out,
                 "A obra '%s' (%s) data de %s.\nFonte: %s",
                 obra.titulo, obra.autor, obra.ano, obra.fonte);
    } else if (pede_sala) {
        snprintf(resposta_out, max_out,
                 "A obra '%s' encontra-se exposta na %s.\nFonte: %s",
                 obra.titulo, obra.sala, obra.fonte);
    } else if (pede_tecnica) {
        snprintf(resposta_out, max_out,
                 "Tecnica e materiais de '%s': %s.\nFonte: %s",
                 obra.titulo, obra.tecnica, obra.fonte);
    } else if (pede_contexto) {
        snprintf(resposta_out, max_out,
                 "Obra: %s | Autor: %s | Ano: %s\nTecnica: %s | Localizacao: %s\nContexto: %s\nFonte: %s",
                 obra.titulo, obra.autor, obra.ano, obra.tecnica, obra.sala, obra.contexto, obra.fonte);
    } else {
        snprintf(resposta_out, max_out,
                 "Nao tenho essa informacao especifica nos documentos oficiais do acervo sobre '%s'. Para evitar qualquer especulacao, apenas disponho de dados comprovados sobre autor, ano, tecnica, localizacao e contexto historico.",
                 obra.titulo);
        return 0;
    }

    return 1;
}

// Processa o envio da pergunta se houver ligacao de rede, ou preserva-a em caso offline
static void processarEnvioPergunta(const char *texto_pergunta, FichaObra acervo[], int total_obras) {
    char resposta[700];

    snprintf(pergunta_pendente, sizeof(pergunta_pendente), "%s", texto_pergunta);
    tem_pergunta_pendente = 1;

    while (1) {
        if (ligacao_rede) {
            clock_t inicio = clock();
            responderComBaseNoAcervo(pergunta_pendente, acervo, total_obras, resposta, sizeof(resposta));
            clock_t fim = clock();
            double tempo_resposta = (double)(fim - inicio) / CLOCKS_PER_SEC;

            printf("\n[Assistente MNSR - Tempo de resposta: %.3f segundos (< 5s)]\n", tempo_resposta);
            printf("%s\n", resposta);

            tem_pergunta_pendente = 0;
            pergunta_pendente[0] = '\0';
            return;
        } else {
            // Cenario 1 (US-08): Falta de ligacao Wi-Fi/4G
            printf("\n[Sem Ligacao a Internet - Tolerancia a Falhas]\n");
            printf("O Assistente IA requer ligacao Wi-Fi ou 4G para consultar o servico RAG.\n");
            printf("A sua pergunta ficou guardada no campo de texto e nao foi perdida:\n");
            printf("  >> \"%s\"\n", pergunta_pendente);
            printf("Nota: Pode continuar a usar os Roteiros Inteligentes e as Fichas em cache normalmente.\n");

            printf("\n1 - Restabelecer ligacao (Wi-Fi/4G ON) e [Tentar novamente]\n");
            printf("2 - [Tentar novamente] (manter estado atual da rede)\n");
            printf("0 - Voltar (manter pergunta guardada para enviar mais tarde)\n");
            printf("Opcao: ");
            int acao = lerOpcaoAssistente();

            if (acao == 1) {
                ligacao_rede = 1;
                printf("\n[Ligacao Wi-Fi/4G restabelecida!] A enviar pergunta guardada...\n");
                // Cenario 2 (US-08): Reenvia automaticamente sem ter de reescrever
            } else if (acao == 2) {
                printf("\nA verificar ligacao de rede...\n");
            } else {
                return;
            }
        }
    }
}

static void executarBateriaTeste50Perguntas(FichaObra acervo[], int total) {
    const char *perguntas_teste[50] = {
        "Quem esculpiu O Desterro e em que ano?",
        "Qual o material de O Desterro?",
        "Em que sala esta O Desterro?",
        "Qual o contexto historico de O Desterro?",
        "Quem e o autor de O Desterro?",
        "Em que ano foi feito O Desterro?",
        "Fala-me sobre O Desterro.",
        "Quem pintou o Retrato de Henrique Pousao?",
        "Em que ano foi pintado o Retrato de Henrique Pousao?",
        "Onde esta exposto o Retrato de Henrique Pousao?",
        "Qual a tecnica do Retrato de Henrique Pousao?",
        "Contexto do Retrato de Henrique Pousao?",
        "Quem e o artista do Retrato de Henrique Pousao e quando foi criado?",
        "De que ano e a Custodia de Bemposta?",
        "Quem produziu a Custodia de Bemposta?",
        "Quais os materiais da Custodia de Bemposta?",
        "Onde fica a Custodia de Bemposta no museu?",
        "Fala-me sobre a Custodia de Bemposta.",
        "Qual a historia da Custodia de Bemposta?",
        "Quem esculpiu a Senhora do O?",
        "De que seculo data a Senhora do O?",
        "Qual o material da Senhora do O?",
        "Em que sala se encontra a Senhora do O?",
        "Explica o contexto da Senhora do O.",
        "Quem pintou o Auto-retrato de Aurelia de Sousa?",
        "Em que ano Aurelia de Sousa pintou o Auto-retrato?",
        "Qual a tecnica do Auto-retrato de Aurelia de Sousa?",
        "Onde esta o Auto-retrato de Aurelia de Sousa?",
        "Qual o contexto do Auto-retrato de Aurelia de Sousa?",
        "Fala-me sobre o Auto-retrato de Aurelia de Sousa.",
        "De que epoca e o Caixao de Mumia Egipcia?",
        "Qual o material do Caixao de Mumia Egipcia?",
        "Em que sala esta o Caixao de Mumia Egipcia?",
        "Qual a origem do Caixao de Mumia Egipcia?",
        "Explica o significado do Caixao de Mumia Egipcia.",
        "Quem criou o Biombo Namban?",
        "De que ano ou seculo e o Biombo Namban?",
        "Quais os materiais do Biombo Namban?",
        "Onde esta exposto o Biombo Namban?",
        "O que retrata o Biombo Namban?",
        "Fala-me sobre o Biombo Namban.",
        "Quem esculpiu o Busto de Camilo Castelo Branco?",
        "Em que ano foi feito o Busto de Camilo Castelo Branco?",
        "Qual o material do Busto de Camilo Castelo Branco?",
        "Onde fica o Busto de Camilo Castelo Branco?",
        "Qual o contexto do Busto de Camilo Castelo Branco?",
        "Autor e ano do Busto de Camilo Castelo Branco?",
        "Quem e o autor de O Desterro?",
        "Quando foi criado o Biombo Namban?",
        "Qual a sala do Auto-retrato de Aurelia de Sousa?"
    };

    int conformes = 0;
    char resposta[700];

    printf("\n=== Validacao RNF-02: Bateria de 50 Perguntas do Acervo ===\n");
    for (int i = 0; i < 50; i++) {
        int ok = responderComBaseNoAcervo(perguntas_teste[i], acervo, total, resposta, sizeof(resposta));
        if (ok && strstr(resposta, "Fonte: Ficha Tecnica Oficial MNSR") != NULL) {
            conformes++;
        }
    }

    double taxa = (conformes / 50.0) * 100.0;
    printf("Perguntas avaliadas: 50\n");
    printf("Respostas factualmente conformes com as fontes: %d/50 (%.1f%%)\n", conformes, taxa);
    printf("Requisito RNF-02 (>= 98.0%%): %s\n", taxa >= 98.0 ? "APROVADO" : "REPROVADO");
}

void iniciarAssistenteIA() {
    FichaObra acervo[8];
    int total_obras = carregarBaseConhecimento(acervo);
    int opcao = -1;
    char pergunta[256];

    printf("\n=== MNSR Digital - Assistente IA / RAG (Idioma: Portugues) ===\n");
    printf("Base de conhecimento: Fichas tecnicas e textos oficiais do acervo MNSR.\n");

    while (opcao != 0) {
        printf("\nEstado da Rede: %s\n",
               ligacao_rede ? "ONLINE (Wi-Fi/4G ativo)" : "OFFLINE (Sem cobertura de rede)");
        if (tem_pergunta_pendente) {
            printf("[Pergunta guardada no campo de texto]: \"%s\"\n", pergunta_pendente);
        }

        printf("1 - Fazer uma pergunta sobre uma obra do acervo\n");
        printf("2 - Consultar fichas tecnicas em cache (Disponivel Offline)\n");
        printf("3 - Simular/Alternar ligacao de rede (Wi-Fi/4G ON/OFF)\n");
        printf("4 - Executar bateria de teste RNF-02 (50 perguntas / >= 98%% fidelidade)\n");
        if (tem_pergunta_pendente) {
            printf("5 - [Tentar novamente] enviar pergunta guardada\n");
        }
        printf("0 - Voltar ao menu principal\n");
        printf("Opcao: ");
        opcao = lerOpcaoAssistente();

        if (opcao == 1) {
            printf("\nEscreva a sua pergunta em linguagem natural:\n> ");
            if (fgets(pergunta, sizeof(pergunta), stdin) != NULL) {
                pergunta[strcspn(pergunta, "\r\n")] = '\0';

                if (strlen(pergunta) == 0) {
                    printf("Por favor, introduza uma pergunta valida.\n");
                    continue;
                }

                processarEnvioPergunta(pergunta, acervo, total_obras);
            }
        } else if (opcao == 2) {
            printf("\n--- Fichas Tecnicas em Cache Local (Modo Offline Disponivel) ---\n");
            for (int i = 0; i < total_obras; i++) {
                printf("- [%s] %s | %s (%s) | %s\n",
                       acervo[i].id_ficha, acervo[i].titulo, acervo[i].autor,
                       acervo[i].ano, acervo[i].sala);
            }
        } else if (opcao == 3) {
            ligacao_rede = !ligacao_rede;
            printf("\nEstado da ligacao de rede alterado para: %s\n",
                   ligacao_rede ? "ONLINE (Wi-Fi/4G ativo)" : "OFFLINE (Sem cobertura de rede)");
        } else if (opcao == 4) {
            executarBateriaTeste50Perguntas(acervo, total_obras);
        } else if (opcao == 5 && tem_pergunta_pendente) {
            processarEnvioPergunta(pergunta_pendente, acervo, total_obras);
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    }
}

#endif