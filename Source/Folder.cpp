#include "Folder.h"

void ListFileInFolder(std::vector<std::string>& list_file, std::string path_folder, std::string path_header)
{
	WIN32_FIND_DATA find_file_data;

	HANDLE hFind = FindFirstFile(path_folder.c_str(), &find_file_data);
	std::string temp = find_file_data.cFileName;
	if (temp != "." && temp != "..")
	{
		temp = '0' + path_header + temp;
		list_file.push_back(temp);
	}
	while (FindNextFile(hFind, &find_file_data))
	{
		std::string temp = find_file_data.cFileName;
		if (temp != "." && temp != "..")
		{
			if (find_file_data.dwFileAttributes == 16)
			{
				int before = list_file.size();
				std::string path = path_folder.substr(0, path_folder.length() - 1) + find_file_data.cFileName + "\\*";
				ListFileInFolder(list_file, path, path_header + find_file_data.cFileName + '\\');
				int after = list_file.size();
				if (before == after)
				{
					temp = '1' + path_header + temp;
					list_file.push_back(temp);
				}
			}
			else
			{
				temp = '0' + path_header + temp;
				list_file.push_back(temp);
			}
		}
	}
}

void Folder::open()
{
	ListFileInFolder(filename, name, name.substr(0, name.length()-1));
}

void Folder::encode(string outputfile)
{
	ofstream output(outputfile, ios::binary);
	output << filename.size() << " ";
	for (int i = 0; i < filename.size(); i++)
	{
		if (filename[i][0] == '0')
		{
			Huffman temp(filename[i], "");
			temp.encodeAFileinFolder(output);
		}
		else if (filename[i][0] == '1')
		{
			output << filename[i].size() << " " << filename[i] << " ";
		}
	}
}

void Folder::decode(string inputfile, string outfolder)
{
	Huffman infile(inputfile, "");
	infile.decodeFolder(outfolder);
}

Folder::Folder()
{
	name = "";
}

Folder::Folder(string input)
{
	name = input + "\\*";
}