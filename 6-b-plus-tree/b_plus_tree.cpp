#include <bits/stdc++.h>

using namespace std;

const int MAXN=37,MAXP=55;//MAXN - maximal count of teams, MAXP - maximal count of players in the team


template<typename T>
class BPlusTree
{
    static const int B=31;//maximal power of the tree

    struct Node
    {
        int keys_count;//count of keys in the node
        int key[B];//key values
        void *pointers[B+1];//children
        Node *parent;
        bool leaf;//1 - node is leaf

        Node()
        {
            keys_count=0;
            pointers[0]=NULL;
            parent=0;
            leaf=0;
        }
    };

    Node *root;
    int size;

    //print the tree
    void print(Node *node)
    {
        for (int i=0;i<=node->keys_count;i++)
        {
            if (node->leaf)
            {
                if (i==0) continue;
                cout<<node->key[i-1]<<": "<<(string)*(T*)(node->pointers[i])<<" ";
            }
            else
            {
                print((Node*)(node->pointers[i]));
            }
        }
    }

    public:

        BPlusTree()
        {
            root=new Node();
            root->leaf=1;
            size=0;
        }

        //find leaf with key value equal key
        Node *find_leaf(int key)
        {
            Node *cur=root;
            while (!(cur->leaf))
            {
                for (int i=0;i<=cur->keys_count;i++)
                    if (i==cur->keys_count||key<cur->key[i])
                    {
                        cur=(Node*)(cur->pointers[i]);
                        break;
                    }
            }
            return cur;
        }

        //insert item into leaf
        void insert_in_leaf(Node *node,int key,void *value)
        {
            int k=0;
            while (k<node->keys_count&&node->key[k]<key)
            {
                k++;
            }
            for (int i=node->keys_count;i>k;i--)
            {
                node->key[i]=node->key[i-1];
            }
            for (int i=node->keys_count+1;i>k+1;i--)
            {
                node->pointers[i]=node->pointers[i-1];
            }
            node->key[k]=key;
            node->pointers[k+1]=value;
            node->keys_count++;
            if (node->keys_count==B)//leaf is full of items
            {
                split(node);
            }
        }

        //insert into the tree
        bool insert(int key,void *value)
        {
            Node *leaf=find_leaf(key);
            for (int i=0;i<leaf->keys_count;i++)
                if (key==leaf->key[i])
                {
                    return 0;
                }
            insert_in_leaf(leaf,key,value);
            size++;
            return 1;
        }

        //splitting the tree
        void split(Node *node)
        {
            Node *new_node=new Node();
            int m=node->key[B/2];//middle key
            //move right part of elements into new Node
            new_node->keys_count=(B-1)/2;
            for (int i=0;i<new_node->keys_count;i++)
            {
                new_node->key[i]=node->key[i+(B/2+1)];
                new_node->pointers[i]=node->pointers[i+(B/2+1)];
            }
            new_node->pointers[new_node->keys_count]=node->pointers[B];
            node->keys_count=B/2;
            if (node->leaf)//node is leaf
            {
                node->keys_count++;
                new_node->pointers[0]=node->pointers[0];
                node->pointers[0]=new_node;
                new_node->leaf=1;
                m=node->key[B/2+1];
            }
            if (!node->parent)//node is a root
            {
                root=new Node();
                root->key[0]=m;
                root->pointers[0]=node;
                root->pointers[1]=new_node;
                root->keys_count=1;
                node->parent=new_node->parent=root;
            }
            else
            {
                new_node->parent=node->parent;
                insert_in_leaf((Node*)(node->parent),m,(void*)new_node);
            }
        }

        //erase from leaf element with key value key
        void erase_from_leaf(Node *node,int key)
        {
            int k=0;
            while (key!=node->key[k])
            {
                k++;
            }
            for (int i=k;i<node->keys_count-1;i++)
                node->key[i]=node->key[i+1];
            for (int i=k+1;i<node->keys_count;i++)
                node->pointers[i]=node->pointers[i+1];
            node->keys_count--;
        }

        //erase element from the tree
        bool erase(int key)
        {
            Node *leaf=find_leaf(key);
            for (int i=0;i<leaf->keys_count;i++)
            {
                if (key==leaf->key[i])
                {
                    erase_from_leaf(leaf,key);
                    size--;
                    return 1;
                }
            }
            return 0;
        }

        //find element by key
        T* search(int key)
        {
            Node *leaf=find_leaf(key);
            for (int i=0;i<leaf->keys_count;i++)
            {
                if (key==leaf->key[i])
                {
                    return (T*)(leaf->pointers[i+1]);
                }
            }
            return NULL;//there's no element with this key
        }

        int get_size()
        {
            return size;
        }

        void print()
        {
            print(root);
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
BPlusTree<Team> bpt;
vector<int> keys;

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
    cout<<"Enter keys for "<<n<<" companies:"<<endl;
    for (int i=0;i<n;i++)
    {
        int key;
        cin>>key;
        keys.push_back(key);
    }
}

void text_interface()
{
    while (1)
    {
        cout<<"There are 5 types of queries: "<<endl;
        cout<<"1 - Add new team"<<endl;
        cout<<"2 - Find team by key"<<endl;
        cout<<"3 - Delete team by key"<<endl;
        cout<<"4 - Print B+ tree"<<endl;
        cout<<"And"<<endl;
        cout<<"5 - Exit"<<endl;
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
            int key;
            cout<<"Key: ";
            cin>>key;
            if (!bpt.insert(key,new Team(teams[n-1].name)))
            {
                cout<<"Cannot insert team with existing key"<<endl;
            }
            else
            {
                cout<<"Successfully inserted!"<<endl;
            }
        }
        else if (t==2)
        {
            cout<<"Key: ";
            int key;
            cin>>key;
            if (bpt.search(key)) cout<<(string)(*bpt.search(key))<<endl;
            else cout<<"NO"<<endl;
        }
        else if (t==3)
        {
            int key;
            cout<<"Key: ";
            cin>>key;
            if (!bpt.erase(key))
            {
                cout<<"There's no team with such key"<<endl;
            }
            else
            {
                cout<<"Successfully deleted!"<<endl;
            }
        }
        else if (t==4)
        {
            bpt.print();
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
    {
        bpt.insert(keys[i],new Team(teams[i].get_name()));
        bpt.print();
    }
    text_interface();
    return 0;
}
