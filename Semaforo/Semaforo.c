#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

// Inicializando os LEDs
const uint32_t R = 11;
const uint32_t Y = 12;
const uint32_t G = 13;

// Booleando para o funcionamento do semáforo
bool RED_ON = true;
bool YELLOW_ON = false;
bool GREEN_ON = false;

// Função temporizada.
bool repeating_timer_callback(struct repeating_timer *t){

    // Abaixo os valores esperados das equações booleanas
    YELLOW_ON = RED_ON && !GREEN_ON && !YELLOW_ON;
    // 1 - true and true and true = true | 2 - false and ... = false | 3 - false and ... = false
    GREEN_ON = !RED_ON && (YELLOW_ON || (!YELLOW_ON && !RED_ON)) && !GREEN_ON; 
    // 1 - false and ... = false  | 2 - true and (false or (true and true)) and true = true | 3 - true and (false or (true and true)) and false = false
    RED_ON = !GREEN_ON && !YELLOW_ON && !RED_ON; 
    // 1 - true and false = false | 2 - false and ... = false | 3 - true and true and true = true

    // Pondo os valores atualizados nos LEDs
    gpio_put(R, RED_ON);
    gpio_put(Y, YELLOW_ON);
    gpio_put(G, GREEN_ON);

    return true;
}

int main()
{
    stdio_init_all();

    // Inicializandos os LEDs
    gpio_init(R);
    gpio_set_dir(R, GPIO_OUT);
    gpio_put(R, RED_ON);

    gpio_init(Y);
    gpio_set_dir(Y, GPIO_OUT);
    gpio_put(Y, YELLOW_ON);

    gpio_init(G);
    gpio_set_dir(G, GPIO_OUT);
    gpio_put(Y, GREEN_ON);

    // Adicionando Temporizador
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);    

    // Main, que mostrará qual a cor atual do semáforo
    while (true) {
        printf("O semáforo está: ");
        if(RED_ON)
            printf("VERMELHO\n");
        else if (YELLOW_ON)
            printf("AMARELO\n");
        else if (GREEN_ON)
            printf("VERDE\n");
        
        sleep_ms(1000);
    }
}
