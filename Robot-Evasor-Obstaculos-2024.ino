/*
  Derechos de autor © UNIT Electronics
  Desarrollado por el Ing. Fabian Ayala
  Este programa se proporciona tal cual, sin garantías de ningún tipo.
  */

// Aquí definimos una caja de herramientas especial para poder controlar el brazo del robot.
#include <Servo.h> //Libreria para controlar el servo motor

// Este es el brazo del robot. Le damos un nombre para poder decirle qué hacer.
Servo miservo; // Le damos el nombre 'miservo' a nuestro brazo.

// Aquí controlamos la posición del brazo del robot y hacemos algunas observaciones.
int PinServo = 11; //Se define el pin 11 de la placa UNO R3 para controlar el brazo del robot.
int ServoPosicion = 0; // Aquí recordamos la posición actual del brazo.
int ServoLecturaIzquierda = 0; // Aquí vemos qué tan lejos mira el brazo a la izquierda. Comenzamos con 0 para después guardar la lectura.
int ServoLecturaDerecha = 0; // Aquí vemos qué tan lejos mira el brazo a la derecha. Comenzamos con 0 para después guardar la lectura.

//----------Aquí decimos qué tan rápido o despacio va ir el robot.----------
/* El pin 6 de la placa UNO R3 se conectara el pin EnA del Puente H para tener 
 * control de velocidad y arranque del motor Derecho */
int PinVeloMotorDerecha = 6;
/* El pin 5 de la placa UNO R3 se conectara el pin Enb del Puente H para tener 
 * control de velocidad y arranque del motor Izquierdo */  
int PinVeloMotorIzquierda = 5;

//----------Aquí decimos qué pines estarán encargados de mover el robot hacia adelante, hacia atrás, izquierda y derecha.----------
/* Los pines 8 y 7 de la placa UNO R3 se conectara a los pines In1 y In2 del Puente H para tener 
 * control del motor Derecho*/ 
int MotorDerechaLn1 = 8; 
int MotorDerechaLn2 = 7;
/* Los pines 4 y 3 de la placa UNO R3 se conectara a los pines In3 y In4 del Puente H para tener 
 * control del motor Izquierdo*/
int MotorIzquierdaLn3 = 4; 
int MotorIzquierdaLn4 = 3;

// Aquí decimos dónde están los ojos del robot para ver y medir qué tan lejos están las cosas.
int echoPin = 12; // Ojo derecho del robot.
int trigPin = 10; // Ojo izquierdo del robot.
// Para que los ojos del robot funcionen correctamente, definimos los siguientes parámetros:  
long Duracion;  // Aquí guardamos cuánto tiempo tarda el eco emitido de un objeto en llegar.
int Distancia;  // Aquí guardamos cuánto está lejos el objeto que detectamos.

//----------Configura la velocidad del robot.----------
// Ajusta este valor para controlar qué tan rápido se mueve el robot.
int Velocidad = 200; // Valores más altos hacen que el robot vaya más rápido, mientras que valores más bajos lo hacen ir más lento.

//-----------Define la distancia de detección para que el robot evite obstáculos.----------
// Ajusta este valor según lo que quieras que el robot pueda detectar.
int DistanciaDeteccion = 25; // Se establece en 20 centímetros, pero puedes cambiarlo según necesites.

//----------Establece el tiempo de giro hacia la derecha o izquierda.----------
// Ajusta estos valores para controlar cuánto tiempo gira el robot en diferentes direcciones.
int TiempoGiroDerechaIzquierda = 100; // Tiempo de giro hacia la derecha y izquierda.
int TiempoGiroAtras = 200;            // Tiempo de giro hacia atrás.

// Configuración inicial del robot.
void setup() {
 
  delay(2000); // Espera un segundo antes de comenzar.

  // Establecemos el modo de los ojos del robot.
  pinMode(trigPin, OUTPUT); // El ojo para enviar señales.
  pinMode(echoPin, INPUT);  // El ojo para recibir señales.

  // Establecemos los pines para el control de los motores del robot como entradas y salidas.
  pinMode(MotorDerechaLn1, OUTPUT);
  pinMode(MotorDerechaLn2, OUTPUT);
  pinMode(MotorIzquierdaLn3, OUTPUT);   
  pinMode(MotorIzquierdaLn4, OUTPUT);
  pinMode(PinVeloMotorDerecha, OUTPUT);   
  pinMode(PinVeloMotorIzquierda, OUTPUT);

  // Configuramos la velocidad de las ruedas del robot.
  analogWrite(PinVeloMotorDerecha, Velocidad);   // Configuramos la velocidad de la rueda derecha.
  analogWrite(PinVeloMotorIzquierda, Velocidad); // Configuramos la velocidad de la rueda izquierda.

  // Conectamos el brazo del robot al pin digital 9 de la placa UNO R3
  miservo.attach(PinServo);
  
  //Posición inicial del brazo del robot a 90°
  miservo.write(90);    // Posición inicial del brazo del robot.
}

// Ejecución contínua del Robot
void loop() {
  // Medimos la distancia con los ojos del robot.
  Distancia = MedirDistancia();
  // Si vemos un obstáculo cercano...
  if(Distancia < DistanciaDeteccion){
    DetenerRobot(); // Detenemos el robot.     
    // El robot mira a la derecha.    
    miservo.write(0); //El brazo gira de 90° a 0°.
    delay(600); //Se detiene 600 Milisegundos
    ServoLecturaDerecha = MedirDistancia(); // El robot obtiene la distancia de la derecha y la guarda.
    // Despues el robot mira a la izquierda.                            
    miservo.write(180); //El brazo gira de 0° a 180°. 
    delay(600); //Se detiene 600 Milisegundos
    ServoLecturaIzquierda = MedirDistancia();// El robot obtiene la distancia de la izquierda y la guarda.
    // El brazo del robot regresa a su posición inicial
    miservo.write(90); //El brazo gira de 180° a 90°. 
    delay(600); //Se detiene 600 Milisegundos

    // Si hay más espacio a la izquierda, giramos hacia ese lado.
    if(ServoLecturaIzquierda > ServoLecturaDerecha){
      IrAtrasRobot(); // El robot retrocede.
      GirarIzquierdaRobot(); // Gira a la izquierda el robot.
    }
    // Si hay más espacio a la derecha, giramos hacia ese lado.
    if(ServoLecturaDerecha > ServoLecturaIzquierda){
      IrAtrasRobot(); // El robot retrocede.
      GirarDerechaRobot(); // Gira a la derecha el robot.
    }
  }
  // Si no hay obstáculos cercanos, el robot avanza para adelante
  if(Distancia > DistanciaDeteccion){
    AvanzaRobotAdelante();
  }  
}

// Función para detener el robot.
void DetenerRobot(){
  // Paramos el robot
  digitalWrite(MotorDerechaLn1,LOW); // Se detiene la rueda izquierda.
  digitalWrite(MotorDerechaLn2,LOW); // Se detiene la rueda izquierda.
  digitalWrite(MotorIzquierdaLn3,LOW); // Se detiene la rueda derecha.
  digitalWrite(MotorIzquierdaLn4,LOW); // Se detiene la rueda derecha.                      
}

// Función para que el robot gire a la derecha.
void GirarDerechaRobot(){
   // Configuramos el sentido de giro para girar a la derecha
  // Hacemos girar la rueda derecha hacia adelante.
  digitalWrite(MotorDerechaLn1, HIGH);
  digitalWrite(MotorDerechaLn2,LOW);
   // Hacemos girar la rueda izquierda hacia atrás.                    
  digitalWrite(MotorIzquierdaLn3, LOW);
  digitalWrite(MotorIzquierdaLn4, HIGH);
  // Tiempo de giro
  delay(TiempoGiroDerechaIzquierda);  
}

// Función para que el robot gire a la izquierda.
void GirarIzquierdaRobot(){
  // Configuramos el sentido de giro para girar a la izquierda
  // Hacemos girar la rueda izquierda hacia adelante.
  digitalWrite(MotorIzquierdaLn3, HIGH);    
  digitalWrite(MotorIzquierdaLn4, LOW);
  // Hacemos girar la rueda derecha hacia atrás.
  digitalWrite(MotorDerechaLn1, LOW);   
  digitalWrite(MotorDerechaLn2, HIGH);
  // Tiempo de giro
  delay(TiempoGiroDerechaIzquierda); 
}

// Función para hacer avanzar el robot hacia adelante
void AvanzaRobotAdelante(){
  // Configuramos el sentido de giro para que el robot avance hacia adelante
  // Hacemos girar la rueda izquierda hacia adelante.                    
  digitalWrite(MotorDerechaLn1, LOW);
  digitalWrite(MotorDerechaLn2,HIGH);
   // Hacemos girar la rueda derecha hacia adelante.
  digitalWrite(MotorIzquierdaLn3, LOW);
  digitalWrite(MotorIzquierdaLn4, HIGH);
}

// Función para hacer retroceder el robot
void IrAtrasRobot(){
  // Configuramos el sentido de giro para que el robot vaya hacia atrás
  // Hacemos girar la rueda izquierda hacia atrás.                    
  digitalWrite(MotorDerechaLn1, HIGH);
  digitalWrite(MotorDerechaLn2,LOW);
  // Hacemos girar la rueda derecha hacia atrás.
  digitalWrite(MotorIzquierdaLn3, HIGH);
  digitalWrite(MotorIzquierdaLn4, LOW);
  // Tiempo de que retrocede el robot 
  delay(TiempoGiroAtras);
}

// Función para saber qué tan lejos está un objeto.
int MedirDistancia(){
  // Enviamos un sonido y esperamos su eco.
  digitalWrite(trigPin, LOW); // Empezamos a enviar sonido.
  delayMicroseconds(2); // Esperamos un poquito.
  digitalWrite(trigPin, HIGH); // Enviamos un sonido.
  delayMicroseconds(10); // Esperamos un poquito más.
  digitalWrite(trigPin, LOW); // Terminamos de enviar sonido.
  
  // Medimos cuánto tarda el eco en llegar y calculamos la distancia.
  Duracion = pulseIn(echoPin, HIGH); // Medimos el tiempo que tarda en volver el eco.
  Distancia = Duracion * 0.034 / 2; // Calculamos la distancia.

   // Devolvemos la distancia que calculamos.
  return Distancia;    
}
