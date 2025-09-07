#pragma once
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

    esp_err_t init_camera(void);

#include "esp_camera.h"
    camera_fb_t *take_picture(void);
    void return_picture(camera_fb_t *fb);
#ifdef __cplusplus
}
#endif