# Código Rojo: Guardia Hospitalaria

Proyecto integrador de **Algoritmos y Estructuras de Datos II** (FaCENA - UNNE, 2026), Grupo 09.

Juego en C donde el jugador es el médico de guardia: recibe pacientes por turnos, los clasifica por gravedad
(triage: Urgente, Media o Leve), los atiende en orden y lleva las historias clínicas de cada paciente.

![Flyer](docs/Flyer%20-%20Codigo%20Rojo.png)

## Estructuras y técnicas

| Parte del sistema | Estructura |
|---|---|
| Sala de espera | Tres colas con punteros (FIFO), una por prioridad |
| Historias clínicas | Árbol binario de búsqueda ordenado por DNI, con funciones recursivas |
| Persistencia | `historias.dat` (binario), `atendidos.txt` y `puntajes.txt` (texto) |
| TAD | `tPaciente` (paciente.h), `tCola` (colaEspera.h), `tArbol` (arbolHistorias.h) |

## Cómo compilar y jugar

Abrir `codigo/codigoRojo.c` en Dev-C++ y compilar (los tres `.h` tienen que estar en la misma carpeta), o desde una terminal:

```
gcc codigoRojo.c -o codigoRojo
./codigoRojo
```

## Archivos

- `codigo/`: el programa (`codigoRojo.c`) y las bibliotecas de los TAD.
- `docs/`: informe, documentación del código y flyer.

## Integrantes

Batalla Diman Gabriel, Buera Silva Nicolas, Caballero Sabrina Alejandra, Centurión Gonzales Agustina Luciana, Diez Santiago Ignacio.

Profesores: Badaracco Numa y Vallejos Sofía.
