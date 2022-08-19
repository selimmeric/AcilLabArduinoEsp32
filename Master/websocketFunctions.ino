
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      def.connecedDeviceCount--;
      if (def.connecedDeviceCount == 0) digitalWrite(LEDGREEN_PIN, LOW); else digitalWrite(LEDGREEN_PIN, HIGH);
      SerialOut.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        def.connecedDeviceCount++;
        if (def.connecedDeviceCount == 0) digitalWrite(LEDGREEN_PIN, LOW); else digitalWrite(LEDGREEN_PIN, HIGH);
        IPAddress ip = webSocket.remoteIP(num);
        SerialOut.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client

        webSocket.sendTXT(num, "{\"msg\":\"Connected\"}");
      }
      break;
    case WStype_TEXT:

      wsPoll(payload, length);
      //
      //      int sizeofarg1 = sizeof(payload);
      //      if (strncmp("start", payload, sizeofarg1) == 0) {
      //        def.countertime = millis();
      //        return;
      //      }else if (strncmp("clear", payload, sizeofarg1) == 0) {
      //        def.countertime=0;
      //      }
      //
      //      if (def.return_data == 1)
      //        def.return_data = 0;
      //      else
      //        def.return_data = 1;

      SerialOut.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      SerialOut.printf("[%u] get binary length: %u\n", num, length);
      //  hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
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
