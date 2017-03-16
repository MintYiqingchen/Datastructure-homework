#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <cstddef>
#include <map>
#include <cstdio>
using namespace std;
/*修改前版本
void decode(string &name,map<string,char> &res_table,string& outname){
	ifstream in(name);
	ofstream out(outname);
	string s="";
	while(in.peek()!=EOF){
		char c;
		in.get(c);
		s+=c;
		map<string,char>::iterator it=res_table.find(s);
		if(it!=res_table.end()){
			s="";
			out<<it->second;
		}
	}
	cout<<"Done."<<endl;
}*/
#include <bitset>
void decode(string &name,map<string,char> &res_table,string& outname){
	ifstream in(name,ios::binary);
	ofstream out(outname);
	string s="";
	char *temp=new char();
	in.seekg(-1,ios::end);
	int size=in.tellg();//get useful size of the file
	in.read(temp,1);
	cout<<"size:"<<size<<endl;
	in.seekg(0,ios::beg);
	int count=(*temp)-'0';//useful bits in the last byte
	//cout<<"count: "<<count<<endl;
	--count;
	delete temp;temp=NULL;

	temp=new char();
	/*
	while(in.peek()!=EOF){
		in.read(temp,1);
		if(first){
			first=0;
			for(int i=count;i>=0;i--){
				if((*temp)&(1<<i)==1){
					s+='1';
				}else{
					s+='0';
				}
				map<string,char>::iterator it=res_table.find(s);
				if(it!=res_table.end()){
					s="";
					out<<it->second;
				}
			}
		}else{
			for(int i=7;i>=0;i--){
				if(*temp&(1<<i)==1){
					s+='1';
				}else{
					s+='0';
				}
				map<string,char>::iterator it=res_table.find(s);
				if(it!=res_table.end()){
					s="";
					out<<it->second;
				}
			}
		}
	}*/
	while(in.tellg()<size){
		//cout<<"tellg: "<<in.tellg()<<endl;
		in.read(temp,1);
		//cout<<"temp:"<<bitset<8>(*temp)<<endl;
		if(in.tellg()==size){//if it is the last byte
			for(int i=count;i>=0;i--){
				//cout<<"(1<<i): "<<((*temp)&(1<<i))<<endl;
				if(((*temp)&(1<<i))!=0){
					s+='1';
				}else{
					s+='0';
				}
				//cout<<"s:"<<s<<endl;
				map<string,char>::iterator it=res_table.find(s);
				if(it!=res_table.end()){
					s="";
					out<<it->second;
				}
			}
		}else{
			for(int i=7;i>=0;i--){
				//cout<<"(1<<i): "<<(((*temp)&(1<<i))==0)<<endl;
				if(((*temp)&(1<<i))!=0){//the parenthesis is important to correct judgement
					s+='1';
				}else{
					s+='0';
				}
				//cout<<"s:"<<s<<endl;
				map<string,char>::iterator it=res_table.find(s);
				if(it!=res_table.end()){
					s="";
					out<<it->second;
				}
			}
		}
	}
	cout<<"Done."<<endl;
	delete temp;temp=NULL;
}

int main(int argc,char* argv[]){
	if(argc==1||argc==2){
		printf("Arguements match error!\n");
		return 1;
	}
	string filename=argv[1],outname=argv[2];
	outname+=".txt";
	map<string,char> res_table;
	cout<<"Please input the code correspond table in the following format:"<<endl;
	cout<<"<C 10011>"<<endl;
	cout<<"(Please end with Ctrl+Z)"<<endl;
	char c;
	while(cin.get(c)){
		string code;
		cin>>code;
		//cout<<c<<"."<<endl;
		res_table.insert(make_pair(code,c));
		getchar();
	}
	decode(filename,res_table,outname);
}