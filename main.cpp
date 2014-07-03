#include <iostream>
#include <map>
#include <vector>
#include <stack>

typedef  int                  Node; 
typedef  std::vector<Node>    AdjList; 
typedef  std::map<Node, AdjList> Graph; 


class CircuitFindingAlgorithm 
{
    std::map<Node, std::vector<Node>> Ak; 
    std::map<Node, std::vector<Node>> B; 
    std::map<Node, bool> blocked; 
    Node s; 

    void UNBLOCK(Node u);  
    bool CIRCUIT(Node v); 
    void SET_Ak(const Graph & __g);  
    Node LEAST_VERTEX_Ak(); 
    Node MIN_VERTEX(const Graph & __g); 
    Node MAX_VERTEX(const Graph & __g); 
    void MAIN(const Graph & __g); 

};

int main(int argc, char const* argv[])
{
    std::cout << "hello, world!" << std::endl;
    CircuitFindingAlgorithm CFA; 


    return 0;
}

void CircuitFindingAlgorithm::MAIN(const Graph & __g) 
{
    Node n = MAX_VERTEX(__g);
         s = MIN_VERTEX(__g);
    while (s < n) {
        SET_Ak(__g); 
        if (!Ak.empty()) {
            s = LEAST_VERTEX_Ak(); 
            for (auto& ci : Ak) {
                Node i = ci.first; 
                blocked[i] = false;
                B[i].clear(); 
            }
            bool dummy = CIRCUIT(s); 
            s = s + 1; 
        } else {
            s = n; 
        }
    }
}

void CircuitFindingAlgorithm::UNBLOCK(Node u) 
{
    blocked[u] = false; 
    for (auto itr = B[u].begin(); itr != B[u].end(); ) {
        Node w = *itr;
        itr    = B[u].erase(itr);
        if (blocked[w]) {
            UNBLOCK(w); 
        }
    }
}

bool  CircuitFindingAlgorithm::CIRCUIT(Node v) 
{
    bool f = false; 
    std::stack<Node> __stack; 
    __stack.push(v); 
    blocked[v] = true; 
    for (auto& w : Ak[v]) {
        if (w == s) {
            f = true; 
        } else if (!blocked[w]) {
            if (CIRCUIT(w)) {
                f = true; 
            }
        }
    }
    if (f) {
        UNBLOCK(v); 
    } else {
        for (auto& w : Ak[v]) {
            if(B[w].end() == std::find(B[w].begin(), B[w].end(), v)) {
            B[w].push_back(v); 
            }
        }
    }
    __stack.pop(); 
    return f; 
}

void CircuitFindingAlgorithm::SET_Ak(const Graph & __g)
{
    
}

Node CircuitFindingAlgorithm::LEAST_VERTEX_Ak()
{
    Node l = -1; 
    for (const auto& cv : Ak) {
        if (cv.first < l or l == -1) {
            l = cv.first; 
        }
    }
    return l; 
}

Node CircuitFindingAlgorithm::MIN_VERTEX(const Graph & __g)
{
    Node m = -1; 
    for (const auto& cv : __g) {
        if (cv.first < m or m == -1) {
            m = cv.first; 
        }
    }
    return m; 
}
Node CircuitFindingAlgorithm::MAX_VERTEX(const Graph & __g)
{
    Node m = -1; 
    for (const auto& cv : __g) {
        if (cv.first > m or m == -1) {
            m = cv.first; 
        }
    }
    return m; 
}
