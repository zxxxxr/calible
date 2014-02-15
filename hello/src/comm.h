#pragma once

static void send_error_handler(DictionaryIterator *iter, AppMessageResult reason, void *context);

uint8_t send_msg(uint8_t len, Tuplet** data);

void open_chan(void);

void hook(void *recieved, void *dropped, void *fail);
