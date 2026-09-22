#include <avr/io.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

#define BUFFER_SIZE 8
#define NUM_PRODUCERS 5
#define NUM_CONSUMERS 5
#define LED_ACTIVE_LOW 1
#define TASK_STACK_SIZE 128

static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail, count;
static uint8_t producer_id[NUM_PRODUCERS] = {0, 1, 2, 3, 4};
static uint8_t consumer_id[NUM_CONSUMERS] = {0, 1, 2, 3, 4};

static uint16_t random_delay(uint16_t *seed)
{
    *seed = (*seed * 25173U) + 13849U;
    return 2000U + (*seed % 2001U);
}

/* Called inside the critical section or before scheduling starts. */
static void update_led(void)
{
    uint8_t n = count;
    uint8_t pattern = n >= BUFFER_SIZE ? 0xFF : (uint8_t)((1U << n) - 1U);
#if LED_ACTIVE_LOW
    PORTB = (uint8_t)~pattern;
#else
    PORTB = pattern;
#endif
}

static void vLCDTask(void *pvParameters)
{
    (void)pvParameters;
    for (;;) {
        uint8_t n;
        taskENTER_CRITICAL();
        n = count;
        taskEXIT_CRITICAL();
        lcd_gotoxy(0, 1);
        lcd_string("Count = ");
        lcd_data(n <= 9 ? '0' + n : '?');
        lcd_string("       ");
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static void vProducerTask(void *pvParameters)
{
    uint8_t id = *((uint8_t *)pvParameters);
    uint8_t data = 0;
    uint16_t seed = 1000U + ((uint16_t)id * 300U);
    for (;;) {
        uint16_t delay_time = random_delay(&seed);
        vTaskDelay(pdMS_TO_TICKS(delay_time));
        data++;
        /* Check and update the buffer as one indivisible operation. */
        taskENTER_CRITICAL();
        if (count < BUFFER_SIZE) {
            uint8_t temp_head = head;
            uint8_t temp_count = count;
            buffer[temp_head] = (id * 10U) + data;
            head = (temp_head + 1U) % BUFFER_SIZE;
            temp_count++;
            count = temp_count;
            update_led();
        }
        taskEXIT_CRITICAL();
    }
}

static void vConsumerTask(void *pvParameters)
{
    uint8_t id = *((uint8_t *)pvParameters);
    uint16_t seed = 30000U + ((uint16_t)id * 200U);
    for (;;) {
        uint16_t delay_time = random_delay(&seed);
        vTaskDelay(pdMS_TO_TICKS(delay_time));
        taskENTER_CRITICAL();
        if (count > 0) {
            uint8_t temp_tail = tail;
            uint8_t temp_count = count;
            uint8_t data = buffer[temp_tail];
            (void)data;
            tail = (temp_tail + 1U) % BUFFER_SIZE;
            temp_count--;
            count = temp_count;
            update_led();
        }
        taskEXIT_CRITICAL();
    }
}

static void stop_on_error(void)
{
    taskDISABLE_INTERRUPTS();
#if LED_ACTIVE_LOW
    PORTB = 0x00;
#else
    PORTB = 0xFF;
#endif
    for (;;) {}
}

int main(void)
{
    uint8_t i;
    DDRB = 0xFF;
    update_led();
    lcd_init();
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_string("Buffer Count:");
    for (i = 0; i < NUM_PRODUCERS; i++) {
        if (xTaskCreate(vProducerTask, "Prod", TASK_STACK_SIZE,
                        &producer_id[i], 1, NULL) != pdPASS) {
            stop_on_error();
        }
    }
    for (i = 0; i < NUM_CONSUMERS; i++) {
        if (xTaskCreate(vConsumerTask, "Cons", TASK_STACK_SIZE,
                        &consumer_id[i], 1, NULL) != pdPASS) {
            stop_on_error();
        }
    }
    if (xTaskCreate(vLCDTask, "LCD", TASK_STACK_SIZE, NULL, 1, NULL) != pdPASS) {
        stop_on_error();
    }
    vTaskStartScheduler();
    stop_on_error();
    return 0;
}
