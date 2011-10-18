/*
 ============================================================================
 Name        : xc2-led.xc
 Description : Illuminate an LED on the XC-2 card 
 ============================================================================
 */

#include <platform.h>

out port x0ledB = PORT_LED_0_1;

static char led = 0u;

void vin_led() {
  x0ledB <: led & 0x01;
  led = ~led;
}