#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <avr/io.h>

// MCU 클럭 설정 (심볼을 사용하거나 직접 지정)
#ifndef F_CPU
#define F_CPU 14745600UL
#endif
#define configCPU_CLOCK_HZ                 ( ( uint32_t ) F_CPU )

// 기본 커널 설정
#define configUSE_IDLE_HOOK                0
#define configUSE_TICK_HOOK                0
#define configUSE_PREEMPTION               1
#define configTICK_RATE_HZ                 ( ( TickType_t ) 1000 )
#define configUSE_TIME_SLICING             1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE            0

// 메모리 설정 (ATmega128은 SRAM이 4KB로 매우 작으므로 최적화가 필수입니다)
#define configMINIMAL_STACK_SIZE           ( ( uint16_t ) 85 )
#define configTOTAL_HEAP_SIZE              ( ( size_t ) 1500 )
#define configSUPPORT_DYNAMIC_ALLOCATION   1
#define configSUPPORT_STATIC_ALLOCATION    0
#define configAPPLICATION_ALLOCATED_HEAP    0

// 태스크 설정
#define configMAX_PRIORITIES               4
#define configMAX_TASK_NAME_LEN            8
#define configUSE_TASK_NOTIFICATIONS       1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES 1
#define configCHECK_FOR_STACK_OVERFLOW     0
#define configUSE_TRACE_FACILITY           0
#define configGENERATE_RUN_TIME_STATS      0
#define configUSE_STATS_FORMATTING_FUNCTIONS 0

// 타이머 및 기타 기능 정의
#define configUSE_16_BIT_TICKS             1
#define configIDLE_SHOULD_YIELD            1
#define configUSE_MUTEXES                  0
#define configUSE_RECURSIVE_MUTEXES        0
#define configUSE_COUNTING_SEMAPHORES      0
#define configUSE_TIMERS                   0
#define configUSE_EVENT_GROUPS             0
#define configUSE_STREAM_BUFFERS           0

// 사용할 API 활성화 (1: 활성화, 0: 비활성화)
#define INCLUDE_vTaskPrioritySet           1
#define INCLUDE_uxTaskPriorityGet          1
#define INCLUDE_vTaskDelete                1
#define INCLUDE_vTaskCleanUpResources      0
#define INCLUDE_vTaskSuspend               1
#define INCLUDE_xTaskDelayUntil            1
#define INCLUDE_vTaskDelay                 1
#define INCLUDE_xTaskGetSchedulerState     0
#define INCLUDE_xTaskGetCurrentTaskHandle  0
#define INCLUDE_uxTaskGetStackHighWaterMark 0
#define INCLUDE_uxTaskGetStackHighWaterMark2 0
#define INCLUDE_eTaskGetState              0
#define INCLUDE_xTaskAbortDelay            0
#define INCLUDE_xTaskGetHandle             0
#define INCLUDE_xTaskResumeFromISR         0

#endif /* FREERTOS_CONFIG_H */
