#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "functions.h"
using namespace std;

int main()
{
    //Initialization
    std::unordered_map<string,string> my;
    char *command[100];
    char *param[20];
    char *arg[20];
    char *command2[100];
    char *param2[20];
    char *arg2[20];
    int status;
   bool pip=0,red=0,rred=0;
   int pip_count=0;
    //Loop

    while(1)
    {
        red=0;
        pip=0;
        rred=0;
        display();
        //cout<<*param<<endl;
        read_inp(command,param,arg,red,pip,rred,command2,param2,arg2);
        historian(arg,arg2);
        if(strcmp(arg[0],"exit")==0)
        {
            break;
        }
        //cout<<*param<<endl;
        if(strcmp(arg[0],"cd")==0)
        {
           bicmd(arg[0],param);
        }
        if(strcmp(arg[0],"history")==0)
        {
           //Display history file
           char *arg3[20];
           arg3[0]="cat";
           arg3[1]="hist.txt";
           arg3[2]=NULL;
           sycmd(arg3[0],arg3);
           continue;
        }
        if(pip)
        {
            //cout<<*command<<endl;
            //cout<<*arg<<endl;
            //cout<<*command2<<endl;
            //cout<<*arg2<<endl;
            int pfd[2];
            int p1,p2,savedout,savedin;
            savedout=dup(1);
            savedin=dup(0);
            if(pipe(pfd)<0)
            {
                cout<<"Pipe init failed"<<endl;
            }
            p1=fork();
            if(p1==0)
            {
                
                close(pfd[0]);
                dup2(pfd[1],1);
                //close(pfd[1]);
                if (execvp(arg[0], arg) < 0) 
                { 
                    cout<<"\nCould not execute command 1.."; 
                    exit(0); 
                }

                exit(0);
            } 
            else
            {
                
                    wait(NULL);
                    dup2(pfd[0],0);
                    close(pfd[1]);
                    sycmd(arg2[0],arg2);
                    // close(pfd[0]);
                    dup2(savedout,1);
                    dup2(savedin,0);
                 
            }
            

        }
        else if(red)
        {
            int saved=dup(1);
            int fd = open(command[1], O_WRONLY|O_TRUNC|O_CREAT, 0644);
            close(1);
            dup2(fd,1);
            sycmd(arg[0],arg);
            dup2(saved,1);
        }
        else if(rred)
        {
            int saved=dup(1);
            int fd = open(command[1], O_WRONLY|O_APPEND|O_CREAT, 0644);
            close(1);
            dup2(fd,1);
            sycmd(arg[0],arg);
            dup2(saved,1);
            //cout<<"Double redirection"<<endl;
            //cout<<arg[0]<<" : ";
        }
        else
        {
            //cout<<arg[0]<<endl;
            sycmd(arg[0],arg);
        }

        
    }
    //End

    return 0;
}