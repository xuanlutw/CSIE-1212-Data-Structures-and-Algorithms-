#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#define HASH_MAX 666649

typedef struct record{
    int date;
    float low;
    float high;
    struct record* next;      // hash for date currency exchange
    struct record* next_ans;  // output next
    long long int cap;
    char currency[16];
    char exchange[16];
    char done;
} Record;

Record* hash_table[HASH_MAX];

unsigned int hash(Record* rec) {
    int ret = (rec->date * 97) % HASH_MAX;
    for (char* ptr = rec->currency;*ptr;++ptr) ret = (ret + *ptr * 881) % HASH_MAX;
    for (char* ptr = rec->exchange;*ptr;++ptr) ret = (ret + *ptr * 991) % HASH_MAX;
    return ret;
}

void hash_insert(Record* rec) {
    int key = hash(rec);
    Record* tmp = hash_table[key];
    if (!tmp) {
        hash_table[key] = rec;
        return;
    }
    while (tmp->next) tmp = tmp->next;
    tmp->next = rec;
    return;
}

Record* hash_search(Record* rec) {
    static Record* pre = NULL;
    if (rec) {
        int key = hash(rec);
        Record* tmp = hash_table[key];
        while (tmp) {
            if (rec->date == tmp->date && !strcmp(rec->currency, tmp->currency) && !strcmp(rec->exchange, tmp->exchange)){
                pre = tmp;
                return tmp;
            }
            tmp = tmp->next;
        }
        pre = NULL;
        return NULL;
    }
    else {
        if (!pre) return NULL;
        Record* tmp = pre->next;
        while (tmp) {
            if (pre->date == tmp->date && !strcmp(pre->currency, tmp->currency) && !strcmp(pre->exchange, tmp->exchange)){
                pre = tmp;
                return tmp;
            }
            tmp = tmp->next;
        }
        pre = NULL;
        return NULL;
    }
}

void record_copy(Record* dest, Record* src) {
    dest->date = src->date;
    strcpy(dest->currency, src->currency);
    strcpy(dest->exchange, src->exchange);
    dest->low = src->low;
    dest->high = src->high;
    dest->cap = src->cap;
    dest->next = src->next;
    return;
}

int main(int argc, char* argv[]){
    FILE* datafile;
    size_t len = 0;
    char* buffer = NULL;
    int n = 0;
    Record* head;

    Record** pre;
    pre = &head;
    while (getline(&buffer, &len, stdin) != -1) {
        char token[128]; 
        strcpy(token, strtok(buffer, "\t\n"));
        if (!strcmp(token, "end")) break;
        Record* tmp = (Record*)malloc(sizeof(Record));
        tmp->next = NULL;
        tmp->next_ans = NULL;
        tmp->done = 0;
        if (!strcmp(token, "query")) {
            tmp->date = atoi(strtok(NULL, "\t\n"));
            strcpy(tmp->currency, strtok(NULL, "\t\n"));
            strcpy(tmp->exchange, strtok(NULL, "\t\n"));
        }
        else if (!strcmp(token, "price")) {
            strcpy(tmp->exchange, strtok(NULL, "\t\n"));
            tmp->date = atoi(strtok(NULL, "\t\n"));
            strcpy(tmp->currency, strtok(NULL, "\t\n"));
            tmp->high = 0;
            tmp->low = FLT_MAX;
        }
        else if (!strcmp(token, "cap")) {
            tmp->date = atoi(strtok(NULL, "\t\n"));
            strcpy(tmp->currency, "cap");
            strcpy(tmp->exchange, strtok(NULL, "\t\n"));
            tmp->cap = 0;
        }
        *pre = tmp;
        pre = &(tmp->next_ans);
        hash_insert(tmp);
        ++n;
    }
    free(buffer);
    buffer = NULL;
    len = 0;
    /* test hash
    int sum = 0, max = 0;
    for (int i = 0;i < HASH_MAX;++i) {
        int sub_sum = 0;
        Record* tmp = hash_table[i];
        while (tmp) {
            ++sub_sum;
            tmp = tmp->next;
        }
        sum += sub_sum;
        if (sub_sum > max) max = sub_sum;
    }
    printf("sum %d\n", sum);
    printf("n %d\n", n);
    printf("max %d\n", max);
    */
    // init data
    if (argc != 2) return 87;
    if (!(datafile = fopen(argv[1], "r"))) return 56;
    while (getline(&buffer, &len, datafile) != -1) {
        Record tmp;
        tmp.date = atoi(strtok(buffer, "\t\n"));
        strcpy(tmp.currency, strtok(NULL, "\t\n"));
        strcpy(tmp.exchange, strtok(NULL, "\t\n"));
        tmp.low = atof(strtok(NULL, "\t\n"));
        tmp.high = atof(strtok(NULL, "\t\n"));
        tmp.cap = atol(strtok(NULL, "\t\n"));
        tmp.next = NULL;
        // query
        Record* index = hash_search(&tmp);
        while (index) {
            if (index) {
                index->cap = tmp.cap;
                index->high = tmp.high;
                index->low = tmp.low;
                index->done = 1;
            }
            index = hash_search(NULL);
        }
        // price
        char name_tmp[16];
        strcpy(name_tmp, tmp.exchange);
        strcpy(tmp.exchange, "max");
        index = hash_search(&tmp);
        while (index) {
            if (index) { 
                if (index->high < tmp.high) index->high = tmp.high;
                index->done = 1;
            }
            index = hash_search(NULL);
        }
        strcpy(tmp.exchange, "min");
        index = hash_search(&tmp);
        while (index) {
            if (index) {
                if (index->low > tmp.low) index->low = tmp.low;
                index->done = 1;
            }
            index = hash_search(NULL);
        }
        // cap
        strcpy(tmp.exchange, name_tmp);
        strcpy(tmp.currency, "cap");
        index = hash_search(&tmp);
        while (index) {
            if (index) {
                index->cap += tmp.cap; 
                index->done = 1;
            }
            index = hash_search(NULL);
        }
    }
    free(buffer);
    buffer = NULL;
    len = 0;
    for (;head;head = head->next_ans) {
        if (head->done) {
            if (!strcmp(head->currency, "cap")) printf("%lld\n", head->cap);
            else if (!strcmp(head->exchange, "max")) printf("%.4f\n", head->high);
            else if (!strcmp(head->exchange, "min")) printf("%.4f\n", head->low);
            else printf("%.4f %.4f %lld\n", head->low, head->high, head->cap);
        }
        else printf("none\n");
    }
    return 0;
}
