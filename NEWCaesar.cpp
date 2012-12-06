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
	cout<<"$ crypt <alphabet> <type> <key> <source> <output>\n\nalphabet - алфавит\n\ntype encode/decode (поумолчанию - encode)\n\nkey - ключ\n\nsource - файл с текстом\n\noutput - файл для вывода(если не указан - output.txt)";
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
	string type = "encode"; // тип операции - по умолчанию шифрование
	FILE *source; 
	FILE *dest;

	if(argc == 2)
		//вероятно вызов справки
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
		//парсим аргументы
		for(int i=1; i<argc; i++)
		{
			if(strstr(argv[i], "-a"))
			{
				if(strstr(argv[i],"-alphabet="))
				{	
					//считываем в алфавит все что между кавычками
					alphabet = argv[i];
					alphabet.erase(0,10);
					alphabet.erase(alphabet.length());
				}
				else
				{
					//значит "-a" - считываем следующий параметр
					i++;
					alphabet = argv[i];
				}
			}
			else if(strstr(argv[i],"-t"))
			{
				// тип операции
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
				//Запоминаем индекс ключа в массиве аргументов, чтобы проверить правильность следующих параметров
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
	if (keyIndex == (argc-2))//Если за ключом ещё 1 параметр - имя входного файла
	{
		if (!(source = fopen(argv[keyIndex+1],"r")))
		{
			Error(InvalidFile);
			return -1;
		}
		// файл-назначение не указан, значит будем писать в output.txt
		dest = fopen("output.txt","w");
	}
	else if (keyIndex == (argc-3))//Если ещё 2 аргумента после ключа - имена файлов
	{
		//указаны оба файла (источник и назначение)
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
	// если расшифровка, то меняем ключ на противоположный
	if(type=="decode") 
	{
		key *= -1;
	}
	while((symbol=fgetc(source)) != EOF) //Посимвольно читаем входной файл, пока не достигнем конца
	{
			if((index = alphabet.find(symbol)) +1)//еcть ли символ  в алфавите
			{
				code = symbol; //получаем код симввола из таблицы ascii
				if (code==32) 
				{
					//пробелы переносим без изменения
					fputc(symbol,dest);
				}
				else
				{
					if((code >=48)and(code <=57))// если встречаються цифры
					{
						code=code-48;
						symbol=(char)((code+key) % 10 +48);
						fputc(symbol,dest);
					}
					else if((code >=65)and(code <=90))// если заглавные буквы
					{
						code=code-65;
						symbol=(char)((code+key) % 26 +65);
						fputc(symbol,dest);
					}
					else if((code >=97)and(code <=122))// строчные буквы
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
	//закрываем файлы
	fclose(source);
	fclose(dest);
	return 0;
}