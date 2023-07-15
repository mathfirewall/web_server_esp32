#include <WiFi.h>

const char* ssid = "";
const char* password = "";
WiFiServer server(80);
String header;   // armazena o request HTTP

// Estado atual do LED1
bool led1Status = false;
bool led2Status = false;

// Pinos GPIOs
const int LED1_Pin = 26;
const int LED2_Pin = 27;
const int LED_WIFI = 2;

void setup() {
  Serial.begin(115200);
  pinMode(LED1_Pin, OUTPUT);
  pinMode(LED2_Pin, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);
  digitalWrite(LED1_Pin, LOW);

  // Conexao com a rede Wi-Fi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Led 2 Belink ativa quando o Wifi é conectado com sucesso
  digitalWrite(LED_WIFI, HIGH);
  // Imprime o endereco IP local e inicia o Servidor Web
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();
  if (client) {                    
    while (client.connected()) { 
        if (client.available()) {
            char c = client.read();  
            header += c;
            if (c == '\n') {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();

                // ajusta os GPIOs on e off
                if (header.indexOf("GET /LED1/on") >= 0) {
                    Serial.println("LED1 on");
                    led1Status = true;
                    digitalWrite(LED1_Pin, HIGH); 
                } else if (header.indexOf("GET /LED1/off") >= 0) {
                    Serial.println("LED1 off");
                    led1Status = false;
                    digitalWrite(LED1_Pin, LOW); 
                }
                if(header.indexOf("GET /LED2/on") >= 0){
                    Serial.println("LED2 on");
                    led2Status = true;
                    digitalWrite(LED2_Pin, HIGH); 
                }
                else if(header.indexOf("GET /LED2/off") >= 0){
                    Serial.println("LED2 on");
                    led2Status = false;
                    digitalWrite(LED2_Pin, LOW); 
                }
                // Gera a pagina HTML
                client.println("<html>");
                client.println("<body style='margin: 0px;'><div style='width: 100vw; height: 100vh; background: #6C7A89; display: flex; flex-direction: row; justify-content: center; align-items: center;'><div style='width: 700px; height: 450px; padding:10px; background: #fff; opacity:0.5; border-radius: 8px;'><h1 style='color:#4b2056; '>ESP32 WEB SERVER DEVTITANS</h1> <img style='width: 100px; height: 100px; float:right; margin-top: -60px;' src='https://devtitans.icomp.ufam.edu.br/moodle/pluginfile.php/1/core_admin/logo/0x200/1680546897/moodle-site-logo.png'>");
                if (led1Status) {
                    client.println("<p>LED1 - State ON</p>");
                    client.println("<p><a href=\"/LED1/off\"><button style='background-color:green; color:white; width:100px; height:60px; border-radius: 10px;' class=\"button\">OFF</button></a></p>");
                }
                else {
                    client.println("<p>LED1 - State OFF</p>");
                    client.println("<p><a href=\"/LED1/on\"><button style='background-color:red; color:white; width:100px; height:60px; border-radius: 10px;' class=\"button\">ON</button></a></p>");
                }

                if (led2Status) {
                    client.println("<p>LED2 - State ON</p>");
                    client.println("<p><a href=\"/LED2/off\"><button style='background-color:green; color:white; width:100px; height:60px; border-radius: 10px;' class=\"button\">OFF</button></a></p>");
                }
                else {
                    client.println("<p>LED2 - State OFF</p>");
                    client.println("<p><a href=\"/LED2/on\"><button style='background-color:red; color:white; width:100px; height:60px; border-radius: 10px;' class=\"button\">ON</button></a></p>");
                }
                client.println("</div></div></body></html>");
                header = "";
                // Fecha a conexao
                client.stop();
            } // if nova linha 
        } // if cliente disponivel
    } // if cliente conectado
  } // if cliente se conectou
} // loop
