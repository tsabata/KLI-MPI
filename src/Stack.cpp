#include "Stack.h"
#include "MPIHolder.h"

#include <cstring>
#include <iostream>

Stack::Stack(){
	array = new int[MPIHolder::getInstance().stackMaxSize];
	memset(array,-1,MPIHolder::getInstance().stackMaxSize);
	array[0]=0;
}

Stack::Stack(int * pomArray){
	array=new int[MPIHolder::getInstance().stackMaxSize];
	array[0]=pomArray[0];
	for(int i=1;i<=pomArray[0];i++){
		array[i]=pomArray[i];
	}
}

Stack::~Stack(){
	delete [] array;
}

int * Stack::serialize(){
	return array;
}

void Stack::push(int value){
	int position=array[0]+1;
	array[position]=value;
	array[0]=position;
}

int Stack::pull(){
	if (array[0] == 0){
		std::cerr << "pull(): Stack is empty\n";
		return -1;
	}
	int position = array[0];
	int value = array[position];
	array[position] = -1;
	array[0] = array[0] -1;
	return value;
}

int Stack::getTop(){
	if(array[0] == 0){
		std::cerr << "getTop(): Stack is empty\n";
		return -1;	
	}
	return array[array[0]];
}

bool Stack::isEmpty(){
	return (array[0] == 0);
}

void Stack::printStack(){
	int size = array[0];
	std::cout<<"Stack: ";
	for(int i=1;i<=size;i++){	
		std::cout<<array[i]<<" ";
	}
	std::cout<<std::endl;	
}

int Stack::getSize(){
	return array[0];
}

int * Stack::getArray(){
	int * arr = new int[array[0]];
	for(int i=0;i<array[0];i++){
		arr[i]=array[i+1];
	}
	return arr;
}

