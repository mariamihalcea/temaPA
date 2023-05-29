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

/*void displayPlayer(playersList *headPlayer, FILE *r)
{
    while (headPlayer!=NULL)
    {
        fprintf (r,"%s %s %d", headPlayer->p.firstname, headPlayer->p.secondname, headPlayer->p.points);
        fprintf(r,"\n");
        headPlayer=headPlayer->next;
    }
    fprintf(r,"\n");
}
*/
void displayTeam(teamList *headTeam, FILE *r)
{
    while (headTeam!=NULL)
    {
        fprintf(r,"%s\n",headTeam->t.team_name);
        //displayPlayer((headTeam->t).player,r);
        headTeam=headTeam->next;
    }
    fprintf(r,"\n");
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

void displayQueue(Queue *queue,FILE *r)
{
    queueElem *q=queue->front;
    while (q!=NULL)
    {
        fprintf(r,"%-33s", q->team1.team_name);
        fprintf(r,"-");
        fprintf(r,"%33s\n",q->team2.team_name);

        q=q->next;
    }

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
    //printf("Adaugat in stiva: %s\n", newElem->team.team_name);
    newElem->next=*top;
    *top=newElem;
}

void winStack(stackElem **top,Queue *queue,FILE *r)
{
    queueElem* q=queue->front;
    while(q!=NULL)
    {
        //fprintf(r,"%0.2f %0.2f\n",q->team1.team_points,q->team2.team_points);
        if(q->team1.team_points>q->team2.team_points)
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
        // printf("%0.1f %0.1f\n",q->team1.team_points,q->team2.team_points);
        if(q->team1.team_points<q->team2.team_points)
            pushStack(top,q->team1);
        else pushStack(top,q->team2);
        q=q->next;
    }
}
void displayStack(stackElem *top,FILE *r)
{
    while (top!=NULL)
    {
        fprintf(r,"%-34s-%0.2f\n",top->team.team_name,top->team.team_points);
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
        team1.team_points=q->team1.team_points;
        team2 = newTeam(q->team2.nr, q->team2.team_name, q->team2.player);
        team2.team_points=q->team2.team_points;
        addAtBeginningTeam(finalists, team1);
        addAtBeginningTeam(finalists, team2);
        q=q->next;
    }
}

//task4
typedef struct elemBST
{
    Team team;
    struct elemBST *left;
    struct elemBST *right;
} elemBST;

elemBST* createBSTnewNode(Team TEAM)
{
    elemBST* node= (elemBST*)malloc(sizeof(elemBST));
    node->team.team_name=(char*)malloc(sizeof(char)*(strlen(TEAM.team_name)+1));
    strcpy(node->team.team_name,TEAM.team_name);
    node->team.team_points=TEAM.team_points;
    node->team.nr=TEAM.nr;
    node->team.player=TEAM.player;
    node->left=node->right= NULL;
    return node;

}

elemBST* insertBST(elemBST* node, Team TEAM)
{
    if(node==NULL) return createBSTnewNode(TEAM);

    if(TEAM.team_points > node->team.team_points)
        node->left=insertBST(node->left,TEAM);
    else if (TEAM.team_points < node->team.team_points)
        node->right=insertBST(node->right,TEAM);
    else
    {
        if(strcmp(node->team.team_name,TEAM.team_name)<0)
            node->left=insertBST(node->left,TEAM);
        else if(strcmp(node->team.team_name,TEAM.team_name)>0)
            node->right=insertBST(node->right,TEAM);
    }

    return node;
}
elemBST* minValueNode(elemBST* node)
{
    elemBST* aux=node;
    while(aux->left !=NULL)
        aux=aux->left;
    return aux;
}

void inorderBST(elemBST*root,FILE *r)
{
    if (root)
    {
        inorderBST(root->left,r);
        fprintf(r,"%-34s- %0.2f\n",root->team.team_name,root->team.team_points);
        inorderBST(root->right,r);
    }
}

//task 5

typedef struct elemAVL
{
    int height;
    Team team;
    struct elemAVL *left,*right;
} elemAVL;

int nodeHeight(elemAVL* root)
{
    if(root==NULL) return -1;
    else return root->height;
}
elemAVL* createAVLnewNode(elemAVL** node,Team key)
{
    *node=(elemAVL*)malloc(sizeof(elemAVL));
    (*node)->team.team_name=(char*)malloc(sizeof(char)*(strlen(key.team_name)+1));
    strcpy((*node)->team.team_name,key.team_name);
    (*node)->team=key;
    (*node)->height=0;
    (*node)->left=(*node)->right=NULL;
    return *node;
}
int max(int a, int b)
{
    if(a>b) return a;
    else return b;
}

elemAVL *rightRotation(elemAVL *z)
{
    elemAVL *y = z->left;
    elemAVL *T3 = y->right;
    y->right = z;
    z->left = T3;
    return y;
}

elemAVL *leftRotation(elemAVL *z)
{
    elemAVL *y = z->right;
    elemAVL *T2 = y->left;
    y->left = z;
    z->right = T2;
    return y;
}

elemAVL* LRRotation(elemAVL*Z)
{
    Z->left = leftRotation(Z->left);
    return rightRotation(Z);
}

elemAVL* RLRotation(elemAVL*Z)
{
    Z->right = rightRotation(Z->right);
    return leftRotation(Z);
}
elemAVL* insertAVL(elemAVL* node, Team key)
{
    if(node == NULL) node=createAVLnewNode(&node,key);

    if(key.team_points<node->team.team_points)
        node->left=insertAVL(node->left,key);
    else if(key.team_points>node->team.team_points)
        node->right=insertAVL(node->right,key);
    else
    {
        if(strcmp(node->team.team_name,key.team_name)>0)
            node->left=insertAVL(node->left,key);
        else if(strcmp(node->team.team_name,key.team_name)<0)
            node->right=insertAVL(node->right,key);
    }

    node->height=1+max(nodeHeight(node->left),nodeHeight(node->right));
    int k=(nodeHeight(node->left)-nodeHeight(node->right));

    if(k>1 && key.team_points < node->left->team.team_points)
        return rightRotation(node);
    if(k<-1 && key.team_points > node->right->team.team_points)
        return leftRotation(node);
    if(k>1 && key.team_points > node->left->team.team_points)
        return RLRotation(node);
    if(k<-1 && key.team_points < node->right->team.team_points)
        return LRRotation(node);
    return node;


}
void inorderAVL(elemAVL*root)
{
    if (root)
    {
        inorderAVL(root->left);
        printf("%0.2f %s",root->team.team_points,root->team.team_name);
        inorderAVL(root->right);
    }
}

elemBST* deleteELEM(elemBST **rootBST, Team T)
{
    if((*rootBST) == NULL) return *rootBST;

    if(T.team_points < (*rootBST)->team.team_points)
        (*rootBST)->left=deleteELEM(&(*rootBST)->left,T);
    else if(T.team_points > (*rootBST)->team.team_points)
        (*rootBST)->right=deleteELEM(&(*rootBST)->right,T);
    else
    {
        if(strcmp(T.team_name,(*rootBST)->team.team_name)<0)
            (*rootBST)->left=deleteELEM(&(*rootBST)->left,T);
        else if(strcmp(T.team_name,(*rootBST)->team.team_name)>0)
           (*rootBST)->right=deleteELEM(&(*rootBST)->right,T);
        else
        {
            if((*rootBST)->left == NULL)
            {
                elemBST *temp=(*rootBST);
                (*rootBST)=(*rootBST)->right;
                free(temp);
                return (*rootBST);
            }
            else if((*rootBST)->right == NULL)
            {
                elemBST *temp=(*rootBST);
                (*rootBST)=(*rootBST)->left;
                free(temp);
                return (*rootBST);
            }

             elemBST *temp=minValueNode((*rootBST)->right);
            (*rootBST)->team=temp->team;
            (*rootBST)->right=deleteELEM(&(*rootBST)->right,temp->team);
        }

    }
    return *rootBST;

}
elemBST* rightBST(elemBST* rootBST)
{
    elemBST *copy= rootBST;
    while(copy->right!=NULL)
        copy=copy->right;
    return copy;
}
elemAVL* putBSTinAVL(elemAVL *rootAVL,elemBST *rootBST,FILE *r)
{
    elemBST *node=NULL;
    int i;
    for(i=0; i<8; i++)
    {
        node=rightBST(rootBST);
        fprintf(r,"%-34s-%0.2f\n",node->team.team_name,node->team.team_points);
       //rootAVL=insertAVL(rootAVL,node->team);
        rootBST=deleteELEM(&rootBST,node->team);
        inorderBST(rootBST,r);

    }
    return rootAVL;

}

int main()
{
    FILE *d;
    if((d=fopen("d.in","r"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru citire");
        exit(1);
    }
    FILE *r;
    if((r=fopen("r.out","w"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru scriere");
        exit(1);
    }

    FILE *c;
    if((c=fopen("c.in","r"))==NULL)
    {
        puts("Fisierul nu a putut fi deschis pentru citire");
        exit(1);
    }
    int task;
    int i,nrEchipe,puncteJucator, nrJucatori;
    char numeJucator[30],prenumeJucator[30],numeEchipa[30];
    Team T;
    teamList *headTeam=NULL;
    Queue *q=createQueue();
    teamList *finalists=NULL;

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
    //fclose(d);
    }

    fscanf(c,"%d",&task);
    if(task==1)
    {teamList *headcopy=headTeam;
    teamPoints(&headcopy);
    eliminationTask(&headTeam,&nrEchipe);
    printf("%d\n",nrEchipe);
    displayTeam(headTeam,r);
    createMatchQueue(&q,headTeam);
    }

    fscanf(c,"%d",&task);
    if(task==1)
    {
    int k=1;
    while(nrEchipe>1)
    {
        fprintf(r,"---ROUND NO:%d\n",k);
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
    elemBST *rootBST=NULL;
    teamList *copy=finalists;
    while(copy !=NULL)
    {
        rootBST=insertBST(rootBST,copy->t);
        copy=copy->next;
    }
    fprintf(r,"\n");
    fprintf(r,"TOP 8 TEAMS:\n");
    inorderBST(rootBST,r);
    }



    return 0;
}
