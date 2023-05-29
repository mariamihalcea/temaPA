#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"
//task2
void teamPoints(teamList **headTeam)
{

    while((*headTeam)!=NULL)
    {
        ((*headTeam)->t).team_points=0;
        playersList *player=(*headTeam)->t.player;
        while(player!=NULL)
        {
            ((*headTeam)->t).team_points+=player->p.points;
            player=player->next;
        }
        ((*headTeam)->t).team_points/=((*headTeam)->t).nr;
        // printf("%0.1f\n",((*headTeam)->t).team_points);
        //printf("%0.1f\n",(*headTeam)->t.team_points);
        (*headTeam)=(*headTeam)->next;
    }
}

void minimumPoints(teamList *headTeam, teamList **minTeam)
{
    *minTeam=headTeam;
    while(headTeam->next!=NULL)
    {
        headTeam=headTeam->next;
        if((*minTeam)->t.team_points>(headTeam->t.team_points))
            (*minTeam)=headTeam;
    }
    //printf("Minimum points=%0.1f\n",(*minTeam)->t.team_points);

}
void deleteTeam(teamList **headTeam)
{
    if(*headTeam==NULL) return;
    teamList *minTeam=NULL;
    minimumPoints((*headTeam),&minTeam);
    // printf("Min points=%0.1f",minTeam->t.team_points);
    teamList *headcopy=*headTeam;
    if(headcopy->t.team_points==minTeam->t.team_points)
    {
        *headTeam=(*headTeam)->next;
        free(headcopy);
        return;
    }
    teamList *prev=*headTeam;
    headcopy=(*headTeam)->next;
    while(headcopy!=NULL)
    {
        if(headcopy->t.team_points==minTeam->t.team_points)
        {
            prev->next=headcopy->next;
            free(headcopy);
            return;
        }
        prev=headcopy;
        headcopy=headcopy->next;
    }
}

void eliminationTask(teamList **headTeam, int *nrEchipe)
{
    int x=1;
    while(x<=(*nrEchipe))
        x*=2;
    x=x/2;
    x=(*nrEchipe)-x; //nr. de echipe care trb sterse

    while(x>0 && (*headTeam)!=NULL)
    {
        deleteTeam(headTeam);
        (*nrEchipe)--;
        x--;
    }
}

//task 5

int max(int a, int b)
{
    if(a>b) return a;
    else return b;
}

void createListOfFinalists(teamList **finalists, Queue *queue)
{
    queueElem *q=queue->front;
    Team team1, team2;
    while(q!=NULL)
    {
        team1 = newTeam(q->team1.nr, q->team1.team_name, q->team1.player);
        team1.team_points=q->team1.team_points;
        team2 = newTeam(q->team2.nr, q->team2.team_name, q->team2.player);
        team2.team_points=q->team2.team_points;
        addAtBeginningTeam(finalists, team1);
        addAtBeginningTeam(finalists, team2);
        q=q->next;
    }
}


int main(int argc, char*argv[])
{
    FILE *d;
    if((d=fopen(argv[1],"r"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru citire");
        exit(1);
    }

    FILE *c;
    if((c=fopen(argv[2],"r"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru citire");
        exit(1);
    }
    FILE *r;
    if((r=fopen(argv[3],"w"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru scriere");
        exit(1);
    }

    int task;
    int i,nrEchipe,puncteJucator, nrJucatori;
    char numeJucator[30],prenumeJucator[30],numeEchipa[30];
    Team T;
    teamList *headTeam=NULL;
    Queue *q=createQueue();
    teamList *finalists=NULL;
    elemBST *rootBST=NULL;

    fscanf(c,"%d",&task);
    if(task==1)
    {
        fscanf(d,"%d", &nrEchipe);
        while(fscanf(d,"%d",&nrJucatori)==1)
        {
            fgets(numeEchipa,30,d);
            char *newline = strchr(numeEchipa, '\n');
            if (newline != NULL)
                *newline = '\0';
            playersList *headPlayer=NULL;
            for(i=0; i<nrJucatori; i++)
            {
                fscanf(d,"%s",numeJucator);
                fscanf(d,"%s",prenumeJucator);
                fscanf(d,"%d",&puncteJucator);
                Player P=newPlayer(numeJucator,prenumeJucator,puncteJucator);
                addAtBeginningPlayer(&headPlayer,P);
            }
            T=newTeam(nrJucatori,numeEchipa,headPlayer);
            addAtBeginningTeam(&headTeam,T);
        }
    }
    fscanf(c,"%d",&task);
    if(task==1)
    {
        teamList *headcopy=headTeam;
        teamPoints(&headcopy);
        eliminationTask(&headTeam,&nrEchipe);
        //printf("%d\n",nrEchipe);
        displayTeam(headTeam,r);
        createMatchQueue(&q,headTeam);
    }
    fscanf(c,"%d",&task);
    if(task==1)
    {   int k=1;
        while(nrEchipe>1)
        {   fprintf(r,"--- ROUND NO:%d\n",k);
            stackElem* winStackTop=NULL;
            stackElem* loseStackTop=NULL;
            displayQueue(q,r);
            fprintf(r,"WINNERS OF ROUND NO:%d\n",k);
            winStack(&winStackTop,q,r);
            loseStack(&loseStackTop,q);
            freeQueue(q);
            displayStack(winStackTop,r);
            StackToQueue(&q,winStackTop);
            freeStack(loseStackTop);
            freeStack(winStackTop);
            if(nrEchipe==16) //coada contine doar castigatorii, adica nrEchipe/2
                createListOfFinalists(&finalists,q);
            nrEchipe/=2;
            k++;
        }
    }
    fscanf(c,"%d",&task);
    if(task==1)
    {
        teamList *copy=finalists;
        while(copy !=NULL)
        {   rootBST=insertBST(rootBST,copy->t);
            copy=copy->next;
        }
        fprintf(r,"\n");
        fprintf(r,"TOP 8 TEAMS:\n");
        inorderBST(rootBST,r);
    }

    return 0;
}
