#include "iostream"
#include "fstream"
#include "locale.h"
#include "string.h"
#include "conio.h"
#include <cstdlib>
using namespace std;

void Ru();//��������� �������� �����������
void Help(); //����� �������
string  Encrypt(string text,string alf,int key); //����������
string  Decrypt(string cyphertext,string alf,int key); //������������

int main(int argc, char* argv[])
{
	int key;
	string type="encode",text="",source,output,alf="AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789";
	Ru();
	if(argc==2)
	{
	//�������� ����� �������
		if(!strcmp(argv[1],"--help"))
		{
			Help();
		}
	}
	if(argc<4)
	{
		cout<<"������!������� ���� ����������!\n";
		getch();
		return 1;
	}

	//��������� ���������� ��������� ������
	type=argv[2];
	alf=argv[1];
	key=atoi(argv[3]);
	source=argv[4];
	output=argv[5];
	

	fstream file;
	string line,result;
	file.open(source.c_str());
	while(file>>line)
	{
		//��������� ������ �� �����-���������
		text.append(line.c_str());
	}
	file.close();
	
	//��������� ��������������� �������
	if(!strcmp(type.c_str(),"decode"))
	{
		result=Decrypt(text.c_str(),alf,key);
	}else{
		result=Encrypt(text.c_str(),alf,key);
	}
	//������� ��������� � ����-����������
	file.open(output.c_str(),ios::out | ios::app | ios::binary);
	file<<result.c_str();
	file.close();

	getch();
	return 0;

}

void Ru()
{
setlocale(LC_CTYPE, "");
}

void Help()
{
	cout<<"$ crypt <alphabet> <type> <key> <source> <output>\n\nalphabet - �������\n\ntype encode/decode (����������� - encode)\n\nkey - ����\n\nsource - ���� � �������\n\noutput - ���� ��� ������";
}

string  Encrypt(string text,string alf,int key)
{
	//����������
	int n;
	string cyphertext="";
	for(unsigned int i=0;i<text.size();i++)
	{
		//���������� �����
		n=alf.find(text[i]);
		(n-key<0)?n=alf.size()-(key-n):n-=key;
		cyphertext[i]=alf[n];
	}
	cyphertext[text.size()]=0;
	return cyphertext.c_str();
}

string  Decrypt(string cyphertext,string alf,int key)
{
	//������������
	int n;
	string text="";
	for(unsigned int i=0;i<cyphertext.size();i++)
	{
		//���������� �����
		n=alf.find(cyphertext[i]);
		(n+key>=alf.size())?n=(key+n)-alf.size():n+=key;
		text[i]=alf[n];
	}
	text[cyphertext.size()]=0;
	return text.c_str();
}