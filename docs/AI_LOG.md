# Registo de Proveniência e Uso de IA Generativa (AI Log)

Projeto: App MNSR Digital | Equipa: 17

| Data | ID Tarefa | Ferramenta GenAI | Prompt Submetido | Resumo do Output Gerado | Ação Humana de Validação / Alterações | Nível de Risco Identificado |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 2026-09-29 | M1 (Sessão 2) | Claude | Geração da Visão do Produto, 3 EPICs, 8 User Stories com cenários BDD, matriz dos 3 Cs e script `setup-m1.sh` a partir da ficha da Sessão 2 | Artefactos de requisitos M1 (`README.md`, backlog, cenários BDD e script de configuração do GitHub) | Revisão de cada User Story pela equipa, validação dos valores numéricos com o docente e execução do script no GitHub | Baixo |
| 2026-10-15 | M0.1 (Sessão 1) | Claude | Análise crítica comparativa entre desenvolvimento assistido por IA e Engenharia de Software tradicional | Relatório PDF e ficheiro HTML gerados | Ficheiros lidos, verificados e validados pela equipa | Baixo |
| 2026-11-15 | M2 / MVP (`#3`–`#12`) | Google Gemini | Implementação modular em C e validação BDD dos 3 épicos (Roteiros Inteligentes, Gamificação QR Code e Assistente IA/RAG com tolerância a falhas e RNF-01/RNF-02) | Código-fonte em C (`main.c`, `roteiros.c`, `gamificacao.c`, `assistente.c`), bateria de testes RNF-02 (50 perguntas) e atualização da documentação | Compilação e execução no CLion; validação manual de todos os cenários BDD; ajuste da cache local, catálogo de 18 obras e gestão de branches/PRs no GitHub | Baixo |
| 2026-11-15 | Sessão 3 (Scrum & Git) | Google Gemini | Apoio na elaboração do Sprint 1 Planning (Fibonacci SP), Definition of Done (DoD) e sequência de comandos Git para Feature Branching, resolução manual de conflito de merge e Peer Review | Ficheiros `docs/SPRINT1_DOD.md`, `docs/ARQUITETURA.md` e comandos CLI para fluxo colaborativo Git | Execução dos comandos Git no terminal do CLion, simulação e resolução manual do conflito de merge e revisão/aprovação cruzada dos Pull Requests pela equipa | Baixo |

# AI_LOG — Registo de proveniência de IA

## Entrada 1
* **Data:** 2026-09-29
* **Módulo/fase:** Sessão 2 / Requisitos M1
* **Ferramenta:** Claude (Anthropic)
* **Prompt utilizado:** Foi anexada a ficha prática da Sessão 2 com o pedido "do it", para gerar os artefactos do M1: secção da Visão no README, 3 EPICs, User Stories com estrutura Como/quero/para que, 3 Cs e cenários BDD, matriz do Exercício 2.3 e script de configuração do GitHub.
* **Resultado obtido:** 3 EPICs, 8 User Stories com 20 cenários BDD, matriz dos 3 Cs, script `setup-m1.sh`.
* **Validação humana realizada:**
  * [x] Revisão de cada User Story pela equipa
  * [x] Valores numéricos confirmados ou ajustados com o curador/docente (ver `docs/BACKLOG.md`)
  * [x] Perguntas da secção "Conversation" discutidas com o PO e respostas registadas nas issues
  * [x] Script `setup-m1.sh` executado e resultado verificado no GitHub

## Entrada 2
* **Data:** 2026-11-15
* **Módulo/fase:** Implementação MVP, Validação BDD e Qualidade ISO/IEC 25010 (M2)
* **Ferramenta:** Google Gemini
* **Prompt utilizado:** Pedidos iterativos por User Story (`#3` a `#12`) com os critérios de aceitação BDD e requisitos não funcionais (RNF-01 desempenho < 1,0s em cache offline; RNF-02 fidelidade factual >= 98%; tolerância a falhas ISO/IEC 25010) para implementar e refinar os módulos em C (`roteiros.c`, `gamificacao.c`, `assistente.c` e `main.c`).
* **Resultado obtido:** Implementação completa dos 3 Épicos em C: geração e reajuste de roteiros sobre 18 obras com cache binária local (`cache_roteiro.dat`), jogo de Caça ao Tesouro por QR Code com recompensa a partir de 80%, assistente IA baseado em fichas oficiais com filtro de domínio, preservação de pergunta pendente offline e bateria automatizada de 50 perguntas (100% conformidade).
* **Validação humana realizada:**
  * [x] Compilação e testes funcionais de todos os cenários BDD executados pela equipa no CLion
  * [x] Verificação dos intervalos de obras (30, 60 e 90 min), tempos de resposta (< 1,0s offline e < 5s RAG) e avisos de tema insuficiente
  * [x] Correção manual da estrutura de `menuRoteiros()` e inclusão dos módulos em `main.c` para compilação limpa no CMake
  * [x] Integração incremental por *feature branches* e *Pull Requests* no GitHub com fecho das *issues* e épicos no quadro Kanban
  * [x] Revisão final do `README.md` e `docs/AI_LOG.md` antes da submissão

## Entrada 3
* **Data:** 2026-11-15
* **Módulo/fase:** Sessão 3 — Métodos Ágeis, Planeamento de Iterações, DoD e Git Colaborativo
* **Ferramenta:** Google Gemini
* **Prompt utilizado:** Apoio na estruturação do Sprint 1 Planning (estimativas em Story Points na sequência de Fibonacci e decomposição técnica), formalização da Definition of Done (DoD) e sequência de comandos Git para Feature Branching, resolução manual de conflito de merge em `docs/ARQUITETURA.md` e Pull Requests com Peer Review.
* **Resultado obtido:** Artefactos `docs/SPRINT1_DOD.md` e `docs/ARQUITETURA.md`, atualização do `docs/AI_LOG.md` e guião de comandos CLI para simulação e resolução de conflitos.
* **Validação humana realizada:**
  * [x] Verificação e ajuste das estimativas em Story Points e responsáveis pela equipa
  * [x] Execução prática do fluxo `feature branch -> conflito de merge -> resolução manual -> Pull Request` no repositório
  * [x] Revisão por pares (`Approve`) efetuada entre os membros da equipa no GitHub
  * [x] Auditoria das colunas do quadro Kanban (`Todo`, `In Progress`, `In Review`, `Done`) no GitHub Projects