#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include<dos.h>
#include<string.h>
#include<math.h>

typedef struct
{
    char type;
    char buildtime[3];
    long rnum;
    short hs;
    short rs;
    char empty[20];
} HEAD;
typedef struct
{
    char name[11];
    char type;
    long begin;
    char width;
    char digit;
    char empty[14];
} field;
typedef struct
{
    char name[10];
    float es;
    float ms;
    float cs;
    float sum;
} score;

char*filename(char*);/*输入文件名*/
void mysound(unsigned int freq);/*声音函数*/
void page1();/*第一页图像*/
void page2();/*第二页图像*/
void page3();/*第三页图像*/
void page4();/*第四页图像*/
void fline();/*坐标轴函数*/
void graph1(score*,HEAD*);/*英语成绩直方图*/
void graph2(score*,HEAD*);/*数学成绩直方图*/
void graph3(score*,HEAD*);/*计算机成绩直方图*/
void graph4(score*,HEAD*);/*英语成绩圆饼图*/
void graph5(score*,HEAD*);/*数学成绩圆饼图*/
void graph6(score*,HEAD*);/*计算机成绩圆饼图*/

int main()
{
    int gmode, gdriver=DETECT;
    char*fname;
    score s[50],t;
    FILE*fp;
    char chart,subject,nu,math[20]= {0},eng[20]= {0},computer[20]= {0},m[26]= {0};

    field f[5];
    HEAD head;
    long num,i,j,k,fnum;
    initgraph(&gdriver,&gmode,"");

    page1();
    getch();

    do
    {
        page2();
        fname=filename(m);
        for(i=0; i<=25; i++)
        {
            if(fname[i]=='.')
                break;
        }
        if(i==26)
        {
            for(j=25; j>=0; j--)
            {
                if(fname[j]!=0)
                {
                    fname[j+1]=46;
                    fname[j+2]=100;
                    fname[j+3]=98;
                    fname[j+4]=102;
                }
            }
        }
        fp=fopen(fname,"rb");

        if(fp==0)/*输错文件名后的选项*/
        {
            do
            {
                cleardevice();
                settextstyle(1,0,5);
                outtextxy(130,180,"FILENAME IS WRONG");
                settextstyle(1,0,2);
                outtextxy(180,260,"1. REWRITE THE FILENAME");
                outtextxy(180,280,"2. EXIT");
                nu=getch();

                if(nu==50)
                    exit(1);
            }
            while(nu!=49&&nu!=50);
        }

        if(nu==49)
            continue;
        fread(&head,32,1,fp);/*读取数据表每个人的各科成绩*/
        fnum=(head.hs-33)/32;
        for(i=0; i<fnum; i++)
        {
            fread(&f[0],32,1,fp);
            if(strcmp(f[0].name,"name")==0)
                f[1]=f[0];
            if(strcmp(f[0].name,"ENGLISH")==0)
                f[2]=f[0];
            if(strcmp(f[0].name,"math")==0)
                f[3]=f[0];
            if(strcmp(f[0].name,"maths")==0)
                f[4]=f[0];
        }
        for(i=0; i<head.rnum; i++)
        {
            fseek(fp,head.hs+head.rs*i,0);
            if(fgetc(fp)=='*')
            {
                s[i].ms=0;
                s[i].es=0;
                s[i].cs=0;
                continue;
            }
            fseek(fp,head.hs+head.rs*i+f[1].begin,0);
            fread(s[i].name,f[1].width,1,fp);
            fseek(fp,head.hs+head.rs*i+f[2].begin,0);
            fread(eng,f[2].width,1,fp);
            fseek(fp,head.hs+head.rs*i+f[3].begin,0);
            fread(math,f[3].width,1,fp);
            fseek(fp,head.hs+head.rs*i+f[4].begin,0);
            fread(computer,f[4].width,1,fp);
            s[i].ms=atof(math);
            s[i].es=atof(eng);
            s[i].cs=atof(computer);
            s[i].sum=s[i].es+s[i].ms+s[i].cs;
            s[i].name[f[1].width]=0;
            for(j=f[1].width-1; j>=0; j--)
            {
                if(s[i].name[j]==32)
                    s[i].name[j]=0;
                else
                    break;
            }

        }

        fclose(fp);
    }
    while(fp==0);

    do
    {
        page3();
        chart=getch();
    }
    while(chart!=49&&chart!=50);
    do
    {
        page4();
        subject=getch();
    }
    while(chart!=49&&chart!=50&&chart!=51);
    cleardevice();
    if(chart=='1')/*选择图像形式与科目*/
    {
        if(subject=='1')
            graph1(s,&head);
        else if(subject=='2')
            graph2(s,&head);
        else if(subject=='3')
            graph3(s,&head);
    }
    else if(chart=='2')
    {
        if(subject=='1')
            graph4(s,&head);
        else if(subject=='2')
            graph5(s,&head);
        else if(subject=='3')
            graph6(s,&head);
    }
    closegraph();
    return 0;
}
void mysound(unsigned int freq)
{
    union
    {
        int divisor;
        char c[2];
    } count;
    char bits;
    count.divisor=1193180/freq;
    outportb(0x43,0xb6);
    outportb(0x42,count.c[0]);
    outportb(0x42,count.c[1]);
    bits=inportb(0x61);
    outportb(0x61,bits|3);
    delay(5000);
    outportb(0x61,bits&0xfc);
    outportb(0x61,bits);
}
void page1()
{

    unsigned int i,s;
    unsigned int freq;
    void *p;
    cleardevice();
    setcolor(WHITE);
    settextstyle(1,0,7);
    outtextxy(130,190,"BIG PROJECT");





    s=imagesize(120,190,520,340);
    p=malloc(s);
    if(p==0)
    {
        closegraph();
        return 0;
    }
    else
        getimage(120,190,520,340,p);

    for(i=0; i<50; i++)
    {
        freq=rand()%8000+1;
        mysound(freq);
        cleardevice();
        putimage(120,40+i*3,p,0);
        delay(500);
    }
    free(p);
}
void page2()
{
    cleardevice();
    settextstyle(3,0,2);
    outtextxy(30,220,"PLEASE ENTER THE FILE NAME");
    outtextxy(30,250,"( THE NAME LENGTH IS LESS THAN 20 BYTE )");
    outtextxy(30,280,"( YOU CAN INPUT THE NAME WITHOUT \".DBF\" )");
    rectangle(330,220,620,245);
}
void page3()
{
    cleardevice();
    settextstyle(3,0,3);
    outtextxy(50,200,"Which chart do you want?");
    outtextxy(50,240,"Column chart please enter 1");
    outtextxy(50,280,"Pie chart please enter 2");

}
void page4()
{
    cleardevice();
    settextstyle(3,0,3);
    outtextxy(50,160,"Which subject do you want to see?");
    outtextxy(50,200,"English please enter 1");
    outtextxy(50,240,"Math please enter 2");
    outtextxy(50,280,"Computer please enter 3");
}
char* filename(char*m)
{

    int i,j=0;
    for(i=0; i<21; i++)
    {
        settextstyle(3,0,1);
        _AH=07;
        geninterrupt(0x21);
        if(_AL!=8&&_AL!=13&&i<20)
        {
            if(_AL!='/'&&_AL!='*'&&_AL!='<'&&_AL!='>'&&_AL!='|'&&_AL!='?'&&_AL!='"')
            {
                m[i]=_AL;
                m[i+1]=0;
                setcolor(WHITE);
                outtextxy(340+14*i,221,&m[i]);
            }
            else
                i--;
        }
        if(_AL==8)
        {
            i-=1;
            setcolor(BLACK);
            outtextxy(340+14*i,221,&m[i]);
            i-=1;
            if(i<0)
                i=-1;
        }
        if(_AL==13)
            break;
        if(i==20)
        {
            outtextxy(355+200,221," ");
            i-=1;
        }
    }

    return m;
}
void fline()
{
    setcolor(WHITE);
    settextstyle(1,0,1);
    line(90,20,90,350);
    line(90,20,85,25);
    line(90,20,95,25);
    line(90,230,95,230);
    outtextxy(60,50,"100");
    line(90,50,95,50);
    outtextxy(60,110,"80");
    line(90,110,95,110);
    outtextxy(60,170,"60");
    line(90,170,95,170);
    outtextxy(60,230,"40");
    line(90,290,95,290);
    outtextxy(60,290,"20");
    outtextxy(60,350,"0");
    line(550,350,90,350);
    line(550,350,545,345);
    line(550,350,545,355);

}
void graph1(score*s,HEAD*head)
{
    int i,j,k;
    char chart,subject,page,a[2]= {0};
    score t;
    a[1]=46;


    settextstyle(3,0,1);
    for(i=0; i<=head->rnum-2; i++)
    {
        k=i;
        for(j=i+1; j<=head->rnum-1; j++)
            if(s[j].es>s[k].es)
                k=j;
        t=s[k];
        s[k]=s[i];
        s[i]=t;
    }
    for(i=0; i<=head->rnum-1; i++)
    {


        setfillstyle(1,RED);
        if(i>4)
            bar(200,420,260,450);
        bar(290,420,350,450);
        bar(380,420,440,450);
        outtextxy(225,425,"P");
        outtextxy(315,425,"E");
        outtextxy(405,425,"N");

        if(i<4)
        {
            setfillstyle(1,7);
            bar(200,420,260,450);
            outtextxy(225,425,"P");

        }

        if(i>=head->rnum-head->rnum%5)
        {
            setfillstyle(1,7);
            bar(380,420,440,450);
            outtextxy(405,425,"N");
        }
        itoa(i+1,a,10);
        fline();
        outtextxy(520,20,"ENGLISH");
        setfillstyle(1,YELLOW);
        bar(130+i%5*90,350,170+i%5*90,350-(int)s[i].es*3);
        outtextxy(110+i%5*90,370,a);
        outtextxy(125+i%5*90,370,".");
        outtextxy(135+i%5*90,370,s[i].name);

        if((i+1)%5==0)
        {
            page=getch();
            if((page=='p'||page=='P')&&i>4)
            {
                i-=10;
                cleardevice();

            }
            else if((page=='n'||page=='N')&&i<=head->rnum-head->rnum%5-1)
                cleardevice();
            else if(page=='e'||page=='E')
            {
                cleardevice();
                break;
            }
            else
                i-=5;
        }

        else if(i==head->rnum-1)
        {

            page=getch();
            i-=head->rnum%5;
            if(page=='p'||page=='P')
            {
                i=i-5;
                cleardevice();
            }
            else if(page=='e'||page=='E')
            {
                cleardevice();
                break;
            }
        }
    }
}
void graph2(score*s,HEAD*head)
{
    int i,j,k;
    char page,a[2]= {0};
    score t;
    a[1]=46;


    settextstyle(3,0,1);
    for(i=0; i<=head->rnum-2; i++)
    {
        k=i;
        for(j=i+1; j<=head->rnum-1; j++)
            if(s[j].ms>s[k].ms)
                k=j;
        t=s[k];
        s[k]=s[i];
        s[i]=t;
    }
    for(i=0; i<=head->rnum-1; i++)
    {
        setfillstyle(1,RED);
        if(i>4)
            bar(200,420,260,450);
        bar(290,420,350,450);
        bar(380,420,440,450);
        outtextxy(225,425,"P");
        outtextxy(315,425,"E");
        outtextxy(405,425,"N");

        if(i<4)
        {
            setfillstyle(1,7);
            bar(200,420,260,450);
            outtextxy(225,425,"P");

        }

        if(i>=head->rnum-head->rnum%5)
        {
            setfillstyle(1,7);
            bar(380,420,440,450);
            outtextxy(405,425,"N");
        }

        itoa(i+1,a,10);
        fline();
        outtextxy(520,20,"MATH");
        setfillstyle(1,YELLOW);
        bar(130+i%5*90,350,170+i%5*90,350-(int)s[i].ms*3);
        outtextxy(110+i%5*90,370,a);
        outtextxy(125+i%5*90,370,".");
        outtextxy(135+i%5*90,370,s[i].name);

        if((i+1)%5==0)
        {
            page=getch();
            if((page=='p'||page=='P')&&i>4)
            {
                i-=10;
                cleardevice();

            }
            else if((page=='n'||page=='N')&&i<=head->rnum-head->rnum%5-1)
                cleardevice();
            else if(page=='e'||page=='E')
            {
                cleardevice();
                break;
            }
            else
                i-=5;

        }
        else if(i==head->rnum-1)
        {
            page=getch();
            i-=head->rnum%5;
            if(page=='p'||page=='P')
            {
                i=i-5;
                cleardevice();
            }
            else if(page=='e'||page=='E')
            {
                cleardevice();
                break;
            }
        }
    }

}
void graph3(score*s,HEAD*head)
{
    int i,j,k;
    char page,a[2]= {0};
    score t;
    a[1]=46;

    settextstyle(3,0,1);
    for(i=0; i<=head->rnum-2; i++)
    {
        k=i;
        for(j=i+1; j<=head->rnum-1; j++)
            if(s[j].cs>s[k].cs)
                k=j;
        t=s[k];
        s[k]=s[i];
        s[i]=t;
    }
    for(i=0; i<=head->rnum-1; i++)
    {
        setfillstyle(1,RED);
        if(i>4)
            bar(200,420,260,450);
        bar(290,420,350,450);
        bar(380,420,440,450);
        outtextxy(225,425,"P");
        outtextxy(315,425,"E");
        outtextxy(405,425,"N");

        if(i<4)
        {
            setfillstyle(1,7);
            bar(200,420,260,450);
            outtextxy(225,425,"P");
        }

        if(i>=head->rnum-head->rnum%5)
        {
            setfillstyle(1,7);
            bar(380,420,440,450);
            outtextxy(405,425,"N");
        }

        itoa(i+1,a,10);
        fline();
        outtextxy(520,20,"COMPUTER");
        setfillstyle(1,YELLOW);
        bar(130+i%5*90,350,170+i%5*90,350-(int)s[i].cs*3);
        outtextxy(110+i%5*90,370,a);
        outtextxy(125+i%5*90,370,".");
        outtextxy(135+i%5*90,370,s[i].name);

        if((i+1)%5==0)
        {
            page=getch();
            if((page=='p'||page=='P')&&i>4)
            {
                i-=10;
                cleardevice();

            }
            else if((page=='n'||page=='N')&&i<=head->rnum-head->rnum%5-1)
                cleardevice();
            else if(page=='e'||page=='E')
            {
                cleardevice();
                break;
            }
            else
                i-=5;

        }
        else if(i==head->rnum-1)
        {
            page=getch();
            i-=head->rnum%5;
            if(page=='p'||page=='P')
            {
                i=i-5;
                cleardevice();
            }
            else if(page=='e'||page=='E')
            {
                cleardevice();
                break;
            }
        }
    }
}
void graph4(score*s,HEAD*head)
{
    int i,a[5]= {0};
    float b[5]= {0};
    char page,c[35]= {0};

    outtextxy(20,30,"ENGLISH");
    setfillstyle(1,BLUE);
    bar(285,415,350,450);
    outtextxy(310,420,"xit");
    setcolor(RED);
    outtextxy(300,420,"E");
    setcolor(WHITE);
    settextstyle(3,0,1);
    outtextxy(550-3,8,"0-60:");
    outtextxy(550-3,35-2,"61-70:");
    outtextxy(550-3,60-2,"71-80:");
    outtextxy(550-3,85-2,"81-90:");
    outtextxy(550-3,110-2,"91-100:");
    rectangle(530,0,640,140);

    for(i=0; i<=head->rnum-1; i++)
    {
        if(s[i].es<60)
            a[0]+=1;
        if(s[i].es>=60&&s[i].es<70)
            a[1]+=1;
        if(s[i].es>=70&&s[i].es<80)
            a[2]+=1;
        if(s[i].es>=80&&s[i].es<90)
            a[3]+=1;
        if(s[i].es>=90)
            a[4]+=1;
    }
    setcolor(WHITE);
    for(i=0; i<5; i++)
    {
        b[i]=(float)a[i]/head->rnum;
        (int)a[i]=360*a[i]/head->rnum;
        sprintf(c+7*i,"%.2f%",b[i]*100);
    }

    setfillstyle(1,1);
    pieslice(300,240,0,a[0],100);
    bar(610,10,630,25);
    setfillstyle(1,2);
    pieslice(300,240,a[0],(a[0]+a[1]),100);
    bar(610,35,630,50);
    setfillstyle(1,3);
    pieslice(300,240,(a[0]+a[1]),(a[0]+a[1]+a[2]),100);
    bar(610,60,630,75);
    setfillstyle(1,4);
    pieslice(300,240,(a[0]+a[1]+a[2]),(a[0]+a[1]+a[2]+a[3]),100);
    bar(610,85,630,100);
    setfillstyle(1,5);
    pieslice(300,240,(a[0]+a[1]+a[2]+a[3]),360,100);
    bar(610,110,630,125);

    settextstyle(3,0,1);
    outtextxy(300+140*cos(3.14*(b[0])),240-140*sin(3.14*(b[0])),c);
    outtextxy(300-40+140*cos(3.14*(b[0]+b[1]+b[0])),240+10-20-140*sin(3.14*(b[0]+b[1]+b[0])),c+7);
    outtextxy(300-20+140*cos(3.14*(2*(b[0]+b[1])+b[2])),240-30-140*sin(3.14*(2*(b[0]+b[1])+b[2])),c+14);
    outtextxy(300-20+140*cos(3.14*(2*(b[0]+b[1]+b[2])+b[3])),240-140*sin(3.14*(2*(b[0]+b[1]+b[2])+b[3])),c+21);
    outtextxy(300+140*cos(3.14*(2*(b[0]+b[1]+b[2]+b[3])+b[4])),240+-20-140*sin(3.14*(2*(b[0]+b[1]+b[2]+b[3])+b[4])),c+28);

    do
    {
        page=getch();
    }
    while(page!=69&&page!=101);
    cleardevice();

}
void graph5(score*s,HEAD*head)
{
    int i,a[5]= {0};
    float b[5]= {0};
    char page,c[35]= {0};

    outtextxy(20,30,"MATH");
    setfillstyle(1,BLUE);
    bar(285,415,350,450);
    outtextxy(310,420,"xit");
    setcolor(RED);
    outtextxy(300,420,"E");
    setcolor(WHITE);
    settextstyle(3,0,1);
    outtextxy(550-3,8,"0-60:");
    outtextxy(550-3,35-2,"61-70:");
    outtextxy(550-3,60-2,"71-80:");
    outtextxy(550-3,85-2,"81-90:");
    outtextxy(550-3,110-2,"91-100:");
    rectangle(530,0,640,140);

    for(i=0; i<=head->rnum-1; i++)
    {
        if(s[i].ms<60)
            a[0]+=1;
        if(s[i].ms>=60&&s[i].ms<70)
            a[1]+=1;
        if(s[i].ms>=70&&s[i].ms<80)
            a[2]+=1;
        if(s[i].ms>=80&&s[i].ms<90)
            a[3]+=1;
        if(s[i].ms>=90)
            a[4]+=1;
    }
    setcolor(WHITE);
    for(i=0; i<5; i++)
    {
        b[i]=(float)a[i]/head->rnum;
        (int)a[i]=360*a[i]/head->rnum;
        sprintf(c+7*i,"%.2f%",b[i]*100);
    }

    setfillstyle(1,1);
    pieslice(300,240,0,a[0],100);
    bar(610,10,630,25);
    setfillstyle(1,2);
    pieslice(300,240,a[0],(a[0]+a[1]),100);
    bar(610,35,630,50);
    setfillstyle(1,3);
    pieslice(300,240,(a[0]+a[1]),(a[0]+a[1]+a[2]),100);
    bar(610,60,630,75);
    setfillstyle(1,4);
    pieslice(300,240,(a[0]+a[1]+a[2]),(a[0]+a[1]+a[2]+a[3]),100);
    bar(610,85,630,100);
    setfillstyle(1,5);
    pieslice(300,240,(a[0]+a[1]+a[2]+a[3]),360,100);
    bar(610,110,630,125);

    settextstyle(3,0,1);
    outtextxy(300+140*cos(3.14*(b[0])),240-140*sin(3.14*(b[0])),c);
    outtextxy(300-40+140*cos(3.14*(b[0]+b[1]+b[0])),240+10-20-140*sin(3.14*(b[0]+b[1]+b[0])),c+7);
    outtextxy(300-20+140*cos(3.14*(2*(b[0]+b[1])+b[2])),240-30-140*sin(3.14*(2*(b[0]+b[1])+b[2])),c+14);
    outtextxy(300-20+140*cos(3.14*(2*(b[0]+b[1]+b[2])+b[3])),240-140*sin(3.14*(2*(b[0]+b[1]+b[2])+b[3])),c+21);
    outtextxy(300+140*cos(3.14*(2*(b[0]+b[1]+b[2]+b[3])+b[4])),240+-20-140*sin(3.14*(2*(b[0]+b[1]+b[2]+b[3])+b[4])),c+28);


    do
    {
        page=getch();
    }
    while(page!=69&&page!=101);
    cleardevice();
}
void graph6(score*s,HEAD*head)
{
    int i,a[5]= {0};
    float b[5]= {0};
    char page,c[35]= {0};

    outtextxy(20,30,"COMPUTER");
    setfillstyle(1,BLUE);
    bar(285,415,350,450);
    outtextxy(310,420,"xit");
    setcolor(RED);
    outtextxy(300,420,"E");
    setcolor(WHITE);
    settextstyle(3,0,1);
    outtextxy(550-3,8,"0-60:");
    outtextxy(550-3,35-2,"61-70:");
    outtextxy(550-3,60-2,"71-80:");
    outtextxy(550-3,85-2,"81-90:");
    outtextxy(550-3,110-2,"91-100:");
    rectangle(530,0,640,140);

    for(i=0; i<=head->rnum-1; i++)
    {
        if(s[i].cs<=60)
            a[0]+=1;
        if(s[i].cs>60&&s[i].cs<=70)
            a[1]+=1;
        if(s[i].cs>70&&s[i].cs<=80)
            a[2]+=1;
        if(s[i].cs>80&&s[i].cs<=90)
            a[3]+=1;
        if(s[i].cs>=91)
            a[4]+=1;
    }
    setcolor(WHITE);
    for(i=0; i<5; i++)
    {
        b[i]=(float)a[i]/head->rnum;
        (int)a[i]=360*a[i]/head->rnum;
        sprintf(c+7*i,"%.2f%",b[i]*100);
    }

    setfillstyle(1,1);
    pieslice(300,240,0,a[0],100);
    bar(610,10,630,25);
    setfillstyle(1,2);
    pieslice(300,240,a[0],(a[0]+a[1]),100);
    bar(610,35,630,50);
    setfillstyle(1,3);
    pieslice(300,240,(a[0]+a[1]),(a[0]+a[1]+a[2]),100);
    bar(610,60,630,75);
    setfillstyle(1,4);
    pieslice(300,240,(a[0]+a[1]+a[2]),(a[0]+a[1]+a[2]+a[3]),100);
    bar(610,85,630,100);
    setfillstyle(1,5);
    pieslice(300,240,(a[0]+a[1]+a[2]+a[3]),360,100);
    bar(610,110,630,125);

    settextstyle(3,0,1);
    outtextxy(300+140*cos(3.14*(b[0])),240-140*sin(3.14*(b[0])),c);
    outtextxy(300-40+140*cos(3.14*(b[0]+b[1]+b[0])),240+10-20-140*sin(3.14*(b[0]+b[1]+b[0])),c+7);
    outtextxy(300-20+140*cos(3.14*(2*(b[0]+b[1])+b[2])),240-30-140*sin(3.14*(2*(b[0]+b[1])+b[2])),c+14);
    outtextxy(300-20+140*cos(3.14*(2*(b[0]+b[1]+b[2])+b[3])),240-140*sin(3.14*(2*(b[0]+b[1]+b[2])+b[3])),c+21);
    outtextxy(300+140*cos(3.14*(2*(b[0]+b[1]+b[2]+b[3])+b[4])),240+-20-140*sin(3.14*(2*(b[0]+b[1]+b[2]+b[3])+b[4])),c+28);

    do
    {
        page=getch();
    }
    while(page!=69&&page!=101);
    cleardevice();
}
