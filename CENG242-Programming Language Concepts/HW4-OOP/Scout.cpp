#include"Scout.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Scout::Scout(uint id,int x, int y, Team team):Player(id,x,y,team){
		HP=125;
	}

	int Scout::getAttackDamage() const{
		return 25;
	}

	int Scout::getHealPower() const{ 
		return 0;
	}

	int Scout::getMaxHP() const{
		return 125;
	}

	std::vector<Goal> Scout::getGoalPriorityList(){
		std::vector<Goal> prio_list = {CHEST,TO_ALLY,ATTACK};
		return prio_list;
	}
	const std::string Scout::getClassAbbreviation() const{
		if(team == BARBARIANS){
			return "SC";
		}
		return "sc";
	}

	std::vector<Coordinate> Scout::getAttackableCoordinates(){
		std::vector<Coordinate> result;
		for(int i=-1; i<=1; i++){
			for(int j=-1;j<=1;j++){
				if( !(i==0 && j==0 )){
					result.push_back(Coordinate(coordinate.x+i,coordinate.y+j));
				}
			}
		}
		
		return result;
	}

	std::vector<Coordinate> Scout::getMoveableCoordinates(){
		return Scout::getAttackableCoordinates();
	}

	std::vector<Coordinate> Scout::getHealableCoordinates(){
		std::vector<Coordinate> v;
		return v;

	}