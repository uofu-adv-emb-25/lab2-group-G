#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

char shiftChar(char c)
{
    // if the entered char is a lower case letter, put the lowercase letter - 32 in the ascii table
    if (c <= 'z' && c >= 'a') return c - 32;
    // else if the entered char is upper case, put the letter + 32 on the ascii table
    else if (c >= 'A' && c <= 'Z') return c + 32;
    // else just put the character
    else return c;
}

bool blink_led(int* count, bool on)
{
    // set led to value of `on`
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
    // toggle the count every time except for multiples of 11? seems weird...
    if ((*count)++ % 11) return !on;
    return on;
}