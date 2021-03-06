#include "stdafx.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

class node {
public:
	void addIncomingNode (int nodeNumber, int pathCost) {
		incomingNodes.push_back (nodeNumber);
		incomingNodesWeight.push_back (pathCost);
	}

	void addOutcomingNode (int nodeNumber, int pathCost) {
		outcomingNodes.push_back (nodeNumber);
		outcomingNodesWeight.push_back(pathCost);
	}

	void printNode () {
		cout << "incoming nodes \n";
		for (auto i = incomingNodes.begin (), j = incomingNodesWeight.begin(); i != incomingNodes.end (); ++i, ++j)
			cout << *i << ' ' << *j << '\n';

		cout << "outcoming nodes \n";
		for (auto i = outcomingNodes.begin (), j = outcomingNodesWeight.begin(); i != outcomingNodes.end (); ++i, ++j)
			cout << *i << ' ' << *j << '\n';
	}

	void setEdjeNumber (int edjeNumber) {
		i = edjeNumber;
	}

	void setPathCost (int pathCost) {
		f = pathCost;
	}

	int getPatCost () {
		return f;
	}

	void setOptimaledjeTo (int edjeNumber) {
		j = edjeNumber;
	}

	friend int findFirstPoint (const vector<node> & graph);

	vector<int> getOutcomingNodes () {
		return outcomingNodes;
	}

	vector<int> getIncomingNodes (){
		return incomingNodes;
	}

	vector <int> getOutcomingNodesWeights () {
		return outcomingNodesWeight;
	}

	friend int findFinalPoint (const vector<node> & graph);

	friend void printRoute (const vector<node> & graph );

private:
	vector <int> incomingNodes; //nodeNumber, path cost to it
	vector<int> incomingNodesWeight;

	vector < int > outcomingNodes; // same
	vector <int> outcomingNodesWeight;
	
	int i; // edgeNumber
	int f; // минимальный путь отэтой вершины
	int n; // domain
	int j; // optimal edje 
};

int findFinalPoint (const vector<node> & graph) {
	for (auto i = graph.begin (); i != graph.end (); ++i) {
		if (i->outcomingNodes.empty ())
			return i->i;
	}

	return -1;
}

int findFirstPoint (const vector<node> & graph) {
	for (auto i = graph.begin (); i != graph.end (); ++i) {
		if (i->incomingNodes.empty ())
			return i->i;
	}

	return -1;
}

void printRoute (const vector <node > &graph) {
	int currentPoint = findFirstPoint (graph);
	int finishPoint = findFinalPoint (graph);

	cout << currentPoint << ' ';

	while (finishPoint != currentPoint) {
		cout << graph.at (currentPoint).j + 1 << ' ';
		currentPoint = graph.at (currentPoint).j;
	};
}

int main()
{
	ifstream inputFile ("input.txt");

	int numberOfEdges = 0;

	inputFile >> numberOfEdges;

	vector<vector<int>> adjacencyMatrix;
	vector<int> currentLine;
	int currentNumber;

	for (int i = 0; i < numberOfEdges; ++i) {

		for (int j = 0; j < numberOfEdges; ++j) {
			inputFile >> currentNumber;
			currentLine.push_back (currentNumber);
		};

		adjacencyMatrix.push_back (currentLine);

		currentLine.erase (currentLine.begin (), currentLine.end ());
	};

	////checking matrix
	//for (int i = 0; i < numberOfEdges; ++i)
	//	for (auto j = adjacencyMatrix.at (i).begin (); j != adjacencyMatrix.at (i).end (); ++j)
	//		cout << *j << ' ';

	vector<node> graph;
	graph.reserve (sizeof (node) * numberOfEdges);
	for (int i = 0; i < numberOfEdges; ++i) {
		node currentVector;
		graph.push_back (currentVector);
	};
	
	for (int i = 0; i < numberOfEdges; ++i) {
		graph.at (i).setEdjeNumber (i);
		graph.at (i).setOptimaledjeTo (-1);
		graph.at (i).setPathCost (-1);
		for (int j = 0; j < numberOfEdges; ++j)
			if (adjacencyMatrix.at (i).at (j) != 0) {
				graph.at (i).addOutcomingNode (j, adjacencyMatrix.at (i).at (j));
				graph.at (j).addIncomingNode (i, adjacencyMatrix.at (i).at (j));
			}
	};

	int finalPointPosition = findFinalPoint (graph);
	int startPointPosition = findFirstPoint (graph);

	queue<int> edjes;

	edjes.push (finalPointPosition);
	int currentPosition;

	while (true) {
		currentPosition = edjes.front ();
		edjes.pop ();

		if (graph.at (currentPosition).getPatCost() != -1)
			continue;
		
		vector<int> out = graph.at (currentPosition).getOutcomingNodes ();
		vector<int> weights = graph.at (currentPosition).getOutcomingNodesWeights ();

		if (out.empty ())
			graph.at (currentPosition).setPathCost(0); // мы в конечной точке 
		else
		{
			//очень костыли 
			int minEdje = -1;
			int minWeight = 10000;

			for (auto i = out.begin (), j = weights.begin (); i != out.end (); ++i, ++j)
				if (graph.at (*i).getPatCost() + *j < minWeight) {
					minWeight = graph.at (*i).getPatCost() + *j;
					minEdje = *i;
				}

			graph.at (currentPosition).setPathCost(minWeight);
			graph.at (currentPosition).setOptimaledjeTo(minEdje);			
		}

		if (currentPosition == startPointPosition)
			break;

		vector<int> in = graph.at (currentPosition).getIncomingNodes();
		for (auto i = in.begin (); i != in.end (); ++i) {
			edjes.push (*i);
		};
	
	};

	cout << graph.at (startPointPosition).getPatCost() << '\n';
	

	printRoute (graph);
	system ("pause");

    return 0;
}