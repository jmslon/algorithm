#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

struct DisjointSet{
    int size;
    vector<int> par;
    
    DisjointSet(int size) {
        this->size = size;
        par.resize(size);
        for(int i = 0; i < size ; ++i) par[i] = i;
    }
    
    ll find(int x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }
    
    void join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (x < y) swap(x, y);
        par[x] = y;
    }
};
