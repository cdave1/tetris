#pragma once

class cList
{
public:
	int m_i;
	cList* m_next;
	cList* m_prev;

	cList (int i);

	void SetNext(cList* next);

	void SetPrev(cList* prev);

	cList* GetNext();

	cList* GetPrev();
private:
};