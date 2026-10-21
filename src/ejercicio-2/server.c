#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 0x4D52
#define MESSAGE_SIZE 128
#define CLIENT1_REQUEST 11
#define CLIENT2_REQUEST 12
#define CLIENT1_REPLY   1
#define CLIENT2_REPLY   2
#define CLIENT1_ACK     21
#define CLIENT2_ACK     22

struct message {
    long mtype;
    char mtext[MESSAGE_SIZE];
};

static int receive_message(int qid, struct message *msg, long type) {
    if (msgrcv(qid, msg, sizeof(msg->mtext), type, 0) == -1) {
        perror("Server: msgrcv");
        return -1;
    }
    return 0;
}

static int send_message(int qid, long type, const char *text) {
    struct message msg;
    msg.mtype = type;
    snprintf(msg.mtext, sizeof(msg.mtext), "%s", text);

    if (msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("Server: msgsnd");
        return -1;
    }
    return 0;
}

int main(void) {
    int qid;
    struct message msg;

    /* Crear una única cola */
    qid = msgget((key_t)QUEUE_KEY, IPC_CREAT | IPC_EXCL | 0666);
    if (qid == -1) {
        perror("Server: msgget");
        fprintf(stderr,
                "Si existe una cola anterior, elimínela con:\n"
                "ipcrm -Q 19794\n");
        return EXIT_FAILURE;
    }

    printf("Servidor iniciado. ID de cola: %d\n", qid);
    printf("Clave de la cola: 19794\n");
    printf("Esperando solicitudes de los dos clientes...\n");
    fflush(stdout);

    /* Recibir solicitud del cliente 1 */
    if (receive_message(qid, &msg, CLIENT1_REQUEST) == -1)
        goto error;

    printf("Solicitud del cliente 1: %s\n", msg.mtext);

    /* Recibir solicitud del cliente 2 */
    if (receive_message(qid, &msg, CLIENT2_REQUEST) == -1)
        goto error;

    printf("Solicitud del cliente 2: %s\n", msg.mtext);

    /* Responder individualmente mediante mtype */
    if (send_message(qid, CLIENT1_REPLY,
                     "Respuesta privada del servidor para Cliente 1") == -1)
        goto error;

    printf("Respuesta enviada al cliente 1 (tipo 1).\n");

    if (send_message(qid, CLIENT2_REPLY,
                     "Respuesta privada del servidor para Cliente 2") == -1)
        goto error;

    printf("Respuesta enviada al cliente 2 (tipo 2).\n");
    printf("Esperando confirmaciones de recepción...\n");

    /* Esperar confirmación de ambos clientes */
    if (receive_message(qid, &msg, CLIENT1_ACK) == -1)
        goto error;

    printf("Confirmación del cliente 1: %s\n", msg.mtext);

    if (receive_message(qid, &msg, CLIENT2_ACK) == -1)
        goto error;

    printf("Confirmación del cliente 2: %s\n", msg.mtext);

    /* Eliminar la cola cuando terminó la comunicación */
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        perror("Server: msgctl");
        return EXIT_FAILURE;
    }

    printf("Cola eliminada. Servidor finalizado.\n");
    return EXIT_SUCCESS;

error:
    /* Limpiar la cola ante un error */
    if (msgctl(qid, IPC_RMID, NULL) == -1)
        perror("Server: msgctl al limpiar");

    return EXIT_FAILURE;
}
