#include <avr/io.h>
#include “UART.h”

#define NUM_DIGITS  4
#define MAX_NUM_TRIES   8

void capture_digits(uint8_t com, char *str, uint8_t echo)
{
    uint8_t idx = 0;
    while (idx < NUM_DIGITS)
    {
        str[idx] = UART_getchar(com);
        if (str[idx] >= '0' && str[idx] <= '9')
        {
            if (echo)
                UART_putchar(com, str[idx]);
            else
                UART_putchar(com, '*');
            idx++;
        }
    }
}

void eval_guess(char *guess,char *secrete, uint8_t *bulls, uint8_t *cows)
{
    // TO-DO: Compare guess and secrete and update bulls and cows accordingly
}

int main( void )
{
    char secrete[5] = {0};
    char guess[5] = {0};
    char cad[20];
    uint8_t cows, bulls;

    UART_Ini(0,12345,8,1,2);
    UART_Ini(2,115200,8,0,1);
    UART_Ini(3,115200,8,0,1);
    while(1) 
    {
        UART_getchar(0);
        UART_clrscr(0);

        UART_gotoxy(0,5,2);
        UART_setColor(0,YELLOW);
        UART_puts(0,"Capture the secrete 4 digits: ");
        
        UART_gotoxy(0,35,2);
        UART_setColor(0,GREEN);
        capture_digits(0, secrete, 0);

        UART_gotoxy(0,5,3);
        UART_setColor(0,YELLOW);
        UART_puts(0,"Guess the 4 digit sequence");
        UART_gotoxy(0,5,4);
        UART_puts(0,"Guess: \tBulls: \tCows:");

        UART_setColor(0,BLUE);
        for(uint8_t try = 0; try < MAX_NUM_TRIES; try++)
        {
            UART_gotoxy(0, 2, 5 + try);
            itoa(try,cad,10);
            UART_puts(0,cad);
            // Capture guess and evaluate against secrete
            UART_gotoxy(0, 5, 5 + try);
            capture_digits(0, guess, 0);
            eval_guess(guess, secrete, &bulls, &cows);
            // Print hints
            UART_putchar(0, '\t');
            itoa(bulls, cad, 10);
            UART_puts(0, cad);
            UART_putchar(0, '\t');
            itoa(cows, cad, 10);
            UART_puts(0, cad);

            if (bulls == NUM_DIGITS)
            {
                UART_puts(0, "You WIN!");
                try = MAX_NUM_TRIES;
            }
            else if (try == MAX_NUM_TRIES)
            {
                UART_puts(0, "You LOSE!");
            }
        }
    }

    return 0;
}