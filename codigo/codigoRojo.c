/*
	Algoritmos y Estructuras de Datos II
	Proyecto Integrador 2026 - Grupo 09
	Codigo Rojo: Guardia Hospitalaria
	Programa principal - codigoRojo.c
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include "paciente.h"
#include "colaEspera.h"
#include "arbolHistorias.h"

#define CANT_PRIORIDADES 3
#define TURNOS_LLEGADA 8
#define LIMITE_ESPERA 2
#define CANT_PERSONAS 15
#define CANT_SINTOMAS 12
#define ARCHIVO_ATENDIDOS "atendidos.txt"
#define ARCHIVO_PUNTAJES "puntajes.txt"

#define PUNTOS_TRIAGE_BIEN 10
#define PUNTOS_TRIAGE_MAL -5
#define PUNTOS_ATENCION 5
#define PUNTOS_URGENTE_ESPERANDO -10
#define PUNTOS_DEMORA -5


/* ===== Datos del juego ===== */

typedef struct{
	int dni;
	tString nombre;
	int edad;
}tPersona;

typedef struct{
	tString descripcion;
	int gravedad;
}tSintoma;

tPersona personas[CANT_PERSONAS] = {
	{30125478, "Ana Gomez", 34}, {41258963, "Juan Perez", 22}, {25874123, "Rosa Benitez", 61},
	{38521479, "Carlos Romero", 40}, {44123698, "Lucia Fernandez", 19}, {20147852, "Hector Ayala", 72},
	{35698741, "Marta Sosa", 45}, {42587413, "Diego Acosta", 27}, {28963214, "Silvia Ramirez", 53},
	{46325874, "Tomas Duarte", 15}, {33698521, "Paula Medina", 38}, {22587469, "Raul Gimenez", 66},
	{39852147, "Laura Vera", 31}, {47123589, "Mateo Ojeda", 12}, {31478526, "Carmen Ruiz", 49}
};

tSintoma sintomas[CANT_SINTOMAS] = {
	{"Dolor fuerte de pecho", URGENTE}, {"Dificultad para respirar", URGENTE},
	{"Hemorragia abundante", URGENTE}, {"Perdida de conocimiento", URGENTE},
	{"Fractura de brazo", MEDIA}, {"Fiebre muy alta", MEDIA},
	{"Quemadura en la mano", MEDIA}, {"Dolor abdominal", MEDIA},
	{"Dolor de cabeza", LEVE}, {"Tos y resfrio", LEVE},
	{"Torcedura de tobillo", LEVE}, {"Control de rutina", LEVE}
};


/* ===== Prototipos ===== */
void mostrarReglas();
void limpiarBuffer();
int leerEntero(int, int);
void leerNombre(tString);
int menu();
void inicializarSala(tCola[]);
bool salaVacia(tCola[]);
bool pacienteEnSala(tCola[], int);
int pacientesEnSala(tCola[]);
void mostrarSala(tCola[]);
void llegadaPacientes(tCola[], tArbol *, int *, int *);
void atenderPaciente(tCola[], tArbol **, int *, int *);
int urgentesEsperando(tCola[]);
int controlarEspera(tCola[]);
void elegirResultado(int, tString);
void registrarAtendido(tPaciente, tString);
void buscarHistoria(tArbol *);
void eliminarHistoria(tArbol **);
void guardarPuntaje(tString, int, int);
void mostrarMejorPuntaje();


/* ===== Programa principal ===== */
int main(){
	tCola sala[CANT_PRIORIDADES];
	tArbol * historias;
	tString jugador;
	int puntaje = 0;
	int llegados = 0;
	int atendidos = 0;
	int turno = 1;
	int opcion = -1;
	int demorados;
	int eficiencia;
	int i;
	bool finTurno;

	srand(time(NULL));
	inicializarSala(sala);
	inicializarArbol(&historias);
	cargarHistorias(&historias);

	mostrarReglas();
	printf("Historias clinicas cargadas: %d\n", contarNodos(historias));
	mostrarMejorPuntaje();
	printf("\nIngrese su nombre: ");
	leerNombre(jugador);

	do{
		printf("\n==================== TURNO %d ====================\n", turno);
		if(turno <= TURNOS_LLEGADA){
			llegadaPacientes(sala, historias, &puntaje, &llegados);
		}else{
			printf("Ya no llegan pacientes. Atienda a los que quedan en la sala.\n");
		}

		finTurno = false;
		do{
			printf("\nPuntaje: %d | Pacientes en sala: %d\n", puntaje, pacientesEnSala(sala));
			opcion = menu();

			switch(opcion){
				case 1:
					if(salaVacia(sala)){
						printf("\nNo hay pacientes esperando.\n");
					}else{
						atenderPaciente(sala, &historias, &puntaje, &atendidos);
						finTurno = true;
					}
					break;

				case 2:
					mostrarSala(sala);
					break;

				case 3:
					buscarHistoria(historias);
					break;

				case 4:
					if(arbolVacio(historias)){
						printf("\nTodavia no hay historias clinicas.\n");
					}else{
						printf("\n%-10s %-18s %-6s %-11s %s\n", "DNI", "Nombre", "Edad", "Atenciones", "Ultima atencion");
						inOrden(historias);
					}
					break;

				case 5:
					eliminarHistoria(&historias);
					break;

				case 6:
					finTurno = true;
					break;

				case 0:
					printf("\nTerminando la guardia...\n");
					break;
			}
		}while(!finTurno && opcion != 0);

		if(finTurno){
			demorados = controlarEspera(sala);
			if(demorados > 0){
				printf("\nAtencion! %d paciente/s urgente/s esperan hace mas de %d turnos: %d puntos\n",
					demorados, LIMITE_ESPERA, demorados * PUNTOS_DEMORA);
				puntaje = puntaje + demorados * PUNTOS_DEMORA;
			}
			turno++;
		}
	}while(opcion != 0 && (turno <= TURNOS_LLEGADA || !salaVacia(sala)));

	if(llegados > 0){
		eficiencia = puntaje * 100 / (llegados * (PUNTOS_TRIAGE_BIEN + PUNTOS_ATENCION));
	}else{
		eficiencia = 0;
	}
	if(eficiencia < 0){
		eficiencia = 0;
	}

	printf("\n==================== FIN DE LA GUARDIA ====================\n");
	printf("Jugador: %s\n", jugador);
	printf("Pacientes llegados: %d | Atendidos: %d | Sin atender: %d\n", llegados, atendidos, pacientesEnSala(sala));
	printf("Puntaje final: %d | Eficiencia: %d%%\n", puntaje, eficiencia);

	guardarPuntaje(jugador, puntaje, eficiencia);
	guardarHistorias(historias);
	mostrarMejorPuntaje();
	printf("Historias clinicas guardadas: %d\n", contarNodos(historias));

	for(i = 0; i < CANT_PRIORIDADES; i++){
		vaciarCola(&sala[i]);
	}
	liberarArbol(&historias);

	return 0;
}


/* ===== Reglas del juego ===== */
void mostrarReglas(){
	printf("=========================================================\n");
	printf("          CODIGO ROJO - Guardia Hospitalaria             \n");
	printf("=========================================================\n");
	printf("Sos el medico de guardia. En cada turno llegan pacientes:\n");
	printf(" - Registralos y asignales una prioridad (triage):\n");
	printf("     1 = URGENTE   2 = MEDIA   3 = LEVE\n");
	printf(" - Cada prioridad tiene su cola de espera.\n");
	printf(" - Se atiende primero la cola URGENTE, despues MEDIA y LEVE.\n");
	printf(" - En cada turno podes atender a un paciente.\n\n");
	printf("Puntos:\n");
	printf("  +%d triage correcto        %d triage incorrecto\n", PUNTOS_TRIAGE_BIEN, PUNTOS_TRIAGE_MAL);
	printf("  +%d por cada atencion       %d si habia un urgente esperando\n", PUNTOS_ATENCION, PUNTOS_URGENTE_ESPERANDO);
	printf("  %d por turno por cada urgente que espera mas de %d turnos\n\n", PUNTOS_DEMORA, LIMITE_ESPERA);
	printf("Objetivo: atender a todos con la mayor eficiencia.\n");
	printf("=========================================================\n");
}


/* ===== Limpiar el buffer del teclado ===== */
/* Funcion propia: descarta lo que quedo escrito hasta el enter, asi una letra no traba al scanf */
void limpiarBuffer(){
	int caracter;

	do{
		caracter = getchar();
	}while(caracter != '\n' && caracter != EOF);
}


/* ===== Leer un entero entre un minimo y un maximo ===== */
/* Funcion propia: basada en leerValor del TP5, pide un numero hasta que este entre pMinimo y pMaximo */
int leerEntero(int pMinimo, int pMaximo){
	int numero;
	int valoresLeidos;

	do{
		valoresLeidos = scanf("%d", &numero);
		if(valoresLeidos == EOF){
			exit(EXIT_FAILURE);
		}
		limpiarBuffer();
		if(valoresLeidos != 1 || numero < pMinimo || numero > pMaximo){
			printf("Valor invalido, ingrese un numero entre %d y %d: ", pMinimo, pMaximo);
		}
	}while(valoresLeidos != 1 || numero < pMinimo || numero > pMaximo);

	return numero;
}


/* ===== Leer el nombre del jugador ===== */
/* Funcion propia: pide el nombre hasta que se escriba uno y lo corta antes de un ';', porque es el separador de puntajes.txt */
void leerNombre(tString pNombre){
	int valoresLeidos;

	do{
		valoresLeidos = scanf(" %49[^\n;]", pNombre);
		if(valoresLeidos == EOF){
			exit(EXIT_FAILURE);
		}
		limpiarBuffer();
		if(valoresLeidos != 1){
			printf("Nombre invalido, ingrese su nombre: ");
		}
	}while(valoresLeidos != 1);
}


/* ===== Menu ===== */
int menu(){
	printf("========== MENU ==========\n");
	printf("1. Atender al siguiente paciente\n");
	printf("2. Ver la sala de espera\n");
	printf("3. Buscar historia clinica por DNI\n");
	printf("4. Listar historias clinicas\n");
	printf("5. Eliminar historia clinica\n");
	printf("6. Pasar el turno sin atender\n");
	printf("0. Terminar la guardia\n");
	printf("==========================\n");
	printf("Ingrese una opcion: ");
	return leerEntero(0, 6);
}


/* ===== Inicializar la sala (una cola por prioridad) ===== */
/* Funcion propia: la sala es un arreglo de 3 colas (URGENTE, MEDIA y LEVE) e inicializa cada una */
void inicializarSala(tCola pSala[]){
	int i;

	for(i = 0; i < CANT_PRIORIDADES; i++){
		inicializarCola(&pSala[i]);
	}
}


/* ===== Sala vacia ===== */
/* Funcion propia: devuelve true solo si las 3 colas estan vacias */
bool salaVacia(tCola pSala[]){
	int i;

	for(i = 0; i < CANT_PRIORIDADES; i++){
		if(!colaVacia(pSala[i])){
			return false;
		}
	}
	return true;
}


/* ===== Saber si un DNI ya esta en la sala ===== */
/* Funcion propia: busca el DNI en las 3 colas con estaEnCola */
bool pacienteEnSala(tCola pSala[], int pDni){
	int i;

	for(i = 0; i < CANT_PRIORIDADES; i++){
		if(estaEnCola(pSala[i], pDni)){
			return true;
		}
	}
	return false;
}


/* ===== Cantidad de pacientes en la sala ===== */
/* Funcion propia: suma los pacientes de las 3 colas */
int pacientesEnSala(tCola pSala[]){
	int i;
	int totalPacientes = 0;

	for(i = 0; i < CANT_PRIORIDADES; i++){
		totalPacientes = totalPacientes + cantidadElementos(pSala[i]);
	}
	return totalPacientes;
}


/* ===== Mostrar la sala de espera ===== */
/* Funcion propia: muestra cada cola con su prioridad, su cantidad y sus pacientes */
void mostrarSala(tCola pSala[]){
	int i;

	if(salaVacia(pSala)){
		printf("\nLa sala de espera esta vacia.\n");
	}else{
		for(i = 0; i < CANT_PRIORIDADES; i++){
			printf("\n*** Cola %s (%d) ***\n", nombrePrioridad(i + 1), cantidadElementos(pSala[i]));
			if(!colaVacia(pSala[i])){
				printf("%-10s %-18s %-6s %-26s %-10s %s\n", "DNI", "Nombre", "Edad", "Motivo", "Prioridad", "Espera");
				visualizarElementos(pSala[i]);
			}
		}
	}
}


/* ===== Llegada de pacientes ===== */
/* Funcion propia: crea 1 o 2 pacientes al azar, muestra su historia, pide la prioridad, suma o resta puntos y los encola */
void llegadaPacientes(tCola pSala[], tArbol * pHistorias, int * pPuntaje, int * pLlegados){
	int cantidadPacientes;
	int i;
	int indicePersona;
	int indiceSintoma;
	int intentos;
	int prioridad;
	tArbol * nodoEncontrado;
	tPaciente paciente;

	cantidadPacientes = 1 + rand() % 2;
	for(i = 0; i < cantidadPacientes; i++){
		indicePersona = rand() % CANT_PERSONAS;
		intentos = 0;
		while(pacienteEnSala(pSala, personas[indicePersona].dni) && intentos < CANT_PERSONAS){
			indicePersona = (indicePersona + 1) % CANT_PERSONAS;
			intentos++;
		}

		if(intentos < CANT_PERSONAS){
			indiceSintoma = rand() % CANT_SINTOMAS;
			paciente = crearPaciente(personas[indicePersona].dni, personas[indicePersona].nombre,
				personas[indicePersona].edad, sintomas[indiceSintoma].descripcion, sintomas[indiceSintoma].gravedad);

			printf("\n>> Llega un paciente:\n");
			printf("   %s, %d anios (DNI %d)\n", paciente.nombre, paciente.edad, paciente.dni);
			printf("   Motivo de consulta: %s\n", paciente.sintoma);

			nodoEncontrado = buscar(pHistorias, paciente.dni);
			if(nodoEncontrado != NULL){
				printf("   Tiene historia clinica: %d atencion/es. Ultima: %s -> %s\n", nodoEncontrado->historia.cantidadAtenciones,
					nodoEncontrado->historia.ultimoSintoma, nodoEncontrado->historia.ultimoResultado);
			}else{
				printf("   Paciente nuevo, no tiene historia clinica.\n");
			}

			printf("   Asigne la prioridad (1 URGENTE, 2 MEDIA, 3 LEVE): ");
			prioridad = leerEntero(URGENTE, LEVE);
			asignarPrioridad(&paciente, prioridad);

			if(triageCorrecto(paciente)){
				printf("   Triage correcto! +%d\n", PUNTOS_TRIAGE_BIEN);
				*pPuntaje = *pPuntaje + PUNTOS_TRIAGE_BIEN;
			}else{
				printf("   Triage incorrecto, era %s. %d\n", nombrePrioridad(paciente.gravedad), PUNTOS_TRIAGE_MAL);
				*pPuntaje = *pPuntaje + PUNTOS_TRIAGE_MAL;
			}

			encolar(&pSala[prioridad - 1], paciente);
			(*pLlegados)++;
		}
	}
}


/* ===== Atender al siguiente paciente ===== */
/* Funcion propia: saca al primero de la cola mas urgente que tenga pacientes, suma o resta puntos y registra la atencion */
void atenderPaciente(tCola pSala[], tArbol ** pHistorias, int * pPuntaje, int * pAtendidos){
	int indiceCola = 0;
	tPaciente paciente;
	tString resultado;

	while(colaVacia(pSala[indiceCola])){
		indiceCola++;
	}
	paciente = primerElemento(pSala[indiceCola]);
	desencolar(&pSala[indiceCola]);

	printf("\nAtendiendo a %s (%s, prioridad %s)\n", paciente.nombre, paciente.sintoma, nombrePrioridad(paciente.prioridad));

	if(paciente.gravedad != URGENTE && urgentesEsperando(pSala) > 0){
		printf("Habia un paciente urgente esperando en otra cola! %d\n", PUNTOS_URGENTE_ESPERANDO);
		*pPuntaje = *pPuntaje + PUNTOS_URGENTE_ESPERANDO;
	}else{
		printf("Buena atencion! +%d\n", PUNTOS_ATENCION);
		*pPuntaje = *pPuntaje + PUNTOS_ATENCION;
	}

	elegirResultado(paciente.gravedad, resultado);
	printf("Resultado: %s\n", resultado);

	registrarAtencion(pHistorias, paciente, resultado);
	registrarAtendido(paciente, resultado);
	(*pAtendidos)++;
}


/* ===== Cantidad de urgentes reales esperando ===== */
/* Funcion propia: cuenta en las 3 colas los pacientes cuya gravedad real es URGENTE */
int urgentesEsperando(tCola pSala[]){
	int i;
	int totalUrgentes = 0;

	for(i = 0; i < CANT_PRIORIDADES; i++){
		totalUrgentes = totalUrgentes + contarGravedad(pSala[i], URGENTE);
	}
	return totalUrgentes;
}


/* ===== Fin de turno: sumar espera y contar urgentes demorados ===== */
/* Funcion propia: suma un turno de espera en las 3 colas y devuelve cuantos urgentes pasaron el limite */
int controlarEspera(tCola pSala[]){
	int i;
	int demorados = 0;

	for(i = 0; i < CANT_PRIORIDADES; i++){
		demorados = demorados + aumentarEspera(&pSala[i], LIMITE_ESPERA);
	}
	return demorados;
}


/* ===== Resultado de la atencion segun la gravedad ===== */
/* Funcion propia: elige al azar un resultado posible para la gravedad real del paciente */
void elegirResultado(int pGravedad, tString pResultado){
	tString resultadosUrgentes[3] = {"Internacion", "Derivado a cirugia", "Terapia intensiva"};
	tString resultadosMedias[3] = {"Tratamiento y alta", "Internacion", "Yeso y control"};
	tString resultadosLeves[3] = {"Alta medica", "Receta y alta", "Reposo en casa"};
	int indiceResultado;

	indiceResultado = rand() % 3;
	switch(pGravedad){
		case URGENTE:
			strcpy(pResultado, resultadosUrgentes[indiceResultado]);
			break;
		case MEDIA:
			strcpy(pResultado, resultadosMedias[indiceResultado]);
			break;
		default:
			strcpy(pResultado, resultadosLeves[indiceResultado]);
	}
}


/* ===== Registrar al atendido (archivo de texto) ===== */
/* Funcion propia: agrega una linea con los datos del atendido al final de atendidos.txt */
void registrarAtendido(tPaciente pPaciente, tString pResultado){
	FILE * archivo;

	archivo = fopen(ARCHIVO_ATENDIDOS, "a");
	if(archivo == NULL){
		printf("No se pudo abrir el registro de atendidos.\n");
	}else{
		fprintf(archivo, "%d;%s;%s;%s;%d;%s\n", pPaciente.dni, pPaciente.nombre, pPaciente.sintoma,
			nombrePrioridad(pPaciente.prioridad), pPaciente.espera, pResultado);
		fclose(archivo);
	}
}


/* ===== Buscar una historia clinica ===== */
/* Funcion propia: pide el DNI y muestra la historia usando buscar del arbol */
void buscarHistoria(tArbol * pHistorias){
	int dni;
	tArbol * nodoEncontrado;

	printf("\nDNI a buscar: ");
	dni = leerEntero(1, 99999999);
	nodoEncontrado = buscar(pHistorias, dni);
	if(nodoEncontrado != NULL){
		printf("%-10s %-18s %-6s %-11s %s\n", "DNI", "Nombre", "Edad", "Atenciones", "Ultima atencion");
		mostrarHistoria(nodoEncontrado->historia);
	}else{
		printf("No existe una historia clinica con DNI %d\n", dni);
	}
}


/* ===== Eliminar una historia clinica ===== */
/* Funcion propia: pide el DNI y lo borra del arbol con eliminarNodo */
void eliminarHistoria(tArbol ** pHistorias){
	int dni;

	printf("\nDNI de la historia a eliminar: ");
	dni = leerEntero(1, 99999999);
	eliminarNodo(pHistorias, dni);
}


/* ===== Guardar el puntaje (archivo de texto) ===== */
/* Funcion propia: agrega el jugador, el puntaje y la eficiencia al final de puntajes.txt */
void guardarPuntaje(tString pJugador, int pPuntaje, int pEficiencia){
	FILE * archivo;

	archivo = fopen(ARCHIVO_PUNTAJES, "a");
	if(archivo == NULL){
		printf("No se pudo guardar el puntaje.\n");
	}else{
		fprintf(archivo, "%s;%d;%d\n", pJugador, pPuntaje, pEficiencia);
		fclose(archivo);
	}
}


/* ===== Mejor puntaje: busqueda del maximo en el archivo ===== */
/* Funcion propia: lee puntajes.txt linea por linea y se queda con el mayor puntaje */
void mostrarMejorPuntaje(){
	FILE * archivo;
	tString nombre;
	tString mejorNombre;
	int puntos;
	int eficiencia;
	int mejorPuntos = 0;
	int partidas = 0;

	archivo = fopen(ARCHIVO_PUNTAJES, "r");
	if(archivo == NULL){
		printf("Todavia no hay partidas registradas.\n");
	}else{
		while(fscanf(archivo, " %49[^;];%d;%d", nombre, &puntos, &eficiencia) == 3){
			if(partidas == 0 || puntos > mejorPuntos){
				mejorPuntos = puntos;
				strcpy(mejorNombre, nombre);
			}
			partidas++;
		}
		fclose(archivo);

		if(partidas > 0){
			printf("Partidas jugadas: %d | Mejor puntaje: %s con %d puntos\n", partidas, mejorNombre, mejorPuntos);
		}
	}
}
