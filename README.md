# 현대자동차그룹 HINT 1기

## 실시간 운영체제 기반 전자제어기 프로그래밍 실습

ATmega128과 FreeRTOS를 이용하여 GPIO, LCD 출력, 태스크 생성 및 스케줄링을 학습하는 실습 저장소입니다. 기본 LCD 제어에서 시작하여 LED 태스크, LCD 태스크, LED·LCD 동시 제어로 확장합니다.

이 문서는 저장소에 포함된 실습 코드와 프로젝트 설정을 기준으로 작성했습니다.

## 개발 환경

| 항목 | 설정 |
| --- | --- |
| 대상 MCU | ATmega128 |
| CPU 클럭 | 14.7456 MHz (`F_CPU=14745600UL`) |
| 개발 도구 | Atmel Studio 7 / Microchip Studio의 AVR 프로젝트 환경 |
| 컴파일러 | AVR-GCC |
| 프로젝트 형식 | `.atsln` 솔루션, `.cproj` 프로젝트 |
| 운영체제 | FreeRTOS |
| 주변장치 | LED, 문자 LCD |

프로젝트는 ATmega_DFP `1.7.374`를 참조합니다. 다른 환경에서는 설치된 디바이스 팩과 도구 경로를 확인해야 합니다. 실제 보드 클럭도 프로젝트 설정과 일치해야 합니다.

## 실습 구성

| 폴더 | 실습 내용 | 주요 동작 |
| --- | --- | --- |
| [LCD_DEMO](LCD_DEMO/) | 기본 LCD 제어 | 문자열 표시와 화면 지우기 반복 |
| [LED_DEMO](LED_DEMO/) | FreeRTOS LED 제어 | LED 태스크에서 500 ms마다 PORTB 반전 |
| [RTOS_LCD_DEMO](RTOS_LCD_DEMO/) | FreeRTOS LCD 제어 | LCD 태스크에서 문자열 표시와 지우기 반복 |
| [RTOS_LCD_LED_CTRL](RTOS_LCD_LED_CTRL/) | LED·LCD 통합 제어 | 별도 태스크로 LED와 LCD 제어 |
| [RTOS_LED_DEMO](RTOS_LED_DEMO/) | 관련 소스 보관 | 현재 폴더에는 Source가 있으며 독립 솔루션은 없음 |
| [FreeRTOS-LTS](FreeRTOS-LTS/) | FreeRTOS 관련 원본 소스 | 커널 및 관련 라이브러리 참고 |

`LED_DEMO`는 이름과 달리 현재 FreeRTOS를 사용하는 LED 실습입니다. 통합 실습의 내부 솔루션·프로젝트 이름은 기존 파일명인 `RTOS_LCD_LED_CRTL`을 사용합니다. 현재 통합 실습에는 LED와 LCD 태스크만 있으며 팬 제어 코드는 포함하지 않습니다.

## 하드웨어 연결

| 기능 | ATmega128 포트 | 코드 기준 |
| --- | --- | --- |
| LED | PB0~PB7 | PORTB 전체를 출력으로 설정하고 상태 반전 |
| LCD 데이터 | PA0~PA7 | 8비트 데이터 버스 |
| LCD RS | PG0 | 명령과 표시 데이터 선택 |
| LCD RW | PG1 | 읽기·쓰기 제어 |
| LCD E | PG2 | Enable 펄스 |

LCD 전원, 접지, 명암 조절 및 백라이트는 사용 중인 보드와 LCD 모듈의 핀 배치를 확인하여 연결합니다. LED의 실제 켜짐 논리는 보드 회로에 따라 달라질 수 있습니다.

## 프로젝트 열기와 빌드

1. 저장소를 내려받습니다.

   ```powershell
   git clone https://github.com/hjy0221/HINT-RTOS-ECU-Programming.git
   cd HINT-RTOS-ECU-Programming
   ```

2. 아래에서 원하는 솔루션을 Atmel Studio 또는 Microchip Studio로 엽니다.

   | 실습 | 솔루션 경로 |
   | --- | --- |
   | 기본 LCD | `LCD_DEMO/LCD_DEMO/LCD_DEMO.atsln` |
   | RTOS LED | `LED_DEMO/LED_DEMO/LED_DEMO.atsln` |
   | RTOS LCD | `RTOS_LCD_DEMO/RTOS_LCD_DEMO/RTOS_LCD_DEMO.atsln` |
   | RTOS LED·LCD | `RTOS_LCD_LED_CTRL/RTOS_LCD_LED_CTRL/RTOS_LCD_LED_CRTL.atsln` |

3. 대상 디바이스가 ATmega128인지 확인하고 Debug 구성으로 빌드합니다.
4. 빌드 결과의 `.hex` 또는 `.elf`를 사용 중인 프로그래머로 보드에 기록합니다. 프로그래머 종류와 연결 인터페이스는 실제 장비에 맞게 선택합니다.
5. LCD에 `ATmega128`, `Hello LCD!`가 표시되는지, LED 상태가 반복해서 바뀌는지 확인합니다.

폴더의 상대 위치를 유지해야 프로젝트가 `Source` 아래의 헤더와 커널 소스를 찾을 수 있습니다. 빌드 산출물은 각 프로젝트의 `Debug` 폴더에 생성되며 새로 추가하는 프로젝트의 산출물과 IDE 캐시는 Git 관리에서 제외합니다.

## 소스 구조와 헤더 참조

LCD 통합 실습의 대표 구조는 다음과 같습니다.

```text
RTOS_LCD_LED_CTRL/
├── RTOS_LCD_LED_CTRL/
│   ├── RTOS_LCD_LED_CRTL.atsln
│   └── RTOS_LCD_LED_CRTL/
│       ├── RTOS_LCD_LED_CRTL.cproj
│       └── main.c
└── Source/
    ├── include/
    │   ├── FreeRTOS.h
    │   ├── FreeRTOSConfig.h
    │   ├── task.h
    │   └── lcd.h
    ├── portable/GCC/ATMega323/
    │   ├── port.c
    │   └── portmacro.h
    ├── heap_1.c
    ├── lcd.c
    ├── list.c
    ├── queue.c
    └── tasks.c
```

`FreeRTOS.h`와 `task.h`는 `main.c`와 같은 폴더에 없어도 됩니다. 프로젝트의 컴파일러 include 검색 경로에 `Source/include`가 등록되어 있으므로 `#include "FreeRTOS.h"`로 사용할 수 있습니다. 포팅 계층의 헤더를 찾기 위해 `Source/portable/GCC/ATMega323`도 검색 경로에 포함합니다.

`portable/GCC/ATMega323`은 이 실습에서 사용하는 AVR-GCC 포팅 계층입니다. 태스크 문맥 전환과 하드웨어 의존 처리를 담당하며, 폴더 이름과 프로젝트의 대상 디바이스 설정은 구분해서 확인해야 합니다. 커널 소스는 `.cproj`의 컴파일 항목에도 등록되어야 링크됩니다.

`LED_DEMO`는 별도 배치를 사용합니다. 커널은 `LED_DEMO/LED_DEMO/Source`에 있고 `FreeRTOSConfig.h`는 해당 프로젝트의 `main.c` 옆에 있습니다.

## 태스크 동작

통합 실습의 `main()`은 우선순위 1로 `LED_TSK`, `LCD_TSK`를 생성하고 `vTaskStartScheduler()`를 호출합니다.

| 태스크 | 수행 내용 | 대기 방식 |
| --- | --- | --- |
| `vLEDBlinkTask` | `PORTB ^= 0xFF`로 출력 반전 | `vTaskDelay(pdMS_TO_TICKS(500))` |
| `vLCDPrintTask` | 두 줄 문자열 출력, 화면 지우기 반복 | 각 단계에서 `_delay_ms(500)` |

LED는 약 500 ms 간격으로 상태가 바뀌므로 켜짐과 꺼짐을 한 번씩 거치는 주기는 약 1초입니다. `vTaskDelay()`는 태스크를 지정한 틱 동안 Blocked 상태로 전환하므로 다른 태스크가 실행될 수 있습니다.

현재 LCD 태스크의 `_delay_ms()`는 CPU를 사용하는 바쁜 대기입니다. 선점형 스케줄러가 실행 중이면 다른 태스크로 전환될 수 있지만, `vTaskDelay()`처럼 태스크를 Blocked 상태로 만들지는 않습니다. 따라서 실제 벽시계 기준 표시 간격은 다른 태스크 실행 시간의 영향을 받을 수 있습니다.

통합 실습의 `FreeRTOSConfig.h` 주요 설정은 다음과 같습니다.

| 설정 | 값 | 의미 |
| --- | --- | --- |
| `configUSE_PREEMPTION` | 1 | 선점형 스케줄링 |
| `configUSE_TIME_SLICING` | 1 | 같은 우선순위 태스크의 시간 분할 |
| `configTICK_RATE_HZ` | 1000 | 초당 1000틱으로 설정 |
| `configMINIMAL_STACK_SIZE` | 85 | 최소 태스크 스택 크기 설정 |
| `configTOTAL_HEAP_SIZE` | 1500 | FreeRTOS 힙 크기, 바이트 단위 |
| `configMAX_PRIORITIES` | 4 | 태스크 우선순위 단계 수 |

`heap_1.c`는 동적 할당을 제공하지만 할당한 메모리를 해제하지 않는 방식입니다. 현재처럼 시작 시 태스크를 생성하고 계속 실행하는 구조에 맞춰 사용합니다.

## 빌드 문제 확인

| 증상 | 확인할 항목 |
| --- | --- |
| `FreeRTOS.h` 또는 `task.h`를 찾지 못함 | 해당 프로젝트의 `Source/include` 검색 경로 |
| `portmacro.h`를 찾지 못함 | `portable/GCC/ATMega323` 검색 경로 |
| 태스크 함수의 undefined reference | `tasks.c`, `list.c`, `queue.c`, `port.c`, `heap_1.c` 등록 여부 |
| LCD 함수의 undefined reference | `lcd.c` 등록 여부 |
| 디바이스 팩 경로 오류 | ATmega_DFP 설치 버전과 프로젝트 참조 |
| 경로 수정 후에도 같은 오류 | Clean 후 Rebuild하여 생성된 Makefile 갱신 |
| 실행 후 태스크가 동작하지 않음 | 태스크 생성 결과, 힙·스택 여유, 보드 클럭과 배선 |

빌드 성공과 실제 보드 동작 확인은 별개입니다. 다운로드 후 LED와 LCD 동작을 장비에서 확인해야 합니다.

## 참고 자료

강의 PDF는 `docs/lectures` 아래에 이론과 실습으로 나누어 보관합니다. 원본 파일명과 내용은 유지했습니다.

### 이론 자료

| 순서 | 자료 |
| --- | --- |
| 0장 | [RTOS 기본 개념](docs/lectures/theory/0장.%20RTOS%20기본%20개념.pdf) |
| 1장 | [1장 강의자료](docs/lectures/theory/1장.pdf) |
| 2장 | [2장 강의자료](docs/lectures/theory/2장.pdf) |

### 실습 자료

| 순서 | 자료 | 관련 코드 |
| --- | --- | --- |
| 1 | [LCD Demo](docs/lectures/practice/1.%20LCD%20Demo.pdf) | [LCD_DEMO](LCD_DEMO/) |
| 2 | [LED Demo](docs/lectures/practice/2.%20LED%20Demo.pdf) | 기본 LED 실습 자료 |
| 3 | [RTOS_LED Demo](docs/lectures/practice/3.%20RTOS_LED%20Demo.pdf) | [LED_DEMO](LED_DEMO/) |
| 4 | [RTOS_LCD Demo](docs/lectures/practice/4.%20RTOS_LCD%20Demo.pdf) | [RTOS_LCD_DEMO](RTOS_LCD_DEMO/) |
| 5 | [RTOS_LCD_LED 제어 실습](docs/lectures/practice/5.%20RTOS_LCD_LED%20제어%20실습.pdf) | [RTOS_LCD_LED_CTRL](RTOS_LCD_LED_CTRL/) |

README는 현재 구현을 설명하므로 강의자료의 예제와 차이가 있을 수 있습니다. 강의자료의 저작권은 원저작자에게 있으며, FreeRTOS와 포함된 외부 라이브러리의 라이선스 및 저작권 고지는 각 소스와 라이선스 파일을 따릅니다.
