/**************************************************************************
*  PARKING CONTROL SYSTEM																									*
*																																					*
*		\brief	Programa para el sistema de control de un                     *
*     			aparcamiento.																									*
*                                                      										*
*************************************************************************** 
*                                                                         *
*         Nombre del Fichero:   \file main.c                  						*
*                                                                         *
***************************************************************************
*                                             *                           *
*  Microcontrolador:    LPC4088					      *  Reloj:      MHz   		    *
*  Lenguaje:            C                     *														*
*  Fecha Inicio:        10 / 05 / 17          *  EDICION:    1.0          *
*  Programador:         Víctor Galvín Coronil *                           *
*                       Miguel                *                           *
***************************************************************************/

#include <LPC407x_8x_177x_8x.h>
#include <stdlib.h>
#include "glcd.h"
#include "joystick.h"
#include "timer_lpc40xx.h"

#define max_plazas 20				//Número máximo de plazas del parking

void ajustar_reloj(void);
void inicializar_reloj(void);

int main (void)
{			
		unsigned int horas = 0, minutos = 0, segundos = 0;
		int n; 
		
		struct vehiculo
		{
				int identificador;
				float hora_entrada;
				float hora_salida;
				char	matricula[7];
				float importe_pago;
		};
		
		struct vehiculo lista_vehiculos[max_plazas];
		struct vehiculo lista_base_datos[n];
		
		glcd_inicializar();
		
		glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
								 "%ui:%ui:%ui", horas, minutos, segundos);
		
		ajustar_reloj();
		
	
		glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
								 "%ui:%ui:%ui", horas, minutos, segundos);		
		
		inicializar_reloj();
}
