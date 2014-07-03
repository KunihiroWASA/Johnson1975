#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

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
    Node __dummy_node = -1;
    int __cnt; 

    void UNBLOCK(Node u);  
    bool CIRCUIT(Node v); 
    void OUTPUT(); 
    void SET_Ak(Graph & __g, std::map<Node, Node> & I); 
    void DFS(Node v, Node & c, std::stack<Node> & S, std::stack<Node> & B,  std::map<Node, Node> & I, Graph & __g); 
    Node LEAST_VERTEX_Ak(); 
    Node MIN_VERTEX(Graph & __g); 
    Node MAX_VERTEX(Graph & __g); 

    void __scc(Graph & __g, std::map<Node, Node> & I); 
    Node __ith_on_stack(int i, std::stack<Node> & s); 

    public:
        void MAIN(Graph & __g); 

};

int main(int argc, char const* argv[])
{
    Graph g = {
        {0, {1, 2, 3}}, 
        {1, {0, 2, 3}},     
        {2, {0, 1, 3}},    
        {3, {0, 1, 2}}    
    }; 
    CircuitFindingAlgorithm CFA; 

    CFA.MAIN(g); 


    return 0;
}

void CircuitFindingAlgorithm::MAIN(Graph & __g) 
{
    Node n = MAX_VERTEX(__g);
         s = MIN_VERTEX(__g);
    std::map<Node, Node> I; 
    __scc(__g, I); 
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
        if (s <= v.first) {
            Vk.push_back(v.first); 
            Ak[v.first] = {}; 
        }
    }
    for (auto&& v : __g) {
        if (v.first != I[s]) {
            continue; 
        }
        for (auto&& u : v.second) {
            if (Vk.end() != std::find(Vk.begin(), Vk.end(), u) and u == I[s]) {
                Ak[v.first].push_back(u); 
            }
        }
    }
}


// Gabow, IPL, 2000. 
void CircuitFindingAlgorithm::__scc(Graph & __g, std::map<Node, Node> & I) 
{
    std::stack<Node> S, B; 
    for (auto& cv : __g) {
        I[cv.first] = __dummy_node; 
    }
    Node c = __dummy_node; // (int) __g.size(); 
    for (auto&& v : __g) {
        if (I[v.first] == __dummy_node) {
            DFS(v.first, c, S, B, I, __g); 
        }
    }
}
    
void CircuitFindingAlgorithm::DFS(Node v, Node & c, std::stack<Node> & S, std::stack<Node> & B,  std::map<Node, Node> & I, Graph & __g) 
{
    S.push(v); 
    I[v] = S.top(); 
    B.push(I[v]); 
    for (auto&& w : __g[v]) {
        if (I[w] == __dummy_node) {
            DFS(w, c, S, B, I, __g); 
        } else {
            while (I[v] < __ith_on_stack(B.top(), B)) {
                B.pop(); 
            }
        }
    }

    if (I[v] == __ith_on_stack(B.top(), B)) {
        B.pop(); ++c; 
        while (I[v] <= S.top()) {
            I[S.top()] = c; 
            S.pop(); 
        }
    }
}

Node CircuitFindingAlgorithm::__ith_on_stack(int i, std::stack<Node> & s)
{
    Node ith_node = __dummy_node; 
    if (s.empty()) {
        return ith_node; 
    }
    std::stack<Node> __dummy_stack; 
    for (; 0 < i and !s.empty(); --i) {
        __dummy_stack.push(s.top()); 
        s.pop(); 
    }
    
    if (!s.empty()) {
        ith_node = s.top(); 
    }
    while (!__dummy_stack.empty()) {
        s.push(__dummy_stack.top()); 
        __dummy_stack.pop(); 
    }

    return ith_node; 
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
