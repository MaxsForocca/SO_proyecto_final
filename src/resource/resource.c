#include "resource.h"
#include "../print/printf/printf.h"

// Inicializa un recurso
void init_resource(resource_t* resource, int id, int is_shared) {
    resource->id = id;
    resource->is_in_use = 0;
    resource->owner = NULL;
    resource->is_shared = is_shared;
    resource->queue_size = 0;
}

// Solicita un recurso
int use_resource(process_t* process, resource_t* resource) {
    if (resource->is_shared) {
        // Si el recurso es compartido, permite su uso simultáneo
        printf("Proceso PID=%lu está usando el recurso compartido %d.\n", process->pid, resource->id);
        return 1; // Recurso asignado
    } else {
        if (resource->is_in_use) {
            // Recurso ocupado, agregar a la cola de espera
            if (resource->queue_size < MAX_WAIT_QUEUE) {
                resource->wait_queue[resource->queue_size++] = process;
                process->state = PROCESS_WAITING;
                printf("Proceso PID=%lu está esperando por el recurso exclusivo %d.\n", process->pid, resource->id);
            } else {
                printf("Cola de espera del recurso %d está llena. Proceso PID=%lu no puede esperar.\n", resource->id, process->pid);
            }
            return 0; // No se pudo asignar el recurso
        } else {
            // Asigna el recurso al proceso
            resource->is_in_use = 1;
            resource->owner = process;
            printf("Proceso PID=%lu adquirió el recurso exclusivo %d.\n", process->pid, resource->id);
            return 1; // Recurso asignado
        }
    }
}

// Libera un recurso
void release_resource(resource_t* resource) {
    if (!resource->is_shared && resource->is_in_use) {
        printf("Recurso %d liberado por el Proceso PID=%lu.\n", resource->id, resource->owner->pid);
        resource->is_in_use = 0;
        resource->owner = NULL;

        // Notificar al siguiente proceso en la cola de espera
        if (resource->queue_size > 0) {
            process_t* next_process = resource->wait_queue[0];
            for (int i = 1; i < resource->queue_size; i++) {
                resource->wait_queue[i - 1] = resource->wait_queue[i];
            }
            resource->queue_size--;
            next_process->state = PROCESS_READY;
            printf("Proceso PID=%lu ahora está LISTO y usando el recurso %d.\n", next_process->pid, resource->id);

            // Asignar el recurso al siguiente proceso
            resource->is_in_use = 1;
            resource->owner = next_process;
        }
    }
}
