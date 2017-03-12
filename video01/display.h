/* display.c */
void clrScreen(int color);
void setPixel(int x, int y, int color);
void drawLine(int x0, int y0, int x1, int y1, unsigned int color);
void drawString(char str[], int x, int y);
void delChar(int x, int y);
void drawChar(char c, int x, int y);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);
extern int multiplylookup[640];
