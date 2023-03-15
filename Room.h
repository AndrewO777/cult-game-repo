#ifndef ROOM_H
#define ROOM_H
#include "Enemy.h"
#include "Item.h"
#include "Interact.h"
#include "Puzzle.h"
#include <string>
#include <vector>

class Room
{
public:
	Room(std::string name);
	Room* GetDirection(std::string dir);
	void SetDirection(std::string dir, Room* pNewRoom);
	std::string GetName() const;
	std::string GetDesc();
	std::string m_RoomDesc;
	std::vector<Item> m_Inv;
	std::vector<Interact> m_Interacts;
	std::vector<Puzzle> m_Puzzles;
	std::vector<Enemy> m_Enemies;
private:
	std::string m_RoomName;
	Room* m_pNorth = 0;
	Room* m_pEast = 0;
	Room* m_pSouth = 0;
	Room* m_pWest = 0;
};
#endif
