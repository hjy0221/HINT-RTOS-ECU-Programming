# 현대자동차그룹 HINT 1기
## 실시간 운영체제 기반 전자제어기 프로그래밍 실습

[日本語](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%E6%97%A5%E6%9C%AC%E8%AA%9E) · [English](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#english) · [한국어](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%ED%95%9C%EA%B5%AD%EC%96%B4)

## 日本語

[日本語](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%E6%97%A5%E6%9C%AC%E8%AA%9E) · [English](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#english) · [한국어](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%ED%95%9C%EA%B5%AD%EC%96%B4)

ATmega128とFreeRTOSでLED・LCD制御、タスクスケジューリング、生産者・消費者同期を学習する実習リポジトリです。文書は現在のコードに基づいて書かれており、講義資料のソースは含まれていません。

## 練習リスト

|プロジェクト|学習内容実行結果|
| --- | --- | --- |
| [LCD_DEMO]（LCD_DEMO /）| RTOSなしのLCD制御|文字列の表示と消去の繰り返し
| [LED_DEMO]（LED_DEMO /）| FreeRTOSタスクの作成と遅延500msごとのLED状態反転|
| [RTOS_LCD_DEMO]（RTOS_LCD_DEMO /）| LCD出力タスク| 2行の文字列を表示して消去を繰り返します。
| [RTOS_LCD_LED_CTRL]（RTOS_LCD_LED_CTRL /）| LED・LCDタスク同時実行LED点滅とL​​CD出力|
| [RTOS_ISR_PROSUMER]（RTOS_ISR_PROSUMER / README.md）|臨界ゾーンを使用した共有バッファ保護生産者・消費者それぞれ5個、バッファ数をLED・LCDで表示
| [RTOS_ISR_QUEUE_DEMO]（RTOS_ISR％20_QUEUE_DEMO / README.md）| FreeRTOSキューとISRセマフォ|キュー数をLED・LCDで表示し、SW2割り込みでキュー初期化
| [GAME](GAME/) |キューと外部割り込みによるLCDシューティングゲーム| SW2・SW3移動、SW4発射、SW6再起動
| [NEW_GAME]（NEW_GAME /）| `GAME`変形：弾薬・再装填・難易度増加スコアに応じて敵の速度が増加、弾薬の自動リロード

`LED_DEMO`はFreeRTOSを使用するプロジェクトです。 `RTOS_LCD_LED_CTRL`の内部ソリューション名は、既存のファイル名である`RTOS_LCD_LED_CRTL`を保持します。

他に、[RTOS_LED_DEMO](RTOS_LED_DEMO/) は独立したソリューションなしで関連ソースを保持し、[FreeRTOS-LTS](FreeRTOS-LTS/) はカーネルと関連ライブラリソースを含んでいます。

## 開発環境

|アイテム|設定|
| --- | --- |
| MCU | ATmega128 |
| CPUクロック| 14.7456 MHz（`F_CPU=14745600UL`）|
| IDE | Atmel Studio 7 / Microchip Studio |
|コンパイラ| AVR-GCC |
|デバイスパックATmega_DFP 1.7.374 |
|プロジェクトフォーマット| `.atsln`ソリューション、`.cproj`プロジェクト|
|周辺機器LED、文字LCD |

実際のボードクロックとプロジェクト設定が一致する必要があります。他の開発環境では、インストールされているデバイスパックとツールパスを確認してください。

## はじめに

```powershell
git clone https://github.com/hjy0221/HINT-RTOS-ECU-Programming.git
cd HINT-RTOS-ECU-Programming
```

目的のソリューションをIDEで開きます。以下のパスはストレージルートベースです。

|練習ソリューションパス|
| --- | --- |
|基本LCD | [LCD_DEMO.atsln]（LCD_DEMO/LCD_DEMO/LCD_DEMO.atsln）|
| RTOS LED | [LED_DEMO.atsln]（LED_DEMO/LED_DEMO/LED_DEMO.atsln）|
| RTOS LCD | [RTOS_LCD_DEMO.atsln]（RTOS_LCD_DEMO/RTOS_LCD_DEMO/RTOS_LCD_DEMO.atsln）|
| RTOS LED・LCD | [RTOS_LCD_LED_CRTL.atsln]（RTOS_LCD_LED_CTRL / RTOS_LCD_LED_CTRL / RTOS_LCD_LED_CRTL.atsln）|
|生産者・消費者[RTOS_ISR_PROSUMER.atsln]（RTOS_ISR_PROSUMER/RTOS_ISR_PROSUMER/RTOS_ISR_PROSUMER.atsln）|
|キュー・割り込み[RTOS_ISR _QUEUE_DEMO.atsln]（RTOS_ISR％20_QUEUE_DEMO / RTOS_ISR％20_QUEUE_DEMO / RTOS_ISR％20_QUEUE_DEMO.atsln）|
| LCDゲーム| [GAME.atsln](GAME/GAME/GAME.atsln) |
| LCDゲームバリエーション[NEW_GAME.atsln]（NEW_GAME/NEW_GAME/NEW_GAME.atsln）|

1. ターゲットデバイスがATmega128であることを確認し、Debug設定でビルドします。
2.プロジェクトの`Debug`フォルダに作成された`.hex`または`.elf`をプログラマとしてボードに書き込みます。
3. 実習リストの実行結果を確認します。 ISR実習の詳細な動作と設定については、[共有バッファの実習]（RTOS_ISR_PROSUMER/README.md）と[キューの実習]（RTOS_ISR%20_QUEUE_DEMO/README.md）を参照してください。プロジェクトと`Source`フォルダの相対位置を維持する必要があり、ヘッダーとカーネルソースを見つけることができます。新しいビルド出力とIDEキャッシュは`.gitignore`として除外されます。

## ハードウェア接続

|機能ATmega128ポート|
| --- | --- |
| LED | PB0〜PB7 |
| LCDデータD0〜D7 | PA0〜PA7 |
| LCD RS | PG0 |
| LCD RW | PG1 |
| LCD E | PG2 |
| SW2 | PE4 / INT4 |
| SW3 | PE5 / INT5 |
| SW4 | PE6 / INT6 |
| SW6 | PE7 / INT7 |

LCD電源・接地・コントラスト・バックライトは、使用中のボードとモジュールのピン配置に合わせて接続します。 LEDの点灯ロジックはボード回路によって異なり、ISRの実践はActive Lowをベースにしています。

## コードを読む

|ファイル・フォルダ|役割|
| --- | --- |
|各プロジェクトの`main.c` |周辺機器の初期化、タスク生成、実践動作
| `Source/include/FreeRTOSConfig.h` |ティックサイクル、ヒープ、スケジューリングなどのカーネル設定|
| `Source/include/FreeRTOS.h`、`task.h` | FreeRTOSとタスクAPI宣言|
| `Source/portable/GCC/ATMega323` | AVR-GCCのコンテキスト切り替えとハードウェア依存処理
| `Source/tasks.c`、`list.c`、`queue.c` |カーネルの実装
| `Source/heap_1.c` |メモリ割り当ての実装、解放はサポートされていません。
| `Source/lcd.c`、`Source/include/lcd.h` | LCDドライバ|

上記の配置はRTOS LCD・統合・ISRプロジェクト基準です。 `LED_DEMO`はカーネルが`LED_DEMO/LED_DEMO/Source`にあり、`FreeRTOSConfig.h`は`main.c`の隣にあります。デフォルトのLCDプロジェクトはRTOSカーネルを使用しません。

ヘッダーはコンパイラのインクルード検索パスとして参照されるため、`main.c`と同じフォルダに配置する必要はありません。実装ファイルは`.cproj`のコンパイル項目にも登録する必要があります。

### 待機と共有データ

- LED タスクの `vTaskDelay()` はタスクを Blocked 状態に切り替えます。
- 従来のLCD実習の`_delay_ms()`は、CPUを使用する忙しい待機です。プリエンプトは可能ですが、タスクをブロック状態にすることはありません。
- `RTOS_ISR_PROSUMER`は短いしきい値ゾーンで共有バッファを更新し、LCD出力と待機はしきい値ゾーンの外で実行します。
- `RTOS_ISR_QUEUE_DEMO`はFreeRTOSキューで生産者・消費者を連結し、INT4 ISRではバイナリセマフォだけが起きてキュー初期化をタスクコンテキストで行います。
- `GAME`はINT4～INT7ボタンISRからコマンドをキューに送り、ゲームタスクがプレイヤーの移動・発射・再起動を処理します。
- `NEW_GAME`は`GAME`を変形し、弾薬3発、自動再装填、スコアベースの速度増加ルールを追加しました。

ヒープとスタックの設定はプロジェクトによって異なります。 LED・LCD統合実習のヒープは1,500バイト、共有バッファISR実習のヒープは2,800バイト、キューISR実習のヒープは3,000バイトです。

## トラブルシューティング

|症状確認する項目
| --- | --- |
| `FreeRTOS.h`または`task.h`が見つかりません。そのプロジェクトの`Source/include`検索パス|
| `portmacro.h`が見つかりません。 `portable/GCC/ATMega323`検索パス|
|タスク関数のundefined reference |カーネル・ポート・ヒープ実装ファイルのコンパイル登録
| LCD関数の未定義参照| `lcd.c`を登録するかどうか|
|デバイスパックパスエラー| ATmega_DFPインストールバージョンとプロジェクトリファレンス|
|パス修正後の同じエラーClean後Rebuild |
|タスクが実行されていません。タスク生成結果、ヒープ・スタック余裕、ボードクロック
| LED・LCD出力異常配線、LED極性、LCDコントラスト設定|
| SW2を押しても初期化されません。 PE4/INT4接続、内部プルアップ、EIMSK・EICRB設定ビルドの成功と実際のボード検証は別々です。 `RTOS_ISR_PROSUMER`はAVR-GCCビルドとボードの動作を確認しました。 `RTOS_ISR_QUEUE_DEMO`はAVR-GCCビルドとAVRISP mkII記録・Verifyを確認し、LCD・LED変化とSW2初期化の実際の観察はボードで別途確認する必要があります。

## ライセンス

FreeRTOSを含む外部ライブラリのライセンスと著作権通知は、各ソースとライセンスファイルに従います。

## English

[日本語](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%E6%97%A5%E6%9C%AC%E8%AA%9E) · [English](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#english) · [한국어](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%ED%95%9C%EA%B5%AD%EC%96%B4)

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

## 한국어

[日本語](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%E6%97%A5%E6%9C%AC%E8%AA%9E) · [English](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#english) · [한국어](https://github.com/hjy0221/HINT-RTOS-ECU-Programming#%ED%95%9C%EA%B5%AD%EC%96%B4)

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
| [GAME](GAME/) | 큐와 외부 인터럽트를 이용한 LCD 슈팅 게임 | SW2·SW3 이동, SW4 발사, SW6 재시작 |
| [NEW_GAME](NEW_GAME/) | `GAME` 변형: 탄약·재장전·난이도 증가 | 점수에 따라 적 속도 증가, 탄약 자동 재장전 |

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
| LCD 게임 | [GAME.atsln](GAME/GAME/GAME.atsln) |
| LCD 게임 변형 | [NEW_GAME.atsln](NEW_GAME/NEW_GAME/NEW_GAME.atsln) |

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
| SW3 | PE5/INT5 |
| SW4 | PE6/INT6 |
| SW6 | PE7/INT7 |

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
- `GAME`은 INT4~INT7 버튼 ISR에서 명령을 큐로 보내고, 게임 태스크가 플레이어 이동·발사·재시작을 처리합니다.
- `NEW_GAME`은 `GAME`을 변형해 탄약 3발, 자동 재장전, 점수 기반 적 속도 증가 규칙을 추가했습니다.

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
