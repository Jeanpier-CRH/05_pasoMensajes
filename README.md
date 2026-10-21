# Práctica de paso de mensajes en GNU/Linux

Implementación en C de mecanismos de comunicación entre procesos mediante colas de mensajes **System V IPC**.

## Contenido

- `message_recv_m.c` y `message_send.c`: intercambio de mensajes entre dos procesos.
- `server.c`: servidor que atiende a dos clientes mediante una sola cola.
- `client1.c` y `client2.c`: clientes que reciben respuestas individuales según el tipo de mensaje (`mtype`).

## Requisitos

- GNU/Linux
- Compilador GCC
- Herramientas de desarrollo (`build-essential` en Debian/Ubuntu)

## Compilación

```bash
gcc -Wall -Wextra -o recv_m message_recv_m.c
gcc -Wall -Wextra -o send_m message_send_m.c

gcc -Wall -Wextra -o server server.c
gcc -Wall -Wextra -o client1 client1.c
gcc -Wall -Wextra -o client2 client2.c
```

## Ejecución

**Ejercicio 1:** ejecutar `./`recv_m y luego `./`send_m en terminales separadas.

**Ejercicio 2:** ejecutar `./server` y después `./client1` y `./client2`, cada uno en una terminal independiente.

Para verificar las colas activas:

```bash
ipcs -q
```

## Objetivo

Comprender el envío, la recepción y el direccionamiento de mensajes entre procesos utilizando las funciones `msgget()`, `msgsnd()`, `msgrcv()` y `msgctl()`.
