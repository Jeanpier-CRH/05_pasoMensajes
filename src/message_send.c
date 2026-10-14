/*
Compilacion del programa
gcc message_send.c -o send
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

/**
Esta estructura define los elementos que tiene un mensaje
*/
typedef struct msgbuf
{
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main()
{
    int msqid;
    // En este programa el que envia el mensaje es el que crea
    // la cola.
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    message_buf rbuf;
    size_t buf_length;

    key = 1234;

    // Invocar a la funcion msgget() con el msgflg con valor IPC_CREAT | 0666
    // provoca que se cree la cola si esta ya no existe. Si existe, se devuelve
    // en msqid el identificador de la cola con llave '1234'.
    if ((msqid = msgget(key, msgflg)) < 0)
    {
        perror("msgget()");
        exit(1);
    }
    // Esto me indica el tipo del mensaje
    sbuf.mtype = 1;
    (void)strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1;
    // En este caso el emisor o sender envia el mensaje pero no espera respuesta
    // para saber si el receptor recibio o no el mensaje
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }
    exit(0);
}

