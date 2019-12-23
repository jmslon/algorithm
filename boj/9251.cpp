//
//  Created by James on 2019/12/20.
//  Copyright © 2019 James. All rights reserved.
//  LCS Dynamic Programming Version.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct BOJ9251 {
    BOJ9251() {
        string str1, str2; cin >> str1 >> str2;
        int len1 = (int) str1.size(), len2 = (int) str2.size();
        vector<vector<int>> dp(len1, vector<int>(len2, 0));
        if (str1[0] == str2[0]) dp[0][0] = 1;
        for (int i = 1 ; i < len1; ++i) {
            if (str1[i] == str2[0]) {
                dp[i][0] = 1;
            } else {
                dp[i][0] = dp[i-1][0];
            }
        }
        for (int j = 1; j < len2; ++j) {
            if (str1[0] == str2[j]) {
                dp[0][j] = 1;
            } else {
                dp[0][j] = dp[0][j-1];
            }
        }
        
        for (int i = 1; i < len1; ++i) for (int j = 1; j < len2; ++j) {
            if (str1[i] == str2[j]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
        cout << dp.back().back() << endl;
    }
};
