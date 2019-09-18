#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef long long ll;

struct MaxSegmentTree { // Max Lazy
   vector<ll> arr, tree, lazy;
   MaxSegmentTree(int size) {
       arr.resize(size);
       tree.resize(size<<2);
       lazy.resize(size<<2);
   }

   ll update(int node, int begin, int end, int l_pos, int r_pos, ll dif) {
       propagate(node, begin, end, lazy[node]);
       lazy[node] = 0;
       if (r_pos < begin || end < l_pos) return tree[node];
       if (l_pos <= begin && end <= r_pos) {
           propagate(node, begin, end, dif);
           return tree[node];
       }
       ll l = update(l_node, begin, mid, l_pos, r_pos, dif);
       ll r = update(r_node, mid+1, end, l_pos, r_pos, dif);
       return tree[node] = l>r?l:r;
   }

   ll query(int node, int begin, int end, int l_pos, int r_pos) {
       propagate(node, begin, end, lazy[node]);
       lazy[node] = 0;
       if (r_pos < begin || end < l_pos) return 0;
       if (l_pos <= begin && end <= r_pos) return tree[node];
       ll l = query(l_node, begin, mid, l_pos, r_pos);
       ll r = query(r_node, mid+1, end, l_pos, r_pos);
       return l>r?l:r;
   }
   
private:
   void propagate(int node, int begin, int end, ll dif) {
       if (dif == 0) return;
       tree[node] += dif;
       if (begin ^ end) {
           lazy[l_node] += dif;
           lazy[r_node] += dif;
       }
   }
};

struct SumSegmentTree { // Sum Lazy
    vector<ll> arr, tree, lazy;
    SumSegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        ll l = init(l_node, begin, mid);
        ll r = init(r_node, mid+1, end);
        return tree[node] = l+r;
    }
    
    ll update(int node, int begin, int end, int l_pos, int r_pos, ll dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l = update(l_node, begin, mid, l_pos, r_pos, dif);
        ll r = update(r_node, mid+1, end, l_pos, r_pos, dif);
        return tree[node] = l+r;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l+r;
    }
   
private:
    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += (ll)(end-begin+1)*dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
};

struct BOJ10999 {
    BOJ10999() {
        int N, M, K; cin >> N >> M >> K;
        SumSegmentTree sum_tree(N+1);
        for (int i = 1; i <= N; ++i)
            cin >> sum_tree.arr[i];
        sum_tree.init(1, 1, N);
        for (int i = 0; i < M+K; ++i) {
            int a; cin >> a;
            if (a == 1) {
                int b, c; ll d; cin >> b >> c >> d;
                sum_tree.update(1, 1, N, b, c, d);
            } else if (a == 2) {
                int b, c; cin >> b >> c;
                cout << sum_tree.query(1, 1, N, b, c) << "\n";
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ10999 p;
    return 0;
}

