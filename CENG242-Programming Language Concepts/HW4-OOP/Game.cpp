#include"Game.h"
#include"Board.h"
#include<iostream>
#include"Player.h"
#include"Archer.h"
#include"Fighter.h"
#include"Scout.h"
#include"Priest.h"
#include"Tank.h"
#include <algorithm>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Game::Game(uint maxTurnNumber, uint boardSize, Coordinate chest):
	board{Board(boardSize,&players,chest)}, turnNumber{0}, maxTurnNumber{maxTurnNumber}
{
}

Game::~Game(){
	
	for(int i=0; i<players.size(); i++){
		delete players[i];
	}
	players.clear();
}

void Game::addPlayer(int id, int x, int y, Team team, std::string cls){
	if(cls=="ARCHER"){
		Archer* new_player = new Archer(id,x,y,team);
		players.push_back(new_player);
	}
	else if(cls=="FIGHTER"){
		Fighter* new_player = new Fighter(id,x,y,team);
		players.push_back(new_player);
	}

	else if(cls == "PRIEST"){
		Priest* new_player = new Priest(id,x,y,team);
		players.push_back(new_player);
	}

	else if(cls == "SCOUT"){
		Scout* new_player = new Scout(id,x,y,team);
		players.push_back(new_player);
	}

	else{
		Tank* new_player = new Tank(id,x,y,team);
		players.push_back(new_player);
	}

}

bool Game::isGameEnded(){
	int barbarian_count=0;
	int knight_count=0;

	for(auto iter = players.begin(); iter!=players.end();iter++){
		if( (*iter)->getTeam() == BARBARIANS){
			if((*iter)->getCoord() == board.getChestCoordinates()){
				std::cout<<"Game ended at turn "<<std::to_string(turnNumber)<<". ";
				std::cout<<"Chest captured. Barbarian victory."<<std::endl;
				return true;
			}
			barbarian_count++;
		}
		if( (*iter)->getTeam() == KNIGHTS){
			knight_count++;
		}
	}
	if(barbarian_count == 0){
		std::cout<<"Game ended at turn "<<std::to_string(turnNumber)<<". ";
		std::cout<<"All barbarians dead. Knight victory."<<std::endl;
		return true;
	}
	if(knight_count == 0){
		std::cout<<"Game ended at turn "<<std::to_string(turnNumber)<<". ";
		std::cout<<"All knights dead. Barbarian victory."<<std::endl;
		return true;
	}
	if(turnNumber>=maxTurnNumber){
		std::cout<<"Game ended at turn "<<std::to_string(turnNumber)<<". ";
		std::cout<<"Maximum turn number reached. Knight victory."<<std::endl;
		return true;
	}
	return false;

}

Player* Game::findNearestPlayer(Player* player, bool is_target_an_enemy){
	int distance_to_nearest = -1;
	int nearest_id=-1;
	Player* nearest_player = NULL;

	for(auto target_player: players){
		if(target_player == player) continue;
		if(is_target_an_enemy && target_player->getTeam()==player->getTeam()) continue;
		if(!is_target_an_enemy && target_player->getTeam()!=player->getTeam()) continue;

		if(distance_to_nearest==-1){
			distance_to_nearest = player->getCoord() - target_player->getCoord();
			nearest_id = target_player->getID();
			nearest_player = target_player;
		}
		else if(distance_to_nearest > player->getCoord() - target_player->getCoord() ){
			distance_to_nearest = player->getCoord() - target_player->getCoord();
			nearest_id = target_player->getID();
			nearest_player=target_player;
		}
		else if(distance_to_nearest == player->getCoord() - target_player->getCoord() ){
			if(target_player->getID() < nearest_id){
				nearest_id = target_player->getID();
				nearest_player=target_player;
			}
		}
	}
	return nearest_player;
}

void Game::playTurn(){
	turnNumber++;
	std::cout<<"Turn "<< std::to_string(turnNumber)<<" has started."<<std::endl;
	std::sort(players.begin(),players.end(), [] ( Player* const &l_player, Player* const &r_player){
		return l_player->getID()>r_player->getID();
	});
	int player_size = players.size();
	
	for(int i=player_size-1; i>=0; i--){
		Player* its_your_turn = players[i];
		
		playTurnForPlayer(its_your_turn);
	}
	
}

Goal Game::playTurnForPlayer(Player* player){
	if(player->isDead()){
		std::cout<<"Player "<<player->getBoardID()<<" has died."<<std::endl;
		for(auto iter=(*board.getBoardPlayers()).begin(); iter!=(*board.getBoardPlayers()).end(); iter++){
			if((*iter)==player){
				delete player;
				(*board.getBoardPlayers()).erase(iter);
				
				return NO_GOAL;
				
			}
		}
		
	}
	for(int i=0; i<player->getGoalPriorityList().size(); i++){
		
		if(player->getGoalPriorityList()[i] == ATTACK){
			std::vector<Coordinate> attackable_coords = player->getAttackableCoordinates();
			int smallest_enemy_id = -1;
			Coordinate attack_to_this_coordinate(0,0);

			for(auto coord: attackable_coords){
				if(board.isCoordinateInBoard(coord) && board.isPlayerOnCoordinate(coord)){
					if(board[coord]->getTeam() != player->getTeam()){
						if(smallest_enemy_id == -1){
							attack_to_this_coordinate = coord;
							smallest_enemy_id = board[attack_to_this_coordinate]->getID();
						}
						else if(smallest_enemy_id > board[coord]->getID()){
							attack_to_this_coordinate = coord;
							smallest_enemy_id = board[attack_to_this_coordinate]->getID();
						}
					}
				}
			}
			if(smallest_enemy_id != -1){
				player->attack(board[attack_to_this_coordinate]);
				return ATTACK;
			}
		}

		else if(player->getGoalPriorityList()[i] == CHEST){ // UPDATE: do horizontal first
			std::vector<Coordinate> moveable_coords = player->getMoveableCoordinates();
			int distance = -1;
			Coordinate moveToCoordinate(0,0);
			for(auto moveable_coord: moveable_coords){
				if(board.isCoordinateInBoard(moveable_coord) && !board.isPlayerOnCoordinate(moveable_coord)){
					if(player->getCoord()-board.getChestCoordinates() > moveable_coord-board.getChestCoordinates()){
						if(distance == -1){
							moveToCoordinate = moveable_coord;
							distance = board.getChestCoordinates() - moveable_coord;
						}
						else if(board.getChestCoordinates() - moveable_coord == distance){
							if(abs(board.getChestCoordinates().x - moveToCoordinate.x) > abs(board.getChestCoordinates().x - moveable_coord.x)){
								moveToCoordinate = moveable_coord;
							}
						}
						else if(board.getChestCoordinates() - moveable_coord < distance){
							moveToCoordinate = moveable_coord;
							distance = board.getChestCoordinates() - moveable_coord;
						}
					}
					
				}
			}
			if(distance != -1){
				player->movePlayerToCoordinate(moveToCoordinate);
				return CHEST;
			}
			
		}
		else if(player->getGoalPriorityList()[i] == TO_ENEMY){
			std::vector<Coordinate> moveable_coords = player->getMoveableCoordinates();
			Player* nearest_player = findNearestPlayer(player, true);
			
			if(nearest_player == NULL) continue;
			
			int nearest_distance = -1;
			Coordinate move_to_this_coordinate(0,0);

			for(auto moveable_coord : moveable_coords){
				if(board.isCoordinateInBoard(moveable_coord) && !board.isPlayerOnCoordinate(moveable_coord)){
					if(nearest_player->getCoord() - player->getCoord() > nearest_player->getCoord() - moveable_coord){
						
						if(nearest_distance==-1){
							move_to_this_coordinate = moveable_coord;
							nearest_distance=nearest_player->getCoord() - moveable_coord;
						}
						
						else if(nearest_distance == nearest_player->getCoord() - moveable_coord){
							if( abs(move_to_this_coordinate.x - nearest_player->getCoord().x) > abs(moveable_coord.x - nearest_player->getCoord().x) ){
								move_to_this_coordinate = moveable_coord;
							}
						}
						else if(nearest_distance > nearest_player->getCoord() - moveable_coord ){
							move_to_this_coordinate = moveable_coord;
							nearest_distance=nearest_player->getCoord() - moveable_coord;
						}
					}
				}
			}
			if(nearest_distance != -1){
				player->movePlayerToCoordinate(move_to_this_coordinate);
				return TO_ENEMY;
			}
		}
		else if(player->getGoalPriorityList()[i] == TO_ALLY){
			std::vector<Coordinate> moveable_coords = player->getMoveableCoordinates();
			Player* nearest_player = findNearestPlayer(player, false);
			
			if(nearest_player == NULL) continue;
			
			int nearest_distance = -1;
			Coordinate move_to_this_coordinate(0,0);

			for(auto moveable_coord : moveable_coords){
				if(board.isCoordinateInBoard(moveable_coord) && !board.isPlayerOnCoordinate(moveable_coord)){
					if(nearest_player->getCoord() - player->getCoord() > nearest_player->getCoord() - moveable_coord){
						
						if(nearest_distance==-1){
							move_to_this_coordinate = moveable_coord;
							nearest_distance=nearest_player->getCoord() - moveable_coord;
						}
						
						else if(nearest_distance == nearest_player->getCoord() - moveable_coord){
							if( abs(move_to_this_coordinate.x - nearest_player->getCoord().x) > abs(moveable_coord.x - nearest_player->getCoord().x) ){
								move_to_this_coordinate = moveable_coord;
							}
						}
						else if(nearest_distance > nearest_player->getCoord() - moveable_coord ){
							move_to_this_coordinate = moveable_coord;
							nearest_distance=nearest_player->getCoord() - moveable_coord;
						}
					}
				}
			}
			if(nearest_distance != -1){
				player->movePlayerToCoordinate(move_to_this_coordinate);
				return TO_ALLY;
			}
		}
		else if(player->getGoalPriorityList()[i] == HEAL){
			std::vector<Coordinate> healable_coords = player->getHealableCoordinates();
			bool flag_healed = false;
			for(auto healable_coord : healable_coords){
				if(board.isCoordinateInBoard(healable_coord) && board.isPlayerOnCoordinate(healable_coord)){
					if(board[healable_coord]->getTeam() == player->getTeam()){
						player->heal(board[healable_coord]);
						flag_healed = true;
					}
				}
			}
			if(flag_healed == true) return HEAL;
		}

	}
	
}