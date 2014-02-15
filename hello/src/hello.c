#include "pebble.h"
#include "math.h"

#define MATH_PI 3.141592653589793238462
#define NUM_DISCS 20
#define DISC_DENSITY 0.25
#define ACCEL_RATIO 0.05

static double next_radius = 3;

static Window *window;

static GRect window_frame;

static Layer *disc_layer;

static AppTimer *timer;

static TextLayer *text_layer;

int x_out, y_out, z_out;

static void timer_callback(void *data) {
  //char *output = NULL;
  //output = malloc(100);

  int i, sumx, sumy, sumz;
  //double x_out, y_out, z_out;

  for (i=1; i<=10; i++) {
    AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
    accel_service_peek(&accel);
    
    sumx += accel.x;
    sumy += accel.y;
    sumz += accel.z;
    
    //snprintf(output,99 , "X: %d, Y: %d, Z: %d", accel.x, accel.y, accel.z);
    //text_layer_set_text(text_layer, output);
    //free(output);
  
    timer = app_timer_register(10 /* milliseconds */, timer_callback, NULL);  
  }

  x_out = sumx / 10;
  y_out = sumy / 10;
  z_out = sumz / 10; 
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
