#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "my_functions.h"
#include "pico/cyw43_arch.h"

// cyw43_arch_gpio_get CYW43_WL_GPIO_LED_PIN TEST_ASSERT_EQUAL_MESSAGE TEST_ASSERT_NOT_EQUAL_MESSAGE TEST_ASSERT_EQUAL('a' - 1, switch_case('a' - 1));

void setUp(void) {}

void tearDown(void) {}

void test_chars_are_shifted_correctly()
{
    char lower_inputs[] = {'a', 'b', 'c', 'd', 'e', 'z'};
    char upper_inputs[] = {'A', 'B', 'C', 'D', 'E', 'Z'};

    char lower_expected[] = {'A', 'B', 'C', 'D', 'E', 'Z'};
    char upper_expected[] = {'a', 'b', 'c', 'd', 'e', 'z'};

    char non_shifts[] = {'/', '2', '`', '\\', '~', '+'};

    for(int i = 0; i < (sizeof(lower_inputs)/sizeof(lower_inputs[0])); i++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(lower_expected[i], shiftChar(lower_inputs[i]), "lower character was not shifted properly");
        TEST_ASSERT_EQUAL_MESSAGE(upper_expected[i], shiftChar(upper_inputs[i]), "upper character was not shifted properly");
        TEST_ASSERT_EQUAL_MESSAGE(non_shifts[i], shiftChar(non_shifts[i]), "non-alpha character was shifted when it should not have");
    }
}

void test_led_toggles_and_increments_correctly()
{
    // Initialize testing context
    bool on = false;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    int count = 0;
    int prev_count = 0;
    bool prev_on = false;
    for(int i = 0; i<1000; i++) 
    {
        // Initialize prev values so we can test prev state
        prev_on = on;
        prev_count = count;

        // Blink the led
        on = blink_led(&count, on);

        // Test that count was increased
        TEST_ASSERT_EQUAL_MESSAGE(count, prev_count+1, "count for led state was not incremented properly");

        // Test that the led is correctly set
        TEST_ASSERT_EQUAL_MESSAGE(on, cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN), "LED was not set properly");

        // Test that non multiples of 11 toggle `on`
        if(count % 11 != 0)
        {
            TEST_ASSERT_NOT_EQUAL_MESSAGE(on, prev_on, "'on' was not toggled during a non multiple of 11");
        }
        // Test that multiples of 11 don't toggle
        else
        {
            TEST_ASSERT_EQUAL_MESSAGE(on, prev_on, "on was toggled during a multiple of 11");
        }
    }   
}

void test_variable_assignment()
{
    int x = 1;
    TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
}

void test_multiplication(void)
{
    int x = 30;
    int y = 6;
    int z = x / y;
    printf("testing");
    TEST_ASSERT_TRUE_MESSAGE(z == 5, "Multiplication of two integers returned incorrect value.");
}

int main (void)
{
    stdio_init_all();
    sleep_ms(30000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_variable_assignment);
    RUN_TEST(test_multiplication);
    RUN_TEST(test_chars_are_shifted_correctly);
    RUN_TEST(test_led_toggles_and_increments_correctly);
    sleep_ms(5000);
    while(1);
    return UNITY_END();
}
