#include "pebble.h"
#include "defines.h"
#include "comm.h"

static Window *window;
static GRect window_frame;
static AppTimer *timer;
static TextLayer *text_layer;

int counter = 0;
int sumx=0; int sumy=0; int sumz=0;

static void timer_callback(void *data) {

  //char *output = NULL;
  //output = malloc(100);
  
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);
    
  sumx += accel.x;
  sumy += accel.y;
  sumz += accel.z;
    
  //snprintf(output,99 , "X: %d, Y: %d, Z: %d", accel.x, accel.y, accel.z);
  //text_layer_set_text(text_layer, output);
  //free(output);
  counter++;

  if (counter==10) {
    int x_out, y_out, z_out;

    x_out = sumx / 10;
    y_out = sumy / 10;
    z_out = sumz / 10;
    sumx=0; sumy=0; sumz=0; 
    counter=0;

    Tuplet msg_0 = TupletInteger(0,time(NULL));
    Tuplet msg_1 = TupletInteger(1,time_ms(NULL,NULL));
    Tuplet msg_2 = TupletInteger(2,x_out);
    Tuplet msg_3 = TupletInteger(3,y_out);
    Tuplet msg_4 = TupletInteger(4,z_out);
    Tuplet msg_5 = TupletInteger(5,1);

    Tuplet** msg = NULL;
    msg = malloc(6*sizeof(void*));

    msg[0] = &msg_0;
    msg[1] = &msg_1;
    msg[2] = &msg_2;
    msg[3] = &msg_3;
    msg[4] = &msg_4;
    msg[5] = &msg_5;

    
    send_msg(6, msg);
    free(msg);
  }

  timer = app_timer_register(10 /* milliseconds */, timer_callback, NULL);  
}

static void handle_accel(AccelData *accel_data, uint32_t num_samples) {
  // do nothing
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 62 }, .size = { bounds.size.w, 40 } });
  text_layer_set_text(text_layer,"Measuring");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
}

static void init(void) {
  open_chan();
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  accel_data_service_subscribe(0, handle_accel);

  timer = app_timer_register(10 /* milliseconds */, timer_callback, NULL);
}

static void deinit(void) {
  accel_data_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
