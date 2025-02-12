# 🤖 Controle de Servomotor com Raspberry Pi Pico

Este projeto demonstra como controlar um servomotor utilizando um Raspberry Pi Pico W. O código configura um sinal PWM para mover o servomotor entre posições fixas (0°, 90°, 180°) e também realiza um movimento suave entre os limites mínimo e máximo do servo.

## 🛠️ Componentes Necessários

- **Raspberry Pi Pico** �
- **Servomotor** (compatível com PWM de 50 Hz) 🎚️
- **Wokwi Simulator** (simulador utilizado para testes)🔌

## 📝 Descrição do Projeto
O objetivo deste projeto é controlar um servomotor conectado ao Raspberry Pi Pico utilizando sinais PWM. O código foi modularizado para facilitar a leitura e manutenção, permitindo que o servomotor execute movimentos fixos e suaves.

- **Frequência PWM:** 50 Hz (período de 20 ms).
- **Resolução PWM:** 16 bits (wrap = 65535).
- **Duty Cycle Mínimo:** 2,5% (0°).
- **Duty Cycle Máximo:** 12% (180°).

## 🚀 Funcionalidades do Código

### 🎛️ Configuração do PWM
- O código configura o pino GP22 para gerar um sinal PWM com frequência de 50 Hz.
- O período do PWM é definido como 20.000 µs (50 Hz), e o duty cycle varia entre 2,5% e 12% para controlar o servomotor.

### 🎯 Movimento para Posições Fixas
- O servomotor é movido para três posições fixas:
  - **180°** (duty cycle de 12%)
  - **90°** (duty cycle de 7,35%)
  - **0°** (duty cycle de 2,5%)

### 🌊 Movimento Suave
- O servomotor se move suavemente entre os limites mínimo e máximo, com um passo de 5 µs.

## 📝 Estrutura do Código

- **`pwm_setup`**: Configura o PWM no pino especificado.
- **`calculate_duty_cycle`**: Calcula o valor do duty cycle com base na porcentagem.
- **`set_pwm_duty_cycle`**: Define o duty cycle no canal PWM.
- **`move_servo`**: Move o servomotor para posições fixas.
- **`move_servo_smoothly`**: Move o servomotor suavemente entre os limites.