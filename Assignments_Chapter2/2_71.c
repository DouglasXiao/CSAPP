//  a. The older method is incorrect because it doesn't treat each byte as a signed value, 


#include <stdio.h>

/* declaration of data type where 4 bytes are packed 
   into an unsigned */

typedef unsigned packed_t;

/* Failed attempt at xbyte */ 
/* extract byte from word. Return as signed integer */
int xbyte(packed_t word, int bytenum)
{
    return (word >> (bytenum <<3)) & 0xFF;
}

/* change the function below to give the correct answer */ 
int myxbyte(packed_t word, int bytenum)
{
    int temp = word << ((3-bytenum) << 3);
    return (temp >> 24 );
}

int main(void)
{
    /* set x to obvious byte pattern */
    packed_t x = 0x1234ABFF;

    printf("Packed value (x) = 0x%x, value returned by xbyte(x,0) = 0x%x\n",
	   x, xbyte(x,0));
    printf("Packed value (x) = 0x%x, value returned by xbyte(x,1) = 0x%x\n",
	   x, xbyte(x,1));
    printf("Packed value (x) = 0x%x, value returned by xbyte(x,2) = 0x%x\n",
	   x, xbyte(x,2));
    printf("Packed value (x) = 0x%x, value returned by xbyte(x,3) = 0x%x\n",
	   x, xbyte(x,3));
    

    printf("Packed value (x) = 0x%x, value returned by myxbyte(x,0) = 0x%x\n",
	   x, myxbyte(x,0));
    printf("Packed value (x) = 0x%x, value returned by myxbyte(x,1) = 0x%x\n",
	   x, myxbyte(x,1));
    printf("Packed value (x) = 0x%x, value returned by myxbyte(x,2) = 0x%x\n",
	   x, myxbyte(x,2));
    printf("Packed value (x) = 0x%x, value returned by myxbyte(x,3) = 0x%x\n",
	   x, myxbyte(x,3));
}


//Packed value (x) = 0x1234abff, value returned by xbyte(x,0) = 0xff
//Packed value (x) = 0x1234abff, value returned by xbyte(x,1) = 0xab
//Packed value (x) = 0x1234abff, value returned by xbyte(x,2) = 0x34
//Packed value (x) = 0x1234abff, value returned by xbyte(x,3) = 0x12
//Packed value (x) = 0x1234abff, value returned by myxbyte(x,0) = 0xffffffff
//Packed value (x) = 0x1234abff, value returned by myxbyte(x,1) = 0xffffffab
//Packed value (x) = 0x1234abff, value returned by myxbyte(x,2) = 0x34
//Packed value (x) = 0x1234abff, value returned by myxbyte(x,3) = 0x12
    