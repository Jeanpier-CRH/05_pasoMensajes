#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 0x4D52
#define MESSAGE_SIZE 128

struct message {
    long mtype;
    char mtext[MESSAGE_SIZE];
};

int main(void) {
    int qid;
    struct message msg;

    qid = msgget((key_t)QUEUE_KEY, 0666);
    if (qid == -1) {
        perror("Client 1: msgget");
        fprintf(stderr, "Inicie primero el servidor.\n");
        return EXIT_FAILURE;
    }

    /* Enviar solicitud al servidor */
    msg.mtype = 11;
    snprintf(msg.mtext, sizeof(msg.mtext),
             "Solicitud de informacion del Cliente 1");

    if (msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("Client 1: msgsnd");
        return EXIT_FAILURE;
    }

    printf("Cliente 1: solicitud enviada.\n");

    /* Recibir solamente mensajes de tipo 1 */
    if (msgrcv(qid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
        perror("Client 1: msgrcv");
        return EXIT_FAILURE;
    }

    printf("Cliente 1 recibe: %s\n", msg.mtext);

    /* Confirmar recepción al servidor */
    msg.mtype = 21;
    snprintf(msg.mtext, sizeof(msg.mtext),
             "Respuesta recibida correctamente");

    if (msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("Client 1: msgsnd ACK");
        return EXIT_FAILURE;
    }

    printf("Cliente 1: confirmacion enviada.\n");
    return EXIT_SUCCESS;
}
