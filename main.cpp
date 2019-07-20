#include <iostream>
using namespace std;
#include <fstream>
#include <cmath>
#include <list>
#include <stack>
#include <string>

double  const INFINITE = 0xFFFFFFFFFFFFF;

struct Vertex
{
	int xCoordinate;
	int yCoordinate;
};




//Dijkstra's algorithm
void Dijkstra( double const* const* const &adjacencyMatrix, int const &totalVertices, int const &sourceVertex, int *&parentList, double *&distanceFromSourceVertexList)
{
	struct DijkstraNode
	{
		int vertexNumber;
		double distance;
	};



	
	for (int i = 0; i < totalVertices; i++)
	{
		distanceFromSourceVertexList[i] = INFINITE;
	}

	distanceFromSourceVertexList[sourceVertex] = 0; //distance of source vertex from source vertex is 0.

	parentList[sourceVertex] = -1; //the parent of source vertex is NULL.
	list<int> queue;

	for (int i = 0; i < totalVertices; i++)
	{

		queue.push_back(i);
	
	}


	//extracting min from the queue 
	while (queue.size() != 0)
	{
		
		int minVertex = queue.front();
		queue.pop_front();
		int length = queue.size();

		for (int i = 0; i < length; i++)
		{
			int secondVertex = queue.front();
			queue.pop_front();
			if (distanceFromSourceVertexList[minVertex] <= distanceFromSourceVertexList[secondVertex])
			{
				queue.push_back(secondVertex);
				continue;
			}
			else
			{
				queue.push_back(minVertex);
				minVertex = secondVertex;
			}

		}

		
		//checking each vertex that has an edge with our minVertex
		for (int i = 0; i < totalVertices; i++)
		{
			if (minVertex == i || adjacencyMatrix[i][minVertex] == -1)
				continue;

			if (distanceFromSourceVertexList[minVertex] + adjacencyMatrix[i][minVertex] < distanceFromSourceVertexList[i])
			{
				distanceFromSourceVertexList[i] = distanceFromSourceVertexList[minVertex] + adjacencyMatrix[i][minVertex];
				parentList[i] = minVertex;

			}
		}

	
	}


}//end of Dikstra function


//This function reads the graph from the file. fileName is the input parameter, all other parameters are output parameters.
void  readGraph(string const& fileName, Vertex *&vertexList, double ** &adjacencyMatrix, int &totalEdges, int &totalVertices)
{
	ifstream read(fileName);

	if (read.is_open() == false)
	{
		cout << "File could not open!" << endl;
		exit(1);
	}



	read >> totalVertices >> totalEdges;

	adjacencyMatrix = new double*[totalVertices]; 
	vertexList = new Vertex[totalVertices]; //vertexList stores information about the each vertex' coordinates

	for (int i = 0; i < totalVertices; i++)
	{
		adjacencyMatrix[i] = new double[totalVertices];
		for (int j = 0; j < totalVertices; j++)
			adjacencyMatrix[i][j] = -1; /*-1 means that there is no edge between vertex i and vertex j. If there is an edge between i and j 
		                                   it will be updated in the loop where edges are being read from the file*/
	}

	//reading coordinates of each vertex
	for (int i = 0; i < totalVertices; i++)
	{
		int vertexNumber;
		read >> vertexNumber;
		read >> vertexList[vertexNumber].xCoordinate >> vertexList[vertexNumber].yCoordinate;
	}

	//reading the edges from file
	for (int i = 0; i < totalEdges; i++)
	{
		int v1, v2;
		read >> v1 >> v2;
	
		//calculating the weight of the edge using distance formula
		double weight = sqrt(((vertexList[v1].xCoordinate - vertexList[v2].xCoordinate)*(vertexList[v1].xCoordinate - vertexList[v2].xCoordinate)) + ((vertexList[v1].yCoordinate - vertexList[v2].yCoordinate)*(vertexList[v1].yCoordinate - vertexList[v2].yCoordinate)));
		
		adjacencyMatrix[v1][v2] = weight; 
		adjacencyMatrix[v2][v1] = weight;
	}

	read.close();

}//end of readGraph function

//This function simply prints the information of graph.
void printGraphInfoOnScreen(Vertex const * const&vertexList, double const* const* const &adjacencyMatrix, int const & totalVertices, int const & totalEdges)
{
	cout << endl << "Total Vertices= " << totalVertices << ". Total Edges=  " << totalEdges << endl;
	cout << endl << "Information About Vertices: " << endl;
	for (int i = 0; i < totalVertices; i++)
	{
		cout << "Vertex: " << i << "   " << vertexList[i].xCoordinate << "   " << vertexList[i].yCoordinate << endl;

	}

	cout << endl << endl << "Information About Edges: " << endl;

	for (int i = 0; i < totalVertices; i++)
	{
		for (int j = 0; j < totalVertices; j++)
		{
			if (i <= j && adjacencyMatrix[i][j] != -1)
				cout << "There is an edge between Vertex: " << i << " and Vertex: " << j << "; the weight of this edge is " << adjacencyMatrix[i][j] << "." << endl << endl;
		}
	}
}//end of printGraphInfoOnScreen function


int main()
{
	int totalVertices = 0,
		totalEdges = 0;

	Vertex *vertexList=NULL;
	double **adjacencyMatrix=NULL;

	string fileName;
	cout << "Please enter the name of the file from which graph is to be read: ";
	cin >> fileName;

	//reading the graph from the file
	readGraph(fileName, vertexList, adjacencyMatrix, totalEdges, totalVertices);
	

	//printing the graph information on the screen.
	printGraphInfoOnScreen(vertexList, adjacencyMatrix, totalVertices, totalEdges);


	int **parentList = new int*[totalVertices];
	double **distanceFromSourceVertex = new double*[totalVertices];
	
	//calculating the distance of every source vertext from every other vertex using Dijkstra
	for (int i = 0; i < totalVertices; i++)
	{

		distanceFromSourceVertex[i] = new double[totalVertices];
		parentList[i] = new int[totalVertices];

		//vertex i is source vertex in each iteration
		Dijkstra( adjacencyMatrix, totalVertices, i, parentList[i], distanceFromSourceVertex[i]);
		

	}
	cout << endl;

	//In the above section, Dikstra's algorithm has computed the distance of every vertex from every other vertex.


	//Now we will simply print the distance and exact path between two vertices given by the user.
	int sourceVertex, destinationVertex;

	cout << "Please enter the source vertex Number: ";
	cin >> sourceVertex;
	
	cout << "Now please enter the destination vertex Number: ";
	cin >>  destinationVertex;

	cout << endl;
	cout << "-> The weight of the minimum path between source Vertex: " << sourceVertex << " and destination vertex: " << destinationVertex << " is " << distanceFromSourceVertex[sourceVertex][destinationVertex] <<"."<< endl;

	cout << "-> The exact minimum path between source vertex: " << sourceVertex << " and destination vertex: " << destinationVertex << " is: " << endl << endl;

	stack<int> path;
	path.push(destinationVertex);
	int j = destinationVertex;
	for (int i = 0; i < totalVertices; i++)
	{
		if (parentList[sourceVertex][j] == -1)
			break;

		path.push(parentList[sourceVertex][j]);
		
		j = parentList[sourceVertex][j];


	}

	//printing path
	while (path.size() != 0)
	{
		cout << path.top();
		path.pop();
		if (path.size() != 0)
			cout << " ---->";
	}
	cout << endl;
}