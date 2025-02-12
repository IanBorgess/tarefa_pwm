#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

// Constantes globais
#define GPIO_PIN 22         // Pino GP22
#define PWM_FREQ 50         // Frequência desejada: 50 Hz
#define PERIOD_US 20000     // Período de 20.000 µs (50 Hz)
#define STEP_US 5           // Passo de 5 µs
#define DUTY_MIN_PCT 0.025  // Duty cycle mínimo: 2,5%
#define DUTY_MAX_PCT 0.12   // Duty cycle máximo: 12%

// Função para configurar o PWM
void pwm_setup(uint gpio_pin, uint32_t freq, uint32_t wrap) {
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM); // Configura o pino como PWM
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin); // Identifica o slice
    float divider = 125000000.0f / (freq * (wrap + 1)); // Calcula o divisor de clock

    // Configura o PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, wrap);
    pwm_config_set_clkdiv(&config, divider);
    pwm_init(slice_num, &config, true);
}

// Função para calcular o duty cycle em função do valor percentual
uint32_t calculate_duty_cycle(uint32_t wrap, float percentage) {
    return (uint32_t)(percentage * (wrap + 1));
}

// Função para definir o duty cycle no canal PWM
void set_pwm_duty_cycle(uint gpio_pin, uint32_t duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);
    uint channel = pwm_gpio_to_channel(gpio_pin);
    pwm_set_chan_level(slice_num, channel, duty_cycle);
}

// Função para mover o servomotor entre ângulos fixos
void move_servo(uint gpio_pin, uint32_t wrap) {
    // Posição 1: 180° (12%)
    set_pwm_duty_cycle(gpio_pin, calculate_duty_cycle(wrap, 0.12));
    sleep_ms(5000);

    // Posição 2: 90° (7,35%)
    set_pwm_duty_cycle(gpio_pin, calculate_duty_cycle(wrap, 0.0735));
    sleep_ms(5000);

    // Posição 3: 0° (2,5%)
    set_pwm_duty_cycle(gpio_pin, calculate_duty_cycle(wrap, 0.025));
    sleep_ms(5000);
}

// Função para mover o servomotor suavemente entre os limites
void move_servo_smoothly(uint gpio_pin, uint32_t wrap, uint32_t step_us) {
    uint32_t duty_min = calculate_duty_cycle(wrap, DUTY_MIN_PCT); // Duty cycle mínimo
    uint32_t duty_max = calculate_duty_cycle(wrap, DUTY_MAX_PCT); // Duty cycle máximo
    uint32_t step_duty = (uint32_t)((step_us / (float)PERIOD_US) * (wrap + 1)); // Passo em valor de duty

    uint32_t duty_current = duty_min; // Começa no mínimo
    int direction = 1;                // Direção inicial: aumentando

    while (true) {
        // Define o duty cycle atual
        set_pwm_duty_cycle(gpio_pin, duty_current);

        // Atualiza o duty cycle
        if (direction == 1) {
            duty_current += step_duty; // Incrementa
            if (duty_current >= duty_max) {
                duty_current = duty_max;
                direction = -1; // Inverte a direção
            }
        } else {
            duty_current -= step_duty; // Decrementa
            if (duty_current <= duty_min) {
                duty_current = duty_min;
                direction = 1; // Inverte a direção
            }
        }

        // Delay de 10 ms entre ajustes
        sleep_ms(10);
    }
}

int main() {
    const uint gpio_pin = GPIO_PIN;       // Pino GP22
    const uint32_t wrap = 65535;          // Resolução de 16 bits

    // Configura o PWM
    pwm_setup(gpio_pin, PWM_FREQ, wrap);

    // Move o servomotor para posições fixas
    move_servo(gpio_pin, wrap);

    // Move o servomotor suavemente entre os limites
    move_servo_smoothly(gpio_pin, wrap, STEP_US);

    return 0;
}