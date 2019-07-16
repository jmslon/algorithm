/*
 Farest points pair, Closest points pair
 Graham Scan, Line Sweeping.
 */

#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

typedef long long ll;

class Vector {
public:
    ll x, y;
    Vector(){}
    Vector(ll x_, ll y_) : x(x_), y(y_) {}
    
    bool    operator == (const Vector& a)   const {return x==a.x&&y==a.y;}
    bool    operator <  (const Vector& a)   const {return y==a.y?x<a.x:y<a.y;}
    Vector  operator +  (const Vector& a)   const {return Vector(x+a.x,y+a.y);}
    Vector  operator -  (const Vector& a)   const {return Vector(x-a.x,y-a.y);}
    Vector  operator -  (void)              const {return Vector(-x,-y);}
    Vector  operator *  (const ll a)        const {return Vector(x*a,y*a);}
    
    ll dot      (const Vector& a)   const {return x*a.x+y*a.y;}
    ll cross    (const Vector& a)   const {return x*a.y-y*a.x;}
    double size (void)              const {return sqrt(dot(*this));}
    ll size2    (void)              const {return dot(*this);}
};

struct PointSet {
    vector<Vector> P;
    Vector O;
    int size;
    
    PointSet(int size) : size(size) {
        P.resize(size);
    }
    
    virtual void sort_points() = 0;
};

struct FarestPair : public PointSet {
    vector<Vector> hull;
    vector<Vector> farest_pair;
    
    FarestPair(int size) : PointSet(size) {}
    
    virtual void sort_points() {
        // select (O)rigin and swap P[0]
        swap(P[0], *min_element(P.begin(), P.end()));
        O = P[0];
        
        // sort Points by O (counter clock wise).
        for (int i = 1; i < size; ++i) P[i] = P[i] - O;
        sort(P.begin()+1, P.end(), [] (const Vector &a, const Vector &b) {
            ll cr = a.cross(b);
            if(cr) return cr > 0;
            return a.y + abs(a.x) < b.y + abs(b.x);
        });
        for (int i = 1; i < size; ++i) P[i] = P[i] + O;
    }
    
    void graham_scan() {
        hull = {O};
        for (int i = 1; i < size; ++i) {
            while (1) {
                if (hull.size() <= 1) break;
                Vector back1 = hull.back(), back2 = hull[hull.size()-2];
                if ((back1-back2).cross(P[i]-back2) > 0) break;
                hull.pop_back();
            }
            hull.push_back(P[i]);
        }
    }
    
    void rotating_calipers() {
        Vector A = hull[0], B = hull[1];
        ll max = (A-B).size2();
        for (int i = 0, j = 1; i < hull.size(); ++i) {
            int ni = (i+1) % hull.size();
            Vector NI = hull[ni], I = hull[i], NJ, J;
            while (1) {
                int nj = (j+1) % hull.size();
                NJ = hull[nj]; J = hull[j];
                if ((NI-I).cross(NJ-J) > 0) j = nj;
                else break;
            }
            ll tmp = (I-J).size2();
            if(max < tmp) {max = tmp; A = I; B = J;}
        }
        farest_pair = {A, B};
    }
};

struct ClosestPair : public PointSet {
    vector<Vector> closest_pair;
    
    ClosestPair(int size) : PointSet(size) {}
    
    virtual void sort_points() {
        sort(P.begin(), P.end(), [] (Vector &a, Vector &b) {
            return a.x == b.x ? a.y < b.y : a.x < b.x;
        });
    }
    
    void sweeping() {
        set<Vector> SET = {P[0], P[1]};
        Vector A = P[0], B = P[1];
        ll min = (A-B).size2();
        for (int i = 2, j = 0; i < size; ++i) {
            Vector I = P[i];
            for (; j < i; ++j) {
                Vector J = P[j];
                if ((I-J).x * (I-J).x < min) break;
                SET.erase(J);
            }
            
            int d = sqrt(min) + 1;
            Vector D = {d, d};
            auto begin = SET.lower_bound(I-D);
            auto end = SET.upper_bound(I+D);
            
            for (auto it = begin; it != end; ++it) {
                Vector J = *it;
                ll tmp = (I-J).size2();
                if (tmp < min) {min = tmp; A = I; B = J;}
            }
            SET.insert(I);
        }
        closest_pair = {A, B};
    }
};
