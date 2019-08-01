/*Modos:/
1- Normal
2- Amarelo Piscante
3- Manual 

Estados:
1- 1Vd - 2Vm
2- 1Am - 2Vm
3- 1Vm - 2Vd
4- 1Vm - 2Am

Velocidades:
1-Rápido
2-Normal
3-Lento
*/
// biblioteca EEPROM do arduino
#include <EEPROM.h> 
/* Estes comandos definem para definir as portas do arduino que será usado para cada led
 *  
 */// testando o primeiro programa do farol para pedestre

// defindo as portas de conecção do controlador de dados 'Arduino' 
#define pinSH_CP 9   //Pino Clock
#define pinST_CP 10  //Pino Latch
#define pinDS    11  //Pino Data
#define qtdeCI   5   //Quantidade de CIS controlados

// Definindo a temporização dos Tempos de acessos transformados por Variaves para acesso no processo dos delays do programa
#define Tempo0 1            // equivale a 1'micro segundo
#define Tempo1 50          // equivale a 750' millesimos de segundo
#define Tempo1_5 250
#define Tempo2 2000         // equivale a 2' segundos
#define Tempo2_5 2500       // equivale a 2,5' segundos
#define Tempo3 3000         // equivale a 3' segundos
#define Tempo4 4000         // equivale a 4' segundos

#define Tempo720 7200       // equivalete a 12' horas em segundos para poder utilizar os postes de iluminação

void OndaVerde ();    // reconhecendo a função antes de iniciar o sistema
void Verificacao ();
void Modo_Normal ();
/*
 * Criando variaveis globais para executar e verificar se todas as portas de OUTPUT dos CIS estão funcionado adequadamente
 */


byte led[40] = {0,1,2,      3,4,5,        6,7,8,  // vetores do farol no sentido norte
+               9,10,11,   12, 13,14,   15,16,17, // vetores do farol no sentido sul  
+               18,19,20,   21,22,23,             // vetores do farol na transversal
+               24,25,    26,27,    28,29,    30,31,    32,33     // vetores dos farois de pedestres
+               34,35,36,37,38,39,40};            // sobra de vetores nas saidas de portas digitais 

void ci74HC595Write(byte pino, bool estado);    // reconhecendo a função antes de tudo para o sistema entrar sabendo desta função

// declarando as variaveis e reservando as portas digitais da placa


 

// delay das variaveis
#define delayV1 5;
#define delayV1Am 1;
#define delayV2 20;
#define delayV2Am 6;
#define delayV3 80;
#define delayV3Am 12;


byte modo;
byte estado;
byte velocidade;

byte modoAnt;
byte estadoAnt;

byte startMode;
byte startVelo;
byte pisca;
int delayControl;
int cicloSemaforo;

String comando;
char serialBuff;

// Função de acesso e controle dos CIS 
void ci74HC595Write(byte pino, bool estado) 
{
static byte ciBuffer[qtdeCI];

bitWrite(ciBuffer[pino / 8], pino % 8, estado);

digitalWrite(pinST_CP, LOW); //Inicia a Transmissão

digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
digitalWrite(pinSH_CP, LOW);

for (int nC = qtdeCI-1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {

        digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
        
        digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
        
        digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
        digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
    }  
}

digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão

} // fecha função de acesso e registro dos dados transmitidos pelos CIS

void setup() 
{


  // declarando as portas como saida e entrada
   
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinDS, OUTPUT);
  
  
  //Lê modo de inicio e grava o próximo modo de inicio na EEPROM
  startMode = EEPROM.read(0);
  if (startMode == 0)
  {
     EEPROM.write(0, 1);
  } else { 
     EEPROM.write(0, 0);
  }

  //Lê modo de inicio e grava o próximo modo de inicio na EEPROM
  startVelo = EEPROM.read(1);
  if ((startVelo < 1) || (startVelo >= 4)) 
  { 
     startVelo = 2; //Velocidade Inicial = Normal
     EEPROM.write(1, 2);
  } 

  //Inicia as variaveis
  modo   = startMode + 1;
  estado = 1; 
  velocidade = startVelo; 
  pisca  = 0;
  delayControl = 0;
  cicloSemaforo = 0;

  modoAnt = 99; //Força a primeira execução
  estadoAnt = 99; //Força a primeira execução

  comando = "";
 
  
  Serial.begin(9600);
  
}
void loop() 
{
  ci74HC595Write (led[31], HIGH);  // acende o led dos postes de iluminação 

  if (delayControl < 49) 
  {  //Executa 49 ciclos sem atividade, porém recebendo comandos. As atividades são executadas apenas no ciclo 50.
    
    while (Serial.available() > 0) 
    {
       serialBuff = Serial.read();
       comando = comando + serialBuff;
      
    }
    
  }
  if (modo == 0)
  {
      OndaVerde();
      ci74HC595Write (led[40], HIGH);  // acende o led dos postes de iluminação 
  }
  if (modo == 1)
  {
    OndaVerde();    
    ci74HC595Write (led[40], HIGH);  // acende o led dos postes de iluminação
  }
  if (modo == 2)
  {
    AmareloPiscanteON ();
    AmareloPiscanteOFF ();
    ci74HC595Write (led[40], HIGH);  // acende o led dos postes de iluminação
  }
  
  
} // fecha loop
 // função criada para executar esquema de sinalização com aspecto de transição das luzes no modo onda verde 
void OndaVerde() 
{
  ci74HC595Write (led[40], HIGH);  // acende o led dos postes de iluminação 
  // semaforo Verde ligado lado 1A                                                                  // semaforo Vermelho ligado transversal 1T                              // semaforo amarelo ligado transversal 3T
      
                       
       
       ci74HC595Write (led[0], HIGH);                                                             //  ci74HC595Write (led[9], LOW); /* led verde */                              ci74HC595Write (led[12], LOW);
       ci74HC595Write (led[1], LOW);                                                              //  ci74HC595Write (led[10], LOW); /* led amarelo */                           ci74HC595Write (led[13], HIGH);
       ci74HC595Write (led[2], LOW);                                                              //  ci74HC595Write (led[11], HIGH); /* led vermelho */                         ci74HC595Write (led[14], LOW);
       ci74HC595Write (led[16], LOW);                                                             //  ci74HC595Write (led[18], HIGH);                                            ci74HC595Write (led[24], LOW);
       ci74HC595Write (led[17], HIGH);                                                            //   ci74HC595Write (led[19], LOW);                                             ci74HC595Write (led[25], HIGH);
       ci74HC595Write (led[11], HIGH);
       ci74HC595Write (led[14], HIGH);
       ci74HC595Write (led[18], HIGH);
       delay (Tempo4); 
                 ci74HC595Write (led[20], LOW); 
                 ped3Vermelho ();
                // semaforo Verde ligado lado 1B                                                   // semaforo Vermelho ligado transversal 2T  
                  ci74HC595Write (led[3], HIGH);                                                  /* ci74HC595Write (led[12], LOW); */
                  ci74HC595Write (led[4], LOW);                                                   /* ci74HC595Write (led[13], LOW); */
                  ci74HC595Write (led[5], LOW);                                                   /* ci74HC595Write (led[14], HIGH); */    
                  ci74HC595Write (led[20], LOW);
                  ci74HC595Write (led[21], HIGH); 
                  delay (Tempo2);
                          ci74HC595Write (led[22], LOW);
                          ped4Vermelho ();
                          // semaforo Verde ligado 1C                                             // semaforo vermelho ligado transversal 3T
                          ci74HC595Write (led[6], HIGH);                                         // ci74HC595Write (led[12], LOW);
                          ci74HC595Write (led[7], LOW);                                          // ci74HC595Write (led[13], LOW);
                          ci74HC595Write (led[8], LOW);                                          // ci74HC595Write (led[14], HIGH);
                          ci74HC595Write (led[22], LOW);                                         // ci74HC595Write (led[24], HIGH);
                          ci74HC595Write (led[23], HIGH);                                        // ci74HC595Write (led[25], LOW);
                          delay (Tempo3);

  // semaforo Amarelo ligado lado 1A                                                                // semaforo Vermelho ligado transversal 1T
       ci74HC595Write (led[0], LOW);                                                               // ci74HC595Write (led[9], LOW);
       ci74HC595Write (led[1], HIGH);                                                              // ci74HC595Write (led[10], LOW);
       ci74HC595Write (led[2], LOW);                                                               // ci74HC595Write (led[11], HIGH);
       ci74HC595Write (led[16], LOW);                                                              // ci74HC595Write (led[18], HIGH);
                                                                                                   // ci74HC595Write (led[19], LOW);
       ci74HC595Write (led[17], HIGH);                                                              
       delay (Tempo2_5); 

                 // semaforo Amarelo ligado lado 1B                                                 // semaforo Vermelho ligado transversal 2T       
                    ci74HC595Write (led[3], LOW);                                                  /*ci74HC595Write (led[12], LOW); */
                    ci74HC595Write (led[4], HIGH);                                                 /* ci74HC595Write (led[13], LOW); */
                    ci74HC595Write (led[5], LOW);                                                  /* ci74HC595Write (led[14], HIGH); */
                    ci74HC595Write (led[20], LOW);
                    ci74HC595Write (led[21], HIGH);
                    delay (Tempo2); 


                          // semaforo Amarelo ligado lado 1C                                        // semaforo vermelho ligado transversal 3T
                          ci74HC595Write (led[6], LOW);                                            // ci74HC595Write (led[12], LOW);
                          ci74HC595Write (led[7], HIGH);                                           // ci74HC595Write (led[13], LOW);
                          ci74HC595Write (led[8], LOW);                                            // ci74HC595Write (led[14], HIGH);
                          ci74HC595Write (led[22], LOW);                                           // ci74HC595Write (led[24], HIGH);
                          ci74HC595Write (led[23], HIGH);                                          //  ci74HC595Write (led[25], LOW);
                          delay (Tempo2);
                         // ci74HC595Write (led[18], LOW);
                         // ped2Vermelho ();
  // semaforo Vermelho lado 1A                                                                            // semaforo verde desligado na trasnversal 1T
       ci74HC595Write (led[0], LOW);                                                                    //  ci74HC595Write (led[9], HIGH);
       ci74HC595Write (led[1], LOW);                                                                    //  ci74HC595Write (led[10], LOW);
       ci74HC595Write (led[2], HIGH);                                                                   //  ci74HC595Write (led[11], LOW);                          
       ci74HC595Write (led[16], HIGH);                                                                  //  ci74HC595Write (led[18], LOW);
       ci74HC595Write (led[17], LOW);                                                                    // ci74HC595Write (led[19], HIGH);
       
       delay (Tempo3);

                  // semaforo Vermelho lado 1B                                                             // semaforo verde ligado na transversal 2T                             // semaforo amarelo ligado na transversal 1T
                    ci74HC595Write (led[3], LOW);                                                       //  /* ci74HC595Write (led[12], HIGH);*/                                        ci74HC595Write (led[9], LOW);
                    ci74HC595Write (led[4], LOW);                                                       //  /* ci74HC595Write (led[13], LOW); */                                        ci74HC595Write (led[10], HIGH);
                    ci74HC595Write (led[5], HIGH);                                                      //  /* ci74HC595Write (led[14], LOW); */                                        ci74HC595Write (led[11], LOW);
                                                                                                         //                                              ci74HC595Write (led[18], LOW);
                    ci74HC595Write (led[21], LOW);                                                         //                                                                          ci74HC595Write (led[19], HIGH);
                    ci74HC595Write (led[20], HIGH);
                    delay (Tempo3);                                                                        // ci74HC595Write (led[24], LOW);
                                                                                                          //  ped5Vermelho ();

                          // semaforo Vermelho ligado lado 1C                                             // semaforo vermelho ligado transversal 3T                            // semaforo Amarelo ligado transversal 2T 
                          ci74HC595Write (led[6], LOW);                                                   //   ci74HC595Write (led[12], HIGH);                                       /* ci74HC595Write (led[12], LOW); */
                          ci74HC595Write (led[7], LOW);                                                   //   ci74HC595Write (led[13], LOW);                                        /* ci74HC595Write (led[13], HIGH); */
                          ci74HC595Write (led[8], HIGH);                                                  //  ci74HC595Write (led[14], LOW);                                        /* ci74HC595Write (led[14], LOW); */
                          ci74HC595Write (led[22], HIGH);                                                 //  ci74HC595Write (led[24], LOW);
                          ci74HC595Write (led[23], LOW);                                                  //  ci74HC595Write (led[25], HIGH);
                          ped2Vermelho ();
                          delay (Tempo3);
                          
                          ci74HC595Write (led[16], LOW);
                          ped1Vermelho ();
                          

       
}  // Fecha função OndaVerde
 
// função para modo amarelo piscante
void AmareloPiscanteON ()
{
  ci74HC595Write (led[1], HIGH);
  ci74HC595Write (led[4], HIGH);
  ci74HC595Write (led[7], HIGH);
  ci74HC595Write (led[10], HIGH);
  ci74HC595Write (led[13], HIGH);
  ci74HC595Write (led[17], HIGH);
  ci74HC595Write (led[19], HIGH);
  ci74HC595Write (led[21], HIGH);
  ci74HC595Write (led[23], HIGH);
  ci74HC595Write (led[25], HIGH);
  delay (Tempo1_5);
} // fecha função amarelo Piscante ligado
void AmareloPiscanteOFF ()
{  
  ci74HC595Write (led[1], LOW);
  ci74HC595Write (led[4], LOW);
  ci74HC595Write (led[7], LOW);
  ci74HC595Write (led[10], LOW);
  ci74HC595Write (led[13], LOW);
  ci74HC595Write (led[17], LOW);
  ci74HC595Write (led[19], LOW);
  ci74HC595Write (led[21], LOW);
  ci74HC595Write (led[23], LOW);
  ci74HC595Write (led[25], LOW);
  delay (Tempo1_5);
} // fecha função amarelo piscante desligado

// função de vericação dos farois ou leds se estão todos operantes
void Verificacao()
{
   for (int i=0; i < 32; i++)
   {
    ci74HC595Write (led[i], LOW);
    delay (Tempo1);
    ci74HC595Write (led[i], HIGH);
    delay (Tempo1);
   } // fecha for de vericação
   for (int i=0; i < 8; i++)
   {
    ci74HC595Write (led[i], LOW);
    delay (Tempo1);
    ci74HC595Write (led[i], HIGH);
    delay (Tempo1);
   }
   for (int i = 8; i < 16; i++)
   {
    ci74HC595Write (led[i], LOW);
    delay (Tempo1_5);
    ci74HC595Write (led[i], HIGH);
    delay (Tempo1_5);
   }
   for (int i = 0; i < 32; i++)
   {
    ci74HC595Write (led[i], LOW);    
   }
   for (int i = 16; i < 24; i++)
   {
    ci74HC595Write (led[i], LOW);
    delay (Tempo1);
    ci74HC595Write (led[i], HIGH);
    delay (Tempo1);
   }
   for (int i = 0; i < 32; i++)
   {
    ci74HC595Write (led[i], LOW);
    delay (Tempo1);
    
   }
   
} // fecha void de Verificação

// função para executar o comando de acende e apaga do led vermelho no processo de transição do verde para o vermelho com sincronismo de pisca
void ped1Vermelho ()
{
       for (int i = 0; i < 5; i++)
       {
          ci74HC595Write (led[17], HIGH);
          delay (Tempo1_5);
          ci74HC595Write (led[17], LOW);
          delay (Tempo1_5);
       } // fecha for para piscar led veremelho dos pedestres

} // fechando função de atenção
// função para executar o comando de acende e apaga do led vermelho no processo de transição do verde para o vermelho com sincronismo de pisca
void ped2Vermelho ()
{
       for (int i = 0; i < 5; i++)
       {
          ci74HC595Write (led[19], HIGH);
          delay (Tempo1_5);
          ci74HC595Write (led[19], LOW);
          delay (Tempo1_5);
       } // fecha for para piscar led veremelho dos pedestres

} // fechando função de atenção
// função para executar o comando de acende e apaga do led vermelho no processo de transição do verde para o vermelho com sincronismo de pisca
void ped3Vermelho ()
{
       for (int i = 0; i < 5; i++)
       {
          ci74HC595Write (led[21], HIGH);
          delay (Tempo1_5);
          ci74HC595Write (led[21], LOW);
          delay (Tempo1_5);
       } // fecha for para piscar led veremelho dos pedestres

} // fechando função de atenção
// função para executar o comando de acende e apaga do led vermelho no processo de transição do verde para o vermelho com sincronismo de pisca
void ped4Vermelho ()
{
       for (int i = 0; i < 5; i++)
       {
          ci74HC595Write (led[23], HIGH);
          delay (Tempo1_5);
          ci74HC595Write (led[23], LOW);
          delay (Tempo1_5);
       } // fecha for para piscar led veremelho dos pedestres

} // fechando função de atenção
// função para executar o comando de acende e apaga do led vermelho no processo de transição do verde para o vermelho com sincronismo de pisca
void ped5Vermelho ()
{
       for (int i = 0; i < 5; i++)
       {
          ci74HC595Write (led[25], HIGH);
          delay (Tempo1_5);
          ci74HC595Write (led[25], LOW);
          delay (Tempo1_5);
       } // fecha for para piscar led veremelho dos pedestres

} // fechando função de atenção
///////////////////////////////////////////////////////////////////////////////////////////////
void Modo_Normal ()
{
//  byte led[32] = {0, 1, 2,   3,4,5,    6,7,8,
//          +      9,10,11,   12,13,14,  15,16,17,        
//          +      18,19,20,  21,22,23,   
//          +      24,25,     26,27,      28,29,    30,31,    32,33};
  Conjunto_Farol_1_norte ();
  Conjunto_Farol_2_norte ();
  Conjunto_Farol_3_norte ();
  Conjunto_Farol_1_transversal ();

  Conjunto_Farol_1_sul ();
  Conjunto_Farol_2_sul ();
  Conjunto_Farol_3_sul ();
  Conjunto_Farol_2_transversal ();
  
}
void Conjunto_Farol_1_norte ()   // função respectiva do conjunto do 1° farol no sentindo norte
{
  ci74HC595Write (led[0], HIGH); // led verde
  ci74HC595Write (led[1], LOW); // led amarelo
  ci74HC595Write (led[2], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[0], LOW); // led verde
  ci74HC595Write (led[1], HIGH); // led amarelo
  ci74HC595Write (led[2], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[0], LOW); // led verde
  ci74HC595Write (led[1], LOW); // led amarelo
  ci74HC595Write (led[2], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
void Conjunto_Farol_2_norte ()  // função respectiva do conjunto do 2° farol no sentindo norte
{
  ci74HC595Write (led[3], HIGH); // led verde
  ci74HC595Write (led[4], LOW); // led amarelo
  ci74HC595Write (led[5], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[3], LOW); // led verde
  ci74HC595Write (led[4], HIGH); // led amarelo
  ci74HC595Write (led[5], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[3], LOW); // led verde
  ci74HC595Write (led[4], LOW); // led amarelo
  ci74HC595Write (led[5], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
void Conjunto_Farol_3_norte () // função respectiva do conjunto do 3° farol no sentido norte
{
  ci74HC595Write (led[6], HIGH); // led verde
  ci74HC595Write (led[7], LOW); // led amarelo
  ci74HC595Write (led[8], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[6], LOW); // led verde
  ci74HC595Write (led[7], HIGH); // led amarelo
  ci74HC595Write (led[8], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[6], LOW); // led verde
  ci74HC595Write (led[7], LOW); // led amarelo
  ci74HC595Write (led[8], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Conjunto_Farol_1_sul ()  // função respectiva do conjunto do 1° farol no sentindo sul
{
  ci74HC595Write (led[9], HIGH); // led verde
  ci74HC595Write (led[10], LOW); // led amarelo
  ci74HC595Write (led[11], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[9], LOW); // led verde
  ci74HC595Write (led[10], HIGH); // led amarelo
  ci74HC595Write (led[11], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[9], LOW); // led verde
  ci74HC595Write (led[10], LOW); // led amarelo
  ci74HC595Write (led[11], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
void Conjunto_Farol_2_sul ()  // função respectiva do conjunto do 2° farol no sentindo sul
{
  ci74HC595Write (led[12], HIGH); // led verde
  ci74HC595Write (led[13], LOW); // led amarelo
  ci74HC595Write (led[14], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[12], LOW); // led verde
  ci74HC595Write (led[13], HIGH); // led amarelo
  ci74HC595Write (led[14], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[12], LOW); // led verde
  ci74HC595Write (led[13], LOW); // led amarelo
  ci74HC595Write (led[14], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
void Conjunto_Farol_3_sul () // função respectiva do conjunto do 3° farol no sentido sul
{
  ci74HC595Write (led[15], HIGH); // led verde
  ci74HC595Write (led[16], LOW); // led amarelo
  ci74HC595Write (led[17], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[15], LOW); // led verde
  ci74HC595Write (led[16], HIGH); // led amarelo
  ci74HC595Write (led[17], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[15], LOW); // led verde
  ci74HC595Write (led[16], LOW); // led amarelo
  ci74HC595Write (led[17], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
///////////////////////////////////////////////////////////////////////////////////////////////
void Conjunto_Farol_1_transversal () // função respectiva do conjunto do 1° farol no sentido transversal
{
  ci74HC595Write (led[18], LOW); // led verde
  ci74HC595Write (led[19], LOW); // led amarelo
  ci74HC595Write (led[20], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[18], LOW); // led verde
  ci74HC595Write (led[19], LOW); // led amarelo
  ci74HC595Write (led[20], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[18], HIGH); // led verde
  ci74HC595Write (led[19], LOW); // led amarelo
  ci74HC595Write (led[20], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
  ci74HC595Write (led[18], LOW); // led verde
  ci74HC595Write (led[19], HIGH); // led amarelo
  ci74HC595Write (led[20], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}
void Conjunto_Farol_2_transversal () // função respectiva do conjunto do 2° farol no sentido transversal
{
  ci74HC595Write (led[21], LOW); // led verde
  ci74HC595Write (led[22], LOW); // led amarelo
  ci74HC595Write (led[23], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol
  ci74HC595Write (led[21], LOW); // led verde
  ci74HC595Write (led[22], LOW); // led amarelo
  ci74HC595Write (led[23], HIGH); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol   
  ci74HC595Write (led[21], HIGH); // led verde
  ci74HC595Write (led[22], LOW); // led amarelo
  ci74HC595Write (led[23], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
  ci74HC595Write (led[21], LOW); // led verde
  ci74HC595Write (led[22], HIGH); // led amarelo
  ci74HC595Write (led[23], LOW); // led vermelho
  delay (Tempo1);  // tempo de intervalo do farol 
}


