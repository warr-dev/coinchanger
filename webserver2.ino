
// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "coinchanger";
const char* password = "c0in1234";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;


const char* host = "www.itexmo.com";
const int httpsPort = 443;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Set your Static IP address
IPAddress local_IP(192, 168, 185, 20);
// Set your Gateway IP address
IPAddress gateway(192, 168, 185, 55);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


String targetnumber="09760730306";
String APIcode="ST-PRINC112245_4CKP3";
String APIpass="fpxya%24k%23%26zs";



void sendsms(String mes="sdds")
{
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  client.setInsecure();


  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/php_api/api.php";
  Serial.print("requesting URL: ");
  Serial.println(url);

//  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" +
//               "User-Agent: BuildFailureDetectorESP8266\r\n" +
//               "Connection: close\r\n\r\n");
 String msg="1=09760730306&2="+mes+"&3="+APIcode+"&passwd="+APIpass;
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
          "Host: " + host + "\r\n" +
          "Accept: */*\r\n" +
          "Content-Type: application/x-www-form-urlencoded\r\n" +
          "Content-Length: "+msg.length()+"\r\n" +
          "\r\n " + msg
           + "\r\n");
           

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void sendmessage(String coinstat,String used){
  Serial.print("used");
  Serial.println(used);
  Serial.print("constats");
  Serial.println(coinstat);
  String message="";
   message="Transaction!%0A";
   message+="%0A";
   message+="Client transaction recorded%0A";
   message+="Transacted Coins: "+used+"PHP%0A";
   message+="Remaining Coins: "+coinstat+"PHP%0A";
   message+="%0A";
   message+="%0A";
   message+="Coin-changer 2021\r\n";
   sendsms(message);
   if(coinstat.toInt()<=20) //alert
   {
     message="Coins alert!%0A";
     message+="%0A";
     message+="you're running out of coins%0A";
     message+="Coins: "+coinstat+"PHP%0A";
     message+="%0A";
     message+="%0A";
     message+="Coin-changer 2021%0A";
     sendsms(message);
   }
}

void setup() {
  Serial.begin(115200);
  
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            int pos=header.indexOf("GET /")+5;
            int pos2=header.indexOf("/",pos+1);
 
            String coinstat=header.substring(pos,header.indexOf("/",pos+1));
            String trans=header.substring(pos2+1,header.indexOf(" ",pos2));
        
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            sendmessage(coinstat,trans);
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
