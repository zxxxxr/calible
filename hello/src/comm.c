#include "pebble.h"
#include "comm.h"

static void send_error_handler(DictionaryIterator *iter, AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SEND_MSG: %d", reason);
}

uint8_t send_msg(uint8_t len, Tuplet** data) {
  DictionaryIterator *iter;
  switch(app_message_outbox_begin(&iter)){
    case APP_MSG_BUSY:
       APP_LOG(APP_LOG_LEVEL_WARNING, "SEND_MSG: E_BUSY");
       break;
    case APP_MSG_INVALID_ARGS:
       APP_LOG(APP_LOG_LEVEL_WARNING, "SEND_MSG: E_INVALID_ARGS");
       break;
    case APP_MSG_OK:
       break;
    default:
       break;
  }
  if (iter == NULL) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "SEND_MSG: NO CHANNEL");
    return 1;
  }
  for (uint8_t i = 0; i < len; i++){
    dict_write_tuplet(iter, data[i]);
  }
  dict_write_end(iter);
  app_message_outbox_send();
  return 0;
}

void hook(void *received, void *dropped, void *fail){
  app_message_register_inbox_received(received);
  app_message_register_inbox_dropped(dropped);
  app_message_register_outbox_failed(fail);
}

void open_chan(void){
  if (app_message_open(64, APP_MESSAGE_INBOX_SIZE_MINIMUM) == APP_MSG_OK) {
    //fetch_msg();
    hook(NULL, NULL, send_error_handler);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "OPEN_CHAN: OOPS.");
  }
}
