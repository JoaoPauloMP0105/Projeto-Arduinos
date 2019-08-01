/*
 * Função desenvolvida para efetuar a sincronia dos farois de transito para executar os comandos 
 * de transição das luzes de farois ao longo da sequencia de transição, contribuindo assim para 
 * uma sincronização perfeita possibilitando a segurança necessaria do transito em ação.
 * 
 * 
 * byte led[40] = {0,1,2,      3,4,5,        6,7,8,  // vetores do farol no sentido norte
+               9,10,11,   12, 13,14,   15,16,17, // vetores do farol no sentido sul  
+               18,19,20,   21,22,23,             // vetores do farol na transversal
+               24,25,    26,27,    28,29,    30,31,    32,33     // vetores dos farois de pedestres
+               34,35,36,37,38,39,40};            // sobra de vetores nas saidas de portas digitais 
 * 
 */
void Onda_Verde ()
{       // conjunto dos 3 farois norte  //    conjunto dos 3 farois sul       //  conjunto dos farois transversais
  //0,1,2,      3,4,5,        6,7,8,   //   9,10,11,   12, 13,14,   15,16,17 //     18,19,20,   21,22,23     //
  ci74HC595Write (led[18], LOW);    // 1° farol sentido transversal led verde
  ci74HC595Write (led[19], LOW);    // 1° farol sentido transversal led amarelo
  ci74HC595Write (led[0], HIGH);    // 1° farol sentido norte led verde
  ci74HC595Write (led[9], HIGH);    // 1° farol sentido sul led verde
  ci74HC595Write (led[20], HIGH);   // 1° farol sentido transversal led vermelho
  ci74HC595Write (led[1], LOW);     // 1° farol sentido norte led amarelo
  ci74HC595Write (led[2], LOW);     // 1° farol sentido norte led vermelho
  ci74HC595Write (led[10], LOW);    // 1° farol sentido sul led amarelo
  ci74HC595Write (led[11], LOW);    // 1° farol sentido sul led vermelho
// espaço para inserir os delay

  ci74HC595Write (led[23], LOW);    // 2° farol sentido transversal led vermelho
  ci74HC595Write (led[21], LOW);    // 2° farol sentido transversal led verde
  ci74HC595Write (led[22], HIGH);    // 2° farol sentido transversal led amarelo
  ci74HC595Write (led[3], HIGH);    // 2° farol sentido norte led verde
  ci74HC595Write (led[12], HIGH);   // 2° farol sentido sul led verde
  ci74HC595Write (led[4], LOW);     // 2° farol sentido norte led amarelo
  ci74HC595Write (led[5], LOW);     // 2° farol sentido norte led vermelho
  ci74HC595Write (led[13], LOW);    // 2° farol sentido sul led amarelo
  ci74HC595Write (led[14], LOW);    // 2° farol sentido sul led vermelho
// espaço para inserir os delay


  ci74HC595Write (led[21], LOW);    // 2° farol sentido transversal led verde
  ci74HC595Write (led[22], LOW);    // 2° farol sentido transversal led amarelo
  ci74HC595Write (led[6], HIGH);    // 3° farol sentido norte led verde
  ci74HC595Write (led[15], HIGH);   // 3° farol sentido sul led verde
  ci74HC595Write (led[23], HIGH);   // 2° farol sentido transversal led vermelho
  ci74HC595Write (led[7], LOW);     // 3° farol sentido norte led amarelo
  ci74HC595Write (led[8], LOW);     // 3° farol sentido norte led vermelho
  ci74HC595Write (led[16], LOW);    // 3° farol sentido sul led amarelo
  ci74HC595Write (led[17], LOW);    // 3° farol sentido sul led vermelho
// espaço para inserir os delay

  ci74HC595Write (led[0], LOW);     // 1° farol sentido norte led verde
  ci74HC595Write (led[9], LOW);     // 1° farol sentido sul led verde
  ci74HC595Write (led[1], HIGH);    // 1° farol sentido norte led amarelo
  ci74HC595Write (led[10], HIGH);   // 1° farol sentido sul led amarelo
  ci74HC595Write (led[20], HIGH);   // 1° farol sentido transversal led vermelho
  ci74HC595Write (led[19], LOW);    // 1° farol sentido transversal led amarelo
  ci74HC595Write (led[18], LOW);    // 1° farol sentido transversal led verde
  ci74HC595Write (led[2], LOW);     // 1° farol sentido norte led vermelho
  ci74HC595Write (led[11], LOW);    // 1° farol sentido sul led vermelho  
// espaço para inserir os delay

  ci74HC595Write (led[3], LOW);     // 2° farol sentido norte led verde
  ci74HC595Write (led[12], LOW);    // 2° farol sentido sul led verde
  ci74HC595Write (led[4], HIGH);    // 2° farol sentido norte led amarelo
  ci74HC595Write (led[13], HIGH);   // 2° farol sentido sul led amarelo
  ci74HC595Write (led[5], LOW);     // 2° farol sentido norte led vermelho
  ci74HC595Write (led[14], LOW);    // 2° farol sentido sul led vermelho
// espaço para inserir os delay

  ci74HC595Write (led[21], LOW);    // 2° farol sentido transversal led verde
  ci74HC595Write (led[22], LOW);    // 2° farol sentido transversal led amarelo
  ci74HC595Write (led[6], LOW);     // 3° farol sentido norte led verde
  ci74HC595Write (led[15], LOW);    // 3° farol sentido sul led verde
  ci74HC595Write (led[7], HIGH);    // 3° farol sentido norte led amarelo
  ci74HC595Write (led[16], HIGH);   // 3° farol sentido sul led amarelo
  ci74HC595Write (led[23], HIGH);   // 2° farol sentido transversal led vermelho
  ci74HC595Write (led[8], LOW);     // 3° farol sentido norte led vermelho
  ci74HC595Write (led[17], LOW);    // 3° farol sentido sul led vermelho
// espaço para inserir os delay

  ci74HC595Write (led[0], LOW);     // 1° farol sentido norte led verde
  ci74HC595Write (led[9], LOW);     // 1° farol sentido sul led verde
  ci74HC595Write (led[1], LOW);     // 1° farol sentido norte led amarelo
  ci74HC595Write (led[10], LOW);    // 1° farol sentido sul led verde
  ci74HC595Write (led[2], HIGH);    // 1° farol sentido norte led vermelho
  ci74HC595Write (led[11], HIGH);   // 1° farol sentido sul led verde
  ci74HC595Write (led[20], LOW);    // 1° farol sentido transversal led vermelho
  ci74HC595Write (led[19], LOW);    // 1° farol sentido transversal led amarelo
  ci74HC595Write (led[18], HIGH);   // 1° farol sentido transversal led verde
// espaço para inserir os delay


  ci74HC595Write (led[3], LOW);     // 2° farol sentido norte led verde
  ci74HC595Write (led[4], LOW);     // 2° farol sentido norte led amarelo
  ci74HC595Write (led[12], LOW);    // 2° farol sentido sul led verde
  ci74HC595Write (led[13], LOW);    // 2° farol sentido sul led amarelo
  ci74HC595Write (led[5], HIGH);    // 2° farol sentido norte led vermelho
  ci74HC595Write (led[14], HIGH);   // 2° farol sentido sul led vermelho
// espaço para inserir os delay

  ci74HC595Write (led[23], LOW);    // 2° farol sentido transversal led vermelho
  ci74HC595Write (led[22], LOW);    // 2° farol sentido transversal led amarelo
  ci74HC595Write (led[6], LOW);     // 3° farol sentido norte led verde
  ci74HC595Write (led[7], LOW);     // 3° farol sentido norte led amarelo
  ci74HC595Write (led[15], LOW);    // 3° farol sentido sul led verde
  ci74HC595Write (led[16], LOW);    // 3° farol sentido sul led amarelo
  ci74HC595Write (led[20],LOW);     // 1° farol sentido transversal led vermelho
  ci74HC595Write (led[18], LOW);    // 1° farol sentido transversal led verde
  ci74HC595Write (led[8], HIGH);    // 3° farol sentido norte led vermelho
  ci74HC595Write (led[17], HIGH);   // 3° farol sentido sul led vermelho
  ci74HC595Write (led[19], HIGH);   // 1° farol sentido transversal led amarelo
  ci74HC595Write (led[21], HIGH);    // 2° farol sentido transversal led verde
// espaço para inserir os delay
} // fecha função onda verde do sistema 

