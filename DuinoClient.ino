/*
* DuinoClient
*  
* This sketch connects to a website using an Arduino Wiznet Ethernet shield
* and display content in a LCD display. 
* 
* Circuit:
*  Ethernet shield attached to pins 10, 11, 12, 13
*  LCD Matrix 16x2 attached to pins 
*/

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

IPAddress ip(10,1,1,8);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "hewertho.mn";    // name address

// Initialize the Ethernet client library
EthernetClient client;

// Initialize the LCD Matrix
LiquidCrystal lcd(8,6,5,4,3,2);

// Variables for content 
char city[16];
char temp[11];
char time[5];

boolean hasStats = false; // until this is true default text will be displed

void setup() {
  lcd.begin(16, 2);
  Ethernet.begin(mac, ip);
  delay(1000);
}

void loop() {
  updateInformation();
  
  if(hasStats) {
    printTemperature();
  } else {
    printDefaultText();
  }
  
  delay(5000);
}

/**
 *  Update the status
 */
void updateInformation() {
    if(client.connect(server, 80)) {
      sendRequest(client);
      lcd.println("Connected");
    } else {
      lcd.println("Connection failed");
      return;
    }
    
    extractData(client);
    client.stop();
}

/** 
* Send the request to server
*/
void sendRequest(EthernetClient client) {
  // Make a HTTP request:
  client.println("GET /api/duino/index.php?s=TEMP HTTP/1.1");
  client.println("Host: hewertho.mn");
  client.println("User-Agent: hewerthomn-Arduino");
  client.println("Connection: close");
  client.println();
}

/**
 * Extract the data and set the variables
 */
 void extractData(EthernetClient client) {
   char currentValue[16];
   boolean dataFlag = false; // True if data has started
   boolean endFlag = false; // True if data is reached
   int j = 0;
   int i = 0;

    while(client.connected() && !endFlag) {
      char c = client.read();
      if(c == '<') {
        dataFlag = true;
        hasStats = true;
      }
      else if(dataFlag && c == '>') { // End of data
        setStatValue(j, currentValue);
        endFlag = true;
      }
      else if(dataFlag && c == '|') { // Next dataset
        setStatValue(j++, currentValue);
        char currentValue[7];
        i=0;
      }
      else if(dataFlag) { // Data
        currentValue[i++] = c;
      }
    }   
}
 
/**
* set a simple stat value depending on the position in the string returned
* @param integer position
* @param string value
*/
void setStatValue(int position, char value[]) {
    
  switch(position) {
    case 0:
      for(int i=0; i<16; i++) {
        city[i] = value[i];
      }
      break;
    case 1:
      for(int i=0; i<11; i++) {
        temp[i] = value[i];
      }
   break;
   case 2:
      for(int i=0; i<5; i++) {
        time[i] = value[i];
      }
    break;
  }
}
  
/*
* print the actual temperature
*/
void printTemperature() {
  printCity();
  printTemp();
  printTime();
}

/*
* print the city name
*/
void printCity() {
  lcd.clear();
  lcd.home();
  lcd.println(city);
}
/**
* print the temperature
*/
void printTemp() {
  lcd.setCursor(0,2);
  lcd.print(temp);
}

/*
* print the time
*/
void printTime() {
 lcd.setCursor(11,2);
 lcd.print(time); 
}

/**
* Print a default text
*/ 
void printDefaultText() {
  lcd.home();
  lcd.print("Temperature for");
  lcd.setCursor(0,1);
  lcd.print("hewertho.mn");
  delay(2000);
}
