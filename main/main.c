#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "test.h"

void app_main(void)
{
  printf("main app starting\n\r");
  TestCreate();   //llama a la funcion que esta en test.c
  for(;;)
  {
  }
}