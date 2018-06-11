#include <cstdio>
#include <cstring>
#define L 100010
#define M 1000000007

typedef struct {
    int num;
    int len;
    int cnt;
    int pre;
} Item;

int dp_item(int index, Item* item) {
    int max = 0;
    int pre = -1;
    int cnt = 0;
    for (int i = 0;i < index;++i) {
        if (item[i].num <= item[index].num && item[i].len > max) {
            max = item[i].len;
            pre = i;
            cnt = item[i].cnt;
        }
        else if (item[i].num <= item[index].num && item[i].len == max) {
            cnt = (cnt + item[i].cnt) % M;
        }
    }
    item[index].len = max + 1;
    item[index].pre = pre;
    item[index].cnt = max ? cnt : 1;
    return item[index].len;
}

int dp(int n, Item* item) {
    int max = 0, ret;
    for (int i = 0;i < n;++i) {
        ret = dp_item(i, item);
        if (ret > max)
            max = ret;
    }
    return max;
}

void print_seq(int now, Item* item) {
    if (item[now].pre == -1)
        printf("%d", now + 1);
    else {
        print_seq(item[now].pre, item);
        printf(" %d", now + 1);
    }
    return;
}

int main() {
    int n, len, cnt = 0, pre = -1;
    Item item[L];
    memset(item, 0, L * sizeof(Item));
    scanf("%d", &n);
    for (int i = 0;i < n;++i)
        scanf("%d", &(item[i].num));
    len = dp(n, item);
    for (int i = 0;i < n;++i) {
        if (item[i].len == len)
            cnt = (cnt + item[i].cnt) % M;
        if (item[i].len == len && pre == -1)
            pre = i;
    }
    printf("%d\n%d\n", len, cnt);
    print_seq(pre, item);
    return 0;
}
