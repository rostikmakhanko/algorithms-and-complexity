#include <bits/stdc++.h>

using namespace std;

//idea of a tree have gotten from this e-olymp problem
//https://www.e-olymp.com/uk/problems/1522

const int MAXN=37,MAXP=55;//MAXN - maximal count of teams, MAXP - maximal count of players in the team

template<typename T>
class BinarySearchTree
{
    struct Node
    {
        Node *l,*r;//left and right sub-trees
        T key;//value

        Node (T key=T(),Node *l=NULL,Node *r=NULL):key(key),l(l),r(r){}
        ~Node(){}
    };

    Node *root;// root of the tree
    int cost;//cost of the tree can be calculated as sum of depth*frequency_of_queries for each node

    /*
        @param [l,r] is keys for which we'll build optimal BST
        @param create is 1 - we need to build new tree, 0 - just need to calculate cost
        @param dp[l][r] minimum cost of the tree has built from keys on [l,r]
        @param f is a vector of frequencies for each key
        @param keys is a vector of key values
    */
    Node *build_optimal(int l,int r,bool create,vector<vector<int>>& dp,vector<int>& f,vector<T>& keys)
    {
        if (l==r)//tree consists of one node
        {
            dp[l][r]=f[l];
            if (create) return new Node(keys[l],NULL,NULL);
            else return NULL;
        }
        int s=0;//sum of the frequencies
        for (int i=l;i<=r;i++)
        {
            s+=f[i];
        }
        int mn=1e9+7,mni=l;//mn is minimal cost, mi is index of preferable root
        for (int i=l;i<=r;i++)
        {
            if (i!=l&&!dp[l][i-1])
            {
                build_optimal(l,i-1,0,dp,f,keys);
            }
            if (i!=r&&!dp[i+1][r])
            {
                build_optimal(i+1,r,0,dp,f,keys);
            }
            int cur_cost=(i==l?0:dp[l][i-1])+(i==r?0:dp[i+1][r]);
            if (cur_cost<mn)
            {
                mn=cur_cost;
                mni=i;
            }
        }
        dp[l][r]=mn+s;//update optimal cost
        if (!create) return NULL;
        //build tree
        Node *node=new Node(keys[mni]);
        node->l=(l==mni?NULL:build_optimal(l,mni-1,1,dp,f,keys));
        node->r=(r==mni?NULL:build_optimal(mni+1,r,1,dp,f,keys));
        return node;
    }

    //build optimal BST with f as vector of frequencies and keys as key values
    int build(vector<int>& f,vector<T>& keys)
    {
        vector<vector<int>> dp(f.size());
        for (int i=0;i<dp.size();i++)
            dp[i].assign(f.size(),0);
        root=build_optimal(0,f.size()-1,1,dp,f,keys);
        return dp[0][f.size()-1];
    }

    //print tree, lr=='l' if left child, 'r' if it's right child
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

    BinarySearchTree():root(NULL){}

    BinarySearchTree(vector<int> f,vector<T> keys):BinarySearchTree()
    {
        cost=build(f,keys);
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
vector<Team> teams;//vector of teams
vector<int> frequencies;//vector of frequencies of queries for each team

void read()
{
    //freopen("input.txt","r",stdin);
    ifstream in;
    in.open("input.txt");
	in>>n;
	in.ignore();
	for (int i=0;i<n;i++)
    {
        Team team;
        getline(in,team.name);
        int k;//count of players
        in>>k;
        in.ignore();
        for (int j=0;j<k;j++)
        {
            string s;
            getline(in,s);
            team.players.push_back(s);
        }
        teams.push_back(team);
    }
    in.close();
    cout<<"Enter frequencies of queries for "<<n<<" teams:"<<endl;
    for (int i=0;i<n;i++)
    {
        cout<<"Frequency for "<<teams[i].name<<": ";
        int f;
        cin>>f;
        frequencies.push_back(f);
    }
}

int main()
{
    read();
    BinarySearchTree<Team> bst(frequencies,teams);
    bst.print();
    return 0;
}
