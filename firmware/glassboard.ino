/*
 * Glassboard — NFC Business Card Firmware
 * ATtiny202 / megaTinyCore
 * 
 * Behavior:
 *   - Deep sleep at ~0.1uA drawing essentially nothing
 *   - NT3H2111 FD pin goes LOW when phone NFC field detected
 *   - PA2 interrupt wakes MCU instantly
 *   - Runs LED animation sequence (~2 seconds)
 *   - Goes back to sleep
 * 
 * Pin assignments:
 *   PA2 - FD input from NT3H2111 (interrupt, active LOW)
 *   PA1 - LED1
 *   PA3 - LED2
 *   PA6 - LED3
 *   PA7 - UPDI (programming, do not use)
 *   PA0 - RESET (do not use)
 */

#include <avr/sleep.h>
#include <avr/interrupt.h>

#define LED1 PIN_PA1
#define LED2 PIN_PA3
#define LED3 PIN_PA6
#define FD_PIN PIN_PA2

// animation timing ms
#define STEP_DELAY 80
#define PULSE_DELAY 40

void allOff() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void allOn() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
}

// chase: 1 -> 2 -> 3 -> all -> off, repeat
void runAnimation() {
  // chase forward
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1, HIGH); delay(STEP_DELAY);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH); delay(STEP_DELAY);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH); delay(STEP_DELAY);
    digitalWrite(LED3, LOW);
  }

  // all flash 3x
  for (int i = 0; i < 3; i++) {
    allOn();  delay(PULSE_DELAY * 2);
    allOff(); delay(PULSE_DELAY);
  }

  // slow fade out (PWM approximation via toggling)
  for (int brightness = 10; brightness > 0; brightness--) {
    allOn();  delay(brightness * 3);
    allOff(); delay((10 - brightness) * 3);
  }

  allOff();
}

// interrupt service routine — just wakes MCU, no action needed
ISR(PORTA_PORT_vect) {
  // clear the interrupt flag for PA2
  PORTA.INTFLAGS = PIN2_bm;
}

void goToSleep() {
  // configure PA2 as input with pullup, trigger on falling edge (FD goes LOW)
  pinMode(FD_PIN, INPUT_PULLUP);
  PORTA.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();
  sleep_cpu();       // MCU stops here until interrupt
  sleep_disable();
  cli();
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  allOff();

  // brief startup flash so you know it programmed correctly
  allOn(); delay(100); allOff();
}

void loop() {
  goToSleep();       // sleep until phone tap
  runAnimation();    // phone tapped — animate
  // loop: go back to sleep
}
