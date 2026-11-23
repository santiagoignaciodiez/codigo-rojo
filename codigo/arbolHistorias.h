/*
	Algoritmos y Estructuras de Datos II
	Proyecto Integrador 2026 - Grupo 09
	Codigo Rojo: Guardia Hospitalaria
	TAD tArbol (historias clinicas) - arbolHistorias.h
*/

/* Evita que el archivo se incluya dos veces */
#ifndef ARBOLHISTORIAS_H
#define ARBOLHISTORIAS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "paciente.h"

#define ARCHIVO_HISTORIAS "historias.dat"


/* ===== Tipo de dato ===== */

typedef struct{
	int dni;
	tString nombre;
	int edad;
	int cantidadAtenciones;
	tString ultimoSintoma;
	tString ultimoResultado;
}tHistoria;

typedef struct nodoArbol{
	tHistoria historia;
	struct nodoArbol * hijoIzdo;
	struct nodoArbol * hijoDcho;
}tArbol;


/* ===== Prototipos ===== */
void inicializarArbol(tArbol **);
bool arbolVacio(tArbol *);
void insertar(tArbol **, tHistoria);
tArbol * buscar(tArbol *, int);
tArbol * minimoElemento(tArbol *);
void eliminarNodo(tArbol **, int);
void registrarAtencion(tArbol **, tPaciente, tString);
void mostrarHistoria(tHistoria);
void inOrden(tArbol *);
int contarNodos(tArbol *);
void guardarPreOrden(tArbol *, FILE *);
void guardarHistorias(tArbol *);
void cargarHistorias(tArbol **);
void liberarArbol(tArbol **);


/* ===== Inicializar el arbol ===== */
void inicializarArbol(tArbol ** pArbol){
	*pArbol = NULL;
}


/* ===== Arbol vacio ===== */
bool arbolVacio(tArbol * pArbol){
	return (pArbol == NULL);
}


/* ===== Insertar una historia (recursiva) ===== */
void insertar(tArbol ** pArbol, tHistoria pHistoria){
	tArbol * nuevoNodo;

	if(*pArbol == NULL){
		nuevoNodo = (tArbol *)malloc(sizeof(tArbol));
		if(nuevoNodo != NULL){
			nuevoNodo->historia = pHistoria;
			nuevoNodo->hijoIzdo = NULL;
			nuevoNodo->hijoDcho = NULL;
			*pArbol = nuevoNodo;
		}else{
			printf("No hay memoria para la historia clinica.\n");
		}
	}else if(pHistoria.dni < (*pArbol)->historia.dni){
		insertar(&((*pArbol)->hijoIzdo), pHistoria);
	}else if(pHistoria.dni > (*pArbol)->historia.dni){
		insertar(&((*pArbol)->hijoDcho), pHistoria);
	}
}


/* ===== Buscar una historia por DNI (recursiva) ===== */
tArbol * buscar(tArbol * pArbol, int pDni){
	if(pArbol == NULL){
		return NULL;
	}else if(pArbol->historia.dni == pDni){
		return pArbol;
	}else if(pDni < pArbol->historia.dni){
		return buscar(pArbol->hijoIzdo, pDni);
	}else{
		return buscar(pArbol->hijoDcho, pDni);
	}
}


/* ===== Minimo elemento (recursiva) ===== */
tArbol * minimoElemento(tArbol * pArbol){
	if(arbolVacio(pArbol)){
		return NULL;
	}else{
		if(pArbol->hijoIzdo != NULL){
			return minimoElemento(pArbol->hijoIzdo);
		}else{
			return pArbol;
		}
	}
}


/* ===== Eliminar una historia por DNI (recursiva) ===== */
void eliminarNodo(tArbol ** pArbol, int pDni){
	tArbol * nodoSuprimir;
	tArbol * nodoMinimo;

	if(arbolVacio(*pArbol)){
		printf("No existe una historia clinica con DNI %d\n", pDni);
	}else{
		if(pDni < (*pArbol)->historia.dni){
			eliminarNodo(&(*pArbol)->hijoIzdo, pDni);
		}else{
			if(pDni > (*pArbol)->historia.dni){
				eliminarNodo(&(*pArbol)->hijoDcho, pDni);
			}else{
				if((*pArbol)->hijoIzdo == NULL){
					nodoSuprimir = *pArbol;
					*pArbol = (*pArbol)->hijoDcho;
					free(nodoSuprimir);
					printf("Historia clinica eliminada.\n");
				}else{
					if((*pArbol)->hijoDcho == NULL){
						nodoSuprimir = *pArbol;
						*pArbol = (*pArbol)->hijoIzdo;
						free(nodoSuprimir);
						printf("Historia clinica eliminada.\n");
					}else{
						nodoMinimo = minimoElemento((*pArbol)->hijoDcho);
						(*pArbol)->historia = nodoMinimo->historia;
						eliminarNodo(&(*pArbol)->hijoDcho, (*pArbol)->historia.dni);
					}
				}
			}
		}
	}
}


/* ===== Registrar una atencion en la historia ===== */
/* Funcion propia: si el DNI ya tiene historia la actualiza, si no crea una nueva y la inserta en el arbol */
void registrarAtencion(tArbol ** pArbol, tPaciente pPaciente, tString pResultado){
	tArbol * nodoEncontrado;
	tHistoria historia;

	nodoEncontrado = buscar(*pArbol, pPaciente.dni);
	if(nodoEncontrado != NULL){
		nodoEncontrado->historia.cantidadAtenciones++;
		nodoEncontrado->historia.edad = pPaciente.edad;
		strcpy(nodoEncontrado->historia.ultimoSintoma, pPaciente.sintoma);
		strcpy(nodoEncontrado->historia.ultimoResultado, pResultado);
	}else{
		historia.dni = pPaciente.dni;
		strcpy(historia.nombre, pPaciente.nombre);
		historia.edad = pPaciente.edad;
		historia.cantidadAtenciones = 1;
		strcpy(historia.ultimoSintoma, pPaciente.sintoma);
		strcpy(historia.ultimoResultado, pResultado);
		insertar(pArbol, historia);
	}
}


/* ===== Mostrar una historia ===== */
void mostrarHistoria(tHistoria pHistoria){
	printf("%-10d %-18s %-6d %-11d %s -> %s\n", pHistoria.dni, pHistoria.nombre, pHistoria.edad,
		pHistoria.cantidadAtenciones, pHistoria.ultimoSintoma, pHistoria.ultimoResultado);
}


/* ===== Listar ordenado por DNI: inorden (recursiva) ===== */
void inOrden(tArbol * pArbol){
	if(!arbolVacio(pArbol)){
		inOrden(pArbol->hijoIzdo);
		mostrarHistoria(pArbol->historia);
		inOrden(pArbol->hijoDcho);
	}
}


/* ===== Contar nodos (recursiva) ===== */
/* Funcion propia: devuelve 1 (el nodo actual) mas los nodos del subarbol izquierdo y los del derecho */
int contarNodos(tArbol * pArbol){
	if(arbolVacio(pArbol)){
		return 0;
	}else{
		return 1 + contarNodos(pArbol->hijoIzdo) + contarNodos(pArbol->hijoDcho);
	}
}


/* ===== Guardar en el archivo: preorden (recursiva) ===== */
/* Funcion propia: recorre en preorden y escribe cada historia con fwrite, asi al cargarlo el arbol queda con la misma forma */
void guardarPreOrden(tArbol * pArbol, FILE * pArchivo){
	if(!arbolVacio(pArbol)){
		fwrite(&pArbol->historia, sizeof(tHistoria), 1, pArchivo);
		guardarPreOrden(pArbol->hijoIzdo, pArchivo);
		guardarPreOrden(pArbol->hijoDcho, pArchivo);
	}
}


/* ===== Guardar las historias (archivo binario) ===== */
/* Funcion propia: abre historias.dat en modo "wb" y guarda todo el arbol */
void guardarHistorias(tArbol * pArbol){
	FILE * archivo;

	archivo = fopen(ARCHIVO_HISTORIAS, "wb");
	if(archivo == NULL){
		printf("No se pudo guardar el archivo de historias clinicas.\n");
	}else{
		guardarPreOrden(pArbol, archivo);
		fclose(archivo);
	}
}


/* ===== Cargar las historias (archivo binario) ===== */
/* Funcion propia: lee historias.dat con fread e inserta cada historia en el arbol */
void cargarHistorias(tArbol ** pArbol){
	FILE * archivo;
	tHistoria historia;

	archivo = fopen(ARCHIVO_HISTORIAS, "rb");
	if(archivo != NULL){
		while(fread(&historia, sizeof(tHistoria), 1, archivo) == 1){
			insertar(pArbol, historia);
		}
		fclose(archivo);
	}
}


/* ===== Liberar el arbol: postorden (recursiva) ===== */
/* Funcion propia: recorre en postorden y libera primero los hijos y despues el nodo */
void liberarArbol(tArbol ** pArbol){
	if(!arbolVacio(*pArbol)){
		liberarArbol(&(*pArbol)->hijoIzdo);
		liberarArbol(&(*pArbol)->hijoDcho);
		free(*pArbol);
		*pArbol = NULL;
	}
}

#endif
