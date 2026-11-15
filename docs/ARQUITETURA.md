# Arquitetura do Sistema — App MNSR Digital

## Parametrização de Roteiros e Cache
* **Tempo máximo de roteiro configurado:** 90 minutos (suporte completo para visitas de 30, 60 e 90 min)
* **Ficheiro de persistência offline:** `cache_roteiro.dat`
* **Módulos principais:** `main.c`, `roteiros.c`, `gamificacao.c`, `assistente.c`