/***************************************************************************//**
 * \file   gpio_lpc40xx.c
 *
 * \brief  Funciones para manejar los puertos E/S digital del LPC40xx.
 *
 *         Las funciones de lectura y escritura en los pines se han declarado
 *         inline y están definidas en el fichero gpio_lpc40xx.h.
 */

#include "gpio_lpc40xx.h"
#include "error.h"
#include <LPC407x_8x_177x_8x.h>

/***************************************************************************//**
 * \brief      Configurar la dirección de uno o más pines.
 *
 * \param[in]  puerto       Puerto del pin o pines cuya dirección se quiere
 *                          ajustar.
 * \param[in]  mascara_pin  Máscara de selección del pin o pines.
 * \param[in]  direccion    Dirección: DIR_ENTRADA o DIR_SALIDA.
 */
void gpio_ajustar_dir(LPC_GPIO_TypeDef *gpio_regs,
                      uint32_t mascara_pin,
                      uint32_t direccion)
{   
    /* C O M P L E T A R
     */
}

/***************************************************************************//**
 * \brief       Consultar la dirección de un pin establecida actualmente.
 *
 * \param[in]   puerto      Puerto del pin cuya dirección se quiere obtener.
 * \param[in]   mascara_pin Máscara de selección del pin.
 *
 * \return      Dirección del puerto (DIR_IN o DIR_OUT).
 */
uint32_t gpio_obtener_dir(LPC_GPIO_TypeDef *gpio_regs,
                          uint32_t mascara_pin)
{
    /* C O M P L E T A R
     */
}
