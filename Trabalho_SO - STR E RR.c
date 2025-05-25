#include <stdio.h>
#include <string.h>

#define MAX 15
#define TIME_QUANTUM 2

typedef struct {
    char nome[10];
    int chegada;
    int duracao;
    int restante;
    int finalizado;
} Processo;

void lerProcessos(Processo p[], int *n) {
    do {
        printf("Quantidade de processos (2 a 15): ");
        scanf("%d", n);
    } while (*n < 2 || *n > MAX);

    for (int i = 0; i < *n; i++) {
        printf("\nProcesso %d:\n", i + 1);
        printf("Nome: ");
        scanf("%s", p[i].nome);
        printf("Tempo de chegada: ");
        scanf("%d", &p[i].chegada);
        printf("Tempo de processamento: ");
        scanf("%d", &p[i].duracao);

        p[i].restante = p[i].duracao;
        p[i].finalizado = 0;
    }
}

void executarSRT(Processo p[], int n) {
    int tempo = 0, completados = 0;

    printf("\n--- Timeline SRT ---\n");

    while (completados < n) {
        int menor = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].chegada <= tempo && p[i].restante > 0) {
                if (menor == -1 || p[i].restante < p[menor].restante)
                    menor = i;
            }
        }

        if (menor != -1) {
            printf("|%d %s ", tempo, p[menor].nome);
            p[menor].restante--;

            if (p[menor].restante == 0) {
                p[menor].finalizado = 1;
                completados++;
            }
        } else {
            printf("|%d IDLE ", tempo);
        }

        tempo++;
    }

    printf("|%d\n", tempo);
}

void executarRR(Processo p[], int n) {
    int tempo = 0, completados = 0;
    int fila[MAX], inicio = 0, fim = 0;
    int visitado[MAX] = {0};

    printf("\n--- Timeline Round Robin ---\n");

    for (int i = 0; i < n; i++) {
        if (p[i].chegada == 0) {
            fila[fim++] = i;
            visitado[i] = 1;
        }
    }

    while (completados < n) {
        if (inicio == fim) {
            printf("|%d IDLE ", tempo);
            tempo++;

            for (int i = 0; i < n; i++) {
                if (!visitado[i] && p[i].chegada <= tempo) {
                    fila[fim++] = i;
                    visitado[i] = 1;
                }
            }

            continue;
        }

        int i = fila[inicio++];
        int exec = (p[i].restante >= TIME_QUANTUM) ? TIME_QUANTUM : p[i].restante;

        for (int t = 0; t < exec; t++) {
            printf("|%d %s ", tempo, p[i].nome);
            tempo++;

            for (int j = 0; j < n; j++) {
                if (!visitado[j] && p[j].chegada <= tempo) {
                    fila[fim++] = j;
                    visitado[j] = 1;
                }
            }
        }

        p[i].restante -= exec;

        if (p[i].restante > 0) {
            fila[fim++] = i;
        } else {
            completados++;
        }
    }

    printf("|%d\n", tempo);
}

int main() {
    Processo processos[MAX];
    int n, escolha;

    lerProcessos(processos, &n);

    printf("\nEscolha o algoritmo:\n1 - SRT (Shortest Remaining Time)\n2 - Round Robin\nOpcao: ");
    scanf("%d", &escolha);

    if (escolha == 1) {
        executarSRT(processos, n);
    } else if (escolha == 2) {
        executarRR(processos, n);
    } else {
        printf("Opcao invalida.\n");
    }

    return 0;
}
