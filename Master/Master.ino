//=============================================
//ESP32 WebSocket Server: potentiometer voltage
//=============================================

#define EEPROM_SIZE 1024
#include "websocket.h"
#include <esp_task_wdt.h>
#include <WiFi.h>
#include "soc/rtc_wdt.h"
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include "Cmd.h"
#include "wsWebSocket.h"
#include <EEPROM.h>
#include "SimpleKalmanFilter.h"


#define SerialOut Serial
//-----------------------------------------------
//const int sensorpin = 34;



//char* ssid = "merics";
//char* password = "sel13mer13";

const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "password";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "type";
const char* PARAM_INPUT_5 = "treshold";

char outputString[127]; // make sure this is big enuffz
void sendEventMsg(int event, char msgInput[127], bool addParam = true);
parameters def = {};
SimpleKalmanFilter kalmanLaser(2, 2, 0.01);
//-----------------------------------------------
//WebServer server(80);
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);
//-----------------------------------------------

String JSONtxt;
//-----------------------------------------------
#include "webpage.h"
//-----------------------------------------------

// Timer interrupts setup
// Timer 0 is using for DAC voice and dont use it for any other job
volatile int interruptCounter;
int totalInterruptCounter;

hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL;
hw_timer_t * timer3 = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux3 = portMUX_INITIALIZER_UNLOCKED;
volatile uint8_t interruptCounter1 = 0;
volatile uint8_t interruptCounter2 = 0;
volatile uint8_t interruptCounter3 = 0;

void IRAM_ATTR onTimer1() {
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux1);
  interruptCounter1++;
  portEXIT_CRITICAL_ISR(&timerMux1);
}
void IRAM_ATTR onTimer2() {
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux2);
  interruptCounter2++;
  portEXIT_CRITICAL_ISR(&timerMux2);
}
void IRAM_ATTR onTimer3() {
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux3);
  interruptCounter3++;
  portEXIT_CRITICAL_ISR(&timerMux3);
}

void handleRoot()
{
  //server.send(200,"text/html", webpageCont);
}

String processor(const String& var) {
  //SerialOut.println(var);
  //  if(var == "BUTTONPLACEHOLDER"){
  //    String buttons = "";
  //    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
  //    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
  //    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
  //    return buttons;
  //  }
  return String();
}

//====================================================================
void setup()
{
  SerialOut.begin(115200);
  cmdInit(&Serial);            // Komut Oluşturma Kütpanesi
  wsInit();

  pinMode(LEDRED_PIN, OUTPUT);
  pinMode(LEDGREEN_PIN, OUTPUT);
  pinMode(LEDYELLOW_PIN, OUTPUT);
  pinMode(LASER_PIN_OUT, OUTPUT);
  pinMode(LASER_PIN_IN, OUTPUT);



  // Seri Port komutları
  cmdAdd("atz", cmd_atz);
  cmdAdd("at+param", cmd_at_setwifi);
  cmdAdd("at+eeprom", cmd_at_eeprom);
  cmdAdd("at+reset", cmd_at_reset);
  cmdAdd("at+laser", cmd_at_laser);
  cmdAdd("at+info", cmd_at_info);

  wsAdd("atz", ws_atz);
  wsAdd("at+param", ws_at_setwifi);
  wsAdd("at+eeprom", ws_at_eeprom);
  wsAdd("at+reset", ws_at_reset);
  wsAdd("at+laser", ws_at_laser);
  wsAdd("at+info", ws_at_info);
  //  SPIFFS.begin();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, def);
  //parameters.eepromSet=0;
  if (def.eepromSet != 40) {
    setDefault();
    EEPROM.put(0, def);
    EEPROM.commit();
  }
  setValues();


  if (def.webInfo.tip == apclient) {

    byte Realip[4];
    byte mask[4];
    byte gatewayIp[4];
    parseBytes(def.webInfo.ip, '.', Realip, 4, 10);
    parseBytes(def.webInfo.mask, '.', mask, 4, 10);
    parseBytes(def.webInfo.gateway, '.', gatewayIp, 4, 10);

    IPAddress local_IP(Realip[0], Realip[1], Realip[2], Realip[3]);
    // Set your Gateway IP address
    IPAddress gateway(gatewayIp[0], gatewayIp[1], gatewayIp[2], gatewayIp[3]);

    IPAddress subnet(mask[0], mask[1], mask[2], mask[3]);

    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }


    WiFi.begin(def.webInfo.ssid, def.webInfo.password);
    digitalWrite(LEDRED_PIN, HIGH);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 50)
    {
      digitalWrite(LEDYELLOW_PIN, !digitalRead(LEDYELLOW_PIN));
      SerialOut.print("wifi baglanamadı"); SerialOut.println(i); delay(500);
      i++;
    }

    if(i==50)digitalWrite(LEDYELLOW_PIN, LOW); else digitalWrite(LEDYELLOW_PIN, HIGH);


    WiFi.mode(WIFI_STA);
    SerialOut.print(" Local IP: ");
    SerialOut.println(WiFi.localIP());

  } else {


    byte Realip[4];
    byte mask[4];
    parseBytes(def.webInfo.ip, '.', Realip, 4, 10);
    parseBytes(def.webInfo.mask, '.', mask, 4, 10);

    WiFi.mode(WIFI_AP_STA);
    IPAddress Ip(Realip[0], Realip[1], Realip[2], Realip[3]);
    IPAddress NMask(mask[0], mask[1], mask[2], mask[3]);
    WiFi.softAPConfig(Ip, Ip, NMask);

    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(def.webInfo.ssid, def.webInfo.password);
    IPAddress myIP = WiFi.softAPIP();
    SerialOut.print("AP IP address: ");
    SerialOut.println(myIP);

    ////////////////////////////////
    // MDNS INIT

    if (MDNS.begin(def.webInfo.host)) {
      MDNS.addService("http", "tcp", 80);
      SerialOut.print(F("Open http://"));
      SerialOut.println(def.webInfo.host);
    }
  }

  //server.on("/", handleRoot);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    SerialOut.print("GPIO: ");
    SerialOut.print(inputMessage1);
    SerialOut.print(" - Set to: ");
    SerialOut.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  server.begin();

  webSocket.begin();
  //  webSocket.begin(def.webInfo.serverip, def.webInfo.serverip, "/");
  webSocket.onEvent(webSocketEvent);

  //  // use HTTP Basic Authorization this is optional remove if not needed
  //  webSocket.setAuthorization("user", "Password");
  //
  //  // try ever 5000 again if connection has failed
  //  webSocket.setReconnectInterval(5000);



  timer1 = timerBegin(1, 80, true); // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer1, &onTimer1, true); // edge (not level) triggered
  timerAlarmWrite(timer1, 1000000, true); // 1000000 * 1 us = 1 s, autoreload true

  timer2 = timerBegin(2, 80, true);  // timer 2, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer2, &onTimer2, true); // edge (not level) triggered
  timerAlarmWrite(timer2, 100000, true); // 750000 * 1 us = 100 ms, autoreload true

  timer3 = timerBegin(3, 80, true);  // timer 3, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer3, &onTimer3, true); // edge (not level) triggered
  timerAlarmWrite(timer3, 250000, true); // 750000 * 1 us = 250 ms, autoreload true

  // at least enable the timer alarms
  timerAlarmEnable(timer1); // enable
  timerAlarmEnable(timer2); // enable
  timerAlarmEnable(timer3); // enable

  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}
//====================================================================

long sensorValue = 0;
float voltage = 0;
long millisZero = 0;
int lastWdt = millis();
void loop()
{
  cmdPoll();
  webSocket.loop();
  sensorValue = analogRead(SENSOR_PIN);
sensorValue = kalmanLaser.updateEstimate(sensorValue);


  if (def.ready == 1) {
    if (sensorValue < def.offset)
    {
      if (def.state == 0) {
        if (def.webInfo.tip > 1) {
          def.countertime = millis();
        }
        def.state = 1;
        JSONtxt = "{\"I\":" + (String)def.id + ", \"S\":1}";
        webSocket.broadcastTXT(JSONtxt);
      }
    } else if (def.state == 1) {
      JSONtxt = "{\"I\":" + (String)def.id + ", \"S\":0}";
      webSocket.broadcastTXT(JSONtxt);
      def.state = 0;
    }
  }

  if (def.countertime > 0) {
    millisZero = millis() - def.countertime;
  }

  if (def.return_data == 1) {
    if (def.tip < 2) {
      JSONtxt = "{\"I\":" + (String)def.id + ", \"L\":" + (String)sensorValue + "}";
      webSocket.broadcastTXT(JSONtxt);
    } else if (def.tip < 3) {
      JSONtxt = "{\"I\":" + (String)def.id + ", \"L\":" + (String)sensorValue + ",\"T\":" + (String)millisZero + "}";
      webSocket.broadcastTXT(JSONtxt);
    }
  }
  if (interruptCounter1 > 0) {
    portENTER_CRITICAL(&timerMux1);
    interruptCounter1--;
    portEXIT_CRITICAL(&timerMux1);
    //  SerialOut.println (sensorValue);
  }
  esp_task_wdt_reset();
}
