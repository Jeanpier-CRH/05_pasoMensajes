#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    // Obtener la cola de mensajes creada por el Proceso A
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget (recv)");
        exit(1);
    }

    // --- MENSAJE 1: Recibir de A (mtype = 1) ---
    printf("[Process 2] Waiting Message 1 from process 1...\n");
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) { // Bloquea hasta recibir tipo 1
        perror("msgrcv 1");
        exit(1);
    }
    printf("[Process 2] Received from process 1: \"%s\"\n", rbuf.mtext);

    // --- MENSAJE 2: Responder a A (mtype = 2) ---
    sbuf.mtype = 2;
    snprintf(sbuf.mtext, MSGSZ, "Message 2: Load and clear.");
    printf("[Process 2] Sending Message 2 (rpt)...\n");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd 2");
        exit(1);
    }

    // --- MENSAJE 3: Recibir de A (mtype = 1) ---
    printf("[Process 2] Waiting Message 3 from process 1...\n");
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) { // Bloquea hasta recibir tipo 1
        perror("msgrcv 3");
        exit(1);
    }
    printf("[Process 2] Received from process 1: \"%s\"\n", rbuf.mtext);

    printf("[Process 2] Process 2 completed.\n");

    return 0;
}
