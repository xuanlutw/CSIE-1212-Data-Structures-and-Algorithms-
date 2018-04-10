#include <cstdio>
#include <cstdlib>
#include <cassert>

#define MASK(x) ((x) & (((long)1 << 55) - 1))
#define RESULT(x) (((x) >> 55) & 3) 
#define ROUND(x) (((x) >> 50) & 31) 
#define GET(x, i, j) (((x) >> (((i) * 5 + (j)) * 2)) & 3)
#define GET2(x, i, j) ((GET((x), (i), (j)) == 2)? 0: GET((x), (i), (j)))     // X -> 0
#define COLOR(x) (((ROUND(x) >> 1) & 1) + 1)

#define UP_ROUND(x) ((x) + ((long)2 << 50))
#define SET(x, i, j, c) ((x) + ((c) << (((i) * 5 + (j)) * 2)))

/*
   O 1
   X 2
*/
/*
   Draw 0
   O 1
   X 2
*/
/*
   56-55    54-50   49-0
   result   round   table
*/

typedef struct record{
    u_long table;
    struct record* next;
} Record;

char valid_table(u_long table) {
    assert(ROUND(table) == 22);
    int count = 0, sum;
    for (int i = 0;i < 5;++i) {
        sum = 0;
        for (int j = 0;j < 5;++j) sum += GET2(table, i, j);
        if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
        sum = 0;
        for (int j = 0;j < 5;++j) sum += GET2(table, j, i);
        if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
    }
    sum = 0;
    for (int i = 0;i < 5;++i) sum += GET2(table, i, i);
    if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
    sum = 0;
    for (int i = 0;i < 5;++i) sum += GET2(table, i, 4 - i);
    if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
    
    if (count > 0) return 1;
    if (count < 0) return 2;
    return 0;
}

char value_table(u_long table) {
    if (ROUND(table) == 22) return valid_table(table);
    int result = 0;
    /*
       0 opposite color
       1 draw
       2 same color
    */
    for (int i = 0;i < 25;++i) {
        if (GET(table, i / 5, i % 5)) continue;
        u_long tmp = SET(table, i / 5, i % 5, COLOR(table));
        for (int j = i + 1;j < 25;++j) {
            if (GET(tmp, j / 5, j % 5)) continue;
            u_long next = UP_ROUND(SET(tmp, j / 5, j % 5, COLOR(tmp)));
            int next_result = value_table(next);
            if (COLOR(table) == next_result) {
                result = 2;
                break;
            }
            if (!next_result && !result) result = 1;
        }
    }
    if (result == 2) return COLOR(table);
    if (result == 1) return 0;
    return COLOR(table)? 2: 1;
}

u_long get_table() {
    u_long ans = 0;
    u_long num = 0;
    for (int i = 0;i < 5;++i) {
        for (int j = 0;j < 5;++j) {
            char c = getchar();
            ans <<= 2;
            if (c == 'O') ans += 1;
            if (c == 'X') ans += 2;
            if (c != '.') ++num;
        }
        getchar();
    }
    ans += (num << 50);
    return ans;
}

void put_table(const u_long table) {
    for (int i = 0;i < 5;++i) {
        for (int j = 0;j < 5;++j) {
            if (GET(table, i, j) == 1) putchar('O');
            else if (GET(table, i, j) == 2) putchar('X');
            else putchar('.');
        }
        printf("\n");
    }
    printf("%lu\n", ROUND(table));
    return;
}
    
int main() {
    int n;
    scanf("%d\n", &n);
    for (int num = 0;num < n;++num) {
        u_long table = get_table();
        char result = value_table(table);
        if (result == 0) printf("Draw\n");
        if (result == 1) printf("O win\n");
        if (result == 2) printf("X win\n");
        //put_table(table);
    }
    return 0;
}
