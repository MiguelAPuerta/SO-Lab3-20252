/*
 * fibonacci.c
 * 
 * Copyright 2025 osboxes.org <osboxes@vbox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estructura para pasar múltiples argumentos al hilo
typedef struct {
    int *arr;   // puntero al arreglo compartido
    int N;      // número de elementos
} fib_data_t;

// Función ejecutada por el hilo trabajador
void* threadFibonacci(void* arg) {
    fib_data_t *data = (fib_data_t*) arg;
    int *arr = data->arr;
    int N = data->N;

    if (N > 0) arr[0] = 0;
    if (N > 1) arr[1] = 1;

    for (int i = 2; i < N; i++) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Error: N debe ser mayor que 0.\n");
        return 1;
    }

    // Reservar memoria para el arreglo compartido
    int *arr = (int*) malloc(N * sizeof(int));
    if (arr == NULL) {
        perror("Error al asignar memoria");
        return 1;
    }

    pthread_t worker;
    fib_data_t data = { arr, N };

    // Crear hilo trabajador
    pthread_create(&worker, NULL, threadFibonacci, &data);

    // Esperar a que el hilo termine
    pthread_join(worker, NULL);

    // Imprimir la secuencia generada
    printf("Secuencia de Fibonacci (%d primeros términos):\n", N);
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Liberar memoria
    free(arr);

    return 0;
}
