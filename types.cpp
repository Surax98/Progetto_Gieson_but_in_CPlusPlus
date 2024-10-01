#include "gamelib.h"
#include <unordered_set>

std::map<Object_t, std::string> Object_n = {
    {JUNK, "Junk"},
    {BANDAID, "Bandaid"},
    {KNIFE, "Knife"},
    {GUN, "Gun"},
    {GASOLINE, "Gasoline"},
    {ADRENALINE, "Adrenaline"}
};

std::map<Zone_t, std::string> Zone_n = {
    {KITCHEN, "Kitchen"},
    {LIVING_ROOM, "Living Room"},
    {GARAGE, "Garage"},
    {ROAD, "Road"},
    {LAKE, "Lake"},
    {EXIT, "Exit"}
};

std::unordered_set<std::string> ValidGenders = {
        "Unknown", "U", "u", "unknown",
        "Male", "M", "m", "male",
        "Female", "F", "f", "female"
};

std::unordered_set<Object_t> Weapons = {
    KNIFE, GUN, GASOLINE
};