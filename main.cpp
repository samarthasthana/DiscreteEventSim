/* 
 * File:   main.cpp
 * Author: Sam
 *
 * Created on September 18, 2013, 7:55 PM
 */

//binwidth=5
//bin(x,width)=width*floor(x/width)
//
//plot 'datafile' using (bin($1,binwidth)):(1.0) smooth freq with boxes

#include <stdlib.h>
#include<math.h>
#include<iostream>
#include <fstream>
using namespace std;

class Node{
private:
    int packno;
    Node* next;
public:
    Node(){
        packno=0;
        next=NULL;
    }
    int Packno(){
        return packno;
    }
    Node* Next(){
        return next;
    }
    void setPackno(int val){
        packno=val;
    }
    void setNext(Node* n){
        if(n==NULL)
            next=NULL;
        else
            next=n;
    } 
};

class Queue{
private:
    Node* head;
public:
    Queue(){
        head=NULL;        
    }
    Node* Head(){
        return head;
    }
    void pushQueue(int packno){
        Node* leaf=new Node(); 
        leaf->setNext(NULL);
        leaf->setPackno(packno);
        Node* temp;
        if(head==NULL){           
            head=leaf;
        }
        else{
            temp=head;
            while(temp->Next()!=NULL){
                temp=temp->Next();
            }
            temp->setNext(leaf);
        }
    }
    int popQueue(){
        int pack;
        if(head==NULL){
          return 0;  
        }
        else{
            pack=head->Packno();
            head=head->Next();
            return pack;
        }        
    }
    int lengthQueue(){
        Node* temp;
        int length=0;
        if(head==NULL){
            return 0;
        }
        temp=head;
        while(temp!=NULL){
            length++;
            temp=temp->Next();            
        }        
        return length;    
    }
    void printQueue(){
        Node* temp;
        if(head==NULL){
            cout<<"EMPTY"<<endl;
        }
        temp=head;
        while(temp!=NULL){
            cout<<temp->Packno()<<endl;
            temp=temp->Next();            
        }         
    }
};


 float newRand(){
 return ((float)rand()/RAND_MAX);   
}
 
 float genExpo(float lam){
     float temp;
     temp=newRand();
     return (-1/lam)*log(temp);     
 }


int main(int argc, char** argv) {

    float times=0.00; // initialize the time to zero
    Queue *q1;
    q1=new Queue();
    long packet_max=5000000;
    float lam1,lam2;
    long packet=0; // initialize the packet number to zero
    long popped=0;
    float lastsysout=0.0,sim_time=0.0;
    int server=0; // 0 signifies available and 1 says not available
    float* iat;
    float* srt;
    float* qin;
    float *qout,*sysout;// create dynamic arrays for inter arrival time 
    float *avgwait, *avgsys,*avgser;
    int *avgnum;
    ofstream file1;
    file1.open("results.dat");
                                  // service time , queue in and out, system in and out
    cout<<"Enter the simulation time"<<endl;
    cin>>sim_time;
    cout<<"Enter the mean for inter arrival time distribution"<<endl;
    cin>>lam1;
    cout<<"Enter the mean for service time distribution"<<endl;
    cin>>lam2;
    packet_max++; // to accomodate for starting at 1 rather than 0
    iat= new float[packet_max];
    srt= new float[packet_max];
    qin= new float[packet_max];
    qout= new float[packet_max];
    sysout= new float[packet_max];
    avgwait= new float[packet_max];
    avgsys=new float[packet_max];
    avgser=new float[packet_max];
    avgnum=new int[packet_max];
    for(int i=1;i<packet_max;i++){
    iat[i]=0.0;
    srt[i]=0.0;
    qin[i]=0.0;
    qout[i]=0.0;
    sysout[i]=0.0;
    avgwait[i]=0.0;
    avgsys[i]=0.0;
    avgser[i]=0.0;
    avgnum[i]=0;
    }
    packet_max--;
    srand (time(NULL));
    while(times<=sim_time){        
        packet++;        
        iat[packet]=genExpo(lam1);
        times=times+iat[packet];
        srt[packet]=genExpo(lam2);
        if(server==1){
            if(lastsysout<=times){
                server=0;
            }
        }
        if(q1->lengthQueue()==0 ){            
            if(server==0){ //both the queue and the server are free.
            qin[packet]=times;            
            qout[packet]=times;            
            sysout[packet]=qout[packet]+srt[packet];
            lastsysout=sysout[packet];      
            avgwait[packet]=(qout[packet]-qin[packet]);
            avgsys[packet]=(sysout[packet]-qin[packet]);
            avgser[packet]=(sysout[packet]-qout[packet]);
            server=1;
            avgnum[packet]=q1->lengthQueue();
            }
            else{ // queue is free ,server is not free               
               q1->pushQueue(packet);
               qin[packet]=times;
               avgnum[packet]=q1->lengthQueue();
            }
        }
        else{ // queue is not empty
            if(server==0){//queue is not free, server is free
               q1->pushQueue(packet);
               qin[packet]=times;
               avgnum[packet]=q1->lengthQueue();
               popped=q1->popQueue();
               qout[popped]=times;
               server=1;
               sysout[popped]=qout[popped]+srt[popped];
               lastsysout=sysout[packet];
               avgwait[popped]=qout[popped]-qin[popped];
               avgsys[popped]=(sysout[popped]-qin[popped]);
               avgser[popped]=(sysout[popped]-qout[popped]);
            }
            else{ // queue and server are not free
                avgnum[packet]=q1->lengthQueue();
                q1->pushQueue(packet);
                qin[packet]=times; 
               
            }     
        }       
    }
    file1<<"Packet \t Qin \t Qout \t IAT \t SRT \t SysOut \t AvgSys \t AvgWait \t AvgSer \t AvgNum \n";
    for(int i=1;i<=packet;i++){
        file1<<i<<"\t"<<qin[i]<<"\t"<<qout[i]<<"\t"<<iat[i]<<"\t"<<srt[i]<<"\t"<<sysout[i]<<"\t"<<avgsys[i]<<"\t"<<avgwait[i]<<"\t"<<avgser[i]<<"\t"<<avgnum[i]<<endl;
//        file1<<"packet: "<<i<<" Qin :"<<qin[i]<<" Qout :"<<qout[i]<<" iat:"<<iat[i]<<" srt: "<<srt[i]<<" sysout:"<<sysout[i]<<endl;
//        file1<<"AvgSRT"<<avgsrt[i]<<"\t AvgSys"<<avgsys[i]<<"\t AvgWait"<<avgwait[i]<<"\t Avg tot time"<<avgtot[i]<<"\t avgnum: "<<avgnum[i]<<endl;        
    }
    file1.close();
    return 0;
}

