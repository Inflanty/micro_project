#ifndef userLib_h
#define userLib_h

#include <stdbool.h>

#define GPIO_PWM0A 18

struct __connData{
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t char_handle;

    bool connecting;
    bool isConnected;
    bool noConnection;
};

#endif
