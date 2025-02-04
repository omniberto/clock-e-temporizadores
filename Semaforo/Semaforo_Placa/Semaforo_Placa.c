#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

// Pinos do LED RGB que serão utillizados
const uint32_t R = 13;
const uint32_t G = 11;

// Booleanos utilizados no funcionamento do semáforo
bool RED_ON = true;
bool YELLOW_ON = false;
bool GREEN_ON = false;

// Função temporizada que simula o funcionamento
bool repeating_timer_callback(struct repeating_timer *t){

    // As equações booleanas utilizadas para definir o funcionamento
    YELLOW_ON = RED_ON && !GREEN_ON && !YELLOW_ON;
    GREEN_ON = !RED_ON && !GREEN_ON;
    RED_ON = !GREEN_ON && !YELLOW_ON && !RED_ON; 

    // Colocando os valores no LED
    gpio_put(R, RED_ON || YELLOW_ON);
    gpio_put(G, GREEN_ON || YELLOW_ON);

    return true;
}

int main()
{
    stdio_init_all();

    // Inicializando as cores do LED RGB
    gpio_init(R);
    gpio_set_dir(R, GPIO_OUT);
    gpio_put(R, RED_ON || YELLOW_ON);

    gpio_init(G);
    gpio_set_dir(G, GPIO_OUT);
    gpio_put(G, GREEN_ON || YELLOW_ON);

    // Adicionando Timer
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);    

    // Função main que printará qual o estado atual do semáforo
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
