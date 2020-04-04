#include <switch.h>
#include <stdlib.h>
#include <stdio.h>

// SDL2
#include <SDL.h>

// lvgl
#include <lvgl/lvgl.h>

// lvgl drivers
#include <lv_drivers/display/monitor.h>
#include <lv_drivers/indev/mouse.h>

// lvgl demo
#include <lv_examples/lv_examples.h>
#include <lv_examples/lv_apps/benchmark/benchmark.h>
#include <lv_examples/lv_apps/demo/demo.h>
#include <lv_examples/lv_apps/sysmon/sysmon.h>
#include <lv_examples/lv_apps/terminal/terminal.h>
#include <lv_examples/lv_apps/tpcal/tpcal.h>

#include <lv_examples/lv_tests/lv_test_theme/lv_test_theme_1.h>
#include <lv_examples/lv_tests/lv_test_theme/lv_test_theme_2.h>

static int tick_thread(void * data)
{
    while(1) 
    {
        SDL_Delay(5);   /*Sleep for 5 millisecond*/
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }
    return 0;
}

static void memory_monitor(lv_task_t * param)
{
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n", (int)mon.total_size - mon.free_size,
            mon.used_pct,
            mon.frag_pct,
            (int)mon.free_biggest_size);
}

static int lvgl_init(void)
{
    lv_init();      /* 初始化lvgl */

    /* Create a display buffer */
    static lv_disp_buf_t disp_buf1;
    static lv_color_t buf1_1[480*10]; 
    lv_disp_buf_init(&disp_buf1, buf1_1, NULL, 480*10);

    /* Create a display */
    monitor_init();
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.buffer = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv);

    /* Set a cursor for the mouse */
    LV_IMG_DECLARE(mouse_cursor_icon);                          /*Declare the image file.*/
    lv_obj_t * cursor_obj =  lv_img_create(lv_disp_get_scr_act(NULL), NULL); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);             /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);               /*Connect the image  object to the driver*/

    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);

    /* Optional:
     * Create a memory monitor task which prints the memory usage in periodically.*/
    lv_task_create(memory_monitor, 3000, LV_TASK_PRIO_MID, NULL);

    return 0; 
}

int main(int argc, char *argv[])
{
    lvgl_init(); 
    // demo_create();

    lv_theme_t *th = lv_theme_material_init(210, LV_FONT_DEFAULT);
    lv_test_theme_1(th); 
    // lv_test_theme_2(); 

    while(1)
    {
        SDL_Event event;

        lv_task_handler();
        svcSleepThread(5000ULL);
        hidScanInput();

        u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if(kDown & KEY_PLUS) 
        {
            break; 
        }

        while(SDL_PollEvent(&event))
        {
            mouse_handler(&event);
        }
    }

    return 0; 
}
