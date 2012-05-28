
void uart_send ( unsigned int );
void hexstring ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x0D);
    uart_send(0x0A);
}
#define AUX_MU_IO_REG   (*((volatile unsigned int *)0x20215040))
#define AUX_MU_LSR_REG  (*((volatile unsigned int *)0x20215054))
void uart_send ( unsigned int x )
{
    while(1)
    {
        if(AUX_MU_LSR_REG&0x20) break;
    }
    AUX_MU_IO_REG=x;
}
