#include "Folder.h"

void ListFileInFolder(std::vector<std::string>& list_file, std::string path_folder, std::string path_header)
{	//TODO: hàm trả về danh sách đường dẫn đến các file con trong folder
	WIN32_FIND_DATAA find_file_data;

	HANDLE hFind = FindFirstFileA(path_folder.c_str(), &find_file_data);
	std::string temp = find_file_data.cFileName;
	if (temp != "." && temp != "..")
	{	// bỏ qua kí tự '.' và '..'
		temp = '0' + path_header + temp;
		list_file.push_back(temp);
	}
	while (FindNextFileA(hFind, &find_file_data))
	{
		std::string temp = find_file_data.cFileName;
		if (temp != "." && temp != "..")
		{
			if (find_file_data.dwFileAttributes == 16)
			{	// nếu là một folder thì đệ quy vào để tiếp tục tìm những file con
				int before = list_file.size();
				std::string path = path_folder.substr(0, path_folder.length() - 1) + find_file_data.cFileName + "\\*";
				ListFileInFolder(list_file, path, path_header + find_file_data.cFileName + '\\');
				int after = list_file.size();
				if (before == after)
				{	// nếu là một folder rỗng thì ghi ra dường dẫn đến folder đó
					temp = '1' + path_header + temp;
					list_file.push_back(temp);
				}
			}
			else
			{	// đánh dấu kí tự '0' đầu đường dẫn để chỉ đó là file con
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
	ofstream output(outputfile, ios::binary);	// số file con chứa trong folder
	output << filename.size() << " ";
	for (int i = 0; i < filename.size(); i++)
	{
		if (filename[i][0] == '0')
		{	// nếu đường dẫn dẫn đến một file thì nén file đó
			Huffman temp(filename[i], "");
			temp.encodeAFileinFolder(output);
		}
		else if (filename[i][0] == '1')
		{	// nếu đường dẫn dẫn đến một folder rỗng thì ghi ra tên folder đó
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