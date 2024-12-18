#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de usuario para login
typedef struct {
    char username[50];
    char password[50];
} User;

// Estrutura para dados do cliente
typedef struct {
    char nomeResponsavel[50];
    char nomeEmpresa[50];
    char cnpj[20];
    char razaoSocial[50];
    char nomeFantasia[50];
    char telefone[20];
    char email[50];
    char endereco[100];
    char dataAbertura[15];
    char regiao[50]; // Região do cliente
    float residuosTratados; // Quantidade de resíduos tratados
    float custoMensal;      // Custo mensal
} Cliente;

// Funcao para criptografar uma string (simples)
void criptografar(char *str) {
    while (*str) {
        *str = *str + 3; // Desloca cada caractere em 3 posicoes na tabela ASCII
        str++;
    }
}

// Funcao para descriptografar uma string (simples)
void descriptografar(char *str) {
    while (*str) {
        *str = *str - 3; // Reverte o deslocamento
        str++;
    }
}

// Funcao para simular o login do usuario
int login() {
    User user;
    FILE *file = fopen("users.dat", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios. Certifique-se de que o sistema esta configurado corretamente.\n");
        return 0;
    }

    char inputUser[50], inputPass[50];
    printf("=== Tela de Login ===\n");
    printf("Usuario: ");
    scanf(" %49s", inputUser);
    printf("Senha: ");
    scanf(" %49s", inputPass);

    while (fread(&user, sizeof(User), 1, file)) {
        descriptografar(user.password);
        if (strcmp(user.username, inputUser) == 0 && strcmp(user.password, inputPass) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    printf("Usuario ou senha incorretos.\n");
    return 0;
}

// Funcao para cadastrar cliente
void cadastrarCliente() {
    Cliente cliente;
    FILE *file = fopen("clientes.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    printf("=== Cadastro de Cliente ===\n");
    printf("Nome do responsavel: ");
    scanf(" %[^\n]%*c", cliente.nomeResponsavel);
    printf("Nome da empresa: ");
    scanf(" %[^\n]%*c", cliente.nomeEmpresa);
    printf("CNPJ: ");
    scanf(" %19s", cliente.cnpj);
    printf("Razao Social: ");
    scanf(" %[^\n]%*c", cliente.razaoSocial);
    printf("Nome Fantasia: ");
    scanf(" %[^\n]%*c", cliente.nomeFantasia);
    printf("Telefone: ");
    scanf(" %19s", cliente.telefone);
    printf("Email: ");
    scanf(" %49s", cliente.email);
    printf("Endereco (Rua, Numero, Bairro, Cidade, Estado, CEP): ");
    scanf(" %[^\n]%*c", cliente.endereco);
    printf("Data de abertura (DD/MM/AAAA): ");
    scanf(" %14s", cliente.dataAbertura);
    printf("Regiao (Cidade/Estado): ");
    scanf(" %[^\n]%*c", cliente.regiao);
    printf("Residuos tratados (kg): ");
    scanf("%f", &cliente.residuosTratados);
    printf("Custo mensal (R$): ");
    scanf("%f", &cliente.custoMensal);

    fwrite(&cliente, sizeof(Cliente), 1, file);
    fclose(file);

    printf("Cliente cadastrado com sucesso!\n");
}

// Funcao para exibir relatorios individualizados
void exibirRelatoriosIndividualizados() {
    Cliente cliente;
    FILE *file = fopen("clientes.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes. Certifique-se de que ha registros.\n");
        return;
    }

    float totalResiduosSemestral = 0, totalGastosMensais = 0;
    int count = 0;

    printf("\n=== Relatorios Individualizados ===\n");
    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        totalResiduosSemestral += cliente.residuosTratados;  // Considerando que o total é calculado semestralmente
        totalGastosMensais += cliente.custoMensal;           // Cálculo mensal
        count++;
    }
    fclose(file);

    printf("Total de insumos tratados semestralmente: %.2f kg\n", totalResiduosSemestral);
    printf("Total de gastos mensais: R$ %.2f\n", totalGastosMensais);
    printf("Numero de clientes: %d\n", count);
}

// Funcao para exibir relatorios globais
void exibirRelatoriosGlobais() {
    Cliente cliente;
    FILE *file = fopen("clientes.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes. Certifique-se de que ha registros.\n");
        return;
    }

    float totalResiduos = 0, totalCusto = 0;
    int count = 0;
    char regiaoMaiorResiduos[50];
    float maiorResiduos = 0;
    char regiaoMenorProducao[50];
    float menorProducao = 999999;

    printf("\n=== Relatorios Globais ===\n");
    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        totalResiduos += cliente.residuosTratados;
        totalCusto += cliente.custoMensal;
        count++;

        if (cliente.residuosTratados > maiorResiduos) {
            maiorResiduos = cliente.residuosTratados;
            strcpy(regiaoMaiorResiduos, cliente.regiao);
        }

        if (cliente.custoMensal < menorProducao) {
            menorProducao = cliente.custoMensal;
            strcpy(regiaoMenorProducao, cliente.regiao);
        }
    }
    fclose(file);

    printf("Total de residuos tratados: %.2f kg\n", totalResiduos);
    printf("Custo total estimado: R$ %.2f\n", totalCusto);
    printf("Numero de clientes: %d\n", count);
    printf("Regiao com maior volume de residuos tratados: %s (%.2f kg)\n", regiaoMaiorResiduos, maiorResiduos);
    printf("Regiao com menor producao: %s (Custo: R$ %.2f)\n", regiaoMenorProducao, menorProducao);
}

// Funcao para salvar relatorios em TXT ou CSV
void salvarRelatorioEmArquivo() {
    int opcao;
    printf("\nEscolha o formato de arquivo para salvar:\n");
    printf("1. Salvar em TXT\n");
    printf("2. Salvar em CSV\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        FILE *file = fopen("relatorio.txt", "w");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de relatorio.\n");
            return;
        }

        fprintf(file, "Relatorio de Residuos e Custos\n");
        fprintf(file, "Total de residuos tratados: %.2f kg\n", 1000.0);  // Exemplo
        fprintf(file, "Custo total estimado: R$ %.2f\n", 5000.0);  // Exemplo
        fclose(file);
        printf("Relatorio salvo em relatorio.txt\n");

    } else if (opcao == 2) {
        FILE *file = fopen("relatorio.csv", "w");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de relatorio.\n");
            return;
        }

        fprintf(file, "Regiao, Residuos Tratados (kg), Custo Mensal (R$)\n");
        fprintf(file, "Sao Paulo, 1200, 500\n");  // Exemplo
        fclose(file);
        printf("Relatorio salvo em relatorio.csv\n");
    } else {
        printf("Opcao invalida.\n");
    }
}

// Funcao principal do menu
void menu() {
    int opcao;
    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Cadastro de Cliente\n");
        printf("2. Relatorios Individualizados\n");
        printf("3. Relatorios Globais\n");
        printf("4. Salvar Relatorio em Arquivo\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarCliente();
                break;
            case 2:
                exibirRelatoriosIndividualizados();
                break;
            case 3:
                exibirRelatoriosGlobais();
                break;
            case 4:
                salvarRelatorioEmArquivo();
                break;
            case 5:
                printf("Saindo do sistema.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);
}

int main() {
    printf("Bem-vindo ao Sistema de Gerenciamento de Solucoes Ambientais\n");

    if (login()) {
        menu();
    } else {
        printf("Encerrando o sistema.\n");
    }

    return 0;
}
