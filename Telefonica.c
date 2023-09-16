#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>


typedef struct Contato {
    char nome[50];
    char email[50];
    char telefone[15];
    struct Contato* proximo;
} Contato;



#define MAX_CONTATOS 100
#define cyper 42

Contato tabela[MAX_CONTATOS];


int hash(char* chave) {
    unsigned int h = 0;
    int i;
    for (i = 0; chave[i] != '\0'; i++) {
        h  += chave[i];
    }
    return h % MAX_CONTATOS;
}

int funcHashMult(char* chave) { 
    srand(time(NULL));
    int h;
    float random = rand() / (float)RAND_MAX; 
    return (int)(h * random * MAX_CONTATOS) % MAX_CONTATOS; 
}


bool validarNome(char* nome) {
    int i;
    for (i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i])) {
            return false;
        }
    }
    return true;
}


bool validarEmail(char* email) {
    int i, atCount = 0, dotCount = 0;
    for (i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            atCount++;
        }
        if (email[i] == '.') {
            dotCount++;
        }
    }
    return atCount == 1 && dotCount >= 1;
}


bool validarTelefone(char* telefone) {
    int i, digitCount = 0, dashCount = 0;
    for (i = 0; telefone[i] != '\0'; i++) {
        if (isdigit(telefone[i])) {
            digitCount++;
        }
        if (telefone[i] == '-') {
            dashCount++;
        }
    }
    return digitCount >= 8 && digitCount <= 12 && dashCount <= 1;
}

void adicionarContato() {
    Contato novoContato;

    // Exibe o menu para adicionar um contato
    printf("** Adicionar contato **\n\n");
    printf("Digite o nome: ");
    scanf("%s", novoContato.nome);

    // Valida o nome do contato
    if (!validarNome(novoContato.nome)) {
        printf("Nome inválido.\n");
        return;
    }

    printf("Digite o e-mail: ");
    scanf("%s", novoContato.email);

    // Valida o e-mail do contato
    if (!validarEmail(novoContato.email)) {
        printf("E-mail inválido.\n");
        return;
    }

    printf("Digite o telefone: ");
    scanf("%s", novoContato.telefone);

    // Valida o telefone do contato
    if (!validarTelefone(novoContato.telefone)) {
        printf("Telefone inválido.\n");
        return;
    }

    // Calcula o índice do hash do contato
    int indice = hash(novoContato.nome);

    // Insere o contato na tabela
    if (tabela[indice].proximo == NULL) {
        tabela[indice].proximo = (Contato*)malloc(sizeof(Contato));
        if (tabela[indice].proximo == NULL) {
            printf("Erro ao alocar memória para o novo contato.\n");
            return;
        }
        *tabela[indice].proximo = novoContato;
        tabela[indice].proximo->proximo = NULL;
        printf("Contato adicionado com sucesso!\n");
    } else {
        Contato* atual = tabela[indice].proximo;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = (Contato*)malloc(sizeof(Contato));
        if (atual->proximo == NULL) {
            printf("Erro ao alocar memória para o novo contato.\n");
            return;
        }
        *atual->proximo = novoContato;
        atual->proximo->proximo = NULL;
        printf("Contato adicionado com sucesso!\n");
    }
}

void listarContatos() {
    int i;
    int contatosListados = 0;  // Variável para controlar o número de contatos listados

    printf("Lista de contatos:\n");
    printf("Nome | E-mail | Telefone\n");

    for (i = 0; i < MAX_CONTATOS; i++) {
        if (tabela[i].proximo != NULL) {
            Contato* atual = tabela[i].proximo;
            while (atual != NULL) {
                printf("%s | %s | %s\n", atual->nome, atual->email, atual->telefone);
                atual = atual->proximo;
                contatosListados++;

                // Adiciona uma pausa após listar cada grupo de contatos
                if (contatosListados % 10 == 0) {
                    printf("Pressione Enter para continuar...");
                    while (getchar() != '\n');
                }
            }
        }
    }

    printf("\n");
}

void buscarContato() {
    char nome[50];
    int indice;

    // Exibe o menu para buscar um contato
    printf("** Buscar contato **\n\n");
    printf("Digite o nome do contato a ser buscado: ");
    scanf("%s", nome);

    // Calcula o índice do hash do contato
    indice = hash(nome);

    // Busca o contato na tabela
    Contato* contato = tabela[indice].proximo;
    while (contato != NULL && strcmp(contato->nome, nome) != 0) {
        contato = contato->proximo;
    }

    // Imprime o contato encontrado
    if (contato != NULL) {
        printf("Contato encontrado:\n");
        printf("Nome: %s\n", contato->nome);
        printf("E-mail: %s\n", contato->email);
        printf("Telefone: %s\n", contato->telefone);
    } else {
        printf("Contato não encontrado.\n");
    }
}
void exportarContatos() {
    FILE* arquivo = fopen("contatos.txt", "w");

    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "nome,email,telefone\n");

    for (int i = 0; i < MAX_CONTATOS; i++) {
        if (tabela[i].proximo != NULL) {
            Contato* atual = tabela[i].proximo;
            while (atual != NULL) {
                fprintf(arquivo, "%s,%s,%s\n", atual->nome, atual->email, atual->telefone);
                atual = atual->proximo;
            }
        }
    }

    fclose(arquivo);

    printf("Contatos exportados com sucesso.\n");
}

int main() {
    int opcao;
    int tentativas = 0;

    // Exibe o menu principal
    while (true) {
        printf("** Agenda de contatos **\n\n");
        printf("1. Adicionar contato\n");
        printf("2. Listar contatos\n");
        printf("3. Buscar contato\n");
        printf("4. Exportar contatos\n");
        printf("5. Sair\n\n");

        // Valida a opção selecionada
        do {
            printf("Digite a opção desejada: ");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 5);

        // Processa a opção selecionada
        switch (opcao) {
            case 1:
                adicionarContato();
                break;
            case 2:
                listarContatos();
                break;
            case 3:
                buscarContato();
                break;
            case 4:
                exportarContatos();
                break;
            case 5:
                printf("Saindo...\n");
                return 0;
        }

        // Verifica se o usuário digitou uma opção válida
        if (tentativas == 3) {
            printf("Excedeu o número máximo de tentativas.\n");
            return 1;
        }
    }

    return 0;
}
