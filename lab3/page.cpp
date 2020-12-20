#include <iostream>
#include <functional>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <queue>

using namespace std;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

const int INF = 0x3f3f3f3f;
const int MAXN = 65536;

const int OFFSET_LENGTH = 4;
const int PAGE_SIZE = (1 << OFFSET_LENGTH);
const int PAGE_FRAME_COUNT = 8;
const int PROCESS_SIZE = PAGE_SIZE << 8;

inline int rand(int l, int r)
{
    return rand() % (r - l + 1) + l;
}

int r[MAXN];

struct PageReplacement
{
    struct PageTableNode
    {
        int32 vpn, ppn;
        bool present; //是否在内存中有对应
        bool dirty;
        bool read, write;
        int32 used;
        struct PageTableNode *next;
        PageTableNode(int32 vpn = 0, int32 ppn = 0, bool pre = false, PageTableNode *next = nullptr) : vpn(vpn), ppn(ppn), present(pre), next(next), used(0) {}
    };
    int a[PROCESS_SIZE];
    int page[PAGE_FRAME_COUNT][PAGE_SIZE];
    PageTableNode *pt_head;
    int hit, miss;
    queue<int> pq;
    // typedef function<int(int, int)> replace_algo;
    typedef int (PageReplacement::*replace_algo)(int, int);
    // typedef int (*replace_algo)(int, int);
    replace_algo f;
    PageReplacement(string method = "fifo") : hit(0), miss(0), pt_head(new PageTableNode())
    {
        if (method == "fifo")
        {
            f = &PageReplacement::fifo;
        }
        else if (method == "lru")
        {
            f = &PageReplacement::lru;
        }
        else if (method == "lfu")
        {
            f = &PageReplacement::lfu;
        }
        else
        {
            printf("unknown method, using FIFO...\n");
            f = &PageReplacement::fifo;
        }
        for (int i = 0; i < PROCESS_SIZE; ++i)
            a[i] = r[i];
        for (int i = 0; i < PAGE_FRAME_COUNT; ++i)
            pq.push(i);
    }
    void load(int ppn, int vpn)
    {
        memcpy(page[ppn], a + (vpn << OFFSET_LENGTH), PAGE_SIZE * sizeof(int32));
    }
    int fifo(int vpn, int vpo)
    {
        auto p = pt_head->next, fp = pt_head;
        int cnt = 0, ppn;
        while (p)
        {
            if (p->present && p->vpn == vpn)
            {
                ++hit;
                return page[p->ppn][vpo];
            }
            fp = p;
            p = p->next;
            ++cnt;
        }
        ++miss;
        if (cnt >= PAGE_FRAME_COUNT)
        {
            pt_head->next->vpn = vpn;
            ppn = pt_head->next->ppn;
            load(ppn, vpn);
        }
        else
        {
            ppn = pq.front();
            pq.pop();
            fp->next = new PageTableNode(vpn, ppn, true);
            load(ppn, vpn);
        }
        return page[ppn][vpo];
    }
    int lfu(int vpn, int vpo) //最少使用
    {
        auto p = pt_head->next, fp = pt_head;
        int32 minu = INF;
        PageTableNode *minp = nullptr;
        int cnt = 0, ppn;
        while (p)
        {
            if (p->used < minu)
            {
                minu = p->used;
                minp = p;
            }
            if (p->present && p->vpn == vpn)
            {
                ++hit;
                ++p->used;
                return page[p->ppn][vpo];
            }
            fp = p;
            p = p->next;
            ++cnt;
        }
        ++miss;
        if (cnt >= PAGE_FRAME_COUNT)
        { //那就把最小的置换出来
            minp->vpn = vpn;
            ppn = minp->ppn;
            minp->used = 1;
            load(ppn, vpn);
        }
        else
        {
            ppn = pq.front();
            pq.pop();
            fp->next = new PageTableNode(vpn, ppn, true);
            fp->next->used = 1;
            load(ppn, vpn);
        }
        return page[ppn][vpo];
    }
    int lru(int vpn, int vpo)
    {
        auto p = pt_head->next, fp = pt_head;
        int cnt = 0, ppn;
        while (p)
        {
            if (p->present && p->vpn == vpn) //如果命中,就放在队列最前面
            {
                ++hit;
                fp->next = p->next;
                p->next = pt_head->next;
                pt_head->next = p;
                return page[p->ppn][vpo];
            }
            fp = p;
            p = p->next;
            ++cnt;
        }
        ++miss;
        if (cnt >= PAGE_FRAME_COUNT)
        {
            fp->vpn = vpn;
            ppn = fp->ppn;
            load(ppn, vpn);
        }
        else
        {
            ppn = pq.front();
            pq.pop();
            pt_head->next = new PageTableNode(vpn, ppn, true, pt_head->next);
            load(ppn, vpn);
        }
        return page[ppn][vpo];
    }
    int access(int va)
    {
        int32 vpn = va >> OFFSET_LENGTH, vpo = va & (PAGE_SIZE - 1);
        return (this->*f)(vpn, vpo);
    }
    void sequential_access()
    {
        for (int i = 0; i < PROCESS_SIZE; ++i)
            access(i);
    }
    void jump_access()
    {
        int res = rand() % PROCESS_SIZE;
        for (int i = 0; i < PROCESS_SIZE; ++i)
        {
            // cout << "res=" << res << endl;
            res = access(res) % PROCESS_SIZE;
        }
    }
    void branch_access()
    {
        int res = 0;
        for (int i = 0; i < PROCESS_SIZE; ++i)
        {
            int tmp = access(res);
            if (tmp & 1)
            {
                res = (res + 1) % PROCESS_SIZE;
            }
            else
            {
                res = tmp % PROCESS_SIZE;
            }
        }
    }
    void loop_access()
    {
        int base = rand() % PROCESS_SIZE, loop = rand(1, PROCESS_SIZE / 2);
        for (int i = 0; i < PROCESS_SIZE; ++i)
        {
            access((base + (i % loop)) % PROCESS_SIZE);
        }
    }
    void random_access()
    {
        for (int i = 0; i < PROCESS_SIZE; ++i)
        {
            access(rand() % PROCESS_SIZE);
        }
    }
    double rate()
    {
        // cout << miss << "," << hit << endl;
        return (double)miss / (miss + hit) * 100;
    }
};

int main()
{
    srand(time(NULL));
    for (int i = 0; i < MAXN; ++i)
        r[i] = rand();
    printf("[MISS]        FIFO     LRU      LFU\n");
    {
        PageReplacement pg1("fifo"), pg2("lru"), pg3("lfu");
        pg1.sequential_access();
        pg2.sequential_access();
        pg3.sequential_access();
        printf("%-10s %7.3lf%% %7.3lf%% %7.3lf%%\n", "sequential", pg1.rate(), pg2.rate(), pg3.rate());
    }
    {
        PageReplacement pg1("fifo"), pg2("lru"), pg3("lfu");
        pg1.jump_access();
        pg2.jump_access();
        pg3.jump_access();
        printf("%-10s %7.3lf%% %7.3lf%% %7.3lf%%\n", "jump", pg1.rate(), pg2.rate(), pg3.rate());
    }
    {
        PageReplacement pg1("fifo"), pg2("lru"), pg3("lfu");
        pg1.branch_access();
        pg2.branch_access();
        pg3.branch_access();
        printf("%-10s %7.3lf%% %7.3lf%% %7.3lf%%\n", "branch", pg1.rate(), pg2.rate(), pg3.rate());
    }
    {
        PageReplacement pg1("fifo"), pg2("lru"), pg3("lfu");
        pg1.loop_access();
        pg2.loop_access();
        pg3.loop_access();
        printf("%-10s %7.3lf%% %7.3lf%% %7.3lf%%\n", "loop", pg1.rate(), pg2.rate(), pg3.rate());
    }
    {
        PageReplacement pg1("fifo"), pg2("lru"), pg3("lfu");
        pg1.random_access();
        pg2.random_access();
        pg3.random_access();
        printf("%-10s %7.3lf%% %7.3lf%% %7.3lf%%\n", "random", pg1.rate(), pg2.rate(), pg3.rate());
    }
    return 0;
}