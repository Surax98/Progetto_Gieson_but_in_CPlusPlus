#include "gamelib.h"
#include <unordered_set>
extern std::unordered_set<Object_t> Weapons;

void InvalidWrapper(){
    ClearScreen();
    std::cout << "!!! Please choose a valid value !!!" << std::endl << std::endl;
}

void ClearScreen() {
    std::cout << "\033[2J\033[1;1H";  // ANSI escape code to clear the screen
}

uint8_t SelectionWrapper(const std::string& s, uint8_t max){
    uint8_t selection = 0;
    bool error = false;
    do {
        if (error){
            ClearScreen();
            std::cout << "!!! Please choose a valid value !!!" << std::endl << std::endl;
        }
        std::string readLine;
        std::cout << s << std::endl;
        try {
            std::getline(std::cin, readLine);
            ClearScreen();
            //std::cout  << "===============" << std::endl << std::endl;
            selection = stoi(readLine); 
        } catch (std::invalid_argument & e){
            std::cout << "!!! Please choose a valid value !!!" << std::endl << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "!!! Please choose a valid value !!!" << std::endl << std::endl;
        }
        error = true;
    } while(selection > max);
    return selection;
}

std::string SelectionWrapper(const std::string& s, char n){
    std::string readLine;
    bool error = false;
    do {
        if (error){
            ClearScreen();
            std::cout << "!!! Please choose a valid value !!!" << std::endl << std::endl;
        }
        std::cout << s << std::endl;
        try{
            std::getline(std::cin, readLine);
            ClearScreen();
            //std::cout  << "===============" << std::endl << std::endl;
            break;
        } catch(std::invalid_argument & e){
            std::cout << "!!! Please choose a valid value !!!" << std::endl << std::endl;
            error=true;
        }
    } while(true);

    return readLine;
}

bool CheckIfRunning(std::vector<Player *>* players){
    size_t counter = 0;

    for (Player * p : *players){
        if(p->GetState() == DEAD || p->GetState() == SAFE)
            counter++;
    }

    if (counter == players->size() && players->size() != 0){
        return false;
    } else {
        return true;
    }    
}

void DeletePlayers(std::vector<Player *>* players){
    for(Player * p : *players){
        delete p;
    }
    players->clear();
}

void DeleteMap(std::list<Zone *>* map){
    for (Zone * z : *map) {
        delete z;
    }
    map->clear();
}

bool searchFunc(const std::pair<const Object_t, ushort> element){
    if (Weapons.find(element.first) == Weapons.end()){
        return false;
    } else if (element.second > 0) {
        return true;
    } else {
        return false;
    }
}