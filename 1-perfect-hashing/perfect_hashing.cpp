#include<bits/stdc++.h>

using namespace std;

typedef unsigned int uint;

const int PRIME=41,MAX_TEAM_COUNT=37,MAX_PLAYER_COUNT=55;

const string INPUT_FILENAME="input.txt";

struct Team
{
    string name;
    vector<string> players;
};

int n;//count of teams
Team teams[MAX_TEAM_COUNT];
//vector with index i consists of strings with hash=i
vector<string> hashed_players[MAX_PLAYER_COUNT],hashed_teams[MAX_TEAM_COUNT];

void read()
{
    ifstream in(INPUT_FILENAME);
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

uint get_hash(string s)
{
    uint ans=0,pow=1;
    for (int i=0;i<s.size();i++)
    {
        ans+=s[i]*pow;
        pow*=PRIME;
    }
    return ans;
}

void hashing()
{
    for (int i=0;i<n;i++)
    {
        int ht=get_hash(teams[i].name)%MAX_TEAM_COUNT;//hash of the team's name
        hashed_teams[ht].push_back(teams[i].name);
        for (int j=0;j<teams[i].players.size();j++)
        {
            int hp=get_hash(teams[i].players[j])%MAX_PLAYER_COUNT;//hash of the player's name
            hashed_players[hp].push_back(teams[i].players[j]);
        }
    }
}

void text_interface()
{
    char t=' ';
    while (true)
    {
        cout<<"There are 2 types of queries:"<<endl;
        cout<<"1 - is team with given name exists"<<endl;
        cout<<"2 - is player with given name exists"<<endl;
        cout<<"And"<<endl;
        cout<<"3 - exit"<<endl;
        cout<<"Choose type of query: ";
        cin>>t;
        if (t=='1')
        {
            cout<<"Enter name of team: ";
            string qs;
            cin.ignore();
            getline(cin,qs);
            int hash_qs=get_hash(qs)%MAX_TEAM_COUNT;
            bool f=0;
            for (int i=0;i<hashed_teams[hash_qs].size();i++)
            {
                if (hashed_teams[hash_qs][i]==qs)
                {
                    f=1;
                    break;
                }
            }
            if (f) cout<<"YES"<<endl;
            else cout<<"NO"<<endl;
        }
        else if (t=='2')
        {
            cout<<"Enter name of player: ";
            string qs;
            cin.ignore();
            getline(cin,qs);
            int hash_qs=get_hash(qs)%MAX_PLAYER_COUNT;
            bool f=0;
            for (int i=0;i<hashed_players[hash_qs].size();i++)
            {
                if (hashed_players[hash_qs][i]==qs)
                {
                    f=1;
                    break;
                }
            }
            if (f) cout<<"YES"<<endl;
            else cout<<"NO"<<endl;
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
    hashing();
    text_interface();
    return 0;
}
