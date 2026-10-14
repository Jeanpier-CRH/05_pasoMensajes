/*
Compilacion del programa
gcc message_rec.c -o recv
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
    char mtext[MSGSZ]; // Un mensaje de longitud MSGSZ
} message_buf;

main()
{
    int msqid;
    key_t key;
    message_buf rbuf, sbuf;
    size_t buf_length;

    key = 1234;
    // Este metodo trata de acceder a una cola de mensaje con llave 1234
    // y con permisos de lectura y escritura para todo el mundo
    if ((msqid = msgget(key, 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }
    // Esta es una llamada BLOQUEANTE sobre la cola de mensajes con identificador
    // msqid. El mensaje se almacenara en la variable rbuf y se leera un total de
    // MSGSZ bytes. El mensaje sera de tipo 1.
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("%s\n", rbuf.mtext);
    exit(0);
}
