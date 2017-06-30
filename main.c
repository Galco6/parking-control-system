/**************************************************************************
*  PARKING CONTROL SYSTEM						  *
*									  *
*		\brief	Programa para el sistema de control de un         *
*     			aparcamiento.					  *
*                                                      		          *
*************************************************************************** 
*                                                                         *
*         Nombre del Fichero:   \file main.c                              *
*                                                                         *
***************************************************************************
*                                             *                           *
*  Microcontrolador:    LPC4088		      *  Reloj:      120MHz	  *
*  Lenguaje:            C                     *			          *
*  Fecha Inicio:        10 / 05 / 17          *  EDICION:    1.0          *
*  Programador:         Victor Galvín Coronil *                           *
*                 	         	      *                           *
***************************************************************************/

#include <LPC407x_8x_177x_8x.h>
#include <stdlib.h>
#include "glcd.h"
#include "timer_lpc40xx.h"
#include "gpio_lpc40xx.h"

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

#define MAX_PLAZAS 20          //Número máximo de plazas del parking

#define MAX_BASE_DATOS 100   //Dimensión máxima base de datos

//Estructura con los datos de cada vehiculo
struct vehiculo
{
		uint32_t num_registro;
		uint8_t hora_entrada[3];
		uint8_t hora_salida[3];
		char	matricula[7];
		float importe_pago;
};
		
struct vehiculo lista_vehiculos[MAX_PLAZAS];		//Array de estructuras con todos los vehiculos en el parking
struct vehiculo lista_base_datos[MAX_BASE_DATOS];	//Array de estructuras con todos los vehiculos que han pasado

//Declaración de subrutinas
void cuenta_plazas_libres(struct vehiculo *ptr_vehiculo);
bool_t busca_plaza_libre(struct vehiculo *ptr_vehiculo);
void registro_entrada_vehiculo(struct vehiculo *ptr_vehiculo);
void registro_hora_salida_vehiculo(struct vehiculo *ptr_vehiculo);
void borra_registro_vehiculo(struct vehiculo *ptr_vehiculo);
void copia_base_datos(struct vehiculo *ptr_vehiculo);
void calcula_importe_pago(struct vehiculo *ptr_vehiculo);
void selecciona_registro(void);
void abrir_barrera(void);
void cerrar_barrera(void);

uint8_t plaza_libre = 0;
uint32_t num_plazas_libres = 20;
uint8_t registro = 1;
uint32_t horas = 0, minutos = 0, segundos = 0;
uint32_t contador_base_datos;

int main (void)
{			
	
		glcd_inicializar();
	
		//Configurar leds de salida
		LPC_GPIO2->DIR |= (1 << 15) | (1 << 19);
	
		LPC_GPIO2->CLR = (1 << 15);
		LPC_GPIO2->SET = (1 << 19);
	
		//Configura los pines 10 y 22 (pulsadores) para generar interrupciones
		LPC_GPIOINT->ENF2 |= (1 << 10)| (1 << 22);
		NVIC_ClearPendingIRQ(GPIO_IRQn);
		NVIC_EnableIRQ(GPIO_IRQn);
		NVIC_SetPriority(GPIO_IRQn, 1);
		__enable_irq();
		
		timer_inicializar(TIMER0);
		
		timer_inicializar(TIMER1);
	
		//Configuración interrupciones TIMER0. Destinado al reloj de tiempos del parking
		NVIC_ClearPendingIRQ(TIMER0_IRQn);
		NVIC_EnableIRQ(TIMER0_IRQn);
		NVIC_SetPriority(TIMER0_IRQn, 0);
		__enable_irq();
		
		timer_iniciar_ciclos_ms(TIMER0, 1000);

while (1) {
	
		cuenta_plazas_libres(&lista_vehiculos[0]);
		glcd_xprintf(95, 20, WHITE, NEGRO, FONT16X32,
							     "PLAZAS DISPONIBLES");
		glcd_xprintf(223, 120, WHITE, NEGRO, FONT16X32,
								"%02i", num_plazas_libres);
						
		}

}
//Reloj
void TIMER0_IRQHandler(void)
{
	LPC_TIM0->IR = 1;
	segundos++;
	if (segundos == 60) 
	{
		segundos = 0;
		minutos++;
				
		if (minutos == 60)
		{
			minutos = 0;
			horas++;
					
			if (horas == 23)
			{
				horas = 0;
			}
		}
	}	
	//Colocar en la parte inferior para que siempre este presente
	glcd_xprintf(191, 230, BLANCO, NEGRO, FONT16X32, "%02u:%02u:%02u", horas, minutos, segundos);

	NVIC_ClearPendingIRQ(TIMER0_IRQn);
}


void GPIO_IRQHandler(void)
{
   	//Interrupcion al accionar el pulsador de entrada
		if (LPC_GPIOINT->STATF2 & (1 << 10))
		{
			NVIC_ClearPendingIRQ(GPIO_IRQn);
			glcd_borrar(NEGRO);			
			glcd_xprintf(180, 100, WHITE, BLACK, FONT16X32,"BIENVENIDO");
			timer_retardo_ms(TIMER1, 2000);
			if(busca_plaza_libre(&lista_vehiculos[0]))
			{
				NVIC_ClearPendingIRQ(GPIO_IRQn);
				
				registro_entrada_vehiculo(&lista_vehiculos[plaza_libre]);
				glcd_borrar(NEGRO);
				glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
				"N Registro: %02i\nHora entrada: %02i:%02i:%02i\nMatricula: ", 
			                              lista_vehiculos[plaza_libre].num_registro, 
						      lista_vehiculos[plaza_libre].hora_entrada[0],
                                                      lista_vehiculos[plaza_libre].hora_entrada[1],
			                              lista_vehiculos[plaza_libre].hora_entrada[2]);
			}
			else
			{
				glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,"LO SENTIMOS, PARKING COMPLETO");

				return;
			}
			timer_retardo_ms(TIMER1, 3000);
			abrir_barrera();
			timer_retardo_ms(TIMER1, 4000);
			if(!(LPC_GPIO4-> PIN & (1 << 30)))		//Detecta señal del sensor
			{
				//Espera que la señal cambie de estado
				while(!(LPC_GPIO4-> PIN & (1 << 30)));
			}
			timer_retardo_ms(TIMER1, 2000);
			cerrar_barrera();
			glcd_borrar(NEGRO);
			LPC_GPIOINT->CLR2 = (1 << 10);
		}
		
		//Interrupcion al accionar el pulsador de salida
		if(LPC_GPIOINT->STATF2 & (1 << 22))
		{
			NVIC_ClearPendingIRQ(GPIO_IRQn);
			glcd_borrar(NEGRO);
			selecciona_registro();
			if(lista_vehiculos[registro - 1].num_registro == 0)
			{
					glcd_borrar(NEGRO);
					glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,"ERROR, VEHICULO NO REGISTRADO");
					timer_retardo_ms(TIMER1, 2000);
					glcd_borrar(NEGRO);
			}
			else
			{
			registro_hora_salida_vehiculo(&lista_vehiculos[registro - 1]);
			calcula_importe_pago(&lista_vehiculos[registro - 1]);
			glcd_borrar(NEGRO);
			glcd_xprintf(0, 0, WHITE, BLACK, FONT16X32,
			"N Registro: %02i\nHora entrada: %02i:%02i:%02i\nHora salida: %02i:%02i:%02i\nImporte: %.2f euros", 
			                                                   lista_vehiculos[registro - 1].num_registro, 
									   lista_vehiculos[registro - 1].hora_entrada[0],
                                                         		   lista_vehiculos[registro - 1].hora_entrada[1],
			                                                   lista_vehiculos[registro - 1].hora_entrada[2],
			                                                   lista_vehiculos[registro - 1].hora_salida[0],
			                                                   lista_vehiculos[registro - 1].hora_salida[1],
			                                                   lista_vehiculos[registro - 1].hora_salida[2],
															                           lista_vehiculos[registro - 1].importe_pago);
			copia_base_datos(&lista_vehiculos[registro - 1]);
			borra_registro_vehiculo(&lista_vehiculos[registro - 1]);
			timer_retardo_ms(TIMER1, 3000);
			abrir_barrera();
			timer_retardo_ms(TIMER1, 4000);
			if(!(LPC_GPIO4-> PIN & (1 << 30)))
			{
			while(!(LPC_GPIO4-> PIN & (1 << 30)));
			}
			timer_retardo_ms(TIMER1, 2000);
			cerrar_barrera();
			glcd_borrar(NEGRO);
			}
			LPC_GPIOINT->CLR2 = (1 << 22);
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
* \brief Cuenta el número de plazas libres de registro en lista_vehiculos[]
*
* \param[in] ptr_vehiculo	puntero al elemento del array lista_vehiculos[]
*
*/
void cuenta_plazas_libres(struct vehiculo *ptr_vehiculo)
{
	uint32_t i;
	num_plazas_libres = 0;
	
	for(i = 0; i < MAX_PLAZAS; i++)
	{
		if(ptr_vehiculo->num_registro == 0)
		{
			num_plazas_libres++;
		}
		ptr_vehiculo++;
	}
}

/***************************************************************************//**
* \brief Registra la entrada de un nuevo vehiculo
*
* \param[in] ptr_vehiculo	puntero al elemento del array lista_vehiculos[]
*
*/
void registro_entrada_vehiculo(struct vehiculo *ptr_vehiculo)
{
	ptr_vehiculo->num_registro = plaza_libre + 1;
	ptr_vehiculo->hora_entrada[0] = horas;
	ptr_vehiculo->hora_entrada[1] = minutos;
	ptr_vehiculo->hora_entrada[2] = segundos;
}

/***************************************************************************//**
* \brief Registra la hora de salida de un vehículo
*
* \param[in] ptr_vehiculo	puntero al elemento del array lista_vehiculos[]
*
*/
void registro_hora_salida_vehiculo(struct vehiculo *ptr_vehiculo)
{
	ptr_vehiculo->hora_salida[0] = horas;
	ptr_vehiculo->hora_salida[1] = minutos;
	ptr_vehiculo->hora_salida[2] = segundos;
}

/***************************************************************************//**
* \brief Copia el registro de un vehiculo a lista_base_datos[]
*
* \param[in] ptr_vehiculo	puntero al elemento del array lista_vehiculos[]
*
*/
void copia_base_datos(struct vehiculo *ptr_vehiculo)
{	
	lista_base_datos[contador_base_datos] = *ptr_vehiculo;
	contador_base_datos++;
}

/***************************************************************************//**
* \brief Borra el registro de un vehiculo en lista_vehiculos[]
*
* \param[in] ptr_vehiculo	puntero al elemento del array lista_vehiculos[]
*
*/
void borra_registro_vehiculo(struct vehiculo *ptr_vehiculo)
{
	ptr_vehiculo->num_registro = 0;
	ptr_vehiculo->hora_entrada[0] = 0;
	ptr_vehiculo->hora_entrada[1] = 0;
	ptr_vehiculo->hora_entrada[2] = 0;
	ptr_vehiculo->hora_salida[0] = 0;
	ptr_vehiculo->hora_salida[1] = 0;
	ptr_vehiculo->hora_salida[2] = 0;
	ptr_vehiculo->importe_pago = 0;
}

/***************************************************************************//**
* \brief Pantalla para seleccionar el registro correspondiente a su vehiculo
*
*/
void selecciona_registro()
{
	glcd_borrar(NEGRO);
	glcd_xprintf(223, 120, BLANCO, NEGRO, FONT16X32, "%02i", registro);
	while(LPC_GPIO2->PIN & (1 << 23))
	{
		if((LPC_GPIO2->PIN & (1 << 25)) == 0)
		{
			if(registro == MAX_PLAZAS)
			{
				registro = 1;
				glcd_xprintf(223, 120, BLANCO, NEGRO, FONT16X32, "%02i", registro);
				timer_retardo_ms(TIMER1, 200);
			}
			else
			{
				registro++;
				glcd_xprintf(223, 120, BLANCO, NEGRO, FONT16X32, "%02i", registro);
				timer_retardo_ms(TIMER1, 200);
			}
		}
		if((LPC_GPIO2->PIN & (1 << 27)) == 0)
		{
			if(registro == 1)
			{
				registro = 20;
				glcd_xprintf(223, 120, BLANCO, NEGRO, FONT16X32, "%02i", registro);
				timer_retardo_ms(TIMER1, 200);
			}
			else
			{
			registro--;
			glcd_xprintf(223, 120, BLANCO, NEGRO, FONT16X32, "%02i", registro);
			timer_retardo_ms(TIMER1, 200);
			}
		}
	}
}

/***************************************************************************//**
* \brief Calcula el importe a pagar por el vehículo saliente
*
*/
void calcula_importe_pago(struct vehiculo *ptr_vehiculo)
{
	float horas_totales;
	float tiempo_total_minutos;
	
	horas_totales = ptr_vehiculo->hora_salida[0] - ptr_vehiculo->hora_entrada[0];
	tiempo_total_minutos = horas_totales * 60 + (ptr_vehiculo->hora_salida[1] - ptr_vehiculo->hora_entrada[1]);
	
	if (tiempo_total_minutos < 31)
	{
		ptr_vehiculo->importe_pago = tiempo_total_minutos * 0.05f;
	}
	else if (tiempo_total_minutos < 91)
	{
		ptr_vehiculo->importe_pago = tiempo_total_minutos * 0.04f;
	}
	else if (tiempo_total_minutos < 660)
	{
		ptr_vehiculo->importe_pago = tiempo_total_minutos * 0.06f;
	}
	else
	{
		ptr_vehiculo->importe_pago = 31.25;
	}
}

/***************************************************************************//**
* \brief Abre barrera
*
*/
void abrir_barrera (void)
{
	LPC_GPIO2->SET = (1 << 15);
	LPC_GPIO2->CLR = (1 << 19);
}

/***************************************************************************//**
* \brief Cierra barrera
*
*/
void cerrar_barrera (void)
{
	LPC_GPIO2->SET = (1 << 19);
	LPC_GPIO2->CLR = (1 << 15);
}

