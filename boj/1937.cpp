
struct BOJ1937 {
    BOJ1937() {
        int N; cin >> N;
        vector<vector<int>> map(N, vector<int>(N));
        vector<vector<int>> dp(N, vector<int>(N, 1));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cin >> map[i][j];
            }
        }
        
        vector<vector<bool>> visit(N, vector<bool>(N));
        for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) {
            dfs(i, j, map, dp, visit);
        }
        
        int answer = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                answer = max(answer, dp[i][j]);
            }
        }
        cout << answer << endl;
    }
    void dfs(int i, int j, vector<vector<int>> &map, vector<vector<int>> &dp, vector<vector<bool>> &visit) {
        if (visit[i][j]) return;
        visit[i][j] = 1;
        int N = (int) map.size();
        if (i != 0   && map[i-1][j] < map[i][j]) {
            dfs(i-1, j, map, dp, visit);
            dp[i][j] = max(dp[i][j], dp[i-1][j] + 1);
        }
        if (i != N-1 && map[i+1][j] < map[i][j]) {
            dfs(i+1, j, map, dp, visit);
            dp[i][j] = max(dp[i][j], dp[i+1][j] + 1);
        }
        if (j != 0   && map[i][j-1] < map[i][j]) {
            dfs(i, j-1, map, dp, visit);
            dp[i][j] = max(dp[i][j], dp[i][j-1] + 1);
        }
        if (j != N-1 && map[i][j+1] < map[i][j]) {
            dfs(i, j+1, map, dp, visit);
            dp[i][j] = max(dp[i][j], dp[i][j+1] + 1);
        }
    }
};
