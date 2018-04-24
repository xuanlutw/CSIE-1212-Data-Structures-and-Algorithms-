#include <cstdio>
#include <queue>
#include <utility>

int main() {
    int n, m;
    long ans = 0;
    std::priority_queue<std::pair<int, int> > salary;
    scanf("%d%d", &n, &m);
    for (int i = 0;i < n;++i) {
        int tmp;
        scanf("%d", &tmp);
        salary.push({tmp, 0});
    }
    for (int i = 0;i < m;++i) {
        int op, tmp;
        scanf("%d%d", &op, &tmp);
        if (op == 1)
            salary.push({tmp, i});
        else {
            while (!salary.empty() && salary.top().first > tmp) {
                ans += salary.top().first * (i - salary.top().second);
                salary.pop();
            }
        }
    }
    while (!salary.empty()) {
        ans += salary.top().first * (m - salary.top().second);
        salary.pop();
    }
    printf("%ld", ans);
    return 0;
}
