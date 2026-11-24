#include <tamtypes.h>
#include <kernel.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <libpad.h>
#include <debug.h>
#include <stdio.h>

static GSGLOBAL *gsGlobal;
static unsigned char padBuf[256] __attribute__((aligned(64)));
static int selected = 0, loading = 0, loadingTicks = 0, lastMove = 0;
static const char *games[] = {
 "Grand Theft Auto VI", "Grand Theft Auto V", "Red Dead Redemption 2",
 "Marvel's Spider-Man 2", "God of War Ragnarok", "PES 2018",
 "Gran Turismo 7", "Horizon Forbidden West"
};
#define GAME_COUNT 8

static u64 rgb(u8 r,u8 g,u8 b){ return GS_SETREG_RGBAQ(r,g,b,0x80,0); }
static void rect(float x1,float y1,float x2,float y2,u64 c){
 gsKit_prim_sprite(gsGlobal,x1,y1,x2,y2,1,c);
}
static int pressed(u32 mask,u32 buttons){ return (buttons & mask)==0; }

static void draw_menu(void){
 int i;
 rect(0,0,640,448,rgb(9,13,25));
 rect(0,0,640,5,rgb(20,120,255));
 rect(32,58,608,60,rgb(40,50,70));
 rect(32,78,608,218,rgb(18,27,47));
 rect(32,78,38,218,rgb(25,130,255));
 for(i=0;i<GAME_COUNT;i++){
  int col=i%4,row=i/4;
  float x=32.0f+col*145.0f,y=244.0f+row*68.0f;
  rect(x,y,x+132,y+56,(i==selected)?rgb(28,90,160):rgb(24,34,55));
  if(i==selected){
   rect(x,y,x+132,y+3,rgb(45,155,255));
   rect(x,y+53,x+132,y+56,rgb(45,155,255));
  }
  rect(x+9,y+9,x+13,y+47,rgb((u8)(50+i*19),(u8)(105+i*11),235));
 }
 init_scr(); scr_clear();
 scr_setXY(3,1); scr_printf("PLAYSTATION 2 | PS5 STYLE DASHBOARD");
 scr_setXY(4,4); scr_printf("WELCOME BACK");
 scr_setXY(4,6); scr_printf("%s",games[selected]);
 scr_setXY(4,8);
 if(selected==0) scr_printf("DISPLAY DEMO ONLY - NOT THE REAL GTA VI");
 else scr_printf("INFO PREVIEW - THIS TITLE DOES NOT LAUNCH");
 scr_setXY(4,10); scr_printf("D-PAD / LEFT STICK: MOVE     X: SELECT");
 scr_setXY(4,11); scr_printf("SELECT: BACK FROM DEMO");
 scr_setXY(4,16); scr_printf("GAMES");
 for(i=0;i<GAME_COUNT;i++){
  int col=i%4,row=i/4;
  scr_setXY(4+col*18,18+row*4);
  scr_printf("%c %s",(i==selected)?'>':' ',games[i]);
 }
 gsKit_queue_exec(gsGlobal); gsKit_sync_flip(gsGlobal);
}

int main(void){
 struct padButtonStatus b;
 int oldSelect=0,oldX=0;
 gsGlobal=gsKit_init_global();
 if(!gsGlobal) return 1;
 dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
 dmaKit_chan_init(DMA_CHANNEL_GIF);
 gsKit_init_screen(gsGlobal);
 padInit(0); padPortOpen(0,0,padBuf);
 draw_menu();
 for(;;){
  int move=0,sel=0,x=0;
  if(padGetState(0,0)==PAD_STATE_STABLE && padRead(0,0,&b)){
   if(pressed(PAD_LEFT,b.btns)||b.ljoy_h<70) move=-1;
   if(pressed(PAD_RIGHT,b.btns)||b.ljoy_h>185) move=1;
   if(pressed(PAD_UP,b.btns)||b.ljoy_v<70) move=-4;
   if(pressed(PAD_DOWN,b.btns)||b.ljoy_v>185) move=4;
   sel=pressed(PAD_SELECT,b.btns); x=pressed(PAD_CROSS,b.btns);
  }
  if(loading){
   if(sel&&!oldSelect){ loading=0; loadingTicks=0; draw_menu(); }
   else if(++loadingTicks>450){ loading=0; loadingTicks=0; draw_menu(); }
  }else{
   if(move&&!lastMove){
    selected+=move;
    if(selected<0) selected=0;
    if(selected>=GAME_COUNT) selected=GAME_COUNT-1;
    draw_menu();
   }
   if(x&&!oldX){
    init_scr(); scr_clear();
    if(selected==0){
     loading=1; loadingTicks=0;
     scr_setXY(10,10); scr_printf("GRAND THEFT AUTO VI");
     scr_setXY(10,13); scr_printf("LOADING...");
     scr_setXY(10,16); scr_printf("PRESS SELECT TO RETURN");
    }else{
     scr_setXY(5,8); scr_printf("%s",games[selected]);
     scr_setXY(5,10); scr_printf("PREVIEW ONLY - GAME NOT INSTALLED");
     scr_setXY(5,13); scr_printf("PRESS SELECT TO RETURN");
     for(;;){
      if(padGetState(0,0)==PAD_STATE_STABLE && padRead(0,0,&b) && pressed(PAD_SELECT,b.btns)) break;
      for (volatile int wait = 0; wait < 50000; ++wait) { }
     }
     draw_menu();
    }
   }
  }
  lastMove=move; oldSelect=sel; oldX=x;
  if(loading && (loadingTicks%90)==0){
   init_scr(); scr_clear();
   scr_setXY(10,10); scr_printf("GRAND THEFT AUTO VI");
   scr_setXY(10,13); scr_printf("LOADING...");
   scr_setXY(10,16); scr_printf("PRESS SELECT TO RETURN");
  }
  for (volatile int wait = 0; wait < 50000; ++wait) { }
 }
 return 0;
}