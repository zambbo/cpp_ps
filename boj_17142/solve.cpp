#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

#define y first
#define x second

typedef pair<int, int> pos;

const int MAX_N = 52;
const int dy[4] = {-1, 1, 0, 0};
const int dx[4] = {0, 0, -1, 1};

int N, M;
int lab[MAX_N][MAX_N];
vector<pos> viruses;

bool is_inlab(int y, int x) {
    return y >= 1 && y <= N && x >= 1 && x <= N;
}

int bfs(const vector<pos>& active_viruses) {
    bool visited[MAX_N][MAX_N] = {false};
    queue<pair<pos, int>> q;

    for (const pos& p : active_viruses) {
        visited[p.y][p.second] = 1;
        q.push({p, 0});
    }

    int time = 0;

    while (!q.empty()) {
        pos cur = q.front().first;
        int cur_time = q.front().second;
        q.pop();

        for (int d=0; d<4; d++) {
            int ny = cur.y + dy[d];
            int nx = cur.x + dx[d];

            if (is_inlab(ny, nx) && !visited[ny][nx] && lab[ny][nx] != 1) {
                visited[ny][nx] = true;
                q.push({{ny, nx}, cur_time + 1});
                if (lab[ny][nx] == 0) {
                    time = max(time, cur_time + 1);
                }
            }
        }
    }

    for (int y=1; y<=N; y++) {
        for (int x=1; x<=N; x++) {
            if (lab[y][x] == 0 && !visited[y][x]) {
                return -1;
            }
        }
    }

    return time;
}

int solve() {
    int min_time = -1;
    vector<bool> selector(viruses.size(), false);
    fill(selector.begin(), selector.begin() + M, true);

    do {
        vector<pos> active_viruses;

        for (int i=0; i<viruses.size(); i++) {
            if (selector[i]) {
                active_viruses.push_back(viruses[i]);
            }
        }

        int temp_min_time = bfs(active_viruses);

        if (temp_min_time != -1) {
            if (min_time == -1) {
                min_time = temp_min_time;
            } else {
                min_time = min(min_time, temp_min_time);
            }
        }
    } while (prev_permutation(selector.begin(), selector.end()));

    return min_time;
}

int main() {
    cin >> N >> M;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> lab[i][j];

            if (lab[i][j] == 2) {
                viruses.push_back({i, j});
            }
        }
    }

    cout << solve() << endl;

    return 0;
}

