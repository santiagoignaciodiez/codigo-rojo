/*
	Algoritmos y Estructuras de Datos II
	Proyecto Integrador 2026 - Grupo 09
	Codigo Rojo: Guardia Hospitalaria
	TAD tCola - colaEspera.h
*/

/* Evita que el archivo se incluya dos veces */
#ifndef COLAESPERA_H
#define COLAESPERA_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "paciente.h"


/* ===== Tipo de dato ===== */

typedef struct nodo{
	tPaciente paciente;
	struct nodo * siguiente;
}tNodo;

typedef struct{
	tNodo * principio;
	tNodo * final;
}tCola;


/* ===== Prototipos ===== */
void inicializarCola(tCola *);
bool colaVacia(tCola);
void encolar(tCola *, tPaciente);
void desencolar(tCola *);
tPaciente primerElemento(tCola);
void visualizarElementos(tCola);
int cantidadElementos(tCola);
bool estaEnCola(tCola, int);
int contarGravedad(tCola, int);
int aumentarEspera(tCola *, int);
void vaciarCola(tCola *);


/* ===== Inicializar la cola ===== */
void inicializarCola(tCola * pCola){
	pCola->principio = NULL;
	pCola->final = NULL;
}


/* ===== Cola vacia ===== */
bool colaVacia(tCola pCola){
	return (pCola.principio == NULL);
}


/* ===== Encolar (agregar al final) ===== */
void encolar(tCola * pCola, tPaciente pPaciente){
	tNodo * nuevoNodo;

	nuevoNodo = (tNodo *)malloc(sizeof(tNodo));
	if(nuevoNodo == NULL){
		printf("No hay memoria para registrar al paciente.\n");
	}else{
		nuevoNodo->paciente = pPaciente;
		nuevoNodo->siguiente = NULL;

		if(colaVacia(*pCola)){
			pCola->principio = nuevoNodo;
		}else{
			pCola->final->siguiente = nuevoNodo;
		}
		pCola->final = nuevoNodo;
	}
}


/* ===== Desencolar (sacar del principio) ===== */
void desencolar(tCola * pCola){
	tNodo * nodoSuprimir;

	if(colaVacia(*pCola)){
		printf("No hay pacientes en esta cola.\n");
	}else{
		nodoSuprimir = pCola->principio;
		pCola->principio = pCola->principio->siguiente;
		if(pCola->principio == NULL){
			pCola->final = NULL;
		}
		free(nodoSuprimir);
		nodoSuprimir = NULL;
	}
}


/* ===== Ver el primero sin sacarlo ===== */
tPaciente primerElemento(tCola pCola){
	return pCola.principio->paciente;
}


/* ===== Mostrar la cola ===== */
void visualizarElementos(tCola pCola){
	tNodo * nodoActual;

	nodoActual = pCola.principio;
	while(nodoActual != NULL){
		mostrarPaciente(nodoActual->paciente);
		nodoActual = nodoActual->siguiente;
	}
}


/* ===== Cantidad de pacientes en la cola ===== */
/* Funcion propia: recorre la cola y cuenta los pacientes */
int cantidadElementos(tCola pCola){
	tNodo * nodoActual;
	int cantidadPacientes = 0;

	nodoActual = pCola.principio;
	while(nodoActual != NULL){
		cantidadPacientes++;
		nodoActual = nodoActual->siguiente;
	}
	return cantidadPacientes;
}


/* ===== Buscar un DNI en la cola ===== */
/* Funcion propia: busqueda secuencial, recorre la cola comparando el DNI de cada paciente */
bool estaEnCola(tCola pCola, int pDni){
	tNodo * nodoActual;

	nodoActual = pCola.principio;
	while(nodoActual != NULL){
		if(nodoActual->paciente.dni == pDni){
			return true;
		}
		nodoActual = nodoActual->siguiente;
	}
	return false;
}


/* ===== Contar pacientes de una gravedad ===== */
/* Funcion propia: recorre la cola y cuenta los pacientes que tienen la gravedad real recibida */
int contarGravedad(tCola pCola, int pGravedad){
	tNodo * nodoActual;
	int cantidadPacientes = 0;

	nodoActual = pCola.principio;
	while(nodoActual != NULL){
		if(nodoActual->paciente.gravedad == pGravedad){
			cantidadPacientes++;
		}
		nodoActual = nodoActual->siguiente;
	}
	return cantidadPacientes;
}


/* ===== Sumar un turno de espera a todos ===== */
/* Funcion propia: suma un turno de espera a cada paciente y devuelve cuantos urgentes pasaron el limite */
int aumentarEspera(tCola * pCola, int pLimite){
	tNodo * nodoActual;
	int demorados = 0;

	nodoActual = pCola->principio;
	while(nodoActual != NULL){
		nodoActual->paciente.espera++;
		if(nodoActual->paciente.gravedad == URGENTE && nodoActual->paciente.espera > pLimite){
			demorados++;
		}
		nodoActual = nodoActual->siguiente;
	}
	return demorados;
}


/* ===== Vaciar la cola ===== */
/* Funcion propia: desencola hasta que la cola queda vacia, asi se libera la memoria de todos los nodos */
void vaciarCola(tCola * pCola){
	while(!colaVacia(*pCola)){
		desencolar(pCola);
	}
}

#endif
