#include "stdafx.h"

cList::cList (int i) 
{
	m_i = i;
	m_next = NULL;
	m_prev = NULL;
}

void cList::SetNext(cList* next) 
{
	m_next = next;
}

void cList::SetPrev(cList* prev)
{
	m_prev = prev;
}

cList* cList::GetNext() 
{
	return m_next;
}

cList* cList::GetPrev() 
{
	return m_prev;
}
