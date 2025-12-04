/**
 * @file processamento.c
 * @brief Ficheiro de processamento de comandos do sistema de vacinas.
 * 
 * @details Contém as funções de processamento dos comandos do sistema (parsing, etc.).
 *          Cada função é responsável por ler e preparar os argumentos
 *          antes de chamar a função correspondente que executa o comando.
 * 
 * @author ist1113402
 * @note project - iaed25
 */

 #include "processamento.h"

 void comando_v(const char* lote, Data data, SistemaVacinas* sistema);

/*------------------------------- PROCESSAMENTO DE COMANDOS ------------------------------*/

/**
 * @brief Processa o comando c - adicionar vacina
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param buffer Buffer para leitura dos dados
 * 
 * @details Lê do stdin os parâmetros do comando:
 *          - Lote da vacina
 *          - Data de validade (dia-mês-ano)
 *          - Número de doses
 *          - Nome da vacina
 * 
 *          Remove a quebra de linha final e chama a função comando_c.
 */
void process_comando_c(SistemaVacinas* sistema, char* buffer) {
    char lote[BUFMAX + 1];
    Data data = {0, 0, 0};
    int doses = 0;
    char nome[MAX_NOME_VACINA + 1];
    
    if (fgets(buffer, BUFMAX, stdin) != NULL) {
        sscanf(buffer, " %s %d-%d-%d %d %[^\n]", lote, &data.dia, &data.mes, &data.ano, &doses, nome);
        nome[strcspn(nome, "\n")] = '\0'; 
        comando_c(lote, data, doses, nome, sistema);
    }
}


/**
 * @brief Processa o comando l - listar vacinas
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param buffer Buffer para leitura dos dados
 * 
 * @details Lê do stdin os nomes das vacinas a filtrar (opcional).
 *          Divide os argumentos usando espaços como delimitadores.
 *          Chama a função comando_l com a lista de argumentos.
 */
void process_comando_l(SistemaVacinas* sistema, char* buffer) {
    memset(buffer, 0, BUFMAX);
    fgets(buffer, BUFMAX, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    int argc = 0;
    char *argv[MAX_NOME_VACINA] = {0};
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    comando_l(sistema, argc, argv);
}


/**
 * @brief Processa o comando t - alterar/consultar data
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * 
 * @details Lê do stdin a nova data no formato dia-mês-ano.
 *          Se todos os valores forem -1, consulta a data atual.
 *          Chama a função comando_t com os valores lidos.
 */
void process_comando_t(SistemaVacinas* sistema) {
    int dia, mes, ano;
    scanf(" %d-%d-%d", &dia, &mes, &ano);
    comando_t(sistema, dia, mes, ano);
}


/**
 * @brief Processa o comando a - adicionar inoculação
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param buffer Buffer para leitura dos dados
 * 
 * @details Lê do stdin:
 *          - Nome do utente (pode estar entre aspas)
 *          - Nome da vacina
 * 
 *          Remove aspas se estiverem no nome do utente e chama a função comando_a.
 */
void process_comando_a(SistemaVacinas* sistema, char* buffer) {
    char nome_utente[BUFMAX + 1];
    char nome_vacina[MAX_NOME_VACINA + 1];
    fgets(buffer, BUFMAX, stdin);

    if (strchr(buffer, '"')) {
        sscanf(buffer, " \"%[^\"]\" %s", nome_utente, nome_vacina);
    } else {
        sscanf(buffer, " %s %s", nome_utente, nome_vacina);
    }

    comando_a(nome_utente, nome_vacina, sistema);
}


/**
 * @brief Processa o comando u -  listar todas as inoculações ou as inoculações de um utente:
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param buffer Buffer para leitura dos dados
 * 
 * @details Lê do stdin o nome do utente (opcional, pode estar entre aspas).
 *          Se não for fornecido nome, lista todas as inoculações.
 *          Chama a função comando_u com o nome do utente ou NULL.
 */
void process_comando_u(SistemaVacinas* sistema, char* buffer) {
    char nome_utente[BUFMAX + 1] = "";
    fgets(buffer, BUFMAX, stdin);

    int utente_passado = sscanf(buffer, " %[^\n]", nome_utente);
    if (utente_passado != 1) {
        comando_u(sistema, NULL);
        return;
    }

    if (strchr(buffer, '"')) {
        sscanf(buffer, " \"%[^\"]\"", nome_utente);
    } else {
        sscanf(buffer, " %s", nome_utente);
    }

    comando_u(sistema, nome_utente);
}


/**
 * @brief Processa o comando r - remover lote
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param buffer Buffer para leitura dos dados
 * 
 * @details Lê do stdin o lote a remover.
 *          Remove a quebra de linha final e chama a função comando_r.
 */
void process_comando_r(SistemaVacinas* sistema, char* buffer) {
    char lote[MAX_LOTE + 1];
    fgets(buffer, BUFMAX, stdin);
    sscanf(buffer, " %20s", lote);
    lote[strcspn(lote, "\n")] = '\0';
    comando_r(lote, sistema);
}


/**
 * @brief Processa o comando d - remover inoculações
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param buffer Buffer para leitura dos dados
 * 
 * @details Lê do stdin:
 *          - Nome do utente (pode estar entre aspas)
 *          - Data no formato dia-mês-ano (opcional)
 *          - Número do lote (opcional)
 * 
 *          Chama a função comando_d com os parâmetros processados.
 */
void process_comando_d(SistemaVacinas* sistema, char* buffer) {
    char nome_utente[BUFMAX + 1];
    char resto_buffer[BUFMAX + 1];
    Data data = { -1, -1, -1 };
    char lote[MAX_LOTE + 1] = "\0";

    fgets(buffer, BUFMAX, stdin);
    
    if (strchr(buffer, '"')) {
        sscanf(buffer, " \"%[^\"]\"%[^\n]", nome_utente, resto_buffer);
    } else {
        sscanf(buffer, " %s%[^\n]", nome_utente, resto_buffer);
    }

    sscanf(resto_buffer, " %d-%d-%d %s", &data.dia, &data.mes, &data.ano, lote);
    sscanf(resto_buffer, " %d-%d-%d", &data.dia, &data.mes, &data.ano);

    comando_d(nome_utente, data, sistema, lote);
}


void process_comando_v(SistemaVacinas* sistema, char* buffer) {
    char lote[MAX_LOTE + 1];
    Data data = {0, 0, 0};

    fgets(buffer, BUFMAX, stdin);
    sscanf(buffer, " %s %02d-%02d-%02d", lote, &data.dia, &data.mes, &data.ano);
    comando_v(lote, data, sistema);
}