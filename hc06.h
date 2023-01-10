#ifndef HC06_HPP
#define HC06_HPP

#include "../../hal/hal.h"
#include "../../tools/data_types/byte_types.h" 
#include "../bluetooth_device.h"

#include "hc06_settings.h"

typedef struct HC06Interface{
    gpio_t rxPin;
    gpio_t txPin;
    uint16_t baudRate;
    mode_t uartMode;
} HC06Interface;

void hc60Init(void);
void hc60SendByte(uint8_t msg);
void hc60SendBuff(uint8_t* buff, uint8_t size);

void CreateHc60Device(BluetoothDevice* self, HC06Interface* interface);

void _hc60RxIRQ(void);

#endif // HC06_HPP
