#include<windows.h>
#include <iostream>
#include <map>

class File 
{
private:
	std::string way;
	std::string WayToPic;
public:
	std::string name;
	File()
	{
	}
	File(std::string name, std::string way)
	{
		this->name = name;
		this->way = way;
	}
	void setName(std::string name) { this->name = name; }
	void setWay(std::string way)   {   this->way = way; }
	std::string GetName() { return name; }
	std::string GetWay()  { return  way; }
	std::string GetWayToPic(int CharDelet = 1) 
	{ 
		WayToPic = way;
		WayToPic.erase(WayToPic.end() - CharDelet, WayToPic.end());
		return  WayToPic; 
	}
};



void Ex(std::vector<File> &Fil, std::vector<std::string> &Str)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	char workdir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, workdir);

	File f;

	std::string s = workdir;
	s = s + "\\1.Data\\Animation\\*";
	const char *fds[]{ s.c_str() };

	std::string wa ="1.Data/Animation/";

	hf = FindFirstFile(s.c_str(), &FindFileData);

	setlocale(LC_ALL, "Russian");

	if (hf != INVALID_HANDLE_VALUE) {
		do 
		{
			f.setName(FindFileData.cFileName);
			Str.push_back(FindFileData.cFileName);
			f.setWay(wa + FindFileData.cFileName);
			Fil.push_back(f);
			std::cout << FindFileData.cFileName << std::endl;
		} 
		while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

}
