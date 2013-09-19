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
            while(temp!=NULL){
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

    srand (time(NULL));
    ofstream file;
    file.open("Exponential.dat");
    for (int i=0;i<10000;i++){
        file<<genExpo(1.0)<<endl;        
    }   
    file.close();
    cout<<"done";
    return 0;
}
