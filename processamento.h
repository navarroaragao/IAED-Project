/**
 * @file processamento.h
 * @brief Funções de processamento de comandos do sistema de vacinas
 * 
 * @author ist1113402
 * @note project - iaed25
 */

#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include "sistema.h"
#include "comandos.h"


void process_comando_c(SistemaVacinas* sistema, char* buffer);
void process_comando_l(SistemaVacinas* sistema, char* buffer);
void process_comando_t(SistemaVacinas* sistema);
void process_comando_a(SistemaVacinas* sistema, char* buffer);
void process_comando_u(SistemaVacinas* sistema, char* buffer);
void process_comando_r(SistemaVacinas* sistema, char* buffer);
void process_comando_d(SistemaVacinas* sistema, char* buffer);
void process_comando_v(SistemaVacinas* sistema, char* buffer);

#endif 