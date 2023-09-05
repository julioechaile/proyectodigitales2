#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "robot.h"

void app_main(void)
{
  printf("main app starting\n\r");
  TestCreate();   //llama a la funcion que esta en robot.c
  while(1)
  {
  vTaskDelay( 1000 /portTICK_PERIOD_MS );
  }
}