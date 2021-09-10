// Header file for the game_process base class and its derived classes
// Anna Ivahnenko  15/05/21

#include <iostream>
#include "game_component.h"


#ifndef GAME_PROCESSES
#define GAME_PROCESSES

using namespace comp;

namespace proc
{
    class game_process
    {
    protected:
        std::vector<std::string> correct_answers;
        const std::vector<std::string> room_names = {"Kitchen", "Balllroom", "Conservatory", "Bathroom", "Library", "Lounge", "Hall", "Study"};
        const std::vector<std::string> suspect_names = {"Scarlett", "Green", "Mustard", "Plum", "Peacock", "White"};
        const std::vector<std::string> weapon_names = {"Candlestick", "Dagger", "Pipe", "Revolver", "Rope", "Wrench"};
    public:
        ~game_process(){};
        virtual std::vector<std::string> get_answers();
    };

    class global : public game_process
    {
    public:
        global(){}
        ~global(){}
        void show_rules();
        std::vector<std::unique_ptr<player>> start_game();
        std::vector<std::unique_ptr<player>> new_game();
        std::vector<std::unique_ptr<player>> load_game();
        std::vector<std::unique_ptr<player>> create_players();
        std::string character(std::string player_colour);
        std::tuple<int, int> starting_position(std::string player_colour);
        void show_cards(std::vector<std::unique_ptr<player>> &players);
        void quit(player &player_current, std::vector<std::unique_ptr<player>> players);
        void save(player &player_current, std::vector<std::unique_ptr<player>> players);
        void card_distribution(std::vector<std::unique_ptr<player>> &players);
        void set_answers(std::string correct_room, std::string correct_suspect, std::string correct_weapon);
    };


    class turn : public game_process
    {
    public:
        turn(){}
        ~turn(){}
        player* move(player &player_current);
        std::vector<std::string> ask_for_evidence(player &player_b, std::vector<std::string> choices, player &player_current);
        void get_evidence(std::vector<std::unique_ptr<player>> &other_players, std::vector<std::string> choices, player &player_current);
        std::vector<std::string> suggestion(std::string current_room, player &player_current, std::vector<std::unique_ptr<player>> &other_players);
        std::vector<std::unique_ptr<player>> accusation(player &player_current, std::vector<std::unique_ptr<player>> players, global &game);
        std::vector<std::string> validate_accusation(std::string room, std::string suspect, std::string weapon);
        bool player_turn(player &player_current, std::vector<std::unique_ptr<player>> &players, global &game);
        void print_board(std::vector<std::unique_ptr<player>> &players);
    };

}

using namespace comp;
//Function template for validating user input
template <class c_type> c_type validate_input(std::vector<c_type> allowed_inputs)
{
    c_type confirmed;
    bool valid{false};
    std::string input;
    while(!valid)
    {
        getline(std::cin, input);
        std::istringstream line_stream(input);
        if(line_stream>>confirmed 
            && input.find_first_of(" ") == std::string::npos
            && std::find(allowed_inputs.begin(), allowed_inputs.end(), confirmed) != allowed_inputs.end())
        {
            valid = true;
            break;
        } else {
            std::cout <<"Sorry, your input was not valid. Try again: "; 
            std::cin.clear(); 
        }
    }
    return confirmed;
}

#endif