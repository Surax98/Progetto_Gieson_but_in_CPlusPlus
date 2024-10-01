#include <iostream>
#include <random>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include "gamelib.h"

static void Proceed(Player *, uint8_t *);
static void ShowObject(Player *, uint8_t *);
static void LootObject(Player *, uint8_t *);
static void Heal(Player *, uint8_t *);
static void UseAdrenaline(Player * player);
static void SpawnGieson(Player * player);
extern bool searchFunc(const std::pair<const Object_t, ushort>);
extern std::unordered_set<Object_t> Weapons;

void Play(std::vector<Player *>* players){
    std::string menu = "1) Proceed to the next Zone\n2) Show the Object in the Zone\n3) Loot the Object in the Zone\n4) Heal wounds\n5) Use Adrenaline\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, players->size() - 1);

    uint8_t turn = distrib(gen), turnsPassed = 1;
    bool playing = true;
    ClearScreen();
    do {   
        size_t counter = 0;
        for (Player * p : *players){
            if (p->GetState() == DEAD || p->GetState() == SAFE)
                counter++;
        }

        if (counter == players->size()){
            playing = false;
            break;
        }

        turn %= players->size();
        auto currentTurn = turn;
        if(players->at(turn)->GetState() == SAFE || players->at(turn)->GetState() == DEAD){
            turn ++;
        } else {
            std::string sentence = "It's now " + players->at(turn)->GetName() + "'s turn.\n";
            std::string pronoun = players->at(turn)->GetGender() == 'M' ? "He" : 
                (players->at(turn)->GetGender() == 'F' ? "She" : "It") ;
            sentence += pronoun + "'s in the " + Zone_n[players->at(turn)->GetPosition()->GetType()] + " zone and " + pronoun + "'s " + (players->at(turn)->GetState() == WOUNDED ? "" : "not ")+ "Wounded\n";
            sentence += "This is turn number " + std::to_string(turnsPassed) + "\n";
            sentence += "What will " + pronoun + " do?\n\n" + menu;
            auto selection = SelectionWrapper(sentence, (uint8_t)5);

            switch (selection){
            case 1:
                Proceed(players->at(turn), &turn);
                break;

            case 2:
                ShowObject(players->at(turn), &turn);
                break;

            case 3:
                LootObject(players->at(turn), &turn);
                break;

            case 4:
                Heal(players->at(turn), &turn);
                break;

            case 5:
                UseAdrenaline(players->at(turn));
                break;
            
            default:
                break;
            }

            if ( ((turn-1)%players->size() ) == (players->size() - 1) && currentTurn != turn ){
                turnsPassed++;
            }

            if (currentTurn != turn){
                SpawnGieson(players->at((turn-1)%players->size()));
            }
        }
    } while (playing);

    std::cout << "All players died... Gieson ripped them all\n\n";

}

static void Proceed(Player * player, uint8_t * turn) {
    if (player->GetPosition()->GetNext() != NULL) {
        player->SetPosition(player->GetPosition()->GetNext());
        std::cout << player->GetName() + " proceeds to the next Zone: the " + Zone_n[player->GetPosition()->GetType()] + "\n\n===============\n\n";
    } else if (player->GetPosition()->GetType() == EXIT) {
        auto selection = SelectionWrapper("You are near the Exit. Do you want to proceed and find salvation? y/n",'a');
        if (selection == "y" || selection == "Y"){
            std::cout << player->GetName() << " is now SAFE" << std::endl << std::endl;
            player->SetState(SAFE);
        } else {
            return;
        }
    }
    if (player->GetAdrenaline()){
            std::cout << player->GetName() << " is under Adrenaline! It can perform an extra action" << std::endl << std::endl;
            player->SetAdrenaline(false);
            return;
        }
    (*turn)++;
}

static void ShowObject(Player * player, uint8_t * turn){
    if (!player->GetPosition()->GetHiddenObject()) {
        std::cout << "You already discovered the hidden Object. You can Loot it" << std::endl << std::endl;
        return;
    }

    std::string toBePrinted = player->GetPosition()->GetObject() == JUNK ? "In the current Zone, there's just a bunch of Junk" : "In the current Zone, there's a " + Object_n[player->GetPosition()->GetObject()] + "!"; 
    std::cout << toBePrinted << std::endl << std::endl << "===============\n\n";
    player->GetPosition()->SetHiddenObject(false);
    if (player->GetAdrenaline()){
            std::cout << player->GetName() << " is under Adrenaline! It can perform an extra action" << std::endl << std::endl;
            player->SetAdrenaline(false);
            return;
        }
    (*turn)++;
}

static void LootObject(Player * player, uint8_t * turn){
    if (player->GetPosition()->GetHiddenObject()) {
        std::cout << "You cannot loot something you don't know about. Use the Show Object function before" << std::endl << std::endl;
        return;
    }

    auto object = player->GetPosition()->GetObject();
    if(object != JUNK){
        std::cout << "Looting the object " << Object_n[object] << " and adding to " << player->GetName() << "'s backpack" << std::endl << std::endl << "===============\n\n";
        player->AddItem(object);
        if (player->GetAdrenaline()){
            std::cout << player->GetName() << " is under Adrenaline! It can perform an extra action" << std::endl << std::endl;
            player->SetAdrenaline(false);
            return;
        }
        (*turn)++;
    } else {
        std::cout << player->GetName() <<" refused to add Junk to the backpack";
    }
}

static void Heal(Player * player, uint8_t * turn){
    if (player->GetItemCount(BANDAID) > 0) {
        if (player->GetState() == WOUNDED) {
            std::cout << player->GetName() << " healed!" << std::endl << std::endl << "===============\n\n";
            player->SetState(ALIVE);
            player->RemoveItem(BANDAID);
            if (player->GetAdrenaline()){
                std::cout << player->GetName() << " is under Adrenaline! It can perform an extra action" << std::endl << std::endl;
                player->SetAdrenaline(false);
                return;
            }
            (*turn)++;
        } else {
            std::cout << player->GetName() << " is not wounded!" << std::endl << std::endl;
        }
    } else {
        std::cout << player->GetName() << " has no Bandaids!" << std::endl << std::endl;
    }
}

static void UseAdrenaline(Player * player){
    if (player->GetAdrenaline()) {
        std::cout << player->GetName() << " already used Adrenaline! Perform other 2 actions!!!" << std::endl << std::endl;
        return;
    } else if (player->GetItemCount(ADRENALINE) < 1){
        std::cout << player->GetName() << " has no Adrenaline!" << std::endl << std::endl;
        return;
    }
    
    std::cout << player->GetName() << " used Adrenaline! Perform 2 more actions!" << std::endl << std::endl;
    player->RemoveItem(ADRENALINE);
    player->SetAdrenaline(true);
}

static void SpawnGieson(Player * player) {
    if (player->GetAdrenaline()){
        return;
    }

    if (player->GetGasoline() != 0){
        player->SetGasoline(player->GetGasoline()-1);

        if (player->GetGasoline() == 0){
            std::cout << player->GetName() << ", Gieson is coming for you..." << std::endl << std::endl;
            std::cout << "===============" << std::endl << std::endl;
        }
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 99);

    auto percentage = distrib(gen);
    auto spawn = false;

    if (player->GetPosition()->GetType() != EXIT && percentage < 30) {
        spawn = true;
    } else if (player->GetPosition()->GetType() == EXIT && percentage < 75) {
        spawn = true;
    }

    if (spawn) {
        std::cout << "Gieson appeared in front of " + player->GetName() << "!" << std::endl;
        auto backpack = player->GetBackpack();
        auto it = backpack->begin();

        if (std::find_if(backpack->begin(), backpack->end(), searchFunc) == backpack->end()){
            std::string pronoun = player->GetGender() == 'M' ? "him" : (player->GetGender() == 'F' ? "her" : "it") ;
            std::cout << "Unfortunately, " + player->GetName() << " is defenseless and Gieson rips " << pronoun << " off..." << std::endl <<std::endl;
            player->SetState(DEAD);
        } else {
            std::string pronoun = player->GetGender() == 'M' ? "He" : (player->GetGender() == 'F' ? "She" : "It") ;
            std::vector<std::pair<Object_t, ushort>> matches;

            for(ushort i = 0; i < Weapons.size(); i++){
                it = std::find_if(it, backpack->end(), searchFunc);
                if (it != backpack->end()){
                    matches.push_back(*it);
                    ++it;
                }
            }

            uint8_t usedWeapon = 0;

            if (matches.size() > 1){
                do {
                    std::cout << pronoun << " can choose to use the following Weapons to fight him:" << std::endl <<std::endl;
                    for(uint8_t i = 0; i < matches.size(); i++){
                        std::cout << (int)i+1 << ") " << Object_n [matches.at(i).first] << std::endl;
                    }
                    usedWeapon = SelectionWrapper("Pick your weapon:", (uint8_t)matches.size());
                    usedWeapon--;
                } while(usedWeapon >= matches.size());

                usedWeapon = matches.at(usedWeapon).first;

            } else {
                usedWeapon = matches.at(0).first;
            }

            std::cout << player->GetName() << " used the "  << Object_n[(Object_t)usedWeapon] << " to face him!"<< std::endl;

            switch (usedWeapon){
                case KNIFE:
                    if (player->GetState() == WOUNDED) {
                        player->SetState(DEAD);
                        std::cout << "Unfortunately, " << player->GetName() << " was already Wounded and couldn't escape Gieson..."<< std::endl << std::endl;
                    } else {
                        player->SetState(WOUNDED);
                        std::cout << player->GetName() << " managed to escape Gieson, but " << pronoun << "'s now Wounded..." << std::endl << std::endl;
                    }
                    player->RemoveItem(KNIFE);
                    break;

                case GUN:
                    std::cout << player->GetName() << " managed to escape Gieson." << std::endl << std::endl;
                    player->RemoveItem(GUN);
                    break;

                case GASOLINE:
                    std::cout << player->GetName() << " managed to escape Gieson. He also won't be able to appear for the next 4 turns of " << player->GetName() << std::endl << std::endl;
                    player->RemoveItem(GASOLINE);
                    player->SetGasoline(4);
                    break;
                
                default:
                    break;
                }
        }

        std::cout << "===============\n\n";
    }
}