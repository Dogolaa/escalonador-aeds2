#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Estrutura para representar um processo
typedef struct {
    int id;       // Identificador do processo
    int tamanho;  // Tamanho do processo
} Processo;

// Estrutura de um nó para a lista encadeada
typedef struct No {
    Processo processo;
    struct No* proximo;
} No;

// Estrutura da fila
typedef struct {
    No* frente;
    No* tras;
} Fila;

// Função para inicializar uma fila
void inicializarFila(Fila* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

// Função para liberar a memória utilizada pela fila
void liberarMemoria(Fila* fila) {
    while (fila->frente != NULL) {
        No* noRemovido = fila->frente;
        fila->frente = noRemovido->proximo;
        free(noRemovido);
    }
    fila->tras = NULL;
}

// Função para enfileirar um processo na fila
void enfileirar(Fila* fila, Processo processo) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->processo = processo;
    novoNo->proximo = NULL;

    if (fila->tras == NULL) {
        fila->frente = novoNo;
        fila->tras = novoNo;
    } else {
        fila->tras->proximo = novoNo;
        fila->tras = novoNo;
    }
}

// Função para desenfileirar um processo da fila
Processo desenfileirar(Fila* fila) {
    if (fila->frente == NULL) {
        Processo processoVazio = {-1, 0}; // Retorna um processo inválido
        return processoVazio;
    }

    No* noRemovido = fila->frente;
    Processo processo = noRemovido->processo;

    fila->frente = noRemovido->proximo;
    if (fila->frente == NULL) {
        fila->tras = NULL;
    }

    free(noRemovido);

    return processo;
}

// Função para imprimir os processos na fila
void imprimirFila(Fila* fila) {
    No* atual = fila->frente;
    while (atual != NULL) {
        printf("ID: %d, Tamanho: %d\n", atual->processo.id, atual->processo.tamanho);
        atual = atual->proximo;
    }
}

// Função para inserir um processo ordenadamente na fila SJF
void inserirSJF(Fila* fila, Processo processo) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->processo = processo;
    novoNo->proximo = NULL;

    No* atual = fila->frente;
    No* anterior = NULL;

    // Encontrar a posição correta na fila
    while (atual != NULL && atual->processo.tamanho < processo.tamanho) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Inserir o novo nó na posição correta
    if (anterior == NULL) {
        novoNo->proximo = fila->frente;
        fila->frente = novoNo;
        if (fila->tras == NULL) {
            fila->tras = novoNo;
        }
    } else {
        anterior->proximo = novoNo;
        novoNo->proximo = atual;
        if (atual == NULL) {
            fila->tras = novoNo;
        }
    }
}

// Função para gerar um processo aleatório
Processo gerarProcessoAleatorio(int id) {
    Processo novoProcesso;
    novoProcesso.id = id;
    novoProcesso.tamanho = rand() % 10 + 1;  // Tamanho aleatório entre 1 e 10
    return novoProcesso;
}

// Função para simular o escalonamento
// Função para simular o escalonamento
void simularEscalonamento(int metodo) {
    Fila filaFCFS, filaSJF, filaRoundRobin;
    inicializarFila(&filaFCFS);
    inicializarFila(&filaSJF);
    inicializarFila(&filaRoundRobin);

    int tempo = 0;
    int idProcesso = 1;  // Começar com o ID 1

    while (tempo < 100) {
        // Gere um novo processo a cada ciclo
        Processo novoProcesso = gerarProcessoAleatorio(idProcesso++);

        // Adicione o novo processo à fila apropriada
        switch (metodo) {
            case 1:  // FCFS
                enfileirar(&filaFCFS, novoProcesso);
                break;

            case 2:  // SJF
                inserirSJF(&filaSJF, novoProcesso);
                break;

            case 3:  // Round-Robin
                enfileirar(&filaRoundRobin, novoProcesso);
                break;
        }

        // Restante do código existente para FCFS
        if (filaFCFS.frente != NULL) {
            Processo processoExecutado = desenfileirar(&filaFCFS);
            printf("Tempo: %d, Executando processo FCFS - ID: %d, Tamanho: %d\n", tempo, processoExecutado.id, processoExecutado.tamanho);
            tempo += processoExecutado.tamanho;
        }

        // Restante do código existente para SJF
        if (filaSJF.frente != NULL) {
            Processo processoExecutado = desenfileirar(&filaSJF);
            printf("Tempo: %d, Executando processo SJF - ID: %d, Tamanho: %d\n", tempo, processoExecutado.id, processoExecutado.tamanho);
            tempo += processoExecutado.tamanho;
        }

        // Restante do código existente para Round-Robin
        if (filaRoundRobin.frente != NULL) {
            Processo processoExecutado = desenfileirar(&filaRoundRobin);
            printf("Tempo: %d, Executando processo Round-Robin - ID: %d, Tamanho: %d\n", tempo, processoExecutado.id, processoExecutado.tamanho);
            tempo += processoExecutado.tamanho;

            // Retorne o processo ao final da fila para o Round-Robin
            enfileirar(&filaRoundRobin, processoExecutado);
        }

        // Imprimir o estado das filas após cada ciclo
        printf("Estado das filas:\n");
        printf("FCFS: ");
        imprimirFila(&filaFCFS);
        printf("SJF: ");
        imprimirFila(&filaSJF);
        printf("Round-Robin: ");
        imprimirFila(&filaRoundRobin);

        // Adicione aqui a lógica para criar novos processos, se necessário

        // Aguarde para facilitar a visualização
        sleep(1);
    }

    // ... (código existente)

    // Libere a memória utilizada pelas filas
    liberarMemoria(&filaFCFS);
    liberarMemoria(&filaSJF);
    liberarMemoria(&filaRoundRobin);
}

// Função principal
int main() {
    int metodo;

    printf("Escolha o método de escalonamento:\n");
    printf("1 - FCFS\n");
    printf("2 - SJF\n");
    printf("3 - Round-Robin\n");
    scanf("%d", &metodo);

    if (metodo < 1 || metodo > 3) {
        printf("Método inválido\n");
        return 1;
    }

    simularEscalonamento(metodo);

    return 0;
}

