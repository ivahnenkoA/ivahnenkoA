// Function definitions for the game_process base class and its derived classes
// Anna Ivahnenko  15/05/21

#include <iostream>    
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <random>
#include "game_process.h"
#include "game_component.h"

using namespace proc;

//Pseudo random number generator acting like a 6-dice
int dice_throw()
{
    int dice = rand()%6 + 1;
    std::cout<<'\n'<<"Dice throw: "<<dice<<". ";
    return dice;
}

std::vector<std::string> game_process::get_answers() {return correct_answers;};

void global::set_answers(std::string correct_room, std::string correct_suspect, std::string correct_weapon) 
{
    correct_answers.push_back(correct_room);
    correct_answers.push_back(correct_suspect);
    correct_answers.push_back(correct_weapon);
}

std::string global::character(std::string player_colour)
{
    std::string name;
    if(player_colour == "b")
    {
        name = "Mrs. Peacock";
    } else if (player_colour == "y"){
        name = "Colonel Mustard";
    } else if (player_colour == "r"){
        name = "Miss Scarlett";
    } else if (player_colour == "w"){
        name = "Mrs. White";
    } else if (player_colour == "g"){
        name = "Rev Green";
    } else if (player_colour == "p"){
        name = "Professor Plum";
    }
    return name;
}

std::tuple<int, int> global::starting_position(std::string player_colour) 
{
    std::tuple<int, int> pos;
    if(player_colour == "b")
    {
        pos = std::make_tuple(5, 2);
    } else if (player_colour == "y"){
        pos = std::make_tuple(1, 4);
    } else if (player_colour == "r"){
        pos = std::make_tuple(2, 5);
    } else if (player_colour == "w"){
        pos = std::make_tuple(2, 1);
    } else if (player_colour == "g"){
        pos = std::make_tuple(4, 1);
    } else if (player_colour == "p"){
        pos = std::make_tuple(5, 4);
    }
    return pos;
}


std::vector<std::unique_ptr<player>> global::create_players()
{
    int player_number;
    std::vector<std::unique_ptr<player>> players;
    std::vector<std::string> suspect_colours{"b", "y", "r", "w", "g", "p"};
    std::vector<std::string> chosen_colours;

    std::cout<<"HOW MANY PLAYERS (between 3 and 6)?  ";
    player_number = validate_input<int>(std::vector<int> {3, 4, 5, 6});

    for(int i{0}; i < player_number; i++)
    {
        std::string player_colour;
        bool valid{false};
        if(player_number < 6)
        {
            std::cout<<'\n'<<"CHOOSE the piece COLOUR for player "<<i+1<<" (available colours are";
            for(auto it = suspect_colours.begin(); it < suspect_colours.end(); it++)
                std::cout<<" "<<*it;
            std::cout<<") : ";
            player_colour = validate_input<std::string>(suspect_colours);
            std::cout<<"You chose "<<character(player_colour)<<'\n';
            suspect_colours.erase(std::remove(suspect_colours.begin(), suspect_colours.end(), player_colour));
        } else {
            if(i == 0) {std::cout<<"\nAll characters will participate in the game. Please distribute them between the players.\n";}
            player_colour = suspect_colours[i];
        }
        chosen_colours.push_back(player_colour);
        std::tuple<int, int> pos = starting_position(player_colour);
        int x_pos = std::get<0>(pos);
        int y_pos = std::get<1>(pos);
        std::unique_ptr<player> temp_ptr(new player(character(player_colour), player_colour, x_pos, y_pos));
        players.push_back(std::move(temp_ptr));
        chosen_colours.push_back(player_colour);
    }
    std::random_shuffle(players.begin(), players.end());
    return players; 
}



void global::show_rules()
{
    std::cout<<"\n\n\n\n\n"<<
    "CLUEDO. THE RULES."<<"\n"<<
    "(Source: Hasbro)"<<"\n\n"<<

    "IN SHORT"<<'\n'<<
    "The objective of the game is to determine who murdered the game's victim, where the crime took place, "<<'\n'<<
    "and which weapon was used. Each player assumes the role of one of the six suspects and attempts to deduce "<<'\n'<<
    "the correct answer by strategically moving around a game board representing the rooms of a mansion and "<<'\n'<<
    "collecting clues about the circumstances of the murder from the other players."<<"\n\n"<<
    "Press ENTER to continue. ";
    std::cin.ignore();

    std::cout<<"\n\n"<<"START OF THE GAME"<<'\n'<<
    "At the beginning of play, three cards - one suspect, one room, and one weapon - are chosen at random "<<'\n'<<
    "and remembered by the program, so that no one can see them. These cards represent the solution. The remainder"<<'\n'<<
    "of the cards are distributed among the players. Each player chooses one of the suspects to play for."<<'\n'<<
    "This choice is not related to the clues and player can make suggestions and accusations about their characters."<<"\n\n"<<

    "CHARACTER COLOURS: \nMrs. Peacock is blue (b) \nColonel Mustard is yellow (y) \nMiss Scarlett is red (r) "<<
    "\nMrs. White is white (w) \nRev Green is green (g) \nProfessor Plum is purple (p)\n"<<
    "Press ENTER to continue. ";
    std::cin.ignore();

    std::cout<<"\n\n"<<"MAKING SUGGESTIONS"<<'\n'<<
    "There are six characters, six murder weapons and eight rooms, leaving the players with 288 possibilities."<<'\n'<<
    "As soon as a player enters a room, they may make a suggestion as to the details, naming a suspect, the "<<'\n'<<
    "room they are in, and weapon. The player's suggestions must include the room they are currently in and may"<<'\n'<<
    "not be made in the corridors. The token for the suggested suspect is immediately moved into that room,"<<'\n'<<
    "if they are not both already present. All suggestions must be shared with other players."<<"\n\n"<<
    "Suggesting an opponent's token is a legitimate board strategy. A player may even suggest himself "<<'\n'<<
    "or herself as the murderer and may include cards in their own hand."<<"\n\n"<<
    "Once a player makes a suggestion, the others are called upon to disprove it. Their cards are checked"<<'\n'<<
    "automatically against the suggestion and if a player holds any of the three named cards, they should"<<'\n'<<
    "choose one (and only one) to show to the player who made the suggestion. A player's turn normally ends"<<'\n'<<
    "once their suggestion is completed."<<"\n\n"<<
    "The weapons are: Candlestick, Dagger, Pipe, Revolver, Rope, Wrench"<<'\n'<<
    "Press ENTER to continue. ";
    std::cin.ignore();

    std::cout<<"\n\n"<<"ACCUSATIONS"<<'\n'<<
    "A player who believes they have determined the correct elements may make an accusation when they "<<'\n'<<
    "have reached the Cellar. If the accusation is correct, the player wins; if not, they may not move "<<'\n'<<
    "nor make suggestions/accusations for the remainder of the game; in effect, losing. If all players "<<'\n'<<
    "except for one player have made an incorrect accusation, the remaining player automatically wins."<<"\n\n"<<
    "Press ENTER to continue. ";
    std::cin.ignore();

    std::cout<<'\n'<<'\n'<<"MOVING AROUND"<<'\n'<<
    "Players are not allowed to make suggestions repeatedly by remaining in one room; if they wish to "<<'\n'<<
    "make a second suggestion, they must first spend a turn out of the room. If a player's suggestion "<<'\n'<<
    "has brought another player's token into a room, the second player is not allowed to make their own"<<'\n'<<
    "suggestion in the same room when their turn comes up."<<"\n\n"<<
    "Press ENTER to finish. ";
    std::cin.ignore();
    std::cout<<"\n\n\n\n\n";
    return;
}


void global::save(player &player_current, std::vector<std::unique_ptr<player>> players)
{
    //Output the name of player whose turn it is currently + the correct answers + the positions and cards of all players into a text file
    std::string filename("SPOILERS.txt");
    std::fstream output_fstream;
    output_fstream.open(filename, std::ios_base::out);
    if (!output_fstream.is_open()) 
    {
        std::cerr<<"Failed to open"<<filename<<'\n';
    } else {
        output_fstream<<player_current.get_last_name()<<" "<<get_answers()[0]<<" "<<get_answers()[1]<<" "<<get_answers()[2];
        for(int i{0}; i<players.size(); i++)
        {
            std::string colour {players[i]->get_colour()};
            int x_pos{players[i]->get_x_position()};
            int y_pos{players[i]->get_y_position()};
            output_fstream<<'\n'<<colour<<" "<<x_pos<<" "<<y_pos;
            std::vector<card*> cards{players[i]->get_cards()};
            for_each(cards.begin(), cards.end(), [&] (card* temp)
            {
                output_fstream<<" "<<temp->get_name();
            } );
        }
        std::cout<<"Done saving. The game is now stored in a SPOILERS.txt file (which the players should not read"<<
        "if they want to continue playing) and can be loaded at any time."<<'\n';
    }
    return;
}


void global::quit(player &player_current, std::vector<std::unique_ptr<player>> players)
{
    std::cin.clear();
    std::cout<<'\n'<<"Quitting the game. SAVE THE PROGRESS (enter y or n)? ";
    std::string saving = validate_input<std::string>(std::vector<std::string> {"y", "Y", "n", "N"});
    if(saving == "y" || saving == "Y")
        save(player_current, std::move(players));
    std::cout<<'\n'<<"Thank you for the game.\n";
    return;
}


//Choose the room, murderer and weapon that the players should deduce in the game and distribute the rest of the cards between players
void global::card_distribution(std::vector<std::unique_ptr<player>> &players)
{
    int player_number = players.size();
    //Create the deck and populate it with cards
    std::vector<std::string> cards = room_names;
    cards.insert(cards.end(), suspect_names.begin(), suspect_names.end());
    cards.insert(cards.end(), weapon_names.begin(), weapon_names.end());

    std::vector<card*> card_deck;
    for(auto it = cards.begin(); it<cards.end(); it++)
        card_deck.push_back(new card{*it});

    //Pick the room, person, and weapon cards randomly, remember them and remove from the deck
    int room_index = rand()%8;
    int murderer_index = rand()%6 + 8;
    int weapon_index = rand()%6 + 15;
    set_answers(card_deck[room_index]->get_name(), card_deck[murderer_index]->get_name(), card_deck[weapon_index]->get_name());

    card_deck.erase(card_deck.begin() + room_index);
    card_deck.erase(card_deck.begin() + murderer_index - 1);
    card_deck.erase(card_deck.begin() + weapon_index - 2);

    //Shuffle the deck and distribute cards between the players
    std::random_shuffle(card_deck.begin(), card_deck.end());
    int cards_per_player = round(17.0/player_number);

    for(int i{}; i<players.size()-1; i++)
    {
        std::vector<card*> player_cards{card_deck.begin(), card_deck.begin() + cards_per_player};
        players[i]->receive_cards(player_cards);
        card_deck.erase(card_deck.begin(), card_deck.begin() + cards_per_player);
    }

    players[players.size()-1]->receive_cards(card_deck);
    return;
}


void global::show_cards(std::vector<std::unique_ptr<player>> &players)
{
    for(int i{}; i < players.size(); i++)
    {
        std::cout<<'\n'<<"PASS THE DEVICE TO "<<players[i]->get_name()<<". Press ENTER when ready. ";
        std::cin.ignore();
        std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        players[i]->print_cards();
        std::cout<<'\n'<<"Press ENTER when ready.";
        std::cin.ignore();
        std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    }
    return;
}


//Create new game
std::vector<std::unique_ptr<player>> global::new_game()
{
    std::vector<std::unique_ptr<player>> players{create_players()};
    card_distribution(players);
    std::cout<<'\n'<<"The clue cards are distributed. Now all the players will need to secretly record their evidence. "<<'\n';
    show_cards(players);
    std::random_shuffle(players.begin(), players.end());
    return players;
}


//Load the current player, correct answers, and the data of all players
std::vector<std::unique_ptr<player>> global::load_game()
{
    std::vector<std::unique_ptr<player>> players_old, players_new;
    std::vector<std::vector<card*>> all_player_cards;
    std::string player_current, correct_room, correct_suspect, correct_weapon;
    std::ifstream saved_game("SPOILERS.txt");
    if(! saved_game.good()) 
    {
        std::cerr<<"Error: there is no saved game in the directory.";
        exit(0);
    }
    std::string line;
    std::getline(saved_game, line);
    std::istringstream line_stream(line);
    line_stream.str(line);
    line_stream >> player_current >> correct_room >> correct_suspect >> correct_weapon;
    set_answers(correct_room, correct_suspect, correct_weapon);
    int i{0};

    //Copy the colours and positions of all players
    while(std::getline(saved_game, line))
    {
            std::string player_colour;
            int x_pos, y_pos;
            std::string all_cards, one_card;
            std::vector<std::string> cards_string;
            std::vector<card*> player_cards;
            std::istringstream line_stream(line);
            line_stream.str(line);
            line_stream >> player_colour >> x_pos >> y_pos;
            all_cards = line.substr(6, line.size()-5);
            std::istringstream card_stream(all_cards);
            while(getline(card_stream, one_card, ' ')) {cards_string.push_back(one_card);}
            for(auto it = cards_string.begin(); it < cards_string.end(); it++)
                player_cards.push_back(new card{*it});
            all_player_cards.push_back(player_cards);
            std::unique_ptr<player> temp_ptr(new player(character(player_colour), player_colour, x_pos, y_pos));
            players_old.push_back(std::move(temp_ptr));
            i++;
    }
        saved_game.close();

        for(int i{0}; i < players_old.size(); i++)
            players_old[i]->receive_cards(all_player_cards[i]);

        //Create a new vector which starts from the player whose turn it was next to continue playing in the right order
        for (const auto& it : players_old)
        {
            if(it->get_name() == player_current)
                players_new.push_back(std::make_unique<player>(*it));
        }

        auto it = std::find_if(players_old.begin(), players_old.end(), 
            [&](std::unique_ptr<player> &temp){ return temp->get_last_name() == player_current;});
        auto pos = std::distance(players_old.begin(), it) + 1;
        players_new.insert(players_new.end(), std::make_move_iterator(players_old.begin()+pos), std::make_move_iterator(players_old.end()) );
        players_old.erase(players_old.begin()+pos, players_old.end());
        players_new.insert(players_new.end(), std::make_move_iterator(players_old.begin()), std::make_move_iterator(players_old.end()) );
        players_old.erase(players_old.begin(), players_old.end());

        std::cout<<'\n'<<"Game loaded successfully."<<'\n';
    return players_new;
}


//Start a new game, load a previous one, or read the rules
std::vector<std::unique_ptr<player>> global::start_game()
{
    std::vector<std::unique_ptr<player>> players;
    std::string command;
    bool done{false};
    std::cout<<"\nWelcome to Cluedo."<<"\n\n";
    while(!done)
    {
        std::ifstream saved_game("SPOILERS.txt");
        if(saved_game.good()) 
        {
            saved_game.close();
            std::cout<<"Choose the command: NEW GAME (n), LOAD GAME (l), SHOW RULES (r),  EXIT (e): ";
            command = validate_input<std::string>(std::vector<std::string> {"n", "N", "l", "L", "r", "R", "e", "E"});
        } else {
            std::cout<<"Choose the command: NEW GAME (n), SHOW RULES (r),  EXIT (e): ";
            command = validate_input<std::string>(std::vector<std::string> {"n", "N", "r", "R", "e", "E"});
        }
        if(command == "e" || command == "E")
        {
            exit(0);
        } else if(command == "n" || command == "N") {
            std::cout<<"\nDo you want to read the rules? Enter y for yes or n for no: ";
            std::string read_rules = validate_input<std::string>(std::vector<std::string> {"y", "Y", "n", "N"});
            if(read_rules == "y" || read_rules == "Y")
                show_rules();
            std::move(players) = new_game();
            done = true;
        } else if(command == "l" || command == "L"){
            std::move(players) = load_game();
            std::cout<<"\nDo the players want to see their cards again? ";
            std::string see_cards = validate_input<std::string>(std::vector<std::string> {"y", "Y", "n", "N"});
            if(see_cards == "y" || see_cards == "Y")
                show_cards(players);
            done = true;
        } else if(command == "r" || command == "R"){
            show_rules();
            continue;
        }
    }
    return players;
}


void turn::print_board(std::vector<std::unique_ptr<player>> &players)
{
    std::vector<std::string> rooms = room_names;
    auto cel_pos = rooms.begin()+4;
    rooms.insert(cel_pos, "Cellar");

    int rows{5}, columns{5}, count{1};
    for(int temp_i{}; temp_i<60; temp_i++)
    {
        std::cout<<"_";
    }
    std::cout<<"\n";
    int j{1};
    for(int line{1}; line<=5; line++)
    {
        if(line% 2 != 0)
        {
            std::cout<<"|";
            int i{1};
            for(int cell{1}; cell<=5; cell++)
            {   
                std::cout<<count<<" ";  count ++;
                std::string room = rooms[(i-1) + (j-1)*3];
                int spaces_no;
                if(count<10) { spaces_no = 12-room.length(); }
                else { spaces_no = 11-room.length(); }

                if(cell% 2 == 0) { 
                    if(count<10) {std::cout<<"    ";}
                    else {std::cout<<"   ";}
                } else {
                    i++;
                    std::cout<<room;
                    for(int temp_i{}; temp_i<spaces_no; temp_i++) {std::cout<<" ";}
                }
                std::cout<<"|";
            }
            std::cout<<'\n';
        } else {
            if(count == 6) 
            { 
                std::cout<<"|"<<count<<"             |"<<count+1<<"     |"<<count+2<<
                "             |"<<count+3<<"     |"<<count+4<<"            |\n";
            } else { 
                std::cout<<"|"<<count<<"            |"<<count+1<<"    |"<<count+2<<
                "            |"<<count+3<<"    |"<<count+4<<"            |\n";
            }
            count +=5;
            j++;
        }

        for(int subline{2}; subline<=3; subline++){
            std::cout<<"|";
            for(int cell{1}; cell<=5; cell++){
                    if(cell%2 != 0) {
                        if(subline == 2) {std::cout<<"              |"; 
                        } else if (subline == 3) {
                            std::cout<<"  ";
                            for(int i{}; i < players.size(); i++)
                            {
                                if(players[i]->get_position() == std::make_tuple(cell, line)) 
                                {
                                    std::cout<<players[i]->get_colour()<<" ";   
                                } else { std::cout<<"  ";}
                            }
                            if(players.size() == 2) { std::cout<<"        "; }
                            else if(players.size() == 3) { std::cout<<"      "; }
                            else if(players.size() == 4) { std::cout<<"    "; }
                            else if(players.size() == 5) { std::cout<<"  "; }
                            std::cout<<"|";
                        }
                    } else {
                        if(subline == 2)
                        { 
                            for(int i{}; i < std::floor(players.size()/2); i++)
                            {
                                if(players[i]->get_position() == std::make_tuple(cell, line))
                                {
                                    std::cout<<" "<<players[i]->get_colour();   
                                } else { std::cout<<"  ";}
                            }
                            if(players.size() == 2 || players.size() == 3) { std::cout<<"    "; }
                            else if(players.size() == 5) { std::cout<<"  "; }
                        } else {
                            int start_from = std::floor(players.size()/2);
                            for(int i{start_from}; i < players.size(); i++)
                            {
                                if(players[i]->get_position() == std::make_tuple(cell, line))
                                {
                                    std::cout<<players[i]->get_colour()<<" ";   
                                } else { std::cout<<"  ";}
                            }
                            if(players.size() == 2) { std::cout<<"    "; } 
                            else if(players.size() == 3) { std::cout<<"  "; }
                        }
                        if(players.size() == 4) { std::cout<<"  "; }
                        std::cout<<"|";
                    }
            }
            std::cout<<'\n';
        }
        std::cout<<"|______________|______|______________|______|______________|\n";
    }
    return;
}

//Function to make a move for a chosen player
player* turn::move(player &player_current)
{
    int chosen_cell;
    std::vector<std::string> allowed_rooms;
    std::vector<std::tuple<int, int>> all_positions, allowed_positions;
    std::vector<int> allowed_cells, all_cells;
    std::tuple<int, int> chosen_position, pos_current{player_current.get_position()};

    for(size_t i{1}; i<=25; i++)
    {
            all_cells.push_back(i);
    }

    for(size_t y{1}; y<=5; y++)
    {
        for(size_t x{1}; x<=5; x++)
        {
            all_positions.push_back(std::make_tuple(x, y));
        }
    }

    //Find the allowed positions
    allowed_positions = player_current.allowed_moves(dice_throw());
    for(auto& i : allowed_positions)
    {
        int x{std::get<0>(i)}, y{std::get<1>(i)};
        allowed_cells.push_back(all_cells[(x-1) + (y-1)*5]);
    }
    std::sort(allowed_cells.begin(), allowed_cells.end());
    std::cout<<"Allowed cells are: ";
    for(auto it = allowed_cells.begin(); it < allowed_cells.end(); it++)
    {
        std::cout<<*it<<" ";
    }

    //Ask player where they want to go
    std::cout<<'\n'<<'\n'<<"PLAYER "<<player_current.get_name()<<", CHOOSE which CELL to move to (using a number): ";
    chosen_cell = validate_input<int>(allowed_cells);
    
    //Find the index of the chosen room and move the piece there
    auto it = std::find(all_cells.begin(), all_cells.end(), chosen_cell);
    chosen_position = all_positions[std::distance(all_cells.begin(), it)];
    player_current.change_position(chosen_position);

    return  &player_current;
}


//Allows player to make a suggestion in the room where they are located
std::vector<std::string> turn::suggestion(std::string chosen_room, player &player_current, std::vector<std::unique_ptr<player>> &other_players)
{
    std::string chosen_suspect, chosen_weapon;

    //Ask for user to input a suspect and a weapon
    char confirmed{'n'};
    while(confirmed != 'y' && confirmed != 'Y')
    {
        std::cout<<'\n'<<"Player "<<player_current.get_name()<<", which SUSPECT do you want to check? Enter their last name: ";
        chosen_suspect = validate_input<std::string>(suspect_names);
        std::cout<<'\n'<<"CHOOSE the WEAPON to complete your suggestion: ";
        chosen_weapon = validate_input<std::string>(weapon_names);
        std::cout<<'\n'<<"Your suggestion is: "<<chosen_suspect<<" in the "<<chosen_room<<" with a "<<chosen_weapon<<"."<<'\n'<<"IS THIS RIGHT (enter y or n)?:  ";
        confirmed = validate_input<char>(std::vector<char> {'y', 'Y', 'n', 'N'});
    }

    return {chosen_room, chosen_suspect, chosen_weapon};
}


//Find which cards of a certain player match the suggestion and ask to choose one to show (used in turn::check_cards)
std::vector<std::string> turn::ask_for_evidence(player &player_b, std::vector<std::string> choices, player &player_current)
{
    std::vector<std::string> cards_to_show, matching_cards;
    std::string to_reveal;
    std::vector<card*> player_cards{player_b.get_cards()};

    std::cout<<'\n'<<"Player "<<player_b.get_name()<<", the following of your cards match the suggestion:"<<'\n';

    bool done{false};
    while(!done)
    {   
        for(auto it = choices.begin(); it < choices.end(); it++)
            if(std::find_if(player_cards.begin(), player_cards.end(), 
                [&](card* &temp){return temp->get_name() == *it;}) != player_cards.end())
            {
                matching_cards.push_back(*it);
            }

        for(auto it = matching_cards.begin(); it < matching_cards.end(); it++)
            std::cout<<(*it)<<" ";

        if(matching_cards.size() == 0)
        {
            std::cout<<"none. "<<'\n';
        }

        //Ask the player to choose which card they want to reveal if there was more than one matching card
        if(matching_cards.size() > 1)
        {
            std::cout<<"WHICH one do you REVEAL to player "<<player_current.get_name()<<"? ";
            to_reveal = validate_input<std::string>(matching_cards);
            std::cout<<'\n'<<to_reveal<<" will be shown to player "<<player_current.get_name()<<"."<<'\n';
            cards_to_show.push_back(to_reveal);
            done = true;
            break;
        } else {
            if(matching_cards.size() == 1) 
            {
                cards_to_show.push_back(matching_cards[0]);
                std::cout<<"\nThis card will be shown to player "<<player_current.get_name()<<"."<<'\n';
            }
            std::cout<<"Type something to avoid suspition (your input will be ignored): ";
            std::string line;
            getline(std::cin, line);
            std::cin.clear();
            done = true;
            break;
        }
    }
    return cards_to_show;
}


//Ask players for their evidence and show it to the player who made the suggestion
void turn::get_evidence(std::vector<std::unique_ptr<player>> &other_players, std::vector<std::string> choices, player &player_current)
{
    std::vector<std::string> cards_to_show;

    //Check against other players' cards
    for(int i{}; i < other_players.size(); i++)
    {
        std::vector<std::string> to_show;
        std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout<<"PASS THE DEVICE TO "<<other_players[i]->get_name()<<". Press ENTER when ready. ";
        std::cin.ignore();
        std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        to_show = ask_for_evidence(*other_players[i], choices, player_current);
        cards_to_show.insert(cards_to_show.end(), std::make_move_iterator(to_show.begin()), std::make_move_iterator(to_show.end()));
    }

    //Show the chosen evidence to the player, whose suggestion it was
    std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    std::cout<<"PASS THE DEVICE BACK TO "<<player_current.get_name()<<". Press ENTER when ready. ";
    std::cin.ignore();
    std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    if(cards_to_show.empty())
    {
        std::cout<<"There is nothing to show you. Please say so to the rest of the players. ";
    } else {

        std::cout<<"The other players have chosen to show you the following evidence: ";
        for_each(cards_to_show.begin(), cards_to_show.end(), [] (std::string temp)
        {
            std::cout<<temp<<" ";
        } );

        std::cout<<'\n'<<"Please tell them that you have been shown "<<cards_to_show.size();
        if(cards_to_show.size() == 1)
        {
            std::cout<<" card."<<"\n\n";
        } else {
            std::cout<<" cards."<<"\n\n";
        }
    }
    std::cout<<"Press ENTER when ready.";
    std::cin.ignore();
    return;
}

std::vector<std::string> turn::validate_accusation(std::string room, std::string suspect, std::string weapon)
{
    bool valid{false};
    std::vector<std::string> choices;
    
    auto itr_room = std::find(room_names.begin(), room_names.end(), room);
    auto itr_suspect = std::find(suspect_names.begin(), suspect_names.end(), suspect);
    auto itr_weapon = std::find(weapon_names.begin(), weapon_names.end(), weapon);
    if(itr_room != room_names.end() && itr_suspect != suspect_names.end() &&  itr_weapon != weapon_names.end())
    {
        choices.push_back(*itr_room);
        choices.push_back(*itr_suspect);
        choices.push_back(*itr_weapon);
    } else {
        std::cin.clear();
        std::cout<<"Not a valid input. Try again. "<<'\n';
    }
    return choices;
}

std::vector<std::unique_ptr<player>> turn::accusation(player &player_current, std::vector<std::unique_ptr<player>> players, global &game)
{
    std::vector<std::string> choices;
    std::string line;

    //Ask the player for the room, suspect and weapon they want to accuse
    while(choices.size() == 0)
    {
        std::cout<<"Please enter the accusation in the format Room Suspect Weapon (e.g Cellar Smith Gun): ";
        getline(std::cin, line);
        std::istringstream line_stream(line);
        std::string room, suspect, weapon;
        line_stream.str(line);
        line_stream >> room >> suspect >> weapon;
        choices = validate_accusation(room, suspect, weapon); 
    }

    if(choices[0] == game.get_answers()[0] && 
        choices[1] == game.get_answers()[1] && 
        choices[2] == game.get_answers()[2])
    {
        std::cout<<"Correct! Player "<<player_current.get_name()<<" won.\n";
        players.clear();
    } else {
        std::cout<<"Sorry, your accusation is incorrect.\n";
        std::string last_name{player_current.get_last_name()};
        auto it = std::find_if(players.begin(), players.end(), 
            [&](std::unique_ptr<player> & temp){ return temp->get_last_name() == last_name;});
        players.erase(std::remove(players.begin(), players.end(), *it));
        std::cout<<"Player "<<last_name<<" cannot make any more suggestions. There are "<<players.size()<<" players left.\n";
    }
    return players;
}


//Defines the sequence of actions taken by a player_a during their turn
bool turn::player_turn(player &player_current, std::vector<std::unique_ptr<player>> &players, global &game)
{ 
    std::vector<std::tuple<int, int>> room_positions = {{1, 1}, {3, 1}, {5, 1}, {1, 3}, {5, 3}, {1, 5}, {3, 5}, {5, 5}};
    std::tuple<int, int> cellar_coords = {3, 3};
    bool quitting{false};
    if(players.size() > 1)
    {
        //Make a separate vector of poiters to players without the current player (needed for getting evidence)
        std::vector<std::unique_ptr<player>> other_players;
        for (const auto& it : players)
            if(it->get_name() != player_current.get_name())
                other_players.push_back(std::make_unique<player>(*it));

        print_board(players);
        std::cout<<"\n\nTurn of player "<<player_current.get_name()<<".";
        move(player_current);
        print_board(players);

        //Offer player to make a suggestion if they have entered one of the rooms
        
        auto it = std::find(room_positions.begin(), room_positions.end(), player_current.get_position());
        if(it != room_positions.end())
        {
            std::string current_room = room_names[std::distance(room_positions.begin(), it)];

            std::cout<<"\nYou are in the "<<current_room<<". Do you want to make a suggestion? (enter y or n) ";
            std::string making_suggestion = validate_input<std::string>(std::vector<std::string> {"y", "Y", "n", "N"});

            if(making_suggestion == "y" || making_suggestion == "Y")
            {
                std::vector<std::string> choices = suggestion(current_room, player_current, other_players);
                get_evidence(other_players, choices, player_current);

                //Move the suspect to the room if he/she is one of the players
                for(int i{}; i < other_players.size(); i++)
                {
                    std::string suspect_name{other_players[i]->get_last_name()};
                    if(choices[1] == suspect_name)
                    {
                        auto it = std::find_if(players.begin(), players.end(), 
                            [&](std::unique_ptr<player> & temp){ return temp->get_last_name() == suspect_name;});
                        (*it)->change_position(player_current.get_position());
                        std::cout<<other_players[i]->get_name()<<" goes to the "<<choices[0]<<"."<<'\n';
                        print_board(players);
                    }
                }
            }
        } else if(player_current.get_position() == cellar_coords) {
            //Ask player if they want to make the final accusation if moved to cellar
            std::cout<<"\nYou are in the cellar. Do you want to make an accusation? This may let you win or lose the game at this turn."<<'\n'
            <<"\nPlease enter y to accuse or n to skip: ";
            std::string accusing = validate_input<std::string>(std::vector<std::string> {"y", "Y", "n", "N"});
            if(accusing == "y" || accusing == "Y"){
                std::move(players) = accusation(player_current, std::move(players), game);}
                if(players.size() == 0)
                    quitting = true;
        }

        if(players.size() > 1)
        {
            std::cout<<"\n\n"<<"Press ENTER to finish your turn or type q to quit the game. ";
            std::string line;
            std::getline(std::cin, line);
            if(line == "q")
            {
                game.quit(player_current, std::move(players));
                quitting = true; 
            }
        }
        
    } else {
        std::cout<<"There is only one player left. "<<player_current.get_name()<<" won.";
        quitting = true;
    } 
    return quitting;
}