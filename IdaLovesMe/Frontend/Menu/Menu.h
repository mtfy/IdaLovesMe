#pragma once
#include "../../Backend/Globalincludes.h"
#include "../../Backend/Misc/singleton.h"

#include <unordered_map>

class CMenu : public singleton<CMenu> 
{
public:
	void Initialize();
	void Draw();

	bool IsMenuOpened();
	void SetMenuOpened(bool v);
	
	int Menu_key = VK_INSERT;
private:
	bool m_bInitialized;
	bool m_bIsOpened;
	
	int m_nCurrentTab;
	int m_nCurrentLegitTab;
};

class c_config : public singleton<c_config> {

public:
	std::unordered_map<std::string, bool> b;
	std::unordered_map<std::string, int> i;
	std::unordered_map<std::string, float> f;
	std::unordered_map<std::string, float[4]> c;
	std::unordered_map<std::string, std::unordered_map<int, bool>> m;
};