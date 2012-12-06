#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;
const string InvalidArguments = "Incorrect arguments.";
const string InvalidFile = "Error:can`t open file. Maybe it doesn`t exist.";
void Help()
{
	setlocale(LC_ALL, "Russian");
	cout<<"$ crypt <alphabet> <type> <key> <source> <output>\n\nalphabet - �������\n\ntype encode/decode (����������� - encode)\n\nkey - ����\n\nsource - ���� � �������\n\noutput - ���� ��� ������(���� �� ������ - output.txt)";
}

void Error(string message)
{
	cout<<message<<"\n";
}

int main(int argc, char* argv[])
{
	int key, keyIndex, index, code;
	char symbol;
	string alphabet="AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789";
	string type = "encode"; // ��� �������� - �� ��������� ����������
	FILE *source; 
	FILE *dest;

	if(argc == 2)
		//�������� ����� �������
		if((!(strcmp(argv[1],"-h"))) or (!(strcmp(argv[1],"--help"))))
		{
			Help();
			return 0;
		}
		else
		{
			Error(InvalidArguments);
			return -1;
		}
	if(argc > 2)
	{
		//������ ���������
		for(int i=1; i<argc; i++)
		{
			if(strstr(argv[i], "-a"))
			{
				if(strstr(argv[i],"-alphabet="))
				{	
					//��������� � ������� ��� ��� ����� ���������
					alphabet = argv[i];
					alphabet.erase(0,10);
					alphabet.erase(alphabet.length());
				}
				else
				{
					//������ "-a" - ��������� ��������� ��������
					i++;
					alphabet = argv[i];
				}
			}
			else if(strstr(argv[i],"-t"))
			{
				// ��� ��������
				if(strstr(argv[i],"--type="))
				{	
					type = string(argv[i]).erase(0,7);
				}
				else
				{
					i++; 
					type = string(argv[i]);
				}
				if(type != "encode" && type != "decode")
				{
					Error(InvalidArguments);
					return -1;
				}
			}
			else if(key = atoi(argv[i]) && argc !=i-1)
			{
				if (key < 1)
				{
					Error(InvalidArguments);
					return -1;				  
				}
				//���������� ������ ����� � ������� ����������, ����� ��������� ������������ ��������� ����������
				keyIndex = i; 
				break;
			}
			else
			{
				Error(InvalidArguments);
				return -1;
			}
		}
	}	
	if (keyIndex == (argc-2))//���� �� ������ ��� 1 �������� - ��� �������� �����
	{
		if (!(source = fopen(argv[keyIndex+1],"r")))
		{
			Error(InvalidFile);
			return -1;
		}
		// ����-���������� �� ������, ������ ����� ������ � output.txt
		dest = fopen("output.txt","w");
	}
	else if (keyIndex == (argc-3))//���� ��� 2 ��������� ����� ����� - ����� ������
	{
		//������� ��� ����� (�������� � ����������)
		if (!(source = fopen(argv[keyIndex+1],"r")))
		{
			Error(InvalidFile);
			return -1;
		}
		if (!(dest = fopen(argv[keyIndex+2],"w")))
		{
			Error(InvalidFile);
			return -1;
		}
	}
	else
	{
		Error(InvalidArguments);
		return -1;
	}
	// ���� �����������, �� ������ ���� �� ���������������
	if(type=="decode") 
	{
		key *= -1;
	}
	while((symbol=fgetc(source)) != EOF) //����������� ������ ������� ����, ���� �� ��������� �����
	{
			if((index = alphabet.find(symbol)) +1)//�c�� �� ������  � ��������
			{
				code = symbol; //�������� ��� �������� �� ������� ascii
				if (code==32) 
				{
					//������� ��������� ��� ���������
					fputc(symbol,dest);
				}
				else
				{
					if((code >=48)and(code <=57))// ���� ������������ �����
					{
						code=code-48;
						symbol=(char)((code+key) % 10 +48);
						fputc(symbol,dest);
					}
					else if((code >=65)and(code <=90))// ���� ��������� �����
					{
						code=code-65;
						symbol=(char)((code+key) % 26 +65);
						fputc(symbol,dest);
					}
					else if((code >=97)and(code <=122))// �������� �����
					{
						code=code-97;
						symbol=(char)((code+key) % 26 +97);
						fputc(symbol,dest);
					}
				}
			}
			else
			{
				fputc(symbol,dest);
			}
	}
	//��������� �����
	fclose(source);
	fclose(dest);
	return 0;
}