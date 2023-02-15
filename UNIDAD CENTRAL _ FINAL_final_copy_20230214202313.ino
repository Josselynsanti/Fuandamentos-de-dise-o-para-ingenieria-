
#include <SPI.h>	// incluye libreria SPI para comunicacion con el modulo
#include <RH_NRF24.h>	// incluye la seccion NRF24 de la libreria RadioHead

RH_NRF24 nrf24;		// crea objeto con valores por defecto para bus SPI
			// y pin digital numero 8 para CE

String str_datos;	
String str_temperatura;	
String str_humedad;		
String str_porcentaje;
String str_temperatu_Cul;


// recepción 
int A;
int Ultimo= str_datos. length();

// Estres hidríco 
int CWSI ;
int T_c;
int T_a;
int porcentaje;



// LEDS configuracion de los pines de salida

int led_v = 0; // verde
int led_r = 1; // Rojo

// RELÉ configuracion de los pines de salida

int RELE= 2;


void setup() 
{
  Serial.begin(9600);		// inicializa monitor serie a 9600 bps
  if (!nrf24.init())		// si falla inicializacion de modulo muestra texto
    Serial.println("fallo de inicializacion");
  if (!nrf24.setChannel(2))	// si falla establecer canal muestra texto
    Serial.println("fallo en establecer canal");
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)) // si falla opciones 
    Serial.println("fallo en opciones RF"); 				    // RF muestra texto
     
    Serial.println("Base iniciada");  // texto para no comenzar con ventana vacia
  
  

  pinMode(led_v, OUTPUT); 
  pinMode(led_r, OUTPUT);

  pinMode(RELE, OUTPUT);
}

void loop()
{
  uint8_t buf[12];			// buffer de 11 posiciones
  uint8_t buflen = sizeof(buf);	// obtiene longitud del buffer
    
    if (nrf24.recv(buf, &buflen))	// si hay informacion valida disponible
    { 
      str_datos = String((char*)buf);	// almacena en str_datos datos recibidos
      Serial.println("Se esta reciviendo") ;
      Serial.println(str_datos) ;
      for (int i = 0; i < str_datos.length(); i++) {	// bucle recorre str_datos desde el inicio
        if (str_datos.substring(i,i+1) == ",") {
          A=A+1;
          if (A ==1){
            str_temperatura = str_datos.substring(i+1, i+3);
            Serial.println(str_temperatura);
          }
          if (A ==2){
            str_humedad= str_datos.substring(i+1, i+3);
          }
          if (A ==3){
            str_temperatu_Cul = str_datos.substring(i+1, i+3);
          }
          if (A==4){
            str_porcentaje = str_datos.substring(i+1,Ultimo);
            porcentaje = str_porcentaje.toInt();
            

          }
          
        }
      }
        
    }
  
 

  if(porcentaje <= 33)
  {
    T_c = str_temperatu_Cul.toInt();
    T_a = str_temperatura.toInt();
    Serial.println("Suelo seco del sensor de humedad ");

    CWSI=((T_c-T_a)-(25-15))/((28-27)-(25-15));
    Serial.println(CWSI);
    if  (0< CWSI < 1 ) {
      Serial.println(" suelo optimo - estres hidrico ");
      digitalWrite(led_v, HIGH); 
      digitalWrite(led_r, LOW); 
      delay(1000);

      digitalWrite(RELE, LOW);
      delay(5000); }
    
    if (CWSI<= 0){
      Serial.println(" suelo humedo del estres");
      digitalWrite(led_v, HIGH); 
      digitalWrite(led_r, LOW); 
      delay(1000);

      digitalWrite(RELE, LOW);
      delay(5000); }
    
    if (CWSI>=1){
      Serial.println(" suelo seco de estres ");
      digitalWrite(led_v, LOW);
      digitalWrite(led_r, HIGH); 
      delay(1000);

      digitalWrite(RELE, HIGH);
      delay(5000);}
  }
  if ( 33 <= porcentaje <=66){
    Serial.println(" suelo humedo");
    digitalWrite(led_v, HIGH); 
    digitalWrite(led_r, LOW); 
    delay(1000);

    digitalWrite(RELE, LOW);
    delay(5000); }

  
  
}








