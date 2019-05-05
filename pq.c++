#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>
#include <utility>
#include <functional>

using namespace std;

class node{
public:
	char name;
	int pred;
	int dist;
	int pqIndex;
	int degree;
};

class adjNode{
public:
	int nodeIndex;
	int linkCost;
};

class Heap{
public:
	Heap(int c);
	// ~Heap();

	bool empty() const {return hSize == 0;}
 //    int size() const { return hSize;}
 //    int capacity() const { return hCapacity ;}
 //    int peekMin() const { return A[0].element ;}
 //    int peekMinPriority() const { return A[0].priority;}
    void insert(vector<node> &vec, int index);
    void printpq();
    int extractMin(vector<node> &vec);
    void decreaseKey(vector<node> &vec, int index);

    class Pair{
    public:
        int element ;
        int priority ;
    };

    Pair* A;
    int hCapacity, hSize;

};

Heap::Heap(int c){
	hCapacity = c ;
    A = new Pair[hCapacity];
    hSize = 0 ;
}

void Heap::insert(vector<node> &vec, int index){
	A[hSize].element = index;
    A[hSize].priority = vec[index].dist;
    vec[index].pqIndex = hSize;
    int child = hSize;
    int parent;
    while (child > 0){
    	parent = child%2==0? (child-2)/2 : (child-1)/2;
    	if (A[parent].priority > A[child].priority){	// is parent >= 0 necessary???
    		Pair temp = A[child];
			A[child] = A[parent];
			A[parent] = temp;
			vec[A[parent].element].pqIndex = parent;
			vec[A[child].element].pqIndex = child;
			child = parent;
    	}
    	else {
    		break;
    	}
    }

    hSize ++;
}

void Heap::printpq(){
	for (int i=0; i<hSize; i++){
		cout << A[i].element << " " << A[i].priority << endl;
	}
	cout << endl;
}

int Heap::extractMin(vector<node> &vec){
	int i = A[0].element;
	hSize--;
	A[0] = A[hSize];
	if (hSize>0){vec[A[0].element].pqIndex = 0;}
	int parent = 0, child;
	int lchild = 1;
	int rchild = 2;
	int min, left;
	 while (lchild<hSize || rchild<hSize){	// at least one child exist
	 	min = INT_MAX;
	 	left = 1;
	 	if (lchild<hSize){
	 		min = A[lchild].priority;
	 	}
	 	if (rchild<hSize && A[rchild].priority < min){
	 		min = A[rchild].priority;
	 		left = 0;
	 	}
    	child = left==1? lchild : rchild;
    	if (A[parent].priority > min){
    		Pair temp = A[child];
			A[child] = A[parent];
			A[parent] = temp;
			vec[A[parent].element].pqIndex = parent;
			vec[A[child].element].pqIndex = child;
			parent = child;
    	}
    	else {
    		break;
    	}
    	lchild = parent*2+1;
    	rchild = parent*2+2;
    }
    return i;
}

void Heap::decreaseKey(vector<node> &vec, int index){
	int child = vec[index].pqIndex;
	A[child].priority = vec[index].dist;
    int parent;
    while (child > 0){
    	parent = child%2==0? (child-2)/2 : (child-1)/2;
    	if (A[parent].priority > A[child].priority){	// is parent >= 0 necessary???
    		Pair temp = A[child];
			A[child] = A[parent];
			A[parent] = temp;
			vec[A[parent].element].pqIndex = parent;
			vec[A[child].element].pqIndex = child;
			child = parent;
    	}
    	else {
    		break;
    	}
    }
}

int find(vector<node> &vec, int len, char c){
	int i=0;
	for (i; i<len; i++){
		if (vec[i].name == c){break;}
	}
	return i;
}

void pathStr(string &str, vector<node> &vec, int index){
	string s;
	while (vec[index].pred != index){
	str.insert(0, 1, vec[index].name);
	str.insert(0, "-");
	index = vec[index].pred;
	}
	str.insert(0, 1, vec[index].name);
}

string inputFileName, outputFileName;

int main(){
// read input

int numOfNode, numOfLink;

cout << "Please enter the name of the input file: ";
getline(cin, inputFileName);

cout << "Please enter the name of the output file: ";
getline(cin, outputFileName);

ifstream inputFile;

inputFile.open(inputFileName.c_str());
if (!inputFile){cout << "Error reading input file!\n"; exit(1);}

inputFile >> numOfNode;
inputFile >> numOfLink;

vector<node> nodeArr;	// array of N nodes

for (int i=0; i<numOfNode; i++){	// read nodes into nodeArr
	node a;
	inputFile >> a.name;
	a.dist = INT_MAX;
	a.degree = 0;
	nodeArr.push_back(a);
}
nodeArr[0].dist = 0;
nodeArr[0].pred = 0;

vector<vector<adjNode>> Adjlist(numOfNode, vector<adjNode>(numOfNode));	// adjacency list

char x, y;
int cost, index1, index2, a, b;
for (int i=0; i<numOfLink; i++){
	inputFile >> x;
	inputFile >> y;
	inputFile >> cost;
	index1 = find(nodeArr, numOfNode, x);
	index2 = find(nodeArr, numOfNode, y);
	a = nodeArr[index1].degree++;
	b = nodeArr[index2].degree++;
	Adjlist[index1][a].nodeIndex = index2;
	Adjlist[index1][a].linkCost = cost;
	Adjlist[index2][b].nodeIndex = index1;
	Adjlist[index2][b].linkCost = cost;
	
}

inputFile.close();

// for (int i=0; i<numOfNode; i++){			// print Adjlist
// 	for (int j=0; j<nodeArr[i].degree; j++){
// 		cout << Adjlist[i][j].nodeIndex << " " << Adjlist[i][j].linkCost << "|";
// 	}
// 	cout << endl;
// }
// return 0;

// finish reading input

// Link-State Routing Algorithm
Heap pq = Heap(numOfNode);

for (int i=0; i<numOfNode; i++){
	pq.insert(nodeArr, i);
}

// pq.printpq();

int neighbor, newCost;
while (!pq.empty()){
	int index = pq.extractMin(nodeArr);
	nodeArr[index].pqIndex = -1;	// no longer in pq
	for (int i=0; i<nodeArr[index].degree; i++){	// for each neighbor

			neighbor = Adjlist[index][i].nodeIndex;
			if (nodeArr[neighbor].pqIndex != -1){	// if still in pq
				newCost = Adjlist[index][i].linkCost + nodeArr[index].dist;
				if (nodeArr[neighbor].dist > newCost){
					nodeArr[neighbor].dist = newCost;
					nodeArr[neighbor].pred = index;
					pq.decreaseKey(nodeArr, neighbor);
				}
			}
	}
	// pq.printpq();
}
// Link-State Routing Algorithm

ofstream outputFile;
outputFile.open(outputFileName.c_str());

for (int i=1; i<numOfNode; i++){
	string path = "";
	pathStr(path, nodeArr, i);
	outputFile << nodeArr[i].name << ": ";
	outputFile << path << " " << nodeArr[i].dist << endl;
}

// Distance Vector Routing Algorithm
int count = 1, needUpdate = 1;
vector<vector<int>> dvTable(numOfNode, vector<int>(numOfNode, INT_MAX)); // dv table

for (int i=0; i<numOfNode; i++){
	dvTable[i][i] = 0;
	for (int j=0; j<nodeArr[i].degree; j++){
		neighbor = Adjlist[i][j].nodeIndex;
		cost = Adjlist[i][j].linkCost;
		dvTable[i][neighbor] = cost;
		dvTable[neighbor][i] = cost;
	}
}

while (needUpdate){
	needUpdate = 0;
	for (int i=0; i<numOfNode; i++){	// for each row in DV table
		for (int j=0; j<numOfNode; j++){	// for each column in DV table
			if (i!=j){
				for (int k=0; k<nodeArr[i].degree; k++){	// for each elt in corresponding row in adjancency list
					neighbor = Adjlist[i][k].nodeIndex;
					if (dvTable[neighbor][j] != INT_MAX){
						cost = Adjlist[i][k].linkCost + dvTable[neighbor][j];
						if (dvTable[i][j] > cost){
						dvTable[i][j] = cost;
						needUpdate = 1;
						}
					}
				}
			}
		}
	}
}

// Distance Vector Routing Algorithm



for (int i=0; i<numOfNode; i++){
	for (int j=0; j<numOfNode; j++){
		outputFile << dvTable[i][j] << " ";
	}
	outputFile << endl;
}

outputFile.close();

return 0;
}

