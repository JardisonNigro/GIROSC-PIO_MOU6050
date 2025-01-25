#include "MPU6050.h"

void setup(){
  
  MPU_begin();
  Serial.println("mantenha o robor parado, estamos calibrando!");
  delay(2000);
  Calibrar_Gyro();
  Serial.print("Robo calibrado, offset atual de ");
  Serial.println(gyroOffsetZ,5);
   delay(5000);
}

void loop(){
    // Exibe os resultados
  CalAng ();
  Serial.print("Velocidade Angular (rad/s): ");
  Serial.println(gyroVelocity, 10);
  
  Serial.print("Ângulo de Deslocamento (rad): ");
  Serial.println(AngTetaZ, 10);
  Serial.println(lastTime, 10);

  delay(5); // Atualização a cada 100 ms
 
}
