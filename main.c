/**************************************************************************
*  PARKING CONTROL SYSTEM						  *
*									  *
*		\brief	Programa para el sistema de control de un         *
*     			aparcamiento.					  *
*                                                      			  *
*************************************************************************** 
*                                                                         *
*         Nombre del Fichero:   \file main.c                  		  *
*                                                                         *
***************************************************************************
*                                             *                           *
*  Microcontrolador:    LPC4088		      *  Reloj:      MHz   	  *
*  Lenguaje:            C                     *				  *
*  Fecha Inicio:        10 / 05 / 17          *  EDICION:    1.0          *
*  Programador:         Víctor Galvín Coronil *                           *
*                       Miguel                *                           *
***************************************************************************/

#include <LPC407x_8x_177x_8x.h>
#include <stdlib.h>
#include "glcd.h"
#include "joystick.h"
#include "timer_lpc40xx.h"
#include "gpio_lpc40xx.h"

#define MAX_PLAZAS 20				//Número máximo de plazas del parking

#define MAX_BASE_DATOS 10000

//Estructura con los datos de cada vehiculo entrante
struct vehiculo
{
		int num_registro;
		float hora_entrada;
		float hora_salida;
		char	matricula[7];
		float importe_pago;
};
		
struct vehiculo lista_vehiculos[MAX_PLAZAS];		//Array de estructuras con todos los vehiculos en el parking
struct vehiculo lista_base_datos[MAX_BASE_DATOS];	//Array de estructuras con todos los vehiculos que han pasado

//Declaración de subrutinas
void ajustar_reloj(void);
void inicializar_reloj(void);
void cuenta_plazas_libres(struct vehiculo *ptr_vehiculo);
bool_t busca_plaza_libre(struct vehiculo *ptr_vehiculo);
void registro_entrada_vehiculo(struct vehiculo *ptr_vehiculo);
void borra_registro_vehiculo(struct vehiculo *ptr_vehiculo);
void copia_base_datos(struct vehiculo *ptr_vehiculo);

typedef struct
{
	__I uint32_t STATUS;
	__I uint32_t STATR0;
	__I uint32_t STATF0;
	__O uint32_t CLR0;
	__IO uint32_t ENR0;
	__IO uint32_t ENF0;
	uint32_t RESERVED0[3];
	__I uint32_t STATR2;
	__I uint32_t STATF2;
	__O uint32_t CLR2;
	__IO uint32_t ENR2;
	__IO uint32_t ENF2;
} LPC_GPIOINT_TypeDef_corregido;
		
#undef LPC_GPIOINT
#define LPC_GPIOINT ((LPC_GPIOINT_TypeDef_corregido *) LPC_GPIOINT_BASE)

uint8_t plaza_libre;
uint32_t num_plazas_libres;

int main (void)
{			
		unsigned int horas = 0, minutos = 0, segundos = 0;
		int n; 
		
		glcd_inicializar();
		
		//Configura los pines 10 y 22 (pulsadores) para generar interrupciones
		LPC_GPIOINT->ENF2 |= (1 << 10)|(1 << 22);
		NVIC_ClearPendingIRQ(GPIO_IRQn);
		NVIC_EnableIRQ(GPIO_IRQn);
		NVIC_SetPriority(GPIO_IRQn, 0);
		__enable_irq();
		
		inicializar_reloj();
		
		cuenta_plazas_libres(&lista_vehiculos[0]);
		
		glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
								"Numero de plazas libres: %ui", num_plazas_libres);
	
}

void GPIO_IRQHandler(void)
{
		//Interrupcion al accionar el pulsador de entrada
		if (LPC_GPIOINT->STATF0 & (1 << 30))
		{
			NVIC_ClearPendingIRQ(GPIO_IRQn);
			glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
									"BIENVENIDO");
			glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
			"N Registro: %i\nHora entrada: %f\nMatricula: %c", lista_vehiculos[plaza_libre].num_registro, 
																												 lista_vehiculos[plaza_libre].hora_entrada,
																												 lista_vehiculos[plaza_libre].matricula);
			busca_plaza_libre(&lista_vehiculos[0]);
			registro_entrada_vehiculo(&lista_vehiculos[plaza_libre]);
			LPC_GPIOINT->CLR2;
		}
		
		//Interrupcion al accionar el pulsador de salida
		if(LPC_GPIOINT->STATF0 & (1 << 22))
		{
			NVIC_ClearPendingIRQ(GPIO_IRQn);
			copia_base_datos(&lista_vehiculos[0]);
			borra_registro_vehiculo(&lista_vehiculo[registro]);
			LPC_GPIOINT->CLR2;
		}
		
}

/***************************************************************************//**
* \brief Busca una plaza libre en el parking. Recorre el array lista_vehiculo
*					y comprueba que num_registro sea 0.
*
* \param[in] ptr_vehiculo	puntero a los elementos del array lista_vehiculos[]
*
* \return TRUE => plaza libre, FALSE => no hay plaza libre
*/
bool_t busca_plaza_libre(struct vehiculo *ptr_vehiculo)
{
		int i;
		for(i = 0; i <= MAX_PLAZAS; i++)
		{
				ptr_vehiculo = &lista_vehiculos[i];
				if (ptr_vehiculo->num_registro == 0)
				{
						plaza_libre = i;
						return TRUE;
				}
		}
		return FALSE;
}

/***************************************************************************//**
* \brief Registra la entrada de un nuevo vehiculo
*
* \param[in] ptr_vehiculo	puntero a los elementos del array lista_vehiculos[]
*
*/
void registro_entrada_vehiculo(struct vehiculo *ptr_vehiculo)
{
		ptr_vehiculo->num_registro = numero_registro();
		ptr_vehiculo->hora_entrada = hora_actual();
		ptr_vehiculo->matricula = obtiene_matricula();
}

void borra_registro_vehiculo()
{
	
}

void copia_base_datos()
{
	
}
