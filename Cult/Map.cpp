#include <iostream>
#include <cstdlib>
#include "Map.h"
#include <algorithm>

Map::Map()
{
	PopulateRoomNames();
	CreateItems();
	CreateInteracts();
	CreatePuzzles();
	CreateEnemies();
	int randNum = rand() % 2;
	if (randNum == 1)
	{
		m_MapPuzzle.push_back("Sloted Chest");
		m_MapPuzzle.push_back("Sapphire");
		m_MapPuzzle.push_back("Ruby");
		m_MapPuzzle.push_back("Emerald");
	}
	else if (randNum == 0)
	{
		m_MapPuzzle.push_back("Safe");
		m_MapPuzzle.push_back("Safe Key");
	}
}

Map::~Map()
{
	Clear(m_pHead, 0);
}

void Map::Clear(Room* pRoom, Room* pPrev)
{
	while (m_pHead != 0)
	{
		std::string dirs[4] = { "NORTH", "EAST", "SOUTH", "WEST" };
		if (ChkDeadEnd(pRoom))
		{
			if (pRoom == m_pHead)
			{
				std::cout << "Room has been deleted: " << pRoom->GetName() << std::endl;
				delete pRoom;
				m_pHead = 0;
			}
			else
			{
				std::cout << "Room has been deleted: " << pRoom->GetName() << std::endl;
				delete pRoom;
				pRoom = m_pHead;
				pPrev = 0;
			}
		}
		else
		{
			for (size_t i = 0; i < 4; ++i)
			{
				if (pRoom->GetDirection(dirs[i]) != 0 && pRoom->GetDirection(dirs[i]) != pPrev)
				{
					Room* pTemp = pRoom->GetDirection(dirs[i]);
					if (ChkDeadEnd(pTemp))
					{
						pRoom->SetDirection(dirs[i], 0);
						pPrev = pRoom;
						pRoom = pTemp;
						break;
					}
					else
					{
						pPrev = pRoom;
						pRoom = pTemp;
						break;
					}
				}
			}
		}
	}
}

bool Map::ChkDeadEnd(Room* pRoom)
{
	std::string dirs[4] = {"NORTH", "EAST", "SOUTH", "WEST"};
	short counter = 0;
	if (pRoom == m_pHead)
	{
		++counter;
	}
	for (size_t i = 0; i < 4; ++i)
	{
		if (pRoom->GetDirection(dirs[i]) != 0)
		{
			++counter;
		}
	}
	if (counter >= 2)
	{
		return false;
	}
	return true;
}

void Map::AddRoom(std::vector<std::string> dirs, std::string name)
{
	if (m_pHead == 0)
	{
		m_pHead = new Room(name);
		GiveDescInter(m_pHead);
		GenRandItems(m_pHead);
	}
	else
	{
		Room* pTemp = m_pHead;
		for (std::vector<std::string>::const_iterator iter = dirs.begin(); iter != dirs.end(); ++iter)
		{
			if (iter != dirs.end() - 1)
			{
				if (pTemp->GetDirection(*iter) != 0)
				{
					pTemp = pTemp->GetDirection(*iter);
				}
				else
				{
					std::cout << "Invalid Directions, exiting AddRoom function.\n";
					break;
				}
			}
			else
			{
				pTemp->SetDirection(*iter, new Room(name));
				Room* pTemp2 = pTemp->GetDirection(*iter);
				GiveDescInter(pTemp2);
				GenRandItems(pTemp2);
				pTemp2->SetDirection(OppDir(*iter), pTemp);
			}
		}
	}
}

void Map::RemoveRoom(std::vector<std::string> dirs)
{
	if (dirs[0] == "")
	{
		delete m_pHead;
	}
	else
	{
		Room* pTemp = m_pHead;
		for (std::vector<std::string>::const_iterator iter = dirs.begin(); iter != dirs.end(); ++iter)
		{
			if (pTemp->GetDirection(*iter) != 0)
			{
				pTemp = pTemp->GetDirection(*iter);
			}
			else
			{
				std::cout << "Invalid Directions, exiting RemoveRoom function.\n";
				pTemp = 0;
				break;
			}
		}
		std::string dirs[4] = {"NORTH", "EAST", "SOUTH", "WEST"};
		Room* pTemp2;
		for (size_t i = 0; i < 4; ++i)
		{
			if (pTemp->GetDirection(dirs[i]) != 0)
			{
				pTemp2 = pTemp->GetDirection(dirs[i]);
				pTemp2->SetDirection(OppDir(dirs[i]), 0);
			}
		}
		delete pTemp;
	}
}

void Map::RandGen(std::vector<std::string> dirs, std::string prevDir, short depth)
{
	std::string name;
	std::string stdDirs[4] = {"NORTH", "EAST", "SOUTH", "WEST"};
	if (size < 8)
	{
		if (m_pHead == 0)
		{
			if (m_TopFloor)
			{
				name = "Sacrificial Room";
				AddRoom(dirs, name);
				++size;
				dirs.clear();
				RandGen(dirs, prevDir, 0);
			}
			else
			{
				name = "Lobby";
				AddRoom(dirs, name);
				++size;
				dirs.clear();
				RandGen(dirs, prevDir, 0);
			}
		}
		else
		{
			name = GiveRandName();
			short randRooms;
			if (ChkDeadEnd(m_pHead))
			{
				randRooms = rand() % 4;
			}
			else
			{
				if (size < size - 2)
				{
					randRooms = rand() % 3;
				}
				else if (size < size - 1)
				{
					randRooms = rand() % 2;
				}
				else
				{
					randRooms = 0;
				}
			}
			if (randRooms == 0)
			{
				short randNum = rand() % 4;
				while (stdDirs[randNum] == prevDir)
				{
					randNum = rand() % 4;
				}
				dirs.push_back(stdDirs[randNum]);
				AddRoom(dirs, name);
				++size;
				prevDir = OppDir(stdDirs[randNum]);
				if (depth < 3)
				{
					RandGen(dirs, prevDir, ++depth);
				}
			}
			else if (randRooms == 1)
			{
				short randNum0 = rand() % 4;
				short randNum1 = rand() % 4;
				while (stdDirs[randNum0] == prevDir || stdDirs[randNum1] == prevDir || randNum0 == randNum1)
				{
					randNum0 = rand() % 4;
					randNum1 = rand() % 4;
				}
				size += 2;
				dirs.push_back(stdDirs[randNum0]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[randNum0]);	
				if (depth < 3)
				{
					RandGen(dirs, prevDir, ++depth);
				}
				dirs.pop_back();
				name = GiveRandName();
				dirs.push_back(stdDirs[randNum1]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[randNum1]);
				if (depth < 3)
				{
					RandGen(dirs, prevDir, ++depth);
				}
			}
			else if (randRooms == 2)
			{
				short randNum0 = rand() % 4;
				short randNum1 = rand() % 4;
				short randNum2 = rand() % 4;
				while (stdDirs[randNum0] == prevDir || stdDirs[randNum1] == prevDir || stdDirs[randNum2] == prevDir || randNum0 == randNum1 || randNum0 == randNum2 || randNum1 == randNum2)
				{
					randNum0 = rand() % 4;
					randNum1 = rand() % 4;
					randNum2 = rand() % 4;
				}
				size += 3;
				dirs.push_back(stdDirs[randNum0]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[randNum0]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
				dirs.pop_back();
				name = GiveRandName();
				dirs.push_back(stdDirs[randNum1]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[randNum1]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
				dirs.pop_back();
				name = GiveRandName();
				dirs.push_back(stdDirs[randNum2]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[randNum2]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
			}
			else if (randRooms == 3)
			{
				size += 4;
				dirs.push_back(stdDirs[0]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[0]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
				dirs.pop_back();
				name = GiveRandName();
				dirs.push_back(stdDirs[1]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[1]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
				dirs.pop_back();
				name = GiveRandName();
				dirs.push_back(stdDirs[2]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[2]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
				dirs.pop_back();
				name = GiveRandName();
				dirs.push_back(stdDirs[3]);
				AddRoom(dirs, name);
				prevDir = OppDir(stdDirs[3]);
				if (depth < 2)
				{
					RandGen(dirs, prevDir, ++depth);
				}
			}
		}
	}
	if (size < 8 && depth == 0)
	{
		Room* pFind = m_pHead;
		for(short i = 0; !ChkDeadEnd(pFind); ++i)
		{
			while(true)
			{
				short randNum = rand() % 4;
				dirs.push_back(stdDirs[randNum]);
				if (pFind->GetDirection(dirs[i]) != 0)
				{
					pFind = pFind->GetDirection(dirs[i]);
					break;
				}
				dirs.pop_back();
			}
		}
		prevDir = OppDir(dirs[dirs.size() - 1]);
		RandGen(dirs, prevDir, 0);
	}
}

Room* Map::GetHead()
{
	return m_pHead;
}

void Map::PopulateRoomNames()
{
	roomNames.push_back("Kitchen");
	roomNames.push_back("Bathroom");
	roomNames.push_back("Dinning Room");
	roomNames.push_back("Bedroom");
	roomNames.push_back("Storage Room");
	roomNames.push_back("Closet");
	roomNames.push_back("Music Room");
	roomNames.push_back("Art Room");
	roomNames.push_back("Hallway");
	roomNames.push_back("Living Room");
	roomNames.push_back("Bedroom 2");
	roomNames.push_back("Bedroom 3");
	roomNames.push_back("Bathroom 2");
	roomNames.push_back("Office");
	roomNames.push_back("Library");
	random_shuffle(roomNames.begin(), roomNames.end());
}

std::string Map::GiveRandName()
{
	std::string name = roomNames[0];
	short counter = 0;
	for (std::vector<std::string>::const_iterator iter = usedRooms.begin(); iter != usedRooms.end();)
	{
		if (name == *iter)
		{
			name = roomNames[counter];
			++counter;
			iter = usedRooms.begin();
		}
		else
		{
			++iter;
		}
	}
	usedRooms.push_back(name);
	return name;
}

std::string Map::OppDir(std::string dir)
{
	std::string dirs[4] = {"NORTH", "EAST", "SOUTH", "WEST"};
	std::string oppDirs[4] = {"SOUTH", "WEST", "NORTH", "EAST"};
	for (size_t i = 0; i < 4; ++i)
	{
		if (dir == dirs[i])
		{
			return oppDirs[i];
		}
	}
	return "";
}

void Map::GiveRoomInteracts(int amtInteract, Room* myRoom)
{
	while (amtInteract > 0)
	{
		Interact temp = m_InteractList[rand() % m_InteractList.size()];
		bool interactExists = false;
		for (std::vector<Interact>::const_iterator iter = myRoom->m_Interacts.begin(); iter != myRoom->m_Interacts.end(); ++iter)
		{
			if ((*iter).GetName() == temp.GetName())
			{
				interactExists = true;
			}
		}
		if (!interactExists)
		{
			myRoom->m_Interacts.push_back(temp);
			--amtInteract;
			myRoom->m_RoomDesc += temp.m_Desc; 
		}
	}
}

void Map::GenEnemies(Room* myRoom)
{
	int chance = rand() % 4;
	if (chance == 2)
	{
		Enemy temp = m_EnemyList[rand() % m_EnemyList.size()];
		myRoom->m_Enemies.push_back(temp);
		myRoom->m_RoomDesc += temp.GetDesc();
	}
}

void Map::CreateEnemies()
{
	m_EnemyList.push_back(Enemy("Vampire Cultist", " There is a vampire, you've never seen one but you can tell this is the real thing (-1 sanity).", " The Vampire falls and turns to dust as you deal the finishing blow.", 7, 2, 1, true));
	m_EnemyList.push_back(Enemy("Giant Spider", " You see a giant spider, it's the size of a small dog. (-1 sanity).", " The spider collapses and turns to dust as you deal the finishing blow.", 8, 1, 1, false));
	m_EnemyList.push_back(Enemy("Prisoner", " A guy is sitting in a corner muttering something to himself, he sees you and starts running toward you.", " The insane prisoner collapses and turns to dust as you deal the finishing blow.", 5, 1, 0, false));
}

void Map::GenPuzzle(Room* myRoom)
{
	if (m_PuzzleObjs == 0)
	{
		if (m_MapPuzzle[0] == "Sloted Chest")
		{
			myRoom->m_Puzzles.push_back(m_PuzzleList[2]);
			myRoom->m_RoomDesc += m_PuzzleList[2].m_Desc;
		}
		else
		{
			myRoom->m_Puzzles.push_back(m_PuzzleList[3]);
			myRoom->m_RoomDesc += m_PuzzleList[3].m_Desc;
		}
		++m_PuzzleObjs;
	}
	else if (m_PuzzleObjs == 1)
	{
		if (m_MapPuzzle[0] == "Sloted Chest")
		{
			if (myRoom->m_Interacts.size() > 0)
			{
				for (std::vector<Interact>::iterator iter = myRoom->m_Interacts.begin(); iter != myRoom->m_Interacts.end(); ++iter)
				{
					(*iter).m_Contains = m_PuzzleItems[0];
					break;
				}
				++m_PuzzleObjs;
			}
		}
		else
		{
			if (myRoom->m_Interacts.size() > 0)
			{
				for (std::vector<Interact>::iterator iter = myRoom->m_Interacts.begin(); iter != myRoom->m_Interacts.end(); ++iter)
				{
					(*iter).m_Contains = m_PuzzleItems[3];
					break;
				}
				++m_PuzzleObjs;
			}
		}
	}
	else if (m_PuzzleObjs == 2 && m_MapPuzzle[0] == "Sloted Chest")
	{
		if (myRoom->m_Interacts.size() > 0)
		{
			for (std::vector<Interact>::iterator iter = myRoom->m_Interacts.begin(); iter != myRoom->m_Interacts.end(); ++iter)
			{
				(*iter).m_Contains = m_PuzzleItems[1];
				break;
			}
			++m_PuzzleObjs;
		}
	}
	else if (m_PuzzleObjs == 3 && m_MapPuzzle[0] == "Sloted Chest")
	{
		if (myRoom->m_Interacts.size() > 0)
		{
			for (std::vector<Interact>::iterator iter = myRoom->m_Interacts.begin(); iter != myRoom->m_Interacts.end(); ++iter)
			{
				(*iter).m_Contains = m_PuzzleItems[2];
				break;
			}
			++m_PuzzleObjs;
		}
	}
}

void Map::GiveDescInter(Room* myRoom)
{
	if (myRoom->GetName() == "Sacrificial Room")
	{
		myRoom->m_RoomDesc = "You look around and see a pentagram drawn in blood on the floor in the center of the room. In the corner you see a trapdoor that appears to lead downwards.";
		myRoom->m_Puzzles.push_back(m_PuzzleList[0]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
	}
	else if (myRoom->GetName() == "Lobby")
	{
		myRoom->m_RoomDesc = "You see a front door that leads to the outside. There are strange writings all over the walls, you can't tell what they say or mean.";
		myRoom->m_Puzzles.push_back(m_PuzzleList[1]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Kitchen")
	{
		myRoom->m_RoomDesc = "You see some countertops, cabinets and some general kitchen appliances such as a stove, microwave and a refrigerator.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[0]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[1]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[2]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[3]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[4]);
		int amtInteract = rand() % 2;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Bathroom")
	{
		myRoom->m_RoomDesc = "You find yourself in a bathroom that seems like it hasn't been cleaned in years. There is a mirror with a crack in the center, a sink, a bathtub and a toilet.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[8]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[9]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[13]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Dinning Room")
	{
		myRoom->m_RoomDesc = "In this room there is a large dinning table in the center and a large chandelier above the table. There is a large window on one side of the room.";
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Bedroom")
	{
		myRoom->m_RoomDesc = "You see a basic bedroom with a bed in the corner and a clock above it.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[7]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Storage Room")
	{
		myRoom->m_RoomDesc = "This room is mostly empty.";
		int amtInteract = rand() % 4;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Closet")
	{
		myRoom->m_RoomDesc = "You find yourself in a small closet with some clothing hanging about.";
		int amtInteract = rand() % 2;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Music Room")
	{
		myRoom->m_RoomDesc = "You look around the room and see a piano.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[12]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Art Room")
	{
		myRoom->m_RoomDesc = "In this room you see several paintings a easel and a few blank canvases.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[11]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[10]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Hallway")
	{
		myRoom->m_RoomDesc = "This hallway is mostly empty.";
		int amtInteract = rand() % 2;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Living Room")
	{
		myRoom->m_RoomDesc = "Looking around this room you see some couches and chairs with a table in the center.";
		int amtInteract = rand() % 4;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Bedroom 2")
	{
		myRoom->m_RoomDesc = "This room is a bedroom with a bed against the wall.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[7]);
		int amtInteract = rand() % 4;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Bedroom 3")
	{
		myRoom->m_RoomDesc = "You see a bed in the corner.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[7]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Bathroom 2")
	{
		myRoom->m_RoomDesc = "In here you find a toilet and a sink with a mirror above it.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[9]);
		myRoom->m_Interacts.push_back(m_PrivateInteractList[8]);
		int amtInteract = rand() % 2;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Office")
	{
		myRoom->m_RoomDesc = "This room has a desk on one side.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[6]);
		int amtInteract = rand() % 3;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	else if (myRoom->GetName() == "Library")
	{
		myRoom->m_RoomDesc = "You see a few chairs and several bookcases in here.";
		myRoom->m_Interacts.push_back(m_PrivateInteractList[5]);
		int amtInteract = rand() % 4;
		GiveRoomInteracts(amtInteract, myRoom);
		GenEnemies(myRoom);
	}
	if (size == 1)
	{
		GenPuzzle(myRoom);
	}
	else if (size >= 5)
	{
		GenPuzzle(myRoom);
	}
}

void Map::CreateItems()
{
	m_ItemList.push_back(Item(0,0,6,0,false,"Sacrificial Dagger", " There is a sacrificial dagger laying on the floor."));
	m_ItemList.push_back(Item(0,0,4,0,true,"Wooden Stake", " There is a wooden stake laying on the floor."));
	m_ItemList.push_back(Item(0,2,3,0,false,"Wine", " You see a bottle of wine on the floor."));
	m_ItemList[2].m_AttackUses = 1;
	m_ItemList.push_back(Item(1,1,0,0,false,"Painkillers", " A bottle of painkillers is on the floor."));
	m_ItemList.push_back(Item(1,0,0,0,false,"Bread", " There is a loaf of bread on the floor."));
	m_ItemList.push_back(Item(1,0,0,0,false,"Berries", " You see berries on the floor."));
	m_ItemList.push_back(Item(0,0,0,0,true,"Holy Water", " You see a glass bottle of holy water on the floor."));
	m_ItemList.push_back(Item(0,0,0,1,false,"Cross Necklace", " There is a cross necklace on the floor."));
	m_TrapList.push_back(Trap("Rats", "When you approch to check, rats scurry in every direction around you. (-1 sanity)", 1));
	m_TrapList.push_back(Trap("Snakes", "As you start checking, snakes pop out and slither around you. (-1 sanity)", 1));
	m_TrapList.push_back(Trap("Scream", "You begin checking and hear loud screams all around you. (-1 sanity)", 1));
	m_TrapList.push_back(Trap("Head", "You see a decapitated head and it frightens you. (-2 sanity)", 2));
}

void Map::GenericRandItemGen(Room* myRoom)
{
	if (myRoom->m_Interacts.size() > 0)
	{
		for (std::vector<Interact>::iterator iter = myRoom->m_Interacts.begin(); iter != myRoom->m_Interacts.end(); ++iter)
		{
			if ((*iter).m_Contains.GetName() == "nothing")
			{
				int chance = rand() % 4;
				if (chance < 3)
				{
					if (chance == 1)
					{
						int sChance = rand() % 2;
						if (sChance == 0)
						{
							(*iter).m_Contains = m_ItemList[rand() % m_ItemList.size()];
						}
					}
					else
					{
						(*iter).m_Contains = m_ItemList[rand() % m_ItemList.size()];
					}
				}
				if (chance == 1)
				{
					(*iter).m_Trap = m_TrapList[rand() % m_TrapList.size()];
					(*iter).m_HasTrap = true;
				}
			}
		}
	}
}

void Map::GenRandItems(Room* myRoom)
{
	if (myRoom->GetName() == "Sacrificial Room")
	{
		(myRoom->m_Inv).push_back(m_ItemList[0]);
		myRoom->m_RoomDesc = myRoom->m_RoomDesc + m_ItemList[0].GetDesc();
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Lobby")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Kitchen")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Bathroom")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Dinning Room")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Bedroom")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Storage Room")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Closet")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Music Room")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Art Room")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Hallway")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Living Room")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Bedroom 2")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Bedroom 3")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Bathroom 2")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Office")
	{
		GenericRandItemGen(myRoom);
	}
	else if (myRoom->GetName() == "Library")
	{
		GenericRandItemGen(myRoom);
	}
}

void Map::CreateInteracts()
{
	m_InteractList.push_back(Interact("Barrel", " You see a barrel in the corner."));
	m_InteractList.push_back(Interact("Brown Bag", " There is a brown bag on the floor."));
	m_InteractList.push_back(Interact("Chest", " Against the wall there is a chest."));
	m_InteractList.push_back(Interact("Box", " You see a box."));
	m_InteractList.push_back(Interact("Shelf", " There is a shelf."));
	m_InteractList.push_back(Interact("Painting", " A painting is on one of the walls."));
	m_PrivateInteractList.push_back(Interact("Countertops", ""));
	m_PrivateInteractList.push_back(Interact("Cabinets", ""));
	m_PrivateInteractList.push_back(Interact("Stove", ""));
	m_PrivateInteractList.push_back(Interact("Microwave", ""));
	m_PrivateInteractList.push_back(Interact("Refrigerator", ""));
	m_PrivateInteractList.push_back(Interact("Bookcases", ""));
	m_PrivateInteractList.push_back(Interact("Desk", ""));
	m_PrivateInteractList.push_back(Interact("Bed", ""));
	m_PrivateInteractList.push_back(Interact("Sink", ""));
	m_PrivateInteractList.push_back(Interact("Toilet", ""));
	m_PrivateInteractList.push_back(Interact("Paintings", ""));
	m_PrivateInteractList.push_back(Interact("Easel", ""));
	m_PrivateInteractList.push_back(Interact("Piano", ""));
	m_PrivateInteractList.push_back(Interact("Bathtub", ""));
}

void Map::CreatePuzzles()
{
	m_PuzzleItems.clear();
	m_PuzzleList.clear();
	m_PuzzleList.push_back(Puzzle("Trapdoor", "", "The trapdoor seems to be locked, if you had a key you could unlock it."));
	m_PuzzleList.push_back(Puzzle("Front Door", "", "The Front Door seems to be locked, if you had a key you could unlock it."));
	m_PuzzleList.push_back(Puzzle("Sloted Chest"," You see a sloted chest on the floor","On the chest you see three slots colored blue, green and red."));
	if (m_TopFloor)
	{
		m_PuzzleList[2].m_Contains = Item(0, 0, 0, 0, false, "Trapdoor Key", " You see a trapdoor key on the floor.");
	}
	else
	{
		m_PuzzleList[2].m_Contains = Item(0, 0, 0, 0, false, "Front Door Key", " You see a front door key on the floor.");
	}
	m_PuzzleItems.push_back(Item(0, 0, 0, 0, false, "Sapphire", " There is a sapphire on the floor."));
	m_PuzzleItems.push_back(Item(0, 0, 0, 0, false, "Ruby", " There is a ruby on the floor."));
	m_PuzzleItems.push_back(Item(0, 0, 0, 0, false, "Emerald", " There is a emerald on the floor."));
	m_PuzzleList.push_back(Puzzle("Safe", " There is a safe in the corner", "The safe seems to be locked, it needs a key to open it."));
	if (m_TopFloor)
	{
		m_PuzzleList[3].m_Contains = Item(0, 0, 0, 0, false, "Trapdoor Key", " You see a trapdoor key on the floor.");
	}
	else
	{
		m_PuzzleList[3].m_Contains = Item(0, 0, 0, 0, false, "Front Door Key", " You see a front door key on the floor.");
	}
	m_PuzzleItems.push_back(Item(0, 0, 0, 0, false, "Safe Key", " You see a safe key on the ground."));
}
