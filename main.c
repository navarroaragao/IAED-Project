/**
 * @file main.c
 * @brief Ficheiro principal do sistema de gestão de vacinas
 * 
 * @details Este ficheiro contém a função main do sistema de vacinas e o processamento
 *          dos comandos. Implementa o loop principal do programa e a 
 *          distribuição dos comandos para as respetivas funções de processamento.
 * 
 * @author ist1113402
 * @note project - iaed25
 */


#include "main.h"


/**
 * @brief Processa o comando escolhido e redireciona para a respetiva função
 * 
 * @param comando caracter que identifica o comando a executar
 * @param sistema ponteiro para a estrutura do sistema de vacinas
 * @param buffer buffer para armazenamento temporário de dados
 * 
 * @details Analisa o comando recebido e chama a função correspondente para o processar.
 *          No caso do comando 'q', liberta a memória e termina o programa.
 *          Para comandos desconhecidos, limpa o buffer de entrada.
 * 
 * @note O buffer é utilizado para armazenar argumentos dos comandos
 */
void processa_comando_escolhido(char comando, SistemaVacinas* sistema, char buffer[]) {
    switch (comando) {
        case 'q':
            libertar_memoria(sistema);
            exit(0);
            
        case 'c': process_comando_c(sistema, buffer); break;
        case 'l': process_comando_l(sistema, buffer); break;
        case 'a': process_comando_a(sistema, buffer); break;
        case 'r': process_comando_r(sistema, buffer); break;
        case 'd': process_comando_d(sistema, buffer); break;
        case 't': process_comando_t(sistema); break;
        case 'u': process_comando_u(sistema, buffer); break;
        case 'v': process_comando_v(sistema, buffer); break;
            
        default:
            while (getchar() != '\n');
            break;
    }
}


/**
 * @brief Função principal do sistema de gestão de vacinas
 * 
 * @param argc número de argumentos da linha de comandos
 * @param argv array de argumentos da linha de comandos
 * 
 * @return int 0
 * 
 * @details Inicializa a estrutura do sistema de vacinas com valores padrão:
 *          - Listas vazias para vacinas, utentes e inoculações
 *          - Contadores a zero
 *          - Data atual definida para 1-1-2025
 * 
 *          Processa o idioma conforme o argumentos recebido e entra num loop
 *          que lê e processa comandos até encontrar EOF.
 * 
 * @note A data inicial é alterada através do comando 't'
 */
int main(int argc, char *argv[]) {
    char comando;
    char buffer[BUFMAX];

    SistemaVacinas sistema = {
        .vacinas_head = NULL,
        .utentes_head = NULL,
        .inoculacoes_head = NULL,
        .num_vacinas = 0,
        .num_inoculacoes = 0,
        .data_atual = {1, 1, 2025} 
    };

    seleciona_idioma(argc, argv);

    while (scanf(" %c", &comando) != EOF) {
        processa_comando_escolhido(comando, &sistema, buffer);
    }

    return 0;
}