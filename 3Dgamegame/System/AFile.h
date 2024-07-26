#pragma once
#include<vector>


class AFile
{
private:
	int m_handle;
	std::vector<uint8_t> m_data;
	int m_cursor;
	int m_cursor;
public:
	AFile(const wchar_t* path);
	bool IsReadble()const;
	bool Read(void* address, int size);
	void Close();

};

