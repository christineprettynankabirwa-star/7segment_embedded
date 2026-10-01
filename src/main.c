#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// 74HC595 Shift Register GPIO Pins
#define DATA_PIN  GPIO_NUM_23  // DS
#define CLOCK_PIN GPIO_NUM_18  // SH_CP
#define LATCH_PIN GPIO_NUM_21  // ST_CP

// Bit patterns for Common Cathode (Q7=DP, Q6=G, Q5=F, Q4=E, Q3=D, Q2=C, Q1=B, Q0=A)
const uint8_t display_sequence[] = {
    0b00000111, // '7'
    0b00111111, // '0'
    0b01011011, // '2'
    0b10000000  // '.'
};

// Function to shift out 8 bits to the 74HC595
void shift_out_byte(uint8_t data) {
    gpio_set_level(LATCH_PIN, 0);
    
    for (int i = 7; i >= 0; i--) {
        gpio_set_level(CLOCK_PIN, 0);
        gpio_set_level(DATA_PIN, (data >> i) & 0x01);
        gpio_set_level(CLOCK_PIN, 1);
    }
    
    gpio_set_level(LATCH_PIN, 1);
}

void app_main(void) {
    // Configure control pins as outputs
    gpio_reset_pin(DATA_PIN);
    gpio_reset_pin(CLOCK_PIN);
    gpio_reset_pin(LATCH_PIN);

    gpio_set_direction(DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLOCK_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LATCH_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        for (int i = 0; i < 4; i++) {
            shift_out_byte(display_sequence[i]);
            vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
        }
    }
}