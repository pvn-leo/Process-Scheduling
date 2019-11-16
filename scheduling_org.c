#include<stdio.h>
#include <stdio_ext.h>//unix only
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<curses.h>
//#include<conio.h>
#include<unistd.h>
#include "fort.h"
#define MAX_TASK 10
#pragma pack(1)
ft_table_t *table, *table1;

FILE* fp;

double avg_waiting_time=0.0; double avg_turn_around_time=0.0;

typedef struct PROCESS_INFO
{
    char pid[5];
    int arrival_time;
    int burst_time;//execution_time
    int wait_time;
    int completion_time;
    int turnaround_time;
    struct PROCESS_INFO *next;
    
}process;

typedef struct PROCESS_QUEUE
{
    process *front;
    process *rear;
}process_q;


process_q* init_queue();
process* create_process(char* pid, int arrival, int burst);
void enQueue(process_q* q, char* pid, int arrival, int burst);
process* deQueue(process_q* q);
void custom(process_q* q);
void read_file(process_q* q);
void display(process_q* q);
void destroy_q(process_q* q);
void tablesetter();



process_q* init_queue()
{
    process_q* q = (process_q*)malloc(sizeof(process_q));
    q->front= NULL;q->rear= NULL;
    return q;
}

process* create_process(char* pid, int arrival, int burst)
{
    process *temp=(process*)malloc(sizeof(process));
    strcpy(temp->pid,pid);
    temp->arrival_time=arrival;
    temp->burst_time=burst;
    temp->completion_time=temp->turnaround_time=temp->wait_time=0;
    temp->next=NULL;
    return temp;
}

void enQueue(process_q* q, char* pid, int arrival, int burst) 
{ 
	process* temp = create_process(pid,arrival,burst); 
	if (q->rear == NULL) 
    { 
		q->front = q->rear = temp; 
		return; 
	} 
	q->rear->next = temp; 
	q->rear = temp; 
} 

process* deQueue(process_q* q) 
{ 
	if (q->front == NULL) 
		return NULL; 
	process* temp = q->front; 
	q->front = q->front->next; 
    free(temp);
	if (q->front == NULL) 
		q->rear = NULL; 
	return temp;  
} 

void FCFS(process_q* q)
{
        char a[10],b[10],c[10],d[10],e[10];
        int ch; process* temp=q->front; int complete=0;int waiting_time=0,tournaround_time=0;
        int complete1=0; int l;
        int count=0;
        
        __fpurge(stdin);
        printf("\nStart Executing processes[0/1]?: ");
        __fpurge(stdin);
        scanf("%d",&ch);
        __fpurge(stdin);
        if(ch==1)
        {
            if(temp==NULL)
                printf("No Process Scheduled\n");
            else
            {
                printf("\n");
                while(temp)
                {

                    if(complete1<temp->arrival_time)
                    {
                        l=temp->arrival_time-complete1;
                        printf("CPU Idle time: %d \n",l);//////idle time
                        complete1+=l;
                        complete=complete1;
                    }
                    complete1+=temp->burst_time;
                   int i;
                    for(i=1;i<=temp->burst_time;i++)
                    {
                        
                        if(complete<=complete1)
                            complete++;
                        printf("\rPID:%s  Arrival:%d  Burst:%d  Completion:%d",temp->pid,temp->arrival_time,i,complete);
                        temp->turnaround_time=(complete-temp->arrival_time);
                        temp->wait_time=temp->turnaround_time-temp->burst_time;
                        sprintf(a,"%d",temp->arrival_time);
                        sprintf(b,"%d",i);
                        sprintf(c,"%d",complete);
                        sprintf(d,"%d",temp->turnaround_time);
                        sprintf(e,"%d",temp->wait_time);
                        ft_write_ln(table, temp->pid,a,b,c);
                        sleep(1);
                        fflush(stdout);
                    }
                    printf("\n");
                    count++;
                    avg_waiting_time+= temp->wait_time;
					avg_turn_around_time+=temp->turnaround_time;
                    ft_write_ln(table1, temp->pid,a,b,c,d,e);
                    deQueue(q);
                    temp=q->front;
                }
				avg_waiting_time/=count;
				avg_turn_around_time/=count;               
                if(q->front==NULL)
                 printf("All processes executed Successfully\n");
            }
        }

}

void read_file(process_q* q)
{
	char filename[100];char line[100];
    char *pid1; int arrival1; int burst1;
	__fpurge(stdin);printf("Enter file-name:");__fpurge(stdin);
	scanf("%[^\n]s",filename);__fpurge(stdin);
    //strcpy(filename,"task3.txt");
	char e;
	fp = fopen(filename, "r");
    if(fp==NULL)
    	printf("File not found"); 
    else 
		while (fgets(line, sizeof(line), fp)) 
            {
                char* token = strtok(line, ",");
                pid1=token; 
                token=strtok(NULL,",");
                arrival1=atoi(token); token=strtok(NULL,",");
                burst1=atoi(token);
                enQueue(q,pid1,arrival1,burst1);
                
            }
	fclose(fp);
}


void custom(process_q* q)
{
	char pid1[10]; int arrival1; int burst1; int n;
	printf("\nEnter no of process: ");scanf("%d",&n);__fpurge(stdin);
    if(n!=0)
	printf("\nEnter process details: PID(ENTER) Arrival Time,CPU Burst Time: \n");
	while(n--)
	{
		__fpurge(stdin);
		scanf("%[^\n]s",pid1);
        __fpurge(stdin);
		scanf("%d,%d",&arrival1,&burst1);
		__fpurge(stdin);
		enQueue(q,pid1,arrival1,burst1);
	}
}

void display(process_q* q)
{
    process* temp=q->front;
    if(temp==NULL)
        printf("EMPTY\n");
    else
    {
        while(temp)
        {
            printf("%s, %d, %d ",temp->pid,temp->arrival_time,temp->burst_time);
            temp=temp->next;
            printf("\n");
        }
        printf("\n");
    }
    
}

void tablesetter()
{
    table = ft_create_table(); table1 = ft_create_table();
    ft_set_border_style(table1, FT_NICE_STYLE);
    ft_set_border_style(table, FT_DOUBLE2_STYLE);
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_set_cell_prop(table1, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "PID", "Arrival Time", "CPU Burst TIme", "Completion Time");
    ft_write_ln(table1, "PID", "Arrival Time", "CPU Burst TIme", "Completion Time", "Turn Around Time", "Waiting Time");
}

void destroy_q(process_q* q)
{
    process* cur;
        for(cur = q->front; cur != NULL; cur = cur->next)
                free(cur);
        free(q);
}

int main()
{
    int k; int ch; int ip_method;
    process_q* q=init_queue();
    tablesetter();
    printf("\n1.Custom Input\t2.File Input: "); 
    scanf("%d",&ip_method);
    if(ip_method==1)
    	custom(q);
    else
        read_file(q);
    
    display(q);
    FCFS(q);
    printf("\nDo You want to see the summary[1/0]: ");
    scanf("%d",&ch);
    if(ch==1)
    {   
        printf("\n%s\n", ft_to_string(table));
        printf("\n");
        printf("\n%s\n", ft_to_string(table1));
        printf("\n");
        printf("Avg Turnaround Time: %f     Avg Waiting Time: %f",avg_turn_around_time,avg_waiting_time);
        printf("\n\n");
    }
    else
        {}
    destroy_q(q);
    ft_destroy_table(table);
    ft_destroy_table(table1);
}
