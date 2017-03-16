#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <cstddef>
#include <map>
#include <cstdio>
#include <bitset>
using namespace std;

template<typename T>
struct Node{
	T entry;
	Node<T>* left,*right;
	Node<T>(){
		left=NULL;
		right=NULL;
	}
	Node<T>(T e){
		entry=e;
		left=NULL;
		right=NULL;
	}
};

template<typename T>
bool isLeaf(Node<T>* n){
	if(n->left==NULL&&n->right==NULL)
		return 1;
	return 0;
}
enum Side
{
	L,R
};
template<typename T>
class CodeTree{
private:
	Node<T>* _root;
	void setCode(Node<T>* subroot,string curr_code,map<T,string> &table,Side s);
public:
	int weight;
	CodeTree(){
		_root=NULL;
	}
	CodeTree(T e,double rate){
		_root=new Node<T>(e);
		weight=rate;
	}
	map<T,string> getCodeTable(){
		//recursive set
		string curr_code;
		map<T,string> code_table;
		if(_root==NULL){
			return code_table;
		}
		else if(isLeaf(_root)){
			code_table[_root->entry]="0";
			return code_table;
		}
		setCode(_root->right,curr_code,code_table,R);
		setCode(_root->left,curr_code,code_table,L);
		return code_table;
	}
	void combineTree(CodeTree<T> &b){
		//after combine tree b become empty and this become a new tree
		Node<T>* new_root=new Node<T>();
		new_root->left=_root;
		new_root->right=b._root;
		weight=weight+b.weight;
		_root=new_root;
		b._root=NULL;
		b.weight=0;
	}
};
template<typename T>
void CodeTree<T>::setCode(Node<T>* subroot,string curr_code,map<T,string> &table,Side s){
	if(s==L) curr_code+="0";
	else curr_code+="1";

	if(isLeaf(subroot)){
		table[subroot->entry]=curr_code;
		return;
	}else{
		setCode(subroot->left,curr_code,table,L);
		setCode(subroot->right,curr_code,table,R);
	}
}

struct Comp
{	
	bool operator () (CodeTree<char> &a,CodeTree<char> &b){
		return a.weight > b.weight;
	}
};

void handle_file(string name,map<char,size_t> &table,string& content){
	ifstream infile(name);
	infile.seekg(0,ios::end);
	cout<<"file size before compressing: "<<infile.tellg()<<" bytes."<<endl;
	infile.seekg(0,ios::beg);
	while(infile.peek()!=EOF){//before file reach to the end
		char c;
		infile.get(c);
		++table[c];
		content+=c;
	}
	cout<<content<<endl;
}
void print(map<char,string> &table){
	printf("code table:\n");
	for(map<char,string>::iterator it=table.begin();it!=table.end();++it){
		printf("%c %s\n",it->first,it->second.c_str());
	}
}
void print(map<char,size_t> &table){
	for(map<char,size_t>::iterator it=table.begin();it!=table.end();++it){
		printf("%c : %d times\n",it->first,it->second);
	}
}
#include <bitset>
void out_binary(ofstream& out,const string& code,char &count,char* &j){
	//cout<<"code: "<<code<<endl;	
	for(int i=0;i<code.size();i++){
		count++;
		if(code[i]=='1'){
			*j=*j<<1;
			*j|=1;
		}else{
			*j=*j<<1;
		}
		if(count=='8'){
			//cout<<"j: "<<bitset<8>(*j)<<endl;
			out.write(j,1);
			*j=0;
			count='0';
		}
	}
	//out.write(j,1);
	//cout<<"j: "<<bitset<8>(*j)<<endl;
}
void translate_and_out(string& content,map<char,string> &table,string outname){
	ofstream out(outname,ios::binary);
	/*
	for(string::size_type i=0;i<content.size();i++){
		out<<table[content[i]];
		cout<<table[content[i]]<<endl;
	}*/
	char count='0';
	char* j=new char();
	for(string::size_type i=0;i<content.size();i++){
		//cout<<"table:"<<table[content[i]]<<endl;
		out_binary(out,table[content[i]],count,j);
	}
	if(count!=0){
		//cout<<"j: "<<bitset<8>(*j)<<endl;
		//cout<<"count: "<<count<<endl;
		out.write(j,1);
		//count=(char)count;
		//cout<<"count: "<<sizeof(count)<<endl;
		out<<count;
	}else{
		out<<'8';
	}
	cout<<"file size after compressing: "<<out.tellp()<<" bytes."<<endl;
}
int main(int argc,char* argv[]){
	//cout<<argv[0]<<" "<<argv[1]<<endl;
	if(argc==1||argc==2){
		cout<<"Arguements match error!"<<endl;
		return 1;
	}
	string filename,outname,content;
	filename.insert(0,argv[1]);
	outname.insert(0,argv[2]);
	outname+=".txt";
	map<char,size_t> rate_table;
	handle_file(filename,rate_table,content);
	priority_queue<CodeTree<char>,vector<CodeTree<char>>,Comp> tree_queue;
	
	for(map<char,size_t>::iterator it=rate_table.begin();it!=rate_table.end();++it){
		tree_queue.push(CodeTree<char>(it->first,it->second));
	}

	while(tree_queue.size()>1){
		CodeTree<char> min1=tree_queue.top();tree_queue.pop();
		CodeTree<char> min2=tree_queue.top();tree_queue.pop();
		//cout<<min1.weight<<" "<<min2.weight<<endl;
		min1.combineTree(min2);
		tree_queue.push(min1);
	}
	CodeTree<char> ultimate=tree_queue.top();
	map<char,string> code_table=ultimate.getCodeTable();
	//print(rate_table);
	print(code_table);
	translate_and_out(content,code_table,outname);
}