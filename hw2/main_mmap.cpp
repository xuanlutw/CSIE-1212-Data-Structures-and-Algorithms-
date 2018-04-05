#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define HASH_MAX 666649

typedef struct record{
    int date;
    float low;
    float high;
    struct record* next;      // hash for date currency exchange
    long long int cap;
    char currency[16];
    char exchange[16];
} Record;

Record* hash_table[HASH_MAX];

unsigned int hash(Record* rec) {
    int ret = (rec->date * 97) % HASH_MAX;
    for (int count = 0;rec->currency[count] != '\0';++count) ret = (ret + rec->currency[count] * 881) % HASH_MAX;
    for (int count = 0;rec->exchange[count] != '\0';++count) ret = (ret + rec->exchange[count] * 991) % HASH_MAX;
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
    int key = hash(rec);
    Record* tmp = hash_table[key];
    while (tmp) {
        if (rec->date == tmp->date && !strcmp(rec->currency, tmp->currency) && !strcmp(rec->exchange, tmp->exchange)) return tmp;
        tmp = tmp->next;
    }
    return NULL;
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

Record* record_malloc() {
    static int n = 0;
    static Record* ptr;
    if (!n) {
        n = 1000000;
        ptr = (Record*)malloc(sizeof(Record) * n);
    }
    --n;
    return ptr + n;
}

char* slice(char** ptr) {
    char* tmp = *ptr;
    while (1) {
        if (**ptr == '\0' || **ptr == '\n' || **ptr =='\t') break;
        ++*ptr;
    }
    if (**ptr != '\0') {
        **ptr = '\0';
        ++*ptr;
    }
    return tmp;
}

int main(int argc, char* argv[]){
    int datafd;
    char* datafile;
    size_t len = 0;
    char* buffer = NULL;
    int n = 0;

    // init data
    if (argc != 2) return 1;
    if ((datafd = open(argv[1], O_RDWR)) == -1) return 1;
    int file_len = lseek(datafd, 0, SEEK_END);
    lseek(datafd, 0, SEEK_SET);
    datafile = (char*)mmap(0, file_len, PROT_READ|PROT_WRITE, MAP_PRIVATE, datafd, 0);
    while (*datafile) {
        // query
        Record* tmp = record_malloc();
        tmp->date = atoi(slice(&datafile));
        strcpy(tmp->currency, slice(&datafile));
        strcpy(tmp->exchange, slice(&datafile));
        tmp->low = atof(slice(&datafile));
        tmp->high = atof(slice(&datafile));
        tmp->cap = atol(slice(&datafile));
        tmp->next = NULL;
        ++n;
        hash_insert(tmp);
        // price
        Record tmp_p;
        record_copy(&tmp_p, tmp);
        strcpy(tmp_p.exchange, "price");
        Record* index = hash_search(&tmp_p);
        if (index) {
            if (tmp->high > index->high) index->high = tmp->high;
            if (tmp->low < index->low) index->low = tmp->low;
        }
        else {
            Record* ptr_p = record_malloc();
            record_copy(ptr_p, &tmp_p);
            hash_insert(ptr_p);
        }
        // cap
        Record tmp_c;
        record_copy(&tmp_c, tmp);
        strcpy(tmp_c.currency, "cap");
        index = hash_search(&tmp_c);
        if (index) {
            index->cap += tmp->cap;
        }
        else {
            Record* ptr_c = record_malloc();
            record_copy(ptr_c, &tmp_c);
            hash_insert(ptr_c);
        }
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
    while (getline(&buffer, &len, stdin) != -1) {
        char token[128]; 
        strcpy(token, strtok(buffer, "\t\n"));
        if (!strcmp(token, "end")) break;
        else if (!strcmp(token, "query")) {
            Record tmp;
            tmp.date = atoi(strtok(NULL, "\t\n"));
            strcpy(tmp.currency, strtok(NULL, "\t\n"));
            strcpy(tmp.exchange, strtok(NULL, "\t\n"));
            Record* index = hash_search(&tmp);
            if (!index) printf("none\n");
            else printf("%.4f %.4f %lld\n", index->low, index->high, index->cap);
        }
        else if (!strcmp(token, "price")) {
            Record tmp;
            int flage = strcmp("min", strtok(NULL, "\t\n"));    // 0: min, 1: max
            tmp.date = atoi(strtok(NULL, "\t\n"));
            strcpy(tmp.currency, strtok(NULL, "\t\n"));
            strcpy(tmp.exchange, "price");
            Record* index = hash_search(&tmp);
            if (!index) printf("none\n");
            else printf("%.4f\n", flage? index->high: index->low);
        }
        else if (!strcmp(token, "cap")) {
            Record tmp;
            tmp.date = atoi(strtok(NULL, "\t\n"));
            strcpy(tmp.currency, "cap");
            strcpy(tmp.exchange, strtok(NULL, "\t\n"));
            Record* index = hash_search(&tmp);
            if (!index) printf("none\n");
            else printf("%lld\n", index->cap);
        }
    }
    return 0;
}
