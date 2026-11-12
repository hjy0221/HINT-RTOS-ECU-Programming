# 현대자동차그룹 HINT 1기

[日本語](README.ja.md) · [English](README.md) · [한국어](README.ko.md)

## 실시간 운영체제 기반 전자제어기 프로그래밍 실습

This is a practice repository for learning LED/LCD control, task scheduling, and producer/consumer synchronization with ATmega128 and FreeRTOS. The document was written based on the current code and does not include the original lecture materials.

## Lab List

| project | What you learn | execution result |
| --- | --- | --- |
| [LCD_DEMO](LCD_DEMO/) | LCD control without RTOS | Repeat display and erase of string |
| [LED_DEMO](LED_DEMO/) | FreeRTOS task creation and delay | LED state inversion every 500 ms |
| [RTOS_LCD_DEMO](RTOS_LCD_DEMO/) | LCD output task | Repeat display and erase of two-line string |
| [RTOS_LCD_LED_CTRL](RTOS_LCD_LED_CTRL/) | Simultaneous execution of LED·LCD tasks | LED blinking and LCD output |
| [RTOS_ISR_PROSUMER](RTOS_ISR_PROSUMER/README.md) | Shared buffer protection using critical sections | 5 each for producer and consumer, number of buffers displayed on LED and LCD |
| [RTOS_ISR_QUEUE_DEMO](RTOS_ISR%20_QUEUE_DEMO/README.md) | FreeRTOS queues and ISR semaphores | Displays the number of queues on LED/LCD and initializes the queue with SW2 interrupt |
| [GAME](GAME/) | LCD shooting game using queues and external interrupts | Move SW2·SW3, fire SW4, restart SW6 |
| [NEW_GAME](NEW_GAME/) | `GAME` Variants: Increased ammo, reload, and difficulty | Enemy speed increases based on score, ammo reloads automatically |

`LED_DEMO` is a project that uses FreeRTOS. The internal solution name of `RTOS_LCD_LED_CTRL` maintains the existing file name `RTOS_LCD_LED_CRTL`.

In addition, [RTOS_LED_DEMO](RTOS_LED_DEMO/) stores related sources without independent solutions, and [FreeRTOS-LTS](FreeRTOS-LTS/) contains the original kernel and related libraries.

## Development environment

| Item | Settings |
| --- | --- |
| MCU | ATmega128 |
| CPU clock | 14.7456 MHz (`F_CPU=14745600UL`) |
| IDE | Atmel Studio 7 / Microchip Studio |
| compiler | AVR-GCC |
| Device Pack | ATmega_DFP 1.7.374 |
| Project Format | `.atsln` solution, `.cproj` project |
| Peripherals | LED, character LCD |

Your actual board clock and project settings must match. In other development environments, check the installed device packs and toolpaths.

## Getting started

```powershell
git clone https://github.com/hjy0221/HINT-RTOS-ECU-Programming.git
cd HINT-RTOS-ECU-Programming
```

Open the desired solution in the IDE. The path below is based on the storage root.

| Practice | Solution Path |
| --- | --- |
| Basic LCD | [LCD_DEMO.atsln](LCD_DEMO/LCD_DEMO/LCD_DEMO.atsln) |
| RTOS LED | [LED_DEMO.atsln](LED_DEMO/LED_DEMO/LED_DEMO.atsln) |
| RTOS LCD | [RTOS_LCD_DEMO.atsln](RTOS_LCD_DEMO/RTOS_LCD_DEMO/RTOS_LCD_DEMO.atsln) |
| RTOS LED·LCD | [RTOS_LCD_LED_CRTL.atsln](RTOS_LCD_LED_CTRL/RTOS_LCD_LED_CTRL/RTOS_LCD_LED_CRTL.atsln) |
| Producer/Consumer | [RTOS_ISR_PROSUMER.atsln](RTOS_ISR_PROSUMER/RTOS_ISR_PROSUMER/RTOS_ISR_PROSUMER.atsln) |
| Queue/Interrupt | [RTOS_ISR _QUEUE_DEMO.atsln](RTOS_ISR%20_QUEUE_DEMO/RTOS_ISR%20_QUEUE_DEMO/RTOS_ISR%20_QUEUE_DEMO.atsln) |
| LCD gaming | [GAME.atsln](GAME/GAME/GAME.atsln) |
| LCD gaming variant | [NEW_GAME.atsln](NEW_GAME/NEW_GAME/NEW_GAME.atsln) |

1. Make sure the target device is ATmega128 and build with Debug configuration.
2. Write `.hex` or `.elf` created in the `Debug` folder of the project to the board as a programmer.
3. Check the execution results of the exercise list. For detailed operations and settings of the ISR lab, refer to [Shared Buffer Lab](RTOS_ISR_PROSUMER/README.md) and [Queue Lab](RTOS_ISR%20_QUEUE_DEMO/README.md).You need to keep the relative positions of your project and `Source` folders so that you can find the headers and kernel sources. Exclude new build artifacts and IDE caches with `.gitignore`.

## Hardware connection

| Features | ATmega128 port |
| --- | --- |
| LED | PB0~PB7 |
| LCD data D0~D7 | PA0~PA7 |
| LCD RS | PG0 |
| LCD RW | PG1 |
| LCD E | PG2 |
| SW2 | PE4/INT4 |
| SW3 | PE5/INT5 |
| SW4 | PE6/INT6 |
| SW6 | PE7/INT7 |

Connect the LCD power, ground, contrast, and backlight according to the pin layout of the board and module in use. The turn-on logic of the LED depends on the board circuitry, and the ISR lab uses Active Low as the default.

## Read code

| File·Folder | Role |
| --- | --- |
| `main.c` for each project | Peripheral device initialization, task creation, practice operation |
| `Source/include/FreeRTOSConfig.h` | Kernel settings such as tick cycle, heap, scheduling, etc. |
| `Source/include/FreeRTOS.h`, `task.h` | FreeRTOS and Task API Declaration |
| `Source/portable/GCC/ATMega323` | Context switching and hardware-dependent processing for AVR-GCC |
| `Source/tasks.c`, `list.c`, `queue.c` | Kernel Implementation |
| `Source/heap_1.c` | Implementation of memory allocation, freeing is not supported |
| `Source/lcd.c`, `Source/include/lcd.h` | LCD Driver |

The above arrangement is based on the RTOS LCD/integration/ISR project. `LED_DEMO` has its kernel next to `LED_DEMO/LED_DEMO/Source`, and `FreeRTOSConfig.h` has its kernel next to `main.c`. The default LCD project does not use the RTOS kernel.

The header is referenced by the compiler's include search path, so it doesn't have to be in the same folder as `main.c`. The implementation file must also be registered in the compilation entry of `.cproj`.

### Waiting and shared data

- `vTaskDelay()` of the LED task switches the task to Blocked state.
- `_delay_ms()` in the existing LCD practice is a busy standby using the CPU. It may be preempted, but it does not put the task in Blocked state.
- `RTOS_ISR_PROSUMER` updates the shared buffer in a short critical section, with LCD output and waiting outside the critical section.
- `RTOS_ISR_QUEUE_DEMO` connects producers and consumers with FreeRTOS queues, and in INT4 ISR, only binary semaphores are awakened and queue initialization is performed in the task context.
- `GAME` queues commands from INT4~INT7 button ISRs, and game tasks handle player movement, firing, and restarting.
- `NEW_GAME` modifies `GAME`, adding 3 rounds of ammo, automatic reload, and score-based enemy speed increase rules.

Heap and stack settings vary from project to project. The heap for the LED·LCD integration lab is 1,500 bytes, the heap for the shared buffer ISR lab is 2,800 bytes, and the heap for the queue ISR lab is 3,000 bytes.

## Troubleshooting

| Symptoms | Items to check |
| --- | --- |
| Didn't find `FreeRTOS.h` or `task.h` | `Source/include` search path for that project |
| `portmacro.h` not found | `portable/GCC/ATMega323` Search Path |
| undefined reference in task function | Compilation registration of kernel, port, and heap implementation files |
| undefined reference in LCD function | `lcd.c` registration status |
| device pack path error | ATmega_DFP installation version and project reference |
| Same error after modifying the path | Clean and Rebuild |
| Task does not run | Task creation result, heap/stack free space, board clock |
| LED·LCD output error | Wiring, LED polarity, LCD contrast settings |
| Does not reset when pressing SW2 | PE4/INT4 connection, internal pull-up, EIMSK·EICRB settings |Build success is separate from actual board verification. `RTOS_ISR_PROSUMER` verified AVR-GCC build and board operation. `RTOS_ISR_QUEUE_DEMO` has confirmed the AVR-GCC build and AVRISP mkII record/Verify, and actual observation of LCD/LED changes and SW2 initialization must be confirmed separately on the board.

## License

The license and copyright notices for FreeRTOS and included external libraries follow their respective source and license files.
