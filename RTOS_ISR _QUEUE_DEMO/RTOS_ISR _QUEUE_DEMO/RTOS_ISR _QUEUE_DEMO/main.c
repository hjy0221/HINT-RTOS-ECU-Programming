#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"

#define QUEUE_SIZE 8
#define NUM_PRODUCERS 5
#define NUM_CONSUMERS 5
#define LED_ACTIVE_LOW 1
#define TASK_STACK_SIZE 160

static QueueHandle_t xDataQueue;
static SemaphoreHandle_t xClearSemaphore;
static uint8_t producer_id[NUM_PRODUCERS] = {0, 1, 2, 3, 4};
static uint8_t consumer_id[NUM_CONSUMERS] = {0, 1, 2, 3, 4};

static uint16_t random_delay(uint16_t *seed, uint16_t min, uint16_t max)
{
    *seed = (*seed * 25173U) + 13849U;
    return min + (*seed % (max - min + 1U));
}

static void update_led(void)
{
    UBaseType_t count;
    uint8_t pattern;
    /* Keep the queue snapshot and PORTB write together across task switches. */
    taskENTER_CRITICAL();
    count = uxQueueMessagesWaiting(xDataQueue);
    pattern = count >= QUEUE_SIZE ? 0xFF : (uint8_t)((1U << count) - 1U);
#if LED_ACTIVE_LOW
    PORTB = (uint8_t)~pattern;
#else
    PORTB = pattern;
#endif
    taskEXIT_CRITICAL();
}

static void vProducerTask(void *parameters)
{
    uint8_t id = *((uint8_t *)parameters);
    uint8_t data = 0;
    uint16_t seed = 1000U + ((uint16_t)id * 300U);
    for (;;) {
        uint16_t delay = random_delay(&seed, 1000, 3000);
        vTaskDelay(pdMS_TO_TICKS(delay));
        if (++data > 8) {
            data = 1;
        }
        if (xQueueSend(xDataQueue, &data, portMAX_DELAY) == pdPASS) {
            update_led();
        }
    }
}

static void vConsumerTask(void *parameters)
{
    uint8_t id = *((uint8_t *)parameters);
    uint8_t data;
    uint16_t seed = 30000U + ((uint16_t)id * 200U);
    for (;;) {
        uint16_t delay = random_delay(&seed, 3000, 6000);
        vTaskDelay(pdMS_TO_TICKS(delay));
        if (xQueueReceive(xDataQueue, &data, portMAX_DELAY) == pdPASS) {
            (void)data;
            update_led();
        }
    }
}

static void vLCDTask(void *parameters)
{
    (void)parameters;
    for (;;) {
        UBaseType_t count = uxQueueMessagesWaiting(xDataQueue);
        lcd_gotoxy(0, 0);
        lcd_string("Queue Count:    ");
        lcd_gotoxy(0, 1);
        lcd_string("Count = ");
        lcd_data(count <= 9 ? '0' + (uint8_t)count : '?');
        lcd_string("       ");
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static void vQueueClearTask(void *parameters)
{
    (void)parameters;
    for (;;) {
        if (xSemaphoreTake(xClearSemaphore, portMAX_DELAY) == pdTRUE) {
            xQueueReset(xDataQueue);
            update_led();
        }
    }
}

static void sw2_interrupt_init(void)
{
    DDRE &= ~(1 << PE4);
    PORTE |= (1 << PE4);
    EICRB = (EICRB & ~((1 << ISC41) | (1 << ISC40))) | (1 << ISC41);
    EIFR = (1 << INTF4);
    EIMSK |= (1 << INT4);
}

ISR(INT4_vect)
{
    BaseType_t higher_priority_task_woken = pdFALSE;
    xSemaphoreGiveFromISR(xClearSemaphore, &higher_priority_task_woken);
    /* This AVR port has no portYIELD_FROM_ISR; the next tick schedules Clear. */
}

static void stop_on_error(void)
{
    cli();
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
    cli();
    DDRB = 0xFF;
#if LED_ACTIVE_LOW
    PORTB = 0xFF;
#else
    PORTB = 0x00;
#endif
    lcd_init();
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_string("Queue Count:");
    lcd_gotoxy(0, 1);
    lcd_string("Count = 0");

    xDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    xClearSemaphore = xSemaphoreCreateBinary();
    if (xDataQueue == NULL || xClearSemaphore == NULL) {
        stop_on_error();
    }
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
    if (xTaskCreate(vQueueClearTask, "Clear", TASK_STACK_SIZE, NULL, 2, NULL) != pdPASS ||
        xTaskCreate(vLCDTask, "LCD", TASK_STACK_SIZE, NULL, 1, NULL) != pdPASS) {
        stop_on_error();
    }
    sw2_interrupt_init();
    vTaskStartScheduler();
    stop_on_error();
    return 0;
}
