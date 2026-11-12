# 현대자동차그룹 HINT 1기

[日本語](README.ja.md) · [English](README.md) · [한국어](README.ko.md)

## 실시간 운영체제 기반 전자제어기 프로그래밍 실습

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
