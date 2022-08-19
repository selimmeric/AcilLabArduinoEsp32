static const char TEXT_PLAIN[] PROGMEM = "text/plain";
static const char TEXT_HTML[] PROGMEM = "text/html";
static const char APPLICATION_JSON[] PROGMEM = "application/json";
static const char FS_INIT_ERROR[] PROGMEM = "FS INIT ERROR";
static const char FILE_NOT_FOUND[] PROGMEM = "FileNotFound";
static const char SEPERATOR[] PROGMEM = "||";
//void replyOK(AsyncWebServerRequest *request) {
//  request->send(200, FPSTR(TEXT_PLAIN), "");
//}
//
//void replyJsonMsg(String json,String json,AsyncWebServerRequest *request) {
//  request->send(200, "application/json", json);
//}
//
//void replyOKWithMsg(String msg,AsyncWebServerRequest *request) {
//  request->send(200, FPSTR(TEXT_PLAIN), msg);
//}
//
//void replyNotFound(String msg,AsyncWebServerRequest *request) {
//  request->send(404, FPSTR(TEXT_PLAIN), msg);
//}
//
//void replyBadRequest(String msg,AsyncWebServerRequest *request) {
//  SerialOut.println(msg);
//  request->send(400, FPSTR(TEXT_PLAIN), msg + "\r\n");
//}
//
//void replyServerError(String msg,AsyncWebServerRequest *request) {
//  SerialOut.println(msg);
//  request->send(500, FPSTR(TEXT_PLAIN), msg + "\r\n");
//}
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot(AsyncWebServerRequest *request) {
  const char index_html[] PROGMEM = "<h1>You are connected</h1>"; // large char array, tested with 14k
  request->send_P(200, "text/html", index_html);
}
//void getlast(AsyncWebServerRequest *request) {
//  sprintf(outputString, "%lu", lastkey);
//  request->send(200, "text/plain", outputString);
//}
