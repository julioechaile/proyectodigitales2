#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <task.h>
//#include "console.h"
#include "test.h"

void app_main(void)
{
  printf("main app starting\n\r");
  TestCreate();
  vTaskStartScheduler();
  for(;;)
  {
  }
}
