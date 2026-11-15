# app-mnsr-digital

## Visão do Produto

**PARA** os jovens, famílias e turistas culturais que visitam a cidade do Porto,

**QUE** procuram uma experiência museológica dinâmica, personalizada e interativa,

**A APP MNSR DIGITAL** é uma aplicação móvel inteligente e gamificada,

**QUE** oferece roteiros personalizados pelo tempo disponível, caça ao tesouro por QR Code e um guia conversacional de IA para responder a dúvidas sobre as obras,

**AO CONTRÁRIO DOS** guias áudio estáticos e brochuras em papel tradicionais,

**O NOSSO PRODUTO** aumenta o envolvimento do visitante, promove o regresso do público local e permite gerir eficientemente o fluxo nas salas do museu.

---

## Funcionalidades Implementadas (MVP)

### EPIC-01 — Roteiros Inteligentes (`roteiros.c`)
* **Geração por Tempo Disponível (US-03):** Criação de itinerários otimizados para visitas de **30 minutos** (5 a 7 obras), **60 minutos** (10 a 14 obras) e **90 minutos** (15 a 21 obras) sobre um acervo de 18 obras emblemáticas, garantindo que o tempo total estimado não ultrapassa a duração selecionada.
* **Filtragem por Tema (US-04):** Personalização por categoria do acervo (*Escultura*, *Pintura*, *Ourivesaria*, *Arqueologia*, *Artes Decorativas* ou *Todos*), com deteção automática de insuficiência de conteúdo para a duração escolhida e sugestão de temas complementares.
* **Reajuste Dinâmico a Meio da Visita (US-05):** Recálculo imediato das obras restantes em função do tempo livre atualizado e das obras já visitadas, operando integralmente sobre dados em cache local.
* **Acesso Offline em Cache (RNF-01):** Persistência do último roteiro gerado em ficheiro local (`cache_roteiro.dat`) com carregamento em **< 1,0 segundo** e indicador discreto de `[Modo Offline]`.

### EPIC-02 — Gamificação / Caça ao Tesouro (`gamificacao.c`)
* **Leitura de QR Code nas Salas (US-06):** Validação dos códigos das obras em exposição, desbloqueio imediato de pistas interativas e feedback claro para códigos inválidos.
* **Progresso e Recompensa Final (US-07):** Registo persistente do progresso do visitante, contagem de pontos e atribuição do certificado/recompensa digital mediante a conclusão mínima de 80% das etapas.

### EPIC-03 — Assistente IA / RAG (`assistente.c`)
* **Perguntas em Linguagem Natural sobre o Acervo (US-10):** Respostas fundamentadas exclusivamente nas fichas técnicas oficiais e textos curatoriais do MNSR (autor, data, técnica, localização e contexto histórico) com citação explícita da fonte e tempo de resposta **< 5 segundos**.
* **Rigor Factual e Restrição de Domínio (US-11 / RNF-02):** Bloqueio de perguntas fora do contexto do museu e recusa de especulação quando a informação não consta nos documentos oficiais, validado através de bateria automatizada de 50 perguntas (**100% de conformidade**, superando o limiar mínimo de 98%).
* **Tolerância a Falhas e Estado Offline (US-12 / ISO/IEC 25010):** Degradação graciosa perante ausência de cobertura Wi-Fi/4G, preservando a pergunta escrita em memória para envio imediato assim que a ligação é restabelecida, sem bloquear o acesso às fichas técnicas em cache nem aos roteiros.

---

## Arquitetura e Estrutura do Projeto

```text
app-mnsr-digital/
├── main.c            # Menu principal e orquestração dos módulos
├── roteiros.c        # Algoritmo de recomendação, cache local e reajuste offline
├── gamificacao.c     # Lógica de validação de QR Codes, pistas e recompensas
├── assistente.c      # Motor de consulta RAG sobre fichas oficiais e bateria RNF-02
├── CMakeLists.txt    # Configuração de compilação CMake
├── README.md         # Documentação principal do produto
└── AI_LOG.md         # Registo de proveniência e validação de IA Generativa
