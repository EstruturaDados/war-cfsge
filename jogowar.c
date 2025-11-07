#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// ================== Constantes ==================
#define MAX_TERRITORIOS 5
#define TAM_STRING 32

// ================== Estruturas ==================
struct Territorio {
    char nome[TAM_STRING];
    char corExercito[TAM_STRING];
    int  tropas;
};

struct Jogador {
    char cor[TAM_STRING];       // cor do exército do jogador
    int  missao;                // 0, 1 ou 2
    char corAlvo[TAM_STRING];   // usado na missão 1
};

// ================== Protótipos ==================
void  limpaBufferEntrada(void);

void  cadastrarTerritorios(struct Territorio *mapa, int *totalTerritorio);
void  listarTerritorios(const struct Territorio *mapa, int total);

void  escolhaCor(const struct Territorio *territorios, struct Jogador *jogador, int numTerritorios);
void  sortearMissao(struct Jogador *jogador, const struct Territorio *mapa, int totalTerritorio);

void  atacar(struct Territorio *atacante, struct Territorio *defensor);
void  loopAtaque(struct Territorio *mapa, int totalTerritorio);

void  verificarMissao(const struct Territorio *mapa, int totalTerritorio, const struct Jogador *jogador);
void  menu(struct Territorio *mapa, int totalTerritorio, struct Jogador *jogador);

// ================== Função principal ==================
int main(void) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand((unsigned)time(NULL));

    struct Territorio *mapa = (struct Territorio *)calloc(MAX_TERRITORIOS, sizeof(struct Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    struct Jogador *jogador = (struct Jogador *)calloc(1, sizeof(struct Jogador));
    if (!jogador) {
        printf("Erro ao alocar memória!\n");
        free(mapa);
        return 1;
    }

    int totalTerritorio = 0;

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, &totalTerritorio);

    // Exibe o mapa
    listarTerritorios(mapa, totalTerritorio);

    // Sorteia a cor do jogador
    escolhaCor(mapa, jogador, totalTerritorio);

    // Sorteia a missão
    sortearMissao(jogador, mapa, totalTerritorio);

    // Menu de ações (ataques / checar missão)
    menu(mapa, totalTerritorio, jogador);

    printf("\n--- Fim de jogo ---\n");
    free(mapa);
    free(jogador);
    return 0;
}

// ================== Implementações ==================

// Cadastrar territórios
void cadastrarTerritorios(struct Territorio *mapa, int *totalTerritorio) {
    printf("\n=======================\n");
    printf("       JOGO WAR         \n");
    printf("=======================\n");
    printf("\n Vamos começar! Cadastre os 5 territórios \n"); 

    while (*totalTerritorio < MAX_TERRITORIOS) {
        int indice = *totalTerritorio;
       
        printf("\n--- Cadastrando o território %d ---\n", indice + 1);

        printf("Digite o nome do território: ");
        fgets(mapa[indice].nome, TAM_STRING, stdin);
        mapa[indice].nome[strcspn(mapa[indice].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[indice].corExercito, TAM_STRING, stdin);
        mapa[indice].corExercito[strcspn(mapa[indice].corExercito, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[indice].tropas);
        limpaBufferEntrada();

        (*totalTerritorio)++;
    }
}

// Listar territórios
void listarTerritorios(const struct Territorio *mapa, int total) {
    printf("\n====================================\n");
    printf("            MAPA DO MUNDO \n");
    printf("====================================\n\n");

    for (int i = 0; i < total; i++) {
        printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }
}

// Escolha (sorteio) da cor do exército do jogador
void escolhaCor(const struct Territorio *territorios, struct Jogador *jogador, int numTerritorios) {
    int indice = rand() % numTerritorios; // sorteia um território existente
    strcpy(jogador->cor, territorios[indice].corExercito);

    printf("\n-------------------------------------\n");
    printf("  Seu exército sorteado foi:  %s.\n", jogador->cor);
    printf("-------------------------------------\n");
}

// Sortear missão
void sortearMissao(struct Jogador *jogador, const struct Territorio *mapa, int totalTerritorio) {
    jogador->missao = rand() % 3;
    int corAleatoria;

    printf("\n--- SUA MISSÃO ---\n");
    switch (jogador->missao) {
        case 0:
            printf("Conquistar 2 territórios.\n\n");
            for (int i = 0; i < totalTerritorio; i++) {
                printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
                       i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
            }
            break;

        case 1:
            // Seleciona uma cor-alvo diferente da cor do jogador
            do {
                corAleatoria = rand() % totalTerritorio;
                strcpy(jogador->corAlvo, mapa[corAleatoria].corExercito);
            } while (strcmp(jogador->corAlvo, jogador->cor) == 0);
            printf("Destruir o exército %s.\n\n", jogador->corAlvo);
            for (int i = 0; i < totalTerritorio; i++) {
                printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
                       i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
            }
            break;

        case 2:
            printf("Destruir 1 territorio com mais de 5 tropas.\n\n");
            for (int i = 0; i < totalTerritorio; i++) {
                printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
                       i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
            }
            break;
    }
}

// Simulação de um ataque
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    printf("\n--- Simulação de Ataque ---\n");
    {
        struct Territorio temp[2] = { *atacante, *defensor };
        const struct Territorio *mapa = temp;
        for (int i = 0; i < 2; i++) {
            printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
                   i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
        }
    }
    printf("%s (%s) atacando %s (%s)\n",
           atacante->nome, atacante->corExercito,
           defensor->nome, defensor->corExercito);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(" Sucesso!\n");
        defensor->tropas -= 1;

        if (defensor->tropas <= 0) {
            printf(" O território %s foi conquistado!\n", defensor->nome);
            {
                struct Territorio temp2[2] = { *atacante, *defensor };
                const struct Territorio *mapa = temp2;
                for (int i = 0; i < 2; i++) {
                    printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
                           i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
                }
            }
            strcpy(defensor->corExercito, atacante->corExercito);
            // transfere metade das tropas
            defensor->tropas = atacante->tropas / 2;
            atacante->tropas /= 2;
        }
    } else {
        printf("O ataque falhou! Atacante perdeu 1 tropa.\n");
        atacante->tropas -= 1;
    }
}

// Loop de ataques
void loopAtaque(struct Territorio *mapa, int totalTerritorio) {
    int iAtacante, iDefensor;
    printf("\n--- Fase de ataques ---\n");
    for (int i = 0; i < totalTerritorio; i++) {
        printf(" %d. %-14s | Cor: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }

    do {
        printf("\nEscolha o território atacante (1 a %d) ou 0 para sair: ", totalTerritorio);
        if (scanf("%d", &iAtacante) != 1) { limpaBufferEntrada(); continue; }
        if (iAtacante == 0) break;

        printf("Escolha o território defensor (1 a %d): ", totalTerritorio);
        if (scanf("%d", &iDefensor) != 1) { limpaBufferEntrada(); continue; }
        limpaBufferEntrada();

        if (iAtacante < 1 || iAtacante > totalTerritorio ||
            iDefensor < 1 || iDefensor > totalTerritorio) {
            printf("Opção inválida!\n");
            continue;
        }
        if (iAtacante == iDefensor) {
            printf("Opção inválida!\n");
            continue;
        }

        // Impede ataque entre territórios da mesma cor
        if (strcmp(mapa[iAtacante - 1].corExercito, mapa[iDefensor - 1].corExercito) == 0) {
            printf("Opção inválida: o exército '%s' não pode atacar território do mesmo exército.\n",
                   mapa[iAtacante - 1].corExercito);
            continue;
        }

        if (mapa[iAtacante - 1].tropas <= 1) {
            printf("O território atacante precisa ter mais de 1 tropa para atacar.\n");
            continue;
        }

        // Executa o ataque
        atacar(&mapa[iAtacante - 1], &mapa[iDefensor - 1]);

        // Exibe o estado atualizado do mapa
        listarTerritorios(mapa, totalTerritorio);

        // Pausa
        printf("\nPressione Enter para continuar para o próximo turno.\n");
        getchar();

    } while (1);
}

// Verificar missão
void verificarMissao(const struct Territorio *mapa, int totalTerritorio, const struct Jogador *jogador) {
    int cont = 0;
    int cumpriu = 0;

    switch (jogador->missao) {
        case 0: // Conquistar 2 territórios
            for (int i = 0; i < totalTerritorio; i++)
                if (strcmp(mapa[i].corExercito, jogador->cor) == 0)
                    cont++;
            if (cont >= 2) cumpriu = 1;
            break;

        case 1: // Destruir o exército alvo sorteado
            for (int i = 0; i < totalTerritorio; i++)
                if (strcmp(mapa[i].corExercito, jogador->corAlvo) == 0)
                    cont++;
            if (cont == 0) cumpriu = 1;
            break;

        case 2: // Manter 2 territórios com 3 ou mais tropas
            for (int i = 0; i < totalTerritorio; i++)
                if (strcmp(mapa[i].corExercito, jogador->cor) == 0 && mapa[i].tropas >= 3)
                    cont++;
            if (cont >= 2) cumpriu = 1;
            break;
    }

    if (cumpriu)
        printf("\nMissão concluída! Parabéns!\n");
    else
        printf("\nSua missão ainda não foi concluída.\n");
}

// Menu de ações
void menu(struct Territorio *mapa, int totalTerritorio, struct Jogador *jogador) {
    (void)jogador; // (se quiser usar no futuro)
    int opcao;

    do {
        printf("\n--- MENU DE AÇÕES ---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("Escolha sua ação: ");
        if (scanf("%d", &opcao) != 1) { limpaBufferEntrada(); continue; }
        limpaBufferEntrada();

        switch (opcao) {
            case 1:
                loopAtaque(mapa, totalTerritorio);
                break;
            case 2:
                verificarMissao(mapa, totalTerritorio, jogador);
                printf("\nPressione Enter para continuar...\n");
                getchar();
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

// Limpa o buffer do teclado
void limpaBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
