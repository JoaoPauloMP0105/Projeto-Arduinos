//Carrega a biblioteca de comunicação serial
#include <SoftwareSerial.h>
//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>
///////////////////////////////////////////////////////////////

// defindo as portas de conecção do controlador de dados 'Arduino'
#define pinSH_CP 10   //Pino Clock
#define pinST_CP 11  //Pino Latch
#define pinDS    12  //Pino Data
#define qtdeCI   2   //Quantidade de CIS controlados

// define variaveis para o sensor de infra-vermelho
int sensor_direita = A1;
int sensor_esquerda = A0;
int valor_sensor_direita = 0;
int valor_sensor_esquerda = 0;
unsigned long media_sensor_direita = 0;
unsigned long media_sensor_esquerda = 0;
int ref_sensor_direita = 0;
int ref_sensor_esquerda = 0;
int cont = 0;
/////////////////////////////////////////////////////////////
byte Front_Ligths = 5; // variavel para ligar os leds
int Back_Ligths = 2; // variavel para ligar os leds
/////////////////////////////////////////////////////////////
//Inicializa o sensor nos pinos definidos acima do sensor ultrasonic

//Define os pinos para o hc04 trigger e echo
#define pino_trigger 8 // fio verde
#define pino_echo 9 // fio azul
Ultrasonic ultrasonic(pino_trigger, pino_echo);
/////////////////////////////////////////////////////////////
#define Tmp5 500
// inicializa o sensor nos pinos definidos ára conexão do modulo HC 06
SoftwareSerial serial1(7, 6); // RX, TX pinos de conexao com o dispositivo bluetooth fio azul 7, fio amarelo 6

//Definicoes pinos Arduino ligados a entrada da Ponte H
//int IN1 = 1;
//int IN2 = 2;
//int IN3 = 3;
//int IN4 = 4;
byte IN [16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
int vlMotorA = 3; // pino para controlar o pwm
int vlMotorB = 5; // pino para controlar o pwm
#define tmp01 1 // variavel de tempo
#define tmp05 250 // variavel de tempo
//////////////////////////////////////////////////////////////////////////////////////////
char Ligar;
char speed1; // variavel para controlar a velocidade dos motores
char Sentido; // variavel para verificar o sentindo dos comandos entre aplicativo mobiel e arduino
String textoRecebido = ""; // string para receber os dados da porta serial via bluetooth
unsigned long delay1 = 0; // variavel para controlar os delay do sistema

float Velocidade = 0; // variavel para armazenar os dados de velocidade
float cmMsec, inMsec; // variaveis para trabalhar com os dados do sensor ultrasonico

// inicializando as funções no começo do codigo
void Up();
void Back();
void Right();
void Left();
void Up_Right();
void Up_Left();
void Back_Left();
void Back_Right();
void Break();
void Speed();
void Sensor();
void Seguidor_linha();
void sensor_Infravermelho();
void sensor_Utrasonic();
void comandos_Serial();
void controle_Sentido();

// Função de acesso e controle dos CIS
void ci74HC595Write(byte pino, bool estado)
{
  static byte ciBuffer[qtdeCI];

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);

  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão

  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);

  for (int nC = qtdeCI - 1; nC >= 0; nC--) 
  {
    for (int nB = 7; nB >= 0; nB--) 
    {

      digitalWrite(pinSH_CP, LOW);  //Baixa o Clock

      digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT

      digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
      digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento
    }
  }

  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão

} // fecha função de acesso e registro dos dados transmitidos pelos CIS
void Speed () // não consegui utilizara essa função pois os motores tem diferente tipos de rotação por serem de baixo custo!!!
{
   /* mapeando os comandos de rotação dos motores
    Para Frente
    motor IN 2 ligado gira a roda para frente
    motor IN 3 ligado gira a roda para frente

    Para Tras
    motor IN 1 ligado gira a roda para tras
    motor IN 4 ligado gira a roda para tras

    Sentido para direita
    motor IN 1 & 3 ligados vira o prototipo para direita 360°
    motor IN 2 & 3 ligados vira o prototipo para direita para frente IN 2= 80 / IN 3= 100 velocidade opcional 90° VALORES DE VELOCIDADE PODEM SER ALTERADOS PARA UMA AÇÃO MAIS SENSIVEL
    motor IN 1 & 4 ligados vira o prototipo para direita para tras IN 1= 80 / IN 4= 100 velocidade opcional 90°

    Sentido para esquerda
    motor IN 2 & 4 ligados vira o prototipo para esquerda 360°
    motor IN 2 & 3 ligados vira o prototipo para esqueda para frente IN 2= 100 / IN 3= 80 velocidade opcional 90°
    motor IN 1 & 4 ligados vira o prototipo para esquerda para tras IN 1= 100 / IN 4= 80 velocidade opcional 90°
    motor
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Linha de comando para aumentar ou diminuir a velocidade dos motores
    if ((Sentido == 'F') || (Sentido == 'B') || (Sentido == 'L') || (Sentido == 'R') || (Sentido == 'G') // condição para acionar a velocidade dos motores fazendo com que controle
    || (Sentido == 'I') || (Sentido == 'H') || (Sentido == 'J') != (Sentido == 'S'))                     // os motores de forma mais livre a velocidade dos motores
    {
    Speed();
    } */
  /* else if (Sentido != Serial.available())
    {
    if (cmMsec >= 20)
    {
        Up();
        delay (tmp05);
    }
    else if (cmMsec < 20)
    {
        Right ();
        delay (tmp05);
        Up ();
        delay (tmp05);
        Left ();
        delay (tmp05);
        Up ();
        delay (tmp05);
        Break ();
        delay (tmp05);
    }
    //Comandos_Automaticos (); // chama a função para o robo se mover automaticamente
    } // fecha else
    /* não consegui utilizar esses comandos por causa da diferença de torque dos motores então a solução
   foi mapear manualmente as velocidades dos motores nas linhas onde o comando aparece da seguinte forma
   analogWrite (vlMotorA, **);
   analogWrite (vlMotorB, **);
  /*
  void Speed () // função para determinar a velocidade dos motores do robo
  {
  /*
    tabela de velocidades dos motores do robo
    0 < 255 rpm
    25,5 = 1
    51 = 2
    76,5 = 3
    102 = 4
    127,5 = 5
    153 = 6
    178,5 = 7
    204 = 8
    229,5 = 9
    255 = q

  if (speed1 == '0')
  {
   Velocidade = 0;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 0");
  // delay (tmp05);
  }
  else if (speed1 == 1)
  {
   Velocidade = 25.5;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 25,5");
  }
  else if (speed1 == '2')
  {
   Velocidade = 51;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 51");
  }
  else if (speed1 == '3')
  {
   Velocidade = 76.5;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 76,5");
  }
  else if (speed1 == '4')
  {
   Velocidade = 102;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 102");
  }
  else if (speed1 == '5')
  {
   Velocidade = 127.5;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 127,5");
  }
  else if (speed1 == '6')
  {
   Velocidade = 153;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 153");
  }
  else if (speed1 == '7')
  {
   Velocidade = 178.5;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 178,5");
  }
  else if (speed1 == '8')
  {
   Velocidade = 204;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 204");
  }
  else if (speed1 == '9')
  {
   Velocidade = 229, 5;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 229,5");
  }
  else if (speed1 == 'q')
  {
   Velocidade = 255;
   analogWrite (vlMotorA && vlMotorB, Velocidade);
   Serial.println ("RPM 255");
  }
  }
*/
}
void Comandos_Automaticos () // função para executar a automação dos comandos
{


}// fecha Comandos_Automaticos
void Up () // função para ir para frente o robo
{
  /*
      Para Frente
      motor IN 2 ligado gira a roda para frente
      motor IN 3 ligado gira a roda para frente
  */

  Serial.println ("Para frente");
  //Speed ();
  analogWrite (vlMotorA, 85);
  analogWrite (vlMotorB, 125);
  ci74HC595Write(IN[1], LOW); // sentido do motor 1 desliga motor
  ci74HC595Write(IN[4], LOW); // sentido do motor 4 desliga motor
  ci74HC595Write(IN[2], HIGH); // sentido do motor 2 para frente
  ci74HC595Write(IN[3], HIGH); // sentido do motor 3 para frente

}
void Back () // função para ir para tras o robo
{
  /*
       Para Tras
      motor IN 1 ligado gira a roda para tras
      motor IN 4 ligado gira a roda para tras
  */
  Serial.println ("Para tras");
  //Speed ();
  analogWrite (vlMotorA, 60); // velocidade media
  analogWrite (vlMotorB, 105); // velocidade media
  ci74HC595Write(IN[2], LOW); // sentido do motor 2 desliga motor
  ci74HC595Write(IN[3], LOW); // sentido do motor 3 desliga motor
  ci74HC595Write(IN[1], HIGH); // sentido do motor 1 para tras
  ci74HC595Write(IN[4], HIGH); // sentido do motor 4 para tras
}
void Right () // função para virar para direita o robo em 360°
{
  /*
       Sentido para direita
      motor IN 1 & 3 ligados vira o prototipo para direita 360°
  */
  Serial.println ("Vira para direita em 360°");
  //Speed ();
  analogWrite (vlMotorA, 100);
  analogWrite (vlMotorB, 105);
  ci74HC595Write(IN[2], LOW); // sentido do motor 2 desliga motor
  ci74HC595Write(IN[4], LOW); // sentido do motor 4 desliga motor
  ci74HC595Write(IN[1], HIGH); // sentido do motor 1 liga para ir para frente
  ci74HC595Write(IN[3], HIGH); // sentido do motor 3 liga para ir para tras

}
void Left () // função para virar para a esquerda o robo em 360°
{
  /*
     Sentido para esquerda
     motor IN 2 & 4 ligados vira o prototipo para esquerda 360°
     motor
  */
  Serial.println ("Vira para esquerda 360°");
  //Speed ();
  analogWrite (vlMotorA, 100);
  analogWrite (vlMotorB, 105);
  ci74HC595Write(IN[1], LOW); // sentido do motor 1 desliga motor
  ci74HC595Write(IN[3], LOW); // sentido do motor 3 desliga motor
  ci74HC595Write(IN[2], HIGH); // sentido do motor 2 liga para ir para tras
  ci74HC595Write(IN[4], HIGH); // sentido do motor 4 liga para ir frente
}
void Up_Right () // função para o robo ir para frente e ir para direita gradadivamente
{
  /*
         Sentido para direita
        motor IN 1 & 3 ligados vira o prototipo para direita 360°
        motor IN 2 & 3 ligados vira o prototipo para direita para frente IN 2= 80 / IN 3= 100 velocidade opcional 90°
        motor IN 1 & 4 ligados vira o prototipo para direita para tras IN 1= 80 / IN 4= 100 velocidade opcional 90°
  */
  Serial.println ("função para o robo ir para frente e ir para direita gradadivamente");
  //Speed ();
  analogWrite (vlMotorA, 75);
  analogWrite (vlMotorB, 190);
  ci74HC595Write(IN[1], LOW); // sentido do motor 2 desliga motor
  ci74HC595Write(IN[4], LOW); // sentido do motor 4 desliga motor
  ci74HC595Write(IN[2], HIGH); // sentido do motor 1 liga para ir para frente
  ci74HC595Write(IN[3], HIGH); // sentido do motor 3 liga para ir para tras
}
void Up_Left () // função para o robo ir para frente e ir para a esquerda gradativamente
{
  /*
      Para Frente
      motor IN 2 ligado gira a roda para frente
      motor IN 3 ligado gira a roda para frente
  */
  Serial.println ("Para frente e para esquerda gradativamente");
  //Speed ();
  analogWrite (vlMotorA, 100);
  analogWrite (vlMotorB, 105);
  ci74HC595Write(IN[1], LOW); // sentido do motor 1 desliga motor
  ci74HC595Write(IN[4], LOW); // sentido do motor 4 desliga motor
  ci74HC595Write(IN[2], HIGH); // sentido do motor 2 para frente
  ci74HC595Write(IN[3], HIGH); // sentido do motor 3 para frente

}
void Back_Left () // função para o robo ir para tras e ir para a esquerda gradativamente
{
  /*
       Para Tras
      motor IN 1 ligado gira a roda para tras
      motor IN 4 ligado gira a roda para tras
  */
  Serial.println ("Para tras e virar para a esquerda");
  //Speed ();
  analogWrite (vlMotorA, 100); // velocidade media
  analogWrite (vlMotorB, 105); // velocidade media
  ci74HC595Write(IN[2], LOW); // sentido do motor 2 desliga motor
  ci74HC595Write(IN[3], LOW); // sentido do motor 3 desliga motor
  ci74HC595Write(IN[1], HIGH); // sentido do motor 1 para tras
  ci74HC595Write(IN[4], HIGH); // sentido do motor 4 para tras

}
void Back_Right () // função para o robo ir para tras e ir para direita gradativamente
{
  /*
       Para Tras
      motor IN 1 ligado gira a roda para tras
      motor IN 4 ligado gira a roda para tras
  */
  Serial.println ("função para o robo ir para tras e ir para direita gradativamente");
  //Speed ();
  analogWrite (vlMotorA, 80); // velocidade media
  analogWrite (vlMotorB, 190); // velocidade media
  ci74HC595Write(IN[2], LOW); // sentido do motor 2 desliga motor
  ci74HC595Write(IN[3], LOW); // sentido do motor 3 desliga motor
  ci74HC595Write(IN[1], HIGH); // sentido do motor 1 para tras
  ci74HC595Write(IN[4], HIGH); // sentido do motor 4 para tras
}
void Break () // função para parar o robo
{
  /*
    Ligando todos os motores ao mesmo tempo o robo fica paralizado
  */
  Serial.println ("Robo parado");
  ci74HC595Write(IN[1], HIGH); // Liga o motor 1
  ci74HC595Write(IN[2], HIGH); // Liga o motor 2
  ci74HC595Write(IN[3], HIGH); // Liga o motor 3
  ci74HC595Write(IN[4], HIGH); // Liga o motor 4
}
void Seguidor_linha () // função para executar controle do prototipo seguidor de linha
{
  // condição para o prototipo se manter em linha reta na linha e evitar obstaculos
  if ((valor_sensor_direita <= 200) && (sensor_esquerda <= 200) && (cmMsec > 20))
  {
    Up(); // prototipo segui para frente
  } // fecha if de condição
  // sensor da direita
  else if (valor_sensor_direita > 200) // condição caso o protipo saia do percuso virando para a esquerda iremos corrigir tal problema
  {
    Break(); // prototipo irá parar
    if (cmMsec <= 20) // verifica se a algum obstaculo a frente
    {
      Back(); // prototipo ira para trás
    }
    Right();// prototipo irá para direita corrigindo assim o trajeto
  } // fecha else if do sensor da direita
  // sensor da esquerda
  else if (valor_sensor_esquerda) // condição caso o protoipo saia do percurso virando para a direita iremos corrigir tal problema
  {
    Break(); // prototipo irá parar
    if (cmMsec <= 20) // verifica se a algum obstaculo a frente
    {
      Back(); // prototipo ira para trás
    }
    Left();// prototipo irá para esquerda corrigindo assim o trajeto
  }// fecha else if do sensor da esquerda
} // fecha função seguidor de linha
void sensor_Utrasonic(Ultrasonic) // função para executar o sensor ultrasonic
{
  //Le as informacoes do sensor, em cm e polegadas
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  //Convertendo a distância em CM e lendo o sensor
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);

  Serial.println("Lendo Distancia em CM & Dados do Sensor de Refletancia");
  Serial.print (cmMsec);
  Serial.print ("\t");
  // delay (tmp05);
}// fecha função para executar os comandos do sensor ultrasonic
void comandos_Serial() // função para executar os comandos serial
{
  // put your main code here, to run repeatedly:
  Sentido = Serial.read();
  Sentido = serial1.read();
  speed1 = Serial.read();
  speed1 = serial1.read();
  Ligar = Serial.read();
  Ligar = serial1.read();
  char caracter;
  if (Serial.available()) // verifica se a comunicação com o dispositivo
  {
    caracter = Serial.read();
    textoRecebido += caracter;
    //delay1 = tmp01;
  }

  //if (((tmp01 - delay1) > 10) && (textoRecebido != ""))
  if (textoRecebido != "")
  {
    serial1.println(textoRecebido);
    textoRecebido = "";
  }
  if (serial1.available())
  {
    caracter = serial1.read();
    Serial.println(caracter);
  }
} // fecha a função de controle de serial do sistema
/*void controle_Sentido() // função para controlar os sentidos do prototipo
{
  //if ((Sentido == 'F') || (Sentido == 'B') || (Sentido == 'L') || (Sentido =='R') || (Sentido =='G') || (Sentido =='I') || (Sentido =='H') || (Sentido =='J') || (Sentido =='S'))
  if (Sentido != 'S') // verificar se o comando executa todos os sentidos
  {

    //  Comandos_Manuais(); // função para executar os comandos manuais através do controle via android
    if ((Sentido == 'F') && (cmMsec >= 20)) // condição para o motor ir para frente
    {
      ci74HC595Write (Front_Ligths, HIGH);
      Up();
    }
    if ((Sentido == 'B') || (cmMsec < 20)) // condição para para o motor ir para tras
    {
      ci74HC595Write (Front_Ligths, LOW);
      Back ();
    }
    if (Sentido == 'L') // condição para o motor ir esquerda
    {
      Left ();
    }
    if (Sentido == 'R') // condição para o motor ir para a direita
    {
      Right ();
    }
    if (Sentido == 'G') // condição para o motor ir para a frente e esquerda
    {
      Up_Left ();
    }
    if (Sentido == 'I') // condição para o motor ir para frente e direita
    {
      Up_Right ();
    }
    if (Sentido == 'H') // condição para o motor ir para tras e esquerda
    {
      Back_Left ();
    }
    if (Sentido == 'J') // condição para o motor ir para tras e direita
    {
      Back_Right ();
    }

  }// fecha if
  else
  {
    Break();
  } // fecha else
} // fecha função controle de sentido
*/
void sensor_Infravermelho() // função para executar o sensor infravermelho de refletancia
{
  // condiçoes do sensor infra-vermelho

  valor_sensor_direita  = analogRead(sensor_direita);
  valor_sensor_esquerda = analogRead(sensor_esquerda);
 /* if (cont < 1000)
  {
    media_sensor_direita = media_sensor_direita + valor_sensor_direita;
    media_sensor_esquerda = media_sensor_esquerda + valor_sensor_esquerda;
    cont = cont + 1;
  }
  else
  {
    cont = 0;
    ref_sensor_direita = media_sensor_direita / 1000;
    ref_sensor_esquerda = media_sensor_esquerda / 1000;

    media_sensor_direita = 0;
    media_sensor_esquerda = 0;
  } */
  Serial.print ("\t SD");
  Serial.print (valor_sensor_direita, DEC);
  Serial.print ("\t SE");
  Serial.print (valor_sensor_esquerda, DEC);
  Serial.print ("\t Cont");
 /* Serial.print (cont, DEC);
  Serial.print ("\t MSD");
  Serial.print (media_sensor_direita, DEC);
  Serial.print ("\t MSE");
  Serial.print (media_sensor_esquerda, DEC);
  Serial.print ("\t RSD");
  Serial.print (ref_sensor_direita, DEC);
  Serial.print ("\t RSE");
  Serial.print (ref_sensor_esquerda, DEC);
  Serial.print ("\n");
*/
} // fecha função infravermelho
void sensor_Utrasonic()
{
  //Le as informacoes do sensor, em cm e polegadas
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  //Convertendo a distância em CM e lendo o sensor
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);

  Serial.println("Lendo Distancia em CM & Dados do Sensor de Refletancia");
  Serial.print (cmMsec);
  Serial.print ("\n");
  //delay (tmp05);
}
void setup() // Setando as informações principais do sistema
{

  /*Define os pinos como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  */
  // declarando as portas como saida e entrada
   
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinDS, OUTPUT);
  
  pinMode(vlMotorA, OUTPUT);
  pinMode(vlMotorB, OUTPUT);
  pinMode(Front_Ligths, OUTPUT);
  pinMode(Back_Ligths, OUTPUT);

  serial1.begin(9600);
  Serial.begin(9600);
}
void loop() // função de loop do sistema executará infinitamente
{
  // put your main code here, to run repeatedly:
  Sentido = Serial.read();
  Sentido = serial1.read();
  speed1 = Serial.read();
  speed1 = serial1.read();
  Ligar = Serial.read();
  Ligar = serial1.read();
  char caracter;
  if (Serial.available()) // verifica se a comunicação com o dispositivo
  {
    caracter = Serial.read();
    textoRecebido += caracter;
    //delay1 = tmp01;
  }

  //if (((tmp01 - delay1) > 10) && (textoRecebido != ""))
  if (textoRecebido != "")
  {
    serial1.println(textoRecebido);
    textoRecebido = "";
  }
  if (serial1.available())
  {
    caracter = serial1.read();
    Serial.println(caracter);
  }
  // chamando algumas funções do sistema para serem executadas initerrupitamente em sistema de loop
 // comandos_Serial(); // controla toda a comunicação do prototipo
  sensor_Utrasonic(); // coleta dados de distancia dos objetos
  sensor_Infravermelho(); // coleta informações dos sensores de refletancia
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  controle_Sentido(); // controla de forma manual através do aplicativo
  Seguidor_linha (); // prototipo executa tarefas de forma autonoma para seguir linhas
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //if ((Sentido == 'F') || (Sentido == 'B') || (Sentido == 'L') || (Sentido =='R') || (Sentido =='G') || (Sentido =='I') || (Sentido =='H') || (Sentido =='J') || (Sentido =='S'))
  if (Sentido != 'S') // verificar se o comando executa todos os sentidos
  {

    //  Comandos_Manuais(); // função para executar os comandos manuais através do controle via android
    if ((Sentido == 'F') && (cmMsec >= 20)) // condição para o motor ir para frente
    {
      digitalWrite (IN[5], HIGH);
      Up();
    }
    if ((Sentido == 'B') || (cmMsec < 20)) // condição para para o motor ir para tras
    {
      digitalWrite (IN[5], LOW);
      Back ();
    }
    if (Sentido == 'L') // condição para o motor ir esquerda
    {
      Left ();
    }
    if (Sentido == 'R') // condição para o motor ir para a direita
    {
      Right ();
    }
    if (Sentido == 'G') // condição para o motor ir para a frente e esquerda
    {
      Up_Left ();
    }
    if (Sentido == 'I') // condição para o motor ir para frente e direita
    {
      Up_Right ();
    }
    if (Sentido == 'H') // condição para o motor ir para tras e esquerda
    {
      Back_Left ();
    }
    if (Sentido == 'J') // condição para o motor ir para tras e direita
    {
      Back_Right ();
    }

  }// fecha if
  else
  {
    Break();
  } // fecha else
} // fecha o loop
