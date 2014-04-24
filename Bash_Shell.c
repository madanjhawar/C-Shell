#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<signal.h>
#include<errno.h>
#include <fcntl.h>
#include <sys/stat.h>


typedef struct node{
	int val;
	int check;                                        /* structure to save details of background process */
	char pro[50];
	char name[50];
}node;

void  split(char *input, char **a)
{
     int len,j=0;
	len=strlen(input);                                                    /* function to split the input command */
	for(;j<len;j++)
	{
		while(input[j]==' ' || input[j]=='\t')
		{
			input[j]='\0';
			j++;                           
		}
		*a=&input[j]; 
		*a++;
		while(input[j]!=' ' && input[j]!='\t' && input[j]!='\n' && input[j]!='\0')
		{
			j++;
		}
		j--;
	}
	*a='\0';
}

void jobs(node *pro_arr,int point)
{
	int i,j=1,key=0;
	for(i=0;i<point;i++)
		if(pro_arr[i].check==0)
		{
			key=1;
			printf("[%d] ",j);                                   /*  to print all the background process */
			printf("%s %s [%d]\n",pro_arr[i].pro,pro_arr[i].name,pro_arr[i].val);       
			j++;
		}
	if(!key)
		printf("No background process running\n");
}

void ok(node *pro_arr,int point)
{
	int i;
	for(i=0;i<point;i++)
	{
		if(pro_arr[i].check==0)
		{
			kill(pro_arr[i].val,9);           /* killing all the background process */
			pro_arr[i].check=1;
		}  

	}
}

void fg(node *pro_arr,int point,char *temp)
{
	int n=atoi(temp);
	int i,j=0,k=-1,m;
	for(i=0;i<point;i++)
	{
		if(pro_arr[i].check==0)
			j++;
		if(j==n)
		{
			k=i;
			break;
		}                                                           /* bringing a background process to foreground */
	}
	if(k>=0)
	{
		
		waitpid(pro_arr[k].val,&m,0);
		pro_arr[k].check=1;
	}
	else
	{
		printf("No such Process exists\n");
	}

}
void kjobs(char **a,int point,node *pro_arr)
{
	char t[1000],*mm[100];
	int i,j=0,k,n,m;
	pid_t c;
	n=atoi(a[1]);
	m=atoi(a[2]);
	for(i=0;i<point;i++)
	{
		if(pro_arr[i].check==0)
			j++;
		if(j==n)
		{
			k=i;
			break;                                                   /* function to implement 'kjob' */
		}
	}
	
	if(i==point)
		printf("No such process exists\n");
	else
		kill(pro_arr[k].val,m);
}
int ppid;
void sig_handler(int signo)
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGKILL)
		printf("received SIGKILL\n");
	else if (signo == SIGSTOP)
		printf("received SIGSTOP\n");
	else if (signo == SIGINT)
		printf("received SIGINT\n");                           /* signals handling fucntion */
	else if (signo == SIGQUIT)
		printf("received SIGQUIT\n");
	else if (signo == SIGTSTP)
	{
		kill(ppid,SIGTSTP);
		printf("received SIGTSTP\n");
	}
}

void ipinfo(int pid,char *home,int home_len)
{
	char t[100],buf[1000],*check,curr_dir[1000];
	int count=0,i,j,k;
	sprintf(t,"/proc/%d/status",pid);
	//	puts(t);
	FILE *file = fopen (t,"r");
	if ( file != NULL )
	{
		char line [ 200 ]; 
		while ( fgets ( line, sizeof(line), file ) != NULL ) 
		{
			if(count==1 || count==3 || count==11)
				fputs ( line, stdout ); 
			count++;
		}
		fclose ( file );
		for(count=0;count<500;count++)
			buf[count]='\0';
		sprintf(t,"/proc/%d/exe",pid);
		readlink(t,buf,1000);
		check=strstr(buf,home);
		curr_dir[0]='~';                                         /* function to implement 'pinfo' and 'pinfo <pid>' */
		curr_dir[1]='\0';
		j=1;
		if(check!=NULL)
		{
			for(i=home_len;i<=strlen(buf);i++,j++)
			{
				curr_dir[j]=buf[i];
			}
			strcpy(buf,curr_dir);
		}
		sprintf(t,"Executable Path -- %s",buf);

		puts(t);
	}
	else{
		printf("Error..(Check if the pid is correct)\n");
	}
}

int main()
{


	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	{
		printf("\ncan't catch SIGUSR1\n");
	}
	if (signal(SIGKILL, sig_handler) == SIG_ERR)             /* handling signals */
	{}
	if (signal(SIGSTOP, sig_handler) == SIG_ERR)
	{}
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		printf("\ncan't catch SIGINT\n");}
	if (signal(SIGQUIT, sig_handler) == SIG_ERR){
		printf("\ncan't catch SIGQUIT\n");}

		char *a[100],home[1000],curr_dir[1000],t3[1000];
		char host_name[20],*check,temp_dir[1000],*usr_name;
		char input[500],*temp;
		int n,m,home_len,i,j,k,temp_len,status,point,c1,newtmp,ty;
		pid_t child2,child1;
		node pro_arr[5000];

		point=0;

		getcwd(home,1000);
		home_len=strlen(home);
		gethostname(host_name,20);
		usr_name=getenv("USER");
		char t1[200];
		
		char *t2;
		
		sprintf(t1,"<%s@%s:~",usr_name,host_name);                                   /*  promt */
		strcpy(t3,t1);
while(1)
{

label:	printf("%s>",t1);
	gets(input);       /* taking input */
	int less=0;
	int pp=0,key1,key2,in,out;
	char in_file[20],out_file[20];
	for(i=0;i<strlen(input);i++)
	{
		if(input[i]=='>' || input[i]=='<')
			less++;
		if(input[i]=='|')
			pp++;
	}

	if(less==0 && pp==0)
	{
			
	
	if(input[0]=='q' && input[1]=='u' && input[2]=='i' && input[3]=='t')
	{
		
		break;
	}
	
	split(input,a);                                                            /* split the input */
	while(1)
	{
		newtmp=waitpid(-1,&status,WNOHANG);
		
		if(newtmp<=0)
			break;
		else
		{
			for(i=0;i<point;i++)
				if(pro_arr[i].val==newtmp && pro_arr[i].check==0)              /* print if any process just terminated */
				{
					printf("%s %s with pid %d exited normally\n",pro_arr[i].pro,pro_arr[i].name,pro_arr[i].val);
					pro_arr[i].check=1;
					break;
				}  
			
		}
	}
	int key=0;
	i=0;
	if(input[0]=='\0')
	{
		strcpy(t1,t3);}
	else{
		
		while(a[i]!='\0')
			i++;
		if(i>0)
		{
			temp="&";
			m=strcmp(a[i-1],temp);                                     /* check if its a background process */
			if(m==0)
			{
				
			
				key=1;
				a[i-1]='\0';
				child1=fork();
				if(child1<0)
				{
					printf("Error Occured\n");
					exit(1);
				}
				else{
					pro_arr[point].check=0;
					strcpy(pro_arr[point].pro,a[0]);      /* store the details in structure if it's a background process */
					if(a[1]!='\0')
					strcpy(pro_arr[point].name,a[1]);
					else
						a[1]='\0';
					
					point++;
					
					if(child1==0)
					{
						
						execvp(a[0],a);                              /* execute the process */
						printf("Invalid Command\n");
						exit(1);
						
					}
					else
					{
						pro_arr[point-1].val=child1;            /* don't make parent wait when it background process */
					}
				}

				

			}
		}
		if(key==0)
		{
			
			temp="jobs";
			n=strcmp(a[0],temp);
			if(n==0)
			{
				jobs(pro_arr,point);
		
			}
			else
			{
				temp="overkill";
				n=strcmp(a[0],temp);
				if(n==0)
				{
					ok(pro_arr,point);
			
				}
				else
				{
					temp="fg";
					n=strcmp(a[0],temp);
					if(n==0)                                          /* call the correspondig fuction as per input */
					{ 
						fg(pro_arr,point,a[1]);
		
					}
					else{
						temp="kjob";
						n=strcmp(a[0],temp);
						if(n==0)
						{	
							kjobs(a,point,pro_arr);
		
						}
						else{
							temp="pinfo";
							n=strcmp(a[0],temp);
							if(n==0)
							{
								if(a[1]!='\0')
								{
									ty=atoi(a[1]);
									ipinfo(ty,home,home_len);
								}
								else
								{
									ipinfo(getpid(),home,home_len);
								}
			
							}
							else{
								temp="cd";
								n=strcmp(a[0],temp);
								if(n==0)
								{
									
									if(a[1]=='\0')
									{
										strcpy(t1,t3);
										chdir(home);
									}
									else
	/* prompt handling while 'cd' command */							{                                 

										if(chdir(a[1])==-1)
											perror(a[1]);
										getcwd(curr_dir,1000);
										check=strstr(curr_dir,home);
										if(check!=NULL)
										{
											strcpy(t1,t3);
											temp_len=strlen(curr_dir);
											i=0;
											for(j=home_len;j<temp_len;j++,i++)
											{
												temp_dir[i]=curr_dir[j];
											}
											temp_dir[i]='\0';
											strcat(t1,temp_dir);
										}
										else
										{
											strcpy(t1,t3);
											t1[strlen(t3)-1]='\0';
											strcat(t1,curr_dir);
										}
									}
								}
								else
								{
					
									child2=fork();
									if(child2<0)
									{
										printf("Eorror Occured\n");
										exit(1);
									}
									else{
										if(child2==0)
										{
											execvp(a[0],a);
											printf("Invalid Command\n");
	/* executing foreground process */						exit(1);
										}
										else
										{
											ppid=child2;
											waitpid(child2,&status,WUNTRACED);
											if(WIFSTOPPED(status))
											{
												pro_arr[point].val=child2;
	/* asking parent to wait and handling ctrl+Z */                                         pro_arr[point].check=0;
												strcpy(pro_arr[point].pro,a[0]);
												point++;
											}
										}
									}

								}
							}
						}
					}}}
			input[0]='\0';
		}}
	}
	else{ if(pp==0)							/*   I/O redirecting without pipes   */
	{
		
		char input2[500];	
		key1=0;
		key2=0;
		
  		int save_in,save_out;
	
		i=0;
			while(input[i]==' ' || input[i]=='\t')
						i++;
			k=0;
			while(input[i]!=' ' && input[i]!='\t' && input[i]!='<' && input[i]!='>'){
					input2[k]=input[i];k++;i++;}
			input2[k]='\0';
		
			k=0;
			while(input[i]==' ' || input[i]=='\t' || input[i]=='\n')
						i++;

			if(input[i]=='<'){						// if input from any file
				i++;
				while(input[i]==' ' || input[i]=='\t' || input[i]=='\n')
						i++;
				while(input[i]!=' ' && input[i]!='\t' && input[i]!='<' && input[i]!='>' && input[i]!='\0'){
					in_file[k]=input[i];k++;i++;}
					in_file[k]='\0';
				 save_in = dup(fileno(stdin));
				in = open(in_file, O_RDONLY);                           // open the input file
				if(in==-1)
				{ 
					perror(in_file);
					goto label;
				}
				dup2(in,0);				  
				close(in);
				key1=1;
				while(input[i]==' ' || input[i]=='\t' )
						i++;
			}
			
			if(i!=strlen(input)){
			if(input[i]=='>'){					// if output to any file
 			i++;
				
				while(input[i]==' ' || input[i]=='\t' )
						i++;
				k=0;
				while(input[i]!=' ' && input[i]!='\t' && input[i]!='\0' && input[i]!='\n'){
					out_file[k]=input[i];
					k++;i++;}
					out_file[k]='\0';
				 save_out = dup(fileno(stdout));
				out = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
													
				dup2(out, 1);close(out);           // open the output file
				key2=1;
			}}
			
			split(input2,a);
			child1=fork();
			if(child1<0)
			{
				printf("Error Occured..!1\n");
				exit(1);
			}
			else
			{
			if(child1==0)
			{
				execvp(a[0],a);                             // execute the command
				printf("Could not execute the command\n");
				exit(1);
			}
			else
			{
				waitpid(child1, &status, 0); 
				if(key2==1)
				{
					dup2(save_out, fileno(stdout));close(save_out);
				}
				if(key1==1)
				{
    					dup2(save_in, fileno(stdin));close(save_in);
				}
			}
			}
			

    			
   			
			


	}
	
	else{	
		                                             /* when one or more pipes are there */
		
		int save_out,save_in,count,k1,k2,error_flag=0;
		save_out = dup(fileno(stdout));save_in = dup(fileno(stdin));
		int pipes[4],in,out;
		split(input,a);
		i=0;
		
		char *second_arr[1000];
		j=0;
		for(i=0;i<=pp;i++)
		{
			k=0;k1=0;k2=0;
			while(strcmp(a[j],"|")!=0)
			{
				second_arr[k]=a[j];k++;
				j++;						/* divide the input as per pipes */
				if(a[j]=='\0'){break;}
				
			}j++;
			second_arr[k]='\0';
				
			pipe(pipes+2);
			child1=fork();	
			if(child1<0)
			{
				printf("Error Occured..!1\n");
				exit(1);
			}
			else
			{
			if(i==0)
			{
				for(count=0;count<k;count++)
				{
					if((strcmp(second_arr[count],"<"))==0)
					{
						second_arr[count]='\0';
						strcpy(in_file,second_arr[count+1]);     /* considering file input and stdin */
						in = open(in_file, O_RDONLY);
						if(in==-1)
						{
							
							
							if(child1==0)
							{
								perror(in_file);
								exit(1);
							}
							else
							{
								waitpid(child1, &status, 0); 
								goto label;
							}
						}
						dup2(in,0);
						k1=1;
						break;
					}
				}
				
			}
			if(i>0)
			{
				dup2(pipes[0],0);
				close(pipes[0]);                                         /* change the inputs of commands to pipe */
				close(pipes[1]);
			}
			if(child1==0)
			{
			if(i<pp)
			{
				dup2(pipes[3],1);
				close(pipes[2]);                                            /* change the outputs to pipe  */
				close(pipes[3]);
			}
			if(i==pp)
			{
				for(count=0;count<k;count++)
				{
					if((strcmp(second_arr[count],">"))==0)
					{
						second_arr[count]='\0';                // considering the last case and file output
						strcpy(out_file,second_arr[count+1]);
						out = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
						dup2(out,1);
						k2=1;
						break;
					}
				}
			}
			execvp(second_arr[0],second_arr);
			printf("Error..!!\n");
			exit(1);                                        // executing the command
			}
			else
			{
				waitpid(child1, &status, 0);                           //  parent waiting for child to finish
				pipes[0]=pipes[2];
				pipes[1]=pipes[3];                                // copying contents of one pipe to other
			}
			}
		}
		if(k1) close(in);                                          // closing I/O file, if there.
		if(k2) close(out);
		for(i=0;i<4;i++)
			close(pipes[i]);                                       // closing all the pipes finally
		
		dup2(save_in, fileno(stdin));close(save_in);
		dup2(save_out, fileno(stdout));close(save_out);             // recovering I/O file descriptors
		
	}
}}		
return 0;		
		
}
