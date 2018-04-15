//SIPODEV SP1064 touchpad 

//compile with $ gcc -o i2ctest i2ctest.c -li2c (needs package i2c-dev or similar)
//use with # watch -n 0.5 ./i2ctest both (needs i2c_dev module loaded)

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

void decode(__s32 *regs);
void read_regs(int start, int end, __s32 *regs, int file);


/* Message structure decoding attempt:
 * There seems to be a 0x22 byte long message
 *
 * Address    Function
 * -------------------
 * 0x00       always 0x1b
 * 0x01       always 0x00
 * 0x02       always 0x04

 * 0x03       ??? (status byte for the next 4 bytes?)
 * 0x04       lsb x-position first finger
 * 0x05       msb x-position
 * 0x06       lsb y-position
 * 0x07       msb y-position

 * 0x08       ??? (status byte for the next 4 bytes?)
 * 0x09       lsb x-position second finger
 * 0x0a       msb x-position
 * 0x0b       lsb y-position
 * 0x0c       msb y-position

 * 0x0d       ??? (status byte for the next 4 bytes?)
 * 0x0e       lsb x-position third finger
 * 0x0f       msb x-position
 * 0x10       lsb y-position
 * 0x11       msb y-position

 * 0x12       ??? (status byte for the next 4 bytes?)
 * ...        ???
 * 
 * 0x17       changes randomly (maybe lsb of 0x18??)
 * 0x18       some kind of timer (counts up independent of tp action)
 * ...        ???
 * 0x1a       1 when a mouse button is pressed, 0 when not
 * ...        ???
 * 
 * A few things I noticed:
 * - I don't see something to indicate whether the left or right mouse button is pressed;
 *   but the position values dont always come alive, e.g. if I use my fingernail
 * - Sometimes the position values stay alive after removing my finger, but mostly not
 * - Sometimes the position values stay alive after stopping finger motion, but mostly not
 * - Maybe this indicates a tap/motion along with the status byte value?
*/
 

int main(int argc, char **argv) {
  int file;
  int adapter_nr = 8; //i2c-8 in my case
  char filename[20];
  int addr=0x2c; //address of the touchpad on the i2c bus
  int reg=0; //counter for the register
  int end=0x22;
  
  if(argc != 2)
    return(1);
  
  
  snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
  file=open(filename, I2C_RDWR);
  if(file < 0) {
    printf("Error opening\n");
    exit(1);
  }
  
  if(ioctl(file, I2C_SLAVE, addr) < 0) {
    printf("Error setting address\n");
    exit(1);
  }
  
  __s32 regs[23];
  int i;
  read_regs(reg,end,regs,file);

  
  //raw dump of the registers
  if(!strcmp(argv[1], "dump") || !strcmp(argv[1], "both")) {
    for(i=0;i<0x23;i++) {
      if(i==0x03 || i==0x08 || i==0x0d || i==0x12 || i ==0x17 || i==0x19) //formatting
	printf("\n");
      printf("%02x: %02x\n", i, regs[i]);
    }
    printf("\n");
  }


  //attempting to decode the message
  if(!strcmp(argv[1], "decode") || !strcmp(argv[1], "both"))
    decode(regs);

  close(file);
}


  

void decode(__s32 *regs) {
  int i;
  int positions[6];
  for(i=0;i<6;i++)
    positions[i]=regs[4+i*2+i/2]+regs[5+i*2+i/2]*256;
  for(i=0;i<3;i++)
    if(positions[i*2] || positions[i*2+1])
      printf("finger %d: x=%03x y=%03x\n",i, positions[i*2], positions[i*2+1]);
 
  if(regs[0x1a] & 1) //condition for click 
    printf("CLICK!\n");
}



void read_regs(int start, int end, __s32 *regs, int file) {
  __s32 res;
  for(;start<=end;start++) {
    res=i2c_smbus_read_byte_data(file, start);
    //    printf("rcv %x\n",start);
    if(res < 0) {
      printf("Error accessing reg %x: %d\n", start, res);
      regs[start]=0;
      continue;
    }
    regs[start]=res;
  }
}
