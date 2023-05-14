#include <stdio.h>
#include <vector>


using namespace std;

//��ά��ͼ  �к���
#define ROWS 9
#define COLS 11

//ֱ�ߴ���
#define ZXDJ 10
//б�ߴ���
#define XXDJ 14


//�������   f=g+h
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

//���Ľڵ����ͣ��˲�������Ϊ�ǾŹ���
struct TreeNode
{
    MyPoint pos;                //��ǰ�ڵ�����
    vector<TreeNode*> childs;   //�洢  ��ǰ����ӽڵ��ָ������
    TreeNode* pParent;          //�洢  ��ǰ�ӵ��ǵĸ��ڵ��ָ�����
};

//Ѱ·����
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


//��ӡ��ͼ
void printMap(int map[ROWS][COLS]);

//����hֵ��endpos���յ����꣬pos����ǰ������
int getH(MyPoint endPos, MyPoint pos);

//��������Ƿ���Ҫͳ��
bool needAdd(MyPoint pos,int map[ROWS][COLS],bool pathMap[ROWS][COLS]);

int main()
{
    //��ͼ    0�յ�   1�ϰ�
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

    //������ͼ�ڵ����ͣ���¼�Ѿ��߹��Ľڵ㣬�߹��ĵ�Ͳ���Ҫ�����ˣ� false:û���߹�    true  :�߹�
    bool pathMap[ROWS][COLS]={0};

    //�����յ�
    MyPoint begPos={2,2};
    MyPoint endPos={4,8};

    //�������߹�
    pathMap[begPos.row][begPos.col]=true;

    //����һ��������������ĸ��ڵ�
    TreeNode* pNew=new TreeNode;
    //��ֵ����
    pNew->pos={0};
    //memset(pNew,0,sizeof(TreeNode));

    //pRootһֱָ�����ĸ��ڵ�
    TreeNode* pRoot=pNew;
    //���ڵ�ָ�����
    pRoot->pos=begPos;


    //��һ��vector���飬�洢�����ȽϵĽڵ�
    vector<TreeNode*> buff;


    //��ʼѰ·
    TreeNode* pCurrent=pRoot;//��¼��ǰ�ڵ�
    TreeNode* pChild=NULL;

    //��������������Ѱ����С��fֵ
    vector<TreeNode*>::iterator it;
    vector<TreeNode*>::iterator itMin;

    bool isFindEnd=false;

    while (1)
    {
        //�ҵ���ǰ��Χ���ߵĵ�
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
            //����g  h  f ��ֵ
            pChild->pos.h=getH(endPos,pChild->pos);
            pChild->pos.setF();

            //�����ߵĵ��������    ��  ����buff����    �����ҽ��б���߹�
            if (needAdd(pChild->pos,map,pathMap))
            {
                //�������νṹ
                pCurrent->childs.push_back(pChild);
                pChild->pParent=pCurrent;

                //����buff����
                buff.push_back(pChild);

                //����߹�
                pathMap[pChild->pos.row][pChild->pos.col]=true;
            }
            else
            {
                delete pChild;
            }
        }

        //��buff�������ҳ�fֵ��С���Ǹ��ڵ�
        itMin=buff.begin();
        for (it = buff.begin(); it != buff.end(); it++)
        {
            itMin=(((*itMin)->pos.f>(*it)->pos.f)?it:itMin);
        }
        //ɾ����ǰ����С�ڵ㣬���Ұ���С�ڵ���Χ���ߵĵ㣨�ӽڵ㣩�������к�buff����
        pCurrent=*itMin;
        buff.erase(itMin);
        //�ж��Ƿ�Ѱ·����
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
        printf_s("�ҵ��յ��ˣ�\n");

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

//��������Ƿ���Ҫͳ��
bool needAdd(MyPoint pos, int map[ROWS][COLS], bool pathMap[ROWS][COLS])
{
    //�ڵ�ͼ��Χ��
    if (pos.row>=ROWS||pos.row<0|| pos.col >= COLS || pos.col < 0)
    {
        return false;
    }
    //�����ϰ�
    if (map[pos.row][pos.col]==1)
    {
        return false;
    }
    //û���߹�
    if (pathMap[pos.row][pos.col] == 1)
    {
        return false;
    }
    return true;
}

//����hֵ��endpos���յ����꣬pos����ǰ������
int getH(MyPoint endPos, MyPoint pos)
{
    int x=((endPos.col>pos.col)?(endPos.col-pos.col):(pos.col-endPos.col));
    int y= ((endPos.row > pos.row) ? (endPos.row - pos.row) : (pos.row - endPos.row));

    return (x+y)*ZXDJ;
}

//��ӡ��ͼ
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
                printf_s("ǽ");
            }
        }
        printf_s("\n");
    }
}