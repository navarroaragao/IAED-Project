/**
 * @file main.h
 * @brief Ficheiro principal do sistema de gestão de vacinas
 * 
 * @author ist1113402
 * @note project - iaed25
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "processamento.h"


void processa_comando_escolhido(char comando, SistemaVacinas* sistema, char buffer[]);
void seleciona_idioma(int argc, char *argv[]);
int main(int argc, char *argv[]);

#endif 