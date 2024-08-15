/*
Instituo Tecnologico de Costa Rica
CE2103 Algoritmos y Estructuras de Datos II
Segundo proyecto ~ Selda
Implementacion controlador
Autor: Fabian Gutierrez Jimenez
Profesor: Isaac Ramirez Herrera
*/

//se incluye la biblioteca keyboard
#include <Keyboard.h> 

//se asignan los pines de los botones del Arduino Pro Micro
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

//Se asigna a cada boton la tecla que se quiere emular con los botones
void loop() {
  
  //Al pin 2 se le asigna la tecla Enter
  if (digitalRead(2) == LOW) {
    Keyboard.press(' ');
  }
  else{
    Keyboard.release(' ');  
  }

  
  //Al pin 3 se le asigna la tecla flecha derecha
  if (digitalRead(3) == LOW) {
    Keyboard.press('d');
  }
  else{
  Keyboard.release('d');
  }  

  
  //Al pin 4 se le asigna la tecla flecha arriba
  if (digitalRead(4) == LOW) {
    Keyboard.press('w');
  }
  else{
  Keyboard.release('w');
  }  

  
  //Al pin 5 se le asigna la tecla flecha izquierda
  if (digitalRead(5) == LOW) {
    Keyboard.press('a');
  }
  else{
  Keyboard.release('a');
  }  

  //Al pin 6 se le asigna la tecla flecha abajo
  if (digitalRead(6) == LOW) {
    Keyboard.press('s');
  } 
  else{
  Keyboard.release('s');
  }  

  
  //Al pin 7 se le asigna la tecla left alt
  if (digitalRead(7) == LOW) {
    Keyboard.press(KEY_LEFT_ALT);
  }
  else{
  Keyboard.release(KEY_LEFT_ALT);
  }  
}
