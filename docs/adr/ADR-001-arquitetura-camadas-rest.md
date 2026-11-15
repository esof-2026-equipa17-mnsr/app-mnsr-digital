# ADR-001: Adoção de Arquitetura em 3 Camadas e Estratégia de Caching Local Offline-First

* **Status:** Aceite (Accepted)
* **Data:** 2026-11-15
* **Decisores:** Equipa 17 — Engenharia de Software (App MNSR Digital)

---

## 1. Contexto (Context)
O Museu Nacional Soares dos Reis (MNSR), instalado no Palácio dos Carrancas (Porto), apresenta paredes estruturais espessas de granito que provocam zonas cegas de cobertura Wi-Fi e dados móveis (4G/5G), especialmente nas galerias interiores e no piso -1 onde se encontra a escultura *O Desterro*. Simultaneamente, o requisito não funcional **RNF-01** exige tempos de resposta inferiores a `1,0s` em operação offline, e o sistema precisa de manter separação clara entre interface, regras de negócio e persistência de dados para evitar código monolítico difícil de testar.

## 2. Opções Consideradas
1. **Arquitetura Monolítica com Acesso Direto à Rede:** Interface acoplada diretamente a chamadas HTTP/SQL. *Descartada* por provocar falhas imediatas nas salas sem cobertura de rede e violar a separação de responsabilidades.
2. **Arquitetura em 3 Camadas com Estratégia Offline-First (Escolhida):** Separação estrita entre Camada de Apresentação, Camada de Lógica de Negócio e Camada de Persistência respaldada por cache local (`cache_roteiro.dat` / SQLite em ambiente móvel).

## 3. Decisão (Decision)
Adotar uma **Arquitetura em 3 Camadas** (Apresentação, Lógica de Negócio e Persistência) combinada com uma estratégia de **Caching Local Offline-First**. Os metadados das obras, fichas curatoriais oficiais e códigos QR são pré-carregados e persistidos localmente, permitindo que o gerador de roteiros, a Caça ao Tesouro e o registo de perguntas pendentes do assistente IA operem continuamente sem depender de conectividade externa em tempo real.

## 4. Consequências (Consequences)
* **Positivas:**
    * Funcionamento ininterrupto nas salas interiores de escultura sem sinal de rede (`RNF-01` cumprido com latência `< 1,0s`).
    * Alta coesão e baixo acoplamento: alterações na interface (CLI/Mobile) ou no formato de armazenamento não afetam os algoritmos de negócio (`roteiros.c`, `gamificacao.c`, `assistente.c`).
    * Facilidade de execução de testes unitários e validação dos cenários BDD de forma isolada.
* **Negativas / Trade-offs:**
    * Necessidade de implementar mecanismos de sincronização e reconciliação de dados (ex.: envio diferido de perguntas pendentes do assistente IA) quando a ligação de rede é restabelecida.
    * Consumo adicional de armazenamento local no dispositivo do visitante para manter o catálogo e os índices de cache atualizados.