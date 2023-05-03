#include<iostream>
#include<algorithm>
#include<limits>
#include<cstring>

using namespace std;
#define MAX_N 501


int T, K;

int sum[MAX_N];

int dp[MAX_N][MAX_N];

int main() {
	int c;
	cin >> T;

	for (int i=0; i<T; i++) {
		cin >> K;

		memset(sum, 0, sizeof(sum));
		memset(dp, 0, sizeof(dp));
	
		for (int i=1; i<K+1; i++) {
			cin >> c;
			sum[i] = sum[i-1] + c;
		}

		for (int r=1; r<K; r++) {
			for (int s=0; s+r<K; s++) {
				int e = s + r;
				
				int psum = sum[e+1] - sum[s];
				
				dp[s][e] = numeric_limits<int>::max();
				for (int k=s; k<e; k++) {
					dp[s][e] = min(dp[s][e], dp[s][k] + dp[k+1][e] + psum);
				}
				
			}
		}

		cout << dp[0][K-1] << endl;

	}
	return 0;
}
