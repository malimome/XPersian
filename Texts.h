#ifndef _TEXTS_H__
#define _TEXTS_H__

#include "loadf.h"

class CTranslator
{
private:
	size_t text_size;
	//int cpid;

public:
	str_list  EngTexts;
	wstr_list PerTexts;
	wstr_list  EngCaption;
	wstr_list PerCaption;

private:
	void Init_Caption();
	int mapnum2id(int arrnum);

public:
	CTranslator();
    void text_from_arrnum(int i);
	void unload_list();
	//int get_cap_size(){return cap_size;}
	~CTranslator();
};

#endif