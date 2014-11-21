#include "periph.h"
#include "display.h"

 int strcmp(const char *cs, const char *ct)
{
         unsigned char c1, c2;
 
         while (1) {
                 c1 = *cs++;
                 c2 = *ct++;
                 if (c1 != c2)
                         return c1 < c2 ? -1 : 1;
                 if (!c1)
                         break;
         }
         return 0;
}
int evalCommand(char buffer[], int x, int y){
	char cmd[100];
	char arg[100];
	int i;
	i = 0;
	while(buffer[i] != '\0' && buffer[i] != ' ' && i < 100){
		cmd[i] = buffer[i];
		i++;
	}
	hexstring(i);
	cmd[i + 1] = '\0';
	i++;
	int i0;
	i0 = i;
	while(buffer[i] != '\0' && buffer[i] != ' ' && i < 200){
		arg[i-i0] = buffer[i];
		i++;
	}
	uart_puts(cmd);
	uart_puts("\r\n");
	uart_puts(arg);
	if(strcmp(cmd,"echo")){
		uart_puts("echoing\r\n");
		drawString(arg,x,y);
			y+= 16;

	}
	i=0;
	for(i=0; i<100; i++){
		cmd[i] = 0;
		arg[i] = 0;
	}
	return y;

}
