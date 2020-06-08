/*
 */

#include <util/delay.h>


{
    DDRB &= ~(1 << PORTD65);
    detect IR()




    uint8_t detect IR(uint8_t sensor){


        if(PORTD & (<< PORTD65))
        return 1;
        else return 0;
        }

}

