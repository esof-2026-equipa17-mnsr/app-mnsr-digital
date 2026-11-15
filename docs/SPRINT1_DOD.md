# Sprint 1 Planning & Definition of Done (DoD) — App MNSR Digital

**Equipa:** 17 | **Repositório:** `app-mnsr-digital` | **Sessão:** 3 (Métodos Ágeis & Git Colaborativo)

---

## 1. Sprint Goal (Objetivo da Sprint 1)
Entregar um incremento funcional, testado e integrado na branch `main` que permita ao visitante do Museu Nacional Soares dos Reis gerar roteiros personalizados (30, 60 e 90 min), jogar à Caça ao Tesouro via QR Code, consultar o guia conversacional IA (RAG) e aceder aos dados essenciais em modo offline.

---

## 2. Sprint 1 Backlog — Estimativas (Fibonacci) e Tarefas Técnicas

| ID & User Story | Prioridade | Story Points (SP) | Decomposição em Tarefas Técnicas | Branch Associada |
| :--- | :--- | :---: | :--- | :--- |
| **US-01 / `#3`–`#5`:** Roteiros Inteligentes por tempo e tema + reajuste dinâmico | Alta | **5 SP** | 1. Definir estruturas `Obra` e `Roteiro` em C<br>2. Implementar algoritmo de seleção por duração (30/60/90 min) e tema<br>3. Implementar reajuste dinâmico e validação de cenários BDD | `feat/us01-gerador-roteiros` |
| **US-02 / `#6`–`#7`:** Caça ao Tesouro por QR Code e recompensas | Alta | **8 SP** | 1. Criar catálogo de códigos QR e pistas das obras<br>2. Implementar validador de leitura e progresso do visitante<br>3. Atribuir certificado/recompensa a partir de 80% de conclusão | `feat/us02-caca-tesouro` |
| **US-03 / RNF-01:** Persistência e operação em Modo Offline | Alta | **5 SP** | 1. Implementar cache binária local (`cache_roteiro.dat`)<br>2. Garantir carregamento `< 1,0s` com indicador `[Modo Offline]`<br>3. Preservar perguntas pendentes do assistente sem ligação | `feat/us03-modo-offline` |
| **US-04 / `#10`–`#12`:** Assistente IA / RAG com rigor factual (RNF-02) | Alta | **13 SP** | 1. Estruturar base de fichas curatoriais oficiais do MNSR<br>2. Implementar filtro de domínio e bloqueio anti-alucinação<br>3. Executar bateria automatizada de 50 perguntas (`>= 98%` rigor) | `feat/us04-assistente-rag` |
| **US-05 / Governação:** Registo de Proveniência de IA & Documentação | Média | **2 SP** | 1. Estruturar `docs/AI_LOG.md` e `docs/SPRINT1_DOD.md`<br>2. Documentar prompts, outputs e validação humana por fase<br>3. Validar rastreabilidade entre commits, PRs e Issues | `docs/sessao3-dod-ailog` |

**Capacidade / Total Planeado na Sprint 1:** `33 SP`

---

## 3. Definition of Done (DoD) — Equipa 17

Uma *User Story* ou tarefa técnica só pode transitar para a coluna **Done** no quadro Kanban quando cumprir simultaneamente todos os critérios seguintes:

1. **Compilação & Código Limpo:**
  * [x] Código-fonte em C compila sem erros nem avisos críticos (`-Wall -Wextra`) em ambiente limpo (CLion / CMake).
  * [x] Código modularizado (`main.c`, `roteiros.c`, `gamificacao.c`, `assistente.c`) e livre de ficheiros temporários/binários no controlo de versões.
2. **Testes & Critérios BDD:**
  * [x] Todos os cenários BDD (Dado / Quando / Então) da respetiva *User Story* executados e validados com sucesso.
  * [x] Requisitos não funcionais verificados (`RNF-01` offline `< 1,0s`; `RNF-02` rigor factual `100%` na bateria de 50 perguntas).
3. **Revisão por Pares (Peer Review) & Git Colaborativo:**
  * [x] Trabalho desenvolvido em *feature branch* dedicada (`feat/...`, `fix/...`, `docs/...`), sem *commits* diretos na `main`.
  * [x] Histórico estruturado segundo *Conventional Commits* (`tipo(escopo): descrição #ID_ISSUE`).
  * [x] *Pull Request* aberto para a `main` com resumo, referência à *issue* e aprovação formal (`Approve`) de pelo menos um colega de equipa.
4. **Rastreabilidade & Governação de IA:**
  * [x] Cartão atualizado no GitHub Projects (`Sprint 1 - App MNSR Digital`) e *Issue* fechada.
  * [x] Prompts, ferramentas GenAI utilizadas e validação humana registados em `docs/AI_LOG.md`.