#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Un numero muy grande para mantener a la CPU ocupada por un buen rato
#define LOOP_COUNT 500000000

// Funcion que simula una carga de trabajo pesada
void heavy_computation(int tickets, int id) {
    int start_time, end_time;

    // Asignar los tiquetes a este proceso usando nuestra nueva System Call
    if (settickets(tickets) < 0) {
        printf("Error: No se pudieron asignar los tiquetes.\n");
        exit(1);
    }

    // Registrar el tiempo de inicio
    start_time = uptime();

    // Ciclo pesado. Usamos 'volatile' para evitar que el compilador
    // optimice y elimine el ciclo por considerarlo inútil.
    volatile int dummy = 0;
    for (int i = 0; i < LOOP_COUNT; i++) {
        dummy = dummy + 1;
    }

    // Registrar el tiempo de finalizacion
    end_time = uptime();

    printf("Proceso %d (Tiquetes: %d) termino en %d ticks de reloj.\n",
            id, tickets, end_time - start_time);

    exit(0);
}

int main() {
    printf("Iniciando prueba del Planificador por Loteria...\n");
    printf("Compitiendo por la CPU...\n");

    // Crear el primer proceso hijo
    int pid1 = fork();
    if (pid1 == 0) {
        heavy_computation(10, 1); // Proceso 1: Poca prioridad (10 tiquetes)
    } else {
        // Crear el segundo proceso hijo
        int pid2 = fork();
        if (pid2 == 0) {
            heavy_computation(100, 2); // Proceso 2: Alta prioridad (100 tiquetes)
        } else {
            // El proceso padre espera a que ambos hijos terminen
            wait(0);
            wait(0);
            printf("Prueba finalizada exitosamente.\n");
        }
    }
    exit(0);
}
