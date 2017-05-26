#include "mbed.h"              
 
//Setup 
Serial pc(USBTX, USBRX); // tx, rx

DigitalIn DVALID(p28);     
DigitalOut DXMIT(p8);        //ChipSelect
DigitalOut Range2(p19);
DigitalOut Range1(p18);
DigitalOut Range0(p17);
DigitalOut Teste(p20);
DigitalOut led_eth1(p30);
DigitalOut led_eth2(p29);


/*  Essa funcao realiza a leitura do Range pelo teclado. O range ser um numero de 1 a 7
    Falta: fazer filtro de ranges, ou seja, não aceitar valores digitados erroneamente
*/
int main()
{
DXMIT.write(1);                     //DXMIT e ativo baixo, portanto deve ser iniciado em nivel alto. Nivel baixo DOUT sera ativado erroneamente
led_eth1.write(1);                  // Acende led1 Ethernet
led_eth2.write(1);                  // Acende led2 Ethernet
DVALID.mode(PullUp);                // DVALID e ativo baixo
SPI spi(p5, p6, p7);                 //MOSI, MISO, SCLK
spi.format(8,0);                     //Configura SPI modo 8 bits, mode 0 (ver referencia)
spi.frequency(8000000);              //Configura SCLK como 8 MHz (96/12 MHz)
unsigned int range = 0;
unsigned int buffer[5];
unsigned int data[2];
unsigned int buffer3_msb = 0;
unsigned int buffer3_lsb = 0;

pc.printf("\n\nE l e c t r o m e t e r - v0.0 - 05/21/2017 - Setup done\r\n\n ");
pc.printf("Digite o Range (1 - 7):\r\n");

scanf("%d", &range);
pc.printf("O Range escolhido foi %d\r\n", range);
if (range == 1)
    {
        Range0 = 1;
        Range1 = 0;
        Range2 = 0;
    }
if (range == 2)
    {
        Range0 = 0;
        Range1 = 1;
        Range2 = 0;
    }
if (range == 3 )
    {
        Range0 = 1;
        Range1 = 1;
        Range2 = 0;
    }
if (range == 4)
    {
        Range0 = 0;
        Range1 = 0;
        Range2 = 1;
    }
if (range == 5)
    {
        Range0 = 1;
        Range1 = 0;
        Range2 = 1;
    }
if (range == 6)
    {
        Range0 = 0;
        Range1 = 1;
        Range2 = 1;
    }
if (range == 7)
    {
        Range0 = 1;
        Range1 = 1;
        Range2 = 1;
    }    
////////////////////////////
/*  Esta funcao realiza a leitura do periodo de integracao e gera sinal CONV pertinente
    Falta: ajustar periodo do PWM para um valor multiplo de 1/10MHz.
    Falta: filtra valores digitados erroneamente
*/
int Tint = 0;
int periodo_pwm = 0;
pc.printf("Digite o Tempo de integracao em us (500 us < T < 1000000 us):\r\n");
scanf("%d", &Tint);
periodo_pwm = 2*Tint;
pc.printf("O tempo de integracao escolhido foi de %d, o periodo do PWM e %d %d\r\n", Tint, periodo_pwm);
PwmOut conv(p26);               //Habilita o PWM no pin26 (Sinal CONV)
conv.period_us(periodo_pwm);    //Seta o periodo do PWM, recebendo como parametro um Int lido pelo usuario
conv.write(0.50f);              //Seta 50% duty cicle

while(1)
    {
        if (!DVALID.read())
        {
        //pc.printf("DVALID state = %d\r\n", DVALID.read());
        DXMIT.write(0);
        buffer[0] = spi.write(0x00);
        buffer[1] = spi.write(0x00);
        buffer[2] = spi.write(0x00);
        buffer[3] = spi.write(0x00);
        buffer[4] = spi.write(0x00);
        DXMIT.write(1);
        buffer3_msb = buffer[2] & 0xF0;
        buffer3_msb = buffer3_msb >> 4;
        buffer3_lsb = buffer[2] & 0x0F;
        //pc.printf("Byte3_msb = %d, Byte3_lsb = %d\r\n", buffer3_msb, buffer3_lsb);
        data[0] = buffer[0] << 12 | buffer[1] << 4 | buffer3_msb;
        data[1] = buffer3_lsb << 16 | buffer[3] << 8 | buffer[4];
        pc.printf("CH1 = %d                 CH2 = %d \r\n", data[0], data[1]);
        //pc.printf("Buffer0 = %d Buffer1 = %d Buffer2 = %d Buffer3 = %d Buffer4 = %d \r\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
        
        }
    }
}

        
