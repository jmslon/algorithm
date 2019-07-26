#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <unordered_map>

#define GRID_SIZE 10000

using namespace std;

typedef int ll;

class Vector {
public:
    ll x, y;
    Vector(){}
    Vector(ll x_, ll y_) : x(x_), y(y_) {}
    
    bool    operator == (const Vector& a)   const {return x==a.x&&y==a.y;}
    bool    operator <  (const Vector& a)   const {return y==a.y?x<a.x:y<a.y;}
    Vector  operator +  (const Vector& a)   const {return {x+a.x,y+a.y};}
    Vector  operator -  (const Vector& a)   const {return {x-a.x,y-a.y};}
    Vector  operator -  (void)              const {return {-x,-y};}
    Vector  operator *  (const ll a)        const {return {x*a,y*a};}
    
    ll dot      (const Vector& a)   const {return x*a.x+y*a.y;}
    ll cross    (const Vector& a)   const {return x*a.y-y*a.x;}
    double size (void)              const {return sqrt(dot(*this));}
    ll size2    (void)              const {return dot(*this);}
};

struct PointSet{
    vector<Vector> P;
    vector<set<ll> > Q;
    
    PointSet(int size) {
        P.reserve(size);
        Q.resize(1+(GRID_SIZE<<1), set<ll>());
    }
    
    int count(Vector p) {return (int) Q[p.x].count(p.y);}
    int count(int x, int y) {return (int) Q[x].count(y);}
    
    int max_square() {
        int ret = 0;
        for (int i = 0; i < P.size(); ++i) {
            auto A = P[i];
            for (int j = i+1; j < P.size(); ++j) {
                auto B = P[j];
                auto tmp = A - B;
                tmp = {tmp.y, -tmp.x};
                auto C = A + tmp;
                auto D = B + tmp;
                if (C.x<0||D.x<0||C.x>(GRID_SIZE<<1)||D.x>(GRID_SIZE<<1)) continue;
                if (count(C) && count(D))
                    ret = max(ret, (A-B).size2());
            }
        }
        return ret;
    }
    
};

int main() {
    int T;
    scanf("%d", &T);
    for (;T--;) {
        int N;
        scanf("%d", &N);
        PointSet ps(N);
        for (int i = 0; i < N; ++i) {
            ll x, y;
            scanf("%d%d", &x, &y);
            x += GRID_SIZE;
            ps.P.push_back({x, y});
            ps.Q[x].insert(y);
        }
        printf("%d\n", ps.max_square());
    }
    return 0;
}
