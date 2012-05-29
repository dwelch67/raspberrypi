
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

unsigned char data[10000];

int main ( void )
{
    unsigned int ra;

    fp=fopen("twain.txt","rb");
    if(fp==NULL) return(1);
    fread(data,1,sizeof(data),fp);
    fclose(fp);

    fp=fopen("testdata.h","wt");
    if(fp==NULL) return(1);
    fprintf(fp,"\n");
    fprintf(fp,"#define TESTDATALEN %u\n",(unsigned int)sizeof(data));
    fprintf(fp,"const unsigned char testdata[TESTDATALEN]=\n");
    fprintf(fp,"{");
    for(ra=0;ra<sizeof(data);ra++)
    {
        if((ra&15)==0) fprintf(fp,"\n");
        fprintf(fp,"0x%02X,",data[ra]);
    }
    fprintf(fp,"\n");
    fprintf(fp,"};");
    fprintf(fp,"\n");
    fclose(fp);


    return(0);
}

