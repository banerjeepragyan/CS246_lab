#include<bits/stdc++.h>

#define pb push_back
#define all(a) a.begin(),a.end()
#define cpresent(container,element) (find(all(container),element)!=container.end())
#define rep(i, a, b) 	for(int i = a; i < b; ++i)
using namespace std;
struct Bucket
{
	int ld=0;
	vector<int>a;
	bool disp = false;
};
map<int,Bucket *>bmap;
int gd,bucket_capacity;
void display()
{
	cout << "************************************\n";
	cout << gd << endl;
	int kk = 0;
	rep(i,0,1<<gd)
	{
		if(bmap[i]->disp==false)
		{
			bmap[i]->disp  = true;
			kk++;
		}
	}
	cout << kk << endl;
	rep(i,0,1<<gd)
	{
		bmap[i]->disp  = false;
	}
	rep(i,0,1<<gd)
	{
		if(bmap[i]->disp==false)
		{
			cout<<bmap[i]->a.size();
			cout<<" "<<bmap[i]->ld<<"\n";
			bmap[i]->disp  = true;
		}
	}
}
int search(int n)
{
	int h = n & ((1<<gd) - 1);
	return cpresent(bmap[h]->a,n);
}
void split(int sp_Index,Bucket * sp_Bucket)
{
	Bucket * newB = new Bucket;
	vector<int>temp;
	for(auto i : sp_Bucket->a)
	{
		temp.pb(i);
	}
	sp_Bucket->a.clear();
	if(sp_Bucket->ld == gd)
	{
		bmap[sp_Index ^ (1<<gd)] = newB;
		if(gd!=0)
		{
			rep(i,0,1<<gd)
			{
				if(i!=sp_Index)
					bmap[i ^ (1<<gd)] = bmap[i];
			}
		}
		gd++;
		newB->ld = ++ sp_Bucket->ld;
		for(auto i : temp)
		{
			int h = i & ((1<<gd) -1);
			bmap[h]->a.pb(i);
		}
	}
	else
	{
		int k = sp_Index & (1<<(sp_Bucket->ld) - 1);
		vector<int>indices;
		rep(i,0,1<<gd)
		{
			int last = i & (1<<(sp_Bucket->ld)) - 1;
			if(last==k)
				indices.pb(i);
		}
		newB->ld = ++sp_Bucket->ld;
		rep(i, indices.size()/2, indices.size())
		{
			bmap[indices[i]] = newB;
		}
		for(auto i : temp)
		{
			int h = i & ((1<<gd) -1);
			bmap[h]->a.pb(i);
		}
	}
}
void insert(int n)
{
	int h = n & ((1<<gd) - 1);
	if(bmap[h]->a.size() < bucket_capacity)
		bmap[h]->a.pb(n);
	else
	{
		split(h,bmap[h]);
		insert(n);
	}
}
void merge()
{
	for(int i = 0; i<(1<<(gd-1));i++)
	{
		int h = i & ( ( 1<< ((bmap[i]->ld)-1) ) - 1);
		Bucket *curr = bmap[i];
		Bucket *matched = bmap[ i ^ (1<<(bmap[i]->ld-1)) ];
		if(curr != matched)
		{
			int sz = curr->a.size() + matched->a.size();
			if(sz <= bucket_capacity)
			{
				copy(all(matched->a),back_inserter(curr->a));
				vector<int>inds;
				for(auto i : bmap)
				{
					if(i.second==matched)
						inds.pb(i.first);
				}
				delete matched;
				for(auto j : inds)
					bmap[j] = bmap[i];
				bmap[i]->ld --;
			}
		}
	}
}
void shrink()
{
	set <Bucket *>buckets;
	for(auto i : bmap)
		buckets.insert(i.second);
	int count = 0;
	int sz = buckets.size();
	for(auto i : buckets)
		if ( (gd - (i->ld)) == 1)
			count++;
	if(count==sz)
		gd--;
}
void del(int n)
{
	if(search(n))
	{
		for(auto i : bmap)
		{
			auto it = find(all(i.second->a),n);
			if(it!=i.second->a.end())
			{
				i.second->a.erase(it);
				break;
			}
		}
	}
	else
		cout<< n <<" : Element not found to delete \n";
}
void menu()
{
	int tmp[2];
	for (int  i = 0; i < 2; i++)
		tmp[i] = 0;
	int choice;
	int n;
	while(1)
	{
		string line1;
		getline(cin,line1);
		stringstream ss1(line1);
		int j = 0;
		int t;
		while (ss1>>t)
		{
			tmp[j] = t;
			j++;
		}
		choice = tmp[0];
		switch(choice)
		{
			case 2:
				n = tmp[1];
				insert(n);
				break;
			case 4:
				n = tmp[1];
				del(n);
				break;
			case 5:
				display();
				break;
			case 3:
				n = tmp[1];
				if(search(n))
					cout<< n <<" : Found\n";
				else
					cout<< n <<" : Not Found\n";
				break;
			case 6:
				exit(0);
			case 0:
				break;
			default:
				cout<<"Invalid choice ! : " << choice << endl;
		}
	}
}
int main()
{
	#ifndef ONLINE_JUDGE
        freopen("input1.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
	int gdk = 0;
	cin >> gdk;
	cin>>bucket_capacity;
	bmap[0] = new Bucket;
	for (int i = gdk; i > 1 ; i=i/2)
	{
		split(0, bmap[0]);
	}
	menu();
}