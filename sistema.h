/**
 * @file sistema.h
 * @brief Sistema de gestão de vacinas
 * 
 * @author ist1113402
 * @note project - iaed25
 */

#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*-------------------------------CONSTANTES ------------------------------*/

#define BUFMAX 65536          /* máximo de caracteres de uma instrução */
#define MAX_VACINAS 1000      /* máximo de vacinas no sistema */
#define MAX_NOME_VACINA 50    /* máximo de bytes do nome da vacina */
#define MAX_LOTE 20           /* máximo de digitos hexadecimais */
#define MAX_DATA 9            /* máximo de caracteres de uma data (dia-mes-ano) */ 


/*--------------------------- MENSAGENS DE ERRO --------------------------*/

typedef enum {
    ERRO_CAPACITY,
    ERRO_DUPLICATE_BATCH,
    ERRO_INVALID_BATCH,
    ERRO_INVALID_NAME,
    ERRO_INVALID_DATE,
    ERRO_INVALID_QUANTITY,
    ERRO_NO_SUCH_VACCINE,
    ERRO_NO_STOCK,
    ERRO_ALREADY_VACCINATED,
    ERRO_NO_SUCH_BATCH,
    ERRO_NO_SUCH_USER,
    ERRO_NO_MEMORY,
    ERRO_INVALID_NAME_LOWER
} ERROS;


/*------------------------------- ESTRUTURAS ------------------------------*/

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef enum {
    ENGLISH,
    PORTUGUESE
} Idioma;

typedef struct {
    char lote[MAX_LOTE + 1];
    Data data_validade;
    int doses;
    int inoculadas;
    char nome_vacina[MAX_NOME_VACINA + 1];
} Vacina;

typedef struct Inoculacao {
    Data data_inoculacao;
    Vacina* vacina;
    struct Utente* utente;
} Inoculacao;

typedef struct InoculacaoListaItem {
    Inoculacao* atual;
    struct InoculacaoListaItem* next;
} InoculacaoListaItem;

typedef struct Utente {
    char nome_utente[BUFMAX + 1];
    InoculacaoListaItem* inoculacao_head;
} Utente;

typedef struct UtenteListaItem {
    Utente* atual;
    struct UtenteListaItem* next;
} UtenteListaItem;

typedef struct VacinaListaItem {
    Vacina* atual;
    struct VacinaListaItem* next;
} VacinaListaItem;

typedef struct {
    Data data_atual;
    VacinaListaItem* vacinas_head;
    UtenteListaItem* utentes_head;
    InoculacaoListaItem* inoculacoes_head;
    int num_vacinas;
    int num_inoculacoes;
} SistemaVacinas;


/*------------------------------- FUNCOES ------------------------------*/

void seleciona_idioma(int argc, char *argv[]);
int diasDosMeses(int mes);
int dataValida(Data data);
int nomeValido(const char* nome);
int loteValido(const char* lote);
int comparaDatas(Data data1, Data data2);
int dataAnterior(Data d1, Data d2);
int valida_designacao_lote(const char* lote);
int valida_Data_anterior(Data data, Data current_date);
int valida_nome(const char* nome);
int valida_Doses(int doses);
int verifica_capacidade(SistemaVacinas* sistema);
int verifica_Lotes_duplicados(SistemaVacinas* sistema, const char* lote);
int valida_data_superior(Data date, Data current_date);
int valida_lote(SistemaVacinas* sistema, const char* lote);
int validar_dados_entrada(const char* lote, Data data, int doses, const char* nome, SistemaVacinas* sistema);


void imprime_erros(ERROS code);
void imprimie_errosComArg(ERROS code, const char* arg);

Vacina* cria_vacina(const char* lote, Data data, int doses, const char* nome);
void insere_vacinas(SistemaVacinas* sistema, Vacina* nova_vacina);
VacinaListaItem* criar_item_lista_vacinas(Vacina* vacina);
void inserir_vacina_no_sistema(SistemaVacinas* sistema, VacinaListaItem* novo_item);

void imprime_info_vacina(const Vacina* vacina);
void lista_todas_vacinas(VacinaListaItem* head);
void lista_vacinas_especificas(VacinaListaItem* head, char* argv[], int argc);
int ja_vacinado(SistemaVacinas* sistema, Utente* utente, const char* nome_vacina);
Vacina* procura_melhor_vacina(VacinaListaItem* head, const char* nome_vacina, Data data_atual);

InoculacaoListaItem* criar_item_lista_inoculacao(Inoculacao inoculacao);
void adicionar_inoculacao_utente(Utente* utente, InoculacaoListaItem* novo_item);
void adicionar_inoculacao_sistema(SistemaVacinas* sistema, InoculacaoListaItem* novo_item);
void atualizar_dados_vacina(Vacina* vacina);
void cria_nova_inoculacao(SistemaVacinas* sistema, Utente* utente, Vacina* melhor_opcao);


Utente* encontra_utente(SistemaVacinas* sistema, const char* nome_utente);
Utente* cria_utente(const char* nome_utente);
void insere_utente(SistemaVacinas* sistema, Utente* novo_utente);
int remove_matching_inoculacao(SistemaVacinas* sistema, Utente* utente, Data data_vacinacao, Vacina* vacina);
Vacina* encontra_vacina(SistemaVacinas* sistema, const char* lote);

void libertar_memoria(SistemaVacinas* sistema);

#endif 