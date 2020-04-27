# Un LASER con LoRaWan y en Docker para vigilar mi garage 
Sistema de detección y notificación de incidentes y accesos en garages
En esta primera iteración, el dispositivo sólo tendrá un sensor de luz para detectar la interrupción de un haz LASER a fin de detectar el paso de una persona. Más adelante podríamos incluir otros detectores de movimientos como un PIR (Passive infrared sensor).

El montaje se basa en un procesador ESP32 programado en C++ con las librerias de [Arduino LMIC](https://github.com/matthijskooijman/arduino-lmic) desarrolladas por IBM para los chips LoRa SX1272, SX1276 que son compatibles con muchos circuitos. 

<img src="./images/garage-cerberus_PoC.png" width="300" align="right" />
El sensor se basa en un LED Laser y una fotoresistencia que se lee por un puerto de conversión Analogico/Digital. El programa ajusta los niveles de luz umbral al arrancar la placa. De manera que si se bloquea la luz se desencadena el proceso de aviso acústivo y envio de mensaje de alerta por LoRa:

<br>
La parte de _backoffice_ es soportada por la plataforma [The Thing Networs (aka TTN)](https://www.thethingsnetwork.org/) e [IFTTT](https://ifttt.com/). La información finalmente se procesa en flujos de un programa en Node-red que corre en un contenedor Docker sobre una Raspberry Pi 3B+ con Raspbian. La foto de esta arquitectura sería esta:
<br>
<img src="./images/garage-cerberus_architecture.png" width="600" align="center" />

## Material utilizado
- He utilizado la *placa* [TTGO LoRa32 V2.0 868 MHz](https://es.aliexpress.com/item/32850086038.html). Basada en el microprocesador ESP32 y en el chip LoRa SX1276, con WiFi y BlueTooth.
- La **fuente Laser** es un pequeño [Diodo de 5mW y longitud de onda roja de 650 nm.](https://es.aliexpress.com/item/32400936169.html)
- El **receptor** es una resistencia sensible a la luz de 12mm [GL12528 LDR](https://es.aliexpress.com/item/32292338404.html)
- Y como **altavoz** un [buzzer pensado para montajes de Arduino](https://es.aliexpress.com/item/32849730395.html)

## El montaje
<img src="./images/garage-cerberus_bb.png" width="500" align="right" />
<img src="./images/garage-cerberus_device_inside.jpg" width="400" align="left" />
<img src="./images/garage-cerberus_device_laser.JPG" width="500" align="center" />

## Registro del sensor en la nube
Vamos a utilizar los servicios de TTN que enrutarán el mensaje desde el _gateway_ que reciba por radiofrecuencia el paquete de datos hasta el _endpoint_ que consumirá la aqplicación que tome acción con la información contenida en la trama da datos. 

Este dispositivo es del tipo ABP (Activation-by-personalisation) lo que significa que se identificará en la red con un _DevAddr_ y una _Session key_ preconfigurada.Para ello tenemos que completar el registro de una aplicación y un dispositivo. Estos son los pasos a seguir:

### Registro de la aplicación
Los registros se comunican con la aplicación en la que han sido registrdos. Para registras un dispositivo, primero hay que añadir una aplicación.

<img src="./images/ttn-add-application.png" width="400" align="right" />

En la consola, selecciona _APPLICATION_ y pulsa _add application_ en la siguiente pantalla. 
- Para el _Application ID_, elige un identificador único, en minúsculas, puedes usar caractares alfanuméricos peor no guiones ´-´ consecutivos.
- Para  _Description_, elige la descripcion que prefieras.
- No hace falta poner nada en _Application EUI_ 
- En _Handler registration_ deja el valor predeterminado: ttn-handler-eu
- Presiona _Add Application_ para finalizar

Ahora seremos redirigidos a la página con la nueva aplicación añadida donde puedes encontrar la _app EUI_ y el _Access Keys_ generados.
<br>
<img src="./images/ttn-application.png" width="600" align="center" />
