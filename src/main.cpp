#include <Arduino.h>
#include "esp_task_wdt.h"

extern "C" {
#include "i_system.h"
}


extern "C" void jsInit();

void doomEngineTask(void *pvParameters)
{
  esp_task_wdt_delete(xTaskGetCurrentTaskHandle());
  char const *argv[]={"doom","-cout","ICWEFDA", NULL};
  doom_main(3, argv);
}

void setup() {
  Serial.begin(921600);
  Serial.setTimeout(0);
  
  Serial.println("Init done");

  TaskHandle_t task;
  esp_task_wdt_init(99999, false);
  esp_task_wdt_delete(xTaskGetCurrentTaskHandle());
	xTaskCreatePinnedToCore(&doomEngineTask, "doomEngine", 22480, NULL, 5, &task, 0);  
}

void loop() {
  esp_task_wdt_delete(xTaskGetCurrentTaskHandle());
  // put your main code here, to run repeatedly:
  while (1)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
