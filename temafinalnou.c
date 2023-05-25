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


void teamPoints(teamList *headTeam)
{
    while(headTeam!=NULL)
    {
        (headTeam->t).team_points=0;
        playersList *player=headTeam->t.player;
        while(player!=NULL)
        {
            (headTeam->t).team_points+=player->p.points;
            player=player->next;
        }
        (headTeam->t).team_points/=(headTeam->t).nr;
       // printf("%0.1f\n",headTeam->t.team_points);
        headTeam=headTeam->next;
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
    while(x<(*nrEchipe))
        x*=2;
    x=x/2;
    x=(*nrEchipe)-x; //nr. de echipe care trb sterse

    while(x>0)
    {
        deleteTeam(headTeam);
        (*nrEchipe)--;
        x--;

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
    displayTeam(headTeam);
    teamPoints(headTeam);
    eliminationTask(&headTeam,&nrEchipe);
    printf("%d\n",nrEchipe);
    displayTeam(headTeam);

}
