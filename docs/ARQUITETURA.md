# Arquitetura do Sistema — App MNSR Digital (Marco M2)

**Equipa:** 17 | **Repositório:** `app-mnsr-digital` | **Sessão:** 4 (Arquitetura & Modelação UML)

---

## 1. Parametrização de Roteiros e Cache
* **Tempo máximo de roteiro configurado:** 90 minutos (suporte completo para visitas de 30, 60 e 90 min)
* **Ficheiro de persistência offline:** `cache_roteiro.dat`
* **Módulos principais:** `main.c`, `roteiros.c`, `gamificacao.c`, `assistente.c`

---

## 2. Arquitetura em 3 Camadas (Separação de Responsabilidades)

O sistema adota o padrão arquitetural em **3 Camadas** (*Layered Architecture*), assegurando **alta coesão**, **baixo acoplamento** e dependência estritamente unidirecional de cima para baixo:

| Camada Arquitetural | Responsabilidade Principal | Componentes Conceptuais (REST/App) | Mapeamento no Código C (MVP) |
| :--- | :--- | :--- | :--- |
| **1. Apresentação (UI / Controller)** | Captura de entradas do visitante, validação de formato e apresentação de resultados, sem regras de negócio nem acesso direto a ficheiros. | `VisaoRoteiro`, `RoteiroController`, `QRScanView` | `main.c` (menus CLI, leitura de inputs e formatação de saída) |
| **2. Lógica de Negócio (Domain / Service)** | Algoritmos de seleção de roteiros por tempo/tema, validação de QR Codes, cálculo de pontos/recompensas e filtro anti-alucinação RAG. | `ServicoRoteiro`, `DesafioGamificado`, `AssistenteService` | `roteiros.c`, `gamificacao.c`, `assistente.c` |
| **3. Persistência (Repository / Dados)** | Armazenamento e recuperação local de obras, fichas curatoriais e estado da sessão, garantindo resiliência *offline-first* (`< 1,0s`). | `RepositorioObra`, `RepositorioVisita`, `BaseDadosLocal` | Catálogo estruturado em memória e persistência binária (`cache_roteiro.dat`) |

---

## 3. Diagrama de Classes UML (Visão Estrutural — Mermaid.js)

O diagrama seguinte representa as entidades centrais do domínio do Museu Nacional Soares dos Reis, os seus atributos, operações e relações estruturais:

```mermaid
classDiagram
    class Obra {
        +String id
        +String titulo
        +String autor
        +int ano
        +String salaId
        +String qrCodeHash
        +int duracaoMinutos
        +int pontosGamificacao
        +getDetalhes() String
    }

    class Roteiro {
        +String id
        +String nome
        +int duracaoEstimada
        +String perfilVisitante
        +adicionarObra(Obra o) void
        +calcularTempoTotal() int
    }

    class Visitante {
        +String id
        +int progressoRoteiro
        +int pontosAcumulados
        +String conquistasDesbloqueadas
        +registarVisita(Obra o) void
        +desbloquearRecompensa() bool
    }

    class DesafioGamificado {
        +String codigoQrLido
        +validarQRCode(String qrHash) bool
        +atribuirPontuacao(Visitante v, Obra o) int
        +verificarElegibilidadePremio(Visitante v) bool
    }

    Roteiro "1" *-- "1..*" Obra : contem
    Visitante "1" --> "0..*" Roteiro : percorre
    DesafioGamificado ..> Obra : consulta
    DesafioGamificado ..> Visitante : atualiza
```

---

## 4. Diagramas de Sequência UML (Visão Dinâmica — Mermaid.js)

### 4.1 US-01: Geração de Roteiro Inteligente com Resiliência Offline
```mermaid
sequenceDiagram
    autonumber
    actor Visitante
    participant UI as Apresentacao (main.c / UI)
    participant Srv as ServicoRoteiro (roteiros.c)
    participant Repo as RepositorioObra (Cache Local)
    participant Rot as Roteiro (Dominio)

    Note over Visitante, UI: Visitante escolhe duracao (30, 60 ou 90 min) e tema
    Visitante->>UI: solicitarRoteiro(duracao, tema)
    activate UI
    UI->>Srv: gerarRoteiro(duracao, tema)
    activate Srv

    Srv->>Repo: carregarObrasDisponiveis()
    activate Repo
    alt Ligacao ativa disponivel
        Repo-->>Srv: listaObrasAtualizada
    else Modo Offline (Galerias sem Wi-Fi/4G)
        Note right of Repo: Leitura rapida de cache_roteiro.dat (inferior a 1.0s)
        Repo-->>Srv: listaObrasCacheLocal
    end
    deactivate Repo

    Srv->>Rot: criarRoteiro(id, nome, duracao)
    activate Rot
    Rot-->>Srv: instanciaRoteiro
    deactivate Rot

    loop Para cada obra compativel com tema e tempo
        Srv->>Rot: adicionarObra(obra)
        activate Rot
        Rot-->>Srv: confirmacao
        deactivate Rot
    end

    Srv-->>UI: RoteiroFinalizado
    deactivate Srv
    UI-->>Visitante: Exibe percurso personalizado e tempo total
    deactivate UI
```

### 4.2 US-02: Validação de QR Code na Caça ao Tesouro
```mermaid
sequenceDiagram
    autonumber
    actor Visitante
    participant UI as QRScanView (main.c / UI)
    participant Game as DesafioGamificado (gamificacao.c)
    participant Repo as RepositorioObra (Dados)
    participant Vis as Visitante (Sessao)

    Visitante->>UI: lerQRCode(codigoQR)
    activate UI
    UI->>Game: validarQRCode(codigoQR)
    activate Game

    Game->>Repo: procurarObraPorQR(codigoQR)
    activate Repo
    Repo-->>Game: obraEncontrada
    deactivate Repo

    alt QR Code Valido
        Game->>Vis: registarVisita(obraEncontrada)
        activate Vis
        Vis-->>Game: progressoAtualizado
        deactivate Vis
        Game->>Game: verificarElegibilidadePremio(Vis)
        Game-->>UI: PistaDesbloqueada + Pontos + EstadoRecompensa
    else QR Code Invalido ou Duplicado
        Game-->>UI: ErroValidacaoQR
    end

    deactivate Game
    UI-->>Visitante: Mostra pista cultural ou aviso de erro
    deactivate UI
```
