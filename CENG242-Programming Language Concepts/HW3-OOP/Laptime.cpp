#include "Laptime.h"
#include <iostream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Laptime::Laptime(int rhs_laptime): laptime{rhs_laptime}, next{NULL} {}

/*
Laptime::Laptime(const Laptime& rhs){
    
    this->laptime = rhs.laptime;
    Laptime* rhs_traveller = rhs.next;
	Laptime* temp = this;
	
    while(rhs_traveller != NULL){
    	Laptime* new_laptime = new Laptime(rhs_traveller->laptime);
    	temp->next = new_laptime;
    	temp = temp->next;
    	rhs_traveller = rhs_traveller->next;
	}
	temp->next = NULL;
}*/

Laptime::Laptime(const Laptime& rhs){
	this->laptime = rhs.laptime;
	this->next = rhs.next;
}

Laptime::~Laptime(){}

/*
Laptime::~Laptime(){
	Laptime* this_traveller=this->next;
	Laptime* this_next_traveller;
	
	while(this_traveller != NULL){
		this_next_traveller = this_traveller->next;
		delete this_traveller;
		this_traveller = this_next_traveller;
	}
	
}
*/

void Laptime::addLaptime(Laptime* rhs_next){ 
	Laptime* temp = this->next;
	this->next = rhs_next;
	this->next->next = temp;
}

bool Laptime::operator<(const Laptime& rhs) const{
	return this->laptime < rhs.laptime;
}

bool Laptime::operator>(const Laptime& rhs) const{
	return this->laptime > rhs.laptime;
}

 /*
 Laptime Laptime::operator[](const int desired_lap) const{ 	 	 	
 	if(desired_lap ==0){
 		return *this;
	}
	else{
		int curr_index = 1;
		Laptime* temp=this->next;
	 	while(curr_index != desired_lap){
 			temp = temp->next;
 			curr_index++;
	 	}
	 	return *temp;
	}
 }*/
 
 
Laptime& Laptime::operator+(const Laptime& rhs){
	this->laptime +=  rhs.laptime;
	
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Laptime& laptime){
	int minute = laptime.laptime/60000;
	int second = (laptime.laptime - minute*60000)/1000;
	int milisecond = laptime.laptime - (minute*60000) - (second*1000);
	
	os<<minute<<":"<<second<<"."<<milisecond;
	return os;
}
 
/* 
int main(){
	
	Laptime a(60000);
	Laptime b(10);
	a.addLaptime(&b);
	

	
	std::cout<<a;
	
	//printf("%d %d\n", a.getVal(),a[1].getVal());
	
 	
	return 0;
}*/
