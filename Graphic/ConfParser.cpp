#include <fstream>
#include <Windows.h>

#include "ConfParser.hpp"


ConfParser::ConfParser() {}
ConfParser::~ConfParser() {}

void ConfParser::parse(std::string path)
{
	std::ifstream file(path);
	if (!file)
	{
		MessageBox(NULL, "can't open config file", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	std::string confStr = "";

	
	while (!file.eof())
	{
		std::getline(file, confStr);
	}

	std::string confSubstr;
	for (int i = 0; i < confStr.size(); i++)
	{
		if (confStr[i] != ';')
		{
			confSubstr += confStr[i];
		}
		else
		{
			confStrs.push_back(confSubstr);
			confSubstr.clear();
		}
	}

	for (int i = 0; i < confStrs.size(); i++)
	{
		Config config;
		bool nameCreated = false;
		for (int j = 0; j < confStrs[i].size(); j++)
		{
			if (confStrs[i][j] != ' ')
			{
				confSubstr += confStrs[i][j];
			}
			else
			{
				config.name = confSubstr;
				confSubstr.clear();
				nameCreated = true;
			}
			if (nameCreated && j == confStrs[i].size() - 1)
			{
				config.data = confSubstr;
				confSubstr.clear();
			}
		}
		configData.push_back(config);
	}

	for (int i = 0; i < configData.size(); i++)
	{
		if (configData[i].name == "rend")
			renderer = configData[i].data;
	}
}