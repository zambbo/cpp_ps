#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <tuple>

#define MAX_N 10
#define MAX_M 10

#define MAX_LEN 10

#define ISLAND_N 6

using namespace std;

int N;
int M;

int dy[4] = {-1, 1, 0, 0};
int dx[4] = {0, 0, -1, 1};

int island_num;

int island_weights[ISLAND_N+1][ISLAND_N+1] = {
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
	{MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN, MAX_LEN},
};

int parent[ISLAND_N+1];

vector<tuple<int, int, int>> weight_vec;

int map[MAX_N][MAX_M];
bool visited[MAX_N][MAX_M] = {false};

int bridge_min = 0;

typedef pair<int, int> pos;

bool bfs(int y, int x, int c) {
	if (map[y][x] == 0 || visited[y][x]) return false;
	
	visited[y][x] = true;
	map[y][x] = c;
	queue<pos> q;

	q.push({y, x});

	while(!q.empty()) {
		pos p = q.front(); q.pop();

		int c_y = p.first;
		int c_x = p.second;
		
		for (int i=0; i<4; i++) {
			int next_y = c_y + dy[i];
			int next_x = c_x + dx[i];

			if (next_y < 0 || next_x < 0 || next_y >= N || next_x >= M || visited[next_y][next_x] || map[next_y][next_x] == 0) continue;
			
			map[next_y][next_x] = c;
			visited[next_y][next_x] = true;
			q.push({next_y, next_x});
		}
	}

	return true;
}

void make_islands() {
	int num = 1;

	for (int i=0; i<N; i++) {
		for (int j=0; j<M; j++) {
			if(bfs(i, j, num)) num += 1;
		}
	}

	island_num = num - 1;
}

void find_island_short_path(int y, int x, int snum) {
	for (int i=0; i<4; i++) {
		int next_y = y + dy[i];
		int next_x = x + dx[i];
		int bridge_len = 0;
		while (next_y >= 0 && next_x >=0 && next_x <M && next_y <N) {
			int dnum = map[next_y][next_x];
			if (map[next_y][next_x] == snum) {
				break;
			} else if (map[next_y][next_x] == 0) {
				bridge_len++;
				next_y += dy[i];
				next_x += dx[i];
				continue;
			} else {
				if (bridge_len <= 1) break;
				island_weights[snum][dnum] = min(bridge_len, island_weights[snum][dnum]);
				island_weights[dnum][snum] = min(bridge_len, island_weights[dnum][snum]);
				break;
			}	
		}
	}
}

bool compare(tuple<int, int, int> t1, tuple<int, int, int> t2) {
	return get<2>(t1) < get<2>(t2);
}

void make_island_weights() {
	for (int i=0; i<N; i++) {
		for (int j=0; j<M; j++) {
			find_island_short_path(i, j, map[i][j]);	
		}
	}

	for (int i=1; i<island_num + 1; i++) {
		for (int j=i+1; j<island_num +1; j++) {
			tuple<int, int, int> row = make_tuple(i, j, island_weights[i][j]);
			if (island_weights[i][j] != MAX_LEN)
				weight_vec.push_back(row);
		}
	}
	sort(weight_vec.begin(), weight_vec.end(), compare);
}

int find_root(int x) {
	if (x == parent[x]) return x;

	parent[x] = find_root(parent[x]);
	return parent[x];
}

void uni(int x, int y) {
	x = find_root(x);
	y = find_root(y);
	parent[x] = y;
}

void cal_bridge_min() {
	int cnt = 0;
	for (int i=1; i < island_num + 1; i++) parent[i] = i;

	
	for (auto elem : weight_vec) {
		int temp1 = find_root(get<0>(elem));
		int temp2 = find_root(get<1>(elem));

		if (temp1 == temp2) continue;

		uni(temp1, temp2);
	
		cnt++;
		bridge_min += get<2>(elem);		
	}		

	if (cnt != island_num - 1) bridge_min = -1;
}


int main() {
	cin >> N >> M;
	
	for (int i=0; i<N ;i++) {
		for (int j=0; j<M; j++) {
			cin >> map[i][j];
			if (map[i][j] == 1) map[i][j] = -1;
		}
	}

	make_islands();
	
	make_island_weights();
	cal_bridge_min();
	cout << bridge_min << endl;
	return 0;
}
