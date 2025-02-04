#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

// Declarando os componentes (LEDs e Botão)
const uint GREEN = 11;
const uint BLUE = 12;
const uint RED = 13;
const uint BUTTON = 5;

// Variável para determinar a funcionalidade do botão
bool ACTIVE_CALLBACK = true;

// Função utilizada para desligar os LEDs e Ativar o Callback
int64_t turn_off_callback(alarm_id_t id, void *user_data){

    // A função receberá os estados atuais dos LEDs
    bool GREEN_STATE = gpio_get(GREEN);
    bool BLUE_STATE = gpio_get(BLUE);
    bool RED_STATE = gpio_get(RED);

    // Se o primeiro LED está ativo, ele é desligado e cria um novo alarme para chamar esta função pros próximos LEDs
    if(GREEN_STATE){
        gpio_put(GREEN, false);
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    }

    // Se o segundo está ligado, desliga-se o LED e cria-se novamente um novo alarme para chamar esta função para desligar o último LED
    else if(BLUE_STATE){
        gpio_put(BLUE, false);
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    }

    // Se somente o último está ligado, desliga-se o LED e ativa a chamada da função através do botão novamente.
    else {
        gpio_put(RED, false);
        ACTIVE_CALLBACK = true;
    }
    return 0;
}
int main()
{

    stdio_init_all();

    // Iniciando os componentes
    // LEDs
    gpio_init(GREEN);
    gpio_set_dir(GREEN, GPIO_OUT);

    gpio_init(BLUE);
    gpio_set_dir(BLUE, GPIO_OUT);

    gpio_init(RED);
    gpio_set_dir(RED, GPIO_OUT);

    // Botão
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    while (true) {
        printf("O botão está ");
        if(ACTIVE_CALLBACK)
            printf("ativo\n");
        else
            printf("inativo\n");
        // Se o botão for pressionado e puder executar a função
        if(!gpio_get(BUTTON) && ACTIVE_CALLBACK){
            sleep_ms(50); // Delay para debouncing

            // Liga todos os LEDs
            gpio_put(GREEN, true);
            gpio_put(BLUE, true);
            gpio_put(RED, true);
            
            // Desativa a execução da função pro botão
            ACTIVE_CALLBACK = false;

            // Cria-se o alarme para desligar os LEDs
            add_alarm_in_ms(3000, turn_off_callback, NULL, false);
            sleep_ms(200);
        }
        // Delay para melhor funcionamento da CPU
        sleep_ms(1000);
    }
}
