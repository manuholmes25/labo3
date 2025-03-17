#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

int main(void)
{
    // Habilitar los puertos N, F y J
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));

    // Configurar PN0, PN1, PF0, PF4 como salida (LEDs)
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    
    // Configurar PJ0 y PJ1 como entrada (Botones) con resistencia pull-up
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Variable para el contador
    int counter = 0;

    uint8_t led_map[4] = {GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_4, GPIO_PIN_0};
    uint32_t port_map[4] = {GPIO_PORTN_BASE, GPIO_PORTN_BASE, GPIO_PORTF_BASE, GPIO_PORTF_BASE};

    while(1)
    {
        // Verificar si el primer botón está presionado (aumentar)
        if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0) 
        {
            if (counter < 15)
            {
                counter++;  // Incrementar el contador
            }
        }
        // Verificar si el segundo botón está presionado (disminuir)
        else if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1) == 0)
        {
            if (counter > 0)
            {
                counter--;  // Decrementar el contador
            }
        }

        // Actualizar LEDs con el valor binario del contador
        for (int i = 0; i < 4; i++)
        {
            // Verificamos si el bit correspondiente está activado
            if (counter & (1 << i)) {
                // Encender el LED si el bit está activado
                GPIOPinWrite(port_map[i], led_map[i], led_map[i]);
            } else {
                // Apagar el LED si el bit está apagado
                GPIOPinWrite(port_map[i], led_map[i], 0);
            }
        }
        
        SysCtlDelay(2000000); // Pequeño retardo para evitar rebotes
    }
}
