/***************************************************************************//**
 * \file    leds.c
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
 
#include <LPC407x_8x_177x_8x.h>
#include "gpio_lpc40xx.h"
#include "leds.h"

/***************************************************************************//**
 * \brief   Configurar los pines del microcontrolador que están conectados
 *          a los LEDs como salidas y apagar todos los LEDs.
 */
void inicializar_leds(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Apagar el LED 1.
 */
void apagar_led_1(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Apagar el LED 2.
 */
void apagar_led_2(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Apagar el LED 3.
 */
void apagar_led_3(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Apagar el LED 4.
 */
void apagar_led_4(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Encender el LED 1.
 */
void encender_led_1(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Encender el LED 2.
 */
void encender_led_2(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Encender el LED 3.
 */
void encender_led_3(void)
{
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief   Encender el LED 4.
 */
void encender_led_4(void)
{
    /* C O M P L E T A R
     */
}

