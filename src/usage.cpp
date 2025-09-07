#include <Arduino.h>
#include "esp_timer.h"
#include "esp_task.h"

float getCpuUsage(unsigned long taskElapsed, unsigned long totalElapsed)
{
    return ((float)taskElapsed / totalElapsed) * 100.0;
}

float getMemoryUsage()
{
    size_t freeHeap = ESP.getFreeHeap();
    size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    return ((float)(totalHeap - freeHeap) / totalHeap) * 100.0;
}

const char *getUsage(unsigned long taskElapsed, unsigned long totalElapsed)
{
    static char buffer[64];
    float cpu = getCpuUsage(taskElapsed, totalElapsed);
    float mem = getMemoryUsage();
    snprintf(buffer, sizeof(buffer), "{\"cpu\":%.1f,\"mem\":%.1f}", cpu, mem);
    return buffer;
}
