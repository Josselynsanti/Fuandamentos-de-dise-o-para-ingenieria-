
#include <OneWire.h>
#include <DS18B20.h>


#include <DHT.h>
#include <DHT_U.h>

#include <SPI.h> // comunicación del modulo 
#include <RH_NRF24.h>	// incluye la seccion NRF24 de la libreria RadioHead

//DHT22


RH_NRF24 nrf24;		// crea objeto con valores por defecto para bus SPI
			// y pin digital numero 8 para CE

#define SENSOR  4	// establece a SENSOR con numero 4, pin de senal del DHT22
DHT dht(SENSOR, DHT22);	// crea objeto dht

int TEMPERATURA;	// variable para almacenar valor de temperatura
int HUMEDAD;		// variable para almacenar valor de humedad
 
String str_humedad;	// string para almacenar valor de humedad
String str_temperatura;	// string para almacenar valor de temperatura
String str_datos;	// string para almacenar valores separados por coma



// HUMEDAD DEL SUELO V1.2

#define AOUT 0 // Pin analógico "A0" para conectar la salida del sensor de humedad capacitivo

const int Valor_Sensor_Aire = 466;
const int Valor_Sensor_Agua = 188; 

int valor_sensor = 0; 
int porcentaje = 0;
String str_valor;
String str_SUELO;



// SEnsor de temperatura DS18B20*************************

// 1-Wire bus Arduino pin
const byte ONEWIRE_PIN = 2;

byte sensorAddress[8] = {0x28, 0x6F, 0xF7, 0xCE, 0x5, 0x0, 0x0, 0x17};

// 1-Wire object
OneWire onewire(ONEWIRE_PIN);
// DS18B20 sensors object
DS18B20 sensors(&onewire);
int temperatu_Cul = 0; 
String str_temperatu_Cul;	// string para almacenar valor de temperatura


void setup() 
{
  while(!Serial);
  Serial.begin(9600);		// inicializa monitor serie a 9600 bps 
    // DS18B20 sensors setup
  sensors.begin();
  
  if (!nrf24.init())		// si falla inicializacion de modulo muestra texto
    Serial.println("fallo de inicializacion");
  if (!nrf24.setChannel(2))	// si falla establecer canal muestra texto
    Serial.println("fallo en establecer canal");
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)) // si falla opciones 
    Serial.println("fallo en opciones RF");				    // RF muestra texto

    dht.begin(); 	// inicializa sensor 
    delay(2000);	// demora de 2 segundos para estabilizar lectura de sensor
}

void loop()
{
  //SENSOR DE TEMPERATURA HUEMADA DEL SUELO DHT22************
    TEMPERATURA = dht.readTemperature();	// obtiene y almacena temperatura
    HUMEDAD = dht.readHumidity();		// obtiene y almacena humedad
    str_temperatura = String(TEMPERATURA);	// convierte a string valor entero de temperatura
    str_humedad = String(HUMEDAD);		// convierte a string valor entero de humedad

    //  SENSOR DE Humedad del SUELO CAPACITIVO V1.2************

    valor_sensor = analogRead(AOUT);
    porcentaje = map(valor_sensor, Valor_Sensor_Agua, Valor_Sensor_Aire, 100, 0);
    if(porcentaje < 0) porcentaje = 0; // Evita porcentajes negativos en la medida del sensor
    if(porcentaje > 100) porcentaje = 100; // Evita porcentajes negativos en la medida del sensor
  



    // Sensor de temperatura del cultivo DS18B20*****************************************
   
    sensors.request(sensorAddress);
    while(!sensors.available());
    temperatu_Cul = sensors.readTemperature(sensorAddress);
    str_temperatu_Cul= String(temperatu_Cul);
  



    str_datos = ","+ str_temperatura + "," + str_humedad + "," + str_temperatu_Cul + "," + porcentaje ;	// concatena valores separados mediante una coma
    
    
    Serial.println(str_datos); // para obsrvar si se está recoítando info*******
  
    static char *datos = str_datos.c_str();		// convierte a string en formato de lenguaje C
    nrf24.send((uint8_t *)datos, strlen(datos));	// envia datos
    nrf24.waitPacketSent();				// espera hasta realizado el envio
    delay(500);					// demora de 1 segundo entre envios
    
}