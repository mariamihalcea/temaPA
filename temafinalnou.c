#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player
{
    char *firstname,*secondname;
    int points;
} Player;

typedef struct playersList
{
    Player p;
    struct playersList *next;
} playersList;

typedef struct Team
{
    char *team_name;
    float team_points;
    int nr; //nr=numarul de jucatori din echipa
    playersList *player;

} Team;

typedef struct teamList
{
    Team t;
    struct teamList *next;
} teamList;

Player newPlayer(char *nume, char *prenume, int puncte)
{
    Player *newPlayer =(Player*)malloc(sizeof(Player));
    newPlayer->firstname=(char*)malloc(sizeof(char)*(strlen(nume)+1));
    newPlayer->secondname=(char*)malloc(sizeof(char)*(strlen(prenume)+1));

    strcpy(newPlayer->firstname,nume);
    strcpy(newPlayer->secondname,prenume);
    newPlayer->points=puncte;
    return *newPlayer;
}

void addAtBeginningPlayer(playersList **head, Player player)
{
    playersList* newPlayer = (playersList*)malloc(sizeof(playersList));
    newPlayer->p=player;
    newPlayer->next=*head;
    *head=newPlayer;
}

Team newTeam(int nr, char *echipa, playersList *Player)
{
    Team *newTeam=(Team*)malloc(sizeof(Team));
    newTeam->team_name=(char*)malloc(sizeof(char)*(strlen(echipa)+1));
    newTeam->team_points=0;
    newTeam->nr=nr;
    strcpy(newTeam->team_name, echipa);
    newTeam->player=Player;

    return *newTeam;
}

void addAtBeginningTeam(teamList **head, Team team)
{
    teamList* newTeam = (teamList*)malloc(sizeof(teamList));
    newTeam->t=team;
    newTeam->next = *head;
    *head = newTeam;
}

void displayPlayer(playersList *headPlayer)
{
    while (headPlayer!=NULL)
    {
        printf ("%s %s %d", headPlayer->p.firstname, headPlayer->p.secondname, headPlayer->p.points);
        printf("\n");
        headPlayer=headPlayer->next;
    }
    printf("\n");
}
void displayTeam(teamList *headTeam)
{
    while (headTeam!=NULL)
    {
        printf("%d %s\n",(headTeam->t).nr, (headTeam)->t.team_name);
        displayPlayer((headTeam->t).player);
        headTeam=headTeam->next;
    }
    printf("\n");
}

//task2
void teamPoints(teamList **headTeam)
{
    //teamList *headcopy=(*headTeam);
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

//task 3

typedef struct queueElem
{
    Team team1,team2;
    struct queueElem *next;
} queueElem;

typedef struct Queue
{
    struct queueElem *front, *rear;
} Queue;

Queue* createQueue()
{
    Queue *q=(Queue*)malloc(sizeof(Queue));
    if(q == NULL) return NULL;
    q->front=q->rear=NULL;
    return q;
}
//adaugare element
void addQueueElement(Queue *q, Team T1, Team T2)
{
    queueElem* newElem=(queueElem*)malloc(sizeof(queueElem));
    newElem->team1.team_name=(char*)malloc(sizeof(char)*(strlen(T1.team_name)+1));
    newElem->team2.team_name=(char*)malloc(sizeof(char)*(strlen(T2.team_name)+1));
    strcpy(newElem->team1.team_name,T1.team_name);
    strcpy(newElem->team2.team_name,T2.team_name);
    newElem->team1.team_points=T1.team_points;
    //printf("%0.1f\n",T1.team_points);
    newElem->team2.team_points=T2.team_points;
    //printf("%0.1f\n",T2.team_points);
    newElem->team1.nr=T1.nr;
    newElem->team2.nr=T2.nr;
    newElem->team1.player=T1.player;
    newElem->team2.player=T2.player;
    newElem->next=NULL;

    if(q->rear==NULL) q->rear=newElem;
    else
    {
        (q->rear)->next=newElem;
        q->rear=newElem;
    }

    if(q->front==NULL) q->front=q->rear;
}
void createMatchQueue(Queue** q, teamList *headTeam)
{
    (*q)=createQueue();
    while(headTeam!=NULL)
    {
        if(headTeam->next == NULL) break;
        addQueueElement((*q),headTeam->t,(headTeam->next)->t);
        headTeam=(headTeam->next)->next;
    }
}

void displayQueue(Queue *queue)
{
    queueElem *q=queue->front;
    while (q!=NULL)
    {
        printf("%s - %s ",q->team1.team_name, q->team2.team_name);
        q=q->next;
    }

    printf("\n");
}
void freeQueue(Queue *queue)
{
    queueElem *q=queue->front;
    while(q !=NULL)
    {
        queueElem *temp=q;
        q=q->next;
        free(temp->team1.team_name);
        free(temp->team2.team_name);
        free(temp);
    }
    queue->front=NULL;
    queue->rear=NULL;
}

typedef struct stackElem
{
    Team team;
    struct stackElem *next;
} stackElem;

void pushStack(stackElem** top, Team T)
{
    stackElem *newElem=(stackElem*)malloc(sizeof(stackElem));
    newElem->team.team_name=(char*)malloc(sizeof(char)*(strlen(T.team_name)+1));
    strcpy(newElem->team.team_name,T.team_name);
    newElem->team.team_points=T.team_points;
    newElem->team.nr=T.nr;
    newElem->team.player=T.player;
    printf("Adaugat in stiva: %s\n", newElem->team.team_name);
    newElem->next=*top;
    *top=newElem;
}

void winStack(stackElem **top,Queue *queue)
{
    queueElem* q=queue->front;
    while(q!=NULL)
    {
        printf("%0.2f %0.2f\n",q->team1.team_points,q->team2.team_points);
        if(q->team1.team_points>=q->team2.team_points)
        {
            pushStack(top,q->team1);
            (*top)->team.team_points+=1;
        }
        else
        {
            pushStack(top,q->team2);
            (*top)->team.team_points+=1;
        }
        q=q->next;
    }
}
void loseStack(stackElem **top,Queue *queue )
{
    queueElem* q=queue->front;
    while(q!=NULL)
    {
        printf("%0.1f %0.1f\n",q->team1.team_points,q->team2.team_points);
        if(q->team1.team_points<q->team2.team_points)
            pushStack(top,q->team1);
        else pushStack(top,q->team2);
        q=q->next;
    }
}
void displayStack(stackElem *top)
{
    while (top!=NULL)
    {
        printf("%s ",top->team.team_name);
        printf("%0.2f\n",top->team.team_points);
        top=top->next;
    }
    //printf("\n");

}
void freeStack(stackElem *StackTop)
{
    while(StackTop !=NULL)
    {
        stackElem *top=StackTop;
        StackTop=StackTop->next;
        free(top->team.team_name);
        free(top);
    }
}

void StackToQueue(Queue** q, stackElem *winStackTop)
{
    (*q)=createQueue();
    while(winStackTop!=NULL)
    {
        if(winStackTop->next == NULL) break;

        addQueueElement((*q),winStackTop->team,(winStackTop->next)->team);
        winStackTop=(winStackTop->next)->next;
    }


}
int isEmpty(stackElem *top)
{
    return top==NULL;
}

void createListOfFinalists(teamList **finalists, Queue *queue)
{
    queueElem *q=queue->front;
    Team team1, team2;
    while(q!=NULL)
    {
        team1 = newTeam(q->team1.nr, q->team1.team_name, q->team1.player);
        team2 = newTeam(q->team2.nr, q->team2.team_name, q->team2.player);

        addAtBeginningTeam(finalists, team1);
        addAtBeginningTeam(finalists, team2);
        q=q->next;
    }
}

int main()
{
    FILE *d;
    if((d=fopen("d.in","r"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru citire");
        exit(1);
    }

    int i,nrEchipe,puncteJucator, nrJucatori;
    char numeJucator[30],prenumeJucator[30],numeEchipa[30];
    Team T;
    teamList *headTeam=NULL;
    fscanf(d,"%d", &nrEchipe);
    while(fscanf(d,"%d",&nrJucatori)==1)
    {
        fgets(numeEchipa,30,d);
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
    //fclose(d);
    //displayTeam(headTeam);
    teamList *headcopy=headTeam;
    teamPoints(&headcopy);
    eliminationTask(&headTeam,&nrEchipe);
    printf("%d\n",nrEchipe);
    //displayTeam(headTeam);
    Queue *q=createQueue();
    createMatchQueue(&q,headTeam);
    displayQueue(q);

    teamList *finalists=NULL;

    while(nrEchipe>1)
    {

        stackElem* winStackTop=NULL;
        stackElem* loseStackTop=NULL;
        winStack(&winStackTop,q);
        loseStack(&loseStackTop,q);
        freeQueue(q);
        displayStack(winStackTop);
        //displayStack(loseStackTop);
        StackToQueue(&q,winStackTop);
        freeStack(loseStackTop);
        freeStack(winStackTop);
        if(nrEchipe==16) //coada contine doar castigatorii, adica nrEchipe/2
            createListOfFinalists(&finalists,q);
        nrEchipe/=2;
    }

    displayTeam(finalists);


    return 0;
}
