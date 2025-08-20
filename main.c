/*
 * 情報
 *      File:main.c
 *      Author:Technoalpha Tomono
 *      PIC:PIC12F683
 * 
 * ハード構成
 *      入力にはプルアップ抵抗をつけている為
 *      入力OFF時に出力がONとなる
 *      GP2はNOTゲートでLEDの切替を行っている
 *      インバータはシュミットトリガ無し
 * 
 * プログラム内容
 * 　   GP3がH(プルアップ抵抗有り)
 *      USB系統1→GP0=1 GP5=1になる 他は0
 *      GP3がL
 *      USB系統2→GP1=1 GP4=1になる 他は0
 * 
 *      GP0のON:ASTERION mouseがON
 *      GP1のON:GATEWAYPC mouseがON 
 *      GP5のON:ASTERION keyboardがON
 *      GP4のON:GATEWAYPC keyboardがON
 *      GP2のON:LED ONの切替
 * 
 *入力チャタリング対策Ver
 */

 // ── インクルードファイル ── 
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// ── コンフィグレーションビット設定（書込み時に反映）──
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// ── マクロ置換 ──
#define MOUSE_UPPER       GP0
#define MOUSE_LOWER       GP5
#define KEYBOARD_UPPER    GP1
#define KEYBOARD_LOWER    GP4
#define LED_SWITCH        GP2
#define SWITCH_IN         GP3

// ── 遅延関数の基準クロック周波数（OSCCONで設定する実クロックと一致させる必要がある）──
#define _XTAL_FREQ 1000000UL

int main(int argc, char** argv)
{
    unsigned char toggle = 0;   // 0=OFF, 1=ON

    // 1MHz/デジタルIO/方向設定
    OSCCON = 0x40;              // 内部クロックを1MHzに設定
    CMCON0 = 0x07;              // コンパレータ機能を無効化（全てI/Oとして使う
    ANSEL  = 0x00;              // アナログ入力を無効化（全ピンをデジタルI/Oに)
    TRISIO = 0x08;              // ポート方向設定。bit3(GP3)=1で入力、それ以外は出力
    GPIO   = 0x00;              // 出力ポートを初期化（電源投入時の不定状態回避の為全て0に）

    // 初期出力（LEDはインバータで切り替え処理をしている）
    MOUSE_UPPER    = 0;         // NOT後=1
    MOUSE_LOWER    = 1;         // NOT後=0
    KEYBOARD_UPPER = 0;         // NOT後=1
    KEYBOARD_LOWER = 1;         // NOT後=0
    LED_SWITCH     = 0;         // NOT後=LED ON

    while (1)
    {
        if (SWITCH_IN)          // GP3 = High（High=未押下・外部でプルアップ）
        {
            if (toggle == 0)    // 前回OFFなら一度だけ反転
            {                   // ^= 1 はビット反転
                                // 例: 0 ^ 1 = 1, 1 ^ 1 = 0
                MOUSE_UPPER    ^= 1;
                MOUSE_LOWER    ^= 1;
                KEYBOARD_UPPER ^= 1;
                KEYBOARD_LOWER ^= 1;
                LED_SWITCH     ^= 1;

                __delay_ms(20); // 指の感覚吸収用デバウンス
            }
            toggle = 1;
        }
        else                    // GP3 = Low（Low=押下）
        {
            if (toggle)
                __delay_ms(20);
            toggle = 0;
        }
    }

    return (EXIT_SUCCESS);      // 形式上
}