# include <conio.h>
# include <string.h>
# include <fstream.h>
# include <stdio.h>
# include <dos.h>
# include <graphics.h>
# include <process.h>
# include <time.h>
# include <stdlib.h>

class movedisplay;
int finished=0;
class mouse;
int plflag=1;
class validation;
class ai;
class graphics;

int xz1=57,yz1=9,moves;
int array[64];
int defeated[30];
int wkm=0;
int wrl=0;
int wrr=0;
int bkm=0;
int brl=0;
int brr=0;
int defeatedcount=0;
int whitedef=0;
int blackdef=0;
char imagepath[13]="C:\\project\\";
char path[13];

getxy(int x,int y)
{
int num=0;
num=y*8;
num=num+x;
return num;
}

getx(int xy)
{
while(xy>7)
xy=xy-8;
return xy;
}

gety(float xy)
{
int x=xy/8;

if(xy/8<x)
x--;
return x;
}

# define SUCCESS 1
# define FAIL 0

# define WROOK 1
# define WHORSE 2
# define WBISHOP 3
# define WQUEEN 4
# define WKING 5
# define WPAWN 6

# define BROOK 11
# define BHORSE 12
# define BBISHOP 13
# define BQUEEN 14
# define BKING 15
# define BPAWN 16

//Mouse Class starting//
//------------------------------------------------------------------------------------------------------//
int graphicsdriver,gm,maxx,maxy,graphicsx,graphicsy,button;
static union REGS inregs, outregs;
static struct SREGS segregs;
typedef enum {ARROW, HAND, HOUR_GLASS, PEN} cursor_type;
union REGS graphicsi,graphicso;

static const int cursor_data[] = {
	/* standard arrow cursor */
	1, 0,                            /* x, graphicsy of hotpoint                    */

	0x9FFF, 0x8FFF, 0x87FF, 0x83FF,  /* SCREEN MASK (16 words)              */
	0x81FF, 0x80FF, 0x807F, 0x803F,  /* Each word marks 16 pixels (lowest   */
	0x801F, 0x800F, 0x80FF, 0x887F,  /* bit is rightmost pixel). The mask   */
	0x987F, 0xBC3F, 0xFC3F, 0xFE7F,  /* is AND_ed with screen contents.     */

	0x0000, 0x2000, 0x3000, 0x3800,  /* CURSOR MASK (16 words)              */
	0x3C00, 0x3E00, 0x3F00, 0x3F80,  /* After the screen is AND_ed with the */
	0x3FC0, 0x3E00, 0x3600, 0x2300,  /* screen mask it is XOR_ed with this  */
	0x0300, 0x0180, 0x0180, 0x0000,  /* cursor mask.                        */

	/* pointing hand cursor */
	2, 1,
	0xCFFF, 0x87FF, 0x87FF, 0x87FF, 0x813F, 0x8007, 0x8003, 0x8003,
	0x8003, 0x8003, 0x8003, 0x8003, 0x8007, 0xC007, 0xC007, 0xE03F,

	0x0000, 0x3000, 0x3000, 0x3000, 0x3000, 0x36C0, 0x36D8, 0x36D8,
	0x36D8, 0x36D8, 0x3FF8, 0x3FF8, 0x3FF0, 0x1FF0, 0x1FC0, 0x0000,

	/* hourglass cursor */
	7, 7,
	0xC003, 0xC003, 0xC003, 0xC003, 0xE007, 0xF00F, 0xF81F, 0xFC3F,
	0xFC3F, 0xF81F, 0xF00F, 0xE007, 0xC003, 0xC003, 0xC003, 0xC003,

	0x0000, 0x1FF8, 0x0000, 0x1FF8, 0x0D50, 0x06A0, 0x0340, 0x0180,
	0x0180, 0x03C0, 0x07E0, 0x0D50, 0x1AA8, 0x0000, 0x1FF8, 0x0000,

	/* pen cursor */
	0, 15,
	0xFFF1, 0xFFE0, 0xFFC0, 0xFF80, 0xFF01, 0xFE03, 0xFC07, 0xF80F,
	0xF01F, 0xE03F, 0xE07F, 0xC0FF, 0xC1FF, 0x87FF, 0x1FFF, 0x3FFF,

	0x0000, 0x000E, 0x001C, 0x003A, 0x0074, 0x00E8, 0x01D0, 0x03A0,
	0x0740, 0x0E80, 0x0D00, 0x1A00, 0x1800, 0x2000, 0x4000, 0x0000
};

static class mouse{
		protected:

		public:
		  mouse();
		  initmouse();
		  showmouseptr();
		  getpoint();
		  restrictmouseptr(int,int,int,int);
		  getmousepos(int*,int*,int*);
		  void setCursor(cursor_type cursor);
		  void disableMouse();
		};

mouse::mouse()
{
setviewport(1,57,maxx-1,maxy-1,1);
restrictmouseptr(1,1,maxx-1,maxy-1);
}

mouse::getpoint()
{
int x,y,button;
getmousepos(&button,&x,&y);
if(x>30 && x<430 && y>70 && y<470)
 {
 int num=0;
 x=x-30;
 y=y-70;
 x=x/50;
 y=y/50;
 num=y*8;
 num=num+x;
 return num;
 }
return -1;
}

mouse::initmouse()
{
graphicsi.x.ax=0;
int86(0x33,&graphicsi,&graphicso);
return(graphicso.x.ax);
}

mouse::showmouseptr()
{
graphicsi.x.ax=1;
int86(0x33,&graphicsi,&graphicso);
return 0;
}

mouse::restrictmouseptr(int x1,int y1,int x2,int y2)
{
graphicsi.x.ax=7;
graphicsi.x.cx=x1;
graphicsi.x.dx=x2;
int86(0x33,&graphicsi,&graphicso);

graphicsi.x.ax=8;
graphicsi.x.cx=y1;
graphicsi.x.dx=y2;
int86(0x33,&graphicsi,&graphicso);
return 0;
}

mouse::getmousepos(int *button,int *x, int *graphicsy)
{
graphicsi.x.ax=3;
int86(0x33,&graphicsi,&graphicso);
*button=graphicso.x.bx;
*x=graphicso.x.cx;
*graphicsy=graphicso.x.dx;
return 0;
}


void mouse::setCursor(cursor_type cursor)  /* types defined in header */
{
inregs.x.ax = 9;
inregs.x.bx = *(cursor_data + 34 * cursor);      /* x hotpoint */
inregs.x.cx = *(cursor_data + 34 * cursor + 1);    /* graphicsy hotpoint */
inregs.x.dx = FP_OFF(cursor_data + 34 * cursor + 2); /* pointer to */
segregs.es = FP_SEG(cursor_data + 34 * cursor + 2);  /* the masks  */
int86x(0x33, &inregs, &outregs, &segregs);
}

void mouse::disableMouse()
{
inregs.x.ax = 2;
int86(0x33,&inregs,&outregs);
}

//------------------------------------------------------------------------------------------------------//
//Mouse Class End//



//Graphic class starting
//------------------------------------------------------------------------------------------------------//


struct palettetype pal;

void Queen(int sx,int sy)
{
//fstream vas;
//vas.open("C:\\project\\data.dat",ios::app);

strcpy(path,imagepath);
char a;
ifstream ifil;
ifil.open(strcat(path,"data.dat"), ios::in);

ifil.seekg(96400);
for(int i=0;i<200;i++)
  for(int j=0;j<100;j++)
     {
	ifil.get(a);
  //	vas.put(a);


	int g=a,k;
	if(a<0)g+=256;
	k=g/16;
	setcolor(k);
	if(a!=0)putpixel(sx+j,sy+200-i,k);
     }
//     vas.close();
ifil.close();
}

void Horse(int sx,int sy)
{
//fstream vas;
//vas.open("C:\\project\\data.dat",ios::app);

strcpy(path,imagepath);
char a;
ifstream ifil;
ifil.open(strcat(path,"data.dat"), ios::in);

ifil.seekg(116400);

for(int i=0;i<132;i++)
 for(int j=0;j<80;j++)
  {
   ifil.get(a);
// vas.put(a);
   int g=a,k;
   if(a<0)g+=256;
   k=g/16;
   setcolor(k);
   if(a!=0)
   putpixel(sx+j,sy+200-i,k);
  }
ifil.close();
//vas.close();
}


void Title(int sx,int sy)
{
//fstream vas;
//vas.open("C:\\project\\data.dat",ios::app);
strcpy(path,imagepath);
char a;
ifstream ifil;
ifil.open(strcat(path,"data.dat"), ios::in);
ifil.seekg(14400);

for(int i=0;i<205;i++)
  for(int j=0;j<400;j++)
    {
     ifil.get(a);
  // vas.put(a);
     int g=a,k;
     if(a<0)g+=256;
     k=15;
     k=g/16;
     setcolor(k);
     if(a!=0)putpixel(sx+j,sy+200-i,k);
    }
ifil.close();
//vas.close();
}

class graphics{
	protected:
	  pawnimage(int num,char); // function to place char image on num block
	  mouse m1;		 // m1 is an object of class m1
	  int button,x,y;		// variable to hold mouse pos and mouse button clicked
	  char ar1[60][40];	// arrays to hold image into memory
	  char ar2[60][40];
	  char ar3[60][40];
	  char ar4[60][40];
	  char ar5[60][40];
	  char ar6[60][40];
	  int r1,r2,r3,r4,r5,r6,vind;	// flag to decide if the image is in file or memory

	public:
	  graphics();	// constructor to initilize variables
	  draw(int,int);	// to draw block
	  void inig();	// function ti initilize graphics mode
	  drawblock(int);	// draws block of int number
	  drawborder(int,int);	// draws border of block int,int
	  drawboard();	// draws the chess board (entire board)
	  startscreen();	// sets the pawns to start position
	  authorinfo(int,int);	// function that displays author details at the begining
	  move(int,int);	// function to move pawn from int block to int block
	  char returncolor(int);	// function returns the colour of the pawn in int cell
	  defeatedimage(int,int); // puts the pawn in int,int to the defeated place
	  void startpawns();      // sets the array to initilize position of board
	  void boardnumbering();	// draws the board numbers along its sides

	  void wr(int num){strcpy(path,imagepath);vind=1;pawnimage(num,'w'); }
	  void wh(int num){strcpy(path,imagepath);vind=2;pawnimage(num,'w');}
	  void wb(int num){strcpy(path,imagepath);vind=3;pawnimage(num,'w');}
	  void wq(int num){strcpy(path,imagepath);vind=4;pawnimage(num,'w');}
	  void wk(int num){strcpy(path,imagepath);vind=5;pawnimage(num,'w');}
	  void wp(int num){strcpy(path,imagepath);vind=6;pawnimage(num,'w');}

	  void br(int num){strcpy(path,imagepath);vind=1;pawnimage(num,'b');}
	  void bh(int num){strcpy(path,imagepath);vind=2;pawnimage(num,'b');}
	  void bb(int num){strcpy(path,imagepath);vind=3;pawnimage(num,'b');}
	  void bq(int num){strcpy(path,imagepath);vind=4;pawnimage(num,'b');}
	  void bk(int num){strcpy(path,imagepath);vind=5;pawnimage(num,'b');}
	  void bp(int num){strcpy(path,imagepath);vind=6;pawnimage(num,'b');}
	  drawdefeated(); //will draw the defeated block spacing
	  deletedefeated(int);    // removes pawn from deleted if undo is used
	  cursor(); // function to change the cursor to haand when it is over a pawn

	  getblockcolor(int);	// will return the color of the block
	  drawborder(int);	// will draw orange border for int selected cell
	  deleteborder(int);	// removes the orange border created by the above function
	  writefile();		// not used but used to create the data.dat file
	      };

graphics g;
mouse m;

graphics::writefile()
{
fstream vasanth;
vasanth.open("data.dat",ios::out);
for(int i=0;i<60;i++)
{
 for(int j=0;j<40;j++)
 {
  vasanth.put(ar1[i][j]);
 }
}

for(i=0;i<60;i++)
{
 for(int j=0;j<40;j++)
 {
  vasanth.put(ar2[i][j]);
 }
}

for(i=0;i<60;i++)
{
 for(int j=0;j<40;j++)
 {
  vasanth.put(ar3[i][j]);
 }
}

for( i=0;i<60;i++)
{
 for(int j=0;j<40;j++)
 {
  vasanth.put(ar4[i][j]);
 }
}

for(i=0;i<60;i++)
{
 for(int j=0;j<40;j++)
 {
  vasanth.put(ar5[i][j]);
 }
}

for(i=0;i<60;i++)
{
 for(int j=0;j<40;j++)
 {
  vasanth.put(ar6[i][j]);
 }
}

vasanth.close();
return 0;
}

graphics::deletedefeated(int del)
{
if(defeatedcount<0)
defeatedcount=0;

int sx,sy;
sx=435;
sy=70;

if(del==1)
{
defeatedcount--;
int num=whitedef;
if(num>8)
 {
  sy=sy+25;
  num=num-8;
 }

while(num>8)
num--;

sx=sx-25;
sx=sx+(25*num);

setfillstyle(1,7);
bar(sx+1,sy+2,sx+20,sy+22);
whitedef--;

if(defeatedcount<0)
defeatedcount=0;
}

if(del==15)
{
defeatedcount--;
sy=420;
int num=blackdef;
if(num>8)
 {
  sy=sy+25;
  num=num-8;
 }

while(num>8)
num--;

sx=sx-25;
sx=sx+(25*num);

setfillstyle(1,7);
bar(sx+1,sy+2,sx+20,sy+22);
blackdef--;
}

if(defeatedcount<0)
defeatedcount=-1;

return 0;
}

graphics::draw(int pwn,int pos)
{
if(pwn==0)
 {
  drawblock(pos);
 }

if(pwn==WKING)
wk(pos);

if(pwn==BKING)
bk(pos);

if(pwn==WQUEEN)
wq(pos);

if(pwn==BQUEEN)
bq(pos);

if(pwn==WBISHOP)
wb(pos);

if(pwn==BBISHOP)
bb(pos);

if(pwn==WHORSE)
wh(pos);

if(pwn==BHORSE)
bh(pos);

if(pwn==WROOK)
wr(pos);

if(pwn==BROOK)
br(pos);

if(pwn==WPAWN)
wp(pos);

if(pwn==BPAWN)
bp(pos);

return 0;
}

graphics::pawnimage(int num,char ch)
{
int sx=35;
int sy=-80;
int count=0;
while(num>7)
 {
 num=num-8;
 count++;
 }

sy=sy+(count*50);
sx=sx+(num*50);

char a;
int flag2=0;
if(vind==1 && r1==1)
 {
  flag2=1;
 }
if(vind==2 && r2==1)
 {
  flag2=1;
 }
if(vind==3 && r3==1)
 {
  flag2=1;
 }
if(vind==4 && r4==1)
 {
  flag2=1;
 }
if(vind==5 && r5==1)
 {
  flag2=1;
 }
if(vind==6 && r6==1)
 {
  flag2=1;
 }

ifstream ifil;
if(flag2==0)
 {
  ifil.open(strcat(path,"data.dat"),ios::in);
  ifil.seekg((vind-1)*60*40);
 }

for(int i=0;i<60;i++)
  for(int j=0;j<40;j++)
    {
     if(flag2==0)
      {
       ifil.get(a);
	if(vind==1)
	{
	ar1[i][j]=a;
	}
	if(vind==2)
	{
	ar2[i][j]=a;
	}
	if(vind==3)
	{
	ar3[i][j]=a;
	}
	if(vind==4)
	{
	ar4[i][j]=a;
	}
	if(vind==5)
	{
	ar5[i][j]=a;
	}
	if(vind==6)
	{
	ar6[i][j]=a;
	}
      }
      else
      {
       if(vind==1)
	{
	 a=ar1[i][j];
	}
       if(vind==2)
	{
	a=ar2[i][j];
	}
       if(vind==3)
	{
	a=ar3[i][j];
	}
       if(vind==4)
	{
	a=ar4[i][j];
	}
       if(vind==5)
	{
	a=ar5[i][j];
	}
       if(vind==6)
	{
	a=ar6[i][j];
	}
      }

    int g=a,k;
    if(a<0)g+=256;
    k=g/16;
    setcolor(k);

    while(k!=WHITE && k!=0 )
     {
      k=k-1;
     }
    if(ch=='w')
     {
      if(k==WHITE)
	k=WHITE;
      else
	k=8;
     }

     if(a!=0)
     putpixel(sx+j,sy+200-i,k);
    }

if(flag2==0)
 {
  ifil.close();
 }

if(vind==1)
 {
  r1=1;
 }
if(vind==2)
 {
  r2=1;
 }
if(vind==3)
 {
  r3=1;
 }
if(vind==4)
 {
  r4=1;
 }
if(vind==5)
 {
  r5=1;
 }
if(vind==6)
 {
  r6=1;
 }

vind=0;
return 0;
}

char graphics::returncolor(int num)
{
if(array[num]>0 && array[num]<10)
return 'w';

if(array[num]>10 && array[num]<20)
return 'b';

return 'n';
}

graphics::graphics()
{
inig();
for(int i=0;i<64;i++)
  array[i]=0;

r1=0,r2=0,r3=0,r4=0,r5=0,r6=0;
vind=0;
}

void graphics::inig()
{
int gdriver = DETECT, gmode, errorcode;
initgraph(&gdriver, &gmode, "");
}

graphics::drawblock(int num)
{
int x,y;
x=30;
y=70;
int count=0;
while(num>7)
 {
 num=num-8;
 count++;
 }

if((num%2==0 && count%2==0) || (num%2!=0 && count%2!=0))
setfillstyle(1,WHITE);
else
setfillstyle(1,7);

y=y+(count*50);
x=x+(num*50);

bar(x,y,x+50,y+50);
setcolor(BLACK);
rectangle(x,y,x+50,y+50);

return 0;
}

graphics::drawborder(int num,int color)
{
if(num<0 || num>63)
return 0;

int x,y;
x=30;
y=70;
int count=0;
while(num>7)
 {
 num=num-8;
 count++;
 }

y=y+(count*50);
x=x+(num*50);
setcolor(color);
for(int i=2;i<5;i++)
rectangle(x+i,y+i,x+50-i,y+50-i);

return 0;
}

graphics::drawboard()
{
for(int i=0;i<64;i++)
  drawblock(i);
return 0;
}

graphics::startscreen()
{
getpalette(&pal);
for(int i=0;i<pal.size;i++)
setrgbpalette(pal.colors[i], i*4, i*4, i*3);
Title(105,120);
Queen(65,100);
Horse(450,80);
m1.initmouse();
m1.setCursor(ARROW);
m1.showmouseptr();
m1.getmousepos(&button,&x,&y);
int flag=0;
while(button!=1 || flag==0)
 {
  if(kbhit())
   {
    char c=getch();
    if(c==27)
     {
      closegraph();
      // g.writefile();
      moves=0;
      fstream store;
      store.open("C:\\project\\store.dat",ios::out);
      store.close();

      exit(0);
     }
   }
  m1.getmousepos(&button,&x,&y);
  if(x>246 && x<341 && y>289 && y<309) // Enter info
   {
    m1.setCursor(HAND);
    settextstyle(7,0,4);
    setcolor(WHITE);
    outtextxy(170,350,"Enter the Game>>>");
    if(button==1)
      flag=1;
   }
  else
    settextstyle(7,0,4);

  setcolor(BLACK);
  outtextxy(170,350,"Enter the Game>>>");

  authorinfo(x,y);
  m1.setCursor(ARROW);
  m1.showmouseptr();
 }

closegraph();
inig();
return 0;
}

void graphics::startpawns()
{
for(int i=0;i<8;i++)
 {
  wp(48+i);
  array[48+i]=WPAWN;
  bp(8+i);
  array[8+i]=BPAWN;
 }

br(0);
array[0]=BROOK;
br(7);
array[7]=BROOK;
wr(63);
array[63]=WROOK;
wr(56);
array[56]=WROOK;
wh(62);
array[62]=WHORSE;
wh(57);
array[57]=WHORSE;
bh(1);
array[1]=BHORSE;
bh(6);
array[6]=BHORSE;
wb(61);
array[61]=WBISHOP;
wb(58);
array[58]=WBISHOP;
bb(2);
array[2]=BBISHOP;
bb(5);
array[5]=BBISHOP;
wk(60);
array[60]=WKING;
bk(4);
array[4]=BKING;
wq(59);
array[59]=WQUEEN;
bq(3);
array[3]=BQUEEN;
}

void graphics::boardnumbering()
{
settextstyle(2,0,5);
setcolor(WHITE);
int nu=85;
outtextxy(10,nu,"1");
outtextxy(10,nu=nu+50,"2");
outtextxy(10,nu=nu+50,"3");
outtextxy(10,nu=nu+50,"4");
outtextxy(10,nu=nu+50,"5");
outtextxy(10,nu=nu+50,"6");
outtextxy(10,nu=nu+50,"7");
outtextxy(10,nu=nu+50,"8");
nu=50;
outtextxy(nu,50,"A");
outtextxy(nu=nu+50,50,"B");
outtextxy(nu=nu+50,50,"C");
outtextxy(nu=nu+50,50,"D");
outtextxy(nu=nu+50,50,"E");
outtextxy(nu=nu+50,50,"F");
outtextxy(nu=nu+50,50,"G");
outtextxy(nu=nu+50,50,"H");
}

graphics::move(int start,int end)
{
if(array[start]!=0 && start<64 && end<64 && start>-1 && end>-1 && ((array[start]<10 && (array[end]>10)||array[end]==0)|| (array[start]>10 && (array[end]<10||array[end]==0))))
 {
  int p=array[start];
  array[end]=array[start];
  array[start]=0;
  drawblock(start);
  drawblock(end);

  if(p==WROOK)
    wr(end);
  if(p==WHORSE)
    wh(end);
  if(p==WBISHOP)
    wb(end);
  if(p==WKING)
    wk(end);
  if(p==WQUEEN)
    wq(end);
  if(p==WPAWN)
    wp(end);
  if(p==BROOK)
    br(end);
  if(p==BHORSE)
    bh(end);
  if(p==BBISHOP)
    bb(end);
  if(p==BKING)
    bk(end);
  if(p==BQUEEN)
    bq(end);
  if(p==BPAWN)
    bp(end);

  return SUCCESS;
 }
return FAIL;
}

graphics::authorinfo(int x,int y)
{
gotoxy(14,22);
if(x>226 && x<291 && y>188 && y<276) // Vas info
 {
  m1.setCursor(HOUR_GLASS);
  cout<<"Vasanth. Incharge of Game Logics			";
 }
else
 if(x>299 && x<368 && y>188 && y<276) // Aks info
  {
   m1.setCursor(HOUR_GLASS);
   cout<<"Akhil. Incharge of Game Animation		";
  }
 else
  if(x>372 && x<438 && y>188 && y<276) // Vind info
   {
    m1.setCursor(HOUR_GLASS);
    cout<<"Arvind. Incharge of Game Interface		";
   }
 else
  if(x>154 && x<221 && y>188 && y<277) // Shef info
   {
    m1.setCursor(HOUR_GLASS);
    cout<<"Shefiu. Incharge of Game Special Algorithms		";
   }
 else
  {
   m1.setCursor(ARROW);
   gotoxy(14,22);
   cout<<"CHESS GAME IMPLEMENTATION by II HND TEAM	";
  }
return 0;
}

graphics::defeatedimage(int pat,int num)
{
char ch;
if(pat<10)
ch='w';
if(pat>10)
ch='b';

char a;
ifstream ifil;
ifil.open(strcat(path,"data.dat"), ios::in);

char address[10]={NULL};

if(pat==WKING || pat==BKING)
ifil.seekg(4*60*40);

if(pat==WQUEEN || pat==BQUEEN)
ifil.seekg(3*60*40);

if(pat==WBISHOP || pat==BBISHOP)
ifil.seekg(2*60*40);

if(pat==WHORSE || pat==BHORSE)
ifil.seekg(60*40);

if(pat==WROOK || pat==BROOK)
ifil.seekg(0);

if(pat==WPAWN || pat==BPAWN)
ifil.seekg(5*60*40);

char temp[13];
strcpy(temp,imagepath);
strcat(temp,address);

int sx=435;
int sy=-105;

if(ch=='w')
 {
  if(num>7)
   {
    sy=sy+25;
    num=num-8;
   }

  while(num>7)
  num--;
  sx=sx+(25*num);
 }

if(ch=='b')
 {
  sy=245;
  if(num>7)
   {
    sy=sy+25;
    num=num-8;
   }

  while(num>7)
  num--;
  sx=sx+(25*num);
 }

for(int i=0;i<60;i++)
  for(int j=0;j<40;j++)
    {
     ifil.get(a);
     int g=a,k;
     if(a<0)g+=256;
     k=g/16;
     setcolor(k);

     while(k!=WHITE && k!=0 )
      {
       k=k-1;
      }
     if(ch=='w')
      {
       if(k==WHITE)
	k=WHITE;
       else
	k=8;
      }

     if(a!=0)
     putpixel(sx+j/2,sy+200-i/2,k);
    }
ifil.close();
return 0;
}

graphics::drawdefeated()
{
setcolor(BROWN);
int x,y;
x=435;
y=70;
setfillstyle(1,7);
bar(x,y,x+200,y+50);
line(x,y+25,x+200,y+25);

rectangle(x,y,x+200,y+50);

x=435;
y=70+(50*6);
y=y+50;
setfillstyle(1,7);
bar(x,y,x+200,y+50);
rectangle(x,y,x+200,y+50);
line(x,y+25,x+200,y+25);

rectangle(435,125,x+200,y-5);
rectangle(435+2,125+2,x+200-2,y-5-2);
rectangle(x+2,y-30,x+200-2,y-5-2);
rectangle(x+3,y-30+1,x+50,y-5-3);
outtextxy(x+10,y-25,"UNDO");
rectangle(x+52,y-30+1,x+100,y-5-3);
outtextxy(x+60,y-25,"SAVE");

rectangle(x+102,y-30+1,x+152,y-5-3);
outtextxy(x+110,y-25,"LOAD");

return 0;
}

graphics::cursor()
{
if(array[m1.getpoint()]!=0 && m1.getpoint()!=-1)
m1.setCursor(HAND);
else
m1.setCursor(ARROW);

return 0;
}

graphics::getblockcolor(int num)
{
int sx,sy;
sx=getx(num);
sy=gety(num);

if((sx%2==0 && sy%2==0) ||(sx%2!=0 && sy%2!=0))
  return WKING;

return BKING;
}

graphics::drawborder(int num)
{
if(num<0 || num>63)
  return 0;

int x,y;
x=30;
y=70;
int count=0;
while(num>7)
 {
  num=num-8;
  count++;
 }

y=y+(count*50);
x=x+(num*50);

setcolor(RED);
for(int i=1;i<4;i++)
 rectangle(x+i,y+i,x+50-i,y+50-i);

return 0;
}

graphics::deleteborder(int num)
{
if(num<0 || num>63)
  return 0;

int nu=num;
int x,y;
x=30;
y=70;
int count=0;
while(num>7)
 {
 num=num-8;
 count++;
 }

y=y+(count*50);
x=x+(num*50);

int col=getblockcolor(nu);

if(col==WKING)
 setcolor(WHITE);
else
 setcolor(7);

for(int i=1;i<5;i++)
 rectangle(x+i,y+i,x+50-i,y+50-i);

return 0;
}

//Graphic class Ending
//------------------------------------------------------------------------------------------------------//

//validation class Starting
//------------------------------------------------------------------------------------------------------//
class validation{
	protected:
	  char TURN;
	public:
	  changeturn();
	  char getturn();
	  validation();
	  validate(int,int);
	  validaterook(int,int);
	  validatebishop(int,int);
	  resetboard();
	  validatequeen(int,int);
	  validatehorse(int,int);
	  validatewpawn(int,int);
	  validatebpawn(int,int);
	  validateking(int,int);
	  isrisk(int);
	  dummyvalidate(int,int);
	  willitrisk(int,int);
	  getkingpos(int);
	  checkmate(int);
		};

validation v1;

validation::validation()
{
TURN='w';
}

validation::resetboard()
{
fstream store;
store.open("C:\\project\\store.dat",ios::out);
store.close();
moves=0;

for(int i=0;i<64;i++)
 {
  array[i]=0;
  if(i<30)
   defeated[i]=0;
 }

wkm=0;
wrl=0;
wrr=0;
bkm=0;
brl=0;
brr=0;
defeatedcount=0;
whitedef=0;
blackdef=0;
g.drawboard();
g.startpawns();
g.boardnumbering();
g.drawdefeated();
m.initmouse();

setcolor(BLACK);
outtextxy(100,15,"CHECK MATE.. WHITE WINS THE GAME");
outtextxy(100,30,"CHECK ON BLACK KING");
outtextxy(100,30,"CHECK ON WHITE KING");
outtextxy(100,15,"WHITE TO PLAY");
outtextxy(100,15,"BLACK TO PLAY");

if(getturn()=='b')
changeturn();

setcolor(GREEN);
outtextxy(100,15,"WHITE TO PLAY");

return 0;
}

char validation::getturn()
{
return TURN;
}

validation::validate(int start,int end)
{
int arst,ared;
arst=array[start];
ared=array[end];
if(array[start]==WKING && start==60 && wkm==0 && isrisk(60)==FAIL)
 {
  if(array[end]==0 && end==62 && wrr==0  && array[61]==0 &&  array[62]==0 && willitrisk(60,61)==FAIL && willitrisk(63,62)==FAIL)
   {
    array[63]=0;
    g.drawblock(63);
    array[61]=WROOK;
    g.wr(61);

    if(TURN=='w')
      TURN='b';
    else
      if(TURN=='b')
	TURN='w';

    array[start]=arst;
    array[end]=ared;

    return SUCCESS;
   }

  if(array[end]==0 && end==58 && wrl==0   &&  array[59]==0 && willitrisk(60,59)==FAIL && willitrisk(56,58)==FAIL)
   {
    array[56]=0;
    g.drawblock(56);
    array[59]=WROOK;
    g.wr(59);

    if(TURN=='w')
      TURN='b';
    else
      if(TURN=='b')
	TURN='w';

    array[start]=arst;
    array[end]=ared;
    return SUCCESS;
   }
 }

if(array[start]==BKING && start==4 && bkm==0 && isrisk(4)==FAIL)
 {
  if(array[end]==0 && end==6 && brr==0  && array[5]==0 &&  array[6]==0 && willitrisk(4,5)==FAIL && willitrisk(7,6)==FAIL)
   {
    array[7]=0;
    g.drawblock(7);
    array[5]=BROOK;
    g.br(5);

    if(TURN=='w')
      TURN='b';
    else
      if(TURN=='b')
	TURN='w';

    array[start]=arst;
    array[end]=ared;

    return SUCCESS;
   }

  if(array[end]==0 && end==2 && brl==0   &&  array[3]==0 && willitrisk(4,3)==FAIL && willitrisk(0,2)==FAIL)
   {
    array[0]=0;
    g.drawblock(0);
    array[3]=BROOK;
    g.br(3);

    if(TURN=='w')
      TURN='b';
    else
      if(TURN=='b')
	TURN='w';


    array[start]=arst;
    array[end]=ared;

    return SUCCESS;
   }
 }

int ars,are;
ars=array[start];
are=array[end];

if(start==end)
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

setcolor(BLACK);
outtextxy(100,30,"CHECK ON BLACK KING");
outtextxy(100,30,"CHECK ON WHITE KING");
outtextxy(100,15,"WHITE TO PLAY");
outtextxy(100,15,"BLACK TO PLAY");

if(TURN=='w')
 {
  if(checkmate(WKING)==SUCCESS)
   {
    plflag=0;
    setcolor(GREEN);
    outtextxy(100,15,"CHECK MATE.. BLACK WINS THE GAME");
    getch();
    exit(0);
   }
  setcolor(GREEN);
  outtextxy(100,15,"WHITE TO PLAY");

  int kpos=getkingpos(WKING);
  if(isrisk(kpos)==SUCCESS)
   {
    setcolor(GREEN);
    outtextxy(100,30,"CHECK ON WHITE KING");

    if(dummyvalidate(start,end)==SUCCESS)
     {
      int st,ed;
      st=array[start];
      ed=array[end];

      array[end]=array[start];
      array[start]=0;

      if(isrisk(kpos)==SUCCESS)
       {
	array[start]=st;
	array[end]=ed;

	array[start]=arst;
	array[end]=ared;
	return FAIL;
       }

      array[start]=st;
      array[end]=ed;
     }
   }
 }
else
if(TURN=='b')
 {
  if(checkmate(BKING)==SUCCESS)
   {
       plflag=0;
    setcolor(GREEN);
    outtextxy(100,15,"CHECK MATE.. WHITE WINS THE GAME");
    getch();
    exit(0);
   }
  setcolor(GREEN);
  outtextxy(100,15,"BLACK TO PLAY");

  int kpos=getkingpos(BKING);
  if(isrisk(kpos)==SUCCESS)
   {
    setcolor(GREEN);
    outtextxy(100,30,"CHECK ON BLACK KING");
    if(dummyvalidate(start,end)==SUCCESS)
     {
      int st,ed;
      st=array[start];
      ed=array[end];

      array[end]=array[start];
      array[start]=0;

      if(isrisk(kpos)==SUCCESS)
       {
	array[start]=st;
	array[end]=ed;

	array[start]=arst;
	array[end]=ared;
	return FAIL;
       }
      array[start]=st;
      array[end]=ed;
     }
   }
 }
array[start]=ars;
array[end]=are;

int flag=FAIL;

if(array[end]==WKING || array[end]==BKING)
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

if(TURN=='w' && array[start]>10)
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

if(TURN=='b' && array[start]<10 && array[start]!=0)
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

if(array[start]==0)
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

if(array[start]<10 && (array[end]<10 && array[end]>0))
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

if(array[start]>10 && array[end]>10)
 {
  array[start]=arst;
  array[end]=ared;
  return FAIL;
 }

if(array[start]==WROOK || array[start]==BROOK)
 {
  flag= validaterook(start,end);
  if(flag==SUCCESS)
   {
    if(start==0)
      brl=1;
    else
      if(start==7)
	brr=1;
      else
	if(start==56)
	wrl=1;
	else
	 if(start==63)
	  wrr=1;
   }
 }

if(array[start]==WBISHOP || array[start]==BBISHOP)
  flag= validatebishop(start,end);

if(array[start]==WQUEEN || array[start]==BQUEEN)
  flag= validatequeen(start,end);

if(array[start]==WHORSE || array[start]==BHORSE)
  flag= validatehorse(start,end);

if(array[start]==WPAWN)
 {
  int num=validatewpawn(start,end);
  if(num==SUCCESS)
   {
    if(end<8)
     {
      ars=WQUEEN;
      arst=WQUEEN;
     }
   }
  flag= num;
 }

if(array[start]==BPAWN)
 {
  int num=validatebpawn(start,end);
  if(num==SUCCESS)
   {
    if(end>55)
     {
      ars=BQUEEN;
      arst=BQUEEN;
     }
   }
  flag= num;
 }

if(array[start]==WKING || array[start]==BKING)
 {
  if(willitrisk(start,end)==FAIL)
   flag= validateking(start,end);
   if(flag==SUCCESS)
    {
     if(array[start]==WKING)
      wkm=1;
     else
      if(array[start]==BKING)
       bkm=1;
    }
 }

array[end]=ars;
array[start]=0;

if((isrisk(getkingpos(WKING))==SUCCESS && g.returncolor(end)=='w') || (isrisk(getkingpos(BKING))==SUCCESS && g.returncolor(end)=='b'))
  flag=FAIL;

array[start]=ars;
array[end]=are;

if(flag==SUCCESS)
 {
  if(TURN=='w')
    TURN='b';
  else
   if(TURN=='b')
     TURN='w';
 }

if(array[end]!=0 && flag==SUCCESS)
 {
  defeated[defeatedcount]=array[end];
  defeatedcount++;

  if(array[end]<10)
   {
    g.defeatedimage(array[end],whitedef);
    whitedef++;
   }
  else
   if(array[end]>10)
    {
     g.defeatedimage(array[end],blackdef);
     blackdef++;
    }
 }

array[start]=arst;
array[end]=ared;
return flag;
}

validation::dummyvalidate(int start,int end)
{
int a;
if(array[start]!=0 && start<64 && end<64 && start>-1 && end>-1 && ((array[start]<10 && (array[end]>10)||array[end]==0)|| (array[start]>10 && (array[end]<10||array[end]==0))))
  a=a;
else
  return FAIL;

int arst,ared;
arst=array[start];
ared=array[end];
if(start==end)
return FAIL;
int flag=FAIL;

if(array[start]==0)
  return FAIL;

if(array[start]<10 && (array[end]<10 && array[end]>0))
  return FAIL;

if(array[start]>10 && array[end]>10)
  return FAIL;

if(array[start]==WROOK || array[start]==BROOK)
  flag= validaterook(start,end);

if(array[start]==WBISHOP || array[start]==BBISHOP)
  flag= validatebishop(start,end);

if(array[start]==WQUEEN || array[start]==BQUEEN)
  flag= validatequeen(start,end);

if(array[start]==WHORSE || array[start]==BHORSE)
  flag= validatehorse(start,end);

if(array[start]==WPAWN)
 {
  int num=validatewpawn(start,end);
  if(num==SUCCESS)
   {
    if(end<8)
     {
      array[start]=WQUEEN;
     }
   }
  flag= num;
 }

if(array[start]==BPAWN)
 {
  int num=validatebpawn(start,end);
  if(num==SUCCESS)
   {
    if(end>55)
     {
      array[start]=BQUEEN;
     }
   }
  flag= num;
 }

if(array[start]==WKING || array[start]==BKING)
  flag= validateking(start,end);

array[start]=arst;
array[end]=ared;

return flag;
}

validation::validaterook(int start,int end)
{
if(getx(start)!=getx(end) && gety(start)!=gety(end))
  return FAIL;

if(gety(start)==gety(end))
 {
  int sp,ep;
  if(start<end)
   {
    sp=start;
    ep=end;
   }
  else
   {
    sp=end;
    ep=start;
   }

  for(int i=sp+1;i<ep;i++)
   {
    if(array[i]!=0)
      return FAIL;
   }

 }

if(getx(start)==getx(end))
 {
  int sp,ep;
  if(start<end)
   {
    sp=start;
    ep=end;
   }
  else
   {
    sp=end;
    ep=start;
   }
  for(int i=sp+8;i<ep;i=i+8)
   {
    if(array[i]!=0)
    return FAIL;
   }
 }

return SUCCESS;
}

validation::validatebishop(int start,int end)
{
int sx,sy,ex,ey;
sx=getx(start);
sy=gety(start);
ex=getx(end);
ey=gety(end);
int flag=FAIL;
if(sx>ex && sy>ey)
 {
  if((start-end)%9!=0)
    return FAIL;

  for(int i=start-9;i>end-1;i=i-9)
    {
     if(array[i]!=0 && i!=end)
       return FAIL;
     else
       flag=SUCCESS;
    }
 }

if(sx<ex && sy>ey)
 {
  if((start-end)%7!=0)
    return FAIL;

  for(int i=start-7;i>end-1;i=i-7)
    {
     if(array[i]!=0 && i!=end)
       return FAIL;
     else
       flag=SUCCESS;
    }
 }

if(sx>ex && sy<ey)
 {
  if((end-start)%7!=0)
    return FAIL;
  for(int i=start+7;i<end+1;i=i+7)
    {
     if(array[i]!=0 && i!=end)
       return FAIL;
     else
       flag=SUCCESS;
    }
 }

if(sx<ex && sy<ey)
 {
  if((end-start)%9!=0)
    return FAIL;

  for(int i=start+9;i<end+1;i=i+9)
    {
     if(array[i]!=0 && i!=end)
       return FAIL;
     else
       flag=SUCCESS;
    }
 }

return flag;
}

validation::validatequeen(int start,int end)
{
if(validaterook(start,end)==SUCCESS || validatebishop(start,end)==SUCCESS)
  return SUCCESS;
else
  return FAIL;
}

validation::validatehorse(int start,int end)
{
int sx,sy,ex,ey;
sx=getx(start);
sy=gety(start);
ex=getx(end);
ey=gety(end);

if(ey==sy-2 && ex==sx-1)
  return SUCCESS;

if(ey==sy-2 && ex==sx+1)
  return SUCCESS;

if(ey==sy-1 && ex==sx+2)
  return SUCCESS;

if(ey==sy-1 && ex==sx-2)
  return SUCCESS;

if(ey==sy+2 && ex==sx-1)
  return SUCCESS;

if(ey==sy+2 && ex==sx+1)
  return SUCCESS;

if(ey==sy+1 && ex==sx-2)
  return SUCCESS;

if(ey==sy+1 && ex==sx+2)
  return SUCCESS;

return FAIL;
}

validation::validatewpawn(int start,int end)
{
int sx,sy,ex,ey;
sx=getx(start);
sy=gety(start);
ex=getx(end);
ey=gety(end);

if(sx==ex)
 {
  if(array[end]!=0)
    return FAIL;

  if(sy==6 && ey==4 && array[start-8]==0)
    return SUCCESS;

  if(ey==sy-1)
    return SUCCESS;
 }

if(sx!=ex)
 {
  if(array[end]==0)
    return FAIL;

  if(ex==sx-1 && ey==sy-1  )
    return SUCCESS;

  if(ex==sx+1 && ey==sy-1)
    return SUCCESS;
 }

return FAIL;
}


validation::validatebpawn(int start,int end)
{
int sx,sy,ex,ey;
sx=getx(start);
sy=gety(start);
ex=getx(end);
ey=gety(end);

if(sx==ex)
 {
  if(array[end]!=0)
    return FAIL;

  if(sy==1 && ey==3 && array[end-8]==0)
    return SUCCESS;

  if(ey==sy+1)
    return SUCCESS;
 }

if(sx!=ex)
 {
  if(array[end]==0)
   return FAIL;

  if(ex==sx+1 && ey==sy+1)
   return SUCCESS;

  if(ex==sx-1 && ey==sy+1)
   return SUCCESS;
 }

return FAIL;
}

validation::validateking(int start,int end)
{
int sx,sy,ex,ey;
sx=getx(start);
sy=gety(start);
ex=getx(end);
ey=gety(end);

if((ex==sx+1 || ex==sx-1)&& (end==start+1 || end==start-1)&&ey==sy )
  return SUCCESS;

if(ex==sx &&(ey==sy+1 || ey==sy-1 )&&(end==start+8 || end==start-8) )
  return SUCCESS;

if(ex!=sx && ey!=sy)
 {
 if(ex==sx-1 && ey==sy-1 && end==start-9)
   return SUCCESS;

 if(ex==sx+1 && ey==sy-1 && end==start-7)
   return SUCCESS;

 if(ex==sx-1 && ey==sy+1 && end==start+7)
   return SUCCESS;

 if(ex==sx+1 && ey==sy+1 && end==start+9)
   return SUCCESS;
 }

return FAIL;
}

validation::isrisk(int num)
{
for(int i=0;i<64;i++)
  {
   if((g.returncolor(num)=='w' && g.returncolor(i)=='b') || (g.returncolor(num)=='b' && g.returncolor(i)=='w'))
    {
     if(dummyvalidate(i,num)==SUCCESS && array[num]!=0)
       return SUCCESS;
    }
  }

return FAIL;
}

validation::changeturn()
{
setcolor(BLACK);
outtextxy(100,15,"WHITE TO PLAY");
outtextxy(100,15,"BLACK TO PLAY");
setcolor(GREEN);
if(TURN=='w')
 {
  TURN='b';
  outtextxy(100,15,"BLACK TO PLAY");
 }
else
 if(TURN=='b')
  {
   TURN='w';
   outtextxy(100,15,"WHITE TO PLAY");
  }

return SUCCESS;
}

validation::willitrisk(int start,int end)
{
int st,ed;
st=array[start];
ed=array[end];
int flag=FAIL;

if(dummyvalidate(start,end)==SUCCESS)
 {
  array[start]=0;
  array[end]=st;
  if(isrisk(end)==SUCCESS)
   flag=SUCCESS;
 }

array[start]=st;
array[end]=ed;

return flag;
}

validation::getkingpos(int num)
{
if(num==WKING)
 {
  for(int i=0;i<64;i++)
   {
    if(array[i]==WKING)
      return i;
   }
 }
else
 if(num==BKING)
  {
   for(int i=0;i<64;i++)
     {
      if(array[i]==BKING)
	return i;
     }
  }

return -1;
}

validation::checkmate(int king)
{
int kpos=-1;

if(king==WKING)
  kpos=getkingpos(WKING);
else
  if(king==BKING)
    kpos=getkingpos(BKING);

if(isrisk(kpos)==SUCCESS)
 {
  for(int i=0;i<64;i++)
   {
    if((king==WKING && g.returncolor(i)=='w') ||(king==BKING && g.returncolor(i)=='b'))
     {
      for(int j=0;j<64;j++)
       {
	if(king==WKING)
	  kpos=getkingpos(WKING);
	else
	 if(king==BKING)
	   kpos=getkingpos(BKING);

	if(dummyvalidate(i,j)==SUCCESS)
	 {
	  int st,ed;
	  st=array[i];
	  ed=array[j];

	  array[j]=st;
	  array[i]=0;

	  if(king==WKING)
	   kpos=getkingpos(WKING);
	  else
	   if(king==BKING)
	    kpos=getkingpos(BKING);

	  if(isrisk(kpos)==FAIL)
	   {
	    array[i]=st;
	    array[j]=ed;
	    return FAIL;
	   }

	  array[i]=st;
	  array[j]=ed;
	 }
       }
     }
   }
 }

if(isrisk(kpos)==SUCCESS)
  return SUCCESS;
else
  return FAIL;
}

//validation class Ending
//------------------------------------------------------------------------------------------------------//


//AI class Starting
//------------------------------------------------------------------------------------------------------//
class ai{
	private:
	  int point;
	  int poin;
	  int aiarray[64];
	  int m1,m2;
	public:
	  void getmove();
	  getm1();
	  getm2();
	};

ai::getm1()
{
return m1;
}

ai::getm2()
{
return m2;
}

void ai::getmove()
{
time_t t;
srand((unsigned) time(&t));
randomize();

poin=0;
point=0;
m1=0;
m2=0;
for(int i=0;i<64;i++)
  aiarray[i]=array[i];

for(int from=0;from<64;from++)
  {
   for(int to=0;to<64;to++)
     {
      for(int i=0;i<64;i++)
	array[i]=aiarray[i];

      if(array[from]>10 && from!=to && v1.dummyvalidate(from,to)==SUCCESS)
	{
	 array[to]=array[from];
	 array[from]=0;

	 if(v1.isrisk(v1.getkingpos(BKING))==FAIL)
	  {
	   if(m1==0 && m2==0)
	    {
	     m1=from;
	     m2=to;
	     point=0;
	     poin=0;
	    }
//--------------------
	   poin=rand() % 10;

	   if(v1.checkmate(WKING)==SUCCESS)
	     poin=poin+10000;

	   if(v1.isrisk(v1.getkingpos(WKING))==SUCCESS)
	     poin=poin+200;

	   if(array[to]==BKING && aiarray[to]==0)
	    {
	     poin=poin-1000;
	    }

	   if(array[to]==BROOK && (from-to==1 || from-to==-1 ))
	    {
	     poin=poin-1000;
	    }

	   if(aiarray[to]==WQUEEN)
	    {
	     if(v1.isrisk(to)==SUCCESS && array[to]==BQUEEN)
	       poin=poin-1200;
	     else
	       poin=poin+1000;
	    }

	   if(aiarray[to]==WHORSE)
	    {
	     if((array[to]==BQUEEN || array[to]==BROOK)&& v1.isrisk(to)==SUCCESS)
	       poin=poin-750;
	     else
	       poin=poin+775;
	    }

	   if(aiarray[to]==WROOK)
	    {
	     if((array[to]==BQUEEN || array[to]==BROOK || array[to]==BHORSE)&& v1.isrisk(to)==SUCCESS)
	       poin=poin-700;
	     else
	       poin=poin+750;
	    }


	   if(aiarray[to]==WBISHOP)
	    {
	     if((array[to]==BQUEEN || array[to]==BROOK || array[to]==BHORSE || array[to]==BBISHOP)&& v1.isrisk(to)==SUCCESS)
	       poin=poin-650;
	     else
	       poin=poin+725;
	    }


	   if(aiarray[to]==WPAWN)
	    {
	     if(v1.isrisk(to)==SUCCESS)
	       poin=poin-600;
	     else
	       poin=poin+700;
	    }

	   if(array[to]==BQUEEN && v1.isrisk(to)==SUCCESS)
	     poin=poin-1200;

	   if(array[to]==BHORSE && v1.isrisk(to)==SUCCESS)
	     poin=poin-750;

	   if(array[to]==BROOK && v1.isrisk(to)==SUCCESS)
	     poin=poin-700;

	   if(array[to]==BBISHOP && v1.isrisk(to)==SUCCESS)
	     poin=poin-650;

	   for(int se=0;se<64;se++)
	     {
	      if(v1.isrisk(se)==SUCCESS && array[se]>10)
	       {
		if(array[se]==BQUEEN)
		 poin=poin-600;

		if(array[se]==BHORSE)
		 poin=poin-550;

		if(array[se]==BROOK)
		 poin=poin-500;

		if(array[se]==BBISHOP)
		 poin=poin-450;
	       }
	     }
//--------------------
	   if(poin>point)
	    {
	     point=poin;
	     m1=from;
	     m2=to;
	     poin=0;
	    }
	  }
	}
     }
  }

for(i=0;i<64;i++)
array[i]=aiarray[i];
}

//AI class Ending
//------------------------------------------------------------------------------------------------------//



//movedisplay class Starting
//------------------------------------------------------------------------------------------------------//
struct chessnode{
		int movefrom;
		int movefromwhat;
		int moveto;
		int movetowhat;
		struct chessnode *prev;
		struct chessnode *next;
		};

class movedisplay{
	protected:
	  struct chessnode *head;
	  struct chessnode *tail;
	  struct chessnode *present;
	  struct chessnode *previous;

	  int movecount;
	public:
	  movedisplay();
	  createnode(int,int,int,int);
	  undomove();
	  getcount();
	  seeundo(int,int,int);
	  save();
	  savedisplay(int,int,int);
	  load();
	  loaddisplay(int,int,int);
	  deletelist();
		 };

movedisplay::deletelist()
{
head=NULL;
tail=NULL;
present=NULL;
previous=NULL;
movecount=-1;
movedisplay();
return 0;
}

movedisplay::seeundo(int button,int x,int y)
{
if(x>438 && x<485 && y>390 && y<412 && getcount()!=-1)
 {
  m.setCursor(HAND);
  setfillstyle(1,GREEN);
  bar(435+3,420-30+1,435+50,420-5-3);
  setcolor(BROWN);
  outtextxy(435+10,420-25,"UNDO");

  if(button==1)
   {
    gotoxy(xz1,yz1++);

    int k=undomove();
    if(k!=-1)
     {
      v1.changeturn();
     }

    button=0;
   }
 }
 else
 {
  setcolor(GREEN);
  setfillstyle(1,BROWN);
  bar(435+3,420-30+1,435+50,420-5-3);
  outtextxy(435+10,420-25,"UNDO");
  m.setCursor(ARROW);
 }
return 0;
}

movedisplay::load()
{
v1.resetboard();
deletelist();
FILE *fp;
fp=fopen("save.dat","r");
struct chessnode *temp;
while(!feof(fp))
 {
  fread(&temp,sizeof(struct chessnode),1,fp);
  if( v1.validate(temp->movefrom,temp->moveto)==SUCCESS)
   {
    g.move(temp->movefrom,temp->moveto);
    createnode(temp->movefrom,temp->moveto,temp->movefromwhat,temp->movetowhat);
   }
 }

if( v1.validate(temp->movefrom,temp->moveto)==SUCCESS)
 {
  g.move(temp->movefrom,temp->moveto);
  createnode(temp->movefrom,temp->moveto,temp->movefromwhat,temp->movetowhat);
 }

fclose(fp);
return 0;
}

movedisplay::savedisplay(int button,int x,int y)
{
if(x>487 && x<534 && y>390 && y<412 && getcount()!=-1)
 {
  m.setCursor(HAND);
  setfillstyle(1,GREEN);
  bar(435+52,420-30+1,435+100,420-5-3);
  setcolor(BROWN);
  outtextxy(435+60,420-25,"SAVE");
  if(button==1)
   {
    save();
   }
 }
else
 {
  setcolor(GREEN);
  setfillstyle(1,BROWN);

  m.setCursor(ARROW);
  bar(435+52,420-30+1,435+100,420-5-3);
  outtextxy(435+60,420-25,"SAVE");
 }
return 0;
}

movedisplay::loaddisplay(int button,int x,int y)
{
if(x>536 && x<583 && y>390 && y<412)
 {
  m.setCursor(HAND);
  setfillstyle(1,GREEN);
  bar(435+102,420-30+1,435+152,420-5-3);
  setcolor(BROWN);
  outtextxy(435+110,420-25,"LOAD");

  if(button==1)
   {
    load();
   }
 }
else
 {
  setcolor(GREEN);
  setfillstyle(1,BROWN);
  m.setCursor(ARROW);
  bar(435+102,420-30+1,435+152,420-5-3);
  outtextxy(435+110,420-25,"LOAD");
 }
return 0;
}

movedisplay::getcount()
{
return movecount;
}


movedisplay::movedisplay()
{
movecount=-1;
head=NULL;
present=previous=NULL;
}

movedisplay::createnode(int from,int to,int fromw,int tow)
{
movecount++;
present=new chessnode;
present->movefrom=from;
present->movefromwhat=fromw;
present->moveto=to;
present->movetowhat=tow;
present->next=NULL;
present->prev=previous;
tail=present;

if(movecount!=0)
 {
  previous->next=present;
 }

if(movecount==0)
  head=present;

previous=present;
return 0;
}


movedisplay::undomove()
{
xz1=57,yz1++;
if(movecount<0)
  return -1;

m.disableMouse();
g.draw(0,tail->movefrom);
g.draw(0,tail->moveto);
g.draw(tail->movefromwhat,tail->movefrom);
g.draw(tail->movetowhat,tail->moveto);

if(tail->movetowhat!=0)
 {
  if(tail->movetowhat<10)
    g.deletedefeated(1);
  else
    g.deletedefeated(15);
  }

array[tail->movefrom]=tail->movefromwhat;
array[tail->moveto]=tail->movetowhat;

struct chessnode *temp;
temp=tail;
tail=tail->prev;
tail->next=NULL;
delete temp;
previous=tail;

movecount--;

return 0;
}

movedisplay::save()
{
FILE *fp;
fp=fopen("save.dat","w");
struct chessnode *temp;
temp=head;
while(temp->next!=NULL)
 {
  fwrite(&temp,sizeof(struct chessnode),1,fp);
  temp=temp->next;
 }
fwrite(&temp,sizeof(struct chessnode),1,fp);
fclose(fp);
return 0;
}

//movedisplay class Ending
//------------------------------------------------------------------------------------------------------//

void movesmade()
{
setfillstyle(1,6);
bar(440,130,630,385);
setfillstyle(1,7);
}

char conchar(int num)
{
return (char)97+num;
}

main()
{
moves=0;
ai ai;
fstream store;
store.open("C:\\project\\store.dat",ios::out);
store.close();



movedisplay dm;
ma:
g.inig();
g.startscreen();
g.drawboard();
g.startpawns();
g.boardnumbering();
g.drawdefeated();
m.initmouse();
m.setCursor(HAND);
int x,y,button,m1,m2;
//movesmade();
setcolor(GREEN);
outtextxy(100,15,"WHITE TO PLAY");
int ch;
while(ch!=27)
 {
  if(plflag==1)
   {
    gotoxy(56,4);
    cout<<"Single Player Mode";
   }
  else
   {
    gotoxy(56,4);
    cout<<"Two Player Mode         ";
   }

  g.cursor();
  if(kbhit())
   {
    ch=getch();

    if(ch=='2')
     {
      if(plflag==0)
	plflag=1;
      else
	plflag=0;
     }

    if(ch==27)
     {
      v1.resetboard();
      ch=' ';
      goto ma;}
     }

    m.showmouseptr();
    m.getmousepos(&button,&x,&y);
    dm.seeundo(button,x,y);
    dm.savedisplay(button,x,y);
    dm.loaddisplay(button,x,y);

    g.deleteborder(m1);

   if(button==1 &&  (v1.getturn()=='w' || plflag==0))
    {
     m1=m.getpoint();
     m.disableMouse();
     g.drawborder(m1);
     m.showmouseptr();
     while(button==1)
      {
       m2=m.getpoint();
       m.getmousepos(&button,&x,&y);
      }
    }

int temm1,temm2;

if(v1.getturn()=='b' && plflag==1)
 {
  ai.getmove();
  m1=ai.getm1();
  m2=ai.getm2();
 }

temm1=array[m1];
temm2=array[m2];

if(v1.validate(m1,m2)==SUCCESS)
 {
  store.open("C:\\project\\store.dat",ios::app);
  if(array[m1]==WKING)
   {
    store<<"Ki ";
   }
  if(array[m1]==WQUEEN)
   {
    store<<"Qu ";
   }
  if(array[m1]==WROOK)
   {
    store<<"Ro ";
   }
  if(array[m1]==WPAWN)
   {
    store<<"Pa ";
   }
  if(array[m1]==WHORSE)
   {
    store<<"Ho ";
   }
  if(array[m1]==WBISHOP)
   {
    store<<"Bi ";
   }
  if(array[m1]==BKING)
   {
    store<<"Ki ";
   }
  if(array[m1]==BQUEEN)
   {
    store<<"Qu ";
   }
  if(array[m1]==BROOK)
   {
    store<<"Ro ";
   }
  if(array[m1]==BPAWN)
   {
    store<<"Pa ";
   }
  if(array[m1]==BHORSE)
   {
    store<<"Ho ";
   }
  if(array[m1]==BBISHOP)
   {
    store<<"Bi ";
   }

  store<<conchar(getx(m1));
  store<<gety(m1)+1;
  store<<"->";
  store<<conchar(getx(m2));

  store<<gety(m2)+1;

  if(v1.getturn()=='w')
   {
    store<<"\n";
    moves++;
   }
  else
    store<<" - ";

  store.close();

  m.disableMouse();
  dm.createnode(m1,m2,temm1,temm2);
  g.move(m1,m2);
  g.deleteborder(m1);

  store.open("C:\\project\\store.dat",ios::in);
  char ch;
  xz1=57,yz1=9;

  if(moves>15)
   store.seekg(23*(moves-15));

  while(!store.eof())
   {
    store.get(ch);
    if(store.eof())
      break;

    if(ch!='\n' && !store.eof())
     {
      gotoxy(xz1++,yz1);
      cout<<ch;
     }
    else
     {
      yz1++;
      xz1=57;
     }
   }
 store.close();
 }
 gotoxy(57,24);
 cout<<"Moves > "<<moves<<"           ";
 }
moves=0;
store.open("C:\\project\\store.dat",ios::out);
store.close();

v1.resetboard();
goto ma;
}