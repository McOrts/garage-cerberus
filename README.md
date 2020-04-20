# Un LASER, LoRaWan y Docker para vigilar mi garage 
Sistema de detección y notificación de incidentes y accesos en garages
En esta primera iteración, el dispositivo sólo tendrá un sensor de luz para detectar la interrupción de un haz LASER a fin de detectar el paso de una persona. Más adelante podríamos incluir otros detectores de movimientos como un PIR (Passive infrared sensor).

## Registro del sensor en la nube
Vamos a utilizar los servicios de [The Thing Networs (aka TTN)](https://www.thethingsnetwork.org/) que enrutarán el mensaje desde el _gateway_ que reciba por radiofrecuencia el paquete de datos hasta el _endpoint_ que consumirá la aqplicación que tome acción con la información contenida en la trama da datos. 

Para ello tenemos que completar el registro de una aplicación y un dispositivo. Estos son los pasos a seguir:
### Registro de la aplicación
Los registros se comunican con la aplicación en la que han sido registrdos. Para registras un dispositivo, primero hay que añadir una aplicación.

<img src="./images/ttn-add-application.png" width="400" align="right" />
En la consola, selecciona _APPLICATION_ y pulsa _add application_ en la siguiente pantalla. 
-Para el _Application ID_, elige un identificador único, en minúsculas, puedes usar caractares alfanuméricos peor no guiones ´-´consecutivos.
-Para  _Description_, elige la descripcion que prefieras.
-No hace falta poner nada en _Application EUI_ 
-En _Handler registration_ deja el valor predeterminado: ttn-handler-eu
-Presiona _Add Application_ para finalizar

Ahora seremos redirigidos a la página con la nueva aplicación añadida donde puedes encontrar la _app EUI_ y el _Access Keys_ generados.
