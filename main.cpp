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
    Queue q1;
    int packet_max=10000;
    float lam1,lam2;
    int packet=0; // initialize the packet number to zero
    int popped;
    float lastsysout=0.0,sim_time=0.0;
    int server=0; // 0 signifies available and 1 says not available
    float* iat;
    float* srt;
    float* qin;
    float *qout,*sysout;// create dynamic arrays for inter arrival time 
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
        if(q1.lengthQueue()==0 ){            
            if(server==0){ //both the queue and the server are free.
            qin[packet]=times;            
            qout[packet]=times;            
            sysout[packet]=qout[packet]+srt[packet];
            lastsysout=sysout[packet];
            server=1;
            }
            else{ // queue is free ,server is not free               
               q1.pushQueue(packet);
               qin[packet]=times;
            }
        }
        else{ // queue is not empty
            if(server==0){//queue is not free, server is free
               q1.pushQueue(packet);
               qin[packet]=times;
               popped=q1.popQueue();
               qout[popped]=times;
               server=1;
               sysout[popped]=qout[popped]+srt[popped];
               lastsysout=sysout[packet];
            }
            else{ // queue and server are not free
                q1.pushQueue(packet);
                qin[packet]=times; 
            }     
        }       
    }
    for(int i=0;i<=packet;i++){
        cout<<"packet: "<<i<<" Qin :"<<qin[i]<<" Qout :"<<qout[i]<<" iat:"<<iat[i]<<" srt: "<<srt[i]<<" sysout:"<<sysout[i]<<endl;
    }
    
    return 0;
}

