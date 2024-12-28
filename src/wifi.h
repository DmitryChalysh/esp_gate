#ifndef WIFI_H_
#define WIFI_H_

#include "stdint.h"

enum{
    WIFI_MODE_LOCAL,
    WIFI_MODE_GLOBAL
};

typedef enum{
    WIFI_NOT_INIT,
    WIFI_INIT,
    WIFI_STARTED,
    WIFI_CLIENT_CONNECTED,
    WIFI_CLIENT_DISCONNECTED
} wifi_state_t;

void wifi_init(uint8_t wifi_mode);
wifi_state_t wifi_get_currrent_state(void);

#endif