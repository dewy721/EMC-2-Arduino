//UNDER DEVELOPEMENT!!! Do not use yet!
#define useEthernet false

#if useEthernet
  #include <SPI.h>          // needed for Arduino versions later than 0018
  #include <Ethernet.h>
  #include <EthernetUdp.h>  // UDP library from: bjoern@cs.stanford.edu 12/30/2008
  
  // Enter a MAC address and IP address for your controller below.
  // The IP address will be dependent on your local network:
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  IPAddress ip(10,1,1,100);
  
  unsigned int localPort = 9997;              // local port to listen on
  
  // buffers for receiving and sending data
  char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
  char  ReplyBuffer[] = "ok";       // a string to send back
  
  // An EthernetUDP instance to let us send and receive packets over UDP
  EthernetUDP udp;
  
  void ethernetSetup() {
    // start the Ethernet and UDP:
    Ethernet.begin(mac,ip);
    udp.begin(localPort);
  }
#endif

