#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CMDCOUNT 9
#define EVACOUNT 19
#define STRLEN 16

#define CMD_START     0
#define CMD_TURN      1
#define CMD_BEGIN     2
#define CMD_BOARD     3
#define CMD_INFO      4
#define CMD_END       5
#define CMD_ABOUT     6
#define CMD_RESTART   7
#define CMD_TAKEBACK  8

#define max(a, b) ((a)>(b)?(a):(b))
#define min(a, b) ((a)<(b)?(a):(b))

typedef struct MYPOINT
{
  int x;
  int y;
  int score;
} MYPOINT;

typedef struct EVALIST
{
  char str[10];
  int score;
} EVALIST;

const char* about_name="My_Five";
const char* about_version="1.0";
const char* about_author="Ribonucleic";
const char* about_country="CHINA";

const char* CMDLIST[CMDCOUNT]={
  "START",    //0
  "TURN",     //1
  "BEGIN",    //2
  "BOARD",    //3
  "INFO",     //4
  "END",      //5
  "ABOUT",    //6
  "RESTART",  //7
  "TAKEBACK"  //8
  };

const EVALIST EVA[EVACOUNT]={ //S:Self O:Opposite B:Border E:Empty
  {"SSSSS", 50000},
  {"ESSSSE", 5000},
  {"OSSSSE", 1000},
  {"ESSSSO", 1000},
  {"SESSS", 1000},
  {"SSESS", 1000},
  {"SSSES", 1000},
  {"BSSSSE", 800},
  {"ESSSSB", 800},
  {"ESSSE", 700},
  {"ESESSE", 300},
  {"ESSESE", 300},
  {"OSSSE", 250},
  {"ESSSO", 250},
  {"BSSSE", 200},
  {"ESSSB", 200},
  {"ESSE", 80},
  {"ESESE", 80},
  {"ESE", 20}
};

int boardsize;
int boardcount;
char* board;

int idCmd(char* str_cmd);
int exeCmd(int cmd);
int ptoi(MYPOINT p);
MYPOINT itop(int pos);
char readPoint(MYPOINT p);
void writePoint(MYPOINT p, char player);
MYPOINT calcPoint();
int calcLayer(int deep, char player, MYPOINT* p);
int calcSinglePoint(const MYPOINT* p, char player);
int calcPattern(char s[], int patternNum);
int mycmp(const void *a, const void *b);
//void swap(MYPOINT* a, MYPOINT* b);

int main(int argc, char* argv[])
{
  srand((unsigned)time(NULL));
  board=NULL;
  while(1)
  {
    char str_cmd[STRLEN]={0};
    int cmd=-1;
    scanf("%s", str_cmd);
    if((cmd=idCmd(str_cmd))==CMD_END)
    {
      if(board!=NULL)
        free(board);
      break;
    }
    exeCmd(cmd);
    /* DEBUG
    char fn[128]={0};
    strcat(fn, argv[0]);
    strcat(fn, "_ex.log");
    FILE* fp;
    if((fp=fopen(fn ,"a"))!=NULL)
    {
      fprintf(fp, "%s\n", str_cmd);
      for(int i=0;i<boardsize;i++)
      {
        for(int j=0;j<boardsize;j++)
        {
          MYPOINT t={i, j};
          fprintf(fp, "%d ", board[ptoi(t)]);
        }
        fprintf(fp, "\n");
      }
      fprintf(fp, "\n");
      fclose(fp);
    }
    */
  }
  return 0;
}

int idCmd(char* str_cmd)
{
  for(int i=0;i<CMDCOUNT;i++)
    if(strcmp(str_cmd, CMDLIST[i])==0)
      return i;
  return -1;
}

int exeCmd(int cmd)
{
  int int_start;
  char str_turn[STRLEN], str_board[STRLEN], str_info[2][STRLEN], str_takeback[STRLEN];
  char* str_comma;
  char str_x[STRLEN]={0}, str_y[STRLEN]={0};
  MYPOINT p;
  switch(cmd)
  {
    case CMD_START:
      scanf("%d", &int_start);
      boardsize=int_start;
      boardcount=0;
      board=(char*)calloc(int_start*int_start, sizeof(char));
      printf("OK\n");
      fflush(stdout);
      break;
    case CMD_TURN:
      scanf("%s", str_turn);
      str_comma=strchr(str_turn, ',');
      strncpy(str_x, str_turn, str_comma-str_turn);
      strncpy(str_y, str_comma+1, (str_turn+strlen(str_turn))-str_comma-1);
      p.x=atoi(str_x);
      p.y=atoi(str_y);
      writePoint(p, 2);
      boardcount++;
      if(boardcount<boardsize*boardsize)
      {
        p=calcPoint();
        writePoint(p, 1);
        boardcount++;
        printf("%d,%d\n", p.x, p.y);
        fflush(stdout);
      }
      else
      {
        printf("ERROR Gameboard has been full.\n");
        fflush(stdout);
      }
      break;
    case CMD_BEGIN:
      //p=calcPoint();
      p.x=p.y=(boardsize-1)/2;
      writePoint(p, 1);
      boardcount++;
      printf("%d,%d\n", p.x, p.y);
      fflush(stdout);
      break;
    case CMD_BOARD:
      do
        scanf("%s", str_board);
      while(strcmp(str_board, "DONE"));
      //board
      printf("0,0\n");
      fflush(stdout);
      break;
    case CMD_INFO:
      scanf("%s", str_info[0]);
      scanf("%s", str_info[1]);
      //info
      break;
    case CMD_ABOUT:
      printf("name=\"%s\", version=\"%s\", author=\"%s\", country=\"%s\"\n", about_name, about_version, about_author, about_country);
      fflush(stdout);
      break;
    case CMD_RESTART:
      //memset(board, 0, sizeof(board)); CANNOT USE sizeof(board) BECAUSE board is a pointer
      memset(board, 0, boardsize*boardsize*sizeof(char));
      boardcount=0;
      printf("OK\n");
      fflush(stdout);
      break;
    case CMD_TAKEBACK:
      scanf("%s", str_takeback);
      //takeback
      printf("OK\n");
      fflush(stdout);
      break;
  }
}

int ptoi(MYPOINT p)
{
  return (boardsize*p.x)+p.y;
}

MYPOINT itop(int pos)
{
  MYPOINT p;
  p.x=pos/boardsize;
  p.y=pos%boardsize;
  return p;
}

char readPoint(MYPOINT p)
{
  return board[ptoi(p)];
}

void writePoint(MYPOINT p, char player)
{
  board[ptoi(p)]=player;
}

MYPOINT calcPoint()
{
  MYPOINT ps, po;
  //p.x=rand()%boardsize;
  //p.y=rand()%boardsize;
  int s=calcLayer(1, 1, &ps);
  int o=calcLayer(1, 2, &po);
  if(s>o||s==50000)
    return ps;
  else
    return po;
}

int calcLayer(int deep, char player, MYPOINT* p)
{
  MYPOINT* re=calloc(boardsize*boardsize, sizeof(MYPOINT));
  for(int i=0;i<boardsize;i++)
    for(int j=0;j<boardsize;j++)
    {
      MYPOINT t={i, j};
      re[ptoi(t)].x=i;
      re[ptoi(t)].y=j;
      if(board[ptoi(t)]!=0)
        re[ptoi(t)].score=-1;
      else
      {
        writePoint(t, player);
        re[ptoi(t)].score=calcSinglePoint(&t, player);
        writePoint(t, 0);
      }
    }
  qsort(re, boardsize*boardsize, sizeof(re[0]), mycmp);
  /* BUBBLE SORT
  for(int i=0;i<boardsize*boardsize;i++)
    for(int j=i;j<boardsize*boardsize;j++)
      if(re[i].score<re[j].score)
        swap(re+i, re+j);
  */

  int m=0;
  for(;re[m+1].score==re[m].score;m++);
  int r=rand()%(m+1);
  p->x=re[r].x;
  p->y=re[r].y;
  int score=re[r].score;
  free(re);
  return score;
}

int calcSinglePoint(const MYPOINT* p, char player)
{
  int score=0;
  {
    char s[10]={0};
    if(p->x-4<0)
      s[0]='B';
    if(p->x+4>boardsize-1)
      s[8]='B';
    int endc=p->x+4>boardsize-1?boardsize-p->x-1:4;
    MYPOINT t;
    for(int i=(p->x-4<0?-p->x:-4), k=(s[0]==0?0:1);i<=endc;i++, k++)
    {
      t.x=p->x+i;
      t.y=p->y;
      char c=*(board+ptoi(t));
      if(c==0)
        s[k]='E';
      else if(c==player)
        s[k]='S';
      else
        s[k]='O';
    }
    int r=0;
    for(int i=0;i<EVACOUNT&&r==0;i++)
      r=calcPattern(s, i);
    score=r;
  }
  {
    char s[10]={0};
    if(p->y-4<0)
      s[0]='B';
    if(p->y+4>boardsize-1)
      s[8]='B';
    int endc=p->y+4>boardsize-1?boardsize-p->y-1:4;
    MYPOINT t;
    for(int i=(p->y-4<0?-p->y:-4), k=(s[0]==0?0:1);i<=endc;i++, k++)
    {
      t.x=p->x;
      t.y=p->y+i;
      char c=*(board+ptoi(t));
      if(c==0)
        s[k]='E';
      else if(c==player)
        s[k]='S';
      else
        s[k]='O';
    }
    int r=0;
    for(int i=0;i<EVACOUNT&&r==0;i++)
      r=calcPattern(s, i);
    score+=r;
  }
  {
    char s[10]={0};
    if(p->x-4<0||p->y-4<0)
      s[0]='B';
    int endc=min(p->x+4>boardsize-1?boardsize-p->x-1:4, p->y+4>boardsize-1?boardsize-p->y-1:4);
    MYPOINT t;
    for(int i=max(p->x-4<0?-p->x:-4, p->y-4<0?-p->y:-4), k=(s[0]==0?0:1);i<=endc;i++, k++)
    {
      t.x=p->x+i;
      t.y=p->y+i;
      char c=*(board+ptoi(t));
      if(c==0)
        s[k]='E';
      else if(c==player)
        s[k]='S';
      else
        s[k]='O';
    }
    if(p->x+4>boardsize-1||p->y+4>boardsize-1)
      s[strlen(s)]='B';
    int r=0;
    for(int i=0;i<EVACOUNT&&r==0;i++)
      r=calcPattern(s, i);
    score+=r;
  }
  {
    char s[10]={0};
    if(p->x-4<0||p->y+4>boardsize-1)
      s[0]='B';
    int endc=min(p->x+4>boardsize-1?boardsize-p->x-1:4, p->y-4<0?p->y:4);
    MYPOINT t;
    for(int i=max(p->x-4<0?-p->x:-4, p->y+4>boardsize-1?-(boardsize-p->y-1):-4), k=(s[0]==0?0:1);i<=endc;i++, k++)
    {
      t.x=p->x+i;
      t.y=p->y-i;
      char c=*(board+ptoi(t));
      if(c==0)
        s[k]='E';
      else if(c==player)
        s[k]='S';
      else
        s[k]='O';
    }
    if(p->x-4>boardsize-1||p->y-4<0)
      s[strlen(s)]='B';
    int r=0;
    for(int i=0;i<EVACOUNT&&r==0;i++)
      r=calcPattern(s, i);
    score+=r;
  }
  return score;
}

int calcPattern(char s[], int pN)
{
  if(strlen(s)<strlen(EVA[pN].str))
    return 0;
  for(int i=0;i<=strlen(s)-strlen(EVA[pN].str);i++)
  {
    char t[10]={0};
    strncpy(t, s+i, strlen(EVA[pN].str));
    if(strcmp(EVA[pN].str, t)==0)
      return EVA[pN].score;
  }
  return 0;
}

int mycmp(const void *a, const void *b)
{
  MYPOINT* pa=(MYPOINT*)a;
  MYPOINT* pb=(MYPOINT*)b;
  return (pa->score>pb->score)?-1:1;
}
/*
void swap(MYPOINT* a, MYPOINT* b)
{
  MYPOINT t;
  t.x=a->x, t.y=a->y, t.score=a->score;
  a->x=b->x, a->y=b->y, a->score=b->score;
  b->x=t.x, b->y=t.y, b->score=t.score;
}
*/
