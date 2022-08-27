#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Enemy
{
public:
	Enemy(std::string name, std::string desc, std::string dead, int health, int damage, int sanity, bool undead);
	void Die();
	std::string GetDesc();
	int m_Health;
	int m_Sanity;
	int m_Damage;
	std::string m_Name;
	bool m_Undead;
private:
	std::string m_Desc;
	std::string m_Dead;
};
#endif