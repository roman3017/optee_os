

#include <platform_config.h>

#include <console.h>
#include <drivers/pl011.h>
#include <trace.h>

unsigned get_mode(void);
static void console_putf(const char *fmt, ...);

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

static void s_puts(const char *s)
{
        while(*s)
                console_putc(*s++);
        //console_flush();
}

static int s_divr(unsigned num, unsigned den, unsigned *rem)
{
        int ret;
        ret = 0;
        while (num >= den)
        {
                num -= den;
                ret++;
        }
        *rem = num;
        return ret;
}

static void s_puti(int xx, unsigned base, int sign)
{
	static char digits[] = "0123456789abcdef";
	char buf[16];
	unsigned i, x, r;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do {
                x = s_divr(x, base, &r);
		buf[i++] = digits[r];
	} while(x != 0);

	if(sign)
		buf[i++] = '-';

	while(i-- > 0)
		console_putc(buf[i]);

        //console_flush();
}

static void console_putf(const char *fmt, ...)
{
	int i, c;
	unsigned int *argp;
	char *s;

	argp = (unsigned int*)(void*)(&fmt + 1);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++)
	{
		if(c != '%')
		{
			console_putc(c);
			continue;
		}
		c = fmt[++i] & 0xff;
		if(c == 0)
			break;

		switch(c)
		{
			case 'd':
				s_puti(*argp++, 10, 1);
				break;
			case 'x':
			case 'p':
				s_puti(*argp++, 16, 0);
				break;
			case 's':
				if((s = (char*)*argp++) == 0)
					s_puts("(null)");
                                else
                                        s_puts(s);
				break;
			case '%':
				console_putc('%');
				break;
			default:
				console_putc('%');
				console_putc(c);
				break;
		}
	}
        console_flush();
}

void console_dbg(unsigned r0);
void console_dbg(unsigned r0)
{
	console_putf("OP-TEE: dbg %s[%x]\n","r0",r0);
}

