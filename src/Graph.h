#ifndef Graph_H
#define Graph_H
#include <cstdio>
#include <iostream>
#include <fstream>
#include "Node.h"

class Graph{
	unsigned long nodesCnt;
	int ** nodes;
public:
	Graph();
	Graph(unsigned long nodesCnt);
	~Graph();
	void parseFile(const char * filename);
	void print();
};

#endif
