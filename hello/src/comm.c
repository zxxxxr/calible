#include "pebble.h"
#include "comm.h"

void send_msg(uint8_t len, Tuplet** data) {
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

void hook(void *received, void *dropped, void *fail){
  app_message_register_inbox_received(received);
  app_message_register_inbox_dropped(dropped);
  app_message_register_outbox_failed(fail);
}

void open_chan(void){
  if (app_message_open(64, APP_MESSAGE_INBOX_SIZE_MINIMUM) == APP_MSG_OK) {
    //fetch_msg();
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "OPEN_CHAN: OOPS.");
  }
}
