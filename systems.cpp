#include <systems.h>

volatile void delay_ms(uint32_t ms) {
	__asm__ __volatile__(	".syntax unified\n"
			//"mov r0, %[num] \n"
			"1: subs %[num], #1 \n"
			"bne 1b \n"
			"bx lr \n"
			".syntax divided\n"
         :
         : [num] "r" ((ms*72000-6)/6)
         : );
}
volatile void delay_us(uint32_t us) {
	__asm__ __volatile__(	".syntax unified\n"
			//"mov r0, %[num] \n"
			"1: subs %[num], #1 \n"
			"bne 1b \n"
			"bx lr \n"
			".syntax divided\n"
         :
         : [num] "r" ((us*72-6)/6)
         : );
}
volatile void robust_delay(uint32_t gib)
{
int a=0,b=0;
 for(int i=0;i<gib*3000;i++){a+=i;}
 b=a;a=b;
}
volatile void robust_us(uint32_t gib)
{
int a=0,b=0;
 for(int i=0;i<gib*3;i++){a+=i;}
 b=a;a=b;
}
