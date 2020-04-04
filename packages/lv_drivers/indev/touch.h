/**
 * @file touch.h
 *
 */

#ifndef TOUCH_H
#define TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_TOUCH

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include <switch.h>

void touch_init(void);
bool touch_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void touch_handler(void);

/**********************
 *      MACROS
 **********************/

#endif /* USE_TOUCH */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TOUCH_H */
