#pragma once

#include <string>
#include <vector>


struct Config
{
	std::string name; // name of configuration variable
	std::string data; // data of configuration variable
	/*
	For example:
	rend OpenGL
	name: rend
	data: OpenGL
	*/
};

class ConfParser
{
public:
	ConfParser();
	~ConfParser();

	void parse(std::string path);
	std::string getRednerer() { return renderer; }

private:
	std::vector<std::string> confStrs;
	std::vector<Config> configData;
	std::string renderer;
};