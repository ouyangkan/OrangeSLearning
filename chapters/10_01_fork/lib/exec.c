#include "const.h"
#include "proc.h"
#include "proto.h"
#include "string.h"
#include "stdio.h"

PUBLIC int exec(const char *path)
{
    MESSAGE msg;
    msg.type = EXEC;
    msg.PATHNAME = (void *)path;
    msg.NAME_LEN = strlen(path);
    msg.BUF = 0;
    msg.BUF_LEN = 0;

    send_recv(BOTH, TASK_MM, &msg);
    assert(msg.type == SYSCALL_RET);

    return msg.RETVAL;
}

PUBLIC int execl(const char *path, const char *arg, ...)
{
    va_list parg = (va_list)(&arg);
    char **p = (char **)parg;
    return execv(path, p);
}

PUBLIC int execv(const char *path, char *argv[])
{
    char **p = argv;
    char arg_stack[PROC_ORIGIN_STACK];
    int stack_len = 0;

    while (*p++) {
        assert(stack_len + 2 * sizeof(char *) < PROC_ORIGIN_STACK);
        stack_len += sizeof(char *);
    }

    *((int *)(&arg_stack[stack_len])) = 0;
    stack_len += sizeof(char *);

    char **q = (char **)arg_stack;
    for (p = argv; *p != 0; p++) {
        *q++ = &arg_stack[stack_len];

        assert(stack_len + strlen(*p) + 1 < PROC_ORIGIN_STACK);
        strcpy(&arg_stack[stack_len], *p);
        stack_len += strlen(*p);
        arg_stack[stack_len] = 0;
        stack_len++;
    }

    MESSAGE msg;
    msg.type = EXEC;
    msg.PATHNAME = (void *)path;
    msg.NAME_LEN = strlen(path);
    msg.BUF = (void *)arg_stack;
    msg.BUF_LEN = stack_len;

    send_recv(BOTH, TASK_MM, &msg);
    assert(msg.type == SYSCALL_RET);

    return msg.RETVAL;

}
