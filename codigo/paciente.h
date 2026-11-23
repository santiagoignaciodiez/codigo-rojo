/*
	Algoritmos y Estructuras de Datos II
	Proyecto Integrador 2026 - Grupo 09
	Codigo Rojo: Guardia Hospitalaria
	TAD tPaciente - paciente.h
*/

/* Evita que se incluya dos veces: lo incluyen colaEspera.h, arbolHistorias.h y codigoRojo.c */
#ifndef PACIENTE_H
#define PACIENTE_H

#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define MAX 50
#define URGENTE 1
#define MEDIA 2
#define LEVE 3


/* ===== Tipo de dato ===== */

typedef char tString[MAX];

typedef struct{
	int dni;
	tString nombre;
	int edad;
	tString sintoma;
	int gravedad;
	int prioridad;
	int espera;
}tPaciente;


/* ===== Prototipos ===== */
tPaciente crearPaciente(int, tString, int, tString, int);
void asignarPrioridad(tPaciente *, int);
bool triageCorrecto(tPaciente);
char * nombrePrioridad(int);
void mostrarPaciente(tPaciente);


/* ===== Crear un paciente ===== */
tPaciente crearPaciente(int pDni, tString pNombre, int pEdad, tString pSintoma, int pGravedad){
	tPaciente paciente;

	paciente.dni = pDni;
	strcpy(paciente.nombre, pNombre);
	paciente.edad = pEdad;
	strcpy(paciente.sintoma, pSintoma);
	paciente.gravedad = pGravedad;
	paciente.prioridad = 0;
	paciente.espera = 0;

	return paciente;
}


/* ===== Asignar la prioridad (triage) ===== */
void asignarPrioridad(tPaciente * pPaciente, int pPrioridad){
	pPaciente->prioridad = pPrioridad;
}


/* ===== Saber si el triage fue correcto ===== */
/* Funcion propia: compara la prioridad que eligio el jugador con la gravedad real del sintoma */
bool triageCorrecto(tPaciente pPaciente){
	return (pPaciente.prioridad == pPaciente.gravedad);
}


/* ===== Nombre de una prioridad ===== */
/* Funcion propia: devuelve el texto de la prioridad (URGENTE, MEDIA o LEVE) para mostrarlo */
char * nombrePrioridad(int pPrioridad){
	switch(pPrioridad){
		case URGENTE:
			return "URGENTE";
		case MEDIA:
			return "MEDIA";
		case LEVE:
			return "LEVE";
		default:
			return "SIN ASIGNAR";
	}
}


/* ===== Mostrar un paciente ===== */
void mostrarPaciente(tPaciente pPaciente){
	printf("%-10d %-18s %-6d %-26s %-10s %d\n", pPaciente.dni, pPaciente.nombre, pPaciente.edad,
		pPaciente.sintoma, nombrePrioridad(pPaciente.prioridad), pPaciente.espera);
}

#endif
