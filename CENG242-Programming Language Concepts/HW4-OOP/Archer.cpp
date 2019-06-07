#include"Archer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Archer::Archer(uint id,int x, int y, Team team):Player(id,x,y,team){
		HP=200;
	}

	int Archer::getAttackDamage() const{
		return 50;
	}

	int Archer::getHealPower() const{ 
		return 0;
	}

	int Archer::getMaxHP() const{
		return 200;
	}

	std::vector<Goal> Archer::getGoalPriorityList(){
		std::vector<Goal> prio_list = {ATTACK};
		return prio_list;
	}
	const std::string Archer::getClassAbbreviation() const{
		if(team == BARBARIANS){
			return "AR";
		}
		return "ar";
	}

	std::vector<Coordinate> Archer::getAttackableCoordinates(){
		std::vector<Coordinate> result;
		
		result.push_back(Coordinate(coordinate.x - 2, coordinate.y));		
		result.push_back(Coordinate(coordinate.x + 2, coordinate.y));		
		result.push_back(Coordinate(coordinate.x , coordinate.y-2));		
		result.push_back(Coordinate(coordinate.x, coordinate.y+2));
		result.push_back(Coordinate(coordinate.x - 1, coordinate.y));		
		result.push_back(Coordinate(coordinate.x + 1, coordinate.y));		
		result.push_back(Coordinate(coordinate.x , coordinate.y-1));		
		result.push_back(Coordinate(coordinate.x, coordinate.y+1));

		return result;
	}

	std::vector<Coordinate> Archer::getMoveableCoordinates(){
		std::vector<Coordinate> result;
		return result;
	}

	std::vector<Coordinate> Archer::getHealableCoordinates(){
		std::vector<Coordinate> v;
		return v;

	}