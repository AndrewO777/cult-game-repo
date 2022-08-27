#include "Room.h"

Room::Room(std::string name):
	m_RoomName(name)
{}

Room* Room::GetDirection(std::string dir)
{
	if (dir == "NORTH" || dir == "N")
	{
		return m_pNorth;
	}
	else if (dir == "EAST" || dir == "E")
	{
		return m_pEast;
	}
	else if (dir == "SOUTH" || dir == "S")
	{
		return m_pSouth;
	}
	else if (dir == "WEST" || dir == "W")
	{
		return m_pWest;
	}
	return 0;
}

void Room::SetDirection(std::string dir, Room* pNewRoom)
{
	if (dir == "NORTH" || dir == "N")
	{
		m_pNorth = pNewRoom;
	}
	else if (dir == "EAST" || dir == "E")
	{
		m_pEast = pNewRoom;
	}
	else if (dir == "SOUTH" || dir == "S")
	{
		m_pSouth = pNewRoom;
	}
	else if (dir == "WEST" || dir == "W")
	{
		m_pWest = pNewRoom;
	}
}

std::string Room::GetName() const
{
	return m_RoomName;
}

std::string Room::GetDesc()
{
	return m_RoomDesc;
}
