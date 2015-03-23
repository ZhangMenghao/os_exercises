#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	unsigned int va,pa;
	char str[50];
	ofstream fout("out.txt");
	while (1)
	{
		cin>>hex>>str>>va>>str>>str>>pa;
		unsigned int pde_idx=va>>22;
		unsigned int pde_ctx=((pde_idx-0x300+1)<<12)|0x3;
		unsigned int pte_idx=(va&0x003FF000)>>12;
		unsigned int pte_ctx=(pa>>12)|0x3;
		fout<<hex<<"va 0x"<<va<<",pa 0x"<<pa<<",pde_idx 0x"<<pde_idx<<",pde_ctx 0x"<<pde_ctx<<",pte_idx 0x"
		<<pte_idx<<",pte_ctx 0x"<<pte_ctx<<endl;
	}
	fout.close();
	return 0;
}
