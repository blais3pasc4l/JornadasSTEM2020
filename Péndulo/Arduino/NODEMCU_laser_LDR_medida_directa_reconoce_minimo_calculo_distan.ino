/*
 Estudiar primero cómo recibe la señal el LDR; ver su máximo, y la luz a medioambiente. Obtner valores, o la gráfica directamente
 Si obtengo la gráfica, guardr en fichero CSV y abrir con LibreOffice Calc. Represnetar y en losdatos, mostrar categorías.
 Se ven muy bien las oscialciones. Reconocer la forma de la onda, y calcular el perído. 
 Comprobar con la fórmula T = 2*Pi*RAIZ(longitud/g) 
 */
#define pinBoton D5
#define pinLASER D8

long tiempo = 0;
long tiempo1 = 0;
long diferencia = 0;

long tiempocorte1=0;
long tiempocorte2=0;

float voltage = 0;
float voltageAnterior = 0;
float valorCorte = 0;

int contar = 0;

boolean estado = false;

boolean estadoBoton = LOW;

// the setup routine runs once when you press reset:
void setup() {

  pinMode(pinBoton,INPUT);
  pinMode(pinLASER,OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  digitalWrite(pinLASER,LOW); // El laser empieza, lógicamnete, apagado  
}

// the loop routine runs over and over again forever:
void loop() {

  if (valorCorte<=0) {vcorte();} // Si el valor de corte no está establecido, lo establece

  // Encender el laser o no
  int estadoBotonAhora = digitalRead(pinBoton);

  if (estadoBotonAhora==HIGH && estadoBoton==LOW) {
      estado = estado xor 1;
      
      delay(50);
  }

  estadoBoton = estadoBotonAhora;
  digitalWrite(pinLASER,estado);
  // Fin de encender el laser
  
  tiempo = millis();
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue;
  String voltaje = String(voltage);
  voltaje.replace(".",",");

  // Serial.println(String(tiempo)+" ; "+voltaje+" ; corte: "+String(valorCorte));

  // Reconocimiento del paso
  if (estado==HIGH && voltage<valorCorte && voltageAnterior>=valorCorte) { // Si se sucede un paso hacia abajo por el valor de Corte      
     contar = contar + 1;
     if (contar==1) { // Primer paso 
        tiempocorte1 = tiempo;
        Serial.println("=========================================");
        Serial.println ("Valor de corte: "+String(valorCorte));
        Serial.println("Primer paso en el valor de tiempo (ms): "+String(float(tiempocorte1)));
     } else if (contar==3 && tiempocorte1>0) { // Tercer paso
        diferencia = tiempo-tiempocorte1;
        Serial.println("T= "+String( float(diferencia) /1000)+"s; L="+longitud(diferencia)+" cm");
        Serial.println("=========================================");
        // Refresco tiempo
        contar = 0; 
        tiempocorte1=0;
     } // Fin de la condición que el conteo sea par
  } 

  voltageAnterior = voltage;
  // delay(1);  
  // Serial.println(voltage);
}

// Funcion que devuelve la longitud
String longitud(long periodo) {
  // perido viene en milisegundos, convertir a segundos.
  float calculo = 100*9.8*pow(((periodo/1000.0) / (2.0 * 3.141596254)),2.0) ;
  // devuelve la longitud del péndulo en centímetros
  String cadena = String(calculo);
  cadena.replace(".",",");
  return cadena;
}

// Funcion que reconoce el valor de corte
void vcorte() {
    // Al principio establece el valor de corte. Una vez establecido, esta rutina no se vuelve a ejecutar
      digitalWrite(pinLASER,LOW); 
      valorCorte = analogRead(A0);
      delay(10); 
      digitalWrite(pinLASER,HIGH);
      delay(10);
      valorCorte = (valorCorte +analogRead(A0))/2; // La semisuma
      digitalWrite(pinLASER,estado);     
}
