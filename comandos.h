/**
 * @file comandos.h
 * @brief Comandos do sistema de gestão de vacinas
 * 
 * @author ist1113402
 * @note project - iaed25
 */

#ifndef COMANDOS_H
#define COMANDOS_H

#include "sistema.h"


void comando_c(const char* lote, Data data, int doses, const char* nome, SistemaVacinas* sistema);
void comando_l(SistemaVacinas* sistema, int argc, char *argv[]);
void comando_t(SistemaVacinas* sistema, int dia, int mes, int ano);
void comando_a(const char* nome_utente, const char* nome_vacina, SistemaVacinas* sistema);
void comando_u(SistemaVacinas* sistema, const char* nome_utente);
void comando_r(const char* lote, SistemaVacinas* sistema);
void comando_d(const char* nome_utente, Data data_vacinacao, SistemaVacinas* sistema, const char* lote);

#endif 