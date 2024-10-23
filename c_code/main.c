/* Copyright 2024 Grug Huhler.  License SPDX BSD-2-Clause.
 *
 * Simple program for Cortex M3 on Tang Nano 4K.
 */

#include "uart.h"
#include "pwm_controller.h"

void help(void)
{
  uart_puts(UART0, "rb addr       : read byte\r\n");
  uart_puts(UART0, "rh addr       : read half word\r\n");
  uart_puts(UART0, "rw addr       : read word\r\n");
  uart_puts(UART0, "wb addr value : write byte\r\n");
  uart_puts(UART0, "wh addr value : write half word\r\n");
  uart_puts(UART0, "ww addr value : write word\r\n");
  uart_puts(UART0, "   all numbers are hex\r\n");
}

void read_byte(unsigned int addr)
{
  volatile unsigned char *p = (volatile unsigned char *) addr;

  uart_print_hex(UART0, *p);
  uart_puts(UART0, "\r\n");
}

void read_half(unsigned int addr)
{
  volatile unsigned short *p = (volatile unsigned short *) addr;

  uart_print_hex(UART0, *p);
  uart_puts(UART0, "\r\n");
}

void read_word(unsigned int addr)
{
  volatile unsigned int *p = (volatile unsigned int *) addr;

  uart_print_hex(UART0, *p);
  uart_puts(UART0, "\r\n");
}

void write_byte(unsigned int addr, unsigned int value)
{
  volatile unsigned char *p = (volatile unsigned char *) addr;

  *p = value;
}

void write_half(unsigned int addr, unsigned int value)
{
  volatile unsigned short *p = (volatile unsigned short *) addr;

  *p = value;
}

void write_word(unsigned int addr, unsigned int value)
{
  volatile unsigned int *p = (volatile unsigned int *) addr;

  *p = value;
}


/* struct command lists available commands.  See function help.
 * Each function takes one or two arguments.  The table below
 * contains a pointer to the function to call for each command.
 */

struct command {
  char *cmd_string;
  int num_args;
  union {
    void (*func0)(void);
    void (*func1)(unsigned int val);
    void (*func2)(unsigned int val1, unsigned int val2);
  } u;
} commands[] = {
  {"he", 0, .u.func0=help},
  {"rb", 1, .u.func1=read_byte}, // addr
  {"rh", 1, .u.func1=read_half}, // addr
  {"rw", 1, .u.func1=read_word}, // addr
  {"wb", 2, .u.func2=write_byte}, // addr, val
  {"wh", 2, .u.func2=write_half}, // addr, val
  {"ww", 2, .u.func2=write_word} // addr, val
};


void eat_spaces(char **buf, unsigned int *len)
{
  while (len > 0) {
    if (**buf == ' ') {
      *buf += 1;
      *len -= 1;
    } else
      break;
  }
}

/* Returns 1 if a number found, else 0.  Number is in *v */

unsigned int get_hex(char **buf, unsigned int *len, unsigned int *v)
{
  int valid = 0;
  int keep_going;
  char ch;

  keep_going = 1;

  *v = 0;
  while (keep_going && (*len > 0)) {

    ch = **buf;
    *buf += 1;
    *len -= 1;

    if ((ch >= '0') && (ch <= '9')) {
      *v = 16*(*v) + (ch - '0');
      valid = 1;
    } else if ((ch >= 'a') && (ch <= 'f')) {
      *v = 16*(*v) + (ch - 'a' + 10);
      valid = 1;
    } else if ((ch >= 'A') && (ch <= 'F')) {
      *v = 16*(*v) + (ch - 'A' + 10);
      valid = 1;
    } else {
      keep_going = 0;
    }
  }

  return valid;
}


void parse(char *buf, unsigned int len)
{
  int i, cmd_not_ok;
  unsigned int val1, val2;

  cmd_not_ok = 1;
  eat_spaces(&buf, &len);
  if (len < 2) goto err;

  for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++)
    if ((buf[0] == commands[i].cmd_string[0]) && (buf[1] == commands[i].cmd_string[1])) {
      buf += 2;
      len -= 2;
      switch (commands[i].num_args) {
      case 0:
        commands[i].u.func0();
        cmd_not_ok = 0;
        break;
      case 1:
        eat_spaces(&buf, &len);
        if (get_hex(&buf, &len, &val1)) {
          commands[i].u.func1(val1);
          cmd_not_ok = 0;
        }
        break;
      case 2:
        eat_spaces(&buf, &len);
        if (get_hex(&buf, &len, &val1)) {
          eat_spaces(&buf, &len);
          if (get_hex(&buf, &len, &val2)) {
            commands[i].u.func2(val1, val2);
            cmd_not_ok = 0;
          }
        }
        break;
      default:
        break;
      }
      break;
    }

 err:
  if (cmd_not_ok)
    uart_puts(UART0,"illegal command, he for help\r\n");
}

#define BUFLEN 64

void spindelay(unsigned int n)
{
  volatile unsigned int nn = n;

  while (nn-- > 0) {}
}

int main()
{
  char buf[BUFLEN];
  unsigned int len;
  
  /* Blink LED once per second, assuming clock frequency is 9 MHz */
  pwm_set_counter_limit(9000000);
  pwm_set_on_limit(9000000/2);

  uart_init(UART0, 78); /* 27e6 / 115200 */
  uart_puts(UART0, "Hello, world!\r\n");

  uart_puts(UART0, "\r\nenter he for a list of commands\r\n");
  
  while (1) {
    len = uart_gets(UART0, buf, BUFLEN);
    if (len != 0) parse(buf, len);
  }
  
  return 0;
}
