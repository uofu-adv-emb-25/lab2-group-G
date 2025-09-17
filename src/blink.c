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
#include "my_functions.h"

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
    int count = 0;
    while (true) {
        blink_led(&count);
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
        char result = shiftChar(c);
        putchar(result);
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