
//-----------------------------------------------------------------------------
// Copyright (c) David Welch 1996, 2012
//-----------------------------------------------------------------------------

unsigned char ser_open ( char *devname );
void strobedtr ( void );
void ser_close ( void );
void ser_senddata ( unsigned char *, unsigned short );
void ser_sendstring ( char *s );
void ser_update ( void );
unsigned short ser_copystring ( unsigned char * );
unsigned short ser_dump ( unsigned short );

//-----------------------------------------------------------------------------
// Copyright (c) David Welch 1996, 2012
//-----------------------------------------------------------------------------

