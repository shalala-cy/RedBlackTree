#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#include <iostream>
#include <QDebug>
#include <QVector2D>
#include "math.h"

const float rootPosX=550.0;
const float rootPosY=10.0;
const float nodeDistanceV=100.0;
const float halfW=500.0;
extern const float nodeLength;

enum Color{
    RED,
    BLACK
};

template<class KEY,class T>
struct RBNode{
    T element;
    KEY key;
    Color color;        //节点颜色，黑或红
    RBNode<KEY,T>* parent;
    RBNode<KEY,T>* leftChild;
    RBNode<KEY,T>* rightChild;
    int layer;          // 节点层数，根节点所在位置是1
    float posXTree;     //节点的横坐标
    float posYTree;     //节点纵坐标
    QVector2D size;     //节点在屏幕显示尺寸
    RBNode(){           //结构体构造函数
        parent=leftChild=rightChild=NULL;
        color=BLACK;
        size=QVector2D(nodeLength, nodeLength);
    }
    //结构体构造函数
    RBNode(const KEY &K, const T &theElement){
        element=theElement;
        key=K;
        parent=leftChild=rightChild=NULL;
        color=BLACK;
        size=QVector2D(nodeLength, nodeLength);
    }
};

template<class KEY, class T>
class RBTree{
    public:
        RBTree(){root=NULL;}
        void insertNode(RBNode<KEY,T> node);
        void deleteNode(const KEY &k);
        void showRBTree();
        RBNode<KEY,T>* grandparent(const RBNode<KEY,T>* p);
        RBNode<KEY,T>* uncle(const RBNode<KEY,T>* p);
        RBNode<KEY,T>* brother(const RBNode<KEY,T>* p);
        void clockwise(RBNode<KEY,T>* p);
        void unclockwise(RBNode<KEY,T>* p);

        //节点插入，需要分情况处理，参考https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
        void insertCase1(RBNode<KEY,T>* p);
        void insertCase2(RBNode<KEY,T>* p);
        void insertCase3(RBNode<KEY,T>* p);
        void insertCase4(RBNode<KEY,T>* p);
        void insertCase5left(RBNode<KEY,T>* p);
        void insertCase5right(RBNode<KEY,T>* p);

        //删除节点，同样需要分情况处理，参考//zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
        void deleteCase1(RBNode<KEY,T>* p);
        void deleteCase2(RBNode<KEY,T>* p);
        void deleteCase3(RBNode<KEY,T>* p);
        void deleteCase4_left(RBNode<KEY,T>* p);
        void deleteCase4_right(RBNode<KEY,T>* p);
        void deleteCase5_left(RBNode<KEY,T>* p);
        void deleteCase5_right(RBNode<KEY,T>* p);
        void deleteCase6_left(RBNode<KEY,T>* p);
        void deleteCase6_right(RBNode<KEY,T>* p);

        void showRBTree(const RBNode<KEY,T>* rt) const; //用于在终端窗口显示红黑树
        void updateTreePos(float w, float h);

        ~RBTree(){};
        RBNode<KEY,T> *root;
    private:
        int size;
        RBNode<KEY,T> *nullNode;
};


template<class KEY, class T>
void RBTree<KEY,T>::showRBTree(){
        showRBTree(root);
    }

template<class KEY, class T>  //终端显示
void RBTree<KEY,T>::showRBTree(const RBNode<KEY,T>* rt) const {
    if(rt!=NULL){
        showRBTree(rt->leftChild);
        std::cout<<rt->key<<'('<<rt->element<<')'<<rt->color<<'\t'<<"POS:"<<rt->posXTree<<","<<rt->posYTree<<std::endl;
        if(rt->parent!=NULL){
            std::cout<<"Parent:"<<rt->parent->key<<'\t';}
        if(rt->leftChild!=NULL){
            std::cout<<"leftchild:"<<rt->leftChild->key<<'\t';}
        if(rt->rightChild!=NULL){
            std::cout<<"rightchild:"<<rt->rightChild->key;}
        std::cout<<std::endl;
        showRBTree(rt->rightChild);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::insertNode(RBNode<KEY,T> node){
    RBNode<KEY,T>*  p;
    RBNode<KEY,T>*  x;
    RBNode<KEY,T>*  n= new RBNode<KEY,T>(node.key, node.element);
    x=root;
    p=NULL;
    while(x!=NULL){
        p=x;
        if(n->key < x->key){
            x=x->leftChild;
        }else if(n->key > x->key){
            x=x->rightChild;
        }else{
            return;
        }
    }
    n->parent=p;
    if(p==NULL){
        root=n;
        root->color=BLACK;
    }else if(n->key < p->key ){
        p->leftChild=n;
        n->color=RED;
        this->insertCase1(n);
    }else{
        p->rightChild=n;
        n->color=RED;
        this->insertCase1(n);
    }
}

template<class KEY, class T>
RBNode<KEY,T>* RBTree<KEY,T>::grandparent(const RBNode<KEY,T>* p){
    if(p->parent!=NULL && p->parent->parent!=NULL){
        return  p->parent->parent;
    }
    else{
        return NULL;
    }
}

template<class KEY, class T>
RBNode<KEY,T>* RBTree<KEY,T>::uncle(const RBNode<KEY,T>* p){
    if(p->parent==grandparent(p)->leftChild){
        return grandparent(p)->rightChild;
    }else{
        return	grandparent(p)->leftChild;
    }
}

template<class KEY, class T>
RBNode<KEY,T>* RBTree<KEY,T>::brother(const RBNode<KEY,T>* p){
    if(p==p->parent->rightChild){
        return p->parent->leftChild;
    }else{
        return p->parent->rightChild;
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::unclockwise(RBNode<KEY,T>* p){
    if(p->parent!=NULL){
        p->parent->rightChild=p->leftChild;
        if(p->leftChild!=NULL){
            p->leftChild->parent=p->parent;
        }
        p->leftChild=p->parent;
        RBNode<KEY,T>* temp= p->parent->parent;
        p->parent->parent=p;
        if(temp!=NULL){
            p->parent=temp;
            if(temp->leftChild==p->leftChild){
                temp->leftChild=p;
            }else{
                temp->rightChild=p;
            }
        }else{
            root=p;
            p->parent=NULL;
        }
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::clockwise(RBNode<KEY,T>* p){
    if(p->parent!=NULL){
        p->parent->leftChild=p->rightChild;
        if(p->rightChild!=NULL){                 //added
            p->rightChild->parent=p->parent;
        }
        p->rightChild=p->parent;
        RBNode<KEY,T>* temp= p->parent->parent;
        p->parent->parent=p;
        if(temp!=NULL){
            p->parent=temp;
            if(temp->leftChild==p->rightChild){
                temp->leftChild=p;
            }else{
                temp->rightChild=p;
            }
        }else{
            root=p;
            p->parent=NULL;
        }
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::insertCase1(RBNode<KEY,T>* p){
    if(p==root){
        p->color=BLACK;
    }else{
        this->insertCase2(p);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::insertCase2(RBNode<KEY,T>* p){
    if(p->parent->color==BLACK){
        return;
    }else{
        this->insertCase3(p);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::insertCase3(RBNode<KEY,T>* p){
    if(uncle(p)!=NULL && uncle(p)->color==RED){
        p->parent->color=BLACK;
        this->uncle(p)->color=BLACK;
        this->grandparent(p)->color=RED;
        this->insertCase1(grandparent(p));
    }else{
        this->insertCase4(p);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::insertCase4(RBNode<KEY,T>* p){
    if(p->parent == this->grandparent(p)->leftChild){
        if(p==p->parent->rightChild){
            this->unclockwise(p);
            p=p->leftChild;
        }
        this->insertCase5left(p);
    }else{
        if(p==p->parent->leftChild){
            this->clockwise(p);
            p=p->rightChild;
        }
        this->insertCase5right(p);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::insertCase5left(RBNode<KEY,T>* p){
    p->parent->color=BLACK;
    this->grandparent(p)->color=RED;
    this->clockwise(p->parent);
}

template<class KEY, class T>
void RBTree<KEY,T>::insertCase5right(RBNode<KEY,T>* p){
    p->parent->color=BLACK;
    this->grandparent(p)->color=RED;
    this->unclockwise(p->parent);
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteNode(const KEY &k){
    RBNode<KEY,T> *p=root, *pp=NULL, *x=NULL;
    while( p!=NULL && p->key!= k){                  //P移动到key为k的节点，没有k则返回
        if(p-> key < k){
            p=p->rightChild;
        }else{
            p=p->leftChild;
        }
    }
    if(p==NULL){        //查找失败，返回
        return;
    }
    if(p->leftChild==NULL || p->rightChild==NULL){
        pp=p;
    }else{
        pp=successor(p);
    }
    if(pp->leftChild!=NULL){
        x=pp->leftChild;
    }else{
        x=pp->rightChild;
    }
    if(x!=NULL){
        x->parent=pp->parent;
    }
    if(pp->parent==NULL){
        root=x;
    }else if(pp==pp->parent->leftChild){
        pp->parent->leftChild=x;
    }else{
        pp->parent->rightChild=x;
    }
    if(pp->key!=p->key){                 //替换目标节点k的值
        p->key=pp->key;
        p->element=pp->element;
    }
    nullNode=new RBNode<KEY,T>();
    if(pp->color==RED){
        delete pp;                        //动态空间中删除节点pp,下同
        return;
    }else if(x!=NULL && x->color==RED){
        x->color=BLACK;
        delete pp;
        return;
    }else if(pp->color==BLACK){
        nullNode->parent = pp->parent;
        delete pp;
        deleteCase1(nullNode);
    }
    delete nullNode;
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteCase1(RBNode<KEY,T>* p){
    if(p->parent!=NULL){
        deleteCase2(p);
    }
}
template<class KEY, class T>
void RBTree<KEY,T>::deleteCase2(RBNode<KEY,T>* p){
    RBNode<KEY,T>* b;
    if(p->parent->leftChild==NULL || p->parent->leftChild ==p ){
        b=p->parent->rightChild;
        if(b->color==RED){
            b->color=BLACK;
            b->parent->color=RED;
            this->unclockwise(b);
            deleteCase4_left(p);
        }else if(p->parent->color==BLACK && b->color==BLACK && (b->leftChild ==NULL || b->leftChild->color==BLACK ) && (b->rightChild ==NULL || b->rightChild->color==BLACK ) ){
            b->color=RED;
            deleteCase1(p->parent);
        }else{
        deleteCase4_left(p);
        }

    }else{
        std::cout<<p->parent->key<<std::endl;
        b=p->parent->leftChild;
        if(b->color==RED){
            b->color=BLACK;
            b->parent->color=RED;
            this->clockwise(b);
            deleteCase4_right(p);
        }else if(p->parent->color==BLACK && b->color==BLACK && (b->leftChild ==NULL || b->leftChild->color==BLACK ) && (b->rightChild ==NULL || b->rightChild->color==BLACK ) ){
            b->color=RED;
            deleteCase1(p->parent);
        }else{
            deleteCase4_right(p);
        }

    }
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteCase4_left(RBNode<KEY,T>* p){
    RBNode<KEY,T>* b;
    if(p->parent->rightChild!=NULL ){
        b=p->parent->rightChild;
        if((b->leftChild ==NULL || b->leftChild->color==BLACK ) && (b->rightChild ==NULL || b->rightChild->color==BLACK ) && p->parent->color==RED && b->color==BLACK){
            p->parent->color=BLACK;
            b->color=RED;
        }else{
            deleteCase5_left(b);
        }
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteCase4_right(RBNode<KEY,T>* p){
    RBNode<KEY,T>* b;
    if(p->parent->leftChild!=NULL){
        b=p->parent->leftChild;
        if((b->leftChild ==NULL || b->leftChild->color==BLACK ) && (b->rightChild ==NULL || b->rightChild->color==BLACK ) && p->parent->color==RED && b->color==BLACK){
            p->parent->color=BLACK;
            b->color=RED;
        }else{
            deleteCase5_right(b);
        }
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteCase5_left(RBNode<KEY,T>* p){
    if(p->leftChild!=NULL && p->leftChild->color==RED && p->color==BLACK && (p->rightChild==NULL || p->rightChild->color==BLACK)){
        p->leftChild->color=BLACK;
        p->color=RED;
        RBNode<KEY,T>* sl=p->leftChild;
        this->clockwise(p->leftChild);
        this->deleteCase6_left(sl);    //修改 2020年5月20日14:39:29，下同
    }else
    this->deleteCase6_left(p);
}

template<class KEY, class T>  // DT
void RBTree<KEY,T>::deleteCase5_right(RBNode<KEY,T>* p){
    if(p->rightChild!=NULL && p->rightChild->color==RED && p->color==BLACK && (p->leftChild==NULL || p->leftChild->color==BLACK)){
        p->rightChild->color=BLACK;
        p->color=RED;
        RBNode<KEY,T>* sr=p->rightChild;
        this->unclockwise(p->rightChild);
        this->deleteCase6_right(sr);
    }else
    this->deleteCase6_right(p);
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteCase6_left(RBNode<KEY,T>* p){
    if(p->rightChild!=NULL && p->rightChild->color==RED && p->color==BLACK){
        Color temp;
        temp=p->parent->color;
        p->parent->color=p->color;
        p->rightChild->color=BLACK;
        p->color=temp;
        this->unclockwise(p);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::deleteCase6_right(RBNode<KEY,T>* p){
    if(p->leftChild!=NULL && p->leftChild->color==RED && p->color==BLACK){
        Color temp;
        temp=p->parent->color;
        p->parent->color=p->color;
        p->leftChild->color=BLACK;
        p->color=temp;
        this->clockwise(p);
    }
}


template<class KEY, class T>
RBNode<KEY,T>*  successor(RBNode<KEY,T>* p){
    p=p->leftChild;
    while(p->rightChild!=NULL){
        p=p->rightChild;
    }
    return p;
}

template<class KEY, class T>
void updateNodePos(RBNode<KEY,T> *p)
{
    if(p->leftChild!=NULL){
        p->leftChild->layer=p->layer+1;
        p->leftChild->posXTree=p->posXTree-halfW/(pow(2,p->leftChild->layer));
        p->leftChild->posYTree=p->posYTree+nodeDistanceV;
        updateNodePos(p->leftChild);
    }
    if(p->rightChild!=NULL){
        p->rightChild->layer=p->layer+1;
        p->rightChild->posXTree=p->posXTree+halfW/(pow(2,p->rightChild->layer));
        p->rightChild->posYTree=p->posYTree+nodeDistanceV;
        updateNodePos(p->rightChild);
    }
}

template<class KEY, class T>
void RBTree<KEY,T>::updateTreePos(float w, float h)
{
    if(root==NULL)
        return;
    root->posXTree = rootPosX;
    root->posYTree = rootPosY;
    root->layer=0;
    updateNodePos(root);
}
#endif // REDBLACKTREE_H
