#include"InputParser.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Game* InputParser::parseGame(){
	int size_of_board;
	int chest_x, chest_y;
	int max_turn_number;
	int num_of_players;
	int p_id, p_x, p_y;
	std::string  p_class, p_team;
	std::cin>>size_of_board>>chest_x>>chest_y>>max_turn_number>>num_of_players;

	static Game* create_game = new Game(max_turn_number, size_of_board, Coordinate(chest_x,chest_y));

	for(int i=0; i<num_of_players; i++){
		std::cin>>p_id>>p_class>>p_team>>p_x>>p_y;
		Team team;
		
		if(p_team == "BARBARIAN") team=Team(BARBARIANS);
		if(p_team == "KNIGHT") team = Team(KNIGHTS);

		create_game->addPlayer(p_id, p_x, p_y, team, p_class);
	}
	return create_game;
}