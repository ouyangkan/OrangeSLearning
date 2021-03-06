#define GLOBAL_VARIABLES_HERE

#include "const.h"
#include "global.h"
#include "proto.h"

/* 进程表 */
PUBLIC PROCESS proc_table[NR_TASKS + NR_PROCS];

/* 任务表 */
PUBLIC TASK task_table[NR_TASKS] = {
    {task_tty, STACK_SIZE_TTY, "TTY"},
    {task_sys, STACK_SIZE_SYS, "SYS"},
    {task_hd,  STACK_SIZE_HD,  "HD" },
    {task_fs,  STACK_SIZE_FS,  "FS" },
    {task_mm,  STACK_SIZE_MM,  "MM" },
};

/* 用户进程表 */
PUBLIC TASK user_proc_table[NR_PROCS] = {
    {Init,  STACK_SIZE_INIT,  "INIT" },
    {TestA, STACK_SIZE_TESTA, "TestA"},
    {TestB, STACK_SIZE_TESTB, "TestB"},
    {TestC, STACK_SIZE_TESTC, "TestC"},
};

/* 进程栈 */
PUBLIC char task_stack[STACK_SIZE_TOTAL];

/* 中断向量 */
PUBLIC irq_handler irq_table[NR_IRQ];

/* 系统调用 */
PUBLIC system_call sys_call_table[NR_SYS_CALL] = {
    sys_printx,
    sys_sendrec,
};

/* tty */
PUBLIC TTY     tty_table[NR_CONSOLES];
PUBLIC CONSOLE console_table[NR_CONSOLES];

/* FS 缓冲区 */
PUBLIC u8        *fsbuf     = (u8 *)0x600000;
PUBLIC const int FSBUF_SIZE =       0x100000; // 1MB bytes

/* MM */
PUBLIC u8        *mmbuf     = (u8 *)0x700000;
PUBLIC const int MMBUF_SIZE =       0x100000;

/* 设备驱动 */
struct dev_drv_map dd_map[] = {
    {INVALID_DRIVER}, // 0 : Unused
    {INVALID_DRIVER}, // 1 : Reserved for floppy driver
    {INVALID_DRIVER}, // 2 : Reserved for cdrom driver
    {TASK_HD},        // 3 : Hard disk
    {TASK_TTY},       // 4 : TTY
    {INVALID_DRIVER}, // 5 : Reserved for scsi disk driver
};
