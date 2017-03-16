#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <queue>
using namespace std;

struct Station{
	string name;
	vector<string> last_station,line;//记录从哪一号线，从哪一站来
	Station(){
		name="";
	}
	void set(string& sname){
		name=sname;
	}
	void setLastStation(string name){
		last_station.push_back(name);
	}
	void setFromLine(string l){
		line.push_back(l);
	}
};

typedef vector<pair<string,string> > InfoList;

class Subway{
private:
	map<string,InfoList> adj_table;
	map<string,Station> stations;//用于记录Station信息：来自哪里
	void handle_file(const char* name){
		//从文件读入地铁信息
		ifstream infi(name);
		string line;
		while(getline(infi,line)){
			string temp;
			if((line.size()==1&&isdigit(line[0]))||
				line=="3_1"||line=="APM"){
				Station last,curr;
				while(getline(infi,temp)){
					if(temp.size()==0)break;
					curr.set(temp);
					stations[temp]=curr;
					if(last.name!=""){
						adj_table[temp].push_back(make_pair(last.name,line));
						adj_table[last.name].push_back(make_pair(temp,line));
					}
					last=curr;
					//cout<<temp<<endl;
				}
			}
		}
		
	}
	void bfs(map<string,bool> &book,string from,const string& dest){
		queue< pair<string,string> > wait;
		book[from]=1;
		Station curr=stations[from];
		//初始处理
		for(auto a:adj_table[from]){
			book[a.first]=1;//change:入队的都标记了访问
			stations[a.first].setLastStation(from);
			stations[a.first].setFromLine(a.second);
			wait.push(a);
		}
		//bfs循环
		while(!wait.empty()){
			auto now_pair=wait.front();wait.pop();
			//cout<<now_pair.first<<" "<<now_pair.second<<endl;
			from=now_pair.first;
			//book[from]=1;//标记已访问
			cout<<from<<" "<<book[from]<<endl;
			if(from==dest){
				return;
			}
			for(auto a:adj_table[from]){
				if(!book[a.first]){
					cout<<a.first<<" "<<book[a.first]<<endl;
					book[a.first]=1;//change:入队的都标记了访问
					stations[a.first].setLastStation(from);
					stations[a.first].setFromLine(a.second);
					wait.push(a);
				}
			}
		}
	}
	void bfs2(map<string,bool> &book,string from,const string& dest);
	void dfs_print(InfoList route,string start,string dest,string);
	void print_way(string start,string dest){
		Station curr=stations[dest];
		for(int k=0;k<curr.last_station.size();k++){
			string now_line=curr.line[k];
			InfoList route;
			route.push_back(make_pair(dest,now_line));
			//暴力枚举所有路线
			dfs_print(route,start,curr.last_station[k],now_line);
		}
	}
public:
	Subway(string filename){//从文件读入地铁信息
		handle_file(filename.c_str());
	};
	void find_way(string& start,string& dest){
		map<string,bool> book;
		book[start]=1;
		bfs2(book,start,dest);
		/*for(auto a:book){
			cout<<a.first<<" "<<a.second<<endl;
		}*/
		//cout<<"最短路线为"<<endl;
		cout<<"The shortest way is:"<<endl;
		print_way(start,dest);
		//cout<<"共经过"<<count<<"站"<<endl;
	}
	void print(){
		for(auto a:adj_table){
			cout<<a.first<<": ";
			for(auto b:a.second)
				cout<<b.first<<" ";
			cout<<endl;
		}
	}
	void print_map(){
		for(auto a:stations){
			cout<<a.second.name<<endl;
		}
	}
};


int main(){
	string subwayname="subway.txt";
	Subway g(subwayname);
	string start,dest;
	//cout<<"请输入始发站："<<endl;
	cout<<"start from:"<<endl;
	cin>>start;
	//cout<<"请输入目的地："<<endl;
	cout<<"destination:"<<endl;
	cin>>dest;
	g.find_way(start,dest);
	//g.print();
	//g.print_map();
}

int SHORT=10000;
bool p=0;
void Subway::dfs_print(InfoList route,string start,string dest,string line){
	if (p==1)return;
	if(dest==start){
		route.push_back(make_pair(start,route.back().second));
		if(route.size()>SHORT)return;
		string temp_line;//记录是否换乘
		cout<<endl<<"ROUTE:"<<endl;
		for(int i=route.size()-1;i>=0;i--){
			if(i==route.size()-1){
				//cout<<route[i].second<<"号线："<<route[i].first;
				cout<<"Line "<<route[i].second<<": "<<route[i].first;
				temp_line=route[i].second;
			}
			else if(temp_line!=route[i].second){
				temp_line=route[i].second;
				//cout<<endl<<"换乘"<<temp_line<<"号线："<<route[i].first;
				cout<<endl<<"change to Line "<<temp_line<<": "<<route[i].first;
			}else{
				cout<<" - "<<route[i].first;
			}
		}
		int count=route.size();
		cout<<endl<<"Totally "<<count<<" stations"<<endl;
		p=1;
	}else{
		Station curr=stations[dest];
		for(int k=0;k<curr.last_station.size();k++){
			dest=curr.last_station[k];
			line=curr.line[k];
			route.push_back(make_pair(curr.name,line));
			dfs_print(route,start,dest,line);
		}
	}
}
//输出所有长度相同最短的路径版本
void Subway:: bfs2(map<string,bool> &book,string from,const string& dest){
	queue< pair<string,string> > wait;
	book[from]=1;
	Station curr=stations[from];
	//初始处理
	for(auto a:adj_table[from]){
		//book[a.first]=1;//change:入队的都标记了访问
		//cout<<a.first<<" "<<book[a.first]<<" "<<a.second;
		stations[a.first].setLastStation(from);
		stations[a.first].setFromLine(a.second);
		wait.push(a);
	}
	//bfs循环
	bool reach=0;
	while(!wait.empty()){
		auto now_pair=wait.front();wait.pop();
		//cout<<now_pair.first<<" "<<now_pair.second<<endl;
		from=now_pair.first;
		book[from]=1;//标记已访问
		//cout<<from<<" "<<book[from]<<endl;
		if(from==dest){
			reach=1;
		}
		for(auto a:adj_table[from]){
			if(!book[a.first]&&!reach){
				//cout<<a.first<<" "<<book[a.first]<<" "<<a.second;
				//book[a.first]=1;//change:入队的都标记了访问
				stations[a.first].setLastStation(from);
				stations[a.first].setFromLine(a.second);
				wait.push(a);
			}
		}
	}
}