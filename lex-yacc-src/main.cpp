#include "main.h"
#include <fstream>
#include <sstream>
#include <cctype>

extern Type* ParseTreeHead;
extern FILE* yyin;
extern vector<string> syntaxErrorInformation;

extern "C"{
    int yyparse();
}

string process(string str);
void dfs(Type* now);
bool outputSyntaxErrorInformation();

int main()
{

#ifdef _WIN32
	string inName="PascalProgram.pas";
	string outName="preProcessed.pas";
#elif __APPLE__
	string inName="/Users/mac/yacc_and_lex_repository/lex_and_yacc/PascalProgram.pas";
	string outName="/Users/mac/yacc_and_lex_repository/lex_and_yacc/preProcessed.pas";
#endif

	ifstream fin(inName.c_str());
	ofstream fout(outName.c_str());
	string str;
	while (getline(fin,str))
		fout << endl << process(str);
	fin.close();
	fout.close();

	yydebug=1;

#ifdef _WIN32
	const char* sFile = "preProcessed.pas";
#elif __APPLE__
	const char *sFile = "/Users/mac/yacc_and_lex_repository/lex_and_yacc/preProcessed.pas";
#endif
	FILE* fp = fopen(sFile,"r");
	if(fp==NULL){
		printf("cannot open %s\n",sFile);
		return -1;
	}
	yyin=fp;

	printf("-----begin parsing %s\n",sFile);
	yyparse();
	printf("-----end parsing\n");
    if(ParseTreeHead!=NULL)
        //dfs(ParseTreeHead);

	fclose(fp);

	outputSyntaxErrorInformation();

	return 0;
}


// ����һ�У�ȥע�͡�תСд
std::string process(const std::string& src)
{
    // static ״̬���Ƿ��ڿ��п�ע����Լ���ע������
    static bool inBlock = false;   // ��ǰ���ڿ�ע���ڣ�
    static int  mode    = 0;       // 1 = {...}, 2 = (*...*)

    std::string dst;
    std::size_t i = 0, n = src.size();

    while (i < n)
    {
        if (inBlock)                     // ���ڿ�ע����
        {
            if (mode == 1 && src[i] == '}')
            { inBlock = false; ++i; }
            else if (mode == 2 && src[i] == '*' && i + 1 < n && src[i + 1] == ')')
            { inBlock = false; i += 2; }
            else
            { ++i; }   // ������ע������
        }
        else                                // ��ͨ������
        {
            // 1) ��ע�Ϳ�ʼ
            if (src[i] == '{')                               { inBlock = true; mode = 1; ++i; }
            else if (src[i] == '(' && i + 1 < n &&
                     src[i + 1] == '*')                      { inBlock = true; mode = 2; i += 2; }

                // 2) ����ע��  //
            else if (src[i] == '/' && i + 1 < n &&
                     src[i + 1] == '/')                      { break; }   // ֱ�Ӷ���ʣ�ಿ��

                // 3) �����ַ���д��Сд��ʽ
            else
            {
                dst.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(src[i]))));
                ++i;
            }
        }
    }

    return dst;   // *��*�� '\n'���ɵ����߲�
}

void dfs(Type* now){
    if(now->children.size()==0){
		if(now->str=="")
			cout << now->token << "\t->\t" << "empty" << endl;
        return;
    }
    cout << now->token << "\t->";
	for (int i = 0; i < now->children.size(); i++) {
		if (now->children[i]->children.size()==0 && now->children[i]->str != "")
			cout << "\t\"" << now->children[i]->str << "\"";
		else
			cout << "\t" << now->children[i]->token;
	}
    cout << endl;
    for(int i=0;i<now->children.size();i++)
        dfs(now->children[i]);
}

string itos(int num){
	stringstream sin;
	sin<<num;
	return sin.str();
}

bool outputSyntaxErrorInformation(){
	if(!syntaxErrorInformation.size())
		return false;
	cout << endl << "Here is the syntax error information" << endl;
	for(int i=0;i<syntaxErrorInformation.size();i++)
		cout << syntaxErrorInformation[i] << endl;
}
