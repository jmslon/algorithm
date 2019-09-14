#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

#define INF 0x6fffffff

typedef int ll;

using namespace std;

#define MAXN 300005
#define pb push_back
typedef long long lld;

int N, M, C;
int dep[MAXN];
int U[MAXN], D[MAXN], F[MAXN];
/*
 * U[i] = i가 루트인 서브트리에 '완전히' 포함된 back edge의 수
 * D[i] = i가 루트인 서브트리에 '조금이라도' 포함된 back edge의 수
 * F[i] = i가 루트인 서브트리에서 i의 부모가 목적지인 back edge의 수
 */
vector <int> adj[MAXN], child[MAXN];

void dfs(int n, int from)
{
    for (int t: adj[n]) if (t != from){
        if (!dep[t]){
            // tree edge
            child[n].pb(t); dep[t] = dep[n]+1;
            int tmp = U[n]; dfs(t, n); F[t] = U[n]-tmp;
            U[n] += U[t]; D[n] += D[t];
        }else if (dep[t] < dep[n]){
            // back edge
            D[n]++; U[t]++;
        }
    }
}


struct Graph {
    struct Edge {
        int src, dst; unsigned long key;
    };
    
    int V;
    vector<vector<unsigned long>> adj, radj;
    vector<Edge> edges;
    
    Graph(int size) {
        V = size;
        adj.resize(size);
        radj.resize(size);
    }
    
    void push(int src, int dst) {
        push(src, dst, edges.size());
    }
    
    void push(int src, int dst, unsigned long key) {
        adj[src].push_back(key);
        radj[dst].push_back(key);
        edges.push_back({src, dst, key});
    }
};

struct DFSSpanningTree: Graph {
    int root;
    vector<int> level, type, f, g;
    vector<bool> inq;
    DFSSpanningTree(int size):Graph(size){}
    
    void dfs_spanning_tree(int root) {
        
        this->root = root;
        
        type.resize(edges.size(), 0);
        level.resize(V, -1);
        f.resize(V, -1);
        inq.resize(V);
        dfs(1, 0);
        
        
    }
    
    void dfs(int src, int par) {
        if (level[src] != -1) return;
        
        level[src] = level[par] + 1;
        f[src] = (int) g.size();
        g.push_back(src);
        inq[src] = 1;
        
        for (auto idx: adj[src]) {
            auto dst = edges[idx].dst;
            if (dst != par) {
                if (level[dst] == -1) {
                    type[idx] = 1;
                } else if (f[src] < f[dst]) {
                    //type[idx] = 2;
                } else if (inq[dst]) {
                    type[idx] = 3;
                } else {
                    type[idx] = 4;
                }
                dfs(edges[idx].dst, src);
            }
        }
                      
        inq[src] = 0;
    }
};

struct BOJ14866 {
    int N, M;
    BOJ14866() {
        cin >> N >> M;
        DFSSpanningTree graph = N+1;
        for (int i = 0; i < M; ++i) {
            int u, v; cin >> u >> v;
            graph.push(u, v);
            graph.push(v, u);
        }
        graph.dfs_spanning_tree(1);
    }
};

int main()
{
//    BOJ14866 p;
    
    
//    scanf("%d%d", &N, &M);
//    for (int i=1;i<=M;i++){
//        int a, b; scanf("%d%d", &a, &b);
//        adj[a].pb(b); adj[b].pb(a);
//    }
//    dep[1] = 1; dfs(1, 0);
//
//    lld ans = 0;
//    for (int i=1;i<=N;i++){
//        bool skip = 0;
//        for (int t: child[i])
//            if (D[t]-F[t] > 1 || U[t]) skip = 1; // 위아래로 사이클 || 아래로 사이클
//        if (skip || M-(N-1)-D[i] != 0) continue; // 위로 사이클
//        ans += i;
//        printf("answer += %d\n", i);
//    }
//    for (int i = 1; i <= N; ++i) {
//        printf("%d :", i);
//        for (int c: child[i]) {
//            printf(" %d", c);
//        } printf("\n");
//    }
//    for (int i = 1; i <= N; ++i) {
//        printf("U[%d] : %d\n", i, U[i]);
//    }
//    for (int i = 1; i <= N; ++i) {
//        printf("D[%d] : %d\n", i, D[i]);
//    }
//    for (int i = 1; i <= N; ++i) {
//        printf("F[%d] : %d\n", i, F[i]);
//    }
//    printf("%lld\n", ans);
}
