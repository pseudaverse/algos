int par[maxn], rang[maxn];
void dsu_make(int u) {
	par[u] = u, rang[u] = 1;
}
 
int dsu_get(int u) {
	return (par[u] == u ? u : par[u] = dsu_get(par[u]));
}
 
void dsu_unite(int u, int v) {
	u = dsu_get(u), v = dsu_get(v);
	if (u == v) return;
	if (rang[u] < rang[v]) swap(u, v);
	rang[u] += rang[v];
	par[v] = u;
}