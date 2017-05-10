#include "mbed.h"              
 
Serial pc(USBTX, USBRX); // tx, rx
unsigned int buffer[5];
unsigned int data[2];
unsigned int buffer3_msb = 0;
unsigned int buffer3_lsb = 0;
int main() {
    pc.printf("Digite o primeiro byte.\r\n");
    scanf("%d", &buffer[0]);
    pc.printf("Digite o segundo byte.\r\n");
    scanf("%d", &buffer[1]);
    pc.printf("Digite o terceiro byte.\r\n");
    scanf("%d", &buffer[2]);
    pc.printf("Digite o quarto byte.\r\n");
    scanf("%d", &buffer[3]);
    pc.printf("Digite o quinto byte.\r\n");
    scanf("%d", &buffer[4]);       
    pc.printf("Byte 0 = %d, Byte 1 = %d, Byte 2 = %d, Byte 3 = %d, Byte 5 = %d\r\n", buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
    
    buffer3_msb = buffer[2] & 0xF0;
    buffer3_msb = buffer3_msb >> 4;
    buffer3_lsb = buffer[2] & 0x0F;
    pc.printf("Byte3_msb = %d, Byte3_lsb = %d\r\n", buffer3_msb, buffer3_lsb);
    data[0] = buffer[0] << 12 | buffer[1] << 4 | buffer3_msb;
    data[1] = buffer3_lsb << 16 | buffer[3] << 8 | buffer[4]; 
    pc.printf("CH1 = %d CH2 = %d \r\n", data[0], data[1]);
    
}
