#include "pebble.h"

#define MATH_PI 3.141592653589793238462
#define NUM_DISCS 1
#define DISC_DENSITY 0.25
#define ACCEL_RATIO 0.05

typedef struct Vec2d {
  double x;
  double y;
  double z;
} Vec3d;

typedef struct Disc {
  Vec3d pos;
  Vec3d vel;
  double mass;
  double radius;
} Disc;

static Disc discs[NUM_DISCS];

static double next_radius = 3;

static Window *window;

static GRect window_frame;

static Layer *disc_layer;

static AppTimer *timer;

static TextLayer *text_layer;

static double disc_calc_mass(Disc *disc) {
  return MATH_PI * disc->radius * disc->radius * DISC_DENSITY;
}

static void disc_init(Disc *disc) {
  GRect frame = window_frame;
  disc->pos.x = frame.size.w/2;
  disc->pos.y = frame.size.h/2;
  disc->pos.y = 100;
  disc->vel.x = 0;
  disc->vel.y = 0;
  disc->vel.z = 0;
  disc->radius = next_radius;
  disc->mass = disc_calc_mass(disc);
  next_radius += 0.5;
}

static void disc_apply_force(Disc *disc, Vec3d force) {
  char* output;
  output = malloc(100);
  disc->vel.x += force.x / disc->mass;
  disc->vel.y += force.y / disc->mass;
  disc->vel.z += force.z / disc->mass;
  snprintf(output, 99, "Vx: %d, Vy: %d, Vz: %d",(int)(disc->vel.x * 100), (int)(disc->vel.y * 100), (int)(disc->vel.z * 100));  
  text_layer_set_text(text_layer,output);
  free(output);
}

static void disc_apply_accel(Disc *disc, AccelData accel) {
  Vec3d force;
  force.x = accel.x * ACCEL_RATIO;
  force.y = -accel.y * ACCEL_RATIO;
  force.z = accel.z * ACCEL_RATIO;
  disc_apply_force(disc, force);
}

static void disc_update(Disc *disc) {
  const GRect frame = window_frame;
  double e = 0.5;
  if ((disc->pos.x - disc->radius < 0 && disc->vel.x < 0)
    || (disc->pos.x + disc->radius > frame.size.w && disc->vel.x > 0)) {
    disc->vel.x = -disc->vel.x * e;
  }
  if ((disc->pos.y - disc->radius < 0 && disc->vel.y < 0)
    || (disc->pos.y + disc->radius > frame.size.h && disc->vel.y > 0)) {
    disc->vel.y = -disc->vel.y * e;
  }
  disc->pos.z += disc->vel.z;
  disc->pos.x += disc->vel.x;
  disc->pos.y += disc->vel.y;
}

static void disc_draw(GContext *ctx, Disc *disc) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(disc->pos.x, disc->pos.y), disc->radius);
}

static void disc_layer_update_callback(Layer *me, GContext *ctx) {
  for (int i = 0; i < NUM_DISCS; i++) {
    disc_draw(ctx, &discs[i]);
  }
}

static void timer_callback(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };

  accel_service_peek(&accel);

  for (int i = 0; i < NUM_DISCS; i++) {
    Disc *disc = &discs[i];
    disc_apply_accel(disc, accel);
    disc_update(disc);
  }

  layer_mark_dirty(disc_layer);

  timer = app_timer_register(100 /* milliseconds */, timer_callback, NULL);
}

static void handle_accel(AccelData *accel_data, uint32_t num_samples) {
  // do nothing
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect frame = window_frame = layer_get_frame(window_layer);

  disc_layer = layer_create(frame);
  layer_set_update_proc(disc_layer, disc_layer_update_callback);
  layer_add_child(window_layer, disc_layer);

  for (int i = 0; i < NUM_DISCS; i++) {
    disc_init(&discs[i]);
  }
  
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 40 } });
  text_layer_set_text(text_layer,"Vx: 0, Vy: 0, Vz: 0");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  layer_destroy(disc_layer);
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
