#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>
#include <Servo.h>

static uint8_t mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t ip[]  = { 192, 168, 1, 1 };

#define PREFIX "/"
WebServer webserver(PREFIX, 80);

Servo servo;

const int LENGTH_POST_NAME  = 16;
const int LENGTH_POST_VALUE = 16;

const int SVID_ALL = 99;

const boolean SV_ATTACH_TRUE  = true;
const boolean SV_ATTACH_FALSE = false;
const boolean SV_DETACH_TRUE  = true;
const boolean SV_DETACH_FALSE = false;

const int SV_PIN_NUMBER    = 0;
const int SV_ANGLE_ON      = 1;
const int SV_ANGLE_OFF     = 2;
const int SV_ANGLE_NEUTRAL = 3;
const int SV_DELAY_TIME    = 4;

const int SV_SETTING[][5] = {{9, 70, 107, 89, 150}, {3, 109, 65, 88, 150}, {5, 67, 106, 87, 150}, {7, 117, 77, 97, 150}};
const int SV_COUNT = sizeof(SV_SETTING) / sizeof(SV_SETTING[0]);

P(response) =
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<meta charset='UTF-8'>"
  "<title>Switch Controller</title>"
  "<meta name='viewport' content='width=device-width, user-scalable=no'>"
  "<meta name='apple-mobile-web-app-capable' content='yes'>"
  "<link rel='apple-touch-icon-precomposed' href='http://img.f.hatena.ne.jp/images/fotolife/g/georz/20130604/20130604170156.png'>"
  "<style>"
  "* {"
  "margin: 0;"
  "padding: 0;"
  "}"
  ".box {"
  "display: -webkit-box;"
  "display: -moz-box;"
  "display: box;"
  "}"
  ".vbox {"
  "display: -webkit-box;"
  "-webkit-box-orient: vertical;"
  "-webkit-box-align: stretch;"
  "display: -moz-box;"
  "-moz-box-orient: vertical;"
  "-moz-box-align: stretch;"
  "display: box;"
  "box-orient: vertical;"
  "box-align: stretch;"
  "}"
  ".boxcenter {"
  "-webkit-box-pack: center;"
  "-moz-box-pack: center;"
  "box-pack: center;"
  "}"
  ".center {"
  "text-align: center;"
  "}"
  ".switch {"
  "width: 70px;"
  "height: 70px;"
  "}"
  ".switch_all {"
  "width: 142px;"
  "height: 70px;"
  "}"
  ".radius_left {"
  "border-top-left-radius: 15px;"
  "border-bottom-left-radius: 15px;"
  "}"
  ".radius_right {"
  "border-top-right-radius: 15px;"
  "border-bottom-right-radius: 15px;"
  "}"
  "#area_header {"
  "position: relative;"
  "top: 0;"
  "left: 0;"
  "height: 36px;"
  "width: 100%;"
  "background-color: black;"
  "}"
  "#area_header > h1 {"
  "font-size: 1.0em;"
  "color: white;"
  "}"
  "#main {"
  "width: 320px;"
  "margin-left: auto;"
  "margin-right: auto;"
  "}"
  ".row {"
  "margin-top: 10px;"
  "}"
  ".pack {"
  "margin: 8px;"
  "}"
  ".info {"
  "height: 45px;"
  "font-size: 22px;"
  "color: red;"
  "}"
  "hr {"
  "width: 90%;"
  "margin: 0 auto;"
  "}"
  ".display_none {"
  "display: none;"
  "}"
  ".white {"
  "color: #606060;"
  "border: solid 1px #b7b7b7;"
  "background: #fff;"
  "background: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#ededed));"
  "background: -moz-linear-gradient(top,  #fff,  #ededed);"
  "}"
  ".white:hover {"
  "background: #ededed;"
  "background: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#dcdcdc));"
  "background: -moz-linear-gradient(top,  #fff,  #dcdcdc);"
  "}"
  ".white:active {"
  "color: #999;"
  "background: -webkit-gradient(linear, left top, left bottom, from(#ededed), to(#fff));"
  "background: -moz-linear-gradient(top,  #ededed,  #fff);"
  "}"
  ".orange {"
  "color: #fef4e9;"
  "border: solid 1px #da7c0c;"
  "background: #f78d1d;"
  "background: -webkit-gradient(linear, left top, left bottom, from(#faa51a), to(#f47a20));"
  "background: -moz-linear-gradient(top,  #faa51a,  #f47a20);"
  "}"
  ".orange:hover {"
  "background: #f47c20;"
  "background: -webkit-gradient(linear, left top, left bottom, from(#f88e11), to(#f06015));"
  "background: -moz-linear-gradient(top,  #f88e11,  #f06015);"
  "}"
  ".orange:active {"
  "color: #fcd3a5;"
  "background: -webkit-gradient(linear, left top, left bottom, from(#f47a20), to(#faa51a));"
  "background: -moz-linear-gradient(top,  #f47a20,  #faa51a);"
  "}"
  "</style>"
  "</head>"
  "<body>"
  "<header id='area_header' class='vbox boxcenter center'>"
  "<h1>Switch Controller</h1>"
  "</header>"
  "<div id='main' class='vbox'>"
  "<div class='box row'>"
  "<div class='vbox pack'>"
  "Light 1"
  "<div class='box'>"
  "<div class='vbox boxcenter center switch white radius_left' onclick=\"xhrPost('svid=0&swid=2');\">Off</div>"
  "<div class='vbox boxcenter center switch orange radius_right' onclick=\"xhrPost('svid=0&swid=1');\">On</div>"
  "</div>"
  "</div>"
  "<div class='vbox pack'>"
  "Light 2"
  "<div class='box'>"
  "<div class='vbox boxcenter center switch white radius_left' onclick=\"xhrPost('svid=1&swid=2');\">Off</div>"
  "<div class='vbox boxcenter center switch orange radius_right' onclick=\"xhrPost('svid=1&swid=1');\">On</div>"
  "</div>"
  "</div>"
  "</div>"
  "<div class='box row'>"
  "<div class='vbox pack'>"
  "Light 3"
  "<div class='box'>"
  "<div class='vbox boxcenter center switch white radius_left' onclick=\"xhrPost('svid=2&swid=2');\">Off</div>"
  "<div class='vbox boxcenter center switch orange radius_right' onclick=\"xhrPost('svid=2&swid=1');\">On</div>"
  "</div>"
  "</div>"
  "<div class='vbox pack'>"
  "Light 4"
  "<div class='box'>"
  "<div class='vbox boxcenter center switch white radius_left' onclick=\"xhrPost('svid=3&swid=2');\">Off</div>"
  "<div class='vbox boxcenter center switch orange radius_right' onclick=\"xhrPost('svid=3&swid=1');\">On</div>"
  "</div>"
  "</div>"
  "</div>"
  "<hr class='row'>"
  "<div class='box row'>"
  "<div class='vbox boxcenter center switch_all pack white radius_left radius_right' onclick=\"xhrPost('svid=99&swid=2');\">"
  "All Off"
  "</div>"
  "<div class='vbox boxcenter center switch_all pack orange radius_left radius_right' onclick=\"xhrPost('svid=99&swid=1');\">"
  "All On"
  "</div>"
  "</div>"
  "<div id='info' class='vbox boxcenter center row info display_none'>"
  "Sending..."
  "</div>"
  "</div>"
  "<script>"
  "var api_url = 'http://192.168.1.1/';"
  "var xhr = new XMLHttpRequest;"
  "var timer = 0;"
  "var info = document.querySelector('#info');"
  "xhr.onreadystatechange = function() {"
  "if (xhr.readyState == 4) {"
  "info.classList.add('display_none');"
  "if (xhr.status != 200) {"
  "alert('Network Error!');"
  "}"
  "window.clearInterval(timer);"
  "timer = 0;"
  "}"
  "};"
  "function xhrPost(data) {"
  "if (timer) { return; }"
  "info.classList.remove('display_none');"
  "timer = window.setInterval('xhr.abort()', 5000);"
  "xhr.open('POST', api_url);"
  "xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
  "xhr.send(data);"
  "};"
  "</script>"
  "</body>"
  "</html>";

void controlServo(int svId, int swId, boolean svAttachFlag, boolean svDetachFlag)
{
  if (svAttachFlag == true) {
    servo.attach(SV_SETTING[svId][SV_PIN_NUMBER]);
  }

  servo.write(SV_SETTING[svId][swId]);
  delay(SV_SETTING[svId][SV_DELAY_TIME]);

  if (svDetachFlag == true) {
    servo.detach();
  }
}

void controlServoAll(int swId)
{
  int i;
  for (i = 0; i < SV_COUNT; i++) {
    controlServo(i, swId, SV_ATTACH_TRUE, SV_DETACH_TRUE);
  }
}

void controlCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();

  if (type == WebServer::POST) {
    int svId, swId;
    char name[LENGTH_POST_NAME], value[LENGTH_POST_VALUE];

    while (server.readPOSTparam(name, LENGTH_POST_NAME, value, LENGTH_POST_VALUE)) {
      if (strcmp(name, "svid") == 0) {
        svId = atoi(value);
      } else if (strcmp(name, "swid") == 0) {
        swId = atoi(value);
      }
    }
    //Serial.print(svId);
    //Serial.print(swId);

    if (swId == SV_ANGLE_ON || swId == SV_ANGLE_OFF) {
      if (svId >= 0 && svId < SV_COUNT) {
        controlServo(svId, swId, SV_ATTACH_TRUE, SV_DETACH_FALSE);
        controlServo(svId, SV_ANGLE_NEUTRAL, SV_ATTACH_FALSE, SV_DETACH_TRUE);
      } else if (svId == SVID_ALL) {
        controlServoAll(swId);
        controlServoAll(SV_ANGLE_NEUTRAL);
      }
    }
    /* reload using a GET method. */
    //server.httpSeeOther(PREFIX);

  } else if (type == WebServer::GET) {
    server.printP(response);
  }
}

void setup()
{
  //Serial.begin(9600);
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&controlCmd);
  webserver.addCommand("index.html", &controlCmd);
  webserver.begin();
  controlServoAll(SV_ANGLE_NEUTRAL);
}

void loop()
{
  webserver.processConnection();
}
