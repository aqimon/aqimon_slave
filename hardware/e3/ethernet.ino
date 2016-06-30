/*#include <EtherCard.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t Ethernet::buffer[350];
static byte macAddr[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

const PROGMEM char formatString[]="GET /api/add/event?$H HTTP/1.1\r\n"
                       "Host: 192.168.0.100\r\n"
                     "\r\n";
const char clientID[]="08";


void etherInit(){
  // TODO: Handle static IP
if (ether.begin(sizeof Ethernet::buffer, macAddr) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
    Serial.print("cxxx");
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

}
Stash stash;
void etherSendHTTPRequest(float temperature, float humidity, float coLevel, float dustLevel){
  Serial.println("[ether] send");
  ether.hisip[0]=192;
  ether.hisip[1]=168;
  ether.hisip[2]=0;
  ether.hisip[3]=100;
  ether.hisport=5000;
  char tempStr[6], humidStr[6], dustStr[5], coStr[5];
  dtostrf(temperature, 5, 2, tempStr);
  dtostrf(humidity, 5, 2, humidStr);
  dtostrf(dustLevel, 4, 2, dustStr);
  dtostrf(coLevel, 4, 2, coStr);
  stash.cleanup();
  stash.release();
  Serial.println("[ether] send2");
  uint8_t sd=stash.create();
  stash.print("?clientID=");
  stash.print("xxxx");
  Serial.println("[ether] send3");
  stash.print("&temperature=");
  stash.print(tempStr);
  Serial.println("[ether] send4");
  stash.print("&humidity=");
  stash.print(humidStr);
  Serial.println("[ether] send5");
  stash.print("&dustLevel=");
  stash.print(dustStr);
  Serial.println("[ether] send6");
  stash.print("&coLevel=");
  stash.println(coStr);
  Serial.println("[ether] send7");
  stash.save();
  Serial.println("[ether] send8");
  Stash::prepare(formatString, stash.size(), sd);
  Serial.println("[ether] send9");
  ether.tcpSend();
  Serial.println("[ether] send10");
}

void etherPacketLoop(){
  ether.packetLoop(ether.packetReceive());
}*/
