#include <iostream>
#include <fstream>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <cstdio>
#include <stdexcept>
#include <conio.h>
#include <io.h>
#include <time.h>
#include <vector>
#include <windows.h>
#include <sys/stat.h>
using namespace std;

struct nod
{
    int nivel1,nivel2;
    int st,dr;
    int nrFii;
    int adancime;
    char expresie[100];
    char tip[50];
    nod *fii[100];
    int linie;
};

int screen_width = GetSystemMetrics(SM_CXSCREEN);
int screen_height = GetSystemMetrics(SM_CYSCREEN);

double instrH=0.03*screen_height;
double returnH=0.045*screen_height;
double loopH=0.04*screen_height;
double ifH=0.05*screen_height;
double switchH=0.065*screen_height;

double separareH=0.08*screen_height;
double loopIndentare=0.02*screen_width;
double functionH=0.06*screen_height;
int scrollH=0.1*screen_height;

nod *rad[100]; nod *stiva[200];
nod *stiva2[200];

int drawFunction(int st, int dr, int nivel, char s[]);
int drawIf(int st, int dr, int nivel, char s[]);
int drawLoop(int st, int dr, int nivel, char s[]);
int drawUntil(int st, int dr, int nivel1, int nivel2, char s[]);
int drawInstruction(int st, int dr, int nivel, char s[]);
int drawReturn(int st, int dr, int nivel, char s[]);
int drawBreak(int st, int dr, int nivel, char s[]);
int drawSwitch(int st, int dr, int nivel, char cond[], char s[][100], int nrCazuri);
void endIf(int st, int dr, int nivel1, int nivel2);
void endLoop(int st, int dr, int nivel1, int nivel2);
void endInstruction(nod *p, int nivel, int nivel2);
void endReturn(int st, int dr, int nivel1, int nivel2);
void endSwitch(int st, int dr, int nivel1, int nivel2);
void endBreak(int st, int dr, int nivel1, int nivel2);

void drawButon(int x1,int y1,int x2,int y2,char s[],int tip);
void drawThemeButton();

bool instructiune(char *s, char instr[]);
void setareCoord(nod *parinte, nod *fiu);
void f();

int dfs(nod *p,int nivel);
void bloc(int x, int y, const char* filename);
int dfs2(nod *p,int x,int y,const char* filename);
int dfs3(nod *p,int x,int y);
void diagrama(int &nivel);
void setNod(nod *p, char *s);
void scroll();
void eroare();
void menu();
void menuFile();
void selectFileMenu();
void helpMenu();
void signalHandler(int sig);
void setFullScreen();
void blocDiagrama(int &nivel);

void CitesteSir(nod*p,int x, int y, char S[200]);

char selectie[100];///tin minte sirul de caractere de afisat in meniul cu fisierele, si sa nu se modifice cand ies din file
char path[256];

int cntstiva,cntrad;
int nivel=0.1*screen_height,nivelFinal;
int scrollJos,scrollSus,isDarkTheme=0,isEditing=0,inDiagrama=0;

int culFunctie=RGB(170, 170, 170),culFunctieDARK=RGB(30, 32, 30),culFunctieLIGHT=RGB(246, 246, 246)
,culProcedura=RGB(170, 170, 170),culProceduraDARK=RGB(30, 32, 30),culProceduraLIGHT=RGB(246, 246, 246)
,culInstructiune=RGB(223, 223, 222),culInstructiuneDARK=RGB(82, 82, 82),culInstructiuneLIGHT=RGB(223, 223, 222)
,culIf=RGB(245, 245, 247),culIfDARK=RGB(49, 49, 49),culIfLIGHT=RGB(245, 245, 247)
,culSwitch=RGB(221, 221, 221),culSwitchDARK=RGB(49, 49, 49),culSwitchLIGHT=RGB(221, 221, 221)
,culLoop=RGB(200, 198, 198),culLoopDARK=RGB(65, 65, 65),culLoopLIGHT=RGB(200, 198, 198)
,culUntil=RGB(200, 198, 198),culUntilDARK=RGB(65, 65, 65),culUntilLIGHT=RGB(200, 198, 198)
,culReturn=RGB(202, 202, 202),culReturnDARK=RGB(22, 22, 22),culReturnLIGHT=RGB(202, 202, 202)
,culBreak=RGB(202, 202, 202),culBreakDARK=RGB(22, 22, 22),culBreakLIGHT=RGB(202, 202, 202)
,culPunctulete=RGB(204, 219, 253),culPunctuleteDARK=RGB(204, 219, 253),culPunctuleteLIGHT=RGB(204, 219, 253)

,culBkDARK=RGB(0, 32, 46)
,culBkLIGHT=RGB(137, 168, 178)
,culButonLIGHT=RGB(251, 248, 239)
,culButonHoverLIGHT=RGB(229, 225, 218)
,culButonDARK=RGB(67, 121, 242)
,culButonHoverDARK=RGB(128, 196, 233)
,culScrisMain=RGB(10, 151, 176)
,culConturLIGHT=RGB(22, 121, 171)
,culConturDARK=RGB(137, 168, 178)
,culScrisLIGHT=RGB(137, 168, 178),culScrisDARK=RGB(255,255,255)
,culBk,culButon,culButonHover,culContur

,blueLightButHover=RGB(229, 225, 218),blueLightBut=RGB(251, 248, 239),blueLightContur=RGB(22, 121, 171),blueLightBk=RGB(137, 168, 178), blueLightScris=RGB(137, 168, 178)
,blueDarkButHover=RGB(128, 196, 233),blueDarkBut=RGB(67, 121, 242),blueDarkContur=RGB(137, 168, 178),blueDarkBk=RGB(0, 32, 46),blueDarkScris=RGB(255,255,255)

,rosLightButHover=RGB(242, 232, 198),rosLightBut=RGB(218, 212, 181),rosLightContur=RGB(128, 0, 0),rosLightBk=RGB(167,30,52),rosLightScris=RGB(67,40,24)
,rosDarkButHover=RGB(100,13,20),rosDarkBut=RGB(128,14,19),rosDarkContur=RGB(37,9,2),rosDarkBk=RGB(56,4,14),rosDarkScris=RGB(255,255,255)

,verdeLightButHover=RGB(216, 196, 182),verdeLightBut=RGB(239, 227, 194),verdeLightContur=RGB(62, 123, 39),verdeLightBk=RGB(133, 169, 71), verdeLightScris=RGB(71, 102, 59)
,verdeDarkButHover=RGB(49, 81, 30),verdeDarkBut=RGB(62, 123, 39),verdeDarkContur=RGB(239, 227, 194),verdeDarkBk=RGB(18, 53, 36),verdeDarkScris=RGB(239, 227, 194)

,rozLightButHover=RGB(255, 207, 207),rozLightBut=RGB(255, 253, 236),rozLightContur=RGB(218, 73, 141),rozLightBk=RGB(225, 149, 171), rozLightScris=RGB(218, 73, 141)
,rozDarkButHover=RGB(106, 66, 194),rozDarkBut=RGB(139, 93, 255),rozDarkContur=RGB(255, 247, 209),rozDarkBk=RGB(59, 30, 84),rozDarkScris=RGB(255, 247, 209)

,orangeLightButHover=RGB(255, 179, 142),orangeLightBut=RGB(255, 207, 157),orangeLightContur=RGB(235, 91, 0),orangeLightBk=RGB(222, 143, 95), orangeLightScris=RGB(222, 143, 95)
,orangeDarkButHover=RGB(155, 57, 34),orangeDarkBut=RGB(242, 97, 63),orangeDarkContur=RGB(235, 91, 0),orangeDarkBk=RGB(72, 30, 20),orangeDarkScris=RGB(239, 227, 194)

,customLightButHover,customLightBut,customLightContur,customLightBk, customLightScris
,customDarkButHover,customDarkBut,customDarkContur,customDarkBk,customDarkScris

,maroLightButHover=RGB(193, 186, 161),maroLightBut=RGB(215, 211, 191),maroLightContur=RGB(165, 157, 132),maroLightBk=RGB(171, 136, 109), maroLightScris=RGB(165, 157, 132)
,maroDarkButHover=RGB(116, 81, 45),maroDarkBut=RGB(175, 143, 111),maroDarkContur=RGB(248, 244, 225),maroDarkBk=RGB(84, 51, 16),maroDarkScris=RGB(248, 244, 225);


int FISIER;
int inFunctie=0;

void colorPicker(char titlu[250]) ;
void drawGradientSlider(int x, int y, int value, char label[300], char color);
int getSliderValue(int sliderX, int sliderY);
void drawColorPreview();

int selectedColor[3] = {0, 0, 0};
int currentLine = 0;

int zoom=0;
char pseudocod[1000][500];
bool secret=0;
bool peSecret=0;
int culSecret=RGB(255, 0, 0);

int main()
{
    srand(time(NULL));
    signal(SIGSEGV, signalHandler);

    initwindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "Fullscreen");
    setFullScreen();
    settextstyle(10,0,2);

    culBk=culBkLIGHT,culButon=culButonLIGHT,culButonHover=culButonHoverLIGHT,culContur=culConturLIGHT;
    menu();
    //getch();
    closegraph();
    return 0;
}

///           ZOOM


void ParcurgereDfsZoom(nod* p)
{
    if (!p) return;
    for (int i = 0; i < p->nrFii; i++)
    {
        setareCoord(p,p->fii[i]);
        ParcurgereDfsZoom(p->fii[i]);
    }
}

int dfs3(nod *p,int x,int y)
{
    if(!p) return 0;

    if (isEditing) return 1;

    int mouseX=mousex(),mouseY=mousey();
    if (strcmp(p->tip, "function")==0 || strcmp(p->tip, "procedure")==0)
    {
        if(p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + functionH)
        {
            inDiagrama=1;
                p->st=0.2*screen_width;
                p->dr=0.8*screen_width;
                ParcurgereDfsZoom(p);
                cleardevice();
                setfillstyle(SOLID_FILL,culBk);
                bar(0,0,screen_width,screen_height);
                dfs(p,0.2*screen_height);
                return 1;

        }
    }
    if (strcmp(p->tip, "for")==0 || strcmp(p->tip, "while")==0)
    {
        if((p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + loopH) ||
           (p->st <= mouseX && mouseX <= p->st+loopIndentare && p->nivel1 <= mouseY && mouseY <= p->nivel2))
        {
            inDiagrama=1;
            p->st=0.2*screen_width;
            p->dr=0.8*screen_width;
            ParcurgereDfsZoom(p);
            cleardevice();
            setfillstyle(SOLID_FILL,culBk);
            bar(0,0,screen_width,screen_height);
            dfs(p,0.2*screen_height);
            return 1;

        }
    }
    if (strcmp(p->tip, "if")==0)
    {
        if(p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + ifH)

        {
            inDiagrama=1;
            p->st=0.2*screen_width;
            p->dr=0.8*screen_width;
            ParcurgereDfsZoom(p);
            cleardevice();
            setfillstyle(SOLID_FILL,culBk);
            bar(0,0,screen_width,screen_height);
            dfs(p,0.2*screen_height);
            return 1;
        }
    }
    if (strcmp(p->tip, "until")==0)
    {
        if((p->st <= mouseX && mouseX <= p->dr && p->nivel2 - loopH <= mouseY && mouseY <= p->nivel2) ||
           (p->st <= mouseX && mouseX <= p->st+loopIndentare && p->nivel1 <= mouseY && mouseY <= p->nivel2))
        {
            inDiagrama=1;
            p->st=0.2*screen_width;
            p->dr=0.8*screen_width;
            ParcurgereDfsZoom(p);
            cleardevice();
            setfillstyle(SOLID_FILL,culBk);
            bar(0,0,screen_width,screen_height);
            dfs(p,0.2*screen_height);
            return 1;
        }
    }
    if (strcmp(p->tip, "instruction")==0)
        if(p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + instrH)
        {
            inDiagrama=1;
            p->st=0.2*screen_width;
            p->dr=0.8*screen_width;
            ParcurgereDfsZoom(p);
            cleardevice();
            setfillstyle(SOLID_FILL,culBk);
            bar(0,0,screen_width,screen_height);
            dfs(p,0.2*screen_height);
            return 1;
        }
    for (int i = 0; i < p->nrFii; i++)
        if (dfs3(p->fii[i],x,y))
            return 1;
    return 0;
}

void ParcurgereDfsPlus(nod* p)
{
    if (!p) return;
    for (int i = 0; i < p->nrFii; i++)
    {
        setareCoord(p,p->fii[i]);
        ParcurgereDfsPlus(p->fii[i]);
    }
}

void ParcurgereDfsMinus(nod* p)
{
    if (!p) return;
    cout<<endl;
    cout << "Tip: " << p->tip << ", Expresie: " << p->expresie << endl;
    for (int i = 0; i < p->nrFii; i++)
    {
        setareCoord(p,p->fii[i]);
        ParcurgereDfsMinus(p->fii[i]);
    }
}

///          EDITARE

void updatePseudocodeByLine(nod* p, const char* filename)
{
    char line[300],linie2[300];
    FILE *file=fopen(filename,"r");
    int cntlin=1,linCaut=p->linie;
    for(int i=0;i<linCaut;i++)
        fgets(line,sizeof(line),file);

    for(int i=0;i<p->adancime;i++)
       linie2[i]=' ';
    linie2[p->adancime] = '\0';
    if(strcmp(p->tip, "function")==0 || strcmp(p->tip, "procedure")==0 || strcmp(p->tip, "while")==0 || strcmp(p->tip, "for")==0 || strcmp(p->tip, "return")==0 || strcmp(p->tip, "if")==0)
       {
           strcat(linie2,p->tip);
            strcat(linie2," ");
       }
    strcat(linie2,p->expresie);
    if(strcmp(p->tip, "if")==0)
    {
        strcat(linie2," ");
        strcat(linie2, "then");
    }
    if(strcmp(p->tip, "while")==0)
    {
        strcat(linie2," ");
        strcat(linie2, "do");
    }
    //if(strcmp(p->tip, "until")==0)
    //{
//      strcat(linie2,";");
    //}
    if(strcmp(p->tip, "for")==0)
    {
        strcat(linie2," ");
        strcat(linie2, "do");
    }
    cout<<p->tip<<' '<<p->expresie<<' '<<p->linie;
    int lg=strlen(linie2);
    linie2[lg]='\n';
    linie2[lg+1]='\0';
    strcpy(pseudocod[linCaut],linie2);
    fclose(file);
    file=fopen(filename,"w");
    for(int i=0;i<currentLine;i++)
    {
        fputs(pseudocod[i],file);
    }
    fclose(file);

}

void bloc(int x, int y, const char* filename)
{
    for(int i=0;i<cntrad;i++)
        if(dfs2(rad[i],x,y,filename))
            return;
}

void CitesteSir(nod *p, int x, int y, char S[200], int cul)
{

    isEditing = true;

    char Enter = 13, BackSpace = 8, Escape = 27;
    char MultimeDeCaractereAcceptabile[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-><;=! )(&*%+-:,[]";
    char Sinitial[200], tasta;
    char Ss[200];

    // Initialize S with the existing expression
    strcpy(S, p->expresie);
    strcpy(Sinitial, S);

    // Reset text properties
    settextstyle(10, 0, 2);
    setfillstyle(SOLID_FILL, cul);
    setbkcolor(cul);
    settextjustify(1, 1);

    // Display initial expression
    strcpy(Ss, S);
    strcat(Ss, "_ ");
    outtextxy(x, y, Ss);

    // Clear input queue before starting
    while (kbhit()) getch();

    while (true) {
        tasta = getch();

        if (tasta == Enter || tasta == Escape) break;

        if (tasta == 0) {
            tasta = getch();
            Beep(200, 200);
        } else if (strchr(MultimeDeCaractereAcceptabile, tasta)) {
            bar(x - 200, y - 15, x + 200, y + 10); // Clear old string
            char tt2[2] = {tasta, '\0'};
            strcat(S, tt2);
            strcpy(Ss, S);
            strcat(Ss, "_ ");
            outtextxy(x, y, Ss);
        } else if (tasta == BackSpace && strlen(S) > 0) {
            bar(x - 200, y - 15, x + 200, y + 10); // Clear old string
            S[strlen(S) - 1] = '\0';
            strcpy(Ss, S);
            strcat(Ss, "_ ");
            outtextxy(x, y, Ss);
        } else {
            Beep(200, 200);
        }
    }

    if (tasta == Escape) {
        strcpy(S, Sinitial); // Restore original if Escape is pressed
    }
    strcpy(p->expresie, S); // Save final result

    // Clear input queue after editing
    while (kbhit()) getch();

    isEditing = false;
    return;
}

int dfs2(nod *p,int x,int y, const char* filename)
{
    if(isDarkTheme)
        setcolor(WHITE);
    else
        setcolor(BLACK);
    if (!p) return 0;
    if (isEditing) return 1;

    int mouseX = x,mouseY = y;

    if (strcmp(p->tip, "function")==0 || strcmp(p->tip, "procedure")==0)
    {
        if(p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + functionH)
        {
            CitesteSir(p, (p->st + p->dr) / 2, p->nivel1 + functionH / 2, p->expresie,culFunctie);
            updatePseudocodeByLine(p, filename);
            drawFunction(p->st, p->dr, p->nivel1, p->expresie);
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_RBUTTONDOWN);
            return 1;
        }
    }
    if (strcmp(p->tip, "for")==0 || strcmp(p->tip, "while")==0)
    {
        if((p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + loopH) ||
           (p->st <= mouseX && mouseX <= p->st+loopIndentare && p->nivel1 <= mouseY && mouseY <= p->nivel2))
        {
            CitesteSir(p, (p->st + p->dr) / 2, p->nivel1 + loopH / 2, p->expresie,culLoop);
            drawLoop(p->st, p->dr, p->nivel1, p->expresie);
            updatePseudocodeByLine(p, filename);
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_RBUTTONDOWN);
            return 1;
        }
    }
    if (strcmp(p->tip, "if")==0)
    {
        if(p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + ifH)

        {
            CitesteSir(p, (p->st + p->dr) / 2, p->nivel1 + ifH / 2, p->expresie,culIf);
            drawIf(p->st, p->dr, p->nivel1, p->expresie);
            updatePseudocodeByLine(p, filename);
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_RBUTTONDOWN);
            return 1;
        }
    }
    if (strcmp(p->tip, "until")==0)
    {
        if((p->st <= mouseX && mouseX <= p->dr && p->nivel2 - loopH <= mouseY && mouseY <= p->nivel2) ||
           (p->st <= mouseX && mouseX <= p->st+loopIndentare && p->nivel1 <= mouseY && mouseY <= p->nivel2))
        {
            CitesteSir(p, (p->st + p->dr) / 2, p->nivel2 - loopH / 2, p->expresie,culUntil);
            drawUntil(p->st, p->dr, p->nivel1,p->nivel2-loopH, p->expresie);
            //updatePseudocodeByLine(p, filename);
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_RBUTTONDOWN);
            return 1;
        }
    }
    if (strcmp(p->tip, "instruction")==0)
        if(p->st <= mouseX && mouseX <= p->dr && p->nivel1 <= mouseY && mouseY <= p->nivel1 + instrH)
    {
            CitesteSir(p, (p->st + p->dr) / 2, p->nivel1 + instrH / 2, p->expresie,culInstructiune);
            drawInstruction(p->st, p->dr, p->nivel1, p->expresie);
            updatePseudocodeByLine(p, filename);
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_RBUTTONDOWN);
            return 1;
    }
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_RBUTTONDOWN);
    for (int i = 0; i < p->nrFii; i++)
        if (dfs2(p->fii[i],x,y,filename))
            return 1;
    return 0;
}


///          PARCURGERE FISIER + DIAGRAMA

void f()
{
    strcpy(path,"fisier");
    char numar[10];
    sprintf(numar,"%d",FISIER);
    strcat(path,numar);
    strcat(path,".txt");
    FILE *fin=fopen(path,"r");

    if(FISIER==0)
    {
        selectFileMenu();
    }

    char s[256];
    cntrad=0;
    cntstiva=-1;
    nod *p;

    char *q;
    currentLine=0;
    while(fgets(s,256,fin))
    {
        strcpy(pseudocod[currentLine],s);
        int lg=strlen(s);
        s[lg-1]='\0';
        lg--;
        if(lg==0)
            inFunctie=0;
        else if(strstr(s,"BEGIN") || strstr(s,"END"))
        {
            currentLine++;
            continue;
        }
        else
        {
            p=new nod;
            setNod(p,s);
            p->linie=currentLine;
            if(strcmp(p->tip,"function")==0 || strcmp(p->tip,"procedure")==0)
            {
                p->linie=currentLine;
                inFunctie=1;
                rad[cntrad++]=p;
                cntstiva=-1;
                stiva[++cntstiva]=p;
                p->st=0.2*screen_width;
                p->dr=0.8*screen_width;

            }
            else if(strcmp(p->tip,"else")==0)
            {
                p->linie=currentLine;
                while(p->adancime <= stiva[cntstiva]->adancime && cntstiva >= -1)
                {
                    if(strcmp(stiva[cntstiva]->tip,"if")==0 && stiva[cntstiva]->adancime==p->adancime)
                        break;
                    cntstiva--;
                }
                setareCoord(stiva[cntstiva],p);
                stiva[cntstiva]->fii[stiva[cntstiva]->nrFii]=p;
                stiva[cntstiva]->nrFii++;
                stiva[++cntstiva]=p;
            }
            else if(strcmp(p->tip,"for")==0 || strcmp(p->tip,"while")==0 || strcmp(p->tip,"if")==0)
            {
                p->linie=currentLine;
                if(!inFunctie)
                {
                    inFunctie=1;
                    rad[cntrad++]=p;
                    cntstiva=-1;
                    stiva[++cntstiva]=p;
                    p->st=0.2*screen_width;
                    p->dr=0.8*screen_width;

                }
                else
                {
                    while(p->adancime <= stiva[cntstiva]->adancime)
                        cntstiva--;
                    setareCoord(stiva[cntstiva],p);
                    stiva[cntstiva]->fii[stiva[cntstiva]->nrFii]=p;
                    stiva[cntstiva]->nrFii++;
                    stiva[++cntstiva]=p;
                }

            }
            else if(strcmp(p->tip,"repeat")==0)
            {

                if(!inFunctie)
                {
                    inFunctie=1;
                    rad[cntrad++]=p;
                    cntstiva=-1;
                    stiva[++cntstiva]=p;
                    p->st=0.2*screen_width;
                    p->dr=0.8*screen_width;

                }
                else
                {
                    while(p->adancime <= stiva[cntstiva]->adancime)
                        cntstiva--;
                    setareCoord(stiva[cntstiva],p);
                    stiva[cntstiva]->fii[stiva[cntstiva]->nrFii]=p;
                    stiva[cntstiva]->nrFii++;
                    stiva[++cntstiva]=p;
                }
            }
            else if(strcmp(p->tip,"until")==0) ///deja am afisat tot ce era inclus in el, deci nu trebuie bagat in stiva
            {
                p->linie=stiva[cntstiva]->linie;
                while(p->adancime <= stiva[cntstiva]->adancime)
                {
                    if(strcmp(stiva[cntstiva]->tip,"repeat")==0 && stiva[cntstiva]->adancime==p->adancime)
                        break;
                    cntstiva--;
                }
                strcpy(stiva[cntstiva]->expresie,p->expresie);
                strcpy(stiva[cntstiva]->tip,p->tip);
                stiva[cntstiva]->linie=p->linie;
                //cout<<p->linie;
                //p->linie=stiva[cntstiva]->linie;
                delete p;
            }
            else if(strcmp(p->tip,"return")==0 || strcmp(p->tip,"break")==0)
            {
                p->linie=currentLine;
                while(p->adancime <= stiva[cntstiva]->adancime)
                    cntstiva--;
                setareCoord(stiva[cntstiva],p);
                stiva[cntstiva]->fii[stiva[cntstiva]->nrFii]=p;
                stiva[cntstiva]->nrFii++;
            }
            else ///instructiune
            {
                p->linie=currentLine;
                if(!inFunctie)
                {
                    inFunctie=1;
                    rad[cntrad++]=p;
                    cntstiva=-1;
                    stiva[++cntstiva]=p;
                    p->st=0.2*screen_width;
                    p->dr=0.8*screen_width;

                }
                else
                {
                    while(p->adancime <= stiva[cntstiva]->adancime)
                        cntstiva--;
                    setareCoord(stiva[cntstiva],p);
                    stiva[cntstiva]->fii[stiva[cntstiva]->nrFii]=p;
                    stiva[cntstiva]->nrFii++;
                }
            }
            currentLine++;
        }
    }
}

void setNod(nod *p, char *s)
{
    int i=0,dr,st,lg=strlen(s);
    while(s[i]==' ' && i<lg) i++; strcpy(s,s+i); ///scot spatii inceput

    p->adancime=i;
    for(i=strlen(s)-1;s[i]==' ';i--); s[i+1]='\0'; ///scot spatii final

    p->nivel1=p->nivel2=0;
    p->nrFii=0;

    if(instructiune(s,"function"))
    {
        strcpy(p->tip,"function");
        dr=strlen(s)-1;
        for(i=dr;s[i]==' ';i--);
        s[i+1]='\0';
        for(i=8;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"procedure"))
    {
        strcpy(p->tip,"procedure");
        dr=strlen(s)-1;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=9;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"if"))
    {
        strcpy(p->tip,"if");
        dr=strlen(s)-5;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=2;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"else"))
    {
        strcpy(p->tip,"else");
        p->expresie[0]='\0';
        return;
    }
    if(instructiune(s,"while"))
    {
        strcpy(p->tip,"while");
        dr=strlen(s)-3;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=5;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"for"))
    {
        strcpy(p->tip,"for");
        dr=strlen(s)-3;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=3;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"until"))
    {
        strcpy(p->tip,"until");
        dr=strlen(s)-2;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=5;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"repeat"))
    {
        strcpy(p->tip,"repeat");
        p->expresie[0]='\0';
        return;
    }
    if(instructiune(s,"return"))
    {
        strcpy(p->tip,"return");
        dr=strlen(s)-1;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=6;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    if(instructiune(s,"break"))
    {
        strcpy(p->tip,"break");
        dr=strlen(s)-1;
        for(i=dr;s[i]==' ';i--);
            s[i+1]='\0';
        for(i=5;s[i]==' ';i++);
        strcpy(p->expresie,s+i);
        return;
    }
    strcpy(p->tip,"instruction");
    strcpy(p->expresie,s);
}

bool isButtonClicked(int x1, int y1, int x2, int y2, int clickX, int clickY)
{
    return (clickX >= x1 && clickX <= x2 && clickY >= y1 && clickY <= y2);
}

void scroll()
{
    cleardevice();
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    if(isDarkTheme)
        setcolor(WHITE);
    else
        setcolor(BLACK);
    drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
    drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
    drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
    drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
    drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
    drawButon(0.935*screen_width,0.54*screen_height,0.955*screen_width,0.505*screen_height,"-",culButon);

    int i,nivel;
    int scrollH=0.1*screen_height;

    nivel=scrollH;
    diagrama(nivel);
    int x,y,nrRad;
    bool peButon1=0,peButon2=0,peButon3=0,peButon4=0,zoom=0,peButon5=0,peButon6=0,peButon7=0;

    while(true)
    {
        int mX=mousex(),mY=mousey();
        if(mX>=0.05*screen_width && mX<=0.1*screen_width && mY>=0.4*screen_height && mY<=0.475*screen_height) ///sus
            {
                if(!peButon1)
                {
                    drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButonHover);
                    peButon1=1;
                }
            }
            else
            {
                if(peButon1)
                {
                    drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                    peButon1=0;
                }
            }

            if(mX>=0.05*screen_width && mX<=0.1*screen_width && mY>=0.525*screen_height && mY<=0.6*screen_height) ///jos
            {
                if(!peButon2)
                {
                    drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButonHover);
                    peButon2=1;
                }
            }
            else
            {
                if(peButon2)
                {
                    drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                    peButon2=0;
                }
            }

            if(mX>=0.92*screen_width && mX<=0.95*screen_width && mY>=0.05*screen_height && mY<=0.1*screen_height)
            {
                if(!peButon3)
                {
                    drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButonHover);
                    peButon3=1;
                }
            }
            else
            {
                if(peButon3)
                {
                    drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                    peButon3=0;
                }
            }

            if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height)
            {
                if(!peButon4)
                {
                    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                    peButon4=1;
                }
            }
            else
            {
                if(peButon4)
                {
                    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                    peButon4=0;
                }
            }
            if(mX>=0.935*screen_width && mX<=0.955*screen_width && mY>=0.505*screen_height && mY<=0.54*screen_height) ///hover la minus sa faca x
            {
                if(!peButon5)
                {
                    if(instrH<=0.031*screen_height)
                    {
                        setcolor(RED);
                        setlinestyle(SOLID_LINE,SOLID_FILL,3);
                        line(0.935*screen_width+1,0.54*screen_height-1,0.955*screen_width-1,0.505*screen_height+1);
                        line(0.955*screen_width-1,0.54*screen_height-1,0.935*screen_width+1,0.505*screen_height+1);
                    }
                    else
                        drawButon(0.935*screen_width,0.505*screen_height,0.955*screen_width,0.54*screen_height,"-",culButonHover);
                    peButon5=1;
                }
            }
            else
            {
                if(peButon5)
                {
                    drawButon(0.935*screen_width,0.505*screen_height,0.955*screen_width,0.54*screen_height,"-",culButon);
                    peButon5=0;
                }
            }
            if(mX>=0.935*screen_width && mX<=0.955*screen_width && mY>=0.46*screen_height && mY<=0.495*screen_height) /// +
            {
                if(!peButon6)
                {
                    drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButonHover);
                    peButon6=1;
                }
            }
            else
            {
                if(peButon6)
                {
                    drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                    peButon6=0;
                }
            }
            if(mX>=0.875*screen_width && mX<=0.925*screen_width && mY>=0.45*screen_height && mY<=0.55*screen_height) /// capture
            {
                if(!peButon7)
                {
                   drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButonHover);
                   setcolor(culBk);
                   outtextxy(0.9*screen_width,0.57*screen_height,"Doar in folder!");
                    peButon7=1;
                }
            }
            else
            {
                if(peButon7)
                {
                    drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                    setfillstyle(SOLID_FILL,culBk);
                    bar(0.85*screen_width,0.551*screen_height,0.95*screen_width,0.6*screen_height);
                    peButon7=0;
                }
            }
            if(ismouseclick(WM_LBUTTONDOWN)) ///editare
            {
                    int mouseX,mouseY;
                    getmouseclick(WM_LBUTTONDOWN,mouseX,mouseY);
                    clearmouseclick(WM_LBUTTONDOWN);

                    bloc(mouseX,mouseY,path);
                    if(!isEditing)
                    {
                        if(isButtonClicked(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,mouseX,mouseY))/// //////SCROLL SUS BUTON
                         {
                            if(scrollH<0)
                            {
                                cleardevice();
                                setfillstyle(SOLID_FILL,culBk);
                                bar(0,0,screen_width,screen_height);
                                scrollH+=0.3*screen_height;
                                nivel=scrollH;
                                diagrama(nivel);
                                drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                                drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                                drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                                drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                                drawButon(0.935*screen_width,0.54*screen_height,0.955*screen_width,0.505*screen_height,"-",culButon);
                                drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                                scrollSus=scrollJos=0;
                            }
                            else
                            {

                            }

                        }
                        if(isButtonClicked(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,mouseX,mouseY))/// //////SCROLL JOS BUTON
                        {
                            //if(scrollH<=nivelFinal)
                            //{
                                cleardevice();
                                setfillstyle(SOLID_FILL,culBk);
                                bar(0,0,screen_width,screen_height);
                                scrollH-=0.3*screen_height;
                                nivel=scrollH;
                                diagrama(nivel);
                                drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                                drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                                drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                                drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                                drawButon(0.935*screen_width,0.54*screen_height,0.955*screen_width,0.505*screen_height,"-",culButon);
                                drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                                scrollSus=scrollJos=0;
                            //}

                        }
                        if(isButtonClicked(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,mouseX,mouseY)) ///plus
                        {
                            for(int i=0;i<cntrad;i++)
                            {
                                ParcurgereDfsPlus(rad[i]);
                            }
                            instrH+=0.01*screen_height;
                            returnH+=0.01*screen_height;
                            loopH+=0.01*screen_height;
                            ifH+=0.01*screen_height;
                            switchH+=0.01*screen_height;

                            separareH+=0.01*screen_height;
                            loopIndentare+=0.01*screen_height;
                            functionH+=0.01*screen_height;
                            scrollH+=0.01*screen_height;

                            for(int i=0;i<cntrad;i++)
                            {
                                ParcurgereDfsPlus(rad[i]);
                            }
                            cleardevice();
                            setfillstyle(SOLID_FILL,culBk);
                            bar(0,0,screen_width,screen_height);
                            nivel=0.1*screen_height;
                            diagrama(nivel);
                            drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                            drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                            drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                            drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                            drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                            drawButon(0.935*screen_width,0.505*screen_height,0.955*screen_width,0.54*screen_height,"-",culButon);
                            drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                        }
                        if(isButtonClicked(0.935*screen_width,0.505*screen_height,0.955*screen_width,0.54*screen_height,mouseX,mouseY) && instrH>0.031*screen_height) ///minus
                        {

                            if(!(instrH<0.03*screen_height))
                            {

                                for(int i=0;i<cntrad;i++)
                                {
                                    ParcurgereDfsMinus(rad[i]);
                                }
                                instrH-=0.01*screen_height;
                                returnH-=0.01*screen_height;
                                loopH-=0.01*screen_height;
                                ifH-=0.01*screen_height;
                                switchH-=0.01*screen_height;

                                separareH-=0.01*screen_height;
                                loopIndentare-=0.01*screen_height;

                                functionH-=0.01*screen_height;
                                scrollH-=0.01*screen_height;

                                for(int i=0;i<cntrad;i++)
                                {
                                    ParcurgereDfsMinus(rad[i]);
                                }
                                cleardevice();
                                f();
                                setfillstyle(SOLID_FILL,culBk);
                                bar(0,0,screen_width,screen_height);
                                nivel=0.1*screen_height;
                                diagrama(nivel);
                                drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                                drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                                drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                                drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                                drawButon(0.935*screen_width,0.54*screen_height,0.955*screen_width,0.505*screen_height,"-",culButon);
                                drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                            }

                        }
                        //x>=0.9*screen_width && y>=0.05*screen_height && x<=0.95*screen_width && y<=0.1*screen_height
                        if(isButtonClicked(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,mouseX,mouseY)) /// ////// X
                        {
                            settextjustify(1,1);
                            setfillstyle(SOLID_FILL,BLACK);
                            bar(0,0,screen_width,screen_height);
                            setbkcolor(BLACK);
                            outtextxy(screen_width/2,screen_height/2,"Se inchide...");
                            delay(300);
                            closegraph();
                            exit(0);
                        }
                        if(isButtonClicked(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,mouseX,mouseY)) /// ////// BACK
                        {
                            /// /////////// la fel ca in main
                            menu();
                        }
                        if(isButtonClicked(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,mouseX,mouseY)) /// POZA
                            writeimagefile(NULL, 0.15*screen_width, 0, 0.85*screen_width, screen_height);

                    }

                    /// ///////////////////
            }
            else if (ismouseclick(WM_RBUTTONDOWN)) ///         DACA E PE ZOOM
            {
                int mX,mY,peButon=0;
                getmouseclick(WM_RBUTTONDOWN, mX, mY);
                clearmouseclick(WM_RBUTTONDOWN);
                for(int i=0;i<cntrad;i++)
                {
                    dfs3(rad[i],mX,mY);
                }
                if(inDiagrama && zoom==0)
                {
                    zoom=1;
                    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                    drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                    drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                    bool peButon1=0,peButon2=0,peButon3=0,peButon4=0,zoom=0,peButon5=0,peButon6=0,peButon7=0;
                    while(true)
                    {
                        int mX=mousex(),mY=mousey();
                        if(mX>=0.92*screen_width && mX<=0.95*screen_width && mY>=0.05*screen_height && mY<=0.1*screen_height) ///  X
                        {
                            if(!peButon3)
                            {
                                drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButonHover);
                                peButon3=1;
                            }
                        }
                        else
                        {
                            if(peButon3)
                            {
                                drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                                peButon3=0;
                            }
                        }

                        if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height) /// BACK
                        {
                            if(!peButon4)
                            {
                                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                                peButon4=1;
                            }
                        }
                        else
                        {
                            if(peButon4)
                            {
                                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                                peButon4=0;
                            }
                        }

                        if(mX>=0.875*screen_width && mX<=0.925*screen_width && mY>=0.45*screen_height && mY<=0.55*screen_height)
                        {
                            if(!peButon7)
                            {
                               drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButonHover);
                                peButon7=1;
                            }
                        }
                        else
                        {
                            if(peButon7)
                            {
                                drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                                peButon7=0;
                            }
                        }

                        if(ismouseclick(WM_LBUTTONDOWN))
                        {
                            getmouseclick(WM_LBUTTONDOWN,x,y);
                            if(isButtonClicked(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,x,y)) /// ////// X
                            {
                                settextjustify(1,1);
                                setfillstyle(SOLID_FILL,BLACK);
                                bar(0,0,screen_width,screen_height);
                                setbkcolor(BLACK);
                                outtextxy(screen_width/2,screen_height/2,"Se inchide...");
                                delay(300);
                                closegraph();
                                exit(0);
                            }
                            if(isButtonClicked(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,x,y)) /// POZA
                            {
                                writeimagefile(NULL, 0.15*screen_width, 0, 0.85*screen_width, screen_height);
                            }
                            if(x>=0.9*screen_width && y>=0.9*screen_height && x<=0.95*screen_width && y<=0.95*screen_height) ///back
                            {
                                inDiagrama=0;
                                clearmouseclick(WM_RBUTTONDOWN);
                                f();
                                scroll();
                            }
                        }
                    }
                    zoom=0;
                }

            }
    }
}

bool instructiune(char *s, char instr[])
{
    int i=strlen(instr);
     if (!(s[i] == ' ' || s[i] == '\0')) return 0;
    for(i=i-1;i>=0;i--)
        if(s[i]!=instr[i])
            return 0;
    return 1;
}

void setareCoord(nod *parinte, nod *fiu)
{
    int st,dr;
    st=parinte->st;
    dr=parinte->dr;

    if(strcmp(parinte->tip,"while")==0 || strcmp(parinte->tip,"for")==0 || strcmp(parinte->tip,"repeat")==0)
    {
        fiu->st=parinte->st+loopIndentare;
        fiu->dr=parinte->dr;
        return;
    }
    if(strcmp(parinte->tip,"function")==0 || strcmp(parinte->tip,"procedure")==0 || strcmp(parinte->tip,"else")==0 || strcmp(parinte->tip,"return")==0 || strcmp(parinte->tip,"break")==0)
    {
        fiu->st=parinte->st;
        fiu->dr=parinte->dr;
        return;
    }
    if(strcmp(parinte->tip,"if")==0)
    {
        if(strcmp(fiu->tip,"else")!=0) ///cazul true
        {
            fiu->st=parinte->st;
            fiu->dr=(parinte->st+parinte->dr)/2;
            return;
        }
        else ///cazul false
        {
            fiu->st=(parinte->st+parinte->dr)/2;
            fiu->dr=parinte->dr;
            return;
        }
    }
}

void diagrama(int &nivel) ///face toata diagrama
{
    int i;
    int x1=0.2*screen_width,x2=0.7*screen_width;
    if(isDarkTheme)
        setcolor(WHITE);
    else
        setcolor(BLACK);
    for(i=0;i<cntrad;i++)
    {
        line(x1,nivel,x2,nivel);
        nivel=dfs(rad[i],nivel);
        nivel=nivel+separareH;
    }
}

void blocDiagrama(nod*p,int &nivel)
{
    int i;
    int x1=0.2*screen_width,x2=0.7*screen_width;
    if(isDarkTheme)
        setcolor(WHITE);
    else
        setcolor(BLACK);
    line(x1,nivel,x2,nivel);
    nivel=dfs(p,nivel);
}

int dfs(nod *p,int nivel) ///face doar o functie/procedure
{
    int i,nou=nivel;
    char q[200];
    if(isDarkTheme)
        setcolor(WHITE);
    else
        setcolor(BLACK);
    if(p==NULL)
        return nivel;
    p->nivel1=nivel;
    if(strcmp(p->tip,"instruction")==0)
        nou=drawInstruction(p->st,p->dr,nou,p->expresie);
    else
    {
        strcpy(q,p->expresie);
        int lg=strlen(p->expresie);
        q[lg]='\0';
        if(strcmp(p->tip,"while")==0 || strcmp(p->tip,"for")==0)
            nou=drawLoop(p->st,p->dr,nou,q);
        else if(strcmp(p->tip,"if")==0)
            nou=drawIf(p->st,p->dr,nou,q);
        else if(strcmp(p->tip,"function")==0 || strcmp(p->tip,"procedure")==0)
            nou=drawFunction(p->st,p->dr,nou,q);
        else if(strcmp(p->tip,"return")==0)
        {
            nou=drawReturn(p->st,p->dr,nou,q);
            endReturn(p->st,p->dr,nivel,nou);
        }
        else if(strcmp(p->tip,"break")==0)
        {
            nou=drawBreak(p->st,p->dr,nou,q);
            endBreak(p->st,p->dr,nivel,nou);
        }
    }
    if(p->nrFii==0) ///nu are fii, deci am terminat cu el
    {
        p->nivel2=nou;
        return nou;
    }
    for(i=0;i<(p->nrFii) -1; i++)
        nou=dfs(p->fii[i],nou);
    if(strcmp(p->fii[i]->tip, "else")==0)
    {
        int nou2=nou;
        nou=max(dfs(p->fii[i],nivel+ifH), nou); ///pentru if cand else-ul este mai lung sau mai scurt, alegem dimensiunea mai mare
        if(dfs(p->fii[i],nivel+ifH)>nou2)
        {
            setbkcolor(culIf);
            setfillstyle(WIDE_DOT_FILL,culPunctulete);
            bar(p->st,nou2,(p->dr+p->st)/2,nou);
            rectangle(p->st,nou2,(p->dr+p->st)/2,nou);
        }
    }
    else if(strcmp(p->tip,"if")==0) ///daca nu am else, desenez cu ceva
    {
        nou=dfs(p->fii[i],nou);
        setbkcolor(culIf);
        setfillstyle(WIDE_DOT_FILL,culPunctulete);
        bar((p->st+p->dr)/2,nivel+ifH,p->dr,nou);
        rectangle((p->st+p->dr)/2,nivel+ifH,p->dr,nou);
    }
    else
        nou=dfs(p->fii[i],nou);

    endInstruction(p,nivel,nou);
    if(strcmp(p->tip,"until") == 0)
        nou=drawUntil(p->st,p->dr,nivel,nou,q);
    p->nivel2=nou;
    return nou;
}

///            MENIURI

void menuFile()
{
    int cul=RGB(137, 168, 178);
    char numar[10];

    cleardevice();
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    drawButon(0.05*screen_width,0.3*screen_height,0.3*screen_width,0.375*screen_height,"1",culButon);
    drawButon(0.2*screen_width,0.4*screen_height,0.45*screen_width,0.475*screen_height,"2",culButon);
    drawButon(0.35*screen_width,0.5*screen_height,0.6*screen_width,0.575*screen_height,"3",culButon);
    drawButon(0.5*screen_width,0.6*screen_height,0.75*screen_width,0.675*screen_height,"4",culButon);
    drawButon(0.65*screen_width,0.7*screen_height,0.9*screen_width,0.775*screen_height,"5",culButon);
    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);

    settextstyle(10,0,7);
    setbkcolor(culBk);
    outtextxy(0.5*screen_width,0.15*screen_height,"Alege fisierul de mai jos...");
    settextstyle(10,0,2);

    if(FISIER==0)
        strcpy(selectie,"NICIUN FISIER SELECTAT");
    drawButon(0.4*screen_width,0.91 *screen_height,0.6*screen_width,0.95*screen_height,selectie,culButon);


    int x,y;
    bool peButon1=0,peButon2=0,peButon3=0,peButon4=0,peButon5=0,peButon6=0;
    while(true)
    {
        int mX=mousex(),mY=mousey();
        /// ///// buton 1
        if(mX>=0.05*screen_width && mX<=0.3*screen_width && mY>=0.3*screen_height && mY<=0.375*screen_height)
        {
            if(!peButon1)
            {
                drawButon(0.05*screen_width,0.3*screen_height,0.3*screen_width,0.375*screen_height,"1",culButonHover);
                peButon1=1;
            }
        }
        else
        {
            if(peButon1)
            {
                drawButon(0.05*screen_width,0.3*screen_height,0.3*screen_width,0.375*screen_height,"1",culButon);
                peButon1=0;
            }
        }
        /// ///// buton 2
        if(mX>=0.2*screen_width && mX<=0.45*screen_width && mY>=0.4*screen_height && mY<=0.475*screen_height)
        {
            if(!peButon2)
            {
                drawButon(0.2*screen_width,0.4*screen_height,0.45*screen_width,0.475*screen_height,"2",culButonHover);
                peButon2=1;
            }
        }
        else
        {
            if(peButon2)
            {
                drawButon(0.2*screen_width,0.4*screen_height,0.45*screen_width,0.475*screen_height,"2",culButon);
                peButon2=0;
            }
        }
        /// ///// buton 3
        if(mX>=0.35*screen_width && mX<=0.6*screen_width && mY>=0.5*screen_height && mY<=0.575*screen_height)
        {
            if(!peButon3)
            {
                drawButon(0.35*screen_width,0.5*screen_height,0.6*screen_width,0.575*screen_height,"3",culButonHover);
                peButon3=1;
            }
        }
        else
        {
            if(peButon3)
            {
                drawButon(0.35*screen_width,0.5*screen_height,0.6*screen_width,0.575*screen_height,"3",culButon);
                peButon3=0;
            }
        }
        /// ///// buton 4
        if(mX>=0.5*screen_width && mX<=0.75*screen_width && mY>=0.6*screen_height && mY<=0.675*screen_height)
        {
            if(!peButon4)
            {
                drawButon(0.5*screen_width,0.6*screen_height,0.75*screen_width,0.675*screen_height,"4",culButonHover);
                peButon4=1;
            }
        }
        else
        {
            if(peButon4)
            {
                drawButon(0.5*screen_width,0.6*screen_height,0.75*screen_width,0.675*screen_height,"4",culButon);
                peButon4=0;
            }
        }
        /// ///// buton 5
        if(mX>=0.65*screen_width && mX<=0.9*screen_width && mY>=0.7*screen_height && mY<=0.775*screen_height)
        {
            if(!peButon5)
            {
                drawButon(0.65*screen_width,0.7*screen_height,0.9*screen_width,0.775*screen_height,"5",culButonHover);
                peButon5=1;
            }
        }
        else
        {
            if(peButon5)
            {
                drawButon(0.65*screen_width,0.7*screen_height,0.9*screen_width,0.775*screen_height,"5",culButon);
                peButon5=0;
            }
        }

        /// ///// buton 6 BACK
        if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height)
        {
            if(!peButon6)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                peButon6=1;
            }
        }
        else
        {
            if(peButon6)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                peButon6=0;
            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if(x>=0.05*screen_width && x<=0.3*screen_width && y>=0.3*screen_height && y<=0.375*screen_height)
                FISIER=1;
            if(x>=0.2*screen_width && x<=0.45*screen_width && y>=0.4*screen_height && y<=0.475*screen_height)
                FISIER=2;
            if(x>=0.35*screen_width && x<=0.6*screen_width && y>=0.5*screen_height && y<=0.575*screen_height)
                FISIER=3;
            if(x>=0.5*screen_width && x<=0.75*screen_width && y>=0.6*screen_height && y<=0.675*screen_height)
                FISIER=4;
            if(x>=0.65*screen_width && x<=0.9*screen_width && y>=0.7*screen_height && y<=0.775*screen_height)
                FISIER=5;

            /// ////////////////////////////   fisier selectat jos
            setbkcolor(cul);

            sprintf(numar,"%d",FISIER);
            strcpy(selectie,"FISIER SELECTAT: ");
            strcat(selectie,numar);
            if(FISIER)
                drawButon(0.4*screen_width,0.91 *screen_height,0.6*screen_width,0.95*screen_height,selectie,culButon);
            /// ////////////////////////////

            if(x>=0.9*screen_width && x<=0.95*screen_width && y>=0.9*screen_height && y<=0.95*screen_height)
            {
                clearmouseclick(WM_RBUTTONDOWN);
                menu();
            }
            clearmouseclick(WM_RBUTTONDOWN);
        }
    }
}

void helpMenu()
{
    cleardevice();
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    settextstyle(10,0,4);
    setbkcolor(culBk);
    outtextxy(screen_width/2,0.1*screen_height,"Reprezentare comenzi");
    settextstyle(10,0,2);

    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);

    readimagefile("poza.jpg",0.125*screen_width,0.125*screen_height,0.875*screen_width,0.875*screen_height);
    int x,y;
    bool peButon1=0, peButon2=0;
    while(true)
    {
        int mX=mousex(),mY=mousey();

        if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height)
        {
            if(!peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                peButon2=1;
            }
        }
        else
        {
            if(peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                peButon2=0;
            }
        }

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            menu();
        }

    }
}

void themeMenu()
{
    cleardevice();
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    settextstyle(10,0,4);
    setbkcolor(culBk);
    outtextxy(screen_width/2,0.1*screen_height,"Alege culoarea");
    settextstyle(10,0,2);

    drawThemeButton();
    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);

    ///rosu
    setfillstyle(SOLID_FILL,rosLightBk);
    bar(0.1*screen_width,0.2*screen_height,0.15*screen_width,0.3*screen_height);
    setfillstyle(SOLID_FILL,rosDarkBut);
    bar(0.15*screen_width,0.2*screen_height,0.2*screen_width,0.3*screen_height);
    setfillstyle(SOLID_FILL,rosDarkBk);
    bar(0.2*screen_width,0.2*screen_height,0.25*screen_width,0.3*screen_height);
    setlinestyle(SOLID_LINE,SOLID_FILL,3);
    rectangle(0.1*screen_width,0.2*screen_height,0.25*screen_width,0.3*screen_height);

    ///albastru
    setfillstyle(SOLID_FILL,blueLightBk);
    bar(0.1*screen_width,0.35*screen_height,0.15*screen_width,0.45*screen_height);
    setfillstyle(SOLID_FILL,blueDarkBut);
    bar(0.15*screen_width,0.35*screen_height,0.2*screen_width,0.45*screen_height);
    setfillstyle(SOLID_FILL,blueDarkBk);
    bar(0.2*screen_width,0.35*screen_height,0.25*screen_width,0.45*screen_height);
    setlinestyle(SOLID_LINE,SOLID_FILL,3);
    rectangle(0.1*screen_width,0.35*screen_height,0.25*screen_width,0.45*screen_height);

    ///verde
    setfillstyle(SOLID_FILL,verdeLightBk);
    bar(0.1*screen_width,0.5*screen_height,0.15*screen_width,0.6*screen_height);
    setfillstyle(SOLID_FILL,verdeDarkBut);
    bar(0.15*screen_width,0.5*screen_height,0.2*screen_width,0.6*screen_height);
    setfillstyle(SOLID_FILL,verdeDarkBk);
    bar(0.2*screen_width,0.5*screen_height,0.25*screen_width,0.6*screen_height);
    setlinestyle(SOLID_LINE,SOLID_FILL,3);
    rectangle(0.1*screen_width,0.5*screen_height,0.25*screen_width,0.6*screen_height);

    ///portocaliu
    setfillstyle(SOLID_FILL,orangeLightBk);
    bar(0.1*screen_width,0.65*screen_height,0.15*screen_width,0.75*screen_height);
    setfillstyle(SOLID_FILL,orangeDarkBut);
    bar(0.15*screen_width,0.65*screen_height,0.2*screen_width,0.75*screen_height);
    setfillstyle(SOLID_FILL,orangeDarkBk);
    bar(0.2*screen_width,0.65*screen_height,0.25*screen_width,0.75*screen_height);
    setlinestyle(SOLID_LINE,SOLID_FILL,3);
    rectangle(0.1*screen_width,0.65*screen_height,0.25*screen_width,0.75*screen_height);

    ///roz
    setfillstyle(SOLID_FILL,rozLightBk);
    bar(0.35*screen_width,0.2*screen_height,0.4*screen_width,0.3*screen_height);
    setfillstyle(SOLID_FILL,rozDarkBut);
    bar(0.4*screen_width,0.2*screen_height,0.45*screen_width,0.3*screen_height);
    setfillstyle(SOLID_FILL,rozDarkBk);
    bar(0.45*screen_width,0.2*screen_height,0.5*screen_width,0.3*screen_height);
    setlinestyle(SOLID_LINE,SOLID_FILL,3);
    rectangle(0.35*screen_width,0.2*screen_height,0.5*screen_width,0.3*screen_height);

    ///maro
    setfillstyle(SOLID_FILL,maroLightBk);
    bar(0.35*screen_width,0.35*screen_height,0.4*screen_width,0.45*screen_height);
    setfillstyle(SOLID_FILL,maroDarkBut);
    bar(0.4*screen_width,0.35*screen_height,0.45*screen_width,0.45*screen_height);
    setfillstyle(SOLID_FILL,maroDarkBk);
    bar(0.45*screen_width,0.35*screen_height,0.5*screen_width,0.45*screen_height);
    setlinestyle(SOLID_LINE,SOLID_FILL,3);
    rectangle(0.35*screen_width,0.35*screen_height,0.5*screen_width,0.45*screen_height);

    ///custom
    if(customLightBk && customDarkBut && customDarkBk)
    {
        setfillstyle(SOLID_FILL,customLightBk);
        bar(0.35*screen_width,0.5*screen_height,0.4*screen_width,0.6*screen_height);
        setfillstyle(SOLID_FILL,customDarkBut);
        bar(0.4*screen_width,0.5*screen_height,0.45*screen_width,0.6*screen_height);
        setfillstyle(SOLID_FILL,customDarkBk);
        bar(0.45*screen_width,0.5*screen_height,0.5*screen_width,0.6*screen_height);
        setlinestyle(SOLID_LINE,SOLID_FILL,3);
        rectangle(0.35*screen_width,0.5*screen_height,0.5*screen_width,0.6*screen_height);
    }
    else
    {
        setbkcolor(culBk);
        settextstyle(10,0,5);
        outtextxy(0.425*screen_width,0.56*screen_height,"+");
        settextstyle(10,0,2);
        setlinestyle(SOLID_LINE,SOLID_FILL,3);
        rectangle(0.35*screen_width,0.5*screen_height,0.5*screen_width,0.6*screen_height);
    }


    int x,y;
    bool peButon2=0;
    while(true)
    {
        int mX=mousex(),mY=mousey();

        if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height)
        {
            if(!peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                peButon2=1;
            }
        }
        else
        {
            if(peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                peButon2=0;
            }
        }

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);

            if(x>=0.05*screen_width && y>=0.05*screen_height && x<=0.15*screen_width && y<=0.15*screen_height) /// DARK THEME
            {
                if(isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;

                    culIf=culIfLIGHT;
                    culBreak=culBreakLIGHT;
                    culFunctie=culFunctieLIGHT;
                    culLoop=culLoopLIGHT;
                    culInstructiune=culInstructiuneLIGHT;
                    culReturn=culReturnLIGHT;
                    culUntil=culUntilLIGHT;
                    culProcedura=culProceduraLIGHT;
                    culSwitch=culSwitchLIGHT;
                    culPunctulete=culPunctuleteLIGHT;
                }
                else if(!isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                    culIf=culIfDARK;
                    culBreak=culBreakDARK;
                    culFunctie=culFunctieDARK;
                    culLoop=culLoopDARK;
                    culInstructiune=culInstructiuneDARK;
                    culReturn=culReturnDARK;
                    culUntil=culUntilDARK;
                    culProcedura=culProceduraDARK;
                    culSwitch=culSwitchDARK;
                    culPunctulete=culPunctuleteDARK;
                }
                isDarkTheme=!isDarkTheme;
                cleardevice();
                themeMenu();
            }
            if(x>=0.9*screen_width && x<=0.95*screen_width && y>=0.9*screen_height && y<=0.95*screen_height) /// BACK
            {
                menu();
            }
            if(x>=0.1*screen_width && x<=0.25*screen_width && y>=0.2*screen_height && y<=0.3*screen_height) ///rosu
            {
                culBkLIGHT= rosLightBk;
                culBkDARK= rosDarkBk;
                culButonLIGHT=rosLightBut;
                culButonDARK=rosDarkBut;
                culButonHoverLIGHT=rosLightButHover;
                culButonHoverDARK=rosDarkButHover;
                culConturLIGHT=rosLightContur;
                culConturDARK=rosDarkContur;
                culScrisDARK=rosDarkScris;
                culScrisLIGHT=rosLightScris;
                if(!isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;
                }
                else if(isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                }
                themeMenu();
            }
            if(x>=0.1*screen_width && x<=0.25*screen_width && y>=0.35*screen_height && y<=0.45*screen_height) ///albastru
            {
                culBkLIGHT= blueLightBk;
                culBkDARK= blueDarkBk;
                culButonLIGHT=blueLightBut;
                culButonDARK=blueDarkBut;
                culButonHoverLIGHT=blueLightButHover;
                culButonHoverDARK=blueDarkButHover;
                culConturLIGHT=blueLightContur;
                culConturDARK=blueDarkContur;
                culScrisDARK=blueDarkScris;
                culScrisLIGHT=blueLightScris;
                if(!isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;
                }
                else if(isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                }
                themeMenu();
            }
            if(x>=0.1*screen_width && x<=0.25*screen_width && y>=0.5*screen_height && y<=0.6*screen_height) ///albastru
            {
                culBkLIGHT= verdeLightBk;
                culBkDARK= verdeDarkBk;
                culButonLIGHT=verdeLightBut;
                culButonDARK=verdeDarkBut;
                culButonHoverLIGHT=verdeLightButHover;
                culButonHoverDARK=verdeDarkButHover;
                culConturLIGHT=verdeLightContur;
                culConturDARK=verdeDarkContur;
                culScrisDARK=verdeDarkScris;
                culScrisLIGHT=verdeLightScris;
                if(!isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;
                }
                else if(isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                }
                themeMenu();
            }
            if(x>=0.1*screen_width && x<=0.25*screen_width && y>=0.65*screen_height && y<=0.75*screen_height)///orange
            {
                culBkLIGHT= orangeLightBk;
                culBkDARK= orangeDarkBk;
                culButonLIGHT=orangeLightBut;
                culButonDARK=orangeDarkBut;
                culButonHoverLIGHT=orangeLightButHover;
                culButonHoverDARK=orangeDarkButHover;
                culConturLIGHT=orangeLightContur;
                culConturDARK=orangeDarkContur;
                culScrisDARK=orangeDarkScris;
                culScrisLIGHT=orangeLightScris;
                if(!isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;
                }
                else if(isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                }
                themeMenu();
            }
            if(x>=0.35*screen_width && x<=0.5*screen_width && y>=0.2*screen_height && y<=0.3*screen_height)///roz
            {
                culBkLIGHT= rozLightBk;
                culBkDARK= rozDarkBk;
                culButonLIGHT=rozLightBut;
                culButonDARK=rozDarkBut;
                culButonHoverLIGHT=rozLightButHover;
                culButonHoverDARK=rozDarkButHover;
                culConturLIGHT=rozLightContur;
                culConturDARK=rozDarkContur;
                culScrisDARK=rozDarkScris;
                culScrisLIGHT=rozLightScris;
                if(!isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;
                }
                else if(isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                }
                themeMenu();
            }
            if(x>=0.35*screen_width && x<=0.5*screen_width && y>=0.35*screen_height && y<=0.45*screen_height)///maro
            {
                culBkLIGHT= maroLightBk;
                culBkDARK= maroDarkBk;
                culButonLIGHT=maroLightBut;
                culButonDARK=maroDarkBut;
                culButonHoverLIGHT=maroLightButHover;
                culButonHoverDARK=maroDarkButHover;
                culConturLIGHT=maroLightContur;
                culConturDARK=maroDarkContur;
                culScrisDARK=maroDarkScris;
                culScrisLIGHT=maroLightScris;
                if(!isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;
                }
                else if(isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                }
                themeMenu();
            }
            if(x>=0.35*screen_width && x<=0.5*screen_width && y>=0.5*screen_height && y<=0.6*screen_height)///custom
            {
                if(!customLightBk && !customDarkBut && !customDarkBk && !customDarkScris)
                {
                    colorPicker("Alege culoarea fundalului - light mode   1/8");
                    customLightBk=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);
                    colorPicker("Alege culoarea fundalului - dark mode   2/8");
                    customDarkBk=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);

                    colorPicker("Alege culoarea butonului - light mode   3/8");
                    customLightBut=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);
                    colorPicker("Alege culoarea butonului - dark mode   4/8");
                    customDarkBut=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);

                    colorPicker("Alege culoarea hoverului - light mode   5/8"  );
                    customLightButHover=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);
                    colorPicker("Alege culoarea hoverului - dark mode   6/8");
                    customDarkButHover=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);

                    colorPicker("Alege culoarea scrisului - light mode   7/8");
                    customLightScris=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);
                    colorPicker("Alege culoarea scrisului - dark mode   8/8");
                    customDarkScris=RGB(selectedColor[0],selectedColor[1],selectedColor[2]);
                    themeMenu();
                }
                else
                {
                    culBkLIGHT= customLightBk;
                    culBkDARK= customDarkBk;
                    culButonLIGHT=customLightBut;
                    culButonDARK=customDarkBut;
                    culButonHoverLIGHT=customLightButHover;
                    culButonHoverDARK=customDarkButHover;
                    culConturLIGHT=customLightContur;
                    culConturDARK=customDarkContur;
                    culScrisDARK=customDarkScris;
                    culScrisLIGHT=customLightScris;
                    if(!isDarkTheme)
                    {
                        culBk=culBkLIGHT;
                        culButonHover=culButonHoverLIGHT;
                        culButon=culButonLIGHT;
                    }
                    else if(isDarkTheme)
                    {

                        culBk=culBkDARK;
                        culButonHover=culButonHoverDARK;
                        culButon=culButonDARK;

                    }
                    themeMenu();
                }
            }
        }
    }
}

void selectFileMenu()
{
    cleardevice();
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    settextstyle(10,0,4);
    setbkcolor(culBk);
    outtextxy(screen_width/2,screen_height/2,"Selecteaza fisier");
    settextstyle(10,0,2);
    drawButon(0.4*screen_width,0.6*screen_height,0.6*screen_width,0.7*screen_height,"FILE",culButon);

    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);

    int x,y;
    bool peButon1=0, peButon2=0;
    while(true)
    {
        int mX=mousex(),mY=mousey();
        if(mX>=0.4*screen_width && mX<=0.6*screen_width && mY>=0.6*screen_height && mY<=0.7*screen_height)
        {
            if(!peButon1)
            {
                drawButon(0.4*screen_width,0.6*screen_height,0.6*screen_width,0.7*screen_height,"FILE",culButonHover);
                peButon1=1;
            }
        }
        else
        {
            if(peButon1)
            {
                drawButon(0.4*screen_width,0.6*screen_height,0.6*screen_width,0.7*screen_height,"FILE",culButon);
                peButon1=0;
            }
        }

        if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height)
        {
            if(!peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                peButon2=1;
            }
        }
        else
        {
            if(peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                peButon2=0;
            }
        }

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if(x>=0.4*screen_width && x<=0.6*screen_width && y>=0.6*screen_height && y<=0.7*screen_height)
            {
                menuFile();
            }
            if(x>=0.9*screen_width && x<=0.95*screen_width && y>=0.9*screen_height && y<=0.95*screen_height)
            {
                menu();
            }
        }
    }
}

void menu()
{
    char numar[10];
    cleardevice();
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    drawButon(0.4*screen_width,0.45*screen_height,0.6*screen_width,0.525*screen_height,"START",culButon);
    drawButon(0.4*screen_width,0.55*screen_height,0.6*screen_width,0.625*screen_height,"FILE",culButon);
    drawButon(0.4*screen_width,0.65*screen_height,0.6*screen_width,0.725*screen_height,"HELP",culButon);
    drawButon(0.4*screen_width,0.75*screen_height,0.6*screen_width,0.825*screen_height,"CLOSE",culButon);
    drawButon(0.05*screen_width,0.875*screen_height,0.1*screen_width,0.925*screen_height,"COLOR",culButon);

    ///titlu
    settextstyle(10,0,10);
    setbkcolor(culBk);
    outtextxy(0.5*screen_width,0.25*screen_height,"Nassi-Schneiderman");
    settextstyle(10,0,2);

    drawThemeButton();

    instrH=0.03*screen_height;
    returnH=0.045*screen_height;
    loopH=0.04*screen_height;
    ifH=0.05*screen_height;
    switchH=0.065*screen_height;
    separareH=0.08*screen_height;
    loopIndentare=0.02*screen_width;
    functionH=0.06*screen_height;
    scrollH=0.1*screen_height;


    char selectie[100];
    setbkcolor(culBk);
    if(FISIER==0)
        strcpy(selectie,"NICIUN FISIER SELECTAT");
    else
    {
        sprintf(numar,"%d",FISIER);
        strcpy(selectie,"FISIER SELECTAT: ");
        strcat(selectie,numar);
    }
    drawButon(0.4*screen_width,0.91 *screen_height,0.6*screen_width,0.95*screen_height,selectie,culButon);
    int x,y;
    bool peButon2=0,peButon1=0,peButon3=0,peButon4=0,peButon5=0;
    while(true)
    {
    /// /////////////////////////////          HOVER BUTOANE MAIN
        int mX=mousex(),mY=mousey();
        if(mX>=0.4*screen_width && mX<=0.6*screen_width && mY>=0.45*screen_height && mY<=0.525*screen_height) ///satrt
        {
            if(!peButon1)
            {
                drawButon(0.4*screen_width,0.45*screen_height,0.6*screen_width,0.525*screen_height,"START",culButonHover);
                peButon1=1;
            }
        }
        else
        {
            if(peButon1)
            {
                drawButon(0.4*screen_width,0.45*screen_height,0.6*screen_width,0.525*screen_height,"START",culButon);
                peButon1=0;
            }
        }

        if(mX>=0.4*screen_width && mX<=0.6*screen_width && mY>=0.55*screen_height && mY<=0.625*screen_height)
        {
            if(!peButon2)
            {
                drawButon(0.4*screen_width,0.55*screen_height,0.6*screen_width,0.625*screen_height,"FILE",culButonHover);
                peButon2=1;
            }
        }
        else
        {
            if(peButon2)
            {
                drawButon(0.4*screen_width,0.55*screen_height,0.6*screen_width,0.625*screen_height,"FILE",culButon);
                peButon2=0;
            }
        }

        if(mX>=0.4*screen_width && mX<=0.6*screen_width && mY>=0.65*screen_height && mY<=0.725*screen_height)
        {
            if(!peButon3)
            {
                drawButon(0.4*screen_width,0.65*screen_height,0.6*screen_width,0.725*screen_height,"HELP",culButonHover);
                peButon3=1;
            }
        }
        else
        {
            if(peButon3)
            {
                drawButon(0.4*screen_width,0.65*screen_height,0.6*screen_width,0.725*screen_height,"HELP",culButon);
                peButon3=0;
            }
        }

        if(mX>=0.4*screen_width && mX<=0.6*screen_width && mY>=0.75*screen_height && mY<=0.825*screen_height)
        {
            if(!peButon4)
            {
                drawButon(0.4*screen_width,0.75*screen_height,0.6*screen_width,0.825*screen_height,"CLOSE",culButonHover);
                peButon4=1;
            }
        }
        else
        {
            if(peButon4)
            {
                drawButon(0.4*screen_width,0.75*screen_height,0.6*screen_width,0.825*screen_height,"CLOSE",culButon);
                peButon4=0;
            }
        }
        if(mX>=0.05*screen_width && mX<=0.1*screen_width && mY>=0.875*screen_height && mY<=0.925*screen_height)
        {
            if(!peButon5)
            {
                drawButon(0.05*screen_width,0.875*screen_height,0.1*screen_width,0.925*screen_height,"COLOR",culButonHover);
                peButon5=1;
            }
        }
        else
        {
            if(peButon5)
            {
                drawButon(0.05*screen_width,0.875*screen_height,0.1*screen_width,0.925*screen_height,"COLOR",culButon);
                peButon5=0;
            }
        }

        if(mX>=0 && mX<=30 && mY>=0 && mY<=30)
        {
            if(!peSecret)
            {
                drawButon(0, 0, 30, 30, "", culSecret);
                peSecret=1;
            }
        }
        else
        {
            if(peSecret)
            {
                setfillstyle(1, culBk);
                bar(0, 0, 35, 35);
                peSecret=0;
            }
        }

    /// /////////////////////////////          daca dau click
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if(x>=0 && x<=30 && y>=0 && y<=30)
            {
                if(secret)
                {
                    secret=0;
                    culSecret=RGB(255, 0, 0);
                }
                else
                {
                    secret=1;
                    culSecret=RGB(0, 255, 0);
                }
                drawButon(0, 0, 30, 30, "", culSecret);
            }

            if(x>=0.05*screen_width && y>=0.05*screen_height && x<=0.15*screen_width && y<=0.15*screen_height) /// DARK THEME
            {
                if(x>=0 && x<=30 && y>=0 && y<=30)
                {
                    if(secret)
                    {
                        secret=0;
                        culSecret=RGB(255, 0, 0);
                    }
                    else
                    {
                        secret=1;
                        culSecret=RGB(0, 255, 0);
                    }
                    drawButon(0, 0, 30, 30, "", culSecret);
                }

                if(isDarkTheme)
                {
                    culBk=culBkLIGHT;
                    culButonHover=culButonHoverLIGHT;
                    culButon=culButonLIGHT;

                    culIf=culIfLIGHT;
                    culBreak=culBreakLIGHT;
                    culFunctie=culFunctieLIGHT;
                    culLoop=culLoopLIGHT;
                    culInstructiune=culInstructiuneLIGHT;
                    culReturn=culReturnLIGHT;
                    culUntil=culUntilLIGHT;
                    culProcedura=culProceduraLIGHT;
                    culSwitch=culSwitchLIGHT;
                    culPunctulete=culPunctuleteLIGHT;
                }
                else if(!isDarkTheme)
                {

                    culBk=culBkDARK;
                    culButonHover=culButonHoverDARK;
                    culButon=culButonDARK;

                    culIf=culIfDARK;
                    culBreak=culBreakDARK;
                    culFunctie=culFunctieDARK;
                    culLoop=culLoopDARK;
                    culInstructiune=culInstructiuneDARK;
                    culReturn=culReturnDARK;
                    culUntil=culUntilDARK;
                    culProcedura=culProceduraDARK;
                    culSwitch=culSwitchDARK;
                    culPunctulete=culPunctuleteDARK;
                }
                isDarkTheme=!isDarkTheme;
                cleardevice();
                menu();
            }
            else if(x>=0.4*screen_width && x<=0.6*screen_width && y>=0.45*screen_height && y<=0.525*screen_height) /// /////  START
            {
                if(FISIER)
                {
                    if(!secret){
                    cleardevice();
                    char s[1];
                    strcpy(s, "X");

                    setfillstyle(SOLID_FILL,culBk);
                    bar(0,0,screen_width,screen_height);

                    int nrRec=rand()%5+1;

                    if(nrRec==1) readimagefile("reclama1.jpg", screen_width/4, screen_height/4, screen_width/4*3, screen_height/4*3);
                    if(nrRec==2) readimagefile("reclama2.jpg", screen_width/4, screen_height/4, screen_width/4*3, screen_height/4*3);
                    if(nrRec==3) readimagefile("reclama3.jpg", screen_width/4, screen_height/4, screen_width/4*3, screen_height/4*3);
                    if(nrRec==4) readimagefile("reclama4.jpg", screen_width/4, screen_height/4, screen_width/4*3, screen_height/4*3);
                    if(nrRec==5) readimagefile("reclama5.jpg", screen_width/4, screen_height/4, screen_width/4*3, screen_height/4*3);

                    settextstyle(10, 0, 5);
                    settextjustify(1, 1);
                    setbkcolor(culBk);
                    outtextxy(screen_width/2, 4*screen_height/5, "Close in 3...");
                    Sleep(1000);
                    outtextxy(screen_width/2, 4*screen_height/5, "Close in 2...");
                    Sleep(1000);
                    outtextxy(screen_width/2, 4*screen_height/5, "Close in 1...");
                    Sleep(1000);
                    outtextxy(screen_width/2, 4*screen_height/5, "             ");
                    settextstyle(10, 0, 2);

                    drawButon(screen_width/4*3-40, screen_height/4, screen_width/4*3, screen_height/4+40, s, culButon);

                    bool peButonRec=0;
                    while(true)
                    {
                        int mX, mY;
                        mX=mousex();
                        mY=mousey();

                        if(mX>=screen_width/4*3-40 && mX<=screen_width/4*3 && mY>=screen_height/4 && mY<=screen_height/4+40)
                        {
                            if(!peButonRec)
                            {
                                drawButon(screen_width/4*3-40, screen_height/4, screen_width/4*3, screen_height/4+40, s, culButonHover);
                                peButonRec=1;
                            }
                        }
                        else
                        {
                            if(peButonRec)
                            {
                                drawButon(screen_width/4*3-40, screen_height/4, screen_width/4*3, screen_height/4+40, s, culButon);
                                peButonRec=0;
                            }
                        }


                        if(ismouseclick(WM_LBUTTONDOWN))
                        {

                            getmouseclick(WM_LBUTTONDOWN,mX,mY);
                            clearmouseclick(WM_LBUTTONDOWN);
                            if(mX>=screen_width/4*3-40 && mX<=screen_width/4*3 && mY>=screen_height/4 && mY<=screen_height/4+40)
                            {
                                cleardevice();
                                setfillstyle(SOLID_FILL,culBk);
                                bar(0,0,screen_width,screen_height);
                                drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                                drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                                drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                                drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                                drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                                drawButon(0.935*screen_width,0.54*screen_height,0.955*screen_width,0.505*screen_height,"-",culButon);

                                inFunctie=0;
                                f();
                                scroll();
                                if(signal)
                                    closegraph();
                                getch();
                                break;
                            }
                        }
                    }}
                    else
                    {
                        cleardevice();
                        setfillstyle(SOLID_FILL,culBk);
                        bar(0,0,screen_width,screen_height);
                        drawButon(0.05*screen_width,0.4*screen_height,0.1*screen_width,0.475*screen_height,"SUS",culButon);
                        drawButon(0.05*screen_width,0.525*screen_height,0.1*screen_width,0.6*screen_height,"JOS",culButon);
                        drawButon(0.92*screen_width,0.05*screen_height,0.95*screen_width,0.1*screen_height,"X",culButon);
                        drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                        drawButon(0.875*screen_width,0.45*screen_height,0.925*screen_width,0.55*screen_height,"CAPTURE",culButon);
                        drawButon(0.935*screen_width,0.46*screen_height,0.955*screen_width,0.495*screen_height,"+",culButon);
                        drawButon(0.935*screen_width,0.54*screen_height,0.955*screen_width,0.505*screen_height,"-",culButon);

                        inFunctie=0;
                        f();
                        scroll();
                        if(signal)
                            closegraph();
                        getch();
                        break;
                    }
                }
                else
                {
                    selectFileMenu();
                }

            }
            else if(x>=0.4*screen_width && y>=0.55*screen_height && x<=0.6*screen_width && y<=0.625*screen_height) /// /////  FILE
            {
                menuFile();
            }
            else if(x>=0.4*screen_width && y>=0.65*screen_height && x<=0.6*screen_width && y<=0.725*screen_height) /// ////// HELP
            {
                helpMenu();
            }
            else if(x>=0.4*screen_width && y>=0.75*screen_height && x<=0.6*screen_width && y<=0.825*screen_height) /// /////  CLOSE
            {
                settextjustify(1,1);
                setfillstyle(SOLID_FILL,BLACK);
                bar(0,0,screen_width,screen_height);
                setbkcolor(BLACK);
                outtextxy(screen_width/2,screen_height/2,"Se inchide...");
                delay(300);
                exit(0);

            }
            else if(x>=0.05*screen_width && x<=0.1*screen_width && y>=0.875*screen_height && y<=0.925*screen_height) ///  ////THEME
            {
                themeMenu();
            }
        }
    }
    closegraph();
    exit(0);
}


///         BUTOANE


void drawButon(int x1,int y1,int x2,int y2,char s[],int cul)
{

        setlinestyle(SOLID_LINE,SOLID_FILL,3);
        setfillstyle(SOLID_FILL,cul);
        bar(x1,y1,x2,y2);
        if(!isDarkTheme)
            setcolor(culConturLIGHT);
        else
            setcolor(culConturDARK);

        rectangle(x1,y1,x2,y2);
        if(!isDarkTheme)
            setcolor(culScrisLIGHT);
        else
            setcolor(culScrisDARK);

        setlinestyle(SOLID_LINE,SOLID_FILL,1);
        setbkcolor(cul);
        settextjustify(1,1);
        outtextxy((x1+x2)/2,(y1+y2)/2,s);
        setcolor(WHITE);
        setbkcolor(cul);
}

void drawThemeButton()
{
    int x1 = 0.05 * screen_width, y1 = 0.05 * screen_height;
    int x2 = 0.15 * screen_width, y2 = 0.15 * screen_height;
    setfillstyle(SOLID_FILL, isDarkTheme ? WHITE : YELLOW);
    fillellipse((x1 + x2) / 2, (y1 + y2) / 2, 30, 30);

    if (isDarkTheme)
    {
        setfillstyle(SOLID_FILL, culBk);
        setcolor(culBk);
        fillellipse((x1 + x2) / 2 + 12, (y1 + y2) / 2 - 12, 25, 25);
    }
}


///        FUNCTII DESENAT


int drawFunction(int st, int dr, int nivel, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culFunctie);
    bar(st,nivel,dr,nivel+functionH);

    line(st,nivel,dr,nivel);
    line(st,nivel,st,nivel+functionH);
    line(st,nivel+functionH,dr,nivel+functionH);
    line(dr,nivel,dr,nivel+functionH);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setbkcolor(culFunctie);
    outtextxy((st+dr)/2,nivel+functionH/2,s);
    setbkcolor(culBk);

    return nivel+functionH;
}

int drawIf(int st, int dr, int nivel, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culIf);
    bar(st,nivel,dr,nivel+ifH);

    line(st,nivel,dr,nivel);///sus
    line(st,nivel,st,nivel+ifH);
    line(dr,nivel,dr,nivel+ifH);
    line(st,nivel+ifH,dr,nivel+ifH);

    line(st,nivel,(st+dr)/2,nivel+ifH);
    line((st+dr)/2,nivel+ifH,dr,nivel);

    setbkcolor(culIf);
    outtextxy((st+dr)/2,nivel+ifH/2,s);
    outtextxy(st+textwidth("True"), nivel+ifH*0.75,"True");
    outtextxy(dr-textwidth("False"), nivel+ifH*0.75,"False");
    setbkcolor(culBk);

    return nivel+ifH;
}

int drawLoop(int st, int dr, int nivel, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culLoop);
    bar(st,nivel,dr,nivel+loopH);

    line(st,nivel,dr,nivel);
    line(st,nivel,st,nivel+loopH);
    line(dr,nivel,dr,nivel+loopH);
    line(st+loopIndentare,nivel+loopH,dr,nivel+loopH);
    setbkcolor(culLoop);
    outtextxy((st+dr)/2,nivel+loopH/2,s);
    setbkcolor(culBk);
    return nivel+loopH;
}

int drawUntil(int st, int dr, int nivel1, int nivel2, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culUntil);
    bar(st,nivel1,st+loopIndentare,nivel2);
    bar(st,nivel2,dr,nivel2+loopH);

    line(st+loopIndentare,nivel2,dr,nivel2);
    line(st,nivel1,st+loopIndentare,nivel1);
    line(st+loopIndentare,nivel1,st+loopIndentare,nivel2);
    line(st,nivel1,st,nivel2+loopH);
    line(dr,nivel2,dr,nivel2+loopH);
    line(st,nivel2+loopH,dr,nivel2+loopH);

    setbkcolor(culUntil);
    outtextxy((st+dr)/2,nivel2+loopH/2,s);
    setbkcolor(culBk);
    return nivel2+loopH;
}

int drawInstruction(int st, int dr, int nivel, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culInstructiune);
    bar(st,nivel,dr,nivel+instrH);

    line(st,nivel,st,nivel+instrH);
    line(dr,nivel,dr,nivel+instrH);
    line(st,nivel,dr,nivel);
    line(st,nivel+instrH,dr,nivel+instrH);

    setbkcolor(culInstructiune);
    outtextxy((st+dr)/2,nivel+instrH*0.6,s);
    setbkcolor(culBk);
    return nivel+instrH;
}

int drawReturn(int st, int dr, int nivel, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culReturn);
    bar(st,nivel,dr,nivel+instrH);
    line(st,nivel,dr,nivel);
    line(st,nivel,st,nivel+returnH);
    line(dr,nivel,dr,nivel+returnH);

    setbkcolor(culReturn);
    outtextxy((st+dr)/2,nivel+returnH/2,s);
    setbkcolor(culBk);
    return nivel+returnH;
}

int drawSwitch(int st, int dr, int nivel, char cond[], char s[][100], int nrCazuri)
{
    int i;
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culSwitch);
    setbkcolor(culSwitch);
    bar(st,nivel,dr,nivel+switchH);

    line(st,nivel,dr,nivel);
    line(st,nivel,st,nivel+switchH);
    line(dr,nivel,dr,nivel+switchH);
    line(st,nivel+switchH,dr,nivel+switchH);

    line(st,nivel,st+(dr-st)*nrCazuri/(nrCazuri+1),nivel+switchH/2);
    line(st+(dr-st)*nrCazuri/(nrCazuri+1),nivel+switchH/2,dr,nivel);

    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(st+(dr-st)*nrCazuri/(nrCazuri+1), nivel+switchH/4, cond);

    for(i=1; i<=nrCazuri; i++)
        line(st+(dr-st)*i/(nrCazuri+1),nivel+switchH*i/(nrCazuri*2)+2, st+(dr-st)*i/(nrCazuri+1), nivel+switchH);

    for(i=0; i<nrCazuri; i++)
        outtextxy(st+((dr-st)*(i+1)/(nrCazuri+1))-(dr-st)/(2*(nrCazuri+1)), nivel+switchH*0.9, s[i]);

    outtextxy(dr-(dr-st)/(2*(nrCazuri+1)), nivel+switchH*0.9, "def");
    setbkcolor(culBk);

    return nivel+switchH;
}

int drawBreak(int st, int dr, int nivel, char s[])
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culBreak);
    bar(st,nivel,dr,nivel+instrH);
    line(st,nivel,dr,nivel);
    line(st,nivel,st,nivel+returnH);
    line(dr,nivel,dr,nivel+returnH);

    return nivel+returnH;
}

void endIf(int st, int dr, int nivel1, int nivel2)
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    line(st,nivel1,st,nivel2);
    line(dr,nivel1,dr,nivel2);
}

void endLoop(int st, int dr, int nivel1, int nivel2)
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culLoop);
    bar(st,nivel1-1,st+loopIndentare,nivel2);
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    line(st+loopIndentare,nivel1,st+loopIndentare,nivel2);
    line(st,nivel1,st,nivel2);
    line(st,nivel2,st+loopIndentare,nivel2);
}

void endReturn(int st, int dr, int nivel1, int nivel2)
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culReturn);
    bar(st,(nivel1+nivel2)/2,dr,nivel2);

    line(st,nivel2,dr,nivel2);
    line(st,nivel1,st,nivel2);
    line(dr,nivel1,dr,nivel2);

    line(st+(dr-st)*0.75,nivel1,dr,(nivel1+nivel2)/2);
    line(st+(dr-st)*0.25,nivel1,st,(nivel1+nivel2)/2);
    line(st+(dr-st)*0.25,nivel2,st,(nivel1+nivel2)/2);
    line(st+(dr-st)*0.75,nivel2,dr,(nivel1+nivel2)/2);
}

void endBreak(int st, int dr, int nivel1, int nivel2)
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    setfillstyle(SOLID_FILL,culBreak);
    bar(st,(nivel1+nivel2)/2,dr,nivel2);

    line(st,nivel2,dr,nivel2);
    line(st,nivel1,st,nivel2);
    line(dr,nivel1,dr,nivel2);

    line(st+(dr-st)*0.75,nivel1,dr,(nivel1+nivel2)/2);
    line(st+(dr-st)*0.75,nivel2,dr,(nivel1+nivel2)/2);
}

void endSwitch(int st, int dr, int nivel1, int nivel2)
{
    setlinestyle(SOLID_LINE,SOLID_FILL,2);
    line(st,nivel1,st,nivel2);
    line(dr,nivel1,dr,nivel2);
    line(st,nivel2,dr,nivel2);
}

void endInstruction(nod *p, int nivel, int nivel2)
{
    if(strcmp(p->tip,"if")==0)
        endIf(p->st, p->dr, nivel+ifH,nivel2);
    if(strcmp(p->tip,"while")==0)
        endLoop(p->st, p->dr, nivel+loopH,nivel2);
    if(strcmp(p->tip,"for")==0)
        endLoop(p->st, p->dr, nivel+loopH,nivel2);
    if(strcmp(p->tip,"return")==0)
        endReturn(p->st,p->dr,nivel+returnH,nivel2);
}



///      RGB colors


void drawGradientSlider(int x, int y, int value, char label[300], char color)
{
    setcolor(WHITE);
    outtextxy(x, y - 20, label);
    // Draw gradient background
    for (int i = 0; i <= 255; i++)
    {
        int gradientColor = 0;
        switch (color)
        {
            case 'R': gradientColor = COLOR(i, 0, 0); break;
            case 'G': gradientColor = COLOR(0, i, 0); break;
            case 'B': gradientColor = COLOR(0, 0, i); break;
        }
        setcolor(gradientColor);
        line(x + i, y, x + i, y + 20);
    }

    // Draw slider handle
    int sliderPos = x + value;
    setcolor(WHITE);
    rectangle(sliderPos - 5, y, sliderPos + 5, y + 20);
    setfillstyle(SOLID_FILL, WHITE);
    bar(sliderPos - 5, y, sliderPos + 5, y + 20);
}

int getSliderValue(int sliderX, int sliderY)
{
    int mouseX, mouseY;

    while (ismouseclick(WM_LBUTTONDOWN) || ismouseclick(WM_MOUSEMOVE))
    {
        getmouseclick(WM_MOUSEMOVE, mouseX, mouseY);

        if (mouseY >= sliderY && mouseY <= sliderY + 20)
        {
            int value = mouseX - sliderX;
            if (value < 0) value = 0;
            if (value > 255) value = 255;
            return value;
        }
    }
    return -1;
}

void drawColorPreview()
{
    setfillstyle(SOLID_FILL, COLOR(selectedColor[0], selectedColor[1], selectedColor[2]));
    bar(0.65*screen_width,0.3*screen_height, 0.85*screen_width, 0.55*screen_height);
    setcolor(WHITE);
    rectangle(0.65*screen_width,0.3*screen_height, 0.85*screen_width, 0.55*screen_height);
    outtextxy(0.75*screen_width, 0.28*screen_height, "Preview Color");
}

void colorPicker(char titlu[250])
{
    cleardevice();
    setlinestyle(0,1,3);
    setfillstyle(SOLID_FILL,culBk);
    bar(0,0,screen_width,screen_height);
    drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
    settextstyle(10,0,5);
    setfillstyle(SOLID_FILL,culBk);
    setbkcolor(culBk);
    outtextxy(0.5*screen_width,0.1*screen_height,titlu);
    settextstyle(10,0,2);
    outtextxy(0.375*screen_width, 0.3*screen_height, "Adjust RGB Sliders:");
    bool done = false;
    drawGradientSlider(0.35*screen_width, 0.35*screen_height, selectedColor[0], "Red", 'R');
    drawGradientSlider(0.35*screen_width, 0.35*screen_height + 70, selectedColor[1], "Green", 'G');
    drawGradientSlider(0.35*screen_width, 0.35*screen_height + 2 * 70, selectedColor[2], "Blue", 'B');
    while (!done)
    {
        drawColorPreview();
        int x,y;
        bool peButon2=0;
        int mX=mousex(),mY=mousey();
        if(mX>=0.9*screen_width && mX<=0.95*screen_width && mY>=0.9*screen_height && mY<=0.95*screen_height)
        {
            if(!peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButonHover);
                peButon2=1;
            }
        }
        else
        {
            if(peButon2)
            {
                drawButon(0.9*screen_width,0.9*screen_height,0.95*screen_width,0.95*screen_height,"BACK",culButon);
                peButon2=0;
            }
        }
        setfillstyle(SOLID_FILL,culBk);
        setbkcolor(culBk);
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            // Check sliders
            if (y >= 0.35*screen_height && y <= 0.35*screen_height + 20)
            {
                selectedColor[0] = getSliderValue(0.35*screen_width, 0.35*screen_height);
                drawGradientSlider(0.35*screen_width, 0.35*screen_height, selectedColor[0], "Red", 'R');
            }
            else if (y >= 0.35*screen_height + 70 && y <= 0.35*screen_height + 70 + 20)
            {
                selectedColor[1] = getSliderValue(0.35*screen_width, 0.35*screen_height + 70);
                drawGradientSlider(0.35*screen_width, 0.35*screen_height + 70, selectedColor[1], "Green", 'G');
            }
            else if (y >= 0.35*screen_height + 2 * 70 && y <= 0.35*screen_height + 2 * 70 + 20)
            {
                selectedColor[2] = getSliderValue(0.35*screen_width, 0.35*screen_height + 2 * 70);
                drawGradientSlider(0.35*screen_width, 0.35*screen_height + 2 * 70, selectedColor[2], "Blue", 'B');
            }
            // Confirm Selection
            if (x >= 0.7*screen_width && x <=0.8*screen_width && y >=0.65*screen_height && y <=0.7*screen_height)
            {
                done = true;
                return;
            }
            if (x>=0.9*screen_width && y>=0.9*screen_height && x<=0.95*screen_width && y<=0.95*screen_height) ///BACK
            {
                if(!customDarkScris)
                {
                    customLightButHover=0;customLightBut=0;customLightContur=0;
                    customLightBk=0;customLightScris=0;customDarkButHover=0;
                    customDarkBut=0;customDarkContur=0;customDarkBk=0;customDarkScris=0;
                }
                themeMenu();
            }
        }
        // Confirmation Button
        setlinestyle(0,1,3);
        rectangle(0.7*screen_width,0.65*screen_height,0.8*screen_width,0.7*screen_height);
        setlinestyle(0,1,2);
        outtextxy(0.75*screen_width,0.675*screen_height,"Confirm");
        delay(50);
    }
}


///      EROARE + FULLSCREEN


void signalHandler(int sig)
{
    eroare();
}

void eroare()
{
    ///waveOutSetVolume(0, 0x800080); ///asta e de bun simt 50%
    ///waveOutSetVolume(0, 0xFFFF); ///100%
    ///PlaySound(TEXT("fnafsunet.wav"), NULL, SND_FILENAME | SND_ASYNC);
    ///readimagefile("fnaf.jpg", 0, 0, screen_width, screen_height);
    getch();
    closegraph();
}

void setFullScreen()
{
    HWND hwnd = GetForegroundWindow();  // Get the current window
    ShowWindow(hwnd, SW_MAXIMIZE);      // Maximize the window
    SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE);  // Remove borders
}

