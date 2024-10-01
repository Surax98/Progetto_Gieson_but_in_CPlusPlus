#pragma once
#include <map>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <iostream>

enum PlayerState_t {DEAD, WOUNDED, ALIVE, SAFE, UNKNOWN};
enum Zone_t {KITCHEN, LIVING_ROOM, GARAGE, ROAD, LAKE, EXIT};
enum Object_t {JUNK, BANDAID, KNIFE, GUN, GASOLINE, ADRENALINE};

extern std::map<Object_t, std::string> Object_n;
extern std::map<Zone_t, std::string> Zone_n;

class Zone {
    private:
        Zone_t m_Type;
        Object_t m_Object;
        bool m_ObjectHidden;
        Zone * m_NextZone;

    public:
        Zone(Zone_t t, Object_t o):
            m_Type(t), m_Object(o), m_ObjectHidden(true), m_NextZone(NULL) {}

        Zone_t GetType() const {
            return m_Type;
        }

        Object_t GetObject() const {
            return m_Object;
        }

        Zone * GetNext() const {
            return m_NextZone;
        }

        void SetNext(Zone * z){
            m_NextZone = z;
        }

        void PrintZone() const {
            std::cout << "Type: " << Zone_n[this->GetType()] << std::endl;
            std::cout << "Object: " << Object_n[this->GetObject()] << std::endl;
            if (m_NextZone != NULL) {
                std::cout << "Next Zone: " << Zone_n[this->GetNext()->GetType()] << std::endl;
            }
        }

        void SetHiddenObject(bool hidden){
            m_ObjectHidden = hidden;
        }

        bool GetHiddenObject(){
            return m_ObjectHidden;
        }
};

class Player {
    private:
        std::string m_Name;
        PlayerState_t m_State;
        Zone * m_Position;
        uint8_t m_Gasoline;
        std::map<Object_t, ushort> m_Backpack;
        char m_Gender;
        bool m_Adrenaline;

    public:

        Player() :
            m_Name(""), m_State(UNKNOWN), m_Position(NULL), m_Gasoline(0), m_Backpack(), m_Gender('U') {}

        Player(std::string n, char s, Zone * p) :
            m_Name(n), m_State(ALIVE), m_Position(p), m_Gasoline(4), m_Backpack(), m_Gender(s) {
                if (m_Name == "Giacomo") {
                    m_Backpack[GUN] = 0;
                    m_Backpack[BANDAID] = 0;
                    m_Backpack[ADRENALINE] = 0;
                    m_Backpack[GASOLINE] = 0;
                    m_Backpack[KNIFE] = 1;
                } else if (m_Name == "Marzia"){
                    m_Backpack[GUN] = 0;
                    m_Backpack[BANDAID] = 0;
                    m_Backpack[ADRENALINE] = 2;
                    m_Backpack[GASOLINE] = 0;
                    m_Backpack[KNIFE] = 0;
                } else if (m_Name == "Giampaolone"){
                    m_Backpack[GUN] = 20;
                    m_Backpack[BANDAID] = 20;
                    m_Backpack[ADRENALINE] = 20;
                    m_Backpack[GASOLINE] = 20;
                    m_Backpack[KNIFE] = 20;
                } else {
                    m_Backpack[GUN] = 0;
                    m_Backpack[BANDAID] = 1;
                    m_Backpack[ADRENALINE] = 0;
                    m_Backpack[GASOLINE] = 1;
                    m_Backpack[KNIFE] = 0;
                }
            }

        void SetState(PlayerState_t s){
            m_State = s;
        }

        void SetPosition(Zone * p){
            m_Position = p;
        }
        
        void AddItem(Object_t o){
            m_Backpack[o]++;
        }

        void RemoveItem(Object_t o){
            m_Backpack[o] = m_Backpack[o] == 0 ? 0 : --m_Backpack[o];
        }

        ushort GetItemCount (const Object_t o) const {
            return m_Backpack.at(o);
        }

        PlayerState_t GetState() const {
            return m_State;
        }

        Zone * GetPosition() const {
            return m_Position;
        }

        std::string GetName() const {
            return m_Name;
        }

        const char GetGender() const {
            return m_Gender;
        }

        void SetAdrenaline(bool a) {
            m_Adrenaline = a;
        }

        bool GetAdrenaline() const {
            return m_Adrenaline;
        }

        std::map<Object_t, ushort>* GetBackpack() {
            return &m_Backpack;
        }

        void SetGasoline(uint8_t g){
            m_Gasoline = g;
        }

        uint8_t GetGasoline () const {
            return m_Gasoline;
        }
};

void ClearScreen();
void Play(std::vector<Player *>*);
void MapEditor(std::list<Zone *>*, std::vector<Player *>*);
void DeleteMap(std::list<Zone *>*);
void DeletePlayers(std::vector<Player *>*);
void InvalidWrapper();
bool CheckIfRunning(std::vector<Player *>*);
uint8_t SelectionWrapper(const std::string&, uint8_t);
std::string SelectionWrapper(const std::string&, char);
