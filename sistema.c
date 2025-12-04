/**
 * @file sistema.c
 * @brief Implementação do sistema de gestão de vacinas
 * 
 * @details Contém todas as funções de suporte para o sistema de vacinas,
 *          incluindo validações, manipulação de dados e gestão de memória.
 * 
 * @author ist1113402
 * @note project - iaed25
 */

#include "sistema.h"

/**
 * @brief Mensagens de erro em inglês e português
 */
const char* errorMessages[2][12] = {
    { /* MENSAGENS DE ERRO EM INGLÊS */

        "too many vaccines\n",           /* caso de exceda a capacidade de registo de vacinas do sistema */    
        "duplicate batch number\n",      /* caso já exista um lote com a mesma designação */   
        "invalid batch\n",               /* caso a designação do lote exceda o limite de digitos ou não sejam constituídos por dígitos hexadecimais maiúsculos */  
        "invalid name\n",                /* caso o nome exceda o limite de carateres ou contenha carateres inválidos */  
        "invalid date\n",                /* caso a data não represente um dia válido ou este dia já tenha sido ultrapassado pela data atual */  
        "invalid quantity\n",            /* caso de o número de doses não seja um número positivo */  
        "no such vaccine\n",             /* caso não exista uma vacina com o nome indicado */   
        "no stock\n",                    /* caso não exista uma dose da vacina indicada válida e disponível */   
        "already vaccinated\n",          /* caso o utente já tenha sido vacinado nesse mesmo dia para a mesma vacina */   
        "no such batch\n",               /* caso não exista um lote com a designação indicada */  
        "no such user\n",                /* caso de não existir nenhuma aplicação de vacina ao utente indicado */  
        "No memory\n"                    /* se a memória se esgotar, o programa termina de forma controlada, imprimindo esta mensagem */  
        "vaccine name cannot begin with a lowercase letter"
    },
    { /* MENSAGENS DE ERRO EM PORTUGUÊS - são as mesmas mensagens de cima, mas com o idioma português (NOTA: ordem pela qual aparecem abaixo é igual)*/

        "demasiadas vacinas\n",             
        "número de lote duplicado\n",       
        "lote inválido\n",                  
        "nome inválido\n",                  
        "data inválida\n",                  
        "quantidade inválida\n",           
        "vacina inexistente\n",             
        "esgotado\n",                      
        "já vacinado\n",                   
        "lote inexistente\n",              
        "utente inexistente\n",             
        "sem memória\n" 
        "vaccine name cannot begin with a lowercase letter"                    
    }
};



Data data_atual = {1, 1, 2025}; /* Data atual do sistema */
Idioma idioma = ENGLISH;        /* Idioma padrão do sistema */


/**
 * @brief Número de dias de cada mês
 * 
 * @param mes Mês (01 - 12)
 * @return Número de dias do mês
 */
int diasDosMeses(int mes) {
    char dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return dias[mes-1];
}


/**
 * @brief Verifica se uma data é válida
 * 
 * @param data Data a validar
 * @return 1 se data é válida, 0 caso contrário
 */
int dataValida(Data data) {
    if (data.ano < 2025) return 0;
    if (data.mes < 1 || data.mes > 12) return 0;
    if (data.dia < 1 || data.dia > diasDosMeses(data.mes)) return 0;
    return 1;
}


/**
 * @brief Verifica se o nome da vacina é válido
 * 
 * @param nome Nome a validar
 * @return 1 se nome da vacina é válido, 0 caso contrário
 */
int nomeValido(const char* nome) {
    if (!nome || nome[0] == '\0') return 0;
    if (strlen(nome) >= MAX_NOME_VACINA) return 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        if (isspace(nome[i])) return 0;
    }
    return 1;
}


/**
 * @brief Verifica se um lote é válido
 * 
 * @param lote Lote a validar
 * @return 1 se lote é válido, 0 caso contrário
 */
int loteValido(const char* lote) {
    if (strlen(lote) > MAX_LOTE) return 0;
    for (int i = 0; lote[i] != '\0'; i++) {
        if (!isxdigit(lote[i]) || (isalpha(lote[i]) && !isupper(lote[i]))) {
            return 0;
        }
    }
    return 1;
}


/**
 * @brief Compara duas datas
 * 
 * @param data1 Primeira data
 * @param data2 Segunda data
 * @return Diferença entre as datas
 */
int comparaDatas(Data data1, Data data2) {
    if (data1.ano != data2.ano) return data1.ano - data2.ano;
    if (data1.mes != data2.mes) return data1.mes - data2.mes;
    return data1.dia - data2.dia;
}


/**
 * @brief Verifica se uma data é anterior a outra
 * 
 * @param d1 Primeira data
 * @param d2 Segunda data
 * @return 1 se d1 < d2, 0 caso contrário
 */
int dataAnterior(Data d1, Data d2) {
    return comparaDatas(d1, d2) < 0;
}


/**
 * @brief Valida a designação de um lote e imprime erro se inválido
 * 
 * @param lote Lote a validar
 * @return 1 se válido, 0 caso contrário
 */
int valida_designacao_lote(const char* lote) {
    if (!loteValido(lote)) {
        imprime_erros(ERRO_INVALID_BATCH);
        return 0;
    }
    return 1;
}


/**
 * @brief Valida uma data e imprime erro se inválida
 * 
 * @param data Data a validar
 * @param data_atual Data atual do sistema
 * @return 1 se válida, 0 caso contrário
 */
int valida_Data_anterior(Data data, Data data_atual) {
    if (!dataValida(data) || dataAnterior(data, data_atual)) {
        imprime_erros(ERRO_INVALID_DATE);
        return 0;
    }
    return 1;
}


/**
 * @brief Valida um nome e imprime erro se inválido
 * 
 * @param nome Nome a validar
 * @return 1 se válido, 0 caso contrário
 */
int valida_nome(const char* nome) {
    if (!nomeValido(nome)) {
        imprime_erros(ERRO_INVALID_NAME);
        return 0;
    }
    return 1;
}


/**
 * @brief Valida o número de doses e imprime erro se houver quantidade inválida
 * 
 * @param doses Número de doses
 * @return 1 se válido, 0 caso contrário
 */
int valida_Doses(int doses) {
    if (doses <= 0) {
        imprime_erros(ERRO_INVALID_QUANTITY);
        return 0;
    }
    return 1;
}


/**
 * @brief Verifica se o sistema atingiu a capacidade máxima de vacinas
 * 
 * @param sistema Ponteiro para o sistema
 * @return 1 se há capacidade, 0 caso contrário
 */
int verifica_capacidade(SistemaVacinas* sistema) {
    if (sistema->num_vacinas >= MAX_VACINAS) {
        imprime_erros(ERRO_CAPACITY);
        return 0;
    }
    return 1;
}


/**
 * @brief Verifica se já existe um lote no sistema com a mesma designação
 * 
 * @param sistema Ponteiro para o sistema
 * @param lote lote a verificar
 * @return 1 se duplicado, 0 caso contrário
 */
int verifica_Lotes_duplicados(SistemaVacinas* sistema, const char* lote) {

    if (!sistema || !lote) {
        return 0;
    }
    
    VacinaListaItem* atual = sistema->vacinas_head;
    while (atual != NULL) {
        if (strcmp(atual->atual->lote, lote) == 0) {
            imprime_erros(ERRO_DUPLICATE_BATCH);
            return 1;
        }
        atual = atual->next;
    }
    return 0;
}


/**
 * @brief Valida uma data em relação à data atual
 * 
 * @param date Data a validar
 * @param current_date Data atual
 * @return 1 se válida, 0 caso contrário
 */
int valida_data_superior(Data date, Data data_atual) {
    return  dataValida(date) && !dataAnterior(data_atual, date);
}


/**
 * @brief Verifica se um lote existe no sistema
 * 
 * @param sistema Ponteiro para o sistema
 * @param lote Número do lote
 * @return 1 se existe, 0 caso contrário
 */
int valida_lote(SistemaVacinas* sistema, const char* lote) {
    
    VacinaListaItem* atual = sistema->vacinas_head;
    while (atual != NULL) {
        if (strcmp(atual->atual->lote, lote) == 0) {
            return 1;
        }
        atual = atual->next;
    }
    return 0;
}


/**
 * @brief Seleciona o idioma do sistema
 * 
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * 
 * @details Se o primeiro argumento for "pt", define o idioma como português.
 *          Caso contrário, mantém o inglês, que é o padrão.
 */
void seleciona_idioma(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "pt") == 0) {
        idioma = PORTUGUESE;
    }
}


/**
 * @brief Imprime uma mensagem de erro
 * 
 * @param code Código do erro (enumerador ERROS)
 */
void imprime_erros(ERROS code) {
    printf("%s", errorMessages[idioma][code]);
}


/**
 * @brief Imprime uma mensagem de erro com argumento
 * 
 * @param code Código do erro
 * @param arg Argumento a incluir na mensagem
 */
void imprimie_errosComArg(ERROS code, const char* arg) {
    printf("%s: %s", arg, errorMessages[idioma][code]);
}


/**
 * @brief Cria uma nova Vacina
 * 
 * @param lote Número do lote
 * @param data Data de validade
 * @param doses Número de doses
 * @param nome Nome da vacina
 * @return Ponteiro para a vacina criada ou NULL em caso de erro
 */
Vacina* cria_vacina(const char* lote, Data data, int doses, const char* nome) {
    if (!lote || !nome) {
        imprime_erros(ERRO_NO_MEMORY);
        return NULL;
    }

    Vacina* nova_vacina = malloc(sizeof(Vacina));
    if (!nova_vacina) {
        imprime_erros(ERRO_NO_MEMORY);
        return NULL;
    }

    strncpy(nova_vacina->lote, lote, MAX_LOTE);
    nova_vacina->lote[MAX_LOTE] = '\0';
    nova_vacina->data_validade = data;
    nova_vacina->doses = doses;
    nova_vacina->inoculadas = 0;

    strncpy(nova_vacina->nome_vacina, nome, MAX_NOME_VACINA);
    nova_vacina->nome_vacina[MAX_NOME_VACINA] = '\0';

    

    return nova_vacina;
}


/**
 * @brief Insere uma vacina na lista ordenada do sistema
 * 
 * @param sistema Ponteiro para o sistema
 * @param nova_vacina Vacina a inserir
 * 
 * @details A lista é mantida ordenada por:
 *          1. Data de validade (mais próxima primeiro)
 *          2. Número de lote (ordem alfabética)
 */
void insere_vacinas(SistemaVacinas* sistema, Vacina* nova_vacina) {
    VacinaListaItem* nova_lista_item = malloc(sizeof(VacinaListaItem));
    if (!nova_lista_item) {
        imprime_erros(ERRO_NO_MEMORY);
        return;
    }
    nova_lista_item->atual = nova_vacina;
    nova_lista_item->next = NULL;

    if (sistema->vacinas_head == NULL) {
        sistema->vacinas_head = nova_lista_item;
        return;
    }

    VacinaListaItem* atual = sistema->vacinas_head;
    VacinaListaItem* anterior = NULL;

    /* insere a vacina na posição correta para manter a lista ordenada */
    while (atual != NULL && 
           (comparaDatas(atual->atual->data_validade, nova_vacina->data_validade) < 0 ||
           (comparaDatas(atual->atual->data_validade, nova_vacina->data_validade) == 0 && 
            strcmp(atual->atual->lote, nova_vacina->lote) < 0))) {
        anterior = atual;
        atual = atual->next;
    }

    if (anterior == NULL) {
        nova_lista_item->next = sistema->vacinas_head;
        sistema->vacinas_head = nova_lista_item;
    } else {
        anterior->next = nova_lista_item;
        nova_lista_item->next = atual;
    }
}


/**
 * @brief Valida todos os dados de entrada para uma nova vacina
 * 
 * @param lote Número do lote a validar
 * @param data Data de validade a validar
 * @param doses Número de doses a validar
 * @param nome Nome a validar
 * @param sistema Ponteiro para o sistema de vacinas
 * @return 1 se todos os dados são válidos, 0 caso contrário
 * 
 * @note Caso algum dado seja inválido, a função imprime automaticamente
 *       a mensagem de erro correspondente.
 */

int validar_dados_entrada(const char* lote, Data data, int doses, const char* nome, SistemaVacinas* sistema) {

    return valida_designacao_lote(lote) && 
           valida_Data_anterior(data, sistema->data_atual) && 
           valida_nome(nome) && 
           valida_Doses(doses);
}


/**
 * @brief Cria um novo item da lista ligada de vacinas
 * 
 * @param vacina Ponteiro para a vacina a ser armazenada no item
 * @return Ponteiro para o novo item criado ou NULL em caso de erro
 * 
 * @details Aloca memória para um novo item da lista ligada de vacinas e
 *          inicializa-o com a vacina fornecida.
 * 
 */
VacinaListaItem* criar_item_lista_vacinas(Vacina* vacina) {
    VacinaListaItem* item = malloc(sizeof(VacinaListaItem));
    if (item) {
        item->atual = vacina;
        item->next = NULL;
    }
    return item;
}


/**
 * @brief Insere uma vacina no sistema de forma ordenada
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param novo_item Item que contem a vacina a inserir
 * 
 * @details Insere a vacina na lista mantendo a ordenação por:
 *          1. Data de validade (mais próxima primeiro)
 *          2. Número de lote (ordem alfabética crescente)
 * 
 *          A inserção é feita no local correto para manter a ordenação.
 */
void inserir_vacina_no_sistema(SistemaVacinas* sistema, VacinaListaItem* novo_item) {
    if (sistema->vacinas_head == NULL) {
        sistema->vacinas_head = novo_item;
        return;
    }

    VacinaListaItem* atual = sistema->vacinas_head;
    VacinaListaItem* anterior = NULL;
    Vacina* nova_vacina = novo_item->atual;

    while (atual != NULL && 
           (comparaDatas(atual->atual->data_validade, nova_vacina->data_validade) < 0 ||
           (comparaDatas(atual->atual->data_validade, nova_vacina->data_validade) == 0 && 
            strcmp(atual->atual->lote, nova_vacina->lote) < 0))) {
        anterior = atual;
        atual = atual->next;
    }

    if (anterior == NULL) {
        novo_item->next = sistema->vacinas_head;
        sistema->vacinas_head = novo_item;
    } else {
        anterior->next = novo_item;
        novo_item->next = atual;
    }
}


/**
 * @brief Imprime a informação de uma vacina no formato padrão
 * 
 * @param vacina Ponteiro para a vacina a ser impressa
 * 
 * @details O formato é:
 *          [nome] [lote] [dd-mm-aaaa] [doses] [inoculadas]
 */
void imprime_info_vacina(const Vacina* vacina) {
    printf("%s %s %02d-%02d-%04d %d %d\n", 
           vacina->nome_vacina,
           vacina->lote, 
           vacina->data_validade.dia, 
           vacina->data_validade.mes, 
           vacina->data_validade.ano, 
           vacina->doses, 
           vacina->inoculadas);
}


/**
 * @brief Lista todas as vacinas do sistema
 * 
 * @param head Ponteiro para o primeiro item da lista de vacinas
 * 
 * @details Percorre toda a lista ligada de vacinas e imprime a informação de cada uma
 *          que tenha doses disponíveis (doses >= 0).
 */
void lista_todas_vacinas(VacinaListaItem* head) {
    VacinaListaItem* atual = head;
    while (atual != NULL) {
        if (atual->atual->doses >= 0) {
            imprime_info_vacina(atual->atual);
        }
        atual = atual->next;
    }
}



/**
 * @brief Lista vacinas específicas de acordo com os nomes que são fornecidos
 * 
 * @param head Ponteiro para o primeiro item da lista de vacinas
 * @param argv Array de strings com os nomes das vacinas a listar
 * @param argc Número de nomes no array argv
 * 
 * @details Para cada nome no array argv, procura e imprime a primeira vacina
 *          correspondente que tenha doses disponíveis.
 *          Caso não encontre uma vacina para algum nome, imprime uma mensagem de erro.
 */
void lista_vacinas_especificas(VacinaListaItem* head, char* argv[], int argc) {
    for (int i = 0; i < argc; i++) {
        argv[i][strcspn(argv[i], "\n")] = '\0'; 
        int existe = 0;
        VacinaListaItem* atual = head;

        /* encontra a vacina e imprime as informações dela */
        while (atual != NULL) {
            if (strcmp(atual->atual->nome_vacina, argv[i]) == 0 && atual->atual->doses > 0) {
                imprime_info_vacina(atual->atual);
                existe = 1;
                break;
            }
            atual = atual->next;
        }

        if (!existe) {
            imprimie_errosComArg(ERRO_NO_SUCH_VACCINE, argv[i]);
        }
    }
}


/**
 * @brief Verifica se um utente já recebeu uma determinada vacina
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param utente Ponteiro para o utente a verificar
 * @param nome_vacina Nome da vacina a verificar
 * @return 1 se o utente já foi vacinado, 0 caso contrário
 * 
 * @details Verifica se o utente já recebeu a vacina especificada na data atual do sistema.
 */
int ja_vacinado(SistemaVacinas* sistema, Utente* utente, const char* nome_vacina) {
    InoculacaoListaItem* inoculacao_item = utente->inoculacao_head;
    while (inoculacao_item != NULL) {
        if (strcmp(inoculacao_item->atual->vacina->nome_vacina, nome_vacina) == 0 &&
            comparaDatas(inoculacao_item->atual->data_inoculacao, sistema->data_atual) == 0) {
            return 1;
        }
        inoculacao_item = inoculacao_item->next;
    }
    return 0;
}


/**
 * @brief Encontra a melhor opção de vacina disponível para administrar
 * 
 * @param head Ponteiro para o primeiro item da lista de vacinas
 * @param nome_vacina Nome da vacina desejada
 * @param data_atual Data atual do sistema
 * @return Ponteiro para a melhor vacina ou NULL se não encontrada
 * 
 * @details A melhor vacina é determinada por:
 *          1. Ter o nome correspondente
 *          2. Ter doses disponíveis
 *          3. Estar dentro do prazo de validade (data de validade >= data atual)
 *          4. Ter a data de validade mais próxima
 * 
 */
Vacina* procura_melhor_vacina(VacinaListaItem* head, const char* nome_vacina, Data data_atual) {
    Vacina* melhor_opcao = NULL;
    VacinaListaItem* atual = head;

    while (atual != NULL) {
        if (strcmp(atual->atual->nome_vacina, nome_vacina) == 0 &&
            atual->atual->doses > 0 &&
            comparaDatas(atual->atual->data_validade, data_atual) >= 0) {
            if (melhor_opcao == NULL || 
                comparaDatas(atual->atual->data_validade, melhor_opcao->data_validade) < 0 ||
                (comparaDatas(atual->atual->data_validade, melhor_opcao->data_validade) == 0 &&
                strcmp(atual->atual->lote, melhor_opcao->lote) < 0)) {
                melhor_opcao = atual->atual;
            }
        }
        atual = atual->next;
    }
    return melhor_opcao;
}


/**
 * @brief Cria um novo item para a lista de inoculações
 * 
 * @param inoculacao Estrutura com os dados da inoculação
 * @return Ponteiro para o novo item ou NULL em caso de erro
 * 
 * @details Aloca memória para um novo item e copia os dados da inoculação.
 *          A memória é alocada tanto para o item como para a cópia dos dados.
 */
InoculacaoListaItem* criar_item_lista_inoculacao(Inoculacao inoculacao) {
    InoculacaoListaItem* novo_item = malloc(sizeof(InoculacaoListaItem));
    if (!novo_item) {
        return NULL;
    }
    
    novo_item->atual = malloc(sizeof(Inoculacao));
    if (!novo_item->atual) {
        free(novo_item);
        return NULL;
    }
    
    *(novo_item->atual) = inoculacao;
    novo_item->next = NULL;
    return novo_item;
}


/**
 * @brief Adiciona uma inoculação à lista de um utente
 * 
 * @param utente Ponteiro para o utente
 * @param novo_item Item que contém a inoculação a adicionar
 * 
 * @details A inoculação é adicionada no final da lista de inoculações do utente.
 */
void adicionar_inoculacao_utente(Utente* utente, InoculacaoListaItem* novo_item) {
    if (utente->inoculacao_head == NULL) {
        utente->inoculacao_head = novo_item;
    } else {
        InoculacaoListaItem* atual = utente->inoculacao_head;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novo_item;
    }
}


/**
 * @brief Adiciona uma inoculação à lista do sistema
 * 
 * @param sistema Ponteiro para o sistema
 * @param novo_item Item que contém a inoculação a adicionar
 * 
 * @details A inoculação é adicionada no final da lista global de inoculações.
 *          O contador de inoculações do sistema é incrementado (num_inoculacoes).
 */
void adicionar_inoculacao_sistema(SistemaVacinas* sistema, InoculacaoListaItem* novo_item) {
    if (sistema->inoculacoes_head == NULL) {
        sistema->inoculacoes_head = novo_item;
    } else {
        InoculacaoListaItem* atual = sistema->inoculacoes_head;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novo_item;
    }
    sistema->num_inoculacoes++;
}


/**
 * @brief Atualiza os dados de uma vacina após inoculação
 * 
 * @param vacina Ponteiro para a vacina administrada
 * 
 * @details Decrementa o número de doses disponíveis e incrementa
 *          o contador de doses inoculadas.
 */
void atualizar_dados_vacina(Vacina* vacina) {
    vacina->doses--;
    vacina->inoculadas++;
}


/**
 * @brief Cria e regista uma nova inoculação no sistema
 * 
 * @param sistema Ponteiro para o sistema
 * @param utente Ponteiro para o utente vacinado
 * @param melhor_opcao Ponteiro para a vacina administrada
 * 
 * @details Esta função realiza todas as operações necessárias para registar uma nova inoculação:
 *          1. Cria a estrutura de inoculação
 *          2. Adiciona à lista do utente
 *          3. Adiciona à lista global do sistema
 *          4. Atualiza os contadores da vacina
 * 
 * @note A função trata de toda a gestão de memória necessária.
 */
void cria_nova_inoculacao(SistemaVacinas* sistema, Utente* utente, Vacina* melhor_opcao) {
    Inoculacao nova_Inoculacao;
    nova_Inoculacao.data_inoculacao = sistema->data_atual;
    nova_Inoculacao.vacina = melhor_opcao;
    nova_Inoculacao.utente = utente;

    /* cria inoculação na lista do utente */
    InoculacaoListaItem* item_utente = criar_item_lista_inoculacao(nova_Inoculacao);
    if (!item_utente) {
        imprime_erros(ERRO_NO_MEMORY);
        return;
    }

    adicionar_inoculacao_utente(utente, item_utente);

    /* cria inoculação na lista do sistema */
    InoculacaoListaItem* item_sistema = malloc(sizeof(InoculacaoListaItem));
    if (!item_sistema) {
        free(item_utente->atual);
        free(item_utente);
        imprime_erros(ERRO_NO_MEMORY);
        return;
    }
    
    item_sistema->atual = item_utente->atual;
    item_sistema->next = NULL;

    adicionar_inoculacao_sistema(sistema, item_sistema);

    /* atualiza dados da vacina */
    atualizar_dados_vacina(melhor_opcao);
}


/**
 * @brief Procura um utente no sistema pelo nome
 * 
 * @param sistema Ponteiro para o sistema de vacinas
 * @param nome_utente Nome do utente a procurar
 * @return Ponteiro para o utente encontrado ou NULL se não existir
 * 
 * @details Percorre a lista de utentes comparando os nomes.
 */
Utente* encontra_utente(SistemaVacinas* sistema, const char* nome_utente) {
    UtenteListaItem* atual = sistema->utentes_head;

    while (atual != NULL) {
        if (strcmp(atual->atual->nome_utente, nome_utente) == 0) {
            return atual->atual;
        }
        atual = atual->next;
    }
    return NULL;
}


/**
 * @brief Cria um novo utente
 * 
 * @param nome_utente Nome do novo utente
 * @return Ponteiro para o utente criado ou NULL em caso de erro
 * 
 * @details Aloca memória para um novo utente e inicializa:
 *          - Nome (com cópia segura da string)
 *          - Lista de inoculações vazia
 */
Utente* cria_utente(const char* nome_utente) {
    Utente* novo_utente = malloc(sizeof(Utente));
    if (!novo_utente) {
        imprime_erros(ERRO_NO_MEMORY);
        return NULL;
    }
    strncpy(novo_utente->nome_utente, nome_utente, BUFMAX);
    novo_utente->nome_utente[BUFMAX] = '\0';
    novo_utente->inoculacao_head = NULL;
    return novo_utente;
}


/**
 * @brief Insere o novo utente no sistema
 * 
 * @param sistema Ponteiro para o sistema
 * @param novo_utente Ponteiro para o utente a inserir
 * 
 * @details Adiciona o utente no final da lista de utentes.
 *          Mantém a lista como uma lista simplesmente ligada.
 */
void insere_utente(SistemaVacinas* sistema, Utente* novo_utente) {
    UtenteListaItem* nova_lista_item = malloc(sizeof(UtenteListaItem));
    if (!nova_lista_item) {
        imprime_erros(ERRO_NO_MEMORY);
        return;
    }
    nova_lista_item->atual = novo_utente;
    nova_lista_item->next = NULL;

    if (sistema->utentes_head == NULL) {
        sistema->utentes_head = nova_lista_item;
        return;
    }

    UtenteListaItem* atual = sistema->utentes_head;
    while (atual->next != NULL) {
        atual = atual->next;
    }
    atual->next = nova_lista_item;
}


/**
 * @brief Prepara a remoção de uma inoculação da lista de inoculações do sistema
 * 
 * @param sistema Ponteiro para o sistema
 * @param inoculacao Ponteiro para a inoculação a remover
 * @return Ponteiro para o item removido ou NULL se não encontrado
 * 
 * @note Esta função apenas remove da lista do sistema, não liberta memória
 */
InoculacaoListaItem* prepara_remocao_sistema(SistemaVacinas* sistema, Inoculacao* inoculacao){
    InoculacaoListaItem* atual = sistema->inoculacoes_head;
    InoculacaoListaItem* anterior = NULL;

    (void) anterior;

    while (atual != NULL) {
        if (atual->atual == inoculacao) {
            if (anterior == NULL) {
                sistema->inoculacoes_head = atual->next;
            } else {
                anterior->next = atual->next;
            }
            break;
        }
        anterior = atual;
        atual = atual->next;
    }
    return atual;
}


/**
 * @brief Remove inoculações que correspondam aos critérios especificados
 * 
 * @param sistema Ponteiro para o sistema
 * @param utente Ponteiro para o utente
 * @param data_vacinacao Data para filtrar (-1 para ignorar)
 * @param vacina Vacina para filtrar (NULL para ignorar)
 * @return Número de inoculações removidas
 * 
 * @details Remove inoculações que correspondam a todos os critérios não-nulos:
 *          - Utente especificado
 *          - Data 
 *          - Vacina (não NULL)
 * 
 * Liberta toda a memória associada às inoculações removidas.
 */
int remove_matching_inoculacao(SistemaVacinas* sistema, Utente* utente, Data data_vacinacao, Vacina* vacina) {
    int remove = 0;
    
    InoculacaoListaItem* atual = utente->inoculacao_head;
    InoculacaoListaItem* anterior = NULL;

    while (atual != NULL) {
        if ((data_vacinacao.dia == -1 || 
            comparaDatas(atual->atual->data_inoculacao, data_vacinacao) == 0) &&
            (vacina == NULL || atual->atual->vacina == vacina)) {
            
            /* remove inoculação da lista de inoculações do utente */
            InoculacaoListaItem* proximo_temp = atual->next;
            if (anterior == NULL) {
                utente->inoculacao_head = proximo_temp;
            } else {
                anterior->next = proximo_temp;
            }
            
            /* remove inoculação da lista de inoculações do sistema */
            InoculacaoListaItem* lista_inoculacao_sistema = prepara_remocao_sistema(sistema, atual->atual);
            
            /* liberta a memória das inoculações removidos */
            free(atual->atual);
            free(atual);
            free(lista_inoculacao_sistema);

            remove++;

            atual = proximo_temp;
        } else {
            anterior = atual;
            atual = atual->next;
        } 
    }
    
    /* Atualiza o número de inoculações do sistema */
    sistema->num_inoculacoes -= remove;
    return remove;
}


/**
 * @brief Encontra uma vacina pelo lote
 * 
 * @param sistema Ponteiro para o sistema
 * @param lote Número do lote a procurar
 * @return Ponteiro para a vacina encontrada ou NULL se não existir
 */
Vacina* encontra_vacina(SistemaVacinas* sistema, const char* lote) {
    VacinaListaItem* atual = sistema->vacinas_head;
    while (atual != NULL) {
        if (strcmp(atual->atual->lote, lote) == 0) {
            return atual->atual;
        }
        atual = atual->next;
    }
    return NULL;
}


/**
 * @brief Liberta toda a memória alocada pelo sistema
 * 
 * @param sistema Ponteiro para o sistema a ser libertado
 * 
 * @details Liberta toda a memória alocada, incluindo:
 *          1. Todas as inoculações (lista global e por utente)
 *          2. Todos os utentes
 *          3. Todas as vacinas e correspondentes lotes
 * 
 * @note Esta função é chamada antes de terminar o programa (comando q)
 */
void libertar_memoria(SistemaVacinas* sistema) {
    /* Liberta a memória da lista de inoculações do sistema*/
    InoculacaoListaItem* inoculacao_atual = sistema->inoculacoes_head;
    while (inoculacao_atual != NULL) {
        InoculacaoListaItem* inoculacao_temp = inoculacao_atual;
        inoculacao_atual = inoculacao_atual->next;
        free(inoculacao_temp->atual);
        free(inoculacao_temp);
    }

    /* Liberta a memória da lista de utentes do sistema*/
    UtenteListaItem* utente_atual = sistema->utentes_head;
    while (utente_atual != NULL) {
        UtenteListaItem* utente_temp = utente_atual;

        /* Liberta a memória da lista de inoculações dos utentes*/
        InoculacaoListaItem* inoculacao_atual = utente_temp->atual->inoculacao_head;
        while (inoculacao_atual != NULL) {
            InoculacaoListaItem* inoculacao_temp = inoculacao_atual;
            inoculacao_atual = inoculacao_atual->next;
            free(inoculacao_temp);
        }

        utente_atual = utente_atual->next;
        free(utente_temp->atual);
        free(utente_temp);
    }

    /* Liberta a memória da lista de vacinas do sistema*/
    VacinaListaItem* vacina_atual = sistema->vacinas_head;
    while (vacina_atual != NULL) {
        VacinaListaItem* vacina_temp = vacina_atual;
        vacina_atual = vacina_atual->next;
        free(vacina_temp->atual);
        free(vacina_temp);
    }
}