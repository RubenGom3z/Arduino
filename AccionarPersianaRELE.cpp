/*Este codigo hostea una pagina web en el arduino NodeMCU con la cual podemos interactuar para poder bajar, subir o parar una persiana gracias a un rele

#include <ESP8266WiFi.h> /* Importamos las librerias*/
#include <RCSwitch.h> 

WiFiClient client; 
WiFiServer server(80); /*Decimos por que puerto va a sacar la pagina web en este caso el 80*/

const char* ssid = "AULA17"; /*SSID de la red WIFI*/
const char* password = "Cctv1234"; /* Contraseña del WIFI*/

String  data =""; 

int pinsubirv = 14; /* GPIO14(D5) -> Subir persiana */
int pinbajarv = 12;  /* GPIO12(D6) -> Bajar persiana */


RCSwitch mySwitch = RCSwitch(); /* Hacemos que RCSswitch se llame mySwitch*/

IPAddress staticIP(192,168,17,80); /* En este parte del codigo lo que hacemos es darle una IP estatica*/
IPAddress gateway(192,168,17,100); /* Le decimos el gateway que vamos a tener dentro de la red*/
IPAddress subnet(255,255,255,0);

void setup()
{
  mySwitch.enableTransmit (13); /* Declaramos el pin 13 como pin de envio de datos*/
  pinMode(pinsubirv, OUTPUT); /*Declaramos el pin de subida como output*/
  pinMode(pinbajarv, OUTPUT); /*Declaramos el pin de bajada como output*/
  
  digitalWrite(pinsubirv, HIGH); /* Hacemos que cuando inicie los pines los ponga en alto (asi el rele no se activa)*/
  digitalWrite(pinbajarv, HIGH);
 
  Serial.begin(115200);
  connectWiFi();
  server.begin(); /*Inicia la pagina WEB*/
  
}

void loop()
{
    client = server.available();
    if (!client) return; 
    data = client.readStringUntil('\r');
    client.flush();

    if (data.indexOf("subirv") != -1) subirv(); /*Si el index tiene uno de los valores ejecuta la funcion asociada a el */
    else if (data.indexOf("bajarv") != -1)bajarv();
    else if (data.indexOf("pararv") != -1) pararv();
    else if (data.indexOf("subirp") != -1) subirp();
    else if (data.indexOf("bajarp") != -1) bajarp();
    else if (data.indexOf("pararp") != -1) pararp();
    
  
  /* Web Browser */
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>NodeMCU v2</title>");
    client.println("</head>");
    client.println("<body>");
    client.println("<center>");
     client.println("<h1>2 S.T.I.</h1>");
     client.println("<h1>I.E.S. Ciudad de Haro</h1>");
    client.println("<h2> WiFi CONTROLLED CAR via ESP8266 </h2>");
    client.println("<br><br>");
    client.println("<hr>");

    /* Botones */
    client.println("<br><br>");
    client.println("<a href=\"/subirv\"\"><button>SubirVentana </button></a>"); /*Aqui lo que hacemos es crear los botones y asociales un valor con los cuales ejecutaremos funciones*/
    client.println("<a href=\"/subirp\"\"><button>SubirProyector </button></a>");
    client.println("<br><br>");
    client.println("<a href=\"/pararv\"\"><button>PararVentana </button></a>");
    client.println("<a href=\"/pararp\"\"><button>PararProyector </button></a>");
    client.println("<br><br>");
    client.println("<a href=\"/bajarv\"\"><button>BajarVentana </button></a>");
    client.println("<a href=\"/bajarp\"\"><button>BajarProyector </button></a>");  
    client.println("<br><br>");
    client.println("<hr>");
    client.println("<br><br>");
    client.println("<pre><h3>NodeMCU v2</br> WiFi controlled Car via Web Browser</br> RADIOCOMUNICACIONES</br></br> Rubén Gómez Úbeda</br></h3></pre>");
    client.println("</center>");
    client.println("</body>");
    client.println("</html>");
} 

void subirv(void)   /*Esta son las funciones en este caso la de subir la persiana*/
{
  digitalWrite(pinbajarv, HIGH); /* Antes de hacer nada lo que hacemos es poner en alto el contrario por que el rele se ejecuta en bajo*/
  digitalWrite(pinsubirv, LOW); /*Ponemos el valor del pin en bajo */
  Serial.print("Tah subiendo la persiana"); /* Manda un mensaje en el monitor serie*/
  Serial.println("");
}

void bajarv(void)   
{
   digitalWrite(pinsubirv, HIGH); 
  digitalWrite(pinbajarv, LOW);
  Serial.print("Tah bajando la persiana");
  Serial.println("");
}

void pararv(void)