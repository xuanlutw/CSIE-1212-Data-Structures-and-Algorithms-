#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <unordered_map>

#define RESULT(x) (((x) >> 55) & 3) 
#define ROUND(x) (((x) >> 50) & 31) 
#define GET(x, i, j) (((x) >> (((i) * 5 + (j)) * 2)) & 3)
#define GET2(x, i, j) ((GET((x), (i), (j)) == 2)? 0: GET((x), (i), (j)))     // X -> 0
#define COLOR(x) (((ROUND(x) >> 1) & 1) + 1)

#define UP_ROUND(x) ((x) + ((long)2 << 50))
#define SET(x, i, j, c) ((x) + ((c) << (((i) * 5 + (j)) * 2)))
#define SET_RESULT(x, r) ((x) | ((long)(r) << 55))
#define SET_ROUND(x, r) ((x) | ((long)(r) << 50))
#define SET_FLAG(x) ((x) | (long)(1) << 57)

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
   57    56-55    54-50    49-0
   set   result   round    table
*/

std::unordered_map<u_long, int> map;

inline int search(u_long table, int set) {
    if (set) table = SET_FLAG(table);
    auto search = map.find(table);
    if(search != map.end())
        return search->second;
    return -1;
}

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

/*
   ret value
   0 unknowm
   1 O win
   2 X win
   */

/*
   if set on, high order has draw sol. that means draw and best sol are the same
 */
char value_table(u_long table, int set, int pset) {
    if (pset && set) return 0;
    int pre = search(table, set);
    if (pre != -1) return pre;
    int state = 0;
    if (ROUND(table) == 22) state = valid_table(table);
    /*
       0 opposite color
       1 draw
       2 same color
    */
    else {
        int count = 0;
        u_long undo[300];
        for (int i = 0;i < 25;++i) {
            if (state == 2) break;
            if (set && state) break;
            if (GET(table, i / 5, i % 5)) continue;
            u_long tmp1 = SET(table, i / 5, i % 5, COLOR(table));
            for (int j = 24;j > i;--j) {
                if (GET(tmp1, j / 5, j % 5)) continue;
                u_long tmp2 = UP_ROUND(SET(tmp1, j / 5, j % 5, COLOR(tmp1)));
                int next = search(tmp2, state | pset);
                if (next == -1) {
                    undo[count] = tmp2;
                    ++count;
                }
                else if (COLOR(table) == next) {
                    state = 2;
                    break;
                }
                else if (!next && !state) {
                    state = 1;
                    if (set) break;
                }
            }
        }
        if (!(state == 2) && !(set && state)) {
            for (int i = 0;i < count;++i) {
                int next = value_table(undo[i], state | pset, set);
                if (COLOR(table) == next) {
                    state = 2;
                    break;
                }
                if (!next && !state) {
                    state = 1;
                    if (set) break;
                }
            }
        }
        if (state == 2) state = COLOR(table);
        else if (state == 1) state = 0;
        else state = (COLOR(table) == 1)? 2: 1;
    }

    if (set) map.insert({SET_FLAG(table), state});
    else map.insert({table, state});
    return state;
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
        char result = value_table(table, 0, 0);
        if (result == 0) printf("Draw\n");
        if (result == 1) printf("O win\n");
        if (result == 2) printf("X win\n");
        //put_table(table);
    }
    return 0;
}
