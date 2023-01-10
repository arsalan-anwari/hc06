#include "hc06.h"

static uint8_t _hc60RxBuff[HC06_RX_BUFF_SIZE];
static uint8_t _hc60MsgBuff[HC06_RX_BUFF_SIZE];
static uint32_t _hc60RxIndex = 0;
static uint8_t _hc60MsgFlag = 0;
static uint8_t _hc60MsgSize = 0;

static HC06Interface* _hc60Interface;

/**
gpio_t rxPin;
gpio_t txPin;
uint16_t baudRate;
mode_t uartMode;
*/

void hc60Init(void){
    uart_init( 
        _hc60Interface.baudRate, 
        _hc60Interface.uartMode, 
        _hc60Interface.txPin, 
        _hc60Interface.rxPin 
    );

    uart_set_interrupt( &_hc60RxIRQ );
}

void hc60SendByte(uint8_t msg){
    while( uart_get_flag() == UART_FLAG_STATUS_RESET ){ yieldThread(); }
    uart_send(msg);
}

void hc60SendBuff(uint8_t* buff, uint8_t size){
    for( uint8_t i=0; i<size; i++ ){
        hc60SendByte(buff[i]);
    }
}

void CreateHc60Device(BluetoothDevice* self, HC06Interface* interface){
    self->init =  &hc60Init;
    self->sendByte = &hc60SendByte;
    self->sendBuff = &hc60SendBuff;
    _hc60Interface = interface;
}

void _hc60RxIRQ(void){
    if ( uart_prepare_irq() ){
        _hc60RXBuff[_hc60RxIndex] = uart_read();

        //check end of rx msg
        if (_hc60RxIndex+1 >= HC06_RX_BUFF_SIZE || _hc60RXBuff[_hc60RxIndex] == '\n' ){
            // store in msg buff
            _hc60MsgFlag = 1;
            for(uint32_t i = 0; i< HC06_RX_BUFF_SIZE; i++){
                if( i <= _hc60RxIndex ){ _hc60MsgBuff[i] = _hc60RXBuff[i]; 
                }else { _hc60MsgBuff[i]=0x00; }
            }

            // format rx buff
            for(uint32_t i = 0; i< HC06_RX_BUFF_SIZE; i++){  _hc60RXBuff[i] = 0x00 }
            _hc60MsgSize = _hc60RxIndex;
            _hc60RxIndex = 0;
        }else{
            _hc60RxIndex++;
        }
    }
    uart_clear()
}
