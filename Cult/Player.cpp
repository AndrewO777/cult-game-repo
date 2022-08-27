#include <iostream>
#include "Player.h"
#include <algorithm>

void Player::Move(Room* pRoom)
{
	m_pCur = pRoom;
	std::cout << m_pCur->GetName() << std::endl;
	std::cout << m_pCur->GetDesc() << std::endl;
	if (m_pCur->m_Enemies.size() > 0)
	{
		if (m_pCur->m_Enemies[0].m_Health > 0)
		{
			m_Sanity -= m_pCur->m_Enemies[0].m_Sanity;
		}
	}
	m_Visited.push_back(m_pCur);
	AttackedBy();
}

void Player::Move(std::string dir)
{
	m_pCur = m_pCur->GetDirection(dir);
	std::cout << m_pCur->GetName() << std::endl;
	bool visited = false;
	for (std::vector<Room*>::const_iterator iter = m_Visited.begin(); iter != m_Visited.end(); ++iter)
	{
		if (m_pCur == *iter)
		{
			visited = true;
		}
	}
	if (!visited)
	{
		std::cout << m_pCur->GetDesc() << std::endl;
		m_Visited.push_back(m_pCur);
	}
	if (m_pCur->m_Enemies.size() > 0)
	{
		if (m_pCur->m_Enemies[0].m_Health > 0 && !visited)
		{
			m_Sanity -= m_pCur->m_Enemies[0].m_Sanity;
		}
	}
	AttackedBy();
}

Room* Player::GetCur()
{
	return m_pCur;
}

std::string Player::Check(std::string checkAt)
{
	bool found = false;
	Interact* pInter = 0;
	Puzzle* pPuz = 0;
	for (std::vector<Interact>::iterator iter = m_pCur->m_Interacts.begin(); iter != m_pCur->m_Interacts.end(); ++iter)
	{
		if (ToUpper((*iter).GetName()) == checkAt)
		{
			if ((*iter).m_HasTrap == true)
			{
				std::cout << (*iter).m_Trap.GetDesc() << std::endl;
				m_Sanity -= (*iter).m_Trap.GetSanity();
				(*iter).m_HasTrap = false;
			}
			std::cout << "You find " << (*iter).m_Contains.GetName() << " on/in the " << (*iter).GetName() << std::endl;
			pInter = &(*iter);
			found = true;
			break;
		}
	}
	for (std::vector<Puzzle>::iterator iter = m_pCur->m_Puzzles.begin(); iter != m_pCur->m_Puzzles.end(); ++iter)
	{
		if (ToUpper((*iter).GetName()) == checkAt)
		{
			std::cout << (*iter).m_CheckDesc << std::endl;
			pPuz = &(*iter);
			found = true;
			break;
		}
	}
	AttackedBy();
	std::string input = "";
	if (!found)
	{
		std::cout << "Nothing to check about that object." << std::endl;
	}
	else if (m_Sanity > 0)
	{
		do
		{
			std::getline(std::cin, input);
			input = ToUpper(input);
			if (input.length() > 5 && input.substr(0,4) == "TAKE")
			{
				if (pInter != 0)
				{
					Take(input.substr(5), pInter);
				}
				else if (pPuz != 0)
				{
					Take(input.substr(5), pPuz);
				}
			}
			else if (input.substr(0,4) == "TAKE")
			{
				std::cout << "Take what?" << std::endl;
			}
		} while (input.substr(0,4) == "TAKE");
	}
	return input;
}

void Player::Take(std::string takeObj, Interact* interactObj)
{
	bool found = false;
	if (ToUpper(interactObj->m_Contains.GetName()) == takeObj && takeObj != "NOTHING")
	{
		m_Inv.push_back(interactObj->m_Contains);
		interactObj->m_Contains = Item(0,0,0,0,false,"nothing","");
		found = true;
		if (takeObj == "CROSS NECKLACE")
		{
			m_Negate = 1;
		}
		std::cout << "Taken." << std::endl;
	}
	if (!found)
	{
		for (std::vector<Item>::iterator iter = m_pCur->m_Inv.begin(); iter != m_pCur->m_Inv.end(); ++iter)
		{
			if (ToUpper((*iter).GetName()) == takeObj)
			{
				m_Inv.push_back(*iter);
				m_pCur->m_RoomDesc.erase(m_pCur->m_RoomDesc.find((*iter).GetDesc()), (*iter).GetDesc().length());
				m_pCur->m_Inv.erase(iter);
				found = true;
				if (takeObj == "CROSS NECKLACE")
				{
					m_Negate = 1;
				}
				std::cout << "Taken." << std::endl;
				break;
			}
		}
	}
	if (!found)
	{
		std::cout << "You cannot take that." << std::endl;
	}
	AttackedBy();
}

void Player::Take(std::string takeObj, Puzzle* puzzleObj)
{
	bool found = false;
	if (ToUpper(puzzleObj->m_Contains.GetName()) == takeObj && takeObj != "NOTHING")
	{
		bool unlocked = false;
		if (puzzleObj->GetName() == "Sloted Chest" && puzzleObj->m_Lock == 3)
		{
			unlocked = true;
		}
		else if (puzzleObj->GetName() == "Safe" && puzzleObj->m_Lock == 1)
		{
			unlocked = true;
		}
		else if (puzzleObj->GetName() == "Trapdoor" && puzzleObj->m_Lock == 1)
		{
			unlocked = true;
		}
		else if (puzzleObj->GetName() == "Front Door" && puzzleObj->m_Lock == 1)
		{
			unlocked = true;
		}
		if (unlocked)
		{
			m_Inv.push_back(puzzleObj->m_Contains);
			puzzleObj->m_Contains = Item(0, 0, 0, 0, false, "nothing", "");
			found = true;
			std::cout << "Taken." << std::endl;
		}
	}
	if (!found)
	{
		for (std::vector<Item>::iterator iter = m_pCur->m_Inv.begin(); iter != m_pCur->m_Inv.end(); ++iter)
		{
			if (ToUpper((*iter).GetName()) == takeObj)
			{
				m_Inv.push_back(*iter);
				m_pCur->m_RoomDesc.erase(m_pCur->m_RoomDesc.find((*iter).GetDesc()), (*iter).GetDesc().length());
				m_pCur->m_Inv.erase(iter);
				found = true;
				if (takeObj == "CROSS NECKLACE")
				{
					m_Negate = 1;
				}
				std::cout << "Taken." << std::endl;
				break;
			}
		}
	}
	if (!found)
	{
		std::cout << "You cannot take that." << std::endl;
	}
	AttackedBy();
}

std::string Player::Look()
{
	return m_pCur->GetDesc();
}

void Player::Take(std::string takeObj)
{
	bool found = false;
	for (std::vector<Item>::iterator iter = m_pCur->m_Inv.begin(); iter != m_pCur->m_Inv.end(); ++iter)
	{
		if (ToUpper((*iter).GetName()) == takeObj)
		{
			m_Inv.push_back(*iter);
			m_pCur->m_RoomDesc.erase(m_pCur->m_RoomDesc.find((*iter).GetDesc()), (*iter).GetDesc().length());
			m_pCur->m_Inv.erase(iter);
			found = true;
			if (takeObj == "CROSS NECKLACE")
			{
				m_Negate = 1;
			}
			std::cout << "Taken." << std::endl;
			break;
		}
	}
	if (!found)
	{
		std::cout << "You cannot take that." << std::endl;
	}
	AttackedBy();
}

std::string Player::ToUpper(const std::string& input)
{
	std::string temp = input;
	for (std::string::iterator iter = temp.begin(); iter != temp.end(); ++iter)
	{
		*iter = toupper(*iter);
	}
	return temp;
}

void Player::Inventory()
{
	int counter = 1;
	for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
	{
		std::cout << counter << ". " << (*iter).GetName() << std::endl;
		++counter;
	}
	if (m_Inv.size() == 0)
	{
		std::cout << "You have nothing in your inventory." << std::endl;
	}
}

void Player::DebugMenu()
{
	std::cout << "m_Negate: " << m_Negate << std::endl;
}

void Player::Drop(std::string dropObj)
{
	bool found = false;
	for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
	{
		if (ToUpper((*iter).GetName()) == dropObj)
		{
			m_pCur->m_Inv.push_back(*iter);
			m_pCur->m_RoomDesc = m_pCur->m_RoomDesc + (*iter).GetDesc();
			m_Inv.erase(iter);
			found = true;
			if (dropObj == "CROSS NECKLACE")
			{
				m_Negate = 0;
			}
			std::cout << "Droped." << std::endl;
			break;
		}
	}
	if (!found)
	{
		std::cout << "You can't drop what you don't have." << std::endl;
	}
}

void Player::Use(std::string useObj)
{
	bool found = false;
	if (m_pCur->m_Puzzles.size() > 0)
	{
		if (m_pCur->m_Puzzles[0].GetName() == "Sloted Chest")
		{
			if (useObj == "SAPPHIRE" || useObj == "RUBY" || useObj == "EMERALD")
			{
				for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
				{
					if (ToUpper((*iter).GetName()) == useObj)
					{
						m_Inv.erase(iter);
						found = true;
						std::cout << "Used." << std::endl;
						m_pCur->m_Puzzles[0].m_Lock += 1;
						m_pCur->m_Puzzles[0].TestLock();
						break;
					}
				}
			}
		}
		else if (m_pCur->m_Puzzles[0].GetName() == "Safe")
		{
			if (useObj == "SAFE KEY")
			{
				for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
				{
					if (ToUpper((*iter).GetName()) == useObj)
					{
						m_Inv.erase(iter);
						found = true;
						std::cout << "Used." << std::endl;
						m_pCur->m_Puzzles[0].m_Lock += 1;
						m_pCur->m_Puzzles[0].TestLock();
						break;
					}
				}
			}
		}
		else if (m_pCur->m_Puzzles[0].GetName() == "Trapdoor")
		{
			if (useObj == "TRAPDOOR KEY")
			{
				for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
				{
					if (ToUpper((*iter).GetName()) == useObj)
					{
						m_Inv.erase(iter);
						found = true;
						std::cout << "Used." << std::endl;
						m_pCur->m_Puzzles[0].m_Lock += 1;
						m_pCur->m_Puzzles[0].TestLock();
						break;
					}
				}
			}
		}
		else if (m_pCur->m_Puzzles[0].GetName() == "Front Door")
		{
			if (useObj == "FRONT DOOR KEY")
			{
				for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
				{
					if (ToUpper((*iter).GetName()) == useObj)
					{
						m_Inv.erase(iter);
						found = true;
						std::cout << "Used." << std::endl;
						m_pCur->m_Puzzles[0].m_Lock += 1;
						m_pCur->m_Puzzles[0].TestLock();
						break;
					}
				}
			}
		}
	}
	for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
	{
		if (ToUpper((*iter).GetName()) == useObj)
		{
			if ((*iter).GetHeal() > 0 || (*iter).GethealSanity() > 0)
			{
				if (m_Health < 5)
				{
					m_Health += (*iter).GetHeal();
				}
				if (m_Sanity < 7)
				{
					m_Sanity += (*iter).GethealSanity();
				}
				m_Inv.erase(iter);
				std::cout << "Used." << std::endl;
			}
			else if ((*iter).GetName() == "Holy Water")
			{
				if (m_pCur->m_Enemies.size() > 0)
				{
					if (m_pCur->m_Enemies[0].m_Undead)
					{
						m_pCur->m_Enemies[0].m_Health -= 20;
						m_pCur->m_Enemies[0].Die();
						m_Inv.erase(iter);
						m_pCur->m_RoomDesc.erase(m_pCur->m_RoomDesc.find(m_pCur->m_Enemies[0].GetDesc()), m_pCur->m_Enemies[0].GetDesc().length());
						std::cout << "Used." << std::endl;
					}
				}
				else
				{
					std::cout << "That won't do anything." << std::endl;
				}
			}
			else
			{
				std::cout << "That won't do anything." << std::endl;
			}
			found = true;
			break;
		}
	}
	if (!found)
	{
		std::cout << "You can't use what you don't have." << std::endl;
	}
	AttackedBy();
}

void Player::Down(Map& pMap)
{
	if (m_pCur->GetName() == "Sacrificial Room" && m_pCur->m_Puzzles[0].m_Lock == 1)
	{
		Move(pMap.GetHead());
	}
	else
	{
		std::cout << "You can't go down here." << std::endl;
	}
}

void Player::Up(Map& pMap)
{
	if (m_pCur->GetName() == "Lobby")
	{
		Move(pMap.GetHead());
	}
	else
	{
		std::cout << "You can't go up here." << std::endl;
	}
}

void Player::Exit()
{
	if (m_pCur->GetName() == "Lobby" && m_pCur->m_Puzzles[0].m_Lock == 1)
	{
		std::cout << "You win! (type 'close' to exit the game.)" << std::endl;
	}
	else
	{
		std::cout << "You can't exit here." << std::endl;
	}
}

void Player::Health()
{
	std::cout << "Your health is: " << m_Health << std::endl;
	std::cout << "Your sanity is: " << m_Sanity << std::endl;
}

void Player::AttackedBy()
{
	if (m_pCur->m_Enemies.size() > 0)
	{
		if (m_pCur->m_Enemies[0].m_Health > 0)
		{
			if (m_pCur->m_Enemies[0].m_Undead)
			{
				std::cout << "The " << m_pCur->m_Enemies[0].m_Name << " attacks you, doing " << (m_pCur->m_Enemies[0].m_Damage - m_Negate)<< " damage!" << std::endl;
				m_Health -= m_pCur->m_Enemies[0].m_Damage - m_Negate;
			}
			else
			{
				std::cout << "The " << m_pCur->m_Enemies[0].m_Name << " attacks you, doing " << m_pCur->m_Enemies[0].m_Damage << " damage!" << std::endl;
				m_Health -= m_pCur->m_Enemies[0].m_Damage;
			}
		}
	}
}

void Player::Attack(Item& wep)
{
	if (m_pCur->m_Enemies.size() > 0)
	{
		if (m_pCur->m_Enemies[0].m_Health > 0)
		{
			if (m_pCur->m_Enemies[0].m_Undead)
			{
				if (wep.GetUndead())
				{
					std::cout << "You attack dealing " << wep.GetDamage() << " damage." << std::endl;
					m_pCur->m_Enemies[0].m_Health -= wep.GetDamage();
					if (m_pCur->m_Enemies[0].m_Health < 0)
					{
						m_pCur->m_Enemies[0].Die();
						m_pCur->m_RoomDesc.erase(m_pCur->m_RoomDesc.find(m_pCur->m_Enemies[0].GetDesc()), m_pCur->m_Enemies[0].GetDesc().length());
					}
					if (wep.m_AttackUses == 0)
					{
						for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
						{
							if (ToUpper((*iter).GetName()) == ToUpper(wep.GetName()))
							{
								std::cout << "Your " << wep.GetName() << " broke." << std::endl;
								m_Inv.erase(iter);
								break;
							}
						}
					}
					wep.m_AttackUses -= 1;
				}

				else
				{
					std::cout << "This weapon can't hurt undead." << std::endl;
				}
			}
			else
			{
				std::cout << "You attack dealing " << wep.GetDamage() << " damage." << std::endl;
				m_pCur->m_Enemies[0].m_Health -= wep.GetDamage();
				if (m_pCur->m_Enemies[0].m_Health < 0)
				{
					m_pCur->m_Enemies[0].Die();
					m_pCur->m_RoomDesc.erase(m_pCur->m_RoomDesc.find(m_pCur->m_Enemies[0].GetDesc()), m_pCur->m_Enemies[0].GetDesc().length());
				}
				if (wep.m_AttackUses == 0)
				{
					for (std::vector<Item>::iterator iter = m_Inv.begin(); iter != m_Inv.end(); ++iter)
					{
						if (ToUpper((*iter).GetName()) == ToUpper(wep.GetName()))
						{
							std::cout << "Your " << wep.GetName() << " broke." << std::endl;
							m_Inv.erase(iter);
							break;
						}
					}
				}
				wep.m_AttackUses -= 1;
			}
		}
		else
		{
			std::cout << "There is nothing to attack." << std::endl;
		}
	}
	else
	{
		std::cout << "There is nothing to attack." << std::endl;
	}
	AttackedBy();
}

void Player::Help() 
{
	std::cout << "\n\n'Look' - this command looks around the room." << std::endl;
	std::cout << "'Health' - Will show you your health and sanity stats. If either hit 0 you lose." << std::endl;
	std::cout << "'Inventory' - Will display what's in your inventory." << std::endl;
	std::cout << "'Check (object to check here)' - If you type 'Check box' it will check the box object in the room if it exists. Check shows what is on/in the object you want to check." << std::endl;
	std::cout << "'Attack (weapon to attack with)' - If you type 'Attack Sacrificial Dagger' you will attack any enemy in the room with the sacrificial dagger item." << std::endl;
	std::cout << "'Use (item to use)' - If you type 'Use bread' it will heal your health stat. The Use command is for using items like holy water, keys, gems, and etc." << std::endl;
	std::cout << "'Drop (item to drop)' - If you type 'Drop berries' you will drop berries on the floor in the room you are currently in, you can always pick items back up later with the Take command" << std::endl;
	std::cout << "'Take (item to take)' - If you type 'Take bread' you will take bread if it is in the room. If you want to take something from an object you checked, you have to use the take command right after using the check command on the object you want to take something from for example, 'check box' then you would type something like 'take bread'" << std::endl;
	std::cout << "'North' - Makes you go north." << std::endl;
	std::cout << "'East' - Makes you go east." << std::endl;
	std::cout << "'South' - Makes you go south." << std::endl;
	std::cout << "'West' - Makes you go west." << std::endl;
	std::cout << "'Down' - Makes you go down." << std::endl;
	std::cout << "'Up - Makes you go up." << std::endl;
	std::cout << "'Close'- This command closes the game" << std::endl;
	std::cout << "'Exit' - This command is used only at the game of the game when you find the exit to the house. (not to exit the game.)" << std::endl;
	std::cout << "'Help' - This command brings you here." << std::endl;
}
