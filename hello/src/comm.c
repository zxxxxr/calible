#include "pebble.h"

static void send_msg(uint8_t len, Tuplet* data[]) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "SEND_MSG: ERROR NO CHANNEL");
    return;
  }
  for (uint8_t i = 0; i < len; i++){
    dict_write_tuplet(iter, data[i]);
  }
  dict_write_end(iter);
  app_message_outbox_send();
}

static void hook(void *recieved, void *dropped, void *fail){
  app_message_register_inbox_received(received);
  app_message_register_inbox_dropped(dropped);
  app_message_register_outbox_failed(fail);
}

static void open_chan(void){
  if (app_message_open(64, 64) == APP_MSG_OK) {
    //fetch_msg();
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "OPEN_CHAN: OOPS.");
  }
}
