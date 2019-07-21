#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

struct DisjointSet{
    ll size;
    vector<ll> par;
    
    DisjointSet(ll size) {
        this->size = size;
        par.resize(size+1);
        for(ll i = 0; i <= size ; ++i) par[i] = i;
    }
    
    ll find(ll x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }
    
    void join(ll x, ll y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (x < y) swap(x, y);
        par[x] = y;
    }
};
