#include <tamtypes.h>
#include <kernel.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <libpad.h>
#include <debug.h>
#include <stdio.h>

#define GAME_COUNT 8
#define SCREEN_W 640
#define SCREEN_H 448

static GSGLOBAL *gsGlobal;
static unsigned char padBuf[256] __attribute__((aligned(64)));
static int selected = 0;
static int page = 0;

static const char *games[GAME_COUNT] = {
    "Homebrew Arcade Pack", "Open Puzzle Collection",
    "PS2 Retro Demo Disc", "Community Platformer",
    "Open Racing Demo", "Homebrew Space Shooter",
    "Classic 2D Collection", "PS2 Indie Showcase"
};
static const char *genres[GAME_COUNT] = {
    "ARCADE", "PUZZLE", "DEMO", "PLATFORM",
    "RACING", "ARCADE", "CLASSICS", "INDIE"
};
static const char *descriptions[GAME_COUNT] = {
    "Community-made arcade homebrew collection.",
    "Small open puzzle games made for PS2 homebrew.",
    "A collection of freely distributable PS2 demos.",
    "Independent platform games for the PS2 scene.",
    "A community racing homebrew showcase.",
    "Independent space shooter homebrew.",
    "A selection of original retro-style homebrew.",
    "New independent projects from the PS2 community."
};

static u64 rgb(u8 r,u8 g,u8 b){ return GS_SETREG_RGBAQ(r,g,b,0x80,0); }
static void rect(float x1,float y1,float x2,float y2,u64 c){ gsKit_prim_sprite(gsGlobal,x1,y1,x2,y2,1,c); }
static int pressed(u32 mask,u32 buttons){ return (buttons & mask)==0; }
static void background(void){
    rect(0,0,SCREEN_W,SCREEN_H,rgb(8,10,17));
    rect(0,0,SCREEN_W,5,rgb(235,38,58));
    rect(0,5,8,SCREEN_H,rgb(235,38,58));
    rect(24,22,616,68,rgb(20,22,32));
    rect(24,76,616,78,rgb(50,52,66));
}
static void present(void){ gsKit_queue_exec(gsGlobal); gsKit_sync_flip(gsGlobal); }

static void draw_store(void){
    int i;
    background();
    init_scr(); scr_clear();
    scr_setXY(3,2); scr_printf("ORBIT GAME STORE");
    scr_setXY(3,4); scr_printf("PS2 HOMEBREW MARKET | COMMUNITY CATALOG");
    scr_setXY(3,6); scr_printf("NETWORK: NOT INITIALIZED IN THIS BUILD");
    scr_setXY(3,8); scr_printf("D-PAD: BROWSE | X: DETAILS | CIRCLE: EXIT");
    for(i=0;i<GAME_COUNT;i++){
        int col=i%2, row=i/2;
        float x=28.0f+col*300.0f, y=112.0f+row*72.0f;
        rect(x,y,x+282,y+60,(i==selected)?rgb(75,24,38):rgb(22,27,40));
        rect(x,y,x+4,y+60,(i==selected)?rgb(255,65,75):rgb(55,65,85));
        scr_setXY(5+col*38,14+row*4);
        scr_printf("%c %s",(i==selected)?'>':' ',games[i]);
        scr_setXY(7+col*38,15+row*4);
        scr_printf("%s",genres[i]);
    }
    scr_setXY(3,31); scr_printf("SELECT: REFRESH CATALOG (COMING SOON)");
    scr_setXY(3,33); scr_printf("Downloads are disabled until the network client is ready.");
    present();
}

static void draw_details(void){
    background();
    init_scr(); scr_clear();
    scr_setXY(4,3); scr_printf("ORBIT GAME STORE / GAME DETAILS");
    scr_setXY(4,7); scr_printf("%s",games[selected]);
    scr_setXY(4,10); scr_printf("CATEGORY: %s",genres[selected]);
    scr_setXY(4,13); scr_printf("%s",descriptions[selected]);
    scr_setXY(4,18); scr_printf("DOWNLOAD: NOT AVAILABLE YET");
    scr_setXY(4,23); scr_printf("This is a catalog placeholder, not a downloadable game.");
    scr_setXY(4,28); scr_printf("CIRCLE: BACK");
    scr_setXY(4,30); scr_printf("Online updates and real byte-based progress are planned.");
    present();
}

int main(void){
    struct padButtonStatus b;
    int oldX=0, oldCircle=0, oldSelect=0, oldLeft=0, oldRight=0;
    gsGlobal=gsKit_init_global();
    if(!gsGlobal) return 1;
    dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF,D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF,D_CTRL_RCYC_8,1<<DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    padInit(0); padPortOpen(0,0,padBuf);
    draw_store();

    for(;;){
        int x=0, circle=0, select=0, left=0, right=0;
        if(padGetState(0,0)==PAD_STATE_STABLE && padRead(0,0,&b)){
            x=pressed(PAD_CROSS,b.btns);
            circle=pressed(PAD_CIRCLE,b.btns);
            select=pressed(PAD_SELECT,b.btns);
            left=pressed(PAD_LEFT,b.btns)||b.ljoy_h<70;
            right=pressed(PAD_RIGHT,b.btns)||b.ljoy_h>185;
        }
        if(page==0){
            if((left&&!oldLeft)||(right&&!oldRight)){
                selected+=(right&&!oldRight)?1:-1;
                if(selected<0) selected=GAME_COUNT-1;
                if(selected>=GAME_COUNT) selected=0;
                draw_store();
            }
            if(x&&!oldX){ page=1; draw_details(); }
            if(select&&!oldSelect) draw_store();
        }else if(circle&&!oldCircle){
            page=0; draw_store();
        }
        oldX=x; oldCircle=circle; oldSelect=select;
        oldLeft=left; oldRight=right;
        for(volatile int wait=0;wait<50000;++wait){}
    }
    return 0;
}