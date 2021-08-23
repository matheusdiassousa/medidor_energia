#include <user_interface.h>;//Biblioteca necessaria para acessar os Timer`s.

os_timer_t tmr0;//Cria o Timer. Maximo de 7 Timer's.

volatile byte status;//Variavel armazenada na RAM para Status do LED.
long x;//Variavel para mostrar o funcionamento do Codigo.

void setup()
{
   os_timer_setfn(&tmr0, led, NULL); //Indica ao Timer qual sera sua Sub rotina.
   os_timer_arm(&tmr0, 1000, true);  //Inidica ao Timer seu Tempo em mS e se sera repetido ou apenas uma vez (loop = true)
                                     //Neste caso, queremos que o processo seja repetido, entao usaremos TRUE.

   pinMode(D1, OUTPUT);//Define D1 como Saida.
   pinMode(D2, OUTPUT);//Define D2 como Saida.
   pinMode(D3, OUTPUT);//Define D3 como Saida.

   Serial.begin(9600);//Inicia a comunicaçao Serial.
}

void loop()
{
   leitura();//Sub rotina para processar os LED`s.

   //O codigo irá funcionar normalmente sem que haja delays e acendendo os LED`s no intervalo definido.
   //Para demonstraçao, abra o Serial monitor e voce vera que o codigo continua normalmente.

   Serial.println(x++);//Print para provar que o codigo nao trava. Abra o Serial monitor e veja
                       //que o codigo continua rodando enquanto os LED`s piscam a cada 1 Segundo.
}

void leitura()//Sub rotina para processar os LED`s.
{
   switch (status)
   {
   case(1)://Caso status seja 1, acenda o led 1.
      digitalWrite(D1, 1);
   break;

   case(2)://Caso status seja 2, acenda o led 2.
      digitalWrite(D2, 1);
   break;

   case(3)://Caso status seja 3, acenda o led 3.
      digitalWrite(D3, 1);
   break;

   case(4)://Caso status seja 4, Apague os 3 LED`s.
      digitalWrite(D1, 0);
      digitalWrite(D2, 0);
      digitalWrite(D3, 0);
   break;
   }
}

void led(void*z)//Sub rotina ISR do Timer sera acessada a cada 1 Segundo e mudara o status do LED.
{
   if (status == 4)//Verifica se o maximo foi alcançado para resetar a contagem.
   {
      status = 0;
   }

   status++;
}
