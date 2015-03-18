#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

int mem[4096];

int main()
{
	ifstream fin("data.txt");
	string str;
	int count=0;
	int num; 
	while (fin>>str)
	{
		if (str=="page")
		{
			fin>>str;
			//cout<<str<<endl;
			continue;
		}
		sscanf(str.c_str(), "%x", &num);
		mem[count]=num;
		//cout<<hex<<mem[count]<<endl;
		//system ("pause");
		count++;
	}
	fin.close();
	ofstream fout("res.txt");
	while(1)
	{
		cin>>hex>>num;
		cout<<hex<<"Virtual Address "<<num<<":"<<endl;
		fout<<hex<<"Virtual Address "<<num<<":"<<endl;
		int offset=num%32;
		//cout<<hex<<offset<<endl;
		num=num>>5;
		int pte=num%32;
		//cout<<pte<<endl;
		num=num>>5;
		int pde=num%32;
		//cout<<pde<<endl;
		
		int pder=0x220;
		int pdecont=mem[pder+pde];
		//cout<<pdecont<<endl;
		int valid1=pdecont>>7;
		if (!valid1)
		{
			int pfn1=pdecont&0x7f;
			cout<<" --> pde index:0x"<<pde<<"  pde contents:(valid 0, pfn 0x"<<pfn1<<")"<<endl;
			cout<<"  Fault (page directory entry not valid)"<<endl;
			fout<<" --> pde index:0x"<<pde<<"  pde contents:(valid 0, pfn 0x"<<pfn1<<")"<<endl;
			fout<<"  Fault (page directory entry not valid)"<<endl;
		}
		else
		{
			int pfn1=pdecont&0x7f;
			cout<<" --> pde index:0x"<<pde<<"  pde contents:(valid 1, pfn 0x"<<pfn1<<")"<<endl;
			fout<<" --> pde index:0x"<<pde<<"  pde contents:(valid 1, pfn 0x"<<pfn1<<")"<<endl;
			int ptecont=mem[(pfn1*32)+pte];
			//cout<< ((pfn1<<8)+pte)<<endl;
			//cout<<ptecont<<endl;
			int valid2=ptecont>>7;
			if (!valid2)
			{
				int pfn2=ptecont&0x7f;
				cout<<"  --> pte index:0x"<<pte<<"  pte contents:(valid 0, pfn 0x"<<pfn2<<")"<<endl;
				cout<<"   Fault (page directory entry not valid)"<<endl;
				fout<<"  --> pte index:0x"<<pte<<"  pte contents:(valid 0, pfn 0x"<<pfn2<<")"<<endl;
				fout<<"   Fault (page directory entry not valid)"<<endl;
			}
			else
			{
				int pfn2=ptecont&0x7f;
				cout<<"  --> pte index:0x"<<pte<<"  pte contents:(valid 1, pfn 0x"<<pfn2<<")"<<endl;
				fout<<"  --> pte index:0x"<<pte<<"  pte contents:(valid 1, pfn 0x"<<pfn2<<")"<<endl;
				
				int cont=mem[(pfn2*32)+offset];
				int pa=(pfn2<<8)+offset;
				cout<<"   --> Translates to Physical Address 0x"<<pa<<" --> Value:"<<cont<<endl;
				fout<<"   --> Translates to Physical Address 0x"<<pa<<" --> Value:"<<cont<<endl;
			}
		}
	}
	fout.close();
	
	
	return 0;
}
