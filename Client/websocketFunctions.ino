
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*) mem;
  SerialOut.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for(uint32_t i = 0; i < len; i++) {
    if(i % cols == 0) {
      SerialOut.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    SerialOut.printf("%02X ", *src);
    src++;
  }
  SerialOut.printf("\n");
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
     // USE_SERIAL.printf("[WSc] Disconnected!\n");
      digitalWrite(LEDGREEN_PIN, LOW);
      break;
    case WStype_CONNECTED:
    digitalWrite(LEDGREEN_PIN, HIGH);
      SerialOut.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      webSocket.sendTXT("Connected");
      break;
    case WStype_TEXT:
       SerialOut.printf("[WSc] get text: %s\n", payload);
wsPoll(payload, length);
      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
       SerialOut.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
      break;
  }

}
//void webSocketEventServer(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//
//  switch (type) {
//    case WStype_DISCONNECTED:
//      def.connecedDeviceCount--;
//      if (def.connecedDeviceCount == 0) digitalWrite(LEDGREEN_PIN, LOW); else digitalWrite(LEDGREEN_PIN, HIGH);
//      SerialOut.printf("[%u] Disconnected!\n", num);
//      break;
//    case WStype_CONNECTED:
//      {
//        def.connecedDeviceCount++;
//        if (def.connecedDeviceCount == 0) digitalWrite(LEDGREEN_PIN, LOW); else digitalWrite(LEDGREEN_PIN, HIGH);
//        IPAddress ip = webSocket.remoteIP(num);
//        SerialOut.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
//        // send message to client
//
//        webSocket.sendTXT("{\"msg\":\"Connected\"}");
//      }
//      break;
//    case WStype_TEXT:
//
//      wsPoll(payload, length);
//      //
//      //      int sizeofarg1 = sizeof(payload);
//      //      if (strncmp("start", payload, sizeofarg1) == 0) {
//      //        def.countertime = millis();
//      //        return;
//      //      }else if (strncmp("clear", payload, sizeofarg1) == 0) {
//      //        def.countertime=0;
//      //      }
//      //
//      //      if (def.return_data == 1)
//      //        def.return_data = 0;
//      //      else
//      //        def.return_data = 1;
//
//      SerialOut.printf("[%u] get Text: %s\n", num, payload);
//
//      // send message to client
//      // webSocket.sendTXT(num, "message here");
//
//      // send data to all connected clients
//      // webSocket.broadcastTXT("message here");
//      break;
//    case WStype_BIN:
//      SerialOut.printf("[%u] get binary length: %u\n", num, length);
//      //  hexdump(payload, length);
//
//      // send message to client
//      // webSocket.sendBIN(num, payload, length);
//      break;
//    case WStype_ERROR:
//    case WStype_FRAGMENT_TEXT_START:
//    case WStype_FRAGMENT_BIN_START:
//    case WStype_FRAGMENT:
//    case WStype_FRAGMENT_FIN:
//    case WStype_PING:
//    case WStype_PONG:
//      break;
//  }
//}
