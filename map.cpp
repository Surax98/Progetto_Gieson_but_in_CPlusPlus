#include "gamelib.h"
#include <random>
#include <unordered_set>

static void PrintMap(std::list<Zone *>* map);
static void DeleteZone(std::list<Zone *>* map);
static void RandomMap(std::list<Zone *>* map);
static void InsertZone(std::list<Zone *>* map, int zone);
static void InitPlayers(std::vector<Player *>*, std::list<Zone *>*);
static Object_t GenerateObject(uint8_t, uint8_t);
extern std::unordered_set<std::string> ValidGenders;

void MapEditor(std::list<Zone *>* map, std::vector<Player *>* players){
    ClearScreen();
    do {
        std::string menu = "1) Insert Zone\n2) Delete Zone\n3) Clear Map\n4) Print Map\n5) Random Map\n6) Close Editor\n\nChoose one of the options:";
        uint8_t selection = SelectionWrapper(menu, (uint8_t)6);

        switch (selection){
        case 1:
            {
                if (map->size() > 0 && map->back()->GetType() == EXIT){
                    std::cout << "The map has already been finalized. Clear the old Map or at least Delete the latest Zone!" << std::endl << std::endl;
                    break;
                }

                uint8_t zone;
                do {
                    zone = SelectionWrapper("1) Kitchen\n2) Living Room\n3) Garage\n4) Road\n5) Lake\n\nChoose one of the following areas:", (uint8_t)5);
                } while (zone == 0 || zone > 5);
                InsertZone(map, zone-1);
            }
            break;

        case 2:
            DeleteZone(map);
            break;

        case 3:
            DeleteMap(map);
            break;

        case 4:
            PrintMap(map); 
            break;

        case 5:
            RandomMap(map);
            break;

        case 6:
            if (map->size() < 9) {
                std::cout << "!!! Unable to close the Map Editor: you can't have less than 9 Zones !!!" << std::endl << std::endl;
                break;
            }
            if (map->back()->GetType() != EXIT){
                InsertZone(map, (int)EXIT);
                InitPlayers(players, map);
            }
            ClearScreen();
            return;
        
        default:
            break;
        }
    } while (true);
}

static void InsertZone(std::list<Zone *>* map, int zone){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    uint8_t randomNumber = distrib(gen);
    Object_t object = GenerateObject(zone, randomNumber);
    auto newZone = new Zone((Zone_t)(zone), object);

    if (map->size() > 0) {
        map->back()->SetNext(newZone);
    }

    map->push_back(newZone);
}

static void DeleteZone(std::list<Zone *>* map){
    if (map->empty()){
        std::cout << "Empty Map. Aborting deletion\n\n";
        return;
    }

    std::string selection = SelectionWrapper("Would you like to delete the last inserted zone? y/n", 'a');

    if (selection == "y" || selection == "Y"){
        auto zone = map->back();
        std::cout << "Deleting zone " << zone->GetType() << ". It had the object " << zone->GetObject();
        
        delete map->back();
        map->pop_back();
        if (map->size() > 0) {
            map->back()->SetNext(NULL);
        }

    } else {
        std::cout << "Aborting deletion\n\n";
    }
}

static void PrintMap(std::list<Zone *>* map){
    uint16_t index = 0;
    for (Zone * z : *map) {
        index++;
        std::cout << "Zone " << index << ":" << std::endl;
        z->PrintZone();
        std::cout << std::endl;              
    }
}

static void RandomMap(std::list<Zone *>* map){
    std::string selection;
    if (map->size() > 0){
        selection = SelectionWrapper("The map is not empty, proceeding will delete the previous one. Continue? y/n", 'a');
        if (selection == "n" || selection == "N") {
            std::cout << "Aborting..." << std::endl << std::endl;
            return;
        }
    }

    DeleteMap(map);
    uint8_t count = SelectionWrapper("How many zones do you want to generate?", (uint8_t)UINT8_MAX);
    if (count == 0)
        return;
        
    for (int i = 0; i < count; i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib((uint8_t)KITCHEN, (uint8_t)LAKE);

        uint8_t zone = distrib(gen);

        InsertZone(map, zone);
    } 
}

static Object_t GenerateObject(uint8_t zone, uint8_t prob){
    switch(zone){
        case 1:
            if(prob <= 30) {
                return JUNK;
            } else if (prob > 30 && prob <= 50) {
                return BANDAID;
            } else if (prob > 50 && prob <= 90){
                return KNIFE;
            } else {
                return ADRENALINE;
            }
            break;

        case 2:
            if(prob <= 20) {
                return JUNK;
            } else if (prob > 20 && prob <= 30) {
                return BANDAID;
            } else if (prob > 30 && prob <= 40){
                return KNIFE;
            } else if (prob > 40 && prob <= 70) {
                return GUN;
            } else {
                return ADRENALINE;
            }
            break;

        case 3:
            if(prob <= 20) {
                return JUNK;
            } else if (prob > 20 && prob <= 30) {
                return BANDAID;
            } else if (prob > 30 && prob <= 60){
                return KNIFE;
            } else if (prob > 40 && prob <= 90) {
                return GASOLINE;
            } else {
                return ADRENALINE;
            }
            break;

        case 4:
            if(prob <= 80) {
                return JUNK;
            } else if (prob > 80 && prob <= 90){
                return KNIFE;
            } else {
                return GASOLINE;
            }
            break;
            
        case 5:
            if(prob <= 70) {
                return JUNK;
            } else if (prob > 70 && prob <= 80){
                return KNIFE;
            } else {
                return GASOLINE;
            }
            break;
        case 6:
        if(prob <= 90) {
                return JUNK;
            } else {
                return KNIFE;
            }
            break;

        default:
            break;
    }
    return JUNK;
}

static void InitPlayers(std::vector<Player *>* players, std::list<Zone *>* map){
    do {
        DeletePlayers(players);
        auto selection = SelectionWrapper("Do you want to play with default characters? y/n", 'a');

        if (selection == "y" || selection == "Y") {
            players->push_back(new Player{"Giacomo",'M', map->front()});
            players->push_back(new Player{"Marzia",'F', map->front()});
        } else {
            auto count = 0;

            do {
                count = SelectionWrapper("How many players do you want to add? (Max 5)", (uint8_t)5);
            } while(count == 0 || count > 5);

            for (int i = 0; i < count; i++){
                do {
                    selection = SelectionWrapper("What's the name of Player " + std::to_string(i+1) + "?", 'a');
                    if (!selection.empty()){
                        break;
                    } 
                    std::cout << "Insert a valid name, not an empty one!!!" << std::endl << std::endl;
                } while (selection.empty());

                std::string sex = "";
                do {
                    sex = SelectionWrapper("What's the gender of Player " + std::to_string(i+1) + "? Male/Female/Unknown", 'a');
                    if (ValidGenders.find(sex) != ValidGenders.end()){
                        break;
                    }
                    std::cout << "Insert a valid gender, you idiot!" << std::endl << std::endl; 
                } while (ValidGenders.find(sex) == ValidGenders.end());
                
                players->push_back(new Player{selection, sex.at(0), map->front()});
            }
        }
        return;
    } while(true);
}