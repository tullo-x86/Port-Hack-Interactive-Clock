// Port Hack Interactive Clock - Master
// Copyright Daniel Tullemans 2013
#include <SoftwareSerial.h>

#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>

#include <Time.h>

SoftwareSerial slaveComms(10, 11);

#define BUFFER_LENGTH 20
#define START_PADDING 0
#define END_PADDING 8
#define DATA_LENGTH (BUFFER_LENGTH - START_PADDING - END_PADDING)

unsigned char writeBuffer[BUFFER_LENGTH];
unsigned char *packetData = writeBuffer + START_PADDING;

byte mac[] = { 0x71, 0xCC, 0x70, 0xCC, 0xDE, 0xAD };
byte myIP[] = { 192, 168, 1, 210 };

unsigned int localPort = 123;      // local port to listen for UDP packets

IPAddress timeServer(192, 168, 1, 233); // `perception` wired ethernet

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

unsigned long timeout;

void setup() 
{
    memset(writeBuffer, 0xFE, BUFFER_LENGTH);
    memset(packetData, 0x00, DATA_LENGTH);

    // Wait for the EtherTen to initialise
    delay(3000);

    Ethernet.begin(mac, myIP);
    Udp.begin(localPort);

    slaveComms.begin(9600);

    makeNtpRequest(timeServer); // send an NTP packet to a time server
    timeout = millis() + 3000;
}

void advanceTimeout() {
    timeout += 1000;
}

bool timeHasBeenSet = false;
void loop()
{
    if (Udp.parsePacket())
        setTimeFromResponse();

    if (millis() > timeout)
    {
        advanceTimeout();
        if(timeSet)
            updateSlave();
    }

    delay(1000);
}

void updateSlave()
{
    unsigned long theTime = now();
    memcpy(packetData, &theTime, sizeof(unsigned long));
    slaveComms.write(writeBuffer, BUFFER_LENGTH);
}

void setTimeFromResponse()
{  
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;

    setTime(epoch);
    timeHasBeenSet = true;
}

// send an NTP request to the time server at the given address 
unsigned long makeNtpRequest(IPAddress& address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE); 
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49; 
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:         
    Udp.beginPacket(address, 123); //NTP requests are to port 123

    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket(); 
}
