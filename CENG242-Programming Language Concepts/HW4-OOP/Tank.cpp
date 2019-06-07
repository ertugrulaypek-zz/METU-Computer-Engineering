#include"Tank.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Tank::Tank(uint id,int x, int y, Team team):Player(id,x,y,team){
		HP=1000;
	}

	int Tank::getAttackDamage() const{
		return 25;
	}

	int Tank::getHealPower() const{ 
		return 0;
	}

	int Tank::getMaxHP() const{
		return 1000;
	}

	std::vector<Goal> Tank::getGoalPriorityList(){
		std::vector<Goal> prio_list = {TO_ENEMY,ATTACK,CHEST};
		return prio_list;
	}
	const std::string Tank::getClassAbbreviation() const{
		if(team == BARBARIANS){
			return "TA";
		}
		return "ta";
	}

	std::vector<Coordinate> Tank::getAttackableCoordinates(){
		std::vector<Coordinate> result;
		
		result.push_back(Coordinate(coordinate.x - 1, coordinate.y));		
		result.push_back(Coordinate(coordinate.x + 1, coordinate.y));		
		result.push_back(Coordinate(coordinate.x , coordinate.y-1));		
		result.push_back(Coordinate(coordinate.x, coordinate.y+1));

		return result;
	}

	std::vector<Coordinate> Tank::getMoveableCoordinates(){
		return Tank::getAttackableCoordinates();
	}

	std::vector<Coordinate> Tank::getHealableCoordinates(){
		std::vector<Coordinate> v;
		return v;

	}