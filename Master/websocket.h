#ifndef SCHUTZTEC_H
#define SCHUTZTEC_H
#include <Arduino.h>
#define SerialOut Serial
#define SerialIn Serial2
/* structure for save parameters to eeprom */
typedef struct
{
  int r = 0;
  int g = 0;
  int y = 0;
} led;                    // this is using to list object for RGB color and alarm parameters

typedef struct
{
  char host[15] = "smeric";
  char ssid[15] = "smeric";
  char ip[15] = "192.168.137.1";
  char mask[15] = "255.255.255.0";

  char gateway[15] = "192.168.137.1";
  char password[12] = "123454321";
  char serverip[15] = "192.168.137.1";
  int tip = 1; // client
  long port = 3333;
  //  int tip = 0; // client
} webinfo;                             // this is using to IrSensor Properties


typedef struct
{
  char ip[15] = "192.168.1.1";
} devices;                             // this is using to IrSensor Properties

typedef struct parameters
{
  int eepromSet = 0;
  char nameOfProject[13] = "Acil Labs";
  int id = 1;
  int tip = 1;
  unsigned long offset = 4000;
  int ready = 0;
  int state = 0;
  int laser_1 = 0;
  int laser_2 = 0;
  int return_data = 0;
  unsigned long countertime = 0;
  unsigned long lastcountertime = 0;
  devices Devices[4];
  webinfo webInfo;
  led Led;
  int connecedDeviceCount = 0;
} parameters;                   // root of structure for using control and save parameters to eeprom

#define LEDRED_PIN            15  // Red leds control pin
#define LEDGREEN_PIN          0   // Green leds control pin
#define LEDYELLOW_PIN         2   // Blue leds control pin

#define SENSOR_PIN            34
#define LASER_PIN_OUT         5
#define LASER_PIN_IN          4

#define WDT_TIMEOUT 3

#define RXD2 16
#define TXD2 17

#define apclient 1
#define apserver 0

#endif
