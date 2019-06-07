#include"Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Board::Board(uint _size, std::vector<Player*>* _players, Coordinate _chest):
	size{_size}, players{_players}, chest{_chest} {}

Board::~Board() = default;
bool Board::isCoordinateInBoard(const Coordinate& c){
	if(c.x>=0 && c.x<size && c.y>=0 && c.y<size){
		return true;
	}
	return false;
}
bool Board::isPlayerOnCoordinate(const Coordinate& c){
	for(auto iter=players->begin(); iter!=players->end();iter++){
		if( (*iter)->getCoord() == c) return true;
	}
	return false;
}
Player * Board::operator [](const Coordinate& c){
	Player* result =NULL;
	for(auto iter=players->begin(); iter!=players->end();iter++){
		if( (*iter)->getCoord() == c){
			result = (*iter);
			break;
		}
	}
	return result;
}

Coordinate Board::getChestCoordinates(){
	return chest;
}

void Board::printBoardwithID(){
	for(int j=0; j<size;j++){
		for(int i=0; i<size; i++){
			Coordinate current_coord = Coordinate(i,j);
			bool found_flag=false;
			bool chest_flag = false;
			

			for(auto iter=players->begin(); iter!=players->end();iter++){
				if( (*iter)->getCoord() == current_coord){
					std::cout<<(*iter)->getBoardID()<<" ";
					found_flag=true;
					break;
				}
				if(current_coord == chest){
					chest_flag=true;
					continue;
				}
			}
			if(!found_flag){
				if(chest_flag){
					std::cout<<"Ch ";
				}
				else{
					std::cout<<"__ ";
				}
			}
		}
		std::cout<<std::endl;
	}
}

void Board::printBoardwithClass(){
	for(int j=0; j<size;j++){
		for(int i=0; i<size; i++){
			Coordinate current_coord = Coordinate(i,j);
			bool found_flag=false;
			bool chest_flag = false;
			

			for(auto iter=players->begin(); iter!=players->end();iter++){
				if( (*iter)->getCoord() == current_coord){
					std::cout<<(*iter)->getClassAbbreviation()<<" ";
					found_flag=true;
					break;
				}
				if(current_coord == chest){
					chest_flag=true;
					continue;
				}
			}
			
			if(!found_flag){
				if(chest_flag){
					std::cout<<"Ch ";
				}
				else{
					std::cout<<"__ ";
				}
			}
		}
		std::cout<<std::endl;
	}
}