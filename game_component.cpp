// Function definitions for the game_component base class and its derived classes
// Anna Ivahnenko  15/05/21

#include <iostream>
#include "game_component.h"

using namespace comp;

std::string card::get_name() {return card_name;}

std::string player::get_name(){return player_name;}
std::string player::get_colour(){return player_colour;}
int player::get_x_position() {return x_position;}
int player::get_y_position() {return y_position;}
std::tuple<int, int> player::get_position() {return {x_position, y_position};}
std::vector<card*> player::get_cards(){return player_cards;}

std::string player::get_last_name() 
{
    return player_name.substr(player_name.find(' ')+1, player_name.size()-player_name.find(' '));
}

void player::change_position(std::tuple<int, int> new_pos)
{
    int new_x_pos{std::get<0>(new_pos)};
    int new_y_pos{std::get<1>(new_pos)};
    x_position = new_x_pos;
    y_position = new_y_pos;
}

void player::receive_cards(std::vector<card*> assigned_cards)
{
    player_cards = assigned_cards;
}

void player::print_cards()
{
    std::cout<<player_name<<", your cards are ("<<player_cards.size()<<" of them)"<<":"<<'\n';
    for_each(player_cards.begin(), player_cards.end(), [] (card* temp)
    {
        std::cout<<temp->get_name()<<", ";
    } );
    std::cout<<'\n';
}


//Find the allowed new positions for a given position and a dice throw
std::vector<std::tuple<int, int>> player::allowed_moves(int dice_result)
{
    std::vector<int> possible_delta_x, possible_delta_y;
    std::vector<std::tuple<int, int>> all_moves;
    int count, temp_dice{dice_result};

    while(temp_dice > 0)
    {
        for(int i{-temp_dice}; i<=temp_dice; i++)
        {
            for(int j{-temp_dice}; j<=temp_dice; j++)
            {
                if(abs(i)+ abs(j) == temp_dice)
                {
                    possible_delta_x.push_back(i);
                    possible_delta_y.push_back(j);
                }
            }
        }
        temp_dice -= 2;
    }
        
    for (int i{0}; i < possible_delta_x.size(); i++) 
    {
        // Position of piece after move
        int new_x = x_position + possible_delta_x[i];
        int new_y = y_position + possible_delta_y[i];  

        if (new_x >= 1 && new_y >= 1 && new_x <= 5 && new_y <= 5)
        {
            all_moves.push_back(std::make_tuple(new_x, new_y));
            count++;
        } else {
            continue;
        } 
    }
    return all_moves;
}