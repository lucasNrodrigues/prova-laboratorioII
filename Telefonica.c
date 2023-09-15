#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <locale.h>


typedef struct {
    char nome[50];
    char email[50];
    char telefone[15];
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
    printf("Digite o nome: ");
    scanf("%s", novoContato.nome);
    if (!validarNome(novoContato.nome)) {
        printf("Nome inválido.\n");
        return;
    }

    printf("Digite o e-mail: ");
    scanf("%s", novoContato.email);
    if (!validarEmail(novoContato.email)) {
        printf("E-mail inválido.\n");
        return;
    }

    printf("Digite o telefone: ");
    scanf("%s", novoContato.telefone);
    if (!validarTelefone(novoContato.telefone)) {
        printf("Telefone inválido.\n");
        return;
    }

    int indice = hash(novoContato.nome);

    if (strlen(tabela[indice].nome) == 0) {
        tabela[indice] = novoContato;
        printf("Contato adicionado com sucesso!\n");
    } else {
       
        Contato* atual = &tabela[indice];
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = (Contato*)malloc(sizeof(Contato)); 
        if (atual->proximo == NULL) {
            printf("Erro ao alocar memória para o novo contato.\n");
            return;
        }
        atual = atual->proximo;
        *atual = novoContato;
        atual->proximo = NULL;
        printf("Contato adicionado com sucesso!\n");
    }
}

void listarContatos() {
    int i;

    printf("Lista de contatos:\n");
    printf("Nome | E-mail | Telefone\n");

    for (i = 0; i < MAX_CONTATOS; i++) {
        if (strlen(tabela[i].nome) > 0) {
            Contato* atual = &tabela[i];
            while (atual != NULL) {
                printf("%s | %s | %s\n", atual->nome, atual->email, atual->telefone);
                atual = atual->proximo;
            }
        }
    }
}

void buscarContato() {
    char nome[50];
    int indice;

    
    printf("Digite o nome do contato a ser buscado: ");
    scanf("%s", nome);

    
    indice = hash(nome);

    if (strlen(tabela[indice].nome) > 0) {
       
        printf("Contato encontrado:\n");
        printf("Nome: %s\n", tabela[indice].nome);
        printf("E-mail: %s\n", tabela[indice].email);
        printf("Telefone: %s\n", tabela[indice].telefone);
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

   
    int i;
    for ( i = 0; i < MAX_CONTATOS; i++) {
        if (strlen(tabela[i].nome) > 0) {
            fprintf(arquivo, "%s,%s,%s\n", tabela[i].nome, tabela[i].email, tabela[i].telefone);
        }
    }

    
    fclose(arquivo);

    
    printf("Contatos exportados com sucesso.\n");
}

int main() {
    int opcao;
    setlocale(LC_ALL, "Portuguese");

   
    while (1) {
        printf(" MENU:\n");
        printf("1. Adicionar contato\n");
        printf("2. Listar contatos\n");
        printf("3. Buscar contato\n");
        printf("4. Exportar contatos\n");
        printf("5. Sair\n");
        printf("Digite a opcao desejada: \n");
        scanf("%d", &opcao);

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
                exit(0);
            default:
                printf("Opcaoo invalida. Tente novamente.\n");
        }

        printf("\n");
    }
}

