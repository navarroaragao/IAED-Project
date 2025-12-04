/**
 * @file comandos.c
 * @brief Implementação dos comandos do sistema de gestão de vacinas
 * 
 * @details Este ficheiro contém a implementação das funções responsáveis pelo
 *          processamento dos comandos do sistema de vacinas. Cada comando é
 *          tratado de forma a garantir a correta execução das operações
 *          associadas ao sistema.
 * 
 * @author ist1113402
 * @note project - iaed25
 */

#include "comandos.h"

/*------------------------------- COMANDOS ------------------------------*/

/**
 * @brief Adiciona uma nova vacina ao sistema
 * 
 * @param lote string que representa o lote da vacina
 * @param data data de validade da vacina (estrutura Data)
 * @param doses número de doses disponíveis da vacina (inteiro)
 * @param nome string que representa o nome da vacina
 * @param sistema ponteiro para o sistema de vacinas (estrutura SistemaVacinas)
 *
 * @details A função verifica se os dados de entrada são válidos, se há capacidade no sistema
 *          e se o lote já existe. Se tudo estiver correto, cria uma nova vacina e a insere
 *          no sistema. Se houver algum erro, imprime a mensagem de erro correspondente.
 *          Se a vacina for criada com sucesso, imprime o lote da vacina.
 */
void comando_c(const char* lote, Data data, int doses, const char* nome, SistemaVacinas* sistema) {

    
    if (!validar_dados_entrada(lote, data, doses, nome, sistema)) {
        return;
    }

    if (nome[0] < 'A' || nome[0] > 'Z') {
        imprime_erros(ERRO_INVALID_NAME_LOWER);
        return;
    }
    
    if (!verifica_capacidade(sistema)) {
        return;
    }
    
    if (verifica_Lotes_duplicados(sistema, lote)) {
        return;
    }
    
    Vacina* nova_vacina = cria_vacina(lote, data, doses, nome);
    if (!nova_vacina) {
        imprime_erros(ERRO_NO_MEMORY);
        return;
    }
    
    VacinaListaItem* novo_item_lista = criar_item_lista_vacinas(nova_vacina); 
    if (!novo_item_lista) {
        free(nova_vacina);
        imprime_erros(ERRO_NO_MEMORY);
        return;
    }
    
    inserir_vacina_no_sistema(sistema, novo_item_lista);
    sistema->num_vacinas++;
    printf("%s\n", lote);
}


/**
 * @brief lista vacinas do sistema.
 * 
 * @param sistema ponteiro para o sistema de vacinas
 * @param argc número de argumentos (nomes de vacinas a filtrar)
 * @param argv array de strings com nomes de vacinas a filtrar
 * 
 * @details Se não forem fornecidos argumentos (argc=0), lista todas as vacinas.
 *          Caso contrário, lista apenas as vacinas cujos fabricantes estão em argv.
 */
void comando_l(SistemaVacinas* sistema, int argc, char *argv[]) {
    if (argc == 0) {
        lista_todas_vacinas(sistema->vacinas_head);
    } else {
        lista_vacinas_especificas(sistema->vacinas_head, argv, argc);
    }
}


/**
 * @brief altera ou consulta a data atual do sistema.
 * 
 * @param sistema ponteiro para o sistema de vacinas
 * @param dia dia da nova data 
 * @param mes mês da nova data 
 * @param ano ano da nova data
 * 
 * @details Se dia, mes e ano forem -1, imprime a data atual do sistema.
 *          Caso contrário, valida a nova data e atualiza o sistema se válida.
 *          Imprime a nova data ou mensagem de erro se aplicável.
 */
void comando_t(SistemaVacinas* sistema, int dia, int mes, int ano) {
    Data data_nova = {dia, mes, ano};

    if (dia == -1 && mes == -1 && ano == -1) {
        printf("%02d-%02d-%04d\n", sistema->data_atual.dia, 
                                  sistema->data_atual.mes, 
                                  sistema->data_atual.ano);
        return;
    } 

    if (!dataValida(data_nova)) {
        imprime_erros(ERRO_INVALID_DATE);
        return;
    }

    if (comparaDatas(data_nova, sistema->data_atual) < 0) {
        imprime_erros(ERRO_INVALID_DATE);
        return;
    }

    sistema->data_atual = data_nova;
    printf("%02d-%02d-%04d\n", sistema->data_atual.dia, 
                              sistema->data_atual.mes, 
                              sistema->data_atual.ano);
}


/**
 * @brief Adiciona uma inoculação a um utente.
 * 
 * @param nome_utente string que representa o nome do utente a vacinar
 * @param nome_vacina string que representa o nome da vacina a administrar
 * @param sistema ponteiro para o sistema de vacinas
 * 
 * @details Cria um novo utente se não existir. Verifica se o utente já recebeu a vacina,
 *          encontra a melhor vacina disponível (data de validade mais proxima da data atual) e regista a inoculação.
 *          Imprime o lote utilizado ou mensagem de erro se aplicável
 */
void comando_a(const char* nome_utente, const char* nome_vacina, SistemaVacinas* sistema) {

    Utente* utente = encontra_utente(sistema, nome_utente);
    if (utente == NULL) {
        utente = cria_utente(nome_utente);
        insere_utente(sistema, utente);
    }


    if (ja_vacinado(sistema, utente, nome_vacina)) {
        imprime_erros(ERRO_ALREADY_VACCINATED);
        return;
    }

    Vacina* melhor_opcao = procura_melhor_vacina(sistema->vacinas_head,
                                               nome_vacina,
                                               sistema->data_atual);
    if (melhor_opcao == NULL) {
        imprime_erros(ERRO_NO_STOCK);
        return;
    }

    cria_nova_inoculacao(sistema, utente, melhor_opcao);
    printf("%s\n", melhor_opcao->lote);
}


/**
 * @brief Lista inoculações de utente(s).
 * 
 * @param sistema ponteiro para o sistema de vacinas
 * @param nome_utente nome do utente a listar (NULL para listar todos)
 * 
 * @details Se nome_utente for NULL, lista todas as inoculações do sistema.
 *          Caso contrário, lista apenas as inoculações do utente especificado.
 *          Imprime mensagem de erro se o utente não for encontrado.
 */
void comando_u(SistemaVacinas* sistema, const char* nome_utente) {
    int encontrou = 0;

    InoculacaoListaItem* atual = sistema->inoculacoes_head;
    while (atual != NULL) {
        if (nome_utente == NULL || strcmp(atual->atual->utente->nome_utente, nome_utente) == 0) {
            printf("%s %s %02d-%02d-%04d\n", 
                atual->atual->utente->nome_utente,
                atual->atual->vacina->lote,
                atual->atual->data_inoculacao.dia,
                atual->atual->data_inoculacao.mes,
                atual->atual->data_inoculacao.ano);
            encontrou = 1;
        }
        atual = atual->next;
    }

    if (!encontrou && nome_utente != NULL) {
        imprimie_errosComArg(ERRO_NO_SUCH_USER, nome_utente);
    }
}


/**
 * @brief Remove um lote de vacinas do sistema.
 * 
 * @param lote string que representa o lote a remover
 * @param sistema ponteiro para o sistema de vacinas
 * 
 * @details Remove completamente o lote se não tiver doses inoculadas.
 *          Se tiver doses inoculadas, define as doses disponíveis para 0.
 *          Imprime o número de doses inoculadas ou mensagem de erro se o lote não existir.
 */
void comando_r(const char* lote, SistemaVacinas* sistema) {
    VacinaListaItem* anterior = NULL;
    VacinaListaItem* atual = sistema->vacinas_head;
    (void) anterior;

    while (atual != NULL) {
        if (strcmp(atual->atual->lote, lote) == 0) {
            printf("%d\n", atual->atual->inoculadas);

            if (atual->atual->inoculadas == 0) {
                atual->atual->doses = 0;
            } else {
                atual->atual->doses = 0;
            }
            return;
        }
        anterior = atual;
        atual = atual->next;
    }

    imprimie_errosComArg(ERRO_NO_SUCH_BATCH, lote);
}


/**
 * @brief Remove inoculações do sistema.
 * 
 * @param nome_utente string que representa o nome do utente
 * @param data_vacinacao data da vacinação para filtrar (-1 ignora)
 * @param sistema ponteiro para o sistema de vacinas
 * @param lote número do lote para filtrar (string vazia ignora)
 * 
 * @details Remove inoculações que correspondam aos critérios:
 *          - Utente especifico;
 *          - Data;
 *          - Lote;
 *          Imprime o número de inoculações removidas ou mensagens de erro apropriadas.
 */
void comando_d(const char* nome_utente, Data data_vacinacao, 
              SistemaVacinas* sistema, const char* lote) {
            
        
    Utente* utente = encontra_utente(sistema, nome_utente);
    if (utente == NULL) {
        imprimie_errosComArg(ERRO_NO_SUCH_USER, nome_utente);
        return;
    }

    if ( data_vacinacao.dia != -1 && !valida_data_superior(data_vacinacao, sistema->data_atual) ) {
        imprime_erros(ERRO_INVALID_DATE);
        return;
    }

    Vacina* vacina = NULL;

    if (lote[0] != '\0') {
        vacina = encontra_vacina(sistema, lote);
        if (vacina == NULL) {
            imprimie_errosComArg(ERRO_NO_SUCH_BATCH, lote);
            return;
        }
    }

    int removed = remove_matching_inoculacao(sistema, utente, data_vacinacao, vacina);
    printf("%d\n", removed);
}


void comando_v(const char* lote, Data nova_data, SistemaVacinas* sistema) {
    VacinaListaItem* atual = sistema->vacinas_head;

    valida_Data_anterior(nova_data, sistema->data_atual);

    while (atual != NULL) {
        if (strcmp(atual->atual->lote, lote) == 0) {
            if (!dataValida(nova_data) || comparaDatas(nova_data, sistema->data_atual) < 0) {
                imprime_erros(ERRO_INVALID_DATE);
                return;
            }
            atual->atual->data_validade = nova_data;
            printf("%s %02d-%02d-%04d\n", lote, nova_data.dia, nova_data.mes, nova_data.ano);
            return;
        }
        atual = atual->next;
    }

    imprimie_errosComArg(ERRO_NO_SUCH_BATCH, lote);
}