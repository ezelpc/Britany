/*
* WebServer con el ESP32 para encender diodo led
* de la placa
* Henry Mera
* https://www.todomaker.com 
*/

#include "DHT.h" 
#include <WiFi.h>       // Añade ibreria

String p="REG";         // dato de apagado del led

// Credenciales de wifi 
// Reemplazar con datos de tu red
const char *ssid = "Emiliano's Galaxy S23";
const char *password = "12345678";

// Crea servidor llamado "server" y establece el puerto 80 para acceder
WiFiServer server(80);   

#define DHTPIN 26     // Pin donde está conectado el sensor
#define DHTTYPE DHT11

float temp; // definimos la variable de temperatura

char dato;

int IN1 = 32;
int IN2 = 33;
int ENA = 25; //pwm de ventilador

int bombilla = 27; //Salida al relevador
#define LED 2 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Configura D2 como salida y lo pone en un estado bajo (0)
  dht.begin();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(bombilla, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //Inicia puerto serial a velocidad de 115200 baudios
  Serial.begin(115200);  

  // Inicia conexión WiFi con las datos de la red especificados
  WiFi.begin(ssid, password);     //configurando las credenciales

  Serial.print("Conectandome");
  while (WiFi.status() != WL_CONNECTED)    // Espera conexión a la red
  {
    delay(500);
    Serial.print(".");                      // Imprime puntos 
  }

  Serial.println();
  Serial.print("Conectado, La dirección IP es: ");
  Serial.println(WiFi.localIP());                    // Imprime la IP del ESP32
 
  server.begin();                                   // Inicia el servidor
  Serial.println("Servidor iniciado");
}

long tiempoUltimaLectura=0; //Para guardar el tiempo de la última lectura

void loop() {
  WiFiClient client = server.available();   // Recibe las conexciones de clientes

  if (client) {                             // Si hay un cliente
    Serial.println("Nuevo cliente.");       // Indica el acceso de un cliente
    String currentLine = "";                // Variable para datos de fin de linea
    while (client.connected()) {            // Cliente conectado
      if (client.available()) {             // Datos disponibles para ser leido

        char c = client.read();             // Lectura de byte en variable C
        Serial.write(c);                    // Muestra los datos
        
        if (c == '\n') {                    // Si el byte es un caracter de nuevo salto de linea

          if (currentLine.length() == 0) {    // Si no hay caracteres, entonces lanza el codgo HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html{font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {border: none;color: white;padding: 15px 32px; text-align: center;");
            client.println("text-decoration: none;display: inline-block;font-size: 16px; margin: 4px 2px;cursor: pointer;}");
            client.println(".button1 {background-color: #4CAF50;} /* Green */");
            client.println(".button2 {background-color: #008CBA;} /* Blue */");
            client.println(".button3 {background-color: #FF0000;} /* Red */");
            client.println("</style></head>");
            
            // Creación de botones 
            client.print("<body><h1>WebServer con ESP32</h1>");
            
              if(millis()-tiempoUltimaLectura>2000)
                {    
                  temp = dht.readTemperature(); //Leemos la temperatura en grados Celsius
                }
              tiempoUltimaLectura=millis();

              String response="<p>"+String(temp) +"</p><br><br>";            
              client.println(response);                        
              
              
              client.println("<button type='button' class='button button3' onClick=location.href='/LED=REG'> Sensores </button><br><br>");

              client.println("<button type='button' class='button button1' onClick=location.href='/LED=ON'> Foco </button><br><br>");
            
              client.println("<button type='button' class='button button2' onClick=location.href='/LED=OFF'> Ventilador </button><br><br>");                                                                           
         
            client.print("</body></html>");
            client.println();
            
            // Rompe el codigo del while-loop
            break;
          } else {    // Limpiando variable
            currentLine = "";
          }
        } else if (c != '\r') {  // Si no hay retorno de carro
          currentLine += c;      // agrega al final de la linea
        }

        // Revisando el datos recibido del url
        if (currentLine.indexOf("GET /LED=ON") != -1) {
          digitalWrite(bombilla, LOW);               // GET /LED=ON
          analogWrite(ENA, 0);
          p="on";
        }
        if (currentLine.indexOf("GET /LED=OFF") != -1) {
          digitalWrite(bombilla, HIGH);                // GET /LED=OFF
          analogWrite(ENA, 191); //Encendemos el ventilador al 50%
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          p="off";
        }
        if(currentLine.indexOf("GET /LED=REG") != -1){
              
          analogWrite(ENA, 0);
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);      
          if ( temp >= 27)
          { 
            Serial.println("Ventiladores Encendidos al 70%");
            analogWrite(ENA, 191); //Encendemos el ventilador al 50%
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            if( temp >= 27)
            {
              Serial.println("Ventiladores Encendidos al 100%");
              analogWrite(ENA, 255); //Encendemos el ventilador al 100%
              digitalWrite(IN1, LOW);
              digitalWrite(IN2, HIGH);
                    
            }
          }


                  //Condición para subir la temperatura.
                  if ( temp < 26 )
                  {
                    
                    Serial.println("Bombilla Encendida");
                    digitalWrite(bombilla, LOW);
                }
                else{
                digitalWrite(bombilla, HIGH);
                }
                p="REG";

            }
      }
    }
    // cerrando la conexión
    client.stop();
    Serial.println("Cliente Desconectado");
  }
}