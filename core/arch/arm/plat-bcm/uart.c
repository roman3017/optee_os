

#include <platform_config.h>

#include <console.h>
#include <drivers/pl011.h>
#include <trace.h>

static void console_puts(const char *s)
{
        while(*s)
                console_putc(*s++);
        console_flush();
}

void console_init(void)
{
	pl011_init(CONSOLE_UART_BASE, CONSOLE_UART_CLK_IN_HZ, CONSOLE_BAUDRATE);
	DMSG("console_init");
}

void console_putc(int ch)
{
	pl011_putc(ch, CONSOLE_UART_BASE);
}

void console_flush(void)
{
	pl011_flush(CONSOLE_UART_BASE);
}

void console_dbg(void);
void console_dbg(void)
{
	console_puts("OP-TEE: console_dbg\n");
}

