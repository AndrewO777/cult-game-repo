#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "Map.h"
#include "Player.h"

void ToUpper(std::string& input);
int main()
{
	srand(static_cast<unsigned int>(time(0)));
	Map myMap;
	Map myMap2;
	myMap2.m_TopFloor = false;
	myMap2.CreatePuzzles();
	Player myPlayer;
	std::vector<std::string> dirs;
	dirs.push_back("");
	myMap.RandGen(dirs, "", 0);
	myMap2.RandGen(dirs, "", 0);
	myPlayer.Move(myMap.GetHead());
	std::string input;
	bool cmdRdy = false;
	std::cout << "If you dont know the commands type 'help'." << std::endl;
	while (input != "CLOSE" && myPlayer.m_Health > 0 && myPlayer.m_Sanity > 0)
	{
		if (!cmdRdy && input != "CLOSE")
		{
			std::getline(std::cin, input);
			ToUpper(input);
		}
		else
		{
			cmdRdy = false;
		}
		if (input == "NORTH" || input == "N")
		{
			if ((myPlayer.GetCur())->GetDirection(input) != 0)
			{
				myPlayer.Move(input);
			}
			else
			{
				std::cout << "Cant go that way!" << std::endl;
			}
		}
		else if (input == "EAST" || input == "E")
		{
			if ((myPlayer.GetCur())->GetDirection(input) != 0)
			{
				myPlayer.Move(input);
			}
			else
			{
				std::cout << "Cant go that way!" << std::endl;
			}
		}
		else if (input == "SOUTH" || input == "S")
		{
			if ((myPlayer.GetCur())->GetDirection(input) != 0)
			{
				myPlayer.Move(input);
			}
			else
			{
				std::cout << "Cant go that way!" << std::endl;
			}
		}
		else if (input == "WEST" || input == "W")
		{
			if ((myPlayer.GetCur())->GetDirection(input) != 0)
			{
				myPlayer.Move(input);
			}
			else
			{
				std::cout << "Cant go that way!" << std::endl;
			}
		}
		else if (input == "LOOK")
		{
			std::cout << myPlayer.Look() << std::endl;
		}
		else if (input.substr(0, 4) == "TAKE")
		{
			if (input.length() > 5)
			{
				myPlayer.Take(input.substr(5));
			}
			else
			{
				std::cout << "Take what?" << std::endl;
			}
		}
		else if (input == "INVENTORY")
		{
			myPlayer.Inventory();
		}
		else if (input.substr(0, 4) == "DROP")
		{
			if (input.length() > 5)
			{
				myPlayer.Drop(input.substr(5));
			}
			else
			{
				std::cout << "Drop what?" << std::endl;
			}
		}
		else if (input.substr(0, 5) == "CHECK")
		{
			if (input.length() > 6)
			{
				input = myPlayer.Check(input.substr(6));
				if (input.substr(0, 4) != "TAKE" && input != "")
				{
					cmdRdy = true;
				}
			}
			else
			{
				std::cout << "Check what?" << std::endl;
			}
		}
		else if (input.substr(0, 3) == "USE")
		{
			if (input.length() > 4)
			{
				myPlayer.Use(input.substr(4));
			}
			else
			{
				std::cout << "Use what?" << std::endl;
			}
		}
		else if (input == "DOWN")
		{
			myPlayer.Down(myMap2);
		}
		else if (input == "UP")
		{
			myPlayer.Up(myMap);
		}
		else if (input == "HEALTH")
		{
			myPlayer.Health();
		}
		else if (input.substr(0, 6) == "ATTACK")
		{
			if (input.length() > 7)
			{
				bool found = false;
				for (std::vector<Item>::iterator iter = myPlayer.m_Inv.begin(); iter != myPlayer.m_Inv.end(); ++iter)
				{
					std::string compare = (*iter).GetName();
					ToUpper(compare);
					if (input.substr(7) == compare)
					{
						if ((*iter).GetDamage() == 0)
						{
							std::cout << "That won't do anything!" << std::endl;
						}
						else
						{
							myPlayer.Attack(*iter);
						}
						found = true;
						break;
					}
				}
				if (!found)
				{
					std::cout << "You don't have that to attack with." << std::endl;
				}
			}
			else
			{
				std::cout << "Attack with what?" << std::endl;
			}
		}
		else if (input == "GODMODE123")
		{
			myPlayer.m_Health += 10000;
			myPlayer.m_Sanity += 10000;
		}
		else if (input == "DEBUGMENU123")
		{
			myPlayer.DebugMenu();
		}
		else if (input == "HELP")
		{
			myPlayer.Help();
		}
		else if (input == "EXIT")
		{
			myPlayer.Exit();
		}
		else if (input != "CLOSE")
		{
			std::cout << "Sorry I don't understand." << std::endl;
		}
	}
	if (myPlayer.m_Health <= 0 || myPlayer.m_Sanity <= 0)
	{
		std::cout << "Game Over, press any key to continue.\n" << std::endl;
		std::getline(std::cin, input);
	}
	return 0;
}

void ToUpper(std::string& input)
{
	for (std::string::iterator iter = input.begin(); iter != input.end(); ++iter)
	{
		*iter = toupper(*iter);
	}
}
