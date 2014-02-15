#include "pebble.h"
#include "lock.h"

uint8_t lock = 0;

void lock_on(void){
  lock = 1;
};

void lock_off(void){
  lock = 0;
};

uint8_t lock_peek(void){
  return lock;
};
