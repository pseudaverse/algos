int n, m;
vector<pair<int, int>>edges;
vector<int>g[maxn], g_inv[maxn], topsort;
bool used[maxn];
int scc[maxn];
int color = 1;

void dfs(int u) {
	used[u] = 1;
	for (int v : g[u]) {
		if (used[v]) continue;
		dfs(v);
	}
	topsort.push_back(u);
}

void dfs_inv(int u) {
	scc[u] = color;
	for (int v : g_inv[u]) {
		if (scc[v]) continue;
		dfs_inv(v);
	}
}

void scc_go() {
	for (int i = 0; i < m; i++) {
		int a = edges[i].X, b = edges[i].Y;
		g[a].push_back(b);
		g_inv[b].push_back(a);
	}
	for (int u = 1; u <= n; u++) {
		if (!used[u]) dfs(u);
	}
	reverse(all(topsort));
	memset(used, 0, sizeof(used));
	for (int u : topsort) {
		if (!scc[u]) {
			dfs_inv(u); color++;
		}
	}
}