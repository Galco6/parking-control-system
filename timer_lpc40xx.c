/***************************************************************************//**
 * \file    timer_lpc40xx.c
 *
 * \brief   Funciones de uso de los timers del LPC40xx.
 */

#include <LPC407x_8x_177x_8x.h>
#include "timer_lpc40xx.h"
#include "error.h"

/***************************************************************************//**
 * \brief       Inicializar un timer habilitándolo en el registro de control de
 *              consumo de periféricos PCONP.
 *
 * \param[in]   timer_regs    puntero al bloque de registros del timer a
 *                            inicializar.
 */
void timer_inicializar(LPC_TIM_TypeDef *timer_regs)
{
		ASSERT(timer_regs == LPC_TIM0 ||
					 timer_regs == LPC_TIM1 ||
					 timer_regs == LPC_TIM2 ||
					 timer_regs == LPC_TIM3, "Argumento timer_regs incorrecto");
	
		if (timer_regs == LPC_TIM0)
		{
				LPC_SC->PCONP |= 1 << 1;
		}
		else if (timer_regs == LPC_TIM1)
		{
				LPC_SC->PCONP |= 1 << 2;
		}
		else if (timer_regs == LPC_TIM2)
		{
				LPC_SC->PCONP |= 1 << 22;
		}
		else if (timer_regs == LPC_TIM3)
		{
				LPC_SC->PCONP |= 1 << 23;
		}
}

/***************************************************************************//**
 * \brief       Usar un timer para generar un retardo del número de milisegundos
 *              indicado. La función no retorna hasta que transcurre este
 *              tiempo.
 *
 * \param[in]   timer_regs      puntero al bloque de registros del timer.
 * \param[in]   retado_en_ms    número de milisegundos de duración del retardo.
 */
void timer_retardo_ms(LPC_TIM_TypeDef *timer_regs,
                      uint32_t retardo_en_ms)
{
		ASSERT(timer_regs == LPC_TIM0 ||
					 timer_regs == LPC_TIM1 ||
					 timer_regs == LPC_TIM2 ||
					 timer_regs == LPC_TIM3, "Argumento timer_regs incorrecto");
	
		if (retardo_en_ms == 0) return; 
		
		timer_regs->TCR = 0; 
		timer_regs->PC = 0;
		timer_regs->TC = 0; 
		timer_regs->PR = PeripheralClock/10000 - 1;
		timer_regs->MR0 = 10 * retardo_en_ms - 1;
		timer_regs->MCR = 1; 
		timer_regs->IR = 1; 
		timer_regs->TCR = 1;
		while ((timer_regs->IR & 1) == 0) {} 
}

/***************************************************************************//**
 * \brief       Programar un timer para que realice ciclos con una duración del
 *              número de milisegundos indicado.
 *
 * \param[in]   timer_regs    puntero al bloque de registros del timer.
 * \param[in]   periodo_en_ms   duración de cada ciclo en milisegundos.
 */
void timer_iniciar_ciclos_ms(LPC_TIM_TypeDef *timer_regs,
                             uint32_t periodo_en_ms)
{
		ASSERT(timer_regs == LPC_TIM0 ||
					 timer_regs == LPC_TIM1 ||
					 timer_regs == LPC_TIM2 ||
					 timer_regs == LPC_TIM3, "Argumento timer_regs incorrecto");
		
		if (periodo_en_ms == 0) return;
		
		timer_regs->TCR = 0; 
		timer_regs->PC = 0;
		timer_regs->TC = 0;
		timer_regs->PR = PeripheralClock/10000 - 1;
		timer_regs->MR0 = 10 * periodo_en_ms - 1;	
		timer_regs->MCR = 1;
		timer_regs->MCR	|= 1 << 1;
		timer_regs->IR = 1;
		timer_regs->TCR = 1;
	
}

/***************************************************************************//**
 * \brief       Esperar a que el bit de petición de interrupción de match 0 de
 *              un timer se ponga a 1. Cuando esto ocurra, ponerlo a 0 y
 *              retornar.
 *
 * \param[in]   timer_regs    puntero al bloque de registros del timer.
 */
void timer_esperar_match0(LPC_TIM_TypeDef *timer_regs)
{
		ASSERT(timer_regs == LPC_TIM0 ||
					 timer_regs == LPC_TIM1 ||
					 timer_regs == LPC_TIM2 ||
					 timer_regs == LPC_TIM3, "Argumento timer_regs incorrecto");

		while ((timer_regs->IR & 1) == 0) {}
		timer_regs->IR = 1;
}
