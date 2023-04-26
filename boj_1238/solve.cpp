#include <cstdio>
#include <vector>
#include <queue>
#include <limits>


#define fi first
#define se second

#define MAX_V 1001

typedef int value;

int N, M, X;
int MAX;

int dist_from_x[MAX_V][2];

using namespace std;

vector<pair<int, value>> edges_from_x[MAX_V][2];

struct cmp {
	bool operator()(pair<int, value> a, pair<int, value> b) {
		return a.se > b.se;
	}
};

void dijkstra_from_x(int type) {
	bool visited[N+1];
	
	for (int i=1; i< N+1; i++){
		visited[i] = false;
		dist_from_x[i][type] = numeric_limits<int>::max();
	}

	dist_from_x[X][type] = 0;

	priority_queue<pair<int, value>, vector<pair<int, value>>, cmp> pq;
	
	pq.push(make_pair(X, 0));

	for(;;) {
		if (pq.empty()) break;

		pair<int, value> x = pq.top(); pq.pop();
	
		if (visited[x.fi]) continue;
	
		visited[x.fi] = true;
	
		vector<pair<int, value>>::iterator iter;

		for (iter=edges_from_x[x.fi][type].begin(); iter!= edges_from_x[x.fi][type].end(); iter++) {
			int idx = iter->fi;
			value w = iter->se;
			
			if (!visited[idx] && x.se + w < dist_from_x[idx][type]) {
				dist_from_x[idx][type] = x.se + w;
				pq.push(make_pair(idx, x.se + w));
			}
		}	
	}	
}

// 핵심 아이디어 : X에서 다른 모든 정점으로 출발하는 다익스트라 하나 ( X에서 모든 정점으로의 최소 거리)
// 다른 정점에서 X로 가는 다익스트라 하나 (단방향 그래프를 2개 만든다) ( 모든 정점에서 X로 가는 최소거리)
int main() {
	scanf("%d %d %d", &N, &M, &X);

	for(int i=0; i<M; i++) {
		int src, dst, weight;
		
		scanf("%d %d %d", &src, &dst, &weight);
		edges_from_x[src][0].push_back(make_pair(dst, weight));
		edges_from_x[dst][1].push_back(make_pair(src, weight));
	}

	dijkstra_from_x(0);
	dijkstra_from_x(1);
	for (int i=1; i<N+1; i++) {
		int cur_dist = dist_from_x[i][0] + dist_from_x[i][1];
		if ( cur_dist > MAX) MAX = cur_dist;
	}
	printf("%d\n", MAX);
}
