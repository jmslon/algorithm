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
        int curr, prev;
        int tmp; cin >> tmp;
        int answer = tmp;
        prev = tmp;
        for (int i = 1; i < N; ++i) {
            cin >> tmp;
            curr = max(0, prev) + tmp;
            answer = max(answer, curr);
            prev = curr;
        }
        cout << answer << endl;
    }
};
