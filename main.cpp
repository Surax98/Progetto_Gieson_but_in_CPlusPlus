#include <iostream>
#include <vector>
#include "gamelib.h"

int main(){
    std::list<Zone *> Map;
    std::vector<Player *> Players;
    const std::string menu = "1) Map Editor\n2) Play\n3) Quit the game\n\nChoose an option:";
    const std::string intro = "    It’s Friday, June 13th, 1980, and a group of university students from the Computer Science Study Course \n\
    are heading off on vacation to 'Lake Trasymeno Campground,' which is about to reopen. Twenty-three years earlier, in fact, \n\
    a boy named Gieson drowned at the same campground due to the negligence of a programmer: \n\
    because of a segmentation fault in his boat rental program, Gieson’s family was given a boat with a faulty engine. \n\
    However, Gieson still haunts the campground, seeking revenge against careless Computer Science students \n\
    who took the Programming I course while playing League of Legends... \n\
    Giacomo and Marzia, both first-year students, are cut off from the others when...\n\nPress any key to continue...\n\n";

    SelectionWrapper(intro, 'a');

    do {
        auto selection = SelectionWrapper(menu, (uint8_t)3);
        switch (selection) {
        case 1:
            MapEditor(&Map, &Players);
            break;

         case 2:
            ClearScreen();
            Play(&Players);
            break;

         case 3:
            ClearScreen();
            DeletePlayers(&Players);
            DeleteMap(&Map);
            SelectionWrapper("Exiting the game\n\nPress any key to continue...", 'a');
            exit(0);
            break;
        
        default:
            break;
        }
    } while (CheckIfRunning(&Players));
}