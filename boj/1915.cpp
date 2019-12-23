
struct BOJ1915 {
    BOJ1915() {
        int N, M; cin >> N >> M;
        vector<vector<bool>> map(N, vector<bool>(M));
        for (int i = 0; i < N; ++i) {
            string cmd; cin >> cmd;
            for (int j = 0; j < M; ++j) {
                map[i][j] = cmd[j] - '0';
            }
        }
        
        vector<vector<int>> dp(N, vector<int>(M, 0));
        for (int i = 0; i < N; ++i) for (int j = 0; j < M; ++j) {
            if (map[i][j]) dp[i][j] = 1;
        }
        
        for (int i = 1; i < N; ++i) for (int j = 1; j < M; ++j) {
            if (map[i][j] == 1) {
                int len = dp[i-1][j-1];
                len = min(len, dp[i-1][j]);
                len = min(len, dp[i][j-1]);
                dp[i][j] = len + 1;
            }
        }
        
        int answer = 0;
        for (int i = 0; i < N; ++i) for (int j = 0; j < M; ++j) {
            answer = max(answer, dp[i][j]);
        }
        
        cout << answer * answer << endl;
    }
};
