#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

const uint32_t R = 11;
const uint32_t Y = 12;
const uint32_t G = 13;

bool RED_ON = true;
bool YELLOW_ON = false;
bool GREEN_ON = false;

bool repeating_timer_callback(struct repeating_timer *t){

    YELLOW_ON = RED_ON && !GREEN_ON && !YELLOW_ON;
     // 1 - true and true and true = true | 2 - false and ... = false | 3 - false and ... = false
    GREEN_ON = !RED_ON && (YELLOW_ON || (!YELLOW_ON && !RED_ON)) && !GREEN_ON; 
    // 1 - false and ... = false  | 2 - true and (false or (true and true)) and true = true | 3 - true and (false or (true and true)) and false = false
    RED_ON = !GREEN_ON && !YELLOW_ON && !RED_ON; 
    // 1 - true and false = false | 2 - false and ... = false | 3 - true and true and true = true

    gpio_put(R, RED_ON);
    gpio_put(Y, YELLOW_ON);
    gpio_put(G, GREEN_ON);

    return true;
}

int main()
{
    stdio_init_all();

    gpio_init(R);
    gpio_set_dir(R, GPIO_OUT);
    gpio_put(R, RED_ON);

    gpio_init(Y);
    gpio_set_dir(Y, GPIO_OUT);

    gpio_init(G);
    gpio_set_dir(G, GPIO_OUT);

    struct repeating_timer timer;

    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);    

    while (true) {
        printf("O semafóro está: ");
        if(RED_ON)
            printf("VERMELHO\n");
        else if (YELLOW_ON)
            printf("AMARELO\n");
        else if (GREEN_ON)
            printf("VERDE\n");
        
        sleep_ms(1000);
    }
}
