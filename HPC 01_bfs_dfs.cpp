#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

struct Graph
{
    int V;
    vector<vector<int>> adj; // vector of type vector of ints

    Graph(int V)
    {
        this->V = V;
        adj.resize(V); // resize adjecency matrix to v
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v); // add v to 'u'th vector inside adj
        adj[v].push_back(u); // add u to 'v'th vector inside adj
    }

    void BFS(int start)
    {
        vector<bool> visited(V, false); // initialize visited list
        queue<int> q;                   // initialize queue
        visited[start] = true;
        q.push(start);

        while (!q.empty()) // recursive bfs
        {
            int u = q.front();
            q.pop();
            cout << u << " ";

#pragma omp parallel for                            // spawn threads and divide loop iterations between them
            for (int i = 0; i < adj[u].size(); i++) // for all neighbours of u
            {
                int v = adj[u][i];
                if (!visited[v]) // if the neighbour is not visited
                {
#pragma omp critical // only one thread executes at a time, the next one runs AFTER this one is done
                    {
                        visited[v] = true; // mark visited
                        q.push(v);         // push to queue
                    }
                }
            }
        }
        cout << endl;
    }

    void DFS(int start)
    {
        vector<bool> visited(V, false); // initialize visited list
#pragma omp parallel                    // initialize multiple threads
        {
#pragma omp single nowait // this must only be run ONCE by one thread
                          // nowait means other threads won't wait for this one to finish executing
            {
                DFSUtil(start, visited);
            }
        }
        cout << endl;
    }

    void DFSUtil(int u, vector<bool> &visited) // recursively explore each branch's neighbours until the branch is over, then ove to the next branch hence DFS
    {
        visited[u] = true; // mark u as visited
        cout << u << " ";

#pragma omp parallel for                        // parallelization
        for (int i = 0; i < adj[u].size(); i++) // for all neighbours
        {
            int v = adj[u][i];
            if (!visited[v]) // if neighbour not visited
            {
                DFSUtil(v, visited); // recur
            }
        }
    }
};

int main()
{
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    Graph g(V);

    int edgeCount;
    cout << "Enter the number of edges: ";
    cin >> edgeCount;

    cout << "Enter the edges (in format 'source destination'): \n";
    for (int i = 0; i < edgeCount; i++)
    {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "BFS traversal starting from node 0: ";
    g.BFS(0);

    cout << "DFS traversal starting from node 0: ";
    g.DFS(0);

    return 0;
}
