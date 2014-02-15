#pragma once
void send_msg(uint8_t len, Tuplet** data);
void open_chan(void);
void hook(void *recieved, void *dropped, void *fail);
