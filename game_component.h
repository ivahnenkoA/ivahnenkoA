// Header file for the game_component base class and its derived classes
// Anna Ivahnenko  15/05/21

#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>

#ifndef GAME_COMPONENTS
#define GAME_COMPONENTS

namespace comp 
{

    class game_component
    {  
    public:
        virtual ~game_component(){}
        virtual std::string get_name()=0;
    };


    class card : public game_component
    {
    private:
        std::string card_name;
    public:
        card(){}
        card(std::string name): card_name{name} {}
        ~card(){}
        std::string get_name();
    };


    class player : public game_component
    {
    private:
        std::string player_colour;
        std::string player_name;
        std::vector<card*> player_cards;
        int x_position{};
        int y_position{};
    public:
        player(){}
        player(std::string name, std::string colour, int x_pos, int y_pos): player_name{name}, player_colour{colour}, 
            x_position{x_pos}, y_position{y_pos} {}
        ~player(){}

        std::vector<std::string> get_rooms();
        std::vector<std::string> get_suspects();
        std::vector<std::string> get_weapons();
        std::string get_name();
        std::string get_last_name();
        std::string get_colour();
        int get_x_position();
        int get_y_position();
        std::vector<card*> get_cards();
        std::tuple<int, int> get_position();
        std::vector<std::tuple<int, int>> allowed_moves(int dice_result);
        void change_position(std::tuple<int, int> new_pos);
        void receive_cards(std::vector<card*> assigned_cards);
        void print_cards();
    };
}

#endif