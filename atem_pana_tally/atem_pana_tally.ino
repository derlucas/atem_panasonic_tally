#include <SPI.h>
#include <Ethernet.h>
#include <ATEMbase.h>
#include <ATEMstd.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };
IPAddress ip(192,168,10,238);
IPAddress atemIp(192, 168, 10, 241);

ATEMstd AtemSwitcher;
bool AtemOnline = false;
int pushButton = 0;


void handleTally() {
  digitalWrite(2, !AtemSwitcher.getProgramTally(4));
  digitalWrite(3, !AtemSwitcher.getProgramTally(5));
  digitalWrite(4, !AtemSwitcher.getProgramTally(6));
  /*
  Serial.print("tally 3: ");
  Serial.print(AtemSwitcher.getProgramTally(3));
  Serial.print(" tally 4: ");
  Serial.print(AtemSwitcher.getProgramTally(4));
  Serial.print(" tally 6: ");
  Serial.print(AtemSwitcher.getProgramTally(6));

  Serial.print("  programm input: ");
  Serial.print(AtemSwitcher.getProgramInput());

  Serial.println(" "); */
}



void setup() {

  for(int i = 2; i<=6; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
  Serial.begin(115200);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Initialize a connection to the switcher:
  AtemSwitcher.begin(atemIp, 56331);
  AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();
  
}



void loop() {

  AtemSwitcher.runLoop();
  
  if (AtemSwitcher.hasInitialized())  {
    if (!AtemOnline)  {
      AtemOnline = true;      
    }
    
    handleTally();
    
  } else {
    // at this point the ATEM is not connected and initialized anymore

    if (AtemOnline) {
      AtemOnline = false;
        
      // turn off the green connection idicator

      Serial.println("conn timeout");
    }     
  }
  
}
