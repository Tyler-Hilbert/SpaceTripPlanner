/* Homework #4 */

/* ----- Include section ----- */
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>


/* ----- Edge Struct ----- */
struct Edge
{
	int source, dest;
	Edge* next = nullptr;
	Edge(int source, int dest) : source(source), dest(dest) {}
};

/* ----- Node Struct ----- */
struct Node {
	std::string task;
	Edge* edgeList = nullptr;
	Node(std::string task) : task(task) {}
	Node() {}
};

/* ----- Digraph Class ----- */
class Digraph
{
public:

	~Digraph() {
		Clear();
	}

	Digraph()
		: _nodeArraySize(1), _numNodes(0), _numEdges(0), _adjList(new Node[1]) {}

	void AddEdge(int source, int dest);
	void AddNode(std::string task);
	void PrintNodes();
	void PrintNodeEdges(int nodeNum);
	void PrintGraph();
	void DeleteEdge(int source, int dest);
	int GetNumNodes();
	void BFS();

private:
	void DeleteList(Edge* head);
	void Clear();
	void ReallocateNodes();

	Node* _adjList;
	int _numNodes;
	int _numEdges;
	int _nodeArraySize;
};



int Digraph::GetNumNodes() {
	return _numNodes;
}

void Digraph::DeleteEdge(int source, int dest) {

	// Get node
	Edge* list = _adjList[source].edgeList;
	Edge* temp;

	// List is empty
	if (list == nullptr) {
		return;
	}

	// Head of list is item we need
	if (list->dest == dest) {
		temp = list->next;
		delete list;
		_adjList[source].edgeList = temp;
		_numEdges--;
		return;
	}

	// Find and remove edge from node from rest of list
	while (list->next != nullptr) {
		if (list->next->dest = dest) {
			temp = list->next->next;
			delete list->next;
			list->next = temp;
			_numEdges--;
			return;
		}
	}
}

void Digraph::DeleteList(Edge* head) {
	if (head == nullptr) {
		return;
	}

	DeleteList(head->next);
	delete head;
}

void Digraph::Clear() {
	// Delete all edges
	for (int i = 0; i < _numNodes; ++i) {
		DeleteList(_adjList[i].edgeList);
	}

	// Delete all nodes
	delete[] _adjList;
}

void Digraph::PrintNodeEdges(int nodeNum) {
	// TODO: CHECK IF EXISTS HERE

	Edge* list = _adjList[nodeNum].edgeList;

	std::cout << nodeNum + 1;

	if (list == nullptr) {
		std::cout << " No Edges.";
	}

	while (list != nullptr) {
		std::cout << " -> " << list->dest + 1;
		list = list->next;
	}

	std::cout << std::endl;
}

void Digraph::PrintNodes() {
	std::cout << "TASKS:" << std::endl;
	for (int i = 0; i < _numNodes; ++i) {
		std::cout << i + 1 << ". " << _adjList[i].task << std::endl;
	}
}

void Digraph::PrintGraph() {
	PrintNodes();           // Prints the tasks next to the indexes
	std::cout << "\nEDGES:\n";
	for (int i = 0; i < _numNodes; i++) {
		PrintNodeEdges(i);  // Prints the edges witht eh node as a source
	}
}

void Digraph::ReallocateNodes() {
	// Calculate new list size
	int newArraySize = _nodeArraySize * 2;
	if (newArraySize == 0) newArraySize = 1;

	// Allocate memory for new list
	Node* newList = new Node[newArraySize];

	// Copy old list to new location
	for (int i = 0; i < _numNodes; ++i) {
		newList[i] = _adjList[i];
	}

	// Replace old list
	delete[] _adjList;
	_adjList = newList;
	_nodeArraySize = newArraySize;
}

void Digraph::AddEdge(int source, int dest) {
	// Check if node is in list
	if (source >= _numNodes || dest >= _numNodes) {
		std::cout << "Node not in list" << std::endl;
		return;
	}

	_numEdges++;

	// Add edge to node
	Edge* list = _adjList[source].edgeList;

	// if first node is null
	if (_adjList[source].edgeList == nullptr) {
		_adjList[source].edgeList = new Edge(source, dest);
		return;
	}

	while (list->next != nullptr) {
		list = list->next;
	}

	list->next = new Edge(source, dest);
}

void Digraph::AddNode(std::string task) {
	// Reallocate list if too small for new task
	if (_numNodes + 1 > _nodeArraySize) {
		ReallocateNodes();
	}

	// Add node to graph
	_adjList[(++_numNodes) - 1] = Node(task);
}


void Digraph::BFS() {
	std::cout << "Starting BFS\n" << std::endl;

	bool* visited = new bool[_numNodes];
	for (int i = 0; i < _numNodes; ++i) {
		visited[i] = false;
	}

	std::string* path = new std::string[_numNodes];
	path[0] = "";
	std::queue<Edge> frontier;

	auto start = std::chrono::system_clock::now();
	frontier.emplace(25,25);
	int currentNode;
	int sourceNode;
	Edge* list;

	while (!frontier.empty()) {
		currentNode = frontier.front().dest;
		sourceNode = frontier.front().source;
		frontier.pop();

		if (visited[currentNode]) continue;

		path[currentNode] = path[sourceNode] + ", " + std::to_string(currentNode);
		std::cout << currentNode << ":\t" << path[currentNode] << std::endl;

		visited[currentNode] = true;
		Node* thisNode = &(_adjList[currentNode]);
		list = thisNode->edgeList;

		while (list != nullptr) {
			if (!visited[list->dest]) {
				frontier.emplace(currentNode, list->dest);
			}
			list = list->next;
		}

	}


	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "Elapsed time: " << elapsed.count() << "ms\n";
	
	delete[] visited;
	delete[] path;
	std::cout << "\nEnding BFS" << std::endl;
}



int main()
{
	Digraph graph;
	
	// THIS IS A GENERATED FILE USING EdgeCreator.py
	// Number of lightyears a spaceship can travel: 1
	graph.AddNode("Andromeda galaxy = M31 = NGC 0224");
	graph.AddNode("Andromeda I");
	graph.AddNode("Andromeda II");
	graph.AddNode("Andromeda III");
	graph.AddNode("Aquarius Dwarf = DDO 210");
	graph.AddNode("Barnards Galaxy = NGC 6822");
	graph.AddNode("Bodes Galaxies = M81/82 = NGC 3031/34");
	graph.AddNode("Carina dwarf");
	graph.AddNode("Centaurus A = NGC 5128 = Arp 135");
	graph.AddNode("Circinus galaxy");
	graph.AddNode("Draco dwarf = DDO 208");
	graph.AddNode("Fornax dwarf");
	graph.AddNode("Grus Quartet = NGC 7552/82/90/99");
	graph.AddNode("GR8 = DDO 155");
	graph.AddNode("Hercules A");
	graph.AddNode("Holmberg II = DDO 050 = Arp 268");
	graph.AddNode("Horologium Dwarf = Schusters Spiral");
	graph.AddNode("Large Magellanic Cloud");
	graph.AddNode("Leo I = Harrington-Wilson No. 1");
	graph.AddNode("Leo II = Harrington-Wilson No.2");
	graph.AddNode("Leo A = Leo III = DDO 069");
	graph.AddNode("Maffei I = UGCA 034");
	graph.AddNode("Maffei II = UGCA 039");
	graph.AddNode("Pegasus Dwarf = DDO 216");
	graph.AddNode("Phoenix Dwarf Irregular");
	graph.AddNode("Pisces Dwarf = LGS 3");
	graph.AddNode("Sculptor Dwarf");
	graph.AddNode("Sculptor Dwarf Irregular");
	graph.AddNode("Sextans A = DDO 075");
	graph.AddNode("Sextans B = DDO 070");
	graph.AddNode("Small Magellanic Cloud");
	graph.AddNode("Triangulum galaxy = M33 = NGC 0598");
	graph.AddNode("Ursa Minor dwarf = DDO 199");
	graph.AddNode("Zwicky No.2 = DDO 105");
	graph.AddEdge(00, 01);
	graph.AddEdge(00, 02);
	graph.AddEdge(00, 03);
	graph.AddEdge(00, 04);
	graph.AddEdge(00, 23);
	graph.AddEdge(00, 24);
	graph.AddEdge(00, 25);
	graph.AddEdge(00, 27);
	graph.AddEdge(00, 31);
	graph.AddEdge(01, 00);
	graph.AddEdge(01, 02);
	graph.AddEdge(01, 03);
	graph.AddEdge(01, 04);
	graph.AddEdge(01, 23);
	graph.AddEdge(01, 25);
	graph.AddEdge(01, 27);
	graph.AddEdge(01, 31);
	graph.AddEdge(02, 00);
	graph.AddEdge(02, 01);
	graph.AddEdge(02, 03);
	graph.AddEdge(02, 04);
	graph.AddEdge(02, 23);
	graph.AddEdge(02, 24);
	graph.AddEdge(02, 25);
	graph.AddEdge(02, 31);
	graph.AddEdge(03, 00);
	graph.AddEdge(03, 01);
	graph.AddEdge(03, 02);
	graph.AddEdge(03, 04);
	graph.AddEdge(03, 23);
	graph.AddEdge(03, 25);
	graph.AddEdge(03, 27);
	graph.AddEdge(03, 31);
	graph.AddEdge(04, 00);
	graph.AddEdge(04, 01);
	graph.AddEdge(04, 02);
	graph.AddEdge(04, 03);
	graph.AddEdge(04, 23);
	graph.AddEdge(04, 25);
	graph.AddEdge(04, 27);
	graph.AddEdge(04, 31);
	graph.AddEdge(05, 07);
	graph.AddEdge(05, 10);
	graph.AddEdge(05, 11);
	graph.AddEdge(05, 12);
	graph.AddEdge(05, 14);
	graph.AddEdge(05, 16);
	graph.AddEdge(05, 17);
	graph.AddEdge(05, 24);
	graph.AddEdge(05, 26);
	graph.AddEdge(05, 30);
	graph.AddEdge(05, 32);
	graph.AddEdge(06, 9);
	graph.AddEdge(06, 28);
	graph.AddEdge(06, 29);
	graph.AddEdge(07, 05);
	graph.AddEdge(07, 10);
	graph.AddEdge(07, 11);
	graph.AddEdge(07, 12);
	graph.AddEdge(07, 14);
	graph.AddEdge(07, 16);
	graph.AddEdge(07, 17);
	graph.AddEdge(07, 18);
	graph.AddEdge(07, 19);
	graph.AddEdge(07, 24);
	graph.AddEdge(07, 26);
	graph.AddEdge(07, 30);
	graph.AddEdge(07, 32);
	graph.AddEdge(8, 13);
	graph.AddEdge(9, 06);
	graph.AddEdge(9, 15);
	graph.AddEdge(9, 20);
	graph.AddEdge(9, 33);
	graph.AddEdge(10, 05);
	graph.AddEdge(10, 07);
	graph.AddEdge(10, 11);
	graph.AddEdge(10, 12);
	graph.AddEdge(10, 14);
	graph.AddEdge(10, 16);
	graph.AddEdge(10, 17);
	graph.AddEdge(10, 18);
	graph.AddEdge(10, 19);
	graph.AddEdge(10, 24);
	graph.AddEdge(10, 26);
	graph.AddEdge(10, 30);
	graph.AddEdge(10, 32);
	graph.AddEdge(11, 05);
	graph.AddEdge(11, 07);
	graph.AddEdge(11, 10);
	graph.AddEdge(11, 12);
	graph.AddEdge(11, 14);
	graph.AddEdge(11, 16);
	graph.AddEdge(11, 17);
	graph.AddEdge(11, 19);
	graph.AddEdge(11, 24);
	graph.AddEdge(11, 26);
	graph.AddEdge(11, 30);
	graph.AddEdge(11, 32);
	graph.AddEdge(12, 05);
	graph.AddEdge(12, 07);
	graph.AddEdge(12, 10);
	graph.AddEdge(12, 11);
	graph.AddEdge(12, 14);
	graph.AddEdge(12, 16);
	graph.AddEdge(12, 17);
	graph.AddEdge(12, 18);
	graph.AddEdge(12, 19);
	graph.AddEdge(12, 24);
	graph.AddEdge(12, 26);
	graph.AddEdge(12, 30);
	graph.AddEdge(12, 32);
	graph.AddEdge(13, 8);
	graph.AddEdge(14, 05);
	graph.AddEdge(14, 07);
	graph.AddEdge(14, 10);
	graph.AddEdge(14, 11);
	graph.AddEdge(14, 12);
	graph.AddEdge(14, 16);
	graph.AddEdge(14, 17);
	graph.AddEdge(14, 18);
	graph.AddEdge(14, 19);
	graph.AddEdge(14, 26);
	graph.AddEdge(14, 30);
	graph.AddEdge(14, 32);
	graph.AddEdge(15, 9);
	graph.AddEdge(15, 20);
	graph.AddEdge(15, 33);
	graph.AddEdge(16, 05);
	graph.AddEdge(16, 07);
	graph.AddEdge(16, 10);
	graph.AddEdge(16, 11);
	graph.AddEdge(16, 12);
	graph.AddEdge(16, 14);
	graph.AddEdge(16, 17);
	graph.AddEdge(16, 18);
	graph.AddEdge(16, 19);
	graph.AddEdge(16, 24);
	graph.AddEdge(16, 26);
	graph.AddEdge(16, 30);
	graph.AddEdge(16, 32);
	graph.AddEdge(17, 05);
	graph.AddEdge(17, 07);
	graph.AddEdge(17, 10);
	graph.AddEdge(17, 11);
	graph.AddEdge(17, 12);
	graph.AddEdge(17, 14);
	graph.AddEdge(17, 16);
	graph.AddEdge(17, 18);
	graph.AddEdge(17, 19);
	graph.AddEdge(17, 24);
	graph.AddEdge(17, 26);
	graph.AddEdge(17, 30);
	graph.AddEdge(17, 32);
	graph.AddEdge(18, 07);
	graph.AddEdge(18, 10);
	graph.AddEdge(18, 12);
	graph.AddEdge(18, 14);
	graph.AddEdge(18, 16);
	graph.AddEdge(18, 17);
	graph.AddEdge(18, 19);
	graph.AddEdge(18, 20);
	graph.AddEdge(18, 26);
	graph.AddEdge(18, 30);
	graph.AddEdge(18, 32);
	graph.AddEdge(19, 07);
	graph.AddEdge(19, 10);
	graph.AddEdge(19, 11);
	graph.AddEdge(19, 12);
	graph.AddEdge(19, 14);
	graph.AddEdge(19, 16);
	graph.AddEdge(19, 17);
	graph.AddEdge(19, 18);
	graph.AddEdge(19, 26);
	graph.AddEdge(19, 30);
	graph.AddEdge(19, 32);
	graph.AddEdge(20, 9);
	graph.AddEdge(20, 15);
	graph.AddEdge(20, 18);
	graph.AddEdge(20, 33);
	graph.AddEdge(21, 22);
	graph.AddEdge(22, 21);
	graph.AddEdge(22, 27);
	graph.AddEdge(23, 00);
	graph.AddEdge(23, 01);
	graph.AddEdge(23, 02);
	graph.AddEdge(23, 03);
	graph.AddEdge(23, 04);
	graph.AddEdge(23, 25);
	graph.AddEdge(23, 27);
	graph.AddEdge(23, 31);
	graph.AddEdge(24, 00);
	graph.AddEdge(24, 02);
	graph.AddEdge(24, 05);
	graph.AddEdge(24, 07);
	graph.AddEdge(24, 10);
	graph.AddEdge(24, 11);
	graph.AddEdge(24, 12);
	graph.AddEdge(24, 16);
	graph.AddEdge(24, 17);
	graph.AddEdge(24, 26);
	graph.AddEdge(24, 30);
	graph.AddEdge(24, 32);
	graph.AddEdge(25, 00);
	graph.AddEdge(25, 01);
	graph.AddEdge(25, 02);
	graph.AddEdge(25, 03);
	graph.AddEdge(25, 04);
	graph.AddEdge(25, 23);
	graph.AddEdge(25, 27);
	graph.AddEdge(25, 31);
	graph.AddEdge(26, 05);
	graph.AddEdge(26, 07);
	graph.AddEdge(26, 10);
	graph.AddEdge(26, 11);
	graph.AddEdge(26, 12);
	graph.AddEdge(26, 14);
	graph.AddEdge(26, 16);
	graph.AddEdge(26, 17);
	graph.AddEdge(26, 18);
	graph.AddEdge(26, 19);
	graph.AddEdge(26, 24);
	graph.AddEdge(26, 30);
	graph.AddEdge(26, 32);
	graph.AddEdge(27, 00);
	graph.AddEdge(27, 01);
	graph.AddEdge(27, 03);
	graph.AddEdge(27, 04);
	graph.AddEdge(27, 22);
	graph.AddEdge(27, 23);
	graph.AddEdge(27, 25);
	graph.AddEdge(27, 31);
	graph.AddEdge(28, 06);
	graph.AddEdge(28, 29);
	graph.AddEdge(29, 06);
	graph.AddEdge(29, 28);
	graph.AddEdge(30, 05);
	graph.AddEdge(30, 07);
	graph.AddEdge(30, 10);
	graph.AddEdge(30, 11);
	graph.AddEdge(30, 12);
	graph.AddEdge(30, 14);
	graph.AddEdge(30, 16);
	graph.AddEdge(30, 17);
	graph.AddEdge(30, 18);
	graph.AddEdge(30, 19);
	graph.AddEdge(30, 24);
	graph.AddEdge(30, 26);
	graph.AddEdge(30, 32);
	graph.AddEdge(31, 00);
	graph.AddEdge(31, 01);
	graph.AddEdge(31, 02);
	graph.AddEdge(31, 03);
	graph.AddEdge(31, 04);
	graph.AddEdge(31, 23);
	graph.AddEdge(31, 25);
	graph.AddEdge(31, 27);
	graph.AddEdge(32, 05);
	graph.AddEdge(32, 07);
	graph.AddEdge(32, 10);
	graph.AddEdge(32, 11);
	graph.AddEdge(32, 12);
	graph.AddEdge(32, 14);
	graph.AddEdge(32, 16);
	graph.AddEdge(32, 17);
	graph.AddEdge(32, 18);
	graph.AddEdge(32, 19);
	graph.AddEdge(32, 24);
	graph.AddEdge(32, 26);
	graph.AddEdge(32, 30);
	graph.AddEdge(33, 9);
	graph.AddEdge(33, 15);
	graph.AddEdge(33, 20);

	graph.BFS();
	
	return 0;
}