#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define N 2000 
typedef struct HuffmanNode
{
 char letter;//存储的字符，叶结点为字母，非叶结点为#
 struct HuffmanNode *parent;//父亲结点
 int code;//如果为父亲结点的左孩子，则为0，右孩子为1
 int weight;//权重
 int ID;//结点编号
}HuffmanNode;
typedef struct HeapNode
{
 int rate;//出现次数
 HuffmanNode *node;//对应于哈夫曼树中的结点
}HeapNode;

/*********全局变量********/
int rate[128]={0};//记录字符出现的次数
char letterInit[26];//记录出现的字母
int heapSize;//堆大小
int heapSize2;
int HuffmancodeLength;//记录哈夫曼编码所需要的二进制数个数
int EachHuffmancodeLength[26];//记录每一个字符的哈夫曼编码所需要的二进制数个数
int ASCIILength;//记录ASCII编码所需要的二进制数个数
int num;//记录字符数量
HeapNode *heap;//堆数组
HuffmanNode **array; //记录叶结点的数组
int ID[N];
int W[N];
int P[N];
int C[N];
int pp;
/*********函数声明********/
int CountRate(char* str,int length,int numofLetters);//计算字符出现次数
void entry(int num);//录入数据
int parent(int i);//求父节点
int left(int i);//求左孩子
int right(int i);//求右孩子
void swap(int i,int j);//交换函数
void heapIfy(int i,int localHeapSize);//维持堆性质函数，使用前提为左右子树均为最小堆
void buildHeap();//初始化堆
HeapNode* extractMin();//去掉并返回堆中最小的元素
void heapInsert(int rate,HuffmanNode *p);//向堆中插入数据（前提：堆已经初始化）
HuffmanNode* buildTree();//构造哈夫曼树
void display();//显示函数
void backPrint(HuffmanNode *p,HuffmanNode *root,int i);//从叶节点回溯打印编码code

/*********函数实现********/
int main()
{
printf("******本程序仅支持含有英文字母的输入且字母不少于2种******\n");
char StrTxt[N];
int i,Len,number;
number=0;

while(number<2){
 memset(StrTxt,0,N);

puts("请输入小于2000字符的英文文章：");
gets(StrTxt);
Len = strlen(StrTxt);
if(Len<2000){
 for(i=0;i<Len;i++)//小写转大写 
  {
    if((StrTxt[i]>=97&&StrTxt[i]<=122))
        StrTxt[i]=StrTxt[i]-32; 
  
	}

number=CountRate(StrTxt ,Len,0);//计算每种字符出现的次数
	}
else{
	printf("错误！字符过多 请重新输入！\n");
	system("pause");
   // exit(0);
}
}

heapSize=number;//堆大小即位字符种类数
heapSize2=number;//heapSize的副本
entry(number);//录入数据
display();//打印哈夫曼树和编码
printf("ASCII编码共需%d位二进制数\n",ASCIILength);
i=1;
while(i<=heapSize2){
HuffmancodeLength+=EachHuffmancodeLength[i];
i++;
}
printf("HUFFMAN编码共需%d位二进制数\n",HuffmancodeLength);
double save=(HuffmancodeLength*1.0)/ASCIILength;
printf("使用HUFFMAN编码的报文长度为使用ASCII编码的%.4f倍\n",save);
if(save<=1){
printf("使用HUFFMAN编码更节省\n");
}
else{
	printf("使用ASCII编码更节省\n");
	}


system("pause");
   return 0;
}


int CountRate(char* str, int length,int numofLetters)//计算每种字符的出现次数并返回字符的总数
{

	int i,j;
	j=0;
		while(j<=128){rate[j]=0;j++;}
		j=0;
		while(j<=26){letterInit[j]=0;j++;}  //清空上次输入的痕迹
	ASCIILength=0;
	j=0;
for(i=0;i<length;i++)
        rate[(int)str[i]]++;
printf("- - - - - - - - - - - - -\n");
for(i=65;i<91;i++){
        if(rate[i]!=0){
            printf("| 字符%c |  出现 %3d  次|\n",(char)i,rate[i]);
			letterInit[j]=(char)i;
			j++;
			ASCIILength+=rate[i]*8;
            printf("- - - - - - - - - - - - -\n");
			numofLetters++;
		}
}
	
		printf("文中共有%d种字母\n",numofLetters);
			printf("它们分别是：");
			for(j=0;j<numofLetters;j++){
			printf("%c  ",letterInit[j]);
			 }
			printf("\n \n");
		return numofLetters;
}

void entry(int num)//录入数据
{
	int i,j;

	heap=(HeapNode*)malloc((num+1)*sizeof(HeapNode));//分配堆空间，最多只需要字符的个数，因为合并过程中删除两个，插入一个
	array=(HuffmanNode **)malloc((num+1)*sizeof(HuffmanNode));//记录叶节点的数组，打印编码的时候可以从叶结点回溯向上
	for(i=1,j=0;i<=num,j<num;i++,j++){
		heap[i].rate=rate[(int)letterInit[i-1]];
		heap[i].node=(HuffmanNode *)malloc(sizeof(HuffmanNode));
		array[i]=heap[i].node;
		heap[i].node->parent=NULL;
		heap[i].node->letter=letterInit[j];
		array[i]->weight=heap[i].rate;
		array[i]->ID=i;
	}

}

void display()//显示哈夫曼树，打印哈夫曼编码
{
	
 HuffmanNode* p=buildTree();////哈夫曼树的根节点地址
 int i=1;
 int j,flag;
 pp=0;
 flag=1;

printf("各字母的哈夫曼编码为:\n");
 while(i<=heapSize2)
 {	
  printf("%c :",array[i]->letter);
     EachHuffmancodeLength[i]=0;
  backPrint(array[i],p,i);
  //printf("Huffman编码共需%d位\n",EachHuffmancodeLength[i]);
  printf("\n");
  i++;
 }
 //HuffmanNode *pparent=(HuffmanNode*)malloc((heapSize2-1)*sizeof(HuffmanNode));
printf("              哈夫曼树               \n");
printf("+----------+--------+--------+------+\n");
printf("| 结点编号 | weight | parent | code |\n");
printf("+----------+--------+--------+------+\n");
for(j=1;j<=heapSize2;j++){
	printf("| %4d     | %4d   | %4d   |   %d  |\n",array[j]->ID,array[j]->weight ,array[j]->parent->ID ,array[j]->code);//打印叶结点
printf("+----------+--------+--------+------+\n");
}

 for(j=heapSize2+1;j<=2*heapSize2-1;j++){
	 flag=1;
	for(pp=0;pp<N;pp++)
	{
			if(flag==1&&(ID[pp]==j))
			{
				printf("| %4d     | %4d   | %4d   |   %d  |\n",ID[pp],W[pp],P[pp],C[pp]);//打印父结点
				printf("+----------+--------+--------+------+\n");
				flag=0;
			}		
	}

 }
 printf("| %4d     | %4d   | NULL   | NULL |\n",p->ID ,p->weight );//打印根结点
printf("+----------+--------+--------+------+");
printf("   其中 code为0表示该结点为其父结点的左孩子结点code为1表示该结点为其父结点的右孩子结点\n");
	
	 
}

void backPrint(HuffmanNode *p,HuffmanNode *root,int i)//从叶节点回溯打印编码
{
 if(p!=root)
 {
	  ID[pp]=p->ID;
  W[pp]=p->weight;
  P[pp]=p->parent->ID;
  C[pp]=p->code;
  pp++;
  backPrint(p->parent,root,i);
  printf("%d",p->code);
 
   EachHuffmancodeLength[i]++;//记录编码所需的二进制数的个数
 }
 
}

HuffmanNode* buildTree()
{		
 buildHeap();//初始化堆
 HeapNode *p;//用于临时存储最小堆结点
 HeapNode *q;//用于临时存储次小堆结点
 int count=heapSize;
 int i;
 for(i=1;i<=count-1;i++)
 {
  HuffmanNode *tree=(HuffmanNode*)malloc(sizeof(HuffmanNode));//生成内结点
  tree->ID=heapSize2+i;
  tree->letter='#';//内结点的字符记作#，没有实际意义
  p=extractMin();
  q=extractMin();
  tree->weight=p->rate+q->rate;
  p->node->parent=tree;
  p->node->code=0;
  q->node->parent=tree;
  q->node->code=1;
  heapInsert(p->rate+q->rate,tree);//插入堆中
 
 }


 return extractMin()->node;
}

void buildHeap()//初始化堆
{
 int i=0;
 for(i=heapSize/2;i>=1;i--)
 {
  heapIfy(i,heapSize);
 }
}

void heapIfy(int i,int localHeapSize)//维持堆性质函数，使用前提为左右子树均为最小堆
{
 int l=left(i);
 int r=right(i);
 int least=i;
 //找出heap成员rate 的i,left(i),right(i)的最小值
 if(l<=localHeapSize&&heap[least].rate>heap[l].rate)
 {
  least=l;
 }
 if(r<=localHeapSize&&heap[least].rate>heap[r].rate)
 {
  least=r;
 }
 if(least!=i)
 {
  swap(i,least);
  heapIfy(least,localHeapSize);
 }
}

int parent(int i)
{
 return i/2;
}
int left(int i)
{
 return 2*i;
}
int right(int i)
{
 return 2*i+1;
}


void swap(int i,int j) //交换结构体数组，需要交换结构体内数据
{
 int rate;
 HuffmanNode *p;
 rate=heap[i].rate;
 p=heap[i].node;
 heap[i].rate=heap[j].rate;
 heap[i].node=heap[j].node;
 heap[j].rate=rate;
 heap[j].node=p;
}

HeapNode* extractMin()////去掉并返回堆中最小的元素
{
 if(heapSize>=1)
 {
  HeapNode *min;
  swap(1,heapSize);
  min=&heap[heapSize];
  --heapSize;
  heapIfy(1,heapSize);
  return min;
 }
 else
 {
  printf("堆中没有元素\n");
  return NULL;
 }
}

void heapInsert(int rate,HuffmanNode *p)
{
 ++heapSize;
 int i=heapSize;
 while(i>1&&heap[parent(i)].rate>rate)
 {
  heap[i].rate=heap[parent(i)].rate;
  heap[i].node=heap[parent(i)].node;
  i=parent(i);
 }
 heap[i].rate=rate;
 heap[i].node=p;
}
