//
//  Created by James on 2019/12/20.
//  Copyright © 2019 James. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct BOJ1912 {
    BOJ1912() {
        int N; cin >> N;
        vector<int> arr(N);
        for (int i = 0; i < N; ++i) {
            cin >> arr[i];
        }
        vector<int> dp(N);
        dp[0] = arr[0];
        int answer = dp[0];
        for (int i = 1; i < N; ++i) {
            dp[i] = max(0, dp[i-1]) + arr[i];
            answer = max(answer, dp[i]);
        }
        cout << answer << endl;
    }
};

int main(int argc, const char * argv[]) {
    
    BOJ1912 p;
    
    return 0;
}
