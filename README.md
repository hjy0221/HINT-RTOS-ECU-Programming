# 현대자동차그룹 HINT 1기
## 실시간 운영체제 기반 전자제어기 프로그래밍 실습

ATmega128과 FreeRTOS로 LED·LCD 제어, 태스크 스케줄링, 생산자·소비자 동기화를 학습하는 실습 저장소입니다. 문서는 현재 코드를 기준으로 작성했으며 강의자료 원본은 포함하지 않습니다.

## 실습 목록

| 프로젝트 | 학습 내용 | 실행 결과 |
| --- | --- | --- |
| [LCD_DEMO](LCD_DEMO/) | RTOS 없이 LCD 제어 | 문자열 표시와 지우기 반복 |
| [LED_DEMO](LED_DEMO/) | FreeRTOS 태스크 생성과 지연 | 500 ms마다 LED 상태 반전 |
| [RTOS_LCD_DEMO](RTOS_LCD_DEMO/) | LCD 출력 태스크 | 두 줄 문자열 표시와 지우기 반복 |
| [RTOS_LCD_LED_CTRL](RTOS_LCD_LED_CTRL/) | LED·LCD 태스크 동시 실행 | LED 점멸과 LCD 출력 |
| [RTOS_ISR_PROSUMER](RTOS_ISR_PROSUMER/README.md) | 임계 구역을 이용한 공유 버퍼 보호 | 생산자·소비자 각각 5개, 버퍼 개수를 LED·LCD로 표시 |
| [RTOS_ISR_QUEUE_DEMO](RTOS_ISR%20_QUEUE_DEMO/README.md) | FreeRTOS 큐와 ISR 세마포어 | 큐 개수를 LED·LCD로 표시하고 SW2 인터럽트로 큐 초기화 |

`LED_DEMO`는 FreeRTOS를 사용하는 프로젝트입니다. `RTOS_LCD_LED_CTRL`의 내부 솔루션명은 기존 파일명인 `RTOS_LCD_LED_CRTL`을 유지합니다.

그 밖에 [RTOS_LED_DEMO](RTOS_LED_DEMO/)는 독립 솔루션 없이 관련 소스를 보관하고, [FreeRTOS-LTS](FreeRTOS-LTS/)는 커널과 관련 라이브러리 원본을 담고 있습니다.

## 개발 환경

| 항목 | 설정 |
| --- | --- |
| MCU | ATmega128 |
| CPU 클럭 | 14.7456 MHz (`F_CPU=14745600UL`) |
| IDE | Atmel Studio 7 / Microchip Studio |
| 컴파일러 | AVR-GCC |
| 디바이스 팩 | ATmega_DFP 1.7.374 |
| 프로젝트 형식 | `.atsln` 솔루션, `.cproj` 프로젝트 |
| 주변장치 | LED, 문자 LCD |

실제 보드 클럭과 프로젝트 설정이 일치해야 합니다. 다른 개발 환경에서는 설치된 디바이스 팩과 도구 경로를 확인합니다.

## 시작하기

```powershell
git clone https://github.com/hjy0221/HINT-RTOS-ECU-Programming.git
cd HINT-RTOS-ECU-Programming
```

원하는 솔루션을 IDE에서 엽니다. 아래 경로는 저장소 루트 기준입니다.

| 실습 | 솔루션 경로 |
| --- | --- |
| 기본 LCD | [LCD_DEMO.atsln](LCD_DEMO/LCD_DEMO/LCD_DEMO.atsln) |
| RTOS LED | [LED_DEMO.atsln](LED_DEMO/LED_DEMO/LED_DEMO.atsln) |
| RTOS LCD | [RTOS_LCD_DEMO.atsln](RTOS_LCD_DEMO/RTOS_LCD_DEMO/RTOS_LCD_DEMO.atsln) |
| RTOS LED·LCD | [RTOS_LCD_LED_CRTL.atsln](RTOS_LCD_LED_CTRL/RTOS_LCD_LED_CTRL/RTOS_LCD_LED_CRTL.atsln) |
| 생산자·소비자 | [RTOS_ISR_PROSUMER.atsln](RTOS_ISR_PROSUMER/RTOS_ISR_PROSUMER/RTOS_ISR_PROSUMER.atsln) |
| 큐·인터럽트 | [RTOS_ISR _QUEUE_DEMO.atsln](RTOS_ISR%20_QUEUE_DEMO/RTOS_ISR%20_QUEUE_DEMO/RTOS_ISR%20_QUEUE_DEMO.atsln) |

1. 대상 디바이스가 ATmega128인지 확인하고 Debug 구성으로 빌드합니다.
2. 프로젝트의 `Debug` 폴더에 생성된 `.hex` 또는 `.elf`를 프로그래머로 보드에 기록합니다.
3. 실습 목록의 실행 결과를 확인합니다. ISR 실습의 세부 동작과 설정은 [공유 버퍼 실습](RTOS_ISR_PROSUMER/README.md)과 [큐 실습](RTOS_ISR%20_QUEUE_DEMO/README.md)을 참고합니다.

프로젝트와 `Source` 폴더의 상대 위치를 유지해야 헤더와 커널 소스를 찾을 수 있습니다. 새 빌드 산출물과 IDE 캐시는 `.gitignore`로 제외합니다.

## 하드웨어 연결

| 기능 | ATmega128 포트 |
| --- | --- |
| LED | PB0~PB7 |
| LCD 데이터 D0~D7 | PA0~PA7 |
| LCD RS | PG0 |
| LCD RW | PG1 |
| LCD E | PG2 |
| SW2 | PE4/INT4 |

LCD 전원·접지·명암·백라이트는 사용 중인 보드와 모듈의 핀 배치에 맞게 연결합니다. LED의 켜짐 논리는 보드 회로에 따라 다르며 ISR 실습은 Active Low를 기본으로 사용합니다.

## 코드 읽기

| 파일·폴더 | 역할 |
| --- | --- |
| 각 프로젝트의 `main.c` | 주변장치 초기화, 태스크 생성, 실습 동작 |
| `Source/include/FreeRTOSConfig.h` | 틱 주기, 힙, 스케줄링 등 커널 설정 |
| `Source/include/FreeRTOS.h`, `task.h` | FreeRTOS 및 태스크 API 선언 |
| `Source/portable/GCC/ATMega323` | AVR-GCC용 문맥 전환과 하드웨어 의존 처리 |
| `Source/tasks.c`, `list.c`, `queue.c` | 커널 구현 |
| `Source/heap_1.c` | 메모리 할당 구현, 해제는 지원하지 않음 |
| `Source/lcd.c`, `Source/include/lcd.h` | LCD 드라이버 |

위 배치는 RTOS LCD·통합·ISR 프로젝트 기준입니다. `LED_DEMO`는 커널이 `LED_DEMO/LED_DEMO/Source`에 있고 `FreeRTOSConfig.h`는 `main.c` 옆에 있습니다. 기본 LCD 프로젝트는 RTOS 커널을 사용하지 않습니다.

헤더는 컴파일러의 include 검색 경로로 참조하므로 `main.c`와 같은 폴더에 없어도 됩니다. 구현 파일은 `.cproj`의 컴파일 항목에도 등록되어야 합니다.

### 대기와 공유 데이터

- LED 태스크의 `vTaskDelay()`는 태스크를 Blocked 상태로 전환합니다.
- 기존 LCD 실습의 `_delay_ms()`는 CPU를 사용하는 바쁜 대기입니다. 선점될 수는 있지만 태스크를 Blocked 상태로 만들지는 않습니다.
- `RTOS_ISR_PROSUMER`는 짧은 임계 구역에서 공유 버퍼를 갱신하며 LCD 출력과 대기는 임계 구역 밖에서 수행합니다.
- `RTOS_ISR_QUEUE_DEMO`는 FreeRTOS 큐로 생산자·소비자를 연결하고, INT4 ISR에서는 이진 세마포어만 깨워 큐 초기화를 태스크 문맥에서 수행합니다.

힙과 스택 설정은 프로젝트마다 다릅니다. LED·LCD 통합 실습의 힙은 1,500바이트, 공유 버퍼 ISR 실습의 힙은 2,800바이트, 큐 ISR 실습의 힙은 3,000바이트입니다.

## 문제 해결

| 증상 | 확인할 항목 |
| --- | --- |
| `FreeRTOS.h` 또는 `task.h`를 찾지 못함 | 해당 프로젝트의 `Source/include` 검색 경로 |
| `portmacro.h`를 찾지 못함 | `portable/GCC/ATMega323` 검색 경로 |
| 태스크 함수의 undefined reference | 커널·포트·힙 구현 파일의 컴파일 등록 |
| LCD 함수의 undefined reference | `lcd.c` 등록 여부 |
| 디바이스 팩 경로 오류 | ATmega_DFP 설치 버전과 프로젝트 참조 |
| 경로 수정 후 같은 오류 | Clean 후 Rebuild |
| 태스크가 실행되지 않음 | 태스크 생성 결과, 힙·스택 여유, 보드 클럭 |
| LED·LCD 출력 이상 | 배선, LED 극성, LCD 명암 설정 |
| SW2를 눌러도 초기화되지 않음 | PE4/INT4 연결, 내부 풀업, EIMSK·EICRB 설정 |

빌드 성공과 실제 보드 검증은 별개입니다. `RTOS_ISR_PROSUMER`는 AVR-GCC 빌드와 보드 동작을 확인했습니다. `RTOS_ISR_QUEUE_DEMO`는 AVR-GCC 빌드와 AVRISP mkII 기록·Verify를 확인했으며, LCD·LED 변화와 SW2 초기화의 실제 관찰은 보드에서 별도 확인해야 합니다.

## 라이선스

FreeRTOS와 포함된 외부 라이브러리의 라이선스 및 저작권 고지는 각 소스와 라이선스 파일을 따릅니다.
