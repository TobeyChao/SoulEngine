#include "StringUtils.h"
#include <locale.h>

std::wstring Soul::StringToWstring(const std::string str)
{
	size_t i;
	size_t len = str.size() * 2;// Ԥ���ֽ���
	setlocale(LC_CTYPE, "");     //������ô˺���
	wchar_t* p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
	mbstowcs_s(&i, p, len, str.c_str(), len);// ת��
	std::wstring str1(p);
	delete[] p;// �ͷ�������ڴ�
	return str1;
}

std::string Soul::WstringToString(const std::wstring str)
{
	size_t i;
	size_t len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char* p = new char[len];
	wcstombs_s(&i, p, len, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}