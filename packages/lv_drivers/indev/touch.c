#include "touch.h"

#if USE_TOUCH != 0

#ifndef MONITOR_ZOOM
#define MONITOR_ZOOM    1
#endif

static bool touch_down = false;
static int16_t last_x = 0;
static int16_t last_y = 0;

void touch_init(void)
{

}

bool touch_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    (void)indev_drv;      /*Unused*/

    /*Store the collected data*/
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = touch_down ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    return false;
}

void touch_handler(void)
{
    touchPosition touch;
    u32 touch_count = hidTouchCount();

    if(touch_count == 0)
    {
        touch_down = false;
    }

    for(int i=0; i<touch_count; i++)
    {
        hidTouchRead(&touch, i);

        touch_down = true;
        last_x = touch.px / MONITOR_ZOOM;
        last_y = touch.py / MONITOR_ZOOM;
    }
}

#endif
