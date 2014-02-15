#include "pebble.h"
#include "defines.h"

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
    char *output = NULL;
    int x_out, y_out, z_out;
    output = malloc(100);

    x_out = sumx / 10;
    y_out = sumy / 10;
    z_out = sumz / 10;
    sumx=0; sumy=0; sumz=0; 
    counter=0;

    snprintf(output, 99, "X: %d, Y: %d, Z: %d,\n T: %d, T_ms: %d", x_out, y_out, z_out, (int)time(NULL), (int)time_ms(NULL, NULL));
    text_layer_set_text(text_layer, output);
    free(output);
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
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
}

static void init(void) {
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
