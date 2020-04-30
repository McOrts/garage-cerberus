# Un barrera LASER, LoRaWan y Docker para vigilar mi garage 

Se me ocurrió que alternativa para proteger el acceso a la plaza de garage comunitaria sin tener que instalar separaciones físicas. Se trata de visibilizar una barrera virtual que detecta y notifica intrusiones.

En esta primera iteración, el dispositivo sólo tendrá un sensor de luz para detectar la interrupción de un haz LASER a fin de avisar del paso de una persona. Más adelante podríamos incluir otros detectores de movimientos como un PIR (Passive infrared sensor). O la conexión son un sistema de grabación de imágen, por ejemplo.

<img src="./images/garage-cerberus_PoC.png" width="300" align="right" />
El montaje se basa en un procesador ESP32 programado en C++ con las librerias de Arduino LMIC desarrolladas por IBM para los chips LoRa SX1272, SX1276 que son compatibles con muchos circuitos. 

El sensor se basa en un LED Laser y una fotoresistencia que se lee por un puerto de conversión Analogico/Digital. El programa ajusta los niveles de luz umbral al arrancar la placa. De manera que si se bloquea la luz se desencadena el proceso de aviso acústivo y envio de mensaje de alerta por LoRa.

La parte de _backoffice_ es soportada por la plataforma [The Thing Networs (aka TTN)](https://www.thethingsnetwork.org/) e [IFTTT](https://ifttt.com/). La información finalmente se procesa en flujos de un programa en Node-red que corre en un contenedor Docker sobre una Raspberry Pi 3B+ con Raspbian. La foto de esta arquitectura sería esta:
<br>
<br>
<img src="./images/garage-cerberus_architecture.png" width="600" align="center" />

## Material utilizado
- He utilizado la *placa* [TTGO LoRa32 V2.0 868 MHz](https://es.aliexpress.com/item/32850086038.html). Basada en el microprocesador ESP32 y en el chip LoRa SX1276, con WiFi y BlueTooth.
- La **fuente Laser** es un pequeño [Diodo de 5mW y longitud de onda roja de 650 nm.](https://es.aliexpress.com/item/32400936169.html)
- El **receptor** es una resistencia sensible a la luz de 12mm [GL12528 LDR](https://es.aliexpress.com/item/32292338404.html)
- Y como **altavoz** un [buzzer pensado para montajes de Arduino](https://es.aliexpress.com/item/32849730395.html)
- Mini [**espejos** acrílicos](https://es.aliexpress.com/item/1861870525.html)

## El montaje
<img src="./images/garage-cerberus_bb.png" width="600" align="center" />
<img src="./images/garage-cerberus_device_inside.jpg" width="300" align="left" />
<img src="./images/garage-cerberus_device_laser.JPG" width="300" align="right" />
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

## Registro del sensor en la nube
Vamos a utilizar los servicios de TTN que enrutarán el mensaje desde el _gateway_ que reciba por radiofrecuencia el paquete de datos hasta el _endpoint_ que consumirá la aqplicación que tome acción con la información contenida en la trama da datos. 

Este dispositivo es del tipo ABP (Activation-by-personalisation) lo que significa que se identificará en la red con un _DevAddr_ y una _Session key_ preconfigurada. Para ello tenemos que completar el registro de una aplicación y un dispositivo. Estos son los pasos a seguir:

### Registro de la aplicación
Los registros se comunican con la aplicación en la que han sido registrados. Para registras un dispositivo, primero hay que añadir una aplicación.

<img src="./images/ttn-add-application.png" width="500" align="right" />

En la consola, selecciona _APPLICATION_ y pulsa _add application_ en la siguiente pantalla. 
- Para el _Application ID_, elige un identificador único, en minúsculas, puedes usar caractares alfanuméricos peor no guiones ´-´ consecutivos.
- Para  _Description_, elige la descripcion que prefieras.
- No hace falta poner nada en _Application EUI_ 
- En _Handler registration_ deja el valor predeterminado: ttn-handler-eu
- Presiona _Add Application_ para finalizar

Ahora seremos redirigidos a la página con la nueva aplicación añadida donde puedes encontrar la _app EUI_ y el _Access Keys_ generados.
<br>
<img src="./images/ttn-application.png" width="600" align="center" />

### Registro del dispositivo

<img src="./images/ttn-add-device.png" width="500" align="right" />

En TTN un dispositivo (devide) representa la configuración de lo que también llama nodo (node) que a fin de cuentas es nuestro circuito. 
Al acceder al formulario de registro, únicamente tenermos que rellenar el _Device ID_ que será el nombre único de este nodo. Es preferible  pulsar el icono marcado en la imágen para que se genere automáticamente el _Device EUI_.

<img src="./images/ttn-add-device_params.png" width="500" align="center" />

Finalmente pulsaremos _Register_ y pulsaremos el icono con el nombre de nuestro nuevo dispositivo para ver sus datos de configuración. Aquí encontraremos los parámetros que necesitamos por ser un dispositivo de tipo ABP. Y que tendremos que pasar al fichero de configuración settings.h que se cargará en el _sketch_ del IDE de Arduino.
Pero el formato para las Keys es diferente. Encontrarás aquí una hoja excel (Encode_EUI.xlsx) que te facilitará esta tarea.

```
// TTN Configuration
// LoRaWAN NwkSKey, network session key provided by TTN Console (https://console.thethingsnetwork.org) in Device settings form:
static const PROGMEM u1_t NWKSKEY[16] = {0x8F,0xDA,......};
// LoRaWAN AppSKey, application session key provided by TTN Console (https://console.thethingsnetwork.org) in Device settings form:
static const u1_t PROGMEM APPSKEY[16] = {0xE5,0x0A,......};
// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x12345678 ; // <-- Change this address for every node!

// Other params
const int update_time_alive = 150000;
const int PhotoCell = 2; 
const int Buzzer = 15;
```
### Formato de la trama

<img src="./images/ttn-add-payload_format.png" width="400" align="left" />

Tendremos que volver a la pantalla de _Application Overbiew_ para hacer una última configuración. Pulsando en la pestaña de _Payload Formats_ accedemos al formulario donde se permite poner un script para decodificar la trama de datos de nuestro mensaje LoRa. En nuestro caso este es el formato.
<hr>

## Configuración servidor local

La arquitectura elegida para el back y front está pensada para tener unos mínimos costes de operación y ser escalable. El uso de contenedores nos permitirá añadir nuevos dispositivos (Nodos TTN) rápidamente.

El servidor utilizado ha sido una Raspberry Pi 3B+. Actualmente no es el modelo más potente pero suficiente para ejecutar varios contenedores. Las tareas inicales de configuración para instalar el sistema operativo Raspbian, Node-red y MySQL pueden ser fácilmente encontradas y son estándar. Las configuraciones propias para este proyecto las describo a continuación.

### Integración Noder-RED con TTN

https://www.thethingsnetwork.org/docs/applications/nodered/

## Demo

[garage cerberus demo](https://img.youtube.com/vi/qcct-dORirM/0.jpg)](https://youtu.be/qcct-dORirM)
