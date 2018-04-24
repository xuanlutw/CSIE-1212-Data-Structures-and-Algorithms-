#include <cstdio>
#include <cstdlib>
#include <cstring>

int is_no_cycle(int now, int n, char** map, char* done) {
    int count = 0;
    for (int i = 0;i < n;++i)
        if (map[now][i] && done[i])
            ++count;
    if (count > 1) return 0;
    for (int i = 0;i < n;++i)
        if (map[now][i] && !done[i]) {
            done[i] = 1;
            if (!is_no_cycle(i, n, map,done)) return 0;
        }
    return 1;
}

int is_connect(int n, char** map) {
    char flood[n];
    memset(flood, 0, sizeof(char) * n);
    flood[0] = 1;
    int fg = 1;
    while (fg) {
        fg = 0;
        for (int i = 0;i < n;++i) {
            if (!flood[i]) continue;
            for (int j = 0;j < n;++j) {
                if (map[i][j] && !flood[j]) {
                    flood[j] = 1;
                    fg = 1;
                }
            }
        }
    }
    for (int i = 0;i < n;++i)
        if (flood[i] == 0)
            return 0;
    return 1;
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    char** map = (char**)malloc(sizeof(char*) * n);
    char* done = (char*)malloc(sizeof(char) * n);
    for (int i = 0;i < n;++i) {
        done[i] = 0;
        map[i] = (char*)malloc(sizeof(char) * n);
        memset(map[i], 0, sizeof(char) * n);
    }
    for (int i = 0;i < m;++i) {
        int s1, s2;
        scanf("%d%d", &s1, &s2);
        --s1;
        --s2;
        map[s1][s2] = map[s2][s1] = 1;
    }
    if (is_connect(n, map) && is_no_cycle(0, n, map, done)) printf("yes");
    else printf("no");
    return 0;
}
