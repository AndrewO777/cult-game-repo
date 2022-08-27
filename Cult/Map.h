#ifndef MAP_H
#define MAP_H
#include "Room.h"
#include "Item.h"
#include "Enemy.h"
#include "Interact.h"
#include "Puzzle.h"
#include "Trap.h"
#include <vector>
#include <string>

class Map
{
public:
	Map();
	~Map();
	void Clear(Room* pRoom, Room* pPrev);
	bool ChkDeadEnd(Room* pRoom);
	void AddRoom(std::vector<std::string> dirs, std::string name);
	void RemoveRoom(std::vector<std::string> dirs);
	void RandGen(std::vector<std::string> dirs, std::string prevDir, short depth);
	Room* GetHead();
	std::string OppDir(std::string dir);
	void PopulateRoomNames();
	std::string GiveRandName();
	void GiveDescInter(Room* myRoom);
	void CreateItems();
	void CreateInteracts();
	void GiveRoomInteracts(int amtInteract, Room* myRoom);
	void GenericRandItemGen(Room* myRoom);
	void CreatePuzzles();
	void CreateEnemies();
	void GenRandItems(Room* myRoom);
	bool m_TopFloor = true;
	std::vector<std::string> m_MapPuzzle;
	void GenPuzzle(Room* myRoom);
	void GenEnemies(Room* myRoom);
private:
	Room* m_pHead = 0;
	int size = 0;
	int m_PuzzleObjs = 0;
	std::vector<std::string> roomNames;
	std::vector<std::string> usedRooms;
	std::vector<Item> m_ItemList;
	std::vector<Interact> m_InteractList;
	std::vector<Interact> m_PrivateInteractList;
	std::vector<Puzzle> m_PuzzleList;
	std::vector<Item> m_PuzzleItems;
	std::vector<Trap> m_TrapList;
	std::vector<Enemy> m_EnemyList;
};
#endif
