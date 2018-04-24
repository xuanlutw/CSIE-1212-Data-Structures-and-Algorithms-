#include <cstdio>

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    long sum_p[n + 1];
    sum_p[0] = 0;
    for (int i = 0;i < n;++i) {
        long tmp;
        scanf("%dl", &tmp);
        sum_p[i + 1] = sum_p[i] + tmp;
    }
    for (int i = 0;i < m;++i) {
        int s1, s2;
        scanf("%d%d", &s1, &s2);
        printf("%ld\n", sum_p[s2] - sum_p[s1 - 1]);
    }
    return 0;
}
