#include<bits/stdc++.h>
#define F first
#define S second
#include "testlib.h"
using namespace std;
 
using ll = long long;
using pii = pair<int, int>;
 
/**
 * Union-find Disjoint Set with Path Compression
 */

void dist(int x1,int x2,int y1,int y2){
    double dx = x1-x2;
    double dy = y1-y2;
    printf("%.6f",sqrt((dx*dx) + (dy*dy)));
    return ;
}
class disjoint_set {
    private:
    int size;
    int merge_count;
    vector<int> parent;
    public:
    disjoint_set(int n) : size(n), parent(n) {
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }
    int count_component() {
        return size-merge_count;
    }
    int root(int u) {
        if (parent[u] == u) return u;
        return parent[u] = root(parent[u]);
    }
    bool merge(int u, int v) {
        u = root(u), v = root(v);
        if (u == v) return false;
        parent[v] = u;
        ++merge_count;
        return true;
    }
};
 
/**
 * ==============================================
 * Distribution functions
 * ==============================================
 */
 
/**
 * Return k random elements from set {0, 1, 2, ..., n-1}.
 * The output is not sorted in any ways.
 */
template<typename T>
vector<T> pick_set(T n, int k)
 {
    ensure(k >= 0 && k <= n);
    unordered_set<T> used;
    vector<T> ret;
    ret.reserve(k);
    for (T r = n-k; r < n; ++r) {
        T v = rnd.next((T)0, r);
        if (used.count(v))
            v = r;
        used.insert(v);
        ret.push_back(v);
    }
    return ret;
}
 
/**
 * Return k random elements from set {min_n, min_n+1, ..., max_n-1, max_n}
 * The output is not sorted in any ways.
 */
template<typename T>
vector<T> pick_set(T min_n, T max_n, int k)
 {
    ensure(min_n <= max_n && k >= 0 && k <= max_n-min_n+1);
    T sz = max_n-min_n+1;
    vector<T> s = pick_set(sz, k);
    for (int i = 0; i < k; ++i)
        s[i] += min_n;
    return s;
}
 
/**
 * ==============================================
 * Graph generation functions
 * ==============================================
 */
 
/**
 * Get corresponding undirected edge to an index
 */
pii get_undirected_edge(ll n, ll m, ll x)
{
    ll a = 1;
    ll b = -2*n + 1;
    ll c = 2*x;
    ll u = floor((-b - sqrt(b*b - 4*a*c) ) / 2*a);
    ll fx = u*n - u*(u+1)/2;
    ll v = (u+1) + (x-fx);
    return pii(u, v);
}
 
/**
 * Generate edge list of random simple undirected graph with n vertices and m edges
 * Vertices are indexed from 0 to n-1
 */
vector<pii> gen_undirected_graph(int n, int m)
{
    ensure(m >= 0 && m <= n*1LL*(n-1)/2);
    vector<ll> s = pick_set(n*1LL*(n-1)/2, m);
    shuffle(s.begin(), s.end());
    vector<pii> edges(m);
    for (int i = 0; i < m; ++i) {
        edges[i] = get_undirected_edge(n, m, s[i]);
        if (rnd.next(2))
            swap(edges[i].first, edges[i].second);
    }
    shuffle(edges.begin(), edges.end());
    return edges;
}
 
/**
 * Generate parent list of a tree with n vertices, rooted at 0.
 * Vertices are indexed from 0 to n-1. Root will have parent -1.
 * For all 0 <= i < j <= n-1, depth of vertex i is less than depth of vertex j.
 * min_k, max_k will determine the result of k in each iteration of generation algorithm.
 * If k is small, tree will be quite deep. If k is large, tree will be quite shallow.
 */
vector<int> gen_min_vertex_tree(int n, int min_k, int max_k)
{
    ensure(min_k <= max_k && min_k >= 1 && max_k <= n);
    vector<int> parents(n);
    parents[0] = -1;
    for (int i = 1; i < n; ++i) {
        int k;
        if (i < min_k) k = rnd.next(1, i);
        else if (i < max_k) k = rnd.next(min_k, i);
        else k = rnd.next(min_k, max_k);
        parents[i] = i-k;
    }
    return parents;
}
 
/**
 * Like above, but there are no conditions on depth of vertex
 * and the tree will be rooted at vertex r.
 * If r = -1, r will be chosen at random.
 */
vector<int> gen_parent_tree(int n, int min_k, int max_k, int r=-1)
{
    if (r == -1) r = rnd.next(n);
    vector<int> parents = gen_min_vertex_tree(n, min_k, max_k);
 
    vector<int> new_label = pick_set(n, n);
    shuffle(new_label.begin(), new_label.end());
    vector<int>::iterator it = find(new_label.begin(), new_label.end(), r);
    swap(*new_label.begin(), *it);
 
    vector<int> new_parents(n);
    for (int i = 0; i < n; ++i)
        new_parents[new_label[i]] = parents[i] == -1 ? -1 : new_label[parents[i]];
 
    return new_parents;
}
 
/**
 * Generate edge list of a tree with n vertices, rooted at r
 * Vertices are numbered from 0 to n-1
 * min_k, max_k will determine the result of k in each iteration of generation algorithm.
 * If k is small, tree will be quite deep. If k is large, tree will be quite shallow
 */
vector<pii> gen_edge_tree(int n, int min_k, int max_k)
{
    vector<int> parents = gen_parent_tree(n, min_k, max_k);
    vector<pii> edges;
    edges.reserve(n-1);
    for (int i = 0; i < n; ++i) {
        if (parents[i] != -1)
            edges.push_back(pii(i, parents[i]));
    }
    shuffle(edges.begin(), edges.end());
    return edges;
}
 
/**
 * Generate edge list of a connected undirected graph
 * Vertices are numbered from 0 to n-1
 */
vector<pii> gen_connected_graph(int n, int m)
{
    ensure(m >= n-1 && m <= n*1LL*(n-1)/2);
    ll max_m = n*1LL*(n-1)/2;
    vector<ll> s = pick_set(max_m, min<ll>(m+(n-1), max_m));
    shuffle(s.begin(), s.end());
 
    set<pii> edges;
    vector<pii> spnt = gen_edge_tree(n, 1, n);
    for (pii e : spnt)
        edges.insert(minmax(e.first, e.second));
 
    while (edges.size() < m) {
        ll x = s.back(); s.pop_back();
        pii e = get_undirected_edge(n, m, x);
        if (!edges.count(e))
            edges.insert(e);
    }
 
    vector<pii> ret;
    for (auto e : edges) {
        if (rnd.next(2))
            swap(e.first, e.second);
        ret.push_back(e);
    }
    shuffle(ret.begin(), ret.end());
    return ret;
}
 
/**
 * Get corresponding directed edge to an index
 */
pii get_directed_edge(int n, int m, ll x)
{
    ll u = x/(n-1);
    ll d = x - u*(n-1);
    ll v = d + (d>=u ? 1 : 0);
    return pii(u, v);
}
 
/**
 * Generate edge list of random directed graph with n vertices and m edges
 * Vertices are indexed from 0 to n-1
 */
vector<pii> gen_directed_graph(int n, int m)
{
    ensure(m >= 0 && m <= n*1LL*(n-1));
    vector<ll> s = pick_set(n*1LL*(n-1), m);
    shuffle(s.begin(), s.end());
    vector<pii> edges(m);
    for (int i = 0; i < m; ++i) {
        edges[i] = get_directed_edge(n, m, s[i]);
    }
    shuffle(edges.begin(), edges.end());
    return edges;
}
 
 
 
const int N = 10000, M = 20000, mod = 1e9+7;
 
bool vis[N+5];
vector<int> g[N+5];
int lvlmx = 0;
int people[N+9];
pair<int,int> point[N+9];
int limit[N+9];
int from[N+9];
//case 1
void bfs(int start_node, vector<vector<pii>>& adj_list,int tmp) {
    queue<pair<int,int>> q;
    vector<bool> visited(adj_list.size(), false);

    q.push({start_node,0});
    visited[start_node] = true;
    // cout << "from\n";
    while (!q.empty()) {
        int u = q.front().first;
        int l = q.front().second;
        q.pop();
        // cout << u + 1 << " " << abs(l-tmp) + 1 << "\n"; // พิมพ์โหนด (1-based index)
        if(tmp != 0){
            people[u] = abs(l-tmp) + 1;
            // cout << u+1 << " " << people[u] << "\n";
        }
        lvlmx = max(lvlmx,l);
        for (auto [v, w] : adj_list[u]) { // [v, w]: โหนดปลายและน้ำหนัก
            if (!visited[v]) {
                visited[v] = true;
                q.push({v,l+1});
            }
        }
    }
}

void rbfs(int start_node, vector<vector<pii>>& adj_list) {
    
    queue<pair<int,int>> q;
    vector<bool> visited(adj_list.size(), false);
    
    q.push({start_node, 0});
    visited[start_node] = true;
    
    while (!q.empty()) {
        int u = q.front().first;
        int l = q.front().second;
        q.pop();
        
        // cout << u + 1 << " "; // พิมพ์โหนด (1-based index)
        lvlmx = max(lvlmx, l);
        
        for (auto [v, w] : adj_list[u]) { // [v, w]: โหนดปลายและน้ำหนัก
            if (!visited[v]) {
                visited[v] = true;
                // from[v] = u + 1; // Store parent node
                // cout << u + 1 << " -> " << v + 1 << "\n";
                from[v+1] = u+1;
                // cout << v+1 << " -> " << from[v+1] << "\n";
                q.push({v, l+1});
            }
        }
    }
    // for(int i=1;i<=7;i++){
    //     cout << i << " " << from[i] << "\n";
    // }
    return ;
}

void lbfs(int start_node, vector<vector<pii>>& adj_list) {
    queue<pair<int,int>> q;
    vector<bool> visited(adj_list.size(), false);

    q.push({start_node,0});
    visited[start_node] = true;

    while (!q.empty()) {
        int u = q.front().first;
        int l = q.front().second;
        q.pop();
        cout << people[u] << " "; // พิมพ์โหนด (1-based index)
        lvlmx = max(lvlmx,l);
        for (auto [v, w] : adj_list[u]) { // [v, w]: โหนดปลายและน้ำหนัก
            if (!visited[v]) {
                visited[v] = true;
                q.push({v,l+1});
            }
        }
    }
}

void setting(int n,int lvlmx){
    if(lvlmx == 0)return ;
    // cout << "this is for setting : \n";
    // cout << "max level : " << lvlmx << "\n";
    // cout << "limit : " << limit[lvlmx] << "\n";
    int tmp = (limit[lvlmx+1]/4);
    // cout << "can range : " << tmp << "\n";
    for(int i=0;i<n;i++){
        if(people[i] == lvlmx){
            // cout << i+1 << " " << from[i+1] <<" ";
            int parent = from[i+1];
            
            int minx = point[parent-1].first - tmp,maxx = point[parent-1].first + tmp;
            int miny = point[parent-1].second - tmp,maxy = point[parent-1].second + tmp;
            int x = rnd.next(minx,maxx);
            int y = rnd.next(miny,maxy);
            // printf("%d [%d] (%d,%d) -> %d [%d] (%d,%d) : dist ",i+1,people[i],x,y,parent,people[parent-1],point[parent-1].first,point[parent-1].second);
            // dist(x,point[parent-1].first,y,point[parent-1].second);
            // printf("\n");
            // cout << x << " " << y << " || " << point[from[i+1]].first << " " << point[from[i+1]].second << "\n";
            point[i] = {x,y};
        }// cout << people[i] << " ";
    }
    // cout << "\n";
    setting(n,lvlmx-1);
    return ;
}

void show(int n){
    cout << "\nparent\n";
    for(int i=1;i<=n;i++){
        printf("%d [%d] -> %d [%d]\n",i,people[i-1],from[i],people[from[i]-1]);
        // cout << i << " " << from[i] << "\n";
    }
    cout << "done\n";
    return ;
}

int main(int argc, char *argv[]){
    registerGen(argc, argv, 1);

    // จำนวนโหนด, เส้นทาง และช่วงของน้ำหนัก
    int n = rnd.next(101, 1000);       // กำหนดจำนวนโหนด (สุ่มจาก 3 ถึง 10)
    int m = rnd.next(n-1, n+1); // กำหนดจำนวนเส้นทาง (สุ่มในช่วงกราฟเชื่อมโยง)
    // int m = rnd.next(n-1, 2*n);
    int min_w = 1, max_w = 100;    // ช่วงของน้ำหนัก (สุ่มระหว่าง 1 ถึง 100)
    int rangemn = 1 , rangemx = 1e7;
    int level_min = 1 , level_max = n;
    // สร้างกราฟแบบ connected graph
    vector<pii> edges = gen_connected_graph(n, m);
    int minx = -1e7, maxx = 1e7,miny = -1e7,maxy = 1e7;
    // สุ่มน้ำหนักและสร้าง adjacency list
    vector<vector<pii>> adj_list(n); // adjacency list ที่มีน้ำหนัก
    for (pii edge : edges) {
        int u = edge.F;
        int v = edge.S;
        // int w = rnd.next(min_w, max_w); // สุ่มน้ำหนัก
        int w = 0;
        // เพิ่มลงใน adjacency list
        adj_list[u].push_back({v, w});
        adj_list[v].push_back({u, w}); // สำหรับกราฟไร้ทิศทาง
    }
    // พิมพ์จำนวนโหนดและเส้นทาง
    // cout << n << " " << m << '\n';

    // เรียก BFS จากโหนดแรก
    // int l = rnd.next(level_min,level_max);
    
    bfs(0, adj_list,0);
    // cout << "\n";
    // cout << "max level : " << lvlmx << "\n";
    bfs(0, adj_list,lvlmx);
    int x = rnd.next(minx,maxx);
    int y = rnd.next(miny,maxy);
    point[0] = {x,y};
    // cout << "adj_list : \n";
    for (pii edge : edges) {
        int u = edge.F + 1, v = edge.S + 1; // แปลงเป็น 1-based index
        int w = rnd.next(min_w, max_w);    // สุ่มน้ำหนักใหม่ (ถ้าต้องการสุ่มอีกครั้ง)
        // cout << u << " " << v << " " << w << " " << people[u-1] << '\n';
        // cout << u << " " << v << " " << w << '\n';
    }
    // show(n);
    // cout << "\n";
    // cout << "BFS Traversal starting from node 1:\n";
    rbfs(0 , adj_list);
    // cout << "\n";
    // for(int i=1;i<=7;i++){
    //     cout << i << " " << from[i] << "\n";
    // }
    // lbfs(0 , adj_list);
    // cout << "\n";
    // cout<<"\n============================\n";
    // cout << "      template testcase     \n";
    // cout << "============================\n";
    cout << n << " " << lvlmx + 1 << "\n";
    for(int i=0;i<=lvlmx;i++){
        int rang = rnd.next(rangemn,rangemx);
        cout << rang;
        if(i != lvlmx)cout << " "; 
        limit[i+1] = rang;
    }cout << "\n";
    // show(n);
    // cout << "=========setting=========\n";
    setting(n,lvlmx);
    for(int i=1;i<=n;i++){
        cout << point[i-1].first << " " << point[i-1].second << " " << people[i-1] << "\n";
    }

    return 0;
}