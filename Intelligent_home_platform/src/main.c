#include "main.h"
#include "hardware.h"
#include "actuators.h"
#include "uart.h"
#include "globals.h"

int main()
{
    setupHardware();    
    while (1)
    {
        handleUART();
    }
    return 0;
}




