#include "Race.h"
#include <iostream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Race::Race(std::string given_race_name): race_name {given_race_name},
								   average_laptime {Laptime(Utilizer::generateAverageLaptime())},
								   head {NULL}
									{}

Race::Race(const Race& rhs_race):average_laptime{rhs_race.average_laptime.getLaptime()},
								 race_name{rhs_race.race_name},
								 head{NULL}			{	
	
	Car* rhs_car_traveller = rhs_race.head;
	Car* this_car_traveller;
	
	if(rhs_car_traveller){
		Car* new_car = new Car(rhs_car_traveller->getDriverName());
		new_car->setPerformance(rhs_car_traveller->getPerformance());
		this->head = new_car;
		
		rhs_car_traveller = rhs_car_traveller->getNext();
		this_car_traveller = this->head;
		while(rhs_car_traveller){
			Car* new_car = new Car(rhs_car_traveller->getDriverName());
			new_car->setPerformance(rhs_car_traveller->getPerformance());
			this_car_traveller->setNext(new_car);
			this_car_traveller = this_car_traveller->getNext();
			rhs_car_traveller = rhs_car_traveller->getNext();
		}
	}
	
}

Race::~Race(){ // TO DO: IT DOES NOT DYNAMICLY DELETES LAPTIMES NOW !! 
	Car* temp_car;
	Car* this_car_traveller=this->head;
	
	while(this_car_traveller){
		temp_car=this_car_traveller->getNext();
		delete this_car_traveller;
		this_car_traveller = temp_car;
	}
	
}

std::string Race::getRaceName() const{
	return race_name;
	}

void Race::addCartoRace(){
	std::string car_name = Race::randomName();
	Car* new_car = new Car(car_name);
	
	Car* temp = this->head;   // TO DO: NOW NEW CAR ADDED TO THE BEGINNING, CHECK IT
	this->head = new_car;
	this->head->setNext(temp);
	
}

void Race::addCartoRace(Car& car){
	Car* temp = this->head;   // TO DO: NOW NEW CAR ADDED TO THE BEGINNING, CHECK IT
	this->head = &car;
	this->head->setNext(temp);
}
int Race::getNumberOfCarsinRace(){
	int total_cars = 0;
	
	Car* this_car_traveller = this->head;
	
	while(this_car_traveller){
		total_cars++;
		this_car_traveller = this_car_traveller->getNext();
	}
	return total_cars;
}

Car Race::operator[](const int car_in_position){
	Car* this_car_traveller = this->head;
	int curr_car =0;
	
	while(curr_car < car_in_position){
		this_car_traveller = this_car_traveller->getNext();
		curr_car++;
		if(this_car_traveller==NULL){
			printf("Car does not exist\n");
			exit(1);
		}
	}
	return (*this_car_traveller);
}

Car Race::operator[](std::string driver_name){
	Car* this_car_traveller = this->head;
	
	while(this_car_traveller){
		if(this_car_traveller->getDriverName() == driver_name){
			return (*this_car_traveller);
		}
		this_car_traveller = this_car_traveller->getNext();
	}
	printf("Car with given name does not exist\n");
	exit(1);
}
int main(){
	Race a("alemciler");
	Car* b = new Car("ayi hazan");
	a.addCartoRace(*b);
	Laptime avg(60000);
	a["ayi hazan"].Lap(avg);
	a.addCartoRace();
	std::cout<<a["ayi hazan"].getHead()->getLaptime()<<std::endl;
	//std::cout<<a.randomName()<<std::endl;
	return 0;
	
}
