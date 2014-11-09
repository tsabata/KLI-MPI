#include "ProblemSolver.h"
#include "Graph.h"
#include "Stack.h"
using namespace std;

void MaxClique::destroyArray(){
	if (this->arrayNodes != NULL)	
		delete [] arrayNodes;
}

void MaxClique::cpyArrayNodes(int * array, int size){
	this->size = size;
	this->arrayNodes = new int[size];
	for (int i = 0; i < size; i++) 
		this->arrayNodes[i] = array[i];
}

MaxClique::MaxClique(){
	this->size = 0;
	this->arrayNodes = NULL;
}

MaxClique::~MaxClique(){
	destroyArray();
}

int MaxClique::sizeArray(){
	return this->size;
}

bool MaxClique::isBiggerThan(int size){
	return (this->size > size);
}

bool MaxClique::isSmallerThan(int size){
	return (this->size < size);
}

void MaxClique::addArrayNodes(int * array, int size){
	destroyArray();
	cpyArrayNodes(array, size);
}

void MaxClique::printArrayNodes(){
	cout << "Nodes: ";
	for (int i = 0; i < this->size; i++)
		cout << this->arrayNodes[i] << " ";
}
// ------------------------------------------------------------------------------------------------

ProblemSolver::ProblemSolver(Graph* graph){
	this->graph = graph;
}

/* Je třeba vytvořit funkci pracujici na stejnem principu, která projde pouze dany podstrom stavového prostoru.
 * Podstrom je daný zasobníkem a předava se jako parametr.
 * Funkce se nesmí vracet ve stromu zpátky více než po počáteční uzel(pozna se dle parametru), místo toho si žádá o další práci. Pouze master proces obsahuje prázdný uzel 
 * Na zacatku se musí neblokujicim způsobem podívat zda někdo nežádá práci. (funkce bool checkWorkAdepts())
 */
void ProblemSolver::SolveProblem(){
	Stack * stack = new Stack();
	int lastNode = graph->size()-1;	
	stack->push(0);
	int lastDeleted=-1;

	while(true){
		if(stack->isEmpty()&&lastDeleted==lastNode)break;
		if(stack->getTop() < lastNode){
			int toPush=lastDeleted;			
			if(toPush==-1){
				toPush=stack->getTop()+1;
			}else{
				if(++toPush>lastNode){
					lastDeleted=stack->pull();
					continue;
				}
			}
			stack->push(toPush);
			lastDeleted=-1;
			if(isClique(stack) == false){
				lastDeleted=stack->pull();
			}
			continue;
		}
		if(stack->getTop() == lastNode){
			lastDeleted=stack->pull();
			continue;
		}
	}

	delete stack;
	this->printMaxClique();
	cout << endl;
}


//Kontrola na pocet uzlu a ochrana proti (vice uzlu nez prace)
void ProblemSolver::sendWorkAtStart(){
	cout<<"CPU"<<MPIHolder::getInstance().myRank<<": Sending work at start..."<<endl;
	int cpuCnt=MPIHolder::getInstance().cpuCounter;
	int destinationCPU=1;	
	stack = new Stack();
	int lastNode = graph->size()-1;
	int lastDeleted=-1;
	
	while(true){
		stack->push(lastDeleted+1);
		if(lastDeleted+1==lastNode){
			break;
		}
		if(destinationCPU<cpuCnt){
			int * array=stack->serialize();
			MPI_Send (array, MPIHolder::getInstance().stackMaxSize, MPI_INT, destinationCPU, FLAG_SEND_JOB, MPI_COMM_WORLD);
			lastDeleted=stack->pull();
			destinationCPU++;
		}else{
			break;		
		}
	}
	int * pomArray = new int[MPIHolder::getInstance().stackMaxSize];
	pomArray[0]=0;
	while(destinationCPU<cpuCnt){
		MPI_Send(pomArray,MPIHolder::getInstance().stackMaxSize,MPI_INT,destinationCPU,FLAG_SEND_JOB,MPI_COMM_WORLD);
		destinationCPU++;
	}
	delete [] pomArray;
}
void ProblemSolver::listenAtStart(){
	cout<<"CPU"<<MPIHolder::getInstance().myRank<<": Listening at start..."<<endl;
	int array[MPIHolder::getInstance().stackMaxSize];
	MPI_Status status;
	MPI_Recv(&array, MPIHolder::getInstance().stackMaxSize, MPI_INT, MPI_ANY_SOURCE, FLAG_SEND_JOB, MPI_COMM_WORLD, &status);
	stack = new Stack(array);
}

void ProblemSolver::startComputing(){
	if(!stack->isEmpty()){
		stack->printStack();
	}else{
		cout<<"I have empty stack"<<endl;
	}	
	/*
	TODO začít symetricky výpočet
	*/
}

void ProblemSolver::aduv(){
	cout<<"aduv started"<<endl;
	/*
	TODO aduv algorithm
	*/
}


Stack * ProblemSolver::getNewWork(){
	// žádá procesy o novou práci. Jako návratová hodnota je zásobník, který je možné rovnou použít pro řešení
}


bool ProblemSolver::checkWorkAdepts(){
	// funkce zkontroluje adepty na praci. Pokud obsahuje zadost o praci. Prideli mu tu co by mel ted delat a vrati true. Jinak vraci false.
	// Navratova hodnota je dulezita pro pripadne vraceni ve stromu a pokracovani v praci.
}

bool ProblemSolver::isClique(Stack * stack){
	int arrSize = stack->getSize();
	int *values = stack->getArray();
	
	for(int i = 0; i < arrSize; i++){
		for(int j = 0; j < arrSize; j++){
			if(i == j) continue;
			if(graph->isConnected(values[i], values[j]) == false){
				delete []values;
				return false;
			}
		}
	}
	if (maxClique.isSmallerThan(arrSize)){
		maxClique.addArrayNodes(values, arrSize);
	}

	delete [] values;
	return true;
}

void ProblemSolver::printMaxClique(){
	cout << "Max clique size is/are: " << this->maxClique.sizeArray() << endl;
	this->maxClique.printArrayNodes();
}

