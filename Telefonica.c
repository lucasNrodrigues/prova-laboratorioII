#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>
#include <regex.h>


typedef struct Contato {
    char nome[50];
    char email[50];
    char telefone[15];
    struct Contato* proximo;
} Contato;



#define MAX_CONTATOS 100
#define cyper 42

Contato tabela[MAX_CONTATOS];
int numElementos = 0;

int hash(char* chave) {
    unsigned int h = 0;
    float A = 0.6180339887;  
    int i;
    for (i = 0; chave[i] != '\0'; i++) {
        h += chave[i];
    }

    
    float valorDecimal = A * h;
    return (int)(MAX_CONTATOS * (valorDecimal - (int)valorDecimal));
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
    
    const char* regex_pattern = "^[A-Za-z0-9+_.-]+@[A-Za-z0-9.-]+$";

    regex_t regex;
    int result = regcomp(&regex, regex_pattern, REG_EXTENDED);

    if (result != 0) {
        printf("Erro ao compilar a expressão regular.\n");
        return false;
    }

    result = regexec(&regex, email, 0, NULL, 0);

    regfree(&regex);

    if (result == 0) {
        return true; 
    } else {
        printf("E-mail inválido. Digite um e-mail válido.\n");
        return false; 
    }
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
    if (numElementos >= MAX_CONTATOS) {
        printf("A tabela está cheia. Não é possível adicionar mais contatos.\n");
        return;
    }

    Contato novoContato;

    printf("** Adicionar contato **\n\n");

    
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

    while (tabela[indice].nome[0] != '\0') {
        indice = (indice + 1) % MAX_CONTATOS;
    }

    tabela[indice] = novoContato;
    numElementos++;

    printf("Contato adicionado com sucesso!\n");
}

void listarContatos() {
    printf("Lista de contatos:\n");
    printf("Nome | E-mail | Telefone\n");

    for (int i = 0; i < MAX_CONTATOS; i++) {
        if (tabela[i].nome[0] != '\0') {
            printf("%s | %s | %s\n", tabela[i].nome, tabela[i].email, tabela[i].telefone);
        }
    }
}
void buscarContato() {
    char nome[50];

    printf("** Buscar contato **\n\n");
    printf("Digite o nome do contato a ser buscado: ");
    scanf("%s", nome);

    
    for (int i = 0; i < MAX_CONTATOS; i++) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            
            printf("Contato encontrado:\n");
            printf("Nome: %s\n", tabela[i].nome);
            printf("E-mail: %s\n", tabela[i].email);
            printf("Telefone: %s\n", tabela[i].telefone);
            return; 
        }
    }

    printf("Contato não encontrado.\n");
}

void exportarContatos() {
    FILE* arquivo = fopen("contatos.txt", "w");

    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "nome,email,telefone\n");

    for (int i = 0; i < MAX_CONTATOS; i++) {
        if (strlen(tabela[i].nome) > 0) {
            fprintf(arquivo, "%s,%s,%s\n", tabela[i].nome, tabela[i].email, tabela[i].telefone);
        }
    }

    fclose(arquivo);

    printf("Contatos exportados com sucesso.\n");
}
void deletarContato(char* nome) {
    for (int i = 0; i < MAX_CONTATOS; i++) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            
            tabela[i].nome[0] = '\0';
            tabela[i].email[0] = '\0';
            tabela[i].telefone[0] = '\0';
            numElementos--;
            printf("Contato '%s' excluído com sucesso.\n", nome);
            return; 
        }
    }
    printf("Contato '%s' não encontrado.\n", nome);
}

int main() {
    int opcao;
    int tentativas = 0;

    
    while (true) {
        printf("** Agenda de contatos **\n\n");
        printf("1. Adicionar contato\n");
        printf("2. Listar contatos\n");
        printf("3. Buscar contato\n");
        printf("4. Exportar contatos\n");
        printf("5. Deletar contato\n");
        printf("6. Sair\n\n");

        
        do {
            printf("Digite a opção desejada: ");
            while (getchar() != '\n'); 
        } while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 6);

        
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
                printf("Digite o nome do contato a ser excluído: ");
                char nomeExcluir[50];
                scanf("%s", nomeExcluir);
                deletarContato(nomeExcluir);
                break;
            case 6:
                printf("Saindo...\n");
                return 0;
        }
        
        if (tentativas == 3) {
            printf("Excedeu o número máximo de tentativas.\n");
            return 1;
        }
    }

    return 0;
}
