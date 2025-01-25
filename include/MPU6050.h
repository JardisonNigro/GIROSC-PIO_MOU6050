#include <Wire.h>
#include <Arduino.h>

#define MPU6050_ADDR 0x68  // Endereço padrão do MPU6050
//#define ACCEL_CONFIG 0x1C  // Registro de configuração do acelerômetro
#define GYRO_CONFIG 0x1B  // Registro de configuração do gisroscópio
#define ACCEL_XOUT 0x3B    // Registro inicial dos dados do acelerômetro
#define PWR_MGMT_1 0x6B    // Registro de gerenciamento de energia
#define GYRO_SENS 131.0    // Sensibilidade do giroscópio para ±250°/s
//#define ACCEL_SENS 16384   // Sensibilidade do acelerômetro para 2g
#define M_PI 3.14159265358979323846  // Valor de pi

float gyroOffsetZ = 0; // Offset inicial do giroscópio
float gyroAngle = 0;             // Ângulo acumulado em radianos
unsigned long lastTime = 0;      // Armazena o tempo da última atualização _ giroscópio

float GyroX, GyroY, GyroZ;
int QLeituras = 3000;
float gyroVelocity, AngTetaZ;
// INICIA O MPU E REALIZA A CONFIGURAÇÃO DE ENERGIA, SENSIBILIDADE DO GIROSCÓPIO E DO ACELERÔMETRO

void MPU_begin(){
  Serial.begin(115200);
  Wire.begin();

//** configuração de energia*//
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1); // Registro de configuração de energia
  Wire.write(0x00);       // Acorda o MPU6050
  Wire.endTransmission(true);

//** configuração de sensibilidade do giroscópio*//

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(GYRO_CONFIG); // 0x1b 
  Wire.write(0);       // configura o gyro para 250 graus
  Wire.endTransmission(true);

  /* configuração de sensibilidade do acelerômetro

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_CONFIG); // 0x1c 
  Wire.write(0);       // configura o gyro para 2g
  Wire.endTransmission(true);
*/

}


/* Realiza a leitura do giroscópio e salva os valores recebidos */
void readGiro() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 2, true);


  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0f;

}

void Calibrar_Gyro(){
  for(int i = 0; i<QLeituras;i++){
    readGiro();
    gyroOffsetZ += GyroZ;
    
  }
  gyroOffsetZ /= QLeituras;

}

void CalAng (){
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // Tempo em segundos

  readGiro();
  gyroVelocity = ((GyroZ - gyroOffsetZ) ) * (M_PI / 180.0);
  AngTetaZ += gyroVelocity*deltaTime;
  // Normaliza ang
  if (AngTetaZ > M_PI) {
    AngTetaZ -= 2 * M_PI;
  } else if (AngTetaZ < -M_PI) {
    AngTetaZ += 2 * M_PI;
  }
  lastTime = currentTime;
} 