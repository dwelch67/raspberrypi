
//-------------------------------------------------------------------
// Copyright (C) 2012 David Welch
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINCLUDES 10
#define FNAMELEN 255

FILE *fpin[MAXINCLUDES];
FILE *fpout;
unsigned char filename[FNAMELEN][MAXINCLUDES];

unsigned int rd,rm,rn,rx;
unsigned int bl_upper;
unsigned int is_const,is_label;

#define ADDMASK 0xFFFF
unsigned short mem[ADDMASK+1];
unsigned short mark[ADDMASK+1];

unsigned int baseadd;
unsigned int curradd;
unsigned int line[MAXINCLUDES];
unsigned int currlevel;

char cstring[1024];
char newline[1024];


#define LABLEN 64

#define MAX_LABS 65536
struct
{
    char name[LABLEN];
    unsigned int addr;
    unsigned int line;
    unsigned int type;
    char fname[FNAMELEN];
} lab_struct[MAX_LABS];
unsigned int nlabs;
unsigned int lab;


#define NREGNAMES (19)
struct
{
    char name[4];
    unsigned char num;
} reg_names[NREGNAMES]=
{
    {"r0",0},{"r1",1},{"r2",2},{"r3",3},{"r4",4},{"r5",5},{"r6",6},{"r7",7},
    {"r8",8},{"r9",9},{"r10",10},{"r11",11},{"r12",12},{"r13",13},{"r14",14},{"r15",15},
    {"sp",13},{"lr",14},{"pc",15}
};


unsigned char hexchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

unsigned char numchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int rest_of_line ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra])
    {
        printf("<%s:%u> Error: garbage at end of line\n",filename[currlevel],line[currlevel]);
        return(1);
    }
    return(0);
}
//-------------------------------------------------------------------
int get_reg_number ( char *s, unsigned int *rx )
{
    unsigned int ra;

    for(ra=0;ra<NREGNAMES;ra++)
    {
        if(strcmp(s,reg_names[ra].name)==0)
        {
            *rx=reg_names[ra].num;
            return(0);
        }
    }
    return(1);
}
//-------------------------------------------------------------------
unsigned int parse_immed ( unsigned int ra )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if((newline[ra]==0x30)&&((newline[ra+1]=='x')||(newline[ra+1]=='X')))
    {
        ra+=2;
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==',') break;
            if(newline[ra]==']') break;
            if(newline[ra]==0x20) break;
            if(!hexchar[newline[ra]])
            {
                printf("<%s:%u> Error: invalid number\n",filename[currlevel],line[currlevel]);
                return(0);
            }
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(rb==0)
        {
            printf("<%s:%u> Error: invalid number\n",filename[currlevel],line[currlevel]);
            return(0);
        }
        rx=strtoul(cstring,NULL,16);
    }
    else
    {
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==',') break;
            if(newline[ra]==']') break;
            if(newline[ra]==0x20) break;
            if(!numchar[newline[ra]])
            {
                printf("<%s:%u> Error: invalid number\n",filename[currlevel],line[currlevel]);
                return(0);
            }
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(rb==0)
        {
            printf("<%s:%u> Error: invalid number\n",filename[currlevel],line[currlevel]);
            return(0);
        }
        rx=strtoul(cstring,NULL,10);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_reg ( unsigned int ra )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    rb=0;
    for(;newline[ra];ra++)
    {
        if(newline[ra]==',') break;
        if(newline[ra]==']') break;
        if(newline[ra]=='}') break;
        if(newline[ra]==0x20) break;
        cstring[rb++]=newline[ra];
    }
    cstring[rb]=0;
    if(get_reg_number(cstring,&rx))
    {
        printf("<%s:%u> Error: not a register\n",filename[currlevel],line[currlevel]);
        return(0);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_low_reg ( unsigned int ra )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    rb=0;
    for(;newline[ra];ra++)
    {
        if(newline[ra]==',') break;
        if(newline[ra]=='!') break;
        if(newline[ra]=='}') break;
        if(newline[ra]==']') break;
        if(newline[ra]==0x20) break;
        cstring[rb++]=newline[ra];
    }
    cstring[rb]=0;
    if(get_reg_number(cstring,&rx))
    {
        printf("<%s:%u> Error: not a register\n",filename[currlevel],line[currlevel]);
        return(0);
    }
    if(rx>7)
    {
        printf("<%s:%u> Error: invalid (high) register\n",filename[currlevel],line[currlevel]);
        return(0);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_comma ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]!=',')
    {
        printf("<%s:%u> Error: syntax error\n",filename[currlevel],line[currlevel]);
        return(0);
    }
    ra++;
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_character ( unsigned int ra, unsigned char ch )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]!=ch)
    {
        printf("<%s:%u> Error: syntax error\n",filename[currlevel],line[currlevel]);
        return(0);
    }
    ra++;
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int no_spaces ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_two_regs ( unsigned int ra )
{
    ra=parse_low_reg(ra); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    ra=parse_low_reg(ra); if(ra==0) return(0);
    rm=rx;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_branch_label ( unsigned int ra )
{
    unsigned int rb;
    unsigned int rc;

    is_const=0;
    is_label=0;
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(numchar[newline[ra]])
    {
        ra=parse_immed(ra); if(ra==0) return(0);
        is_const=1;
    }
    else
    {
        //assume label, find space or eol.
        for(rb=ra;newline[rb];rb++)
        {
            if(newline[rb]==0x20) break; //no spaces in labels
        }
        //got a label
        rc=rb-ra;
        if(rc==0)
        {
            printf("<%s:%u> Error: Invalid label\n",filename[currlevel],line[currlevel]);
            return(0);
        }
        rc--;
        if(rc>=LABLEN)
        {
            printf("<%s:%u> Error: Label too long\n",filename[currlevel],line[currlevel]);
            return(0);
        }
        for(rb=0;rb<=rc;rb++)
        {
            lab_struct[nlabs].name[rb]=newline[ra++];
        }
        lab_struct[nlabs].name[rb]=0;
        lab_struct[nlabs].addr=curradd;
        lab_struct[nlabs].line=line[currlevel];
        strcpy(lab_struct[nlabs].fname,filename[currlevel]);
        lab_struct[nlabs].type=1;
        nlabs++;
        rx=0;
        is_label=1;
    }
    return(ra);
}//-------------------------------------------------------------------
//-------------------------------------------------------------------
int assemble ( unsigned int level )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    int ret;

printf("assemble(%u)\n",level);

    currlevel=level;

    if(level>=MAXINCLUDES)
    {
        printf("<%s:%u> Error: too many levels of include\n",filename[currlevel],line[currlevel]);
        return(1);
    }

    while(fgets(newline,sizeof(newline)-1,fpin[level]))
    {
        line[level]++;
        //tabs to spaces and other things
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]<0x20)  newline[ra]=0x20;
            if(newline[ra]>=0x7F) newline[ra]=0;
        }

        //various ways to comment lines
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]==';') newline[ra]=0;
            if(newline[ra]=='@') newline[ra]=0;
            if((newline[ra]=='/')&&(newline[ra+1]=='/')) newline[ra]=0;
            if(newline[ra]==0) break;
        }

        //skip spaces
        for(ra=0;newline[ra];ra++) if(newline[ra]!=0x20) break;
        if(newline[ra]==0) continue;

        //look for a label?
        for(rb=ra;newline[rb];rb++)
        {
            if(newline[rb]==0x20) break; //no spaces in labels
            if(newline[rb]==':') break;
        }
        if(newline[rb]==':')
        {
            //got a label
            rc=rb-ra;
            if(rc==0)
            {
                printf("<%s:%u> Error: Invalid label\n",filename[currlevel],line[currlevel]);
                return(1);
            }
            rc--;
            if(rc>=LABLEN)
            {
                printf("<%s:%u> Error: Label too long\n",filename[currlevel],line[currlevel]);
                return(1);
            }
            for(rb=0;rb<=rc;rb++)
            {
                lab_struct[nlabs].name[rb]=newline[ra++];
            }
            lab_struct[nlabs].name[rb]=0;
            lab_struct[nlabs].addr=curradd<<1;
            lab_struct[nlabs].line=line[currlevel];
            strcpy(lab_struct[nlabs].fname,filename[currlevel]);
            lab_struct[nlabs].type=0;
            ra++;
            for(lab=0;lab<nlabs;lab++)
            {
                if(lab_struct[lab].type) continue;
                if(strcmp(lab_struct[lab].name,lab_struct[nlabs].name)==0) break;
            }
            if(lab<nlabs)
            {
                printf("<%s:%u> Error: label [%s] already defined on line %u\n",filename[currlevel],line[currlevel],lab_struct[lab].name,lab_struct[lab].line);
                return(1);
            }
            nlabs++;
            //skip spaces
            for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
            if(newline[ra]==0) continue;
        }
// .include -----------------------------------------------------------
        if(strncmp(&newline[ra],".include ",9)==0)
        {
            if((level+1)>=MAXINCLUDES)
            {
                printf("<%s:%u> Error: too many levels of include\n",filename[currlevel],line[currlevel]);
                return(1);
            }
            ra+=9;
            for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
            for(rd=0;newline[ra];ra++,rd++)
            {
                if(rd>=FNAMELEN)
                {
                    printf("<%s:%u> Error: filename too long\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
                filename[level+1][rd]=newline[ra];
                if(newline[ra]==0x20) break;
            }
            filename[level+1][rd]=0;
            if(rest_of_line(ra)) return(1);
            fpin[level+1]=fopen(filename[level+1],"rt");
            if(fpin[level+1]==NULL)
            {
                printf("<%s:%u> Error opening file [%s]\n",filename[currlevel],line[currlevel],filename[level+1]);
                return(1);
            }
            line[level+1]=0;
            ret=assemble(level+1);
            fclose(fpin[level+1]);
            if(ret)
            {
                return(1);
            }
            continue;
        }
// .align -----------------------------------------------------------
        if(strncmp(&newline[ra],".align",6)==0)
        {
            ra+=6;
            if(rest_of_line(ra)) return(1);
            if(curradd&1)
            {
                printf("<%s:%u> Adding halfword to align on a word boundary\n",filename[currlevel],line[currlevel]);
                mem[curradd]=0x0000;
                mark[curradd]|=0x9000;
                curradd++;
            }
            continue;
        }
// .word -----------------------------------------------------------
        if(strncmp(&newline[ra],".word ",6)==0)
        {
            ra+=6;
            if(curradd&1)
            {
                printf("<%s:%u> Error: not word aligned\n",filename[currlevel],line[currlevel]);
                return(1);
            }
            for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
            if(newline[ra]=='=')
            {
                ra=parse_character(ra,'='); if(ra==0) return(1);
                ra=parse_branch_label(ra); if(ra==0) return(1);
                if((is_const)||(!is_label))
                {
                    printf("<%s:%u> Error: expecting a label\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
                mem[curradd]=0x0000;
                mark[curradd]|=0x9000;
                curradd++;
                mem[curradd]=0x0000;
                mark[curradd]|=0x9000;
                curradd++;
            }
            else
            {
                while(1)
                {
                    ra=parse_immed(ra); if(ra==0) return(1);
                    mem[curradd]=rx&0xFFFF;
                    mark[curradd]|=0x9000;
                    curradd++;
                    mem[curradd]=(rx>>16)&0xFFFF;
                    mark[curradd]|=0x9000;
                    curradd++;
                    if(newline[ra]!=',') break;
                    ra=parse_comma(ra); if(ra==0) return(1);
                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// .hword -----------------------------------------------------------
        if(strncmp(&newline[ra],".hword ",7)==0)
        {
            ra+=7;
            while(1)
            {
                ra=parse_immed(ra); if(ra==0) return(1);
                if((rx&0xFFFF)!=rx)
                {
                    printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
                mem[curradd]=rx;
                mark[curradd]|=0x9000;
                curradd++;
                if(newline[ra]!=',') break;
                ra=parse_comma(ra); if(ra==0) return(1);
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// adc -----------------------------------------------------------
        if(strncmp(&newline[ra],"adc ",4)==0)
        {
            ra+=4;
            //adc rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4140|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// add -----------------------------------------------------------
        if(strncmp(&newline[ra],"add ",4)==0)
        {
            ra+=4;
            //add rd,rm,#immed_3
            //add rd,#immed_8
            //add rd,rn,rm
            //add rd,rm one or both high
            //add rd,pc,#immed_8*4
            //add rd,sp,#immed_8*4
            //add sp,#immed_7*4
            ra=parse_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(rd>7)
            {
                //add rdhi,
                if((rd==13)&&(newline[ra]=='#'))
                {
                    ra++;
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x1FC)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //add sp,#immed_7*4
                    mem[curradd]=0xB000|(rx>>2);
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    ra=parse_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    //add rd,rm one or both high
                    mem[curradd]=0x4400|((rd&8)<<4)|(rm<<3)|(rd&7);
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                //add rdlo,
                if(newline[ra]=='#')
                {
                    ra++;
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0xFF)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //add rd,#immed_8
                    mem[curradd]=0x3000|(rd<<8)|rx;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //add rdlo,r?
                    ra=parse_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    if(rm>7)
                    {
                        //add rdlo,rhi
                        if(newline[ra]==',')
                        {
                            //add rdlo,rhi,
                            ra=parse_comma(ra); if(ra==0) return(1);
                            if(newline[ra]!='#')
                            {
                                printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                            if(rm==15)
                            {
                                ra++;
                                ra=parse_immed(ra); if(ra==0) return(1);
                                if((rx&0x3FC)!=rx)
                                {
                                    printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                                    return(1);
                                }
                                //add rd,pc,#immed_8*4
                                mem[curradd]=0xA000|(rd<<8)|(rx>>2);
                                mark[curradd]=0x8000;
                                curradd++;
                            }
                            else
                            if(rm==13)
                            {
                                ra++;
                                ra=parse_immed(ra); if(ra==0) return(1);
                                if((rx&0x3FC)!=rx)
                                {
                                    printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                                    return(1);
                                }
                                //add rd,pc,#immed_8*4
                                mem[curradd]=0xA800|(rd<<8)|(rx>>2);
                                mark[curradd]=0x8000;
                                curradd++;
                            }
                            else
                            {
                                printf("<%s:%u> Error: Invalid second register\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                        }
                        else
                        {
                            //add rdlo,rdhi
                            //add rd,rm one or both high
                            mem[curradd]=0x4400|((rd&8)<<4)|(rm<<3)|(rd&7);
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                    }
                    else
                    {
                        //add rlo,rlo
                        ra=parse_comma(ra); if(ra==0) return(1);
                        if(newline[ra]=='#')
                        {
                            rn=rm;
                            ra++;
                            ra=parse_immed(ra); if(ra==0) return(1);
                            if((rx&0x7)!=rx)
                            {
                                printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                            //add rd,rm,#immed_3
                            mem[curradd]=0x1C00|(rx<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                        else
                        {
                            rn=rm;
                            ra=parse_low_reg(ra); if(ra==0) return(1);
                            rm=rx;
                            mem[curradd]=0x1800|(rm<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                    }
                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// and -----------------------------------------------------------
        if(strncmp(&newline[ra],"and ",4)==0)
        {
            ra+=4;
            //and rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4000|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// asr -----------------------------------------------------------
        if(strncmp(&newline[ra],"asr ",4)==0)
        {
            ra+=4;
            //asr rd,rm,#immed_5
            //asr rd,rs
            ra=parse_two_regs(ra); if(ra==0) return(1);
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                if(newline[ra]=='#')
                {
                    ra++;
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx<1)||(rx>32))
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    rx&=0x1F;
                    //asr rd,rm,#immed_5
                    mem[curradd]=0x1000|(rx<<6)|(rm<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
            }
            else
            {
                //asr rd,rs
                mem[curradd]=0x4100|(rm<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// b -----------------------------------------------------------
        if(strncmp(&newline[ra],"b ",2)==0)
        {
            ra+=2;
            //b label
            //b immed
            ra=parse_branch_label(ra); if(ra==0) return(1);
            if(rest_of_line(ra)) return(1);
            if(is_label)
            {
                mem[curradd]=0xE000;
                mark[curradd]=0x8002;
                curradd++;
                continue;
            }
            if(is_const)
            {
                rd=rx-((curradd<<1)+4);
                rm=0xFFFFFFFF<<11;
                if(rd&rm)
                {
                    if((rd&rm)!=rm)
                    {
                        printf("<%s:%u> Error: Branch destination too far\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                }
                //ignore even/odd address?
                rm=(rd>>1)&0x7FF;
                mem[curradd]=0xE000|rm;
                mark[curradd]=0x8002;
                curradd++;
                continue;
            }
        }
// bcond -----------------------------------------------------------
        rn=0;
        if(strncmp(&newline[ra],"beq ",4)==0) rn=0x10;
        if(strncmp(&newline[ra],"bne ",4)==0) rn=0x11;
        if(strncmp(&newline[ra],"bcs ",4)==0) rn=0x12;
        if(strncmp(&newline[ra],"bcc ",4)==0) rn=0x13;
        if(strncmp(&newline[ra],"bmi ",4)==0) rn=0x14;
        if(strncmp(&newline[ra],"bpl ",4)==0) rn=0x15;
        if(strncmp(&newline[ra],"bvs ",4)==0) rn=0x16;
        if(strncmp(&newline[ra],"bvc ",4)==0) rn=0x17;
        if(strncmp(&newline[ra],"bhi ",4)==0) rn=0x18;
        if(strncmp(&newline[ra],"bls ",4)==0) rn=0x19;
        if(strncmp(&newline[ra],"bge ",4)==0) rn=0x1A;
        if(strncmp(&newline[ra],"blt ",4)==0) rn=0x1B;
        if(strncmp(&newline[ra],"bgt ",4)==0) rn=0x1C;
        if(strncmp(&newline[ra],"ble ",4)==0) rn=0x1D;
        if(strncmp(&newline[ra],"bal ",4)==0) rn=0x1E;
        if(rn)
        {
            ra+=3;
            ra=parse_branch_label(ra); if(ra==0) return(1);
            if(rest_of_line(ra)) return(1);
            if(is_label)
            {
                mem[curradd]=0xD000|(rn<<8);
                mark[curradd]=0x8002;
                curradd++;
                continue;
            }
            if(is_const)
            {
                rd=rx-((curradd<<1)+4);
                rm=0xFFFFFFFF<<8;
                if(rd&rm)
                {
                    if((rd&rm)!=rm)
                    {
                        printf("<%s:%u> Error: Branch destination too far\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                }
                //ignore even/odd address?
                rm=(rd>>1)&0xFF;
                rn&=0xF;
                mem[curradd]=0xD000|(rn<<8)|rm;
                mark[curradd]=0x8002;
                curradd++;
                continue;
            }
        }
// bic -----------------------------------------------------------
        if(strncmp(&newline[ra],"bic ",4)==0)
        {
            ra+=4;
            //bic rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4380|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// bkpt -----------------------------------------------------------
        if(strncmp(&newline[ra],"bkpt ",5)==0)
        {
            ra+=5;
            //bkpt immed_8
            ra=parse_immed(ra); if(ra==0) return(1);
            if((rx&0xFF)!=rx)
            {
                printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                return(1);
            }
            mem[curradd]=0xBE00|rx;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// bl -----------------------------------------------------------
        if(strncmp(&newline[ra],"bl ",3)==0)
        {
            ra+=3;
            //bl label
            //bl immed
            ra=parse_branch_label(ra); if(ra==0) return(1);
            if(rest_of_line(ra)) return(1);
            if(is_label)
            {
                mem[curradd]=0xF000;
                mark[curradd]=0x8002;
                curradd++;
                mem[curradd]=0xF800;
                mark[curradd]=0x8002;
                curradd++;
                continue;
            }
            if(is_const)
            {
                rd=rx-((curradd<<1)+4);
                rm=0xFFFFFFFF<<22;
                if(rd&rm)
                {
                    if((rd&rm)!=rm)
                    {
                        printf("<%s:%u> Error: Branch destination too far\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                }
                //ignore even/odd address?
                rn=(rd>>12)&0x7FF;
                rm=(rd>>1)&0x7FF;
                mem[curradd]=0xF000|rn;
                mark[curradd]=0x8002;
                curradd++;
                mem[curradd]=0xF800|rm;
                mark[curradd]=0x8002;
                curradd++;
                continue;
            }
        }
// bx -----------------------------------------------------------
        if(strncmp(&newline[ra],"bx ",3)==0)
        {
            ra+=3;
            //bx rm
            ra=parse_reg(ra); if(ra==0) return(1);
            rm=rx;
            mem[curradd]=0x4700|(rm<<3);
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// cmn -----------------------------------------------------------
        if(strncmp(&newline[ra],"cmn ",4)==0)
        {
            ra+=4;
            //cmn rn,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x42C0|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// cmp -----------------------------------------------------------
        if(strncmp(&newline[ra],"cmp ",4)==0)
        {
            ra+=4;
            //cmp rn,#immed_8
            //cmp rn,rm both low
            //cmp rn,rm one or the other high
            ra=parse_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(rn>7)
            {
                ra=parse_reg(ra); if(ra==0) return(1);
                rm=rx;
                if(rm>7)
                {
                    if(rm==15)
                    {
                        printf("<%s:%u> Error: Not wise to use r15 in this way\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //cmp rn,rm one or the other high
                    mem[curradd]=0x4500|((rn&8)<<4)|(rm<<3)|rn;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //cmp rn,rm both low
                    mem[curradd]=0x4280|(rm<<3)|rn;
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                if(newline[ra]=='#')
                {
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0xFF)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //cmp rn,#immed_8
                    mem[curradd]=0x2800|(rn<<8)|rx;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    ra=parse_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    if(rm>7)
                    {
                        if(rm==15)
                        {
                            printf("<%s:%u> Error: Not wise to use r15 in this way\n",filename[currlevel],line[currlevel]);
                            return(1);
                        }
                        //cmp rn,rm one or the other high
                        mem[curradd]=0x4500|((rn&8)<<4)|(rm<<3)|rn;
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                    else
                    {
                        //cmp rn,rm both low
                        mem[curradd]=0x4280|(rm<<3)|rn;
                        mark[curradd]=0x8000;
                        curradd++;
                    }

                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// eor -----------------------------------------------------------
        if(strncmp(&newline[ra],"eor ",4)==0)
        {
            ra+=4;
            //eor rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4040|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldmia -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldmia ",6)==0)
        {
            ra+=6;
            //ldmia rn!,{r0,r1,...,r7}
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_character(ra,'!'); if(ra==0) return(1);
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'{'); if(ra==0) return(1);
            rm=0;
            while(1)
            {
                ra=parse_low_reg(ra); if(ra==0) return(1);
                if(rm&(1<<rx))
                {
                    printf("<%s:%u> Warning: You already specified r%u\n",filename[currlevel],line[currlevel],rx);
                }
                rm|=(1<<rx);
                if(newline[ra]=='}')
                {
                    ra++;
                    break;
                }
                ra=parse_comma(ra); if(ra==0) return(1);
            }
            mem[curradd]=0xC800|(rn<<8)|rm;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldr -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldr ",4)==0)
        {
            ra+=4;
            //ldr rd,[rn,#immed_5*4]
            //ldr rd,[rn,rm]
            //ldr rd,[pc,#immed_8*4]
            //ldr rd,[sp,#immed_8*4]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(newline[ra]=='[')
            {
                ra=parse_character(ra,'['); if(ra==0) return(1);
                ra=parse_reg(ra); if(ra==0) return(1);
                rn=rx;
                if(rx>7)
                {
                    if(rn==15)
                    {
                    }
                    else
                    if(rn==13)
                    {
                    }
                    else
                    {
                        printf("<%s:%u> Error: Invalid base register\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    if(newline[ra]==']')
                    {
                        rx=0;
                    }
                    else
                    {
                        ra=parse_comma(ra); if(ra==0) return(1);
                        ra=parse_character(ra,'#'); if(ra==0) return(1);
                        ra=parse_immed(ra); if(ra==0) return(1);
                        if((rx&0x3FC)!=rx)
                        {
                            printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                            return(1);
                        }
                    }
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    //ldr rd,[pc,#immed_8*4]
                    //ldr rd,[sp,#immed_8*4]
                    mem[curradd]=0x0000|(rd<<8)|(rx>>2);
                    if(rn==15) mem[curradd]|=0x4800;
                    if(rn==13) mem[curradd]|=0x9800;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    if(newline[ra]==',')
                    {
                        ra=parse_comma(ra); if(ra==0) return(1);
                        if(newline[ra]=='#')
                        {
                            //ldr rd,[rn,#immed_5*4]
                            ra=parse_character(ra,'#'); if(ra==0) return(1);
                            ra=parse_immed(ra); if(ra==0) return(1);
                            if((rx&0x7C)!=rx)
                            {
                                printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                            ra=parse_character(ra,']'); if(ra==0) return(1);
                            mem[curradd]=0x6800|((rx>>2)<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                        else
                        {
                            //ldr rd,[rn,rm]
                            ra=parse_low_reg(ra); if(ra==0) return(1);
                            rm=rx;
                            ra=parse_character(ra,']'); if(ra==0) return(1);
                            mem[curradd]=0x5800|(rm<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                    }
                    else
                    {
                        //ldr rd,[rn] immed_5 = 0
                        ra=parse_character(ra,']'); if(ra==0) return(1);
                        mem[curradd]=0x6800|(0<<6)|(rn<<3)|rd;
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                }
            }
            else
            {
                ra=parse_branch_label(ra); if(ra==0) return(1);
                if(is_const)
                {
                    printf("<%s:%u> Error: not a label",filename[currlevel],line[currlevel]);
                    return(1);
                }
                mem[curradd]=0x4800|(rd<<8)|0;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldrb -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldrb ",5)==0)
        {
            ra+=5;
            //ldrb rd,[rn,#immed_5]
            //ldrb rd,[rn,rm]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                if(newline[ra]=='#')
                {
                    //ldrb rd,[rn,#immed_5]
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x1F)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x7800|((rx>>0)<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //ldrb rd,[rn,rm]
                    ra=parse_low_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x5C00|(rm<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                //ldrb rd,[rn] immed_5 = 0
                ra=parse_character(ra,']'); if(ra==0) return(1);
                mem[curradd]=0x7800|(0<<6)|(rn<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldrh -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldrh ",5)==0)
        {
            ra+=5;
            //ldrh rd,[rn,#immed_5]
            //ldrh rd,[rn,rm]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                if(newline[ra]=='#')
                {
                    //ldrh rd,[rn,#immed_5]
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&(0x1F<<1))!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x8800|((rx>>1)<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //ldrb rd,[rn,rm]
                    ra=parse_low_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x5A00|(rm<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                //ldrh rd,[rn] immed_5 = 0
                ra=parse_character(ra,']'); if(ra==0) return(1);
                mem[curradd]=0x8800|(0<<6)|(rn<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldrsb -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldrsb ",6)==0)
        {
            ra+=6;
            //ldrsb rd,[rn,rm]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rm=rx;
            ra=parse_character(ra,']'); if(ra==0) return(1);
            mem[curradd]=0x5600|(rm<<6)|(rn<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldrsh -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldrsh ",6)==0)
        {
            ra+=6;
            //ldrsh rd,[rn,rm]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rm=rx;
            ra=parse_character(ra,']'); if(ra==0) return(1);
            mem[curradd]=0x5E00|(rm<<6)|(rn<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// lsl -----------------------------------------------------------
        if(strncmp(&newline[ra],"lsl ",4)==0)
        {
            ra+=4;
            //lsl rd,rm,#immed_5
            //lsl rd,rs
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rm=rx;
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                ra=parse_character(ra,'#'); if(ra==0) return(1);
                ra=parse_immed(ra); if(ra==0) return(1);
                if((rx<1)||(rx>32))
                {
                    printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
                rx&=0x1F;
                //lsl rd,rm,#immed_5
                mem[curradd]=0x0000|(rx<<6)|(rm<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            else
            {
                //lsl rd,rs
                mem[curradd]=0x4080|(rm<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// lsr -----------------------------------------------------------
        if(strncmp(&newline[ra],"lsr ",4)==0)
        {
            ra+=4;
            //lsr rd,rm,#immed_5
            //lsr rd,rs
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rm=rx;
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                ra=parse_character(ra,'#'); if(ra==0) return(1);
                ra=parse_immed(ra); if(ra==0) return(1);
                if((rx<1)||(rx>32))
                {
                    printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
                rx&=0x1F;
                //lsr rd,rm,#immed_5
                mem[curradd]=0x0800|(rx<<6)|(rm<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            else
            {
                //lsr rd,rs
                mem[curradd]=0x40C0|(rm<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// mov -----------------------------------------------------------
        if(strncmp(&newline[ra],"mov ",4)==0)
        {
            ra+=4;
            //mov rd,#immed_8
            //mov rd,rn low
            //mov rd,rm one or both high
            ra=parse_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(rd>7)
            {
                ra=parse_reg(ra); if(ra==0) return(1);
                rm=rx;
                //mov rd,rm
                mem[curradd]=0x4600|((rd&8)<<4)|(rm<<3)|(rd&7);
                mark[curradd]=0x8000;
                curradd++;
            }
            else
            {
                if(newline[ra]=='#')
                {
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0xFF)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //mov rd,#immed_8
                    mem[curradd]=0x2000|(rd<<8)|rx;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    ra=parse_reg(ra); if(ra==0) return(1);
                    if(rx>7)
                    {
                        rm=rx;
                        //mov rd,rm
                        mem[curradd]=0x4600|((rd&8)<<4)|(rm<<3)|(rd&7);
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                    else
                    {
                        rn=rx;
                        //mov rd,rn both low
                        mem[curradd]=0x1C00|(rn<<3)|rd;
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// mul -----------------------------------------------------------
        if(strncmp(&newline[ra],"mul ",4)==0)
        {
            ra+=4;
            //mul rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            if(rd==rm)
            {
                printf("<%s:%u> Warning: using the same register might not work\n",filename[currlevel],line[currlevel]);
            }
            mem[curradd]=0x4340|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// mvn -----------------------------------------------------------
        if(strncmp(&newline[ra],"mvn ",4)==0)
        {
            ra+=4;
            //mvn rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x43C0|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// neg -----------------------------------------------------------
        if(strncmp(&newline[ra],"neg ",4)==0)
        {
            ra+=4;
            //neg rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4240|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// orr -----------------------------------------------------------
        if(strncmp(&newline[ra],"orr ",4)==0)
        {
            ra+=4;
            //orr rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4300|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// pop -----------------------------------------------------------
        if(strncmp(&newline[ra],"pop ",4)==0)
        {
            ra+=4;
            //pop {r0,r1,...,r7}
            ra=parse_character(ra,'{'); if(ra==0) return(1);
            rm=0;
            while(1)
            {
                ra=parse_reg(ra); if(ra==0) return(1);
                if(rx>7)
                {
                    if(rx!=15)
                    {
                        printf("<%s:%u> Error: Invalid Register\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    rx=8;
                }
                rm|=(1<<rx);
                if(newline[ra]=='}')
                {
                    ra=parse_character(ra,'}'); if(ra==0) return(1);
                    break;
                }
                ra=parse_comma(ra); if(ra==0) return(1);
            }
            mem[curradd]=0xBC00|rm;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// push -----------------------------------------------------------
        if(strncmp(&newline[ra],"push ",5)==0)
        {
            ra+=5;
            //push {r0,r1,...,r7}
            ra=parse_character(ra,'{'); if(ra==0) return(1);
            rm=0;
            while(1)
            {
                ra=parse_reg(ra); if(ra==0) return(1);
                if(rx>7)
                {
                    if(rx!=14)
                    {
                        printf("<%s:%u> Error: Invalid Register\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    rx=8;
                }
                rm|=(1<<rx);
                if(newline[ra]=='}')
                {
                    ra=parse_character(ra,'}'); if(ra==0) return(1);
                    break;
                }
                ra=parse_comma(ra); if(ra==0) return(1);
            }
            mem[curradd]=0xB400|rm;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ror -----------------------------------------------------------
        if(strncmp(&newline[ra],"ror ",4)==0)
        {
            ra+=4;
            //ror rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x41C0|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// sbc -----------------------------------------------------------
        if(strncmp(&newline[ra],"sbc ",4)==0)
        {
            ra+=4;
            //ror rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4180|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// stmia -----------------------------------------------------------
        if(strncmp(&newline[ra],"stmia ",6)==0)
        {
            ra+=6;
            //stmia rn!,{r0,r1,...,r7}
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_character(ra,'!'); if(ra==0) return(1);
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'{'); if(ra==0) return(1);
            rm=0;
            while(1)
            {
                ra=parse_low_reg(ra); if(ra==0) return(1);
                if(rm&(1<<rx))
                {
                    printf("<%s:%u> Warning: You already specified r%u\n",filename[currlevel],line[currlevel],rx);
                }
                rm|=(1<<rx);
                if(newline[ra]=='}')
                {
                    ra++;
                    break;
                }
                ra=parse_comma(ra); if(ra==0) return(1);
            }
            mem[curradd]=0xC000|(rn<<8)|rm;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// str -----------------------------------------------------------
        if(strncmp(&newline[ra],"str ",4)==0)
        {
            ra+=4;
            //str rd,[rn,#immed_5*4]
            //str rd,[rn,rm]
            //str rd,[sp,#immed_8*4]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_reg(ra); if(ra==0) return(1);
            rn=rx;
            if(rx>7)
            {
                if(rn!=13)
                {
                    printf("<%s:%u> Error: Invalid base register\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
                if(newline[ra]==']')
                {
                    rx=0;
                }
                else
                {
                    ra=parse_comma(ra); if(ra==0) return(1);
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x3FC)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                }
                ra=parse_character(ra,']'); if(ra==0) return(1);
                //str rd,[sp,#immed_8*4]
                mem[curradd]=0x9000|(rd<<8)|(rx>>2);
                mark[curradd]=0x8000;
                curradd++;
            }
            else
            {
                if(newline[ra]==',')
                {
                    ra=parse_comma(ra); if(ra==0) return(1);
                    if(newline[ra]=='#')
                    {
                        //str rd,[rn,#immed_5*4]
                        ra=parse_character(ra,'#'); if(ra==0) return(1);
                        ra=parse_immed(ra); if(ra==0) return(1);
                        if((rx&0x7C)!=rx)
                        {
                            printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                            return(1);
                        }
                        ra=parse_character(ra,']'); if(ra==0) return(1);
                        mem[curradd]=0x6000|((rx>>2)<<6)|(rn<<3)|rd;
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                    else
                    {
                        //str rd,[rn,rm]
                        ra=parse_low_reg(ra); if(ra==0) return(1);
                        rm=rx;
                        ra=parse_character(ra,']'); if(ra==0) return(1);
                        mem[curradd]=0x5000|(rm<<6)|(rn<<3)|rd;
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                }
                else
                {
                    //str rd,[rn] immed_5 = 0
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x6000|(0<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// strb -----------------------------------------------------------
        if(strncmp(&newline[ra],"strb ",5)==0)
        {
            ra+=5;
            //strb rd,[rn,#immed_5]
            //strb rd,[rn,rm]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                if(newline[ra]=='#')
                {
                    //strb rd,[rn,#immed_5]
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x1F)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x7000|((rx>>0)<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //strb rd,[rn,rm]
                    ra=parse_low_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x5400|(rm<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                //strb rd,[rn] immed_5 = 0
                ra=parse_character(ra,']'); if(ra==0) return(1);
                mem[curradd]=0x7000|(0<<6)|(rn<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// strh -----------------------------------------------------------
        if(strncmp(&newline[ra],"strh ",5)==0)
        {
            ra+=5;
            //strh rd,[rn,#immed_5*2]
            //strh rd,[rn,rm]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                if(newline[ra]=='#')
                {
                    //strh rd,[rn,#immed_5*2]
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&(0x1F<<1))!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x8000|((rx>>1)<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //strh rd,[rn,rm]
                    ra=parse_low_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    ra=parse_character(ra,']'); if(ra==0) return(1);
                    mem[curradd]=0x5200|(rm<<6)|(rn<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                //strh rd,[rn] immed_5 = 0
                ra=parse_character(ra,']'); if(ra==0) return(1);
                mem[curradd]=0x8000|(0<<6)|(rn<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// sub -----------------------------------------------------------
        if(strncmp(&newline[ra],"sub ",4)==0)
        {
            ra+=4;
            //sub rd,rm,#immed_3
            //sub rd,#immed_8
            //sub rd,rn,rm
            //sub sp,#immed_7*4
            ra=parse_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(rd>7)
            {
                //sub rdhi,
                if((rd==13)&&(newline[ra]=='#'))
                {
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x1FC)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //sub sp,#immed_7*4
                    mem[curradd]=0xB080|(rx>>2);
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    printf("<%s:%u> Error: Invalid rd register\n",filename[currlevel],line[currlevel]);
                    return(1);
                }
            }
            else
            {
                //sub rdlo,
                if(newline[ra]=='#')
                {
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0xFF)!=rx)
                    {
                        printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                        return(1);
                    }
                    //sub rd,#immed_8
                    mem[curradd]=0x3800|(rd<<8)|rx;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //sub rdlo,r?
                    ra=parse_low_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    {
                        //sub rlo,rlo
                        ra=parse_comma(ra); if(ra==0) return(1);
                        if(newline[ra]=='#')
                        {
                            rn=rm;
                            ra=parse_character(ra,'#'); if(ra==0) return(1);
                            ra=parse_immed(ra); if(ra==0) return(1);
                            if((rx&0x7)!=rx)
                            {
                                printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                            //sub rd,rm,#immed_3
                            mem[curradd]=0x1E00|(rx<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                        else
                        {
                            //sub rd,rn,rm
                            rn=rm;
                            ra=parse_low_reg(ra); if(ra==0) return(1);
                            rm=rx;
                            mem[curradd]=0x1A00|(rm<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                    }
                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// swi -----------------------------------------------------------
        if(strncmp(&newline[ra],"swi ",4)==0)
        {
            ra+=4;
            //swi immed_8
            ra=parse_immed(ra); if(ra==0) return(1);
            if((rx&0xFF)!=rx)
            {
                printf("<%s:%u> Error: Invalid immediate\n",filename[currlevel],line[currlevel]);
                return(1);
            }
            mem[curradd]=0xDF00|rx;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// tst -----------------------------------------------------------
        if(strncmp(&newline[ra],"tst ",4)==0)
        {
            ra+=4;
            //tst rn,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4200|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
//-------------------------------------------------------------------
// pseudo instructions
//-------------------------------------------------------------------
// nop -----------------------------------------------------------
        if(strncmp(&newline[ra],"nop",3)==0)
        {
            ra+=3;
            if(rest_of_line(ra)) return(1);
            //nop, encode mov r8,r8
            mem[curradd]=0x46C0;
            mark[curradd]=0x8000;
            curradd++;
            continue;
        }


// -----------------------------------------------------------
        printf("<%s:%u> Error: syntax error\n",filename[currlevel],line[currlevel]);
        return(1);
    }

    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1111; mark[curradd]=0x8000; curradd++;


    //mem[curradd]=0xb5f0; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4657; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x464e; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4645; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0xb4e0; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x688b; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x680e; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x684d; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x68cf; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x491a; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x6804; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x6842; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x469c; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x46b1; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x46a8; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x2300; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x468a; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4e17; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x0115; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x199e; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x18b1; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x444d; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x404d; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x0951; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4441; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x404d; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1965; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x012c; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x0969; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4464; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x19c9; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x19ae; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4061; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4071; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4c0f; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x1851; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x191b; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x010c; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x185a; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x444c; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x094e; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4054; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4446; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4074; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x192c; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x0126; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x0962; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4466; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x19d2; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4072; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x18e5; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x406a; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x188a; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4553; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0xd1d9; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x6004; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x6042; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0xbc1c; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4690; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x4699; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x46a2; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0xbdf0; mark[curradd]=0x8000; curradd++;
    //mem[curradd]=0x3720; mark[curradd]=0x9000; curradd++;
    //mem[curradd]=0xc6ef; mark[curradd]=0x9000; curradd++;
    //mem[curradd]=0x79b9; mark[curradd]=0x9000; curradd++;
    //mem[curradd]=0x9e37; mark[curradd]=0x9000; curradd++;
    //mem[curradd]=0xf372; mark[curradd]=0x9000; curradd++;
    //mem[curradd]=0x3c6e; mark[curradd]=0x9000; curradd++;



















    return(0);
}
//-------------------------------------------------------------------
void dissassemble ( FILE *fp, unsigned int addr, unsigned short inst )
{
    if((inst&0xFFC0)==0x4140)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"adc r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFE00)==0x1C00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rx=(inst>>6)&7;
        fprintf(fp,"add r%u,r%u,#%u",rd,rn,rx);
        return;
    }
    if((inst&0xF800)==0x3000)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        fprintf(fp,"add r%u,#0x%02X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x1800)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"add r%u,r%u,r%u",rd,rn,rm);
        return;
    }
    if((inst&0xFF00)==0x4400)
    {
        rd=inst&7;
        rd|=(inst>>4)&8;
        rm=(inst>>3)&0xF;
        fprintf(fp,"add r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0xA000)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        rx<<=2;
        fprintf(fp,"add r%u,pc,#0x%03X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xF800)==0xA800)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        rx<<=2;
        fprintf(fp,"add r%u,sp,#0x%03X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xFF80)==0xB000)
    {
        rx=inst&0x7F;
        rx<<=2;
        fprintf(fp,"add sp,#0x%03X ; %u",rx,rx);
        return;
    }
    if((inst&0xFFC0)==0x4000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"and r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0x1000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F;
        if(rx==0) rx=32;
        fprintf(fp,"asr r%u,r%u,#%u",rd,rm,rx);
        return;
    }
    if((inst&0xFFC0)==0x4100)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"asr r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF000)==0xD000)
    {
        rx=inst&0xFF;
        rm=(inst>>8)&0xF;
        if(rx&0x80) rd=0xFFFFFFFF;
        else        rd=0x00000000;
        rd<<=9;
        rd|=rx<<1;
        rd+=addr+4;
        switch(rm)
        {
            case 0x0: fprintf(fp,"beq 0x%08X",rd); return;
            case 0x1: fprintf(fp,"bne 0x%08X",rd); return;
            case 0x2: fprintf(fp,"bcs 0x%08X",rd); return;
            case 0x3: fprintf(fp,"bcc 0x%08X",rd); return;
            case 0x4: fprintf(fp,"bmi 0x%08X",rd); return;
            case 0x5: fprintf(fp,"bpl 0x%08X",rd); return;
            case 0x6: fprintf(fp,"bvs 0x%08X",rd); return;
            case 0x7: fprintf(fp,"bvc 0x%08X",rd); return;
            case 0x8: fprintf(fp,"bhi 0x%08X",rd); return;
            case 0x9: fprintf(fp,"bls 0x%08X",rd); return;
            case 0xA: fprintf(fp,"bge 0x%08X",rd); return;
            case 0xB: fprintf(fp,"blt 0x%08X",rd); return;
            case 0xC: fprintf(fp,"bgt 0x%08X",rd); return;
            case 0xD: fprintf(fp,"ble 0x%08X",rd); return;
            case 0xE: fprintf(fp,"bal 0x%08X",rd); return;
            //case 0xF:
        }
    }
    if((inst&0xFFC0)==0x4380)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"bic r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFF00)==0xBE00)
    {
        rx=inst&0xFF;
        fprintf(fp,"bkpt 0x%02X ; %u",rx,rx);
        return;
    }
    if((inst&0xE000)==0xE000)
    {
        rx=inst&0x07FF;
        rm=(inst>>11)&0x3;
        switch(rm)
        {
            case 0:
            {
                if(rx&(1<<10)) rd=0xFFFFFFFF;
                else           rd=0x00000000;
                rd<<=12;
                rd|=rx<<1;
                rd+=addr+4;
                fprintf(fp,"b 0x%08X",rd);
                return;
            }
            case 2:
            {
                fprintf(fp,";bl upper");
                bl_upper=rx;
                return;
            }
            case 3:
            {
                if(bl_upper&(1<<10)) rd=0xFFFFFFFF;
                else                 rd=0x00000000;
                rd<<=11;
                rd|=bl_upper;
                rd<<=12;
                rd|=rx<<1;
                rd+=addr+(4-2);
                fprintf(fp,"bl 0x%08X",rd);
                return;
            }
        }
    }
    if((inst&0xFF87)==0x4700)
    {
        rm=(inst>>3)&0xF;
        fprintf(fp,"bx r%u",rm);
        return;
    }
    if((inst&0xFFC0)==0x42C0)
    {
        rn=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"cmn r%u,r%u",rn,rm);
        return;
    }
    if((inst&0xF800)==0x2800)
    {
        rx=inst&0xFF;
        rn=(inst>>8)&7;
        fprintf(fp,"cmp r%u,#0x%02X ; %u",rn,rx,rx);
        return;
    }
    if((inst&0xFFC0)==0x4280)
    {
        rn=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"cmp r%u,r%u",rn,rm);
        return;
    }
    if((inst&0xFF00)==0x4500)
    {
        rn=inst&7;
        rn|=(inst>>4)&8;
        rm=(inst>>3)&0xF;
        fprintf(fp,"cmp r%u,r%u",rn,rm);
        return;
    }
    if((inst&0xFFC0)==0x4040)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"eor r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0xC800)
    {
        rm=inst&0xFF;
        rn=(inst>>8)&7;
        fprintf(fp,"ldmia r%u!,{",rn);
        for(rx=0;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,"r%u",rx);
                break;
            }
        }
        for(rx++;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,",r%u",rx);
            }
        }
        fprintf(fp,"}");
        return;
    }
    if((inst&0xF800)==0x6800)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; rx<<=2;
        if(rx==0) fprintf(fp,"ldr r%u,[r%u]",rd,rm);
        else      fprintf(fp,"ldr r%u,[r%u,#0x%02X] ; %u",rd,rm,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x5800)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"ldr r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xF800)==0x4800)
    {
        rx=inst&0xFF; rx<<=2;
        rd=(inst>>8)&7;
        if(rx&0x200) rm=0xFFFFFFFF<<10; else rm=0;
        rm=addr+4+rx;
        if(rx==0) fprintf(fp,"ldr r%u,[pc] ; [0x%04X]",rd,rm);
        else      fprintf(fp,"ldr r%u,[pc,#0x%03X] ; %u [0x%04X]",rd,rx,rx,rm);
        return;
    }
    if((inst&0xF800)==0x9800)
    {
        rx=inst&0xFF; rx<<=2;
        rd=(inst>>8)&7;
        if(rx==0) fprintf(fp,"ldr r%u,[sp]",rd);
        else      fprintf(fp,"ldr r%u,[sp,#0x%03X] ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xF800)==0x7800)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; rx<<=0;
        if(rx==0) fprintf(fp,"ldrb r%u,[r%u]",rd,rm);
        else      fprintf(fp,"ldrb r%u,[r%u,#0x%02X] ; %u",rd,rm,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x5C00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"ldrb r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xF800)==0x8800)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; rx<<=1;
        if(rx==0) fprintf(fp,"ldrh r%u,[r%u]",rd,rm);
        else      fprintf(fp,"ldrh r%u,[r%u,#0x%02X] ; %u",rd,rm,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x5A00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"ldrh r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xFE00)==0x5600)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"ldrsb r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xFE00)==0x5E00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"ldrsh r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xF800)==0x0000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; if(rx==0) rx=32;
        fprintf(fp,"lsl r%u,r%u,#%u",rd,rm,rx);
        return;
    }
    if((inst&0xFFC0)==0x4080)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"lsl r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0x0800)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; if(rx==0) rx=32;
        fprintf(fp,"lsr r%u,r%u,#%u",rd,rm,rx);
        return;
    }
    if((inst&0xFFC0)==0x40C0)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"lsr r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0x2000)
    {
        rx=inst&0xFF;
        rd=(inst>>8)&7;
        fprintf(fp,"mov r%u,#0x%02X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xFFC0)==0x1C00)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"mov r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFF00)==0x4600)
    {
        rd=inst&7;
        rd|=(inst>>4)&8;
        rm=(inst>>3)&0xF;
        fprintf(fp,"mov r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFFC0)==0x4340)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"mul r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFFC0)==0x43C0)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"mvn r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFFC0)==0x4240)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"neg r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFFC0)==0x4300)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"orr r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFE00)==0xBC00)
    {
        rm=inst&0xFF;
        rn=inst&0x100;
        fprintf(fp,"pop {");
        for(rx=0;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,"r%u",rx);
                break;
            }
        }
        for(rx++;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,",r%u",rx);
            }
        }
        if(rn)
        {
            if(rm==0) fprintf(fp,"pc");
            else      fprintf(fp,",pc");
        }
        fprintf(fp,"}");
        return;
    }
    if((inst&0xFE00)==0xB400)
    {
        rm=inst&0xFF;
        rn=inst&0x100;
        fprintf(fp,"push {");
        for(rx=0;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,"r%u",rx);
                break;
            }
        }
        for(rx++;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,",r%u",rx);
            }
        }
        if(rn)
        {
            if(rm==0) fprintf(fp,"lr");
            else      fprintf(fp,",lr");
        }
        fprintf(fp,"}");
        return;
    }
    if((inst&0xFFC0)==0x41C0)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"ror r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFFC0)==0x4180)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"sbc r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0xC000)
    {
        rm=inst&0xFF;
        rn=(inst>>8)&7;
        fprintf(fp,"stmia r%u!,{",rn);
        for(rx=0;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,"r%u",rx);
                break;
            }
        }
        for(rx++;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                fprintf(fp,",r%u",rx);
            }
        }
        fprintf(fp,"}");
        return;
    }
    if((inst&0xF800)==0x6000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; rx<<=2;
        if(rx==0) fprintf(fp,"str r%u,[r%u]",rd,rm);
        else      fprintf(fp,"str r%u,[r%u,#0x%02X] ; %u",rd,rm,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x5000)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"str r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xF800)==0x9000)
    {
        rx=inst&0xFF; rx<<=2;
        rd=(inst>>8)&7;
        if(rx==0) fprintf(fp,"str r%u,[sp]",rd);
        else      fprintf(fp,"str r%u,[sp,#0x%03X] ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xF800)==0x7000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; rx<<=0;
        if(rx==0) fprintf(fp,"strb r%u,[r%u]",rd,rm);
        else      fprintf(fp,"strb r%u,[r%u,#0x%02X] ; %u",rd,rm,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x5400)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"strb r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xF800)==0x8000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F; rx<<=1;
        if(rx==0) fprintf(fp,"strh r%u,[r%u]",rd,rm);
        else      fprintf(fp,"strh r%u,[r%u,#0x%02X] ; %u",rd,rm,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x5200)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"strh r%u,[r%u,r%u]",rd,rn,rm);
        return;
    }
    if((inst&0xFE00)==0x1E00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rx=(inst>>6)&7;
        fprintf(fp,"sub r%u,r%u,#%u",rd,rn,rx);
        return;
    }
    if((inst&0xF800)==0x3800)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        fprintf(fp,"sub r%u,#0x%02X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x1A00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        fprintf(fp,"sub r%u,r%u,r%u",rd,rn,rm);
        return;
    }
    if((inst&0xFF80)==0xB080)
    {
        rx=inst&0x7F;
        rx<<=2;
        fprintf(fp,"sub sp,#0x%03X ; %u",rx,rx);
        return;
    }
    if((inst&0xFF00)==0xDF00)
    {
        rx=inst&0x7F;
        fprintf(fp,"swi 0x%02X ; %u",rx,rx);
        return;
    }
    if((inst&0xFFC0)==0x4200)
    {
        rn=inst&7;
        rm=(inst>>3)&7;
        fprintf(fp,"tst r%u,r%u",rn,rm);
        return;
    }






    fprintf(fp,"UNKNOWN");
}
//-------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    int ret;
    unsigned int ra;
    unsigned int rb;
    unsigned int inst;
    unsigned int inst2;

    if(argc!=2)
    {
        printf("tas filename\n");
        return(1);
    }

baseadd=0x8000;

    currlevel=0;
    curradd=0;
    nlabs=0;
    memset(mem,0x00,sizeof(mem));
    memset(mark,0x00,sizeof(mark));

    strcpy(filename[0],argv[1]);

    fpin[0]=fopen(argv[1],"rt");
    if(fpin[0]==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    line[0]=0;
    ret=assemble(0);
    fclose(fpin[0]);
    if(ret)
    {
        printf("failed %u\n",line[0]);
        return(1);
    }

    currlevel=0;

    for(ra=0;ra<nlabs;ra++)
    {
        printf("label%04u: [0x%08X] [%s] %u\n",ra,lab_struct[ra].addr,lab_struct[ra].name,lab_struct[ra].type);
        if(lab_struct[ra].type==1)
        {
            for(rb=0;rb<nlabs;rb++)
            {
                if(lab_struct[rb].type) continue;
                if(strcmp(lab_struct[ra].name,lab_struct[rb].name)==0)
                {
                    rx=lab_struct[rb].addr;
                    inst=mem[lab_struct[ra].addr];
                    line[currlevel]=lab_struct[ra].line;
                    strcpy(filename[currlevel],lab_struct[ra].fname);
                    if((inst&0xF800)==0x0000)
                    {
                        //no this really isnt an lsl, using this to
                        //mark an address
                        inst2=mem[lab_struct[ra].addr+1];
                        if((inst2&0xF800)!=0x0000)
                        {
                            printf("<%s:%u> Error: expecting a .word\n",filename[currlevel],line[currlevel]);
                            return(1);
                        }
                        rx+=baseadd;
                        inst=rx&0xFFFF;
                        inst2=(rx>>16)&0xFFFF;
                        mem[lab_struct[ra].addr+1]=inst2;
                        lab_struct[ra].type++;
                    }
                    if((inst&0xF800)==0x4800)
                    {
                        //ldr rd,[pc,#immed_8]
                        if(lab_struct[ra].addr&1)
                        {
                            rd=rx-((lab_struct[ra].addr<<1)+2);
                        }
                        else
                        {
                            rd=rx-((lab_struct[ra].addr<<1)+4);
                        }
                        rm=0xFFFFFFFF;
                        rm<<=8; //I think this is wrong by two or four
                        if(rd&rm)
                        {
                            if((rd&rm)!=rm)
                            {
                                printf("<%s:%u> Error: Load destination too far\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                        }
                        inst|=(rd>>2)&0x7FF;
                        lab_struct[ra].type++;
                    }
                    if((inst&0xF000)==0xD000)
                    {
                        //bcond signed_immed_8
                        rd=rx-((lab_struct[ra].addr<<1)+4);
                        rm=0xFFFFFFFF;
                        rm<<=8;
                        if(rd&rm)
                        {
                            if((rd&rm)!=rm)
                            {
                                printf("<%s:%u> Error: Branch destination too far\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                        }
                        inst|=(rd>>1)&0xFF;
                        lab_struct[ra].type++;
                    }
                    if((inst&0xF000)==0xE000)
                    {
                        //b signed_immed_11
                        rd=rx-((lab_struct[ra].addr<<1)+4);
                        rm=0xFFFFFFFF;
                        rm<<=11;
                        if(rd&rm)
                        {
                            if((rd&rm)!=rm)
                            {
                                printf("<%s:%u> Error: Branch destination too far\n",filename[currlevel],line[currlevel]);
                                return(1);
                            }
                        }
                        inst|=(rd>>1)&0x7FF;
                        lab_struct[ra].type++;
                    }
                    if((inst&0xF800)==0xF000)
                    {
                        inst2=mem[lab_struct[ra].addr+1];
                        if((inst2&0xF800)!=0xF800)
                        {
                            printf("<%s:%u> Error: bl should be a pair of instructions (internal error)\n",filename[currlevel],line[currlevel]);
                            return(1);
                        }
                        rd=rx-((lab_struct[ra].addr<<1)+4);
                        rm=0xFFFFFFFF;
                        rm<<=22;
                        if(rd&rm)
                        {
                            if((rd&rm)!=rm)
                            {
                                printf("<%s:%u> Error: Branch destination too far\n",filename[currlevel],line[currlevel]);
                            }
                        }
                        inst|=(rd>>12)&0x7FF;
                        inst2|=(rd>>1)&0x7FF;
                        mem[lab_struct[ra].addr+1]=inst2;
                        lab_struct[ra].type++;
                    }
                    if(lab_struct[ra].type==1)
                    {
                        printf("<%s:%u> Error: internal error, unknown instruction 0x%08X\n",filename[currlevel],line[currlevel],inst);
                        return(1);
                    }
                    mem[lab_struct[ra].addr]=inst;
                    break;
                }
            }
            if(rb<nlabs) ; else
            {
                printf("<%s:%u> Error: unresolved label\n",filename[currlevel],line[currlevel]);
                return(1);
            }
        }
    }

    sprintf(newline,"%s.hex",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }

    for(ra=0;ra<=ADDMASK;ra++)
    {
        curradd=(ra<<1);
        if(mark[ra]&0x8000)
        {
            printf("0x%08X: 0x%04X ",curradd,mem[ra]);
            if(mark[ra]&0x1000)
            {
                fprintf(stdout,"data");
            }
            else
            {
                dissassemble(stdout,curradd,mem[ra]);
            }
            printf("\n");

            //rb=0x04;
            //rb+=(curradd>>8)&0xFF;
            //rb+=(curradd>>0)&0xFF;
            //rb+=0x00;
            //rb+=(mem[ra]>>24)&0xFF;
            //rb+=(mem[ra]>>16)&0xFF;
            //rb+=(mem[ra]>> 8)&0xFF;
            //rb+=(mem[ra]>> 0)&0xFF;
            //rb=(-rb)&0xFF;
            //fprintf(fpout,":%02X%04X%02X%08X%02X\n",0x04,curradd&0xFFFF,0x00,mem[ra],rb);
        }
    }

    fclose(fpout);


    sprintf(newline,"%s.s",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }
    for(ra=0;ra<=ADDMASK;ra++)
    {
        curradd=(ra<<1);
        if(mark[ra]&0x8000)
        {
            //fprintf(fpout,"0x%08X: 0x%04X ",curradd,mem[ra]);
            if(mark[ra]&0x1000)
            {
                fprintf(fpout,"data");
            }
            else
            {
                dissassemble(fpout,curradd,mem[ra]);
            }
            fprintf(fpout,"\n");
        }
    }
    fclose(fpout);

    sprintf(newline,"%s.diss",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }
    for(ra=0;ra<=ADDMASK;ra++)
    {
        curradd=(ra<<1);
        if(mark[ra]&0x8000)
        {
            fprintf(fpout,"0x%08X: 0x%04X ",curradd,mem[ra]);
            if(mark[ra]&0x1000)
            {
                fprintf(fpout,"data");
            }
            else
            {
                dissassemble(fpout,curradd,mem[ra]);
            }
            fprintf(fpout,"\n");
        }
    }
    fclose(fpout);

    rb=0;
    for(ra=0;ra<=ADDMASK;ra++)
    {
        if(mark[ra]&0x8000) rb=ra;
    }
    sprintf(newline,"%s.bin",argv[1]);
    fpout=fopen(newline,"wb");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }
    fwrite(mem,2,rb+1,fpout);
    fclose(fpout);

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


