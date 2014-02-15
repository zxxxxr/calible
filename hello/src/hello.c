#include "pebble.h"

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

static void timer_callback(void *data) {
  char output[100];
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };

  accel_service_peek(&accel);
  
  snprintf(output, 99, "X: %d, Y: %d, Z: %d", accel.x, accel.y, accel.z);
  
  text_layer_set_text(text_layer, output);
  
  timer = app_timer_register(100 /* milliseconds */, timer_callback, NULL);
}

static void handle_accel(AccelData *accel_data, uint32_t num_samples) {
  // do nothing
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  
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

  timer = app_timer_register(100 /* milliseconds */, timer_callback, NULL);
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
