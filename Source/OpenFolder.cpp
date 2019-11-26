#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <vector>

std::vector<std::string> ListFileInFolder(std::string path_folder)
{
	WIN32_FIND_DATA find_file_data;

	std::vector<std::string> list_file;

	HANDLE hFind = FindFirstFile(path_folder.c_str(), &find_file_data);
	std::string temp = find_file_data.cFileName;
	if (temp != "." && temp != "..")
		list_file.push_back(temp);
	while (FindNextFile(hFind, &find_file_data))
	{
		std::string temp = find_file_data.cFileName;
		if (temp != "."&& temp != "..")
			list_file.push_back(temp);
	}
	return list_file;
}

void main() {
	std::string str_path_folder("ABC\\*");

	std::vector<std::string> list_file = ListFileInFolder(str_path_folder);

	for (int i = 0; i < list_file.size(); i++) 
	{
		std::cout << list_file[i] << std::endl;
	}
}