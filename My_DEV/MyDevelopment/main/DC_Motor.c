
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "DC_Motor.h"

int var = 0;


void init_IO_Motors(void)
{
  //Resetting
  //gpio_pad_select_gpio(uint8_t gpio_num);
  gpio_reset_pin(VEHICLE_FRONT_LEFT_MOTOR_1);
  gpio_reset_pin(VEHICLE_FRONT_LEFT_MOTOR_2);
  gpio_reset_pin(VEHICLE_FRONT_RIGHT_MOTOR_1);
  gpio_reset_pin(VEHICLE_FRONT_RIGHT_MOTOR_2);
  gpio_reset_pin(VEHICLE_BACK_LEFT_MOTOR_1);
  gpio_reset_pin(VEHICLE_BACK_LEFT_MOTOR_2);
  gpio_reset_pin(VEHICLE_BACK_RIGHT_MOTOR_1);
  gpio_reset_pin(VEHICLE_BACK_RIGHT_MOTOR_2);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(VEHICLE_FRONT_LEFT_MOTOR_1, GPIO_MODE_OUTPUT);
  gpio_set_direction(VEHICLE_FRONT_LEFT_MOTOR_2, GPIO_MODE_OUTPUT);

  gpio_set_direction(VEHICLE_FRONT_RIGHT_MOTOR_1, GPIO_MODE_OUTPUT);
  gpio_set_direction(VEHICLE_FRONT_RIGHT_MOTOR_2, GPIO_MODE_OUTPUT);

  gpio_set_direction(VEHICLE_BACK_LEFT_MOTOR_1, GPIO_MODE_OUTPUT);
  gpio_set_direction(VEHICLE_BACK_LEFT_MOTOR_2, GPIO_MODE_OUTPUT);

  gpio_set_direction(VEHICLE_BACK_RIGHT_MOTOR_1, GPIO_MODE_OUTPUT);
  gpio_set_direction(VEHICLE_BACK_RIGHT_MOTOR_2, GPIO_MODE_OUTPUT);
}

void Move_Front(void)
{
    gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_1, TRUE);
    gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_2, FALSE);

    gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_1, TRUE);
    gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_2, FALSE);

    gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_1, TRUE);
    gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_2, FALSE);

    gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_1, TRUE);
    gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_2, FALSE);
}

void Move_Back(void)
{
    gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_1, FALSE);
    gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_2, TRUE);

    gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_1, FALSE);
    gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_2, TRUE);

    gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_1, FALSE);
    gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_2, TRUE);

    gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_1, FALSE);
    gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_2, TRUE);
}

void Move_Left(void)
{
  gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_2, TRUE);

  gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_2, TRUE);

  gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_1, TRUE);
  gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_2, FALSE);

  gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_1, TRUE);
  gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_2, FALSE);
}


void Move_Right(void)
{
  gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_1, TRUE);
  gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_2, FALSE);

  gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_1, TRUE);
  gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_2, FALSE);

  gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_2, TRUE);

  gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_2, TRUE);
}

void Move_Stop(void)
{
  gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_FRONT_LEFT_MOTOR_2, FALSE);

  gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_FRONT_RIGHT_MOTOR_2, FALSE);

  gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_BACK_LEFT_MOTOR_2, FALSE);

  gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_1, FALSE);
  gpio_set_level(VEHICLE_BACK_RIGHT_MOTOR_2, FALSE);
}
