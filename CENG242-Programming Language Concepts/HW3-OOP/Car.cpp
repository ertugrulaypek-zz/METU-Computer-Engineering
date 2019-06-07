#include "Car.h"
#include <iostream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Car::Car(std::string given_driver_name)
			:performance{Utilizer::generatePerformance()}, driver_name {given_driver_name}, head {NULL}, next {NULL}
	{}
	
Car::Car(const Car& rhs){
	this->driver_name = rhs.driver_name;
	this->performance = rhs.performance;
	Car* rhs_car_traveller = rhs.next;
	Car* this_car_traveller = NULL;
	Laptime* rhs_laptime_traveller=rhs.head;
	Laptime* new_laptime_head = NULL;	
	Laptime* temp;
	
	if(rhs_laptime_traveller != NULL){
		new_laptime_head = new Laptime(rhs_laptime_traveller->getLaptime());
		this->head = new_laptime_head;
		
		while(rhs_laptime_traveller->getNext()){
			rhs_laptime_traveller = rhs_laptime_traveller->getNext();
			temp = new Laptime(rhs_laptime_traveller->getLaptime());
			new_laptime_head->addLaptime(temp);
		}
	}
	else{
		this->head=NULL;
	}
	
	if(rhs_car_traveller != NULL){
		this->next = rhs_car_traveller;
		/*
		while(rhs_car_traveller != NULL){ // TO DO :: TRAVERSE ALL CARS !!!!
			
		}*/
	}
	else{
		this->next = NULL;
	}
		
}
Car::~Car(){
	
	Laptime* temp=NULL;
	Laptime* laptime_traveller=NULL;
	Car* car_traveller = this;
	while(car_traveller != NULL){
		laptime_traveller = car_traveller->head;
		
		while(laptime_traveller!=NULL){
			temp=laptime_traveller->getNext();
			delete laptime_traveller;
			laptime_traveller = temp;
		}						
		car_traveller = car_traveller->next;
	}
}
	
std::string Car::getDriverName() const{return this->driver_name;}

double Car::getPerformance() const{return this->performance;}

void Car::addCar(Car* next_car){

	Car* temp = this->next;
	this->next = next_car;
	this->next->next = temp;
	
}
bool Car::operator<(const Car& rhs) const{
	int sum_rhs=0;
	int sum_this=0;
	Laptime *this_traveller=this->head;
	Laptime *rhs_traveller=rhs.head;
	while(this_traveller){
		sum_this+=this_traveller->getLaptime();
		this_traveller=this_traveller->getNext();
	}
	while(rhs_traveller){
		sum_rhs=rhs_traveller->getLaptime();
		rhs_traveller=rhs_traveller->getNext();
	}
	return sum_this < sum_rhs;
	
}

bool Car::operator>(const Car& rhs) const{
	int sum_rhs=0;
	int sum_this=0;
	Laptime *this_traveller=this->head;
	Laptime *rhs_traveller=rhs.head;
	while(this_traveller){
		sum_this+=this_traveller->getLaptime();
		this_traveller=this_traveller->getNext();
	}
	while(rhs_traveller){
		sum_rhs=rhs_traveller->getLaptime();
		rhs_traveller=rhs_traveller->getNext();
	}
	return sum_this > sum_rhs;
	
}
	
Laptime Car::operator[](const int desired_lap) const{
	Laptime* laptime_traveller = this->head;
	int curr_lap = 0;
	int return_value = 0;
	
	while(laptime_traveller){
		if(curr_lap == desired_lap){
			return_value = laptime_traveller->getLaptime();
			break;
		}
		curr_lap++;
		laptime_traveller = laptime_traveller->getNext();
	}
	return return_value;
}

void Car::Lap(const Laptime& average_laptime){ // QUESTION: USE average_laptime DIRECTLY OR GET A MEAN BY TRAVERSING IT ? 

	const double car_performance = this->performance;
	
	const int avg_laptime = average_laptime.getLaptime();
	
	int calculated_laptime = avg_laptime + Utilizer::generateLaptimeVariance(car_performance);
	Laptime* new_laptime = new Laptime(calculated_laptime);
	
	Laptime* this_laptime_traveller = this->head;
	
	if(this_laptime_traveller){
		while(this_laptime_traveller->getNext()){
			this_laptime_traveller=this_laptime_traveller->getNext();
		}
		this_laptime_traveller->setNext(new_laptime);
	}
	else{
		this->head = new_laptime;
	}

}
	
std::ostream& operator<<(std::ostream& os, const Car& car){
	int total_laptime = 0;
	int fastest_laptime = -1;
	int latest_laptime = 0;
	int curr_laptime=0;
	Laptime* laptime_traveller = car.head;
	
	while(laptime_traveller != NULL){
		
		curr_laptime=laptime_traveller->getLaptime();
		total_laptime+=curr_laptime;
		
		if(curr_laptime>latest_laptime){
			latest_laptime=curr_laptime;
		}
		
		if( (fastest_laptime == -1) || curr_laptime<fastest_laptime) {
			fastest_laptime = curr_laptime;
		}
		laptime_traveller = laptime_traveller->getNext();
	}
	if(fastest_laptime==-1){
		fastest_laptime=0;
	}
	
	car.getSurname(car.getDriverName(),os);
	os<<"--";
	car.getZeroFilled(latest_laptime,os);
	os<<"--";
	car.getZeroFilled(fastest_laptime,os);
	os<<"--";
	car.getZeroFilled(total_laptime,os);
	return os;
	
}


int main(){
	
	Car a("selami koniciva");
	Laptime avg(60000);
	a.Lap(avg);
	a.Lap(avg);
	//std::cout<<a.gethead()->getNext()->getLaptime();
	//printf("constr laptime\n");
	

	//printf("%f",a.getPerformance());
	std::cout<<a<<std::endl;
	
	
	
	return 0 ;
}
