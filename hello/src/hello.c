#include "pebble.h"
#include "defines.h"
#include "comm.h"
#include "lock.h"

static Window *window;
static GRect window_frame;
static AppTimer *timer;
static TextLayer *text_layer;

int total=0, bad=0;
int counter = 0;
int threshold = 10;
int sumx = 0; int sumy = 0; int sumz = 0;
//double time_last, time_now;
int toggle = 0;
AccelData v_out;
AccelData accel_g;


static void timer_callback(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);
  if(toggle == 1 || (counter < threshold && counter != 0) || lock_peek() != 0){
    timer = app_timer_register(30 /* milliseconds */, timer_callback, NULL); 
  }
  int sign_x = (accel.x > 0 ? 1 : -1);
  int sign_y = (accel.y > 0 ? 1 : -1);
  int sign_z = (accel.z > 0 ? 1 : -1);
  v_out.x += sign_x * ((accel.x - accel_g.x + 50) / 100) * 100;
  v_out.y += sign_y * ((accel.y - accel_g.y + 50) / 100) * 100;
  v_out.z += sign_z * ((accel.z - accel_g.z + 50) / 100) * 100;

  counter++;

  if (counter == threshold) {
    counter = 0;

    Tuplet msg_0 = TupletInteger(0,time(NULL));
    Tuplet msg_1 = TupletInteger(1,time_ms(NULL,NULL));
    Tuplet msg_2 = TupletInteger(2,v_out.x);
    Tuplet msg_3 = TupletInteger(3,v_out.y);
    Tuplet msg_4 = TupletInteger(4,v_out.z);
    Tuplet msg_5 = TupletInteger(5,toggle);
    if(toggle == 0){
      lock_off();
    }
    Tuplet** msg = NULL;
    msg = malloc(6*sizeof(void*));

    msg[0] = &msg_0;
    msg[1] = &msg_1;
    msg[2] = &msg_2;
    msg[3] = &msg_3;
    msg[4] = &msg_4;
    msg[5] = &msg_5;

    
    total++;
    bad += send_msg(6, msg);
    if ((double)bad/total > 0.1) {
      threshold += 5;
      bad = bad/2;
    };
    free(msg);
  }
  
}

static void handle_accel(AccelData *accel_data, uint32_t num_samples) {
  // do nothing
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 62 }, .size = { bounds.size.w, 40 } });
  text_layer_set_text(text_layer,"Press SELECT to start!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  toggle = (toggle + 1) % 2;
  if(toggle == 0){
    lock_on();
    text_layer_set_text(text_layer,"Stopped!");
  }else{
    text_layer_set_text(text_layer,"Measuring...");
    v_out = (AccelData) { .x = 0, .y = 0, .z = 0 };
    accel_g = (AccelData) { .x = 0, .y = 0, .z = 0 };
    accel_service_peek(&accel_g);
    timer = app_timer_register(30 /* milliseconds */, timer_callback, NULL);
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_unload(Window *window) {
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);
  window_set_click_config_provider(window, click_config_provider);

  accel_data_service_subscribe(25, handle_accel);
  
  open_chan(); /* Open a channel to send data */
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
