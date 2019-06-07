#include"Priest.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Priest::Priest(uint id,int x, int y, Team team):Player(id,x,y,team){
		HP=150;
	}

	int Priest::getAttackDamage() const{
		return 0;
	}

	int Priest::getHealPower() const{ 
		return 50;
	}

	int Priest::getMaxHP() const{
		return 150;
	}

	std::vector<Goal> Priest::getGoalPriorityList(){
		std::vector<Goal> prio_list = {HEAL,TO_ALLY,CHEST};
		return prio_list;
	}
	const std::string Priest::getClassAbbreviation() const{
		if(team == BARBARIANS){
			return "PR";
		}
		return "pr";
	}

	std::vector<Coordinate> Priest::getAttackableCoordinates(){
		std::vector<Coordinate> result;
		
		return result;
	}

	std::vector<Coordinate> Priest::getMoveableCoordinates(){
		return Priest::getHealableCoordinates();
	}

	std::vector<Coordinate> Priest::getHealableCoordinates(){
		std::vector<Coordinate> result;
		for(int i=-1; i<=1; i++){
			for(int j=-1;j<=1;j++){
				if(!(i==0 && j==0 )){
					result.push_back(Coordinate(coordinate.x+i,coordinate.y+j));
				}
			}
		}
		return result;

	}