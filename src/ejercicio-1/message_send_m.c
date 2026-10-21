#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    // Crear la cola de mensajes (si no existe) con permisos de lectura y escritura
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget (send)");
        exit(1);
    }

    // --- MENSAJE 1: Proceso A -> Proceso B (mtype = 1) ---
    sbuf.mtype = 1;
    snprintf(sbuf.mtext, MSGSZ, "Message 1: Are you hearing me?.");
    printf("[Process 1] Sending Message 1...\n");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd 1");
        exit(1);
    }

    // --- MENSAJE 2: Proceso B -> Proceso A (mtype = 2) ---
    printf("[Process 1] Awaiting a response (Message 2)...\n");
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) { // Bloquea hasta recibir tipo 2
        perror("msgrcv 2");
        exit(1);
    }
    printf("[Process 1]:Received from process 2 \"%s\"\n", rbuf.mtext);

    // --- MENSAJE 3: Proceso A -> Proceso B (mtype = 1) ---
    sbuf.mtype = 1;
    snprintf(sbuf.mtext, MSGSZ, "Message 3: I can hear you too.");
    printf("[Process 1] Sending Messaje 3...\n");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd 3");
        exit(1);
    }

    // Pausa breve para permitir que el Proceso B lea el ultimo mensaje
    sleep(1);

    // Eliminar la cola de mensajes de la memoria del sistema
    if (msgctl(msqid, IPC_RMID, NULL) < 0) {
        perror("msgctl IPC_RMID");
        exit(1);
    }
    printf("[Process 1] Process 1 completed.\n");

    return 0;
}
