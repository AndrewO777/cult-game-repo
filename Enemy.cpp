#include "Enemy.h"
#include <iostream>

Enemy::Enemy(std::string name, std::string desc, std::string dead, int health, int damage, int sanity, bool undead) :
	m_Name(name), m_Desc(desc), m_Dead(dead), m_Health(health), m_Damage(damage), m_Sanity(sanity), m_Undead(undead)
{}

void Enemy::Die()
{
	std::cout << m_Dead << std::endl;
}

std::string Enemy::GetDesc()
{
	return m_Desc;
}