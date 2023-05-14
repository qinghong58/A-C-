#include <stdio.h>
#include <vector>


using namespace std;

//二维地图  行和列
#define ROWS 9
#define COLS 11

//直线代价
#define ZXDJ 10
//斜线代价
#define XXDJ 14


//点的类型   f=g+h
struct MyPoint
{
    int row;
    int col;
    int f;
    int g;
    int h;
    void setF()
    {
        f=g+h;
    }
};

//树的节点类型（八叉树，因为是九宫格）
struct TreeNode
{
    MyPoint pos;                //当前节点坐标
    vector<TreeNode*> childs;   //存储  当前点的子节点的指针数组
    TreeNode* pParent;          //存储  当前接地那的父节点的指针变量
};

//寻路方向
enum direct
{
    p_up,
    p_down,
    p_left,
    p_right,
    p_lup,
    p_ldown,
    p_rup,
    p_rdown
};


//打印地图
void printMap(int map[ROWS][COLS]);

//计算h值，endpos：终点坐标，pos：当前点坐标
int getH(MyPoint endPos, MyPoint pos);

//看这个点是否需要统计
bool needAdd(MyPoint pos,int map[ROWS][COLS],bool pathMap[ROWS][COLS]);

int main()
{
    //地图    0空地   1障碍
    int map[ROWS][COLS] = {
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
    };

    //辅助地图节点类型（记录已经走过的节点，走过的点就不需要在走了） false:没有走过    true  :走过
    bool pathMap[ROWS][COLS]={0};

    //起点和终点
    MyPoint begPos={2,2};
    MyPoint endPos={4,8};

    //标记起点走过
    pathMap[begPos.row][begPos.col]=true;

    //创建一棵树，起点是树的根节点
    TreeNode* pNew=new TreeNode;
    //赋值操作
    pNew->pos={0};
    //memset(pNew,0,sizeof(TreeNode));

    //pRoot一直指向树的根节点
    TreeNode* pRoot=pNew;
    //根节点指向起点
    pRoot->pos=begPos;


    //用一个vector数组，存储用来比较的节点
    vector<TreeNode*> buff;


    //开始寻路
    TreeNode* pCurrent=pRoot;//记录当前节点
    TreeNode* pChild=NULL;

    //用两个迭代器，寻找最小的f值
    vector<TreeNode*>::iterator it;
    vector<TreeNode*>::iterator itMin;

    bool isFindEnd=false;

    while (1)
    {
        //找到当前周围能走的点
        for (int i = 0; i < 8; i++)
        {
            pChild=new TreeNode;
            pChild->pos={0};
           // memset(pChild,0,sizeof(TreeNode));

            pChild->pos=pCurrent->pos;

            switch (i)
            {
            case p_up:
                pChild->pos.row--;

                pChild->pos.g+=ZXDJ;
                break;

            case p_down:
                pChild->pos.row++;

                pChild->pos.g += ZXDJ;
                break;

            case p_left:
                pChild->pos.col--;

                pChild->pos.g += ZXDJ;
                break;

            case p_right:
                pChild->pos.col++;

                pChild->pos.g += ZXDJ;
                break;
            case p_lup:
                pChild->pos.row--;
                pChild->pos.col--;

                pChild->pos.g += XXDJ;
                break;
            case p_rup:
                pChild->pos.row--;
                pChild->pos.col++;

                pChild->pos.g += XXDJ;
                break;
            case p_ldown:
                pChild->pos.row++;
                pChild->pos.col--;

                pChild->pos.g += XXDJ;
                break;
            case p_rdown:
                pChild->pos.row++;
                pChild->pos.col++;

                pChild->pos.g += XXDJ;
                break;


            default:
                break;
            }
            //计算g  h  f 的值
            pChild->pos.h=getH(endPos,pChild->pos);
            pChild->pos.setF();

            //将能走的点放入树中    和  放入buff数组    ，并且进行标记走过
            if (needAdd(pChild->pos,map,pathMap))
            {
                //放入树形结构
                pCurrent->childs.push_back(pChild);
                pChild->pParent=pCurrent;

                //放入buff数组
                buff.push_back(pChild);

                //标记走过
                pathMap[pChild->pos.row][pChild->pos.col]=true;
            }
            else
            {
                delete pChild;
            }
        }

        //从buff数组中找出f值最小的那个节点
        itMin=buff.begin();
        for (it = buff.begin(); it != buff.end(); it++)
        {
            itMin=(((*itMin)->pos.f>(*it)->pos.f)?it:itMin);
        }
        //删掉当前的最小节点，并且把最小节点周围能走的点（子节点）放入树中和buff数组
        pCurrent=*itMin;
        buff.erase(itMin);
        //判断是否寻路结束
        if (pCurrent->pos.row==endPos.row&&pCurrent->pos.col==endPos.col)
        {
            isFindEnd=true;
            break;
        }
        if (buff.empty())
        {
            break;
        }
    }
    printMap(map);
    if (isFindEnd)
    {
        printf_s("找到终点了！\n");

        while (pCurrent)
        {
            printf_s("(%d,%d) ",pCurrent->pos.row,pCurrent->pos.col);
            pCurrent=pCurrent->pParent;
        }
        printf_s("\n");
    }


    while (1);



    return 0;
}

//看这个点是否需要统计
bool needAdd(MyPoint pos, int map[ROWS][COLS], bool pathMap[ROWS][COLS])
{
    //在地图范围内
    if (pos.row>=ROWS||pos.row<0|| pos.col >= COLS || pos.col < 0)
    {
        return false;
    }
    //不是障碍
    if (map[pos.row][pos.col]==1)
    {
        return false;
    }
    //没有走过
    if (pathMap[pos.row][pos.col] == 1)
    {
        return false;
    }
    return true;
}

//计算h值，endpos：终点坐标，pos：当前点坐标
int getH(MyPoint endPos, MyPoint pos)
{
    int x=((endPos.col>pos.col)?(endPos.col-pos.col):(pos.col-endPos.col));
    int y= ((endPos.row > pos.row) ? (endPos.row - pos.row) : (pos.row - endPos.row));

    return (x+y)*ZXDJ;
}

//打印地图
void printMap(int map[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j=0;j<COLS;j++)
        {
            if (map[i][j]==0)
            {
                printf_s(" ");
            }
            else if (map[i][j] == 1)
            {
                printf_s("墙");
            }
        }
        printf_s("\n");
    }
}