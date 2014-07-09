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
    std::stack<Node> __stack; 
    Node s; 
    Node __dummy_node = 0;
    int __cnt; 

    void UNBLOCK(Node u);  
    bool CIRCUIT(Node v); 
    void OUTPUT(); 
    void SET_Ak(Graph & __g, std::map<Node, Node> & I); 
    Node LEAST_VERTEX_Ak(); 
    Node MIN_VERTEX(Graph & __g); 
    Node MAX_VERTEX(Graph & __g); 

    void __scc(Graph & __g, std::map<Node, int> & I); 
    void DFS(Node v, int & c, std::stack<Node> & S, std::stack<int> & B,  std::map<Node, int> & I, Graph & __g); 

    public:
        void MAIN(Graph & __g); 

};

int main(int argc, char const* argv[])
{
    Graph g = {
        {1, {2, 3, 4}}, 
        {2, {1, 3, 4}},     
        {3, {1, 2, 4}},    
        {4, {1, 2, 3}}    
    }; 
    CircuitFindingAlgorithm CFA; 

    CFA.MAIN(g); 


    return 0;
}

void CircuitFindingAlgorithm::MAIN(Graph & __g) 
{
    Node n = MAX_VERTEX(__g);
         s = MIN_VERTEX(__g);
    std::map<Node, int> I; 
    __scc(__g, I); 
    // for (auto& ii : I) {
        // std::cout << ii.first << ", " << ii.second << std::endl; 
    // }
    __cnt = 0; 
    while (s < n) {
        SET_Ak(__g, I); 
        if (!Ak.empty()) {
            s = LEAST_VERTEX_Ak(); 
            for (auto& ci : Ak) {
                Node i = ci.first; 
                blocked[i] = false;
                B[i].clear(); 
            }
            CIRCUIT(s); 
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

void CircuitFindingAlgorithm::OUTPUT()
{
    std::stack<Node> __dummy_stack; 
    std::cout << "C" << __cnt++ << ": " << s; 
    while (!__stack.empty()) {
        Node t = __stack.top(); 
        std::cout << " -> " << t; 
        __stack.pop(); 
        __dummy_stack.push(t); 
    }
    std::cout << std::endl; 
    while (!__dummy_stack.empty()) {
        Node t = __dummy_stack.top(); 
        __dummy_stack.pop(); 
        __stack.push(t); 
    }
}

bool  CircuitFindingAlgorithm::CIRCUIT(Node v) 
{
    bool f = false; 
    __stack.push(v); 
    blocked[v] = true; 
    for (auto& w : Ak[v]) {
        if (w == s) {
            f = true; 
            OUTPUT(); 
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

void CircuitFindingAlgorithm::SET_Ak(Graph & __g, std::map<Node, Node> & I)
{
    std::vector<Node> Vk; 
    Ak.clear(); 
    for (auto&& v: I) {
        if (v.second == I[s] and v.first >= s) {
            Ak[v.first] = {}; 
            for (auto& w : __g[v.first]) {
                if (I[s] == I[w] and w >= s) {
                    Ak[v.first].push_back(w); 
                }
            }
        }
    }
    // for (auto& ak : Ak) {
        // std::cout << ak.first << ": "; 
        // for (auto& v : ak.second) {
            // std::cout << v << " "; 
        // }
        // std::cout << std::endl; 
    // }
    // std::cout << std::endl; 
}


// Gabow, IPL, 2000. 
void CircuitFindingAlgorithm::__scc(Graph & __g, std::map<Node, int> & I) 
{
    std::stack<Node> S; 
    std::stack<int> B; 
    for (auto& v : __g) {
        I[v.first] = 0; 
    }
    int c = (int) __g.size(); 
    for (auto& v : __g) {
        if (I[v.first] == 0) {
            DFS(v.first, c, S, B, I, __g); 
        }
    }
}
    
void CircuitFindingAlgorithm::DFS(Node v, int & c, std::stack<Node> & S, std::stack<int> & B,  std::map<Node, int> & I, Graph & __g) 
{
    S.push(v); 
    I[v] = S.size(); 
    B.push(I[v]); 
    for (auto& w : __g[v]) {
        if (I[w] == 0) {
            DFS(w, c, S, B, I, __g); 
        } else {
            while (I[w] < B.top()) {
                B.pop(); 
            }
        }
    }

    if (I[v] == B.top()) {
        B.pop(); ++c; 
        while (I[v] <= S.size()) {
            I[S.top()] = c; 
            S.pop(); 
        }
    }
}


Node CircuitFindingAlgorithm::LEAST_VERTEX_Ak()
{
    Node l = __dummy_node; 
    for (auto& cv : Ak) {
        if (cv.first < l or l == __dummy_node) {
            l = cv.first; 
        }
    }
    return l; 
}

Node CircuitFindingAlgorithm::MIN_VERTEX(Graph & __g)
{
    Node m = __dummy_node; 
    for (auto& cv : __g) {
        if (cv.first < m or m == __dummy_node) {
            m = cv.first; 
        }
    }
    return m; 
}
Node CircuitFindingAlgorithm::MAX_VERTEX(Graph & __g)
{
    Node m = __dummy_node; 
    for (auto& cv : __g) {
        if (cv.first > m or m == __dummy_node) {
            m = cv.first; 
        }
    }
    return m; 
}
