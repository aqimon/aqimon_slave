#ifndef WIFISERVER_H

#define WIFISERVER_H

#include <avr/pgmspace.h>
#include "wifiCore.h"
#include "../config/config.h"

struct ssid {
	char ssid[32];
	char mac[18];
	unsigned char encryption;
	char rssi;
	unsigned char channel;
};

const PROGMEM char http200[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
const PROGMEM char http200json[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                                   "Connection: close\r\n\r\n";
const PROGMEM char http404[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\nHTTP 404 Not Found";
const PROGMEM char http400[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\nHTTP 400 Bad Request";
const PROGMEM char success[]="{\"result\":\"success\"}";
const PROGMEM char index[] =
    "<!DOCTYPE html><meta content=\"initial-scale=1,user-scalable=no\"name=viewport><ti"
    "tle>Device configuration</title><style>.form-control,body{background-color:#fff}"
    ".btn,button{cursor:pointer}html{font-family:sans-serif;-ms-text-size-adjust:100%%"
    ";-webkit-text-size-adjust:100P%%}button,input,select{color:inherit;font:inherit;ma"
    "rgin:0}button{overflow:visible;-webkit-appearance:button}button,select{text-tran"
    "sform:none}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}*"
    ",:after,:before{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-siz"
    "ing:border-box}html{font-size:10px;-webkit-tap-highlight-color:transparent}body{"
    "margin:0;font-family:Helvetica,Arial,sans-serif;font-size:15px;line-height:1.428"
    "57143;color:#2c3e50}button,input,select{font-family:inherit;font-size:inherit;li"
    "ne-height:inherit}h1{margin:21px 0 10.5px;font-family:Helvetica,Arial,sans-serif"
    ";font-weight:400;line-height:1.1;color:inherit;font-size:39px}.btn,.form-control"
    "{font-size:15px;line-height:1.42857143;background-image:none}.container-fluid{ma"
    "rgin-right:auto;margin-left:auto;padding-left:15px;padding-right:15px}.col-sm-1,"
    ".col-sm-10,.col-sm-2,.col-sm-3,.col-sm-6{position:relative;min-height:1px;paddin"
    "g-left:15px;padding-right:15px}@media (min-width:768px){.col-sm-1,.col-sm-10,.co"
    "l-sm-2,.col-sm-3,.col-sm-6{float:left}.col-sm-10{width:83.33333333%%}.col-sm-6{wi"
    "dth:50%%}.col-sm-3{width:25%%}.col-sm-2{width:16.66666667%%}.col-sm-1{width:8.33333"
    "333%%}}label{display:inline-block;max-width:100%%;margin-bottom:5px;font-weight:70"
    "0}.form-control{display:block;width:100%%;height:45px;padding:10px 15px;color:#2c"
    "3e50;border:1px solid #dce4ec;border-radius:4px;-webkit-transition:border-color "
    "ease-in-out .15s,-webkit-box-shadow ease-in-out .15s;-o-transition:border-color "
    "ease-in-out .15s,box-shadow ease-in-out .15s;transition:border-color ease-in-out"
    " .15s,box-shadow ease-in-out .15s}.form-control:focus{border-color:#2c3e50;outli"
    "ne:0}.form-control::-moz-placeholder{color:#acb6c0;opacity:1}.form-control:-ms-i"
    "nput-placeholder{color:#acb6c0}.form-control::-ms-expand{border:0;background-col"
    "or:transparent}.form-group{margin-bottom:15px}.form-horizontal .form-group{margi"
    "n-left:-15px;margin-right:-15px}@media (min-width:768px){.form-horizontal .contr"
    "ol-label{text-align:right;margin-bottom:0;padding-top:11px}}.btn{display:inline-"
    "block;margin-bottom:0;font-weight:400;text-align:center;vertical-align:middle;-m"
    "s-touch-action:manipulation;touch-action:manipulation;border:1px solid transpare"
    "nt;white-space:nowrap;padding:10px 15px;border-radius:4px;-webkit-user-select:no"
    "ne;-moz-user-select:none;-ms-user-select:none;user-select:none;border-width:2px}"
    ".btn:active:focus,.btn:focus{outline:dotted thin;outline:-webkit-focus-ring-colo"
    "r auto 5px;outline-offset:-2px}.btn:focus,.btn:hover{color:#fff;text-decoration:"
    "none}.btn:active{outline:0;background-image:none;-webkit-box-shadow:none;box-sha"
    "dow:none}.btn-primary{color:#fff;background-color:#2c3e50;border-color:#2c3e50}."
    "btn-primary:focus{color:#fff;background-color:#1a242f;border-color:#000}.btn-pri"
    "mary:active,.btn-primary:hover{color:#fff;background-color:#1a242f;border-color:"
    "#161f29}.btn-primary:active:focus,.btn-primary:active:hover{color:#fff;backgroun"
    "d-color:#0d1318;border-color:#000}.btn-primary:active{background-image:none}.btn"
    "-block{display:block;width:100%%}.container-fluid:after,.container-fluid:before,."
    "form-horizontal .form-group:after,.form-horizontal .form-group:before{content:\" "
    "\";display:table}.container-fluid:after,.form-horizontal .form-group:after{clear:"
    "both}.form-control,input{border-width:2px;-webkit-box-shadow:none;box-shadow:non"
    "e}.form-control:focus,input:focus{-webkit-box-shadow:none;box-shadow:none}</styl"
    "e><div class=container-fluid><h1>Device configuration</h1><br><form class=form-h"
    "orizontal onsubmit=return!1 role=form><div class=form-group><label class=\"contro"
    "l-label col-sm-2\"for=ip>IP address</label><div class=col-sm-10><input class=form"
    "-control id=ip></div></div><div class=form-group><label class=\"control-label col"
    "-sm-2\"for=subnet>Subnet mask</label><div class=col-sm-10><input class=form-contr"
    "ol id=subnet></div></div><div class=form-group><label class=\"control-label col-s"
    "m-2\"for=gateway>Default gateway</label><div class=col-sm-10><input class=form-co"
    "ntrol id=gateway></div></div><div class=form-group><label class=\"control-label c"
    "ol-sm-2\"for=ssid>SSID</label><div class=col-sm-10><select class=form-control id="
    "ssid><option value=a>xxx</select></div></div><div class=form-group><label class="
    "\"control-label col-sm-2\"for=ssidpassword>SSID password</label><div class=col-sm-"
    "10><input class=form-control id=password type=password></div></div><div class=fo"
    "rm-group><label class=\"control-label col-sm-2\"for=host>Host</label><div class=co"
    "l-sm-6><input class=form-control id=host></div><label class=\"control-label col-s"
    "m-1\"for=port>Port</label><div class=col-sm-3><input class=form-control id=port t"
    "ype=number></div></div><div class=form-group><label class=\"control-label col-sm-"
    "2\"for=clientid>Client ID</label><div class=col-sm-10><input class=form-control i"
    "d=clientid></div></div><div class=form-group><label class=\"control-label col-sm-"
    "2\"for=apikey>API key</label><div class=col-sm-10><input class=form-control id=ap"
    "ikey></div></div><button class=\"btn btn-block btn-large btn-primary\"type=button>"
    "Save settings</button></form></div>";

void wifiServerInit();
void wifiServerListener();
void wifiServerSendIndex(FILE* stream);
void wifiServerSend404(FILE* stream);
void wifiServerSendSSID(FILE* stream, char linkID);
void wifiServerSend400(FILE* stream);
void wifiServerSend200(FILE* stream);
void wifiServerSend200JSON(FILE* stream);
void wifiServerSaveSettings(char *httpPath, char linkID);
void wifiServerSendSuccess(FILE* stream);
#endif
