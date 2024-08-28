#include "pitches.h"
#include <NewTone.h>
#include "melody.h"
 //リセット関数
 void(*resetFunc) (void) = 0;
const int BUZZER_PIN=13;//ブザーとLEDに接続するピン
const int PATTERN_NUM = 4;
int pattern_i = 0;
int button_cur, button_pre;
const int PATTERN[PATTERN_NUM] = {
 0b0001,
 0b0010,
 0b0100,
 0b1000,
};
const int NOTE_NUM = 113;
const int canonMelody[][2] = {
 {NOTE_G4, 4},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 4},
 
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 8},
 
 {NOTE_G3, 8},
 {NOTE_A3, 8},
 {NOTE_B3, 8},
 {NOTE_C4, 8},
 {NOTE_D4, 8},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_E4, 4},
 {NOTE_C4, 8},
 {NOTE_D4, 8},
 {NOTE_E4, 4},
 {NOTE_E3, 8},
 {NOTE_F3, 8},
 {NOTE_G3, 8},
 {NOTE_A3, 8},
 {NOTE_G3, 8},
 {NOTE_F3, 8},
 {NOTE_G3, 8},
 {NOTE_E3, 8},
 {NOTE_F3, 8},
 {NOTE_G3, 8},
 {NOTE_F3, 4},
 {NOTE_A3, 8},
 {NOTE_G3, 8},
 {NOTE_F3, 4},
 {NOTE_E3, 8},
 {NOTE_D3, 8},
 {NOTE_E3, 8},
 {NOTE_D3, 8},
 {NOTE_C3, 8},
 {NOTE_D3, 8},
 {NOTE_E3, 8},
 {NOTE_F3, 8},
 {NOTE_G3, 8},
 {NOTE_A3, 8},
 {NOTE_F3, 4},
 {NOTE_A3, 8},
 {NOTE_G3, 8},
 {NOTE_A3, 4},
 {NOTE_B3, 8},
 {NOTE_C4, 8},
 {NOTE_G3, 8},
 {NOTE_A3, 8},
 {NOTE_B3, 8},
 {NOTE_C4, 8},
 {NOTE_D4, 8},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 8},
 {NOTE_E4, 4},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 4},
 
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 8},
 
 {NOTE_G3, 8},
 {NOTE_A3, 8},
 {NOTE_B3, 8},
 {NOTE_C4, 8},
 {NOTE_D4, 8},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_E4, 4},
 {NOTE_A4, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 4},
 {NOTE_C4, 8},
 {NOTE_D4, 8},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_E4, 8},
 {NOTE_D4, 8},
 {NOTE_E4, 8},
 {NOTE_C5, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 8},
 {NOTE_A4, 4},
 {NOTE_C5, 8},
 {NOTE_B4, 8},
 {NOTE_A4, 4},
 {NOTE_G4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 8},
 {NOTE_F4, 8},
 {NOTE_E4, 8},
 {NOTE_F4, 8},
 {NOTE_G4, 8},
 {NOTE_A4, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 8},
 {NOTE_A4, 4},
 {NOTE_C5, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 4},
 
 {NOTE_B4, 8},
 {NOTE_A4, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 8},
 {NOTE_D5, 8},
 {NOTE_C5, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 8},
 {NOTE_A4, 8},
 {NOTE_B4, 8},
 {NOTE_C5, 4}};
unsigned long startTimeMs;
unsigned long presentTimeMs;
unsigned long endTimeMs = 10000;
const int CNOTE_NUM = 37;
const int cantryMelody[][2] = {
  {NOTE_F3, 4},
 {NOTE_G3, 4},
 {NOTE_A3, 1},
 {NOTE_Delay,2},
 {NOTE_A3, 4},
 {NOTE_G3, 4},
 {NOTE_F3, 2},
  {NOTE_G3,1},
{NOTE_Delay,2},
    {NOTE_A3, 4},
 {NOTE_G3, 4},
 {NOTE_F3, 1},
{NOTE_Delay,2},
  {NOTE_A3, 4},
 {NOTE_C4, 4},
 {NOTE_D4, 1},
 {NOTE_Delay,2},
 {NOTE_D4, 4},
 {NOTE_A3, 4},
 {NOTE_C4, 8},
 {NOTE_C4, 4},
 {NOTE_A3, 1},
 {NOTE_Delay,2},
  {NOTE_A3, 8},
 {NOTE_G3, 8},
 {NOTE_F3, 4},
 {NOTE_G3, 2},
 {NOTE_A3, 1},
 {NOTE_Delay,2},

 {NOTE_A3, 8},
 {NOTE_G3, 8},
 {NOTE_F3, 4},
 {NOTE_F3, 1},
 {NOTE_Delay,2},
   {NOTE_F3, 4},
 {NOTE_G3, 4},
 {NOTE_F3, 1},
 
};
const int sNOTE_NUM = 39;
const int sMelody[][2] = {
 {NOTE_A4, 4},
 {NOTE_B4, 4},
 {NOTE_C5, 4},
 {NOTE_C5,4},
 {NOTE_D5, 4},
 {NOTE_C5, 4},
 {NOTE_B4, 2},
 
  {NOTE_E4,4},
{NOTE_G4,4},
    {NOTE_A4, 4},
 {NOTE_A4, 4},
 {NOTE_G4, 4},
{NOTE_F4,4},
{NOTE_G4, 1.75},

  {NOTE_G4, 4},
 {NOTE_G4, 4},
 {NOTE_F4, 4},
 {NOTE_F4,4},
 {NOTE_DS4, 4},
 {NOTE_F4, 2},
 {NOTE_C4, 4},
 {NOTE_F4, 4},
 {NOTE_G4, 1.75},
 {NOTE_GS4,4},
  {NOTE_G4, 2},
  
 {NOTE_A4, 4},
 {NOTE_B4, 4},
 {NOTE_C5, 4},
 {NOTE_C5,4},
 {NOTE_D5, 4},
 {NOTE_C5, 4},
 {NOTE_B4, 2},
 
  {NOTE_E4,4},
{NOTE_G4,4},
    {NOTE_A4, 4},
 {NOTE_A4, 4},
 {NOTE_G4, 4},
{NOTE_F4,4},
{NOTE_G4, 2},
 
};
void canon()
{
 for (int thisNote = 56; thisNote < NOTE_NUM; thisNote++) {
 int noteDuration = 1000 / canonMelody[thisNote][1];
 NewTone(BUZZER_PIN, canonMelody[thisNote][0], noteDuration);
 int pauseBetweenNotes = noteDuration * 1.30;
 delay(pauseBetweenNotes);
 noNewTone(BUZZER_PIN);
 }
 
}


void cantry()
{
 for (int thisNote = 0; thisNote < CNOTE_NUM; thisNote++) {
 int noteDuration = 1000 / cantryMelody[thisNote][1];
 NewTone(BUZZER_PIN, cantryMelody[thisNote][0], noteDuration);
 int pauseBetweenNotes = noteDuration * 1.30;
 delay(pauseBetweenNotes);
 noNewTone(BUZZER_PIN);
 }
}
 void canonfull()
{
 for (int thisNote = 0; thisNote < NOTE_NUM; thisNote++) {
 int noteDuration = 1000 / canonMelody[thisNote][1];
 NewTone(BUZZER_PIN, canonMelody[thisNote][0], noteDuration);
 int pauseBetweenNotes = noteDuration * 1.30;
 delay(pauseBetweenNotes);
 noNewTone(BUZZER_PIN);
 }
 
}
 void summer()
{
 for (int thisNote = 0; thisNote < sNOTE_NUM; thisNote++) {
 int noteDuration = 1000 / sMelody[thisNote][1];
 NewTone(BUZZER_PIN, sMelody[thisNote][0], noteDuration);
 int pauseBetweenNotes = noteDuration * 1.30;
 delay(pauseBetweenNotes);
 noNewTone(BUZZER_PIN);
 }
 
}
