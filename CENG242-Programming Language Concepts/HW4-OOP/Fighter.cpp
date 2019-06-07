#include"Fighter.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Fighter::Fighter(uint id,int x, int y, Team team):Player(id,x,y,team){
		HP=400;
	}

	int Fighter::getAttackDamage() const{
		return 100;
	}

	int Fighter::getHealPower() const{ 
		return 0;
	}

	int Fighter::getMaxHP() const{
		return 400;
	}

	std::vector<Goal> Fighter::getGoalPriorityList(){
		std::vector<Goal> prio_list = {ATTACK,TO_ENEMY,CHEST};
		return prio_list;
	}
	const std::string Fighter::getClassAbbreviation() const{
		if(team == BARBARIANS){
			return "FI";
		}
		return "fi";
	}

	std::vector<Coordinate> Fighter::getAttackableCoordinates(){
		std::vector<Coordinate> result;
		
		result.push_back(Coordinate(coordinate.x - 1, coordinate.y));		
		result.push_back(Coordinate(coordinate.x + 1, coordinate.y));
		result.push_back(Coordinate(coordinate.x , coordinate.y-1));		
		result.push_back(Coordinate(coordinate.x, coordinate.y+1));

		return result;
	}

	std::vector<Coordinate> Fighter::getMoveableCoordinates(){
		return Fighter::getAttackableCoordinates();
	}

	std::vector<Coordinate> Fighter::getHealableCoordinates(){
		std::vector<Coordinate> v;
		return v;

	}