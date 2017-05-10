/***************************************************************************//**
 * \file    leds.h
 *
 * \brief   Funciones para manejar cuatro de los LEDs de la tarjeta Embedded
 *          Artist LPC4088.
 *
 * Las conexiones entre dos de los LEDs de la tarjeta y el microcontrolador son
 * las siguientes:
 *
 *      P1[5]   --> LED4 (ROJO) 
 *      P0[14] 	--> LED3 (VERDE) 
 *      P0[13]  --> LED2 (VERDE)  
 *		P1[18]  -->	LED1 (VERDE) 
 *
 * Los LEDs son activos a nivel bajo: cuando uno de estos pines del
 * microcontrolador se pone a 1 el correspondiente LED se apaga y cuando se
 * pone a 0 el LED se enciende.
 */

#ifndef LEDS_H
#define LEDS_H

void inicializar_leds(void);
void apagar_led_1(void);
void apagar_led_2(void);
void apagar_led_3(void);
void apagar_led_4(void);
void encender_led_1(void);
void encender_led_2(void);
void encender_led_3(void);
void encender_led_4(void);

#endif  /* LEDS */
