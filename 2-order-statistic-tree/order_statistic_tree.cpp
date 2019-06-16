#include <bits/stdc++.h>

using namespace std;

const int MAXN=37,MAXP=55;//MAXN - maximal count of teams, MAXP - maximal count of players in the team

template <class T>
class RedBlackTree
{
private:

    struct Node
    {
        Node *l,*r;//pointers on the left and right sub-trees
        T& value;//value of node
        bool red;//1 - node is red, 0 - node is black
        int child_count;//count of children nodes

        Node(T& value) : value(value)
        {
            this->l=this->r=0;
            this->red=1;
            this->child_count=0;
        }
    };

    Node *tree_root;//root of the tree
    int size;//count of nodes in the tree

    void delete_node(Node *node)//deleting of the node
    {
        delete node;
    }

    void clear(Node *node)//deleting of all nodes in the tree
    {
        if (!node) return;
        clear(node->l);
        clear(node->r);
        delete_node(node);
    }

    void show(Node *node,int depth,char lr)
    {
        if (!node) return;
        for (int i=0;i<depth;i++)
            cout<<" ";
        cout<<lr<<" "<<(string)node->value<<" ("<<(node->red?"red":"black")<<")"<<endl;
        show(node->l,depth+1,'l');
        show(node->r,depth+1,'r');
    }

    void update_children_count(Node *node)
    {
        if (!node) return;
        node->child_count=(node->l?node->l->child_count+1:0)+(node->r?node->r->child_count+1:0);
    }

    Node *rotate21(Node *node)//left rotate
    {
        Node *p2=node->r;
        Node *p21=p2->l;
        p2->l=node;
        node->r=p21;
        update_children_count(node);
        update_children_count(node->l);
        update_children_count(p2->l->r);
        return p21;
    }

    Node *rotate12(Node *node)//right rotate
    {
        Node *p1=node->l;
        Node *p12=p1->r;
        p1->r=node;
        node->l=p12;
        update_children_count(node);
        update_children_count(node->l);
        update_children_count(p1->r->l);
        return p1;
    }

    void balance_insert(Node **root)//insertion of the node with balancing
    {
        Node *p1,*p2,*px1,*px2;
        Node *node=*root;
        if (node->red) return;
        p1=node->l;
        p2=node->r;
        //find two neighboring red nodes
        if (p1)
        {
            if (p1->red)
            {
                px2=p1->r;
                if (px2)
                {
                    if (px2->red) p1=node->l=rotate21(p1);
                }
                px1=p1->l;
                if (px1)
                {
                    if (px1->red)
                    {
                        node->red=1;
                        p1->red=0;
                        if (p2)
                        {
                            if (p2->red)//redrawing of nodes
                            {
                                px1->red=1;
                                p2->red=0;
                                return;
                            }
                        }
                        *root=rotate12(node);
                    }
                }
            }
        }
        //similar let's find two neighboring red nodes in the right sub-tree
        if (p2)
        {
            if (p2->red)
            {
                px1=p2->l;
                if (px1)
                {
                    if (px1->red) p2=node->r=rotate12(p2);
                }
                px2=p2->r;
                if (px2)
                {
                    if (px2->red)
                    {
                        node->red=1;
                        p2->red=0;
                        if (p1)
                        {
                            if (p1->red)//redrawing of nodes
                            {
                                px2->red=1;
                                p1->red=0;
                                return;
                            }
                        }
                        *root=rotate21(node);
                    }
                }
            }
        }
        update_children_count(node->l);
        update_children_count(node->r);
        update_children_count(node);
    }

    //recursively insertion, if returns true than there's no change or balancing is not needed
    bool insert(T& value,Node **root)
    {
        Node *node=*root;
        if (!node)
        {
            *root=new Node(value);
        }
        else
        {
            (*root)->child_count++;
            if (value==node->value) return 1;
            if (insert(value,value<node->value?&node->l:&node->r)) return 1;
            //size++;
            balance_insert(root);
        }
        return 0;
    }

    T& statistic(Node *root,int k)//find k-th element in the non-decreasing sequence
    {
        int cur=(root->l?root->l->child_count+1:0);
        if (k==cur+1)
        {
            return root->value;
        }
        else if (k<=cur)
        {
            return statistic(root->l,k);
        }
        else
        {
            return statistic(root->r,k-cur-1);
        }
    }

public:

    RedBlackTree()
    {
        size=0;
        tree_root=0;
    }

    //print red-black tree
    void print()
    {
        show(tree_root,0,'*');
        cout<<endl;
    }

    //insert val in the tree
    void insert(T& val)
    {
        size++;
        insert(val,&tree_root);
        if (tree_root) tree_root->red=0;
    }

    //delete all nodes
    void clear()
    {
        clear(tree_root);
        tree_root=0;
    }

    //find k-th minimum in the tree
    T& statistic(int k)
    {
        if (k>size)
        {
            cout<<"Wrong input"<<endl;
            exit(0);
        }
        return statistic(tree_root,k);
    }

    int get_size()
    {
        return size;
    }
};

class Team
{
private:

    string name;
    vector<string> players;

    friend void read();
    friend void print();

public:

    operator string()
    {
        return name;
    }

    operator <(Team& a)
    {
        return this->name<a.name;
    }

    operator ==(Team& a)
    {
        return this->name==a.name;
    }

    operator >(Team& a)
    {
        return this->name>a.name;
    }
};

int n;//count of teams
Team teams[MAXN];
RedBlackTree<Team> rbt;

void read()
{
    //freopen("input.txt","r",stdin);
    ifstream in;
    in.open("input.txt");
	in>>n;
	in.ignore();
	for (int i=0;i<n;i++)
    {
        getline(in,teams[i].name);
        int k;//count of players
        in>>k;
        in.ignore();
        for (int j=0;j<k;j++)
        {
            string s;
            getline(in,s);
            teams[i].players.push_back(s);
        }
    }
    in.close();
}

void print()
{
    for (int i=0;i<n;i++)
    {
        string s=teams[i].name;
        cout<<s<<endl;
        cout<<teams[i].players.size()<<endl;
        for (int j=0;j<teams[i].players.size();j++)
            cout<<teams[i].players[j]<<endl;
    }
}

void text_interface()
{
    cout<<"Print number of queries: ";
    int q;
    cin>>q;
    while (q--)
    {
        int s;
        cin>>s;
        cout<<(string)rbt.statistic(s)<<endl;
    }
}

int main()
{
    read();
    rbt.print();
    for (int i=0;i<n;i++)
    {
        rbt.insert(teams[i]);
    }
    rbt.print();
    text_interface();
    return 0;
}
