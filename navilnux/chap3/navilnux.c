#include <navilnux.h>


int main(void)
{
	int i = 0;
	printf("Hello world!\n");
	while(1){
		GPIO_SetLED(i, LED_OFF);
		printf("LED %d off\n", i);
		msleep(500);
		GPIO_SetLED(i, LED_ON);
		printf("LED %d On\n", i);
		msleep(500);
		i++;
		
		if ( i>=4 )
			i = 0;
	}
	return 0;
}
