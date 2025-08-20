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
 *      GP3がH(プルアップ抵抗有り)
 *       USB系統1→GP0=1 GP5=1になる 他は0
 *      GP3がL
 *       USB系統2→GP1=1 GP4=1になる 他は0
 * 
 *     GP0のON:ASTERION mouseがON
 *     GP1のON:GATEWAYPC mouseがON 
 *     GP5のON:ASTERION keyboardがON
 *     GP4のON:GATEWAYPC keyboardがON
 *     GP2のON:LED ONの切替
 *  
 * 入力チャタリング対策Ver
 */

// インクルードファイル
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// コンフィグレーションビット設定
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// マクロ置換
#define MOUSE_UPPER GP0
#define MOUSE_LOWER GP5
#define KEYBOARD_UPPER GP1
#define KEYBOARD_LOWER GP4
#define LED_SWICH GP2
#define SWICH_IN GP3

// クロック周波数指定 (ここでは8MHzに設定)
// delay_ms(x) の為の定義
#define _XTAL_FREQ  8000000

/*
 * main()関数
 */

int main(int argc, char** argv) {
        
     // 変数宣言
     unsigned char toggle; // スイッチの状態を格納。OFF=0, ON=1とする
    
     // PICマイコン設定
     OSCCON = 0x40;     // クロック周波数を1MHzに設定
     CMCON0 = 0x07;     // コンパレータ無効 GP0-2を入出力で使用する場合はCM0-2を全て1
     ANSEL  = 0x00;     // アナログは使用しない（すべてデジタルI/Oに割当てる）
     TRISIO = 0x00;     // GP3以外のピンを出力に割当て(GP3は入力専用)
     
     // IO初期化   
     GPIO   = 0x00;     // 出力ピンの初期化(全てL)
     
     // USB系統を1にする(入力がプルアップの為GP0,GP5は0)
     MOUSE_UPPER = 0;
     MOUSE_LOWER = 1;
     KEYBOARD_UPPER = 0;
     KEYBOARD_LOWER = 1;
     
     // LEDはインバータで切替え処理をしている
     LED_SWICH = 0;
             
     // トグル処理
     // 最初に「トグル変数」に0を入れておく
     toggle = 0;

     // トグル変数が0、かつ入力がONの時
     // トグル処理を行う
     while(1){
        // 入力状態を確認する
        if(SWICH_IN){
            // 入力がONのときの処理
            // 入力ON、かつトグル変数が0のとき            
            if(toggle == 0){
                MOUSE_UPPER = !MOUSE_UPPER;
                MOUSE_LOWER = !MOUSE_LOWER;
                KEYBOARD_UPPER = !KEYBOARD_UPPER;
                KEYBOARD_LOWER = !KEYBOARD_LOWER;
                LED_SWICH = !LED_SWICH;
                // チャタリング防止のウエイトを入れる
                __delay_ms(20);
            }
            // ループ終了
            // 入力状態を「トグル変数」に入れて記憶させる
            toggle = 1;
            
        } else {
            // 入力がOFFのときの処理
            // 入力がOFFの場合はトグル処理はしないが、
            // トグル変数が1だったら、スイッチがOFFに変わったということなので
            // チャタリング防止のウエイトを入れる
            if(toggle){
                __delay_ms(20);
            }
            // ループ終了
            // 次回の処理の為にトグル変数を元に戻す
            toggle = 0;
        }
    }

     // 到達無し
    return (EXIT_SUCCESS);
}
