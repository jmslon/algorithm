/*
  Farest points pair, Closest points pair
  Convex Hull, Graham Scan, Line Sweeping. 
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
    int x, y;
    Vector(){}
    Vector(int x_, int y_) : x(x_), y(y_) {}
    
    bool    operator == (const Vector& a)   const {return x==a.x&&y==a.y;}
    bool    operator <  (const Vector& a)   const {return y==a.y?x<a.x:y<a.y;}
    Vector  operator +  (const Vector& a)   const {return Vector(x+a.x,y+a.y);}
    Vector  operator -  (const Vector& a)   const {return Vector(x-a.x,y-a.y);}
    Vector  operator -  (void)              const {return Vector(-x,-y);}
    Vector  operator *  (const double a)    const {return Vector(x*a,y*a);}
    
    ll dot      (const Vector& a)   const {return (ll)x*a.x+(ll)y*a.y;}
    ll cross    (const Vector& a)   const {return (ll)x*a.y-(ll)y*a.x;}
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
    vector<Vector> cvx;
    vector<Vector> farest_pair;
    
    FarestPair(int size) : PointSet(size) {}
    
    virtual void sort_points() {
        // select (O)rigin and swap P[0]
        int o_idx = 0;
        for (int i = 1; i < size; ++i)
            if (!(P[o_idx] < P[i]))
                o_idx = i;
        swap(P[0], P[o_idx]);
        O = P[0];
        
        // sort Points by O (counter clock wise).
        for (int i = 1; i < size; ++i)
            P[i] = P[i] - O;
        sort(P.begin()+1, P.end(), [] (const Vector &a, const Vector &b) {
            ll cr = a.cross(b);
            if(cr) return cr > 0;
            return a.y + abs(a.x) < b.y + abs(b.x);
        });
        for (int i = 1; i < size; ++i)
            P[i] = P[i] + O;
    }
    
    void convex_hull() {
        cvx = {O};
        for (int i = 1; i < size; ++i) {
            while (1) {
                if (cvx.size() <= 1) break;
                Vector back1 = cvx.back(), back2 = cvx[cvx.size()-2];
                if ((back1-back2).cross(P[i]-back2) > 0) break;
                cvx.pop_back();
            }
            cvx.push_back(P[i]);
        }
    }
    
    void graham_scan() {
        ll max = 0;
        Vector A, B;
        for (int i = 0, j = 1; i < cvx.size(); ++i) {
            int ni = (i+1) % cvx.size();
            Vector NI = cvx[ni], I = cvx[i], NJ, J;
            while (1) {
                int nj = (j+1) % cvx.size();
                NJ = cvx[nj]; J = cvx[j];
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
    
    void get_closest_pair() {
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
