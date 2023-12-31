#include <iostream>
#include <algorithm>
#include <cstdio>
using namespace std;

#define N 100      //最多可能物体数
struct goods       //物品结构体
{
    int sign;      //物品序号
    int w;         //物品重量
    int p;         //物品价值
}a[N];

bool m(goods a, goods b)
{
    return (a.p / a.w) > (b.p / b.w);
}

int max(int a, int b)
{
    return a < b ? b : a;
}

int n, C, bestP = 0, cp = 0, cw = 0;

int X[N], cx[N];

struct KNAPNODE   //状态结构体
{
    bool s1[N];   //当前放入物体
    int k;        //搜索深度
    int b;        //价值上界
    int w;        //物体重量
    int p;        //物体价值
};

struct HEAP       //堆元素结构体
{
    KNAPNODE* p;  //结点数据
    int b;        //所指结点的上界
};

//交换两个堆元素
void swap(HEAP& a, HEAP& b)
{
    HEAP temp = a;
    a = b;
    b = temp;
}

//堆中元素上移
void mov_up(HEAP H[], int i)
{
    bool done = false;
    if (i != 1)
    {
        while (!done && i != 1)
        {
            if (H[i].b > H[i / 2].b)
            {
                swap(H[i], H[i / 2]);
            }
            else
            {
                done = true;
            }
            i = i / 2;
        }
    }
}

//堆中元素下移
void mov_down(HEAP H[], int n, int i)
{
    bool done = false;
    if ((2 * i) <= n)
    {
        while (!done && ((i = 2 * i) <= n))
        {
            if (i + 1 <= n && H[i + 1].b > H[i].b)
            {
                i++;
            }
            if (H[i / 2].b < H[i].b)
            {
                swap(H[i / 2], H[i]);
            }
            else
            {
                done = true;
            }
        }
    }
}

//往堆中插入结点
void insert(HEAP H[], HEAP x, int& n)
{
    n++;
    H[n] = x;
    mov_up(H, n);
}

//删除堆中结点
void del(HEAP H[], int& n, int i)
{
    HEAP x, y;
    x = H[i]; y = H[n];
    n--;
    if (i <= n)
    {
        H[i] = y;
        if (y.b >= x.b)
        {
            mov_up(H, i);
        }
        else
        {
            mov_down(H, n, i);
        }
    }
}

//获得堆顶元素并删除
HEAP del_top(HEAP H[], int& n)
{
    HEAP x = H[1];
    del(H, n, 1);
    return x;
}

//计算分支节点的上界
void bound(KNAPNODE* node, int M, goods a[], int n)
{
    int i = node->k;
    float w = node->w;
    float p = node->p;
    if (node->w > M)
    {        //物体重量超过背包载重量
        node->b = 0;       //上界置为0
    }
    else
    {
        while ((w + a[i].w <= M) && (i < n))
        {
            w += a[i].w;   //计算背包已装入载重
            p += a[i++].p; //计算背包已装入价值
        }
        if (i < n)
        {
            node->b = p + (M - w) * a[i].p / a[i].w;
        }
        else
        {
            node->b = p;
        }
    }
}

//用分支限界法实现0/1背包问题
int KnapSack(int n, goods a[], int C, int X[])
{
    int i, k = 0;                 //堆中元素个数的计数器初始化为0
    int v;
    KNAPNODE* xnode, * ynode, * znode;
    HEAP x, y, z, * heap;
    heap = new HEAP[n * n];         //分配堆的存储空间
    for (i = 0; i < n; i++)
    {
        a[i].sign = i;               //记录物体的初始编号
    }
    sort(a, a + n, m);                //对物体按照价值重量比排序
    xnode = new KNAPNODE;         //建立父亲结点
    for (i = 0; i < n; i++)
    {          //初始化结点
        xnode->s1[i] = false;
    }
    xnode->k = xnode->w = xnode->p = 0;
    while (xnode->k < n)
    {
        ynode = new KNAPNODE;      //建立结点y
        *ynode = *xnode;           //结点x的数据复制到结点y
        ynode->s1[ynode->k] = true;    //装入第k个物体
        ynode->w += a[ynode->k].w;     //背包中物体重量累计
        ynode->p += a[ynode->k].p;     //背包中物体价值累计
        ynode->k++;               //搜索深度++
        bound(ynode, C, a, n);     //计算结点y的上界
        y.b = ynode->b;
        y.p = ynode;
        insert(heap, y, k);       //结点y按上界的值插入堆中
        znode = new KNAPNODE;      //建立结点z
        *znode = *xnode;           //结点x的数据复制到结点z
        znode->k++;                //搜索深度++
        bound(znode, C, a, n);     //计算节点z的上界
        z.b = znode->b;
        z.p = znode;
        insert(heap, z, k);        //结点z按上界的值插入堆中
        delete xnode;
        x = del_top(heap, k);      //获得堆顶元素作为新的父亲结点
        xnode = x.p;
    }
    v = xnode->p;
    for (i = 0; i < n; i++)
    {          //取装入背包中物体在排序前的序号
        if (xnode->s1[i])
        {
            X[a[i].sign] = 1;
        }
        else
        {
            X[a[i].sign] = 0;
        }
    }
    delete xnode;
    delete heap;
    return v;                    //返回背包中物体的价值
}

/*测试以上算法的主函数*/
int main()
{
    goods b[N];
    printf("物品种数n: ");
    cin >> n;         //输入物品种数
    printf("背包容量C: ");
    cin >> C;       //输入背包容量
    for (int i = 0; i < n; i++)     //输入物品i的重量w及其价值v
    {
        printf("物品%d的重量w[%d]及其价值v[%d]: ", i + 1, i + 1, i + 1);
   
        cin >> a[i].w;
        cin >> a[i].p;
        
        b[i] = a[i];
    }

    int sum = KnapSack(n, a, C, X);//调用分支限界法求0/1背包问题
    printf("分支限界法求解0/1背包问题:\nX=[ ");
    for (int i = 0; i < n; i++)
        cout << X[i] << " ";//输出所求X[n]矩阵
    printf("]  装入总价值[%d]\n", sum);
    return 0;
}