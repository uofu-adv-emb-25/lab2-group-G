/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;

// define interrupt priorities and stack sizes for our tasks we're going to create
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// blink the CYW43 led on the pico
void blink_task(__unused void *params) {
    // assert that the pico is running and initialized
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        // set led to value of `on`
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        // toggle the count every time except for multiples of 11? seems weird...
        if (count++ % 11) on = !on;
        vTaskDelay(500);
    }
}

// main task
void main_task(__unused void *params) {
    // create a new task named BlinkThread and assign the task to perform the method blink_task
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    char c;
    // while we can get a char from stdin
    while(c = getchar()) {
        // if the entered char is a lower case letter, put the lowercase letter - 32 in the ascii table
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        // else if the entered char is upper case, put the letter + 32 on the ascii table
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        // else just put the character
        else putchar(c);
    }
}

int main( void )
{
    // init the board
    stdio_init_all();
    // unused variable? No purpose?
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    // Create new main task
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    // Start the tasks
    vTaskStartScheduler();
    return 0;
}