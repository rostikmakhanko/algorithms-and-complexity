#include <bits/stdc++.h>

using namespace std;

const int MAXN=37,MAXP=55;//MAXN - maximal count of teams, MAXP - maximal count of players in the team

template<typename T>
class SplayTree
{
private:

    int size;

    struct Node
    {
        Node *l,*r;//left and right children
        Node *p;//parent node
        T key;//value
        Node (const T& key=T()):l(0),r(0),p(0),key(key){}
        ~Node (){}
    };

    Node *root;//root of the tree

    void left_rotate(Node *x)//left rotate around x
    {
        Node *y=x->r;
        if (y)
        {
            x->r=y->l;
            if (y->l) y->l->p=x;
            y->p=x->p;
        }
        if (!(x->p)) root=y;
        else if (x==x->p->l) x->p->l=y;
        else x->p->r=y;
        if (y) y->l=x;
        x->p=y;
    }

    void right_rotate(Node *x)//right rotate around x
    {
        Node *y=x->l;
        if (y)
        {
            x->l=y->r;
            if (y->r) y->r->p=x;
            y->p=x->p;
        }
        if (!(x->p)) root=y;
        else if (x==x->p->l) x->p->l=y;
        else x->p->r=y;
        if (y) y->r=x;
        x->p=y;
    }

    void splay(Node *x)//x is node which we want to replace into root
    {
        while (x->p)
        {
            if (!(x->p->p))
            {
                if (x->p->l==x) right_rotate(x->p);
                else left_rotate(x->p);
            }
            else if (x->p->l==x&&x->p->p->l==x->p)
            {
                right_rotate(x->p->p);
                right_rotate(x->p);
            }
            else if (x->p->r==x&&x->p->p->r==x->p)
            {
                left_rotate(x->p->p);
                left_rotate(x->p);
            }
            else if (x->p->l==x&&x->p->p->r==x->p)
            {
                right_rotate(x->p);
                left_rotate(x->p);
            }
            else
            {
                left_rotate(x->p);
                right_rotate(x->p);
            }
        }
    }

    void replace(Node *u,Node *v)//change node u with the node v
    {
        if (!(u->p)) root=v;
        else if (u==u->p->l) u->p->l=v;
        else u->p->r=v;
        if (v) v->p=u->p;
    }

    Node *find_min(Node *node)//find minimum element by value
    {
        while (node->l) node=node->l;
        return node;
    }

    Node *find_max(Node *node)//find maximum element by value
    {
        while (node->r) node=node->r;
        return node;
    }

    Node *find_by_key(const T& key)
    {
        Node *r=root;
        while (r)
        {
            if (r->key<key) r=r->r;
            else if (r->key>key) r=r->l;
            else
            {
                splay(r);//splay tree to make r as root of tree
                return r;
            }
        }
        return NULL;
    }

    void show(Node *node,int depth,char lr)
    {
        if (!node) return;
        for (int i=0;i<depth;i++)
            cout<<" ";
        cout<<lr<<" "<<(string)node->key<<endl;
        show(node->l,depth+1,'l');
        show(node->r,depth+1,'r');
    }

public:

    SplayTree():root(0),size(0){}

    void insert(const T& key)//paste key into tree
    {
        Node *r=root;
        Node *p=NULL;
        while (r)
        {
            p=r;
            if (r->key<key) r=r->r;
            else r=r->l;
        }
        r=new Node(key);
        r->p=p;
        if (!p) root=r;
        else if (p->key<r->key) p->r=r;
        else p->l=r;
        splay(r);
        size++;
    }

    bool find(const T& key)
    {
        if (find_by_key(key)!=NULL) return 1;
        else return 0;
    }

    void erase(const T& key)
    {
        Node *f=find_by_key(key);
        if (!f) return;
        splay(f);
        if (!(f->l)) replace(f,f->r);
        else if (!(f->r)) replace(f,f->l);
        else
        {
            Node *y=find_min(f->r);
            if (y->p!=f)
            {
                replace(y,y->r);
                y->r=f->r;
                y->r->p=y;
            }
            replace(f,y);
            y->l=f->l;
            y->l->p=y;
        }
        delete f;
        size--;
    }

    T& find_min()
    {
        return find_min(root)->key;
    }

    void print()
    {
        show(root,0,'*');
        cout<<endl;
    }
};

class Team
{
private:

    string name;
    vector<string> players;

    friend void read();
    friend void print();
    friend void text_interface();
    friend bool operator <(const Team& a,const Team& b);
    friend bool operator >(const Team& a,const Team& b);

public:

    Team(){}

    Team(string name)
    {
        this->name=name;
    }

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

    string get_name()
    {
        return name;
    }
};

//compare two teams by name
bool operator <(const Team& a,const Team& b)
{
    return a.name<b.name;
}
bool operator >(const Team& a,const Team& b)
{
    return a.name>b.name;
}

int n;//count of teams
Team teams[MAXN];
SplayTree<Team> st;

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

void text_interface()
{
    while (1)
    {
        cout<<"There are 5 types of queries: "<<endl;
        cout<<"1 - Add new team"<<endl;
        cout<<"2 - Find team by name"<<endl;
        cout<<"3 - Print first team in alphabetic order"<<endl;
        cout<<"4 - Delete team by name"<<endl;
        cout<<"5 - Print splay tree"<<endl;
        cout<<"And"<<endl;
        cout<<"6 - Exit"<<endl;
        cout<<"Choose type of query: ";
        int t;
        cin>>t;
        if (t==1)
        {
            cin.ignore();
            cout<<"Name of team: ";
            string s;
            getline(cin,s);
            teams[n].name=s;
            n++;
            cout<<"Count of players: ";
            int k;
            cin>>k;
            cin.ignore();
            for (int i=0;i<k;i++)
            {
                string s1;
                getline(cin,s1);
                teams[n-1].players.push_back(s1);
            }
            st.insert(teams[n-1]);
        }
        else if (t==2)
        {
            cin.ignore();
            cout<<"Name of the team: ";
            string s;
            getline(cin,s);
            if (st.find(Team(s))) cout<<"YES"<<endl;
            else cout<<"NO"<<endl;
        }
        else if (t==3)
        {
            cout<<(st.find_min()).name<<endl;
        }
        else if (t==4)
        {
            cin.ignore();
            cout<<"Name of the team: ";
            string s;
            getline(cin,s);
            st.erase(s);
        }
        else if (t==5)
        {
            st.print();
        }
        else
        {
            exit(0);
        }
    }
}

int main()
{
    read();
    for (int i=0;i<n;i++)
        st.insert(teams[i]);
    st.print();
    text_interface();
    return 0;
}
