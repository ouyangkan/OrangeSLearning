#include "const.h"
#include "proto.h"
#include "keyboard.h"
#include "global.h"
#include "tty.h"
#include "console.h"

#define TTY_FIRST (tty_table)
#define TTY_END   (tty_table + NR_CONSOLES)

PRIVATE void init_tty(TTY *p_tty);
PRIVATE void tty_do_read(TTY *p_tty);
PRIVATE void tty_do_write(TTY *p_tty);

PUBLIC void task_tty()
{
    TTY *p_tty;

    init_keyboard();

    for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++)
    {
        init_tty(p_tty);
    }

    while (1)
    {
        for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++)
        {
            tty_do_read(p_tty);
            tty_do_write(p_tty);
        }
    }
}

PRIVATE void init_tty(TTY *p_tty)
{
    p_tty->inbuf_count = 0;
    p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

    int nr_tty = p_tty - tty_table;
    p_tty->p_console = console_table + nr_tty;

    nr_current_console = 0;
}

PUBLIC void in_process(TTY *p_tty, u32 key)
{
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT))
    {
        if (p_tty->inbuf_count < TTY_IN_BYTES)
        {
            *(p_tty->p_inbuf_head) = key;
            p_tty->p_inbuf_head++;
            if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES)
            {
                p_tty->p_inbuf_head = p_tty->in_buf;
            }
            p_tty->inbuf_count++;
        }
    }
    else
    {
        int raw_code = key & MASK_RAW;
        switch (raw_code)
        {
            case UP:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
                {
                    disable_interrupt();
                    out_byte(CRTC_ADDR_REG, START_ADDR_H);
                    out_byte(CRTC_DATA_REG, ((80 * 15) >> 8) & 0xFF);
                    out_byte(CRTC_ADDR_REG, START_ADDR_L);
                    out_byte(CRTC_DATA_REG, (80 * 15) & 0xFF);
                    enable_interrupt();
                }
                break;
            case DOWN:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
                {
                }
                break;
            default:
                break;
        }
    }
}

PRIVATE void tty_do_read(TTY *p_tty)
{
    if (is_current_console(p_tty->p_console))
    {
        keyboard_read(p_tty);
    }
}

PRIVATE void tty_do_write(TTY *p_tty)
{
    if (p_tty->inbuf_count)
    {
        char ch = *(p_tty->p_inbuf_tail);
        p_tty->p_inbuf_tail++;
        if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES)
        {
            p_tty->p_inbuf_tail = p_tty->in_buf;
        }
        p_tty->inbuf_count--;

        out_char(p_tty->p_console, ch);
    }
}
