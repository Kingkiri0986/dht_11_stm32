#include "stm32f1xx_hal.h"

#define DHT_PORT GPIOA
#define DHT_PIN  GPIO_PIN_1

uint8_t data[5];

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

void set_output(void)
{
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = DHT_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT_PORT, &gpio);
}

void set_input(void)
{
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = DHT_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT_PORT, &gpio);
}

uint8_t DHT11_Read(void)
{
    uint8_t i, j;

    set_output();
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 0);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 1);
    delay_us(30);
    set_input();

    for (j = 0; j < 5; j++)
    {
        data[j] = 0;
        for (i = 0; i < 8; i++)
        {
            while (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
            delay_us(40);
            if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
                data[j] |= (1 << (7 - i));
            while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
        }
    }
    return 1;
}
