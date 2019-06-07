#include"Player.h"
#include "Coordinate.h"
#include "Fighter.h"
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/


Player::Player(uint id,int x, int y, Team team): 
	id{id}, coordinate{Coordinate(x,y)}, team{team}
	{}

uint Player::getID() const{
	return id;
}
const Coordinate& Player::getCoord() const{
	return coordinate;
}

int Player::getHP() const{ return HP; }

Team Player::getTeam() const{ return team;}

std::string Player::getBoardID(){
	std::string result="";
	if(id<10) result ="0";
	result += std::to_string(id);
	return result;							
}

bool Player::attack(Player* enemy){
	enemy->HP -= this->getAttackDamage();
	std::cout<<"Player "<<this->getBoardID()<<" attacked Player "<<enemy->getBoardID()<<" ("<< this->getAttackDamage()<<")"<<std::endl;
	return enemy->isDead() <=0;

}

void Player::heal(Player* ally){
	if(ally->getMaxHP() - ally->getHP() < this->getHealPower()){
		ally->HP = ally->getMaxHP();
	}
	else{
		ally->HP += this->getHealPower();
	}
	std::cout<<"Player "<<this->getBoardID()<<" healed Player "<<ally->getBoardID()<<std::endl;
}

void Player::movePlayerToCoordinate(Coordinate new_coordinate){
	
	std::cout<<"Player "<<this->getBoardID()<<" moved from (";
	std::cout<<this->coordinate.x<<"/"<<this->coordinate.y<<") to (";
	std::cout<<new_coordinate.x<<"/"<<new_coordinate.y<<")"<<std::endl;
	this->coordinate = new_coordinate;
}
bool Player::isDead() const{ return HP <=0;}