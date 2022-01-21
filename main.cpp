#define __CREATOR_MODE 1
#ifdef __CREATOR_MODE

#include <iostream>
#include <fstream>
#include "stringext.h"
#include "filepath.h"

#define StdErr(msg)                    \
	do                                 \
	{                                  \
		std::cerr << msg << std::endl; \
	} while (0)

#define StdOut(msg)                    \
	do                                 \
	{                                  \
		std::cout << msg << std::endl; \
	} while (0)

std::string HeaderFileTemplate = R"(#define FILE_NAME_{fileAlias} "{fileName}"
#define FILE_SIZE_{fileAlias} {fileSize}
void Release_{fileBaseName}();)";
std::string SourceFileTemplate = R"(#include "{fileBaseName}.h"
#include <fstream>
char BIN_{fileAlias}[] = {{binContent}};
void Release_{fileBaseName}()
{
	std::ofstream ofs(FILE_NAME_{fileAlias},std::ios_base::trunc|std::ios_base::binary);
	if(!ofs.is_open()) return;
	ofs.write(BIN_{fileAlias},FILE_SIZE_{fileAlias});
	ofs.close();
})";

class Creator
{
public:
	Creator(const char *filePath)
	{
		this->filePath = filePath;
		ifs.open(filePath, std::ios_base::binary);
		if (!ifs.is_open())
		{
			this->fileSize = 0;
			return;
		}
		this->fileSize = Creator::GetFileSize(ifs);
		this->fileName = filepath::LastElement(filePath);
		this->fileBaseName = filepath::BaseName(this->fileName);
		this->fileAlias = fileName;
		stringext::ToUpper(this->fileAlias);
		stringext::ReplaceAll(this->fileAlias, ".", "_");
	}

	~Creator()
	{
		if (ifs.is_open())
			ifs.close();
	}

	static size_t GetFileSize(std::ifstream &ifs)
	{
		size_t fileSize = ifs.seekg(std::ios_base::beg, std::ios_base::end).tellg();
		ifs.seekg(std::ios_base::beg);
		return fileSize;
	}

	bool CreateHeaderFile()
	{
		if (!this->ifs.is_open())
		{
			StdErr("file not open");
			return false;
		}
		std::ofstream ofs(fileBaseName + ".h", std::ios_base::binary | std::ios_base::trunc);
		if (!ofs.is_open())
		{
			StdErr("create header file failed");
			return false;
		}
		stringext::ReplaceAll(HeaderFileTemplate, "{fileAlias}", fileAlias);
		stringext::ReplaceAll(HeaderFileTemplate, "{fileName}", fileName);
		stringext::ReplaceAll(HeaderFileTemplate, "{fileSize}", std::to_string(fileSize));
		stringext::ReplaceAll(HeaderFileTemplate, "{fileBaseName}", fileBaseName);
		ofs << HeaderFileTemplate;
		ofs.close();
		return true;
	}

	bool CreateSourceFile()
	{
		if (!this->ifs.is_open())
		{
			StdErr("file not open");
			return false;
		}
		std::ofstream ofs(fileBaseName + ".cpp", std::ios_base::binary | std::ios_base::trunc);
		if (!ofs.is_open())
		{
			StdErr("create source failed");
			return false;
		}
		char *buffer = (char *)malloc(fileSize);
		ifs.read(buffer, fileSize);
		std::string binContent;
		int count = 0;
		const int sepNum = 20;
		for (size_t i = 0; i < fileSize; i++)
		{
			binContent += std::to_string(int(buffer[i])) + ",";
			if (((++count) % sepNum) == 0)
				binContent += "\n";
		}
		free(buffer);
		stringext::ReplaceAll(SourceFileTemplate, "{fileAlias}", fileAlias);
		stringext::ReplaceAll(SourceFileTemplate, "{fileName}", fileName);
		stringext::ReplaceAll(SourceFileTemplate, "{fileSize}", std::to_string(fileSize));
		stringext::ReplaceAll(SourceFileTemplate, "{fileBaseName}", fileBaseName);
		stringext::ReplaceAll(SourceFileTemplate, "{fileBaseName}", fileBaseName);
		stringext::ReplaceAll(SourceFileTemplate, "{binContent}", binContent);
		ofs.write(SourceFileTemplate.c_str(), SourceFileTemplate.size());
		ofs.close();
		return true;
	}

protected:
	std::ifstream ifs;
	std::string filePath;
	size_t fileSize;
	std::string fileName;
	std::string fileBaseName;
	std::string fileAlias;
};

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		StdErr("need filepath");
		return 1;
	}
	Creator creator(argv[1]);
	if (!creator.CreateHeaderFile())
	{
		StdErr("Create header file failed");
		return 1;
	}
	if (!creator.CreateSourceFile())
	{
		StdErr("Create Source file failed");
		return 1;
	}
	StdOut("create files successfully");
	return 0;
}

#else

#include "7z.h"

int main(int argc, char **argv)
{
	Release_7z();
}

#endif
