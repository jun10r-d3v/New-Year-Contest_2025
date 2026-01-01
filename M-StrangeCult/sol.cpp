#include <bits/stdc++.h>
using namespace std;

struct stc {
    int id, x, y, l;
};

double dist(double x1, double y1, double x2, double y2) {
    double dx = abs(x1 - x2);
    double dy = abs(y1 - y2);
    return sqrt((dx * dx) + (dy * dy));
}

double dist2(double x1, double y1, double x2, double y2) {
    double dx = abs(x1 - x2);
    double dy = abs(y1 - y2);
    return ((dx * dx) + (dy * dy));
}

int n, m;
vector<double> lvl;
vector<stc> people;
vector<int> head;
vector<pair<double, pair<int, int>>> v;

int find(int st) {
    if (head[st] == st) return st;
    return head[st] = find(head[st]);
}

double mst() {
    sort(v.begin(), v.end());
    double ans = 0;
    int cnt = 0;

    for (auto tmp : v) {
        double w = tmp.first;
        int x = tmp.second.first;
        int y = tmp.second.second;

        if (find(x) != find(y)) {
            head[find(x)] = find(y);
            ans += w;
            cnt++;
        }
    }

    if (cnt == n - 1) {
        cout << fixed << setprecision(0) << ans << "\n";
    } else {
        cout << "Error\n";
    }

    return ans;
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m;
    lvl.resize(m + 1);
    head.resize(n + 1);

    for (int i = 0; i < m; i++) cin >> lvl[i];
    for (int i = 0; i < n; i++) {
        int x, y, l;
        cin >> x >> y >> l;
        people.push_back({i, x, y, l});
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (people[i].l - 1 == people[j].l) {
                double distt = dist(people[i].x, people[i].y, people[j].x, people[j].y);
                if (distt <= lvl[people[i].l - 1]) {
                    double dist22 = dist2(people[i].x, people[i].y, people[j].x, people[j].y);
                    v.push_back({dist22, {people[i].id, people[j].id}});
                    // cout << i+1 << " " << j+1 << " " << distt << "\n";
                }
            }
        }
    }

    for (int i = 0; i < n; i++) head[i] = i;
    mst();

    return 0;
}
