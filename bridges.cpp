struct edge {
	int to, num;
};
 
int tin[maxn], tup[maxn];
bool used[maxn];
vector<edge>g[maxn];
int timer = 0;
bool is_bridge[maxn];
 
void dfs(int u, int p = -1) {
	tin[u] = timer++;
	used[u] = 1;
	tup[u] = tin[u];
	for (auto e : g[u]) {
		if (e.to == p) continue;
		if (used[e.to]) tup[u] = min(tup[u], tin[e.to]);
		else {
			dfs(e.to, u);
			tup[u] = min(tup[u], tup[e.to]);
			if (tup[e.to] > tin[u]) is_bridge[e.num] = 1;
		}
	}
}