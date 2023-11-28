#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include "head.h"
using namespace std;

struct Node
{
    string Tok;
    string sym;
    int tnum;
    struct Node *next;
    struct Node *pre;
};
struct Node *head = NULL;
struct Node *nowtok;      // 用于语法分析
char ch, chl, chne = '@'; // ch是现在要分析的字符，chl是多读到的字符，nech是下一个待分析的字符
bool lch;
string fileContent;
string token;
string symbol;
int num, idx;

unordered_map<string, string> mp = {
    {"main", "MAINTK"},
    {"const", "CONSTTK"},
    {"int", "INTTK"},
    {"break", "BREAKTK"},
    {"continue", "CONTINUETK"},
    {"if", "IFTK"},
    {"else", "ELSETK"},
    {"!", "NOT"},
    {"&&", "ADN"},
    {"||", "OR"},
    {"while", "WHILETK"},
    {"getint", "GETINTTK"},
    {"printf", "PRINTFTK"},
    {"return", "RETURNTK"},
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"void", "VOIDTK"},
    {"*", "MULT"},
    {"/", "DIV"},
    {"%", "MOD"},
    {"<", "LSS"},
    {"<=", "LEQ"},
    {">", "GRE"},
    {">=", "GEQ"},
    {"==", "EQL"},
    {"!=", "NEQ"},
    {"=", "ASSIGN"},
    {";", "SEMICN"},
    {",", "COMMA"},
    {"(", "LPARENT"},
    {")", "RPARENT"},
    {"[", "LBRACK"},
    {"]", "RBRACK"},
    {"{", "LBRACE"},
    {"}", "RBRACE"},
};

void go()
{
    nowtok = nowtok->next;
    if (nowtok->sym == "INTCON")
        cout << nowtok->sym << " " << nowtok->tnum << endl;
    else
        cout << nowtok->sym << " " << nowtok->Tok << endl;
}

void appendNode(struct Node **head)
{
    // 创建新节点
    // struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    struct Node *newNode = new struct Node;
    if (newNode == NULL)
        return;
    newNode->Tok = token;
    newNode->sym = symbol;
    newNode->tnum = num;
    newNode->next = NULL;
    newNode->pre = NULL;

    // 如果链表为空，新节点即为头节点
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        // 否则，遍历链表找到末尾，将新节点连接到末尾
        struct Node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
        newNode->pre = current;
    }
}

void freeList(struct Node *head)
{
    struct Node *current = head;
    struct Node *next;

    while (current != NULL)
    {
        next = current->next;
        delete (current);
        current = next;
    }
}

void CompUnit()
{
    nowtok = head;
    if (nowtok->sym == "INTCON")
        cout << nowtok->sym << " " << nowtok->tnum << endl;
    else
        cout << nowtok->sym << " " << nowtok->Tok << endl;

    while (1)
    {
        if (nowtok->sym == "CONSTTK")
        {
            Decl();
        }
        else if (nowtok->sym == "VOIDTK") // 变量定义
        {
            FuncDef();
        }
        else // INTTK
        {
            go();
            if (nowtok->sym == "MAINTK") // 主函数
            {
                MainFuncDef();
                break;
            }
            else if (nowtok->sym == "IDENFR") // 变量定义
            {
                Decl();
            }
        }
    }
    cout << "<CompUnit>" << endl;
}

void Decl()
{
    if (nowtok->sym == "CONSTTK")
    {
        ConstDecl();
    }
    else
    {
        VarDecl();
    }
    // cout << "<Decl>" << endl;
}

void MainFuncDef()
{
    go(); // 取(
    go(); // 取）
    Block();
    cout << "<MainFuncDef>" << endl;
}

void ConstDecl()
{
    BType();
    ConstDef();
    // 判断是否定义多变量
    while (nowtok->next->sym == "COMMA")
    {
        ConstDef();
        go(); // 最后一次读到分号;
    }
    cout << "<ConstDecl>" << endl;
}

void VarDecl()
{
    BType();
    VarDef();
    while (nowtok->next->sym == "COMMA")
    {
        VarDef();
    }
    go(); // 取;
    cout << "<VarDecl>" << endl;
}

void BType()
{
    if (nowtok->sym != "INTTK")
        go(); // 获取int
    // cout << "<BType>" << endl;
}

void ConstDef()
{
    go(); // 获取Ident
    go(); // 获取等号，假设不是数组
    ConstInitVal();
    cout << "<ConstDef>" << endl;
}
void Ident() {}
void ConstExp()
{
    AddExp(); // CONST
    cout << "<ConstExp>" << endl;
}
void ConstInitVal()
{
    ConstExp();
    cout << "<ConstInitVal>" << endl;
}
void VarDef()
{
    go(); // 取变量名
    if (nowtok->next->sym == "ASSIGN")
        InitVal();
    cout << "<VarDef>" << endl;
}
void InitVal()
{
    Exp();
    cout << "<InitVal>" << endl;
}
void Exp()
{
    AddExp();
    cout << "<Exp>" << endl;
}
void FuncDef()
{
    // FuncDef → FuncType Ident '(' [FuncFParams] ')' Block
    FuncType();
    go(); // 获取函数名Ident
    go(); // 获取左括号(

    if (nowtok->next->sym != "RPARENT") // 有参数
    {
        FuncFParams();
    }
    else
    {
        go(); // 取)
        Block();
    }
    cout << "<FuncDef>" << endl;
}

void FuncType()
{
    // FuncType → 'void' | 'int'
    go(); // 取函数类型
    cout << "<FuncType>" << endl;
}

void FuncFParams()
{
    FuncFParam();
    cout << "<FuncFParams>" << endl;
}

void FuncFParam()
{
    BType();
    cout << "<FuncFParam>" << endl;
}
void Block()
{
    go(); // 取左花括号{
    do
    {
        BlockItem();
    } while (nowtok->next->sym != "RBRACE");
    go(); // 取右花括号}
    cout << "<Block>" << endl;
}
void BlockItem()
{
    if (nowtok->next->sym == "CONSTTK" || nowtok->next->sym == "INTTK")
    {
        if (nowtok->next->sym == "CONSTTK")
            ConstDecl();
        else
            VarDecl();
    }
    else
    {
        Stmt();
    }
    // cout << "<BlockItem>" << endl;
}
void Stmt()
{
    if (nowtok->next->sym == "IDENFR")
    {
        // LVal '=' Exp ';'
        LVal();
        go(); // 取=
        if (nowtok->next->sym == "GETINTTK")
        {
            go(); // 取 getint
            go(); // 取(
            go(); // 取）
            go(); // 取;
        }
        else
        {
            Exp();
            go(); // 取;
        }
    }
    else if (nowtok->next->sym == "IFTK")
    {
        go(); // 取 if
        go(); // 取(
        Cond();
        go();                              // 取）
        if (nowtok->next->sym == "ELSETK") // 判断有没有else
        {
            go(); // 取 else
            Stmt();
        }
        else
            Stmt();
    }
    else if (nowtok->next->sym == "WHILETK")
    {
        go(); // 取 while
        go(); // 取(
        Cond();
        go(); // 取）
        Stmt();
    }
    else if (nowtok->next->sym == "BREAKTK")
    {
        go(); // 取 break
        go(); // 取;
    }
    else if (nowtok->next->sym == "CONTINUETK")
    {
        go(); // 取 continue
        go(); // 取;
    }
    else if (nowtok->next->sym == "RETURNTK")
    {
        //'return' [Exp] ';'
        go(); // 取 return
        if (nowtok->next->sym != "SEMICN")
            Exp();
        go(); // 取;
    }
    else if (nowtok->next->sym == "PRINTFTK")
    {
        go();                                // 取 printf
        go();                                // 取(
        go();                                // 取一个字符串
        while (nowtok->next->sym == "COMMA") // 有参数
        {
            go(); // 取 ,
            Exp();
        }
        go(); // 取 )
        go(); // 取 ;
    }
    else if (nowtok->next->sym == "LBRACE") // BLOCK情况
    {
        Block();
    }
    else //[Exp] ';'
    {
        if (nowtok->next->sym != "SEMICN")
            Exp();
        go(); // 取;
    }
    cout << "<Stmt>" << endl;
}
void LVal()
{
    if (nowtok->sym != "IDENFR") // 看是否已经取到了变量名
        go();
    cout << "<LVal>" << endl;
}
void Cond()
{
}
void FormatString() {}
void AddExp()
{
    MulExp();
    cout << "<AddExp>" << endl;
}
void LOrExp() {}
void PrimaryExp()
{
    if (nowtok->next->sym == "IDENFR")
        LVal();
    else
        Number();
    cout << "<PrimaryExp>" << endl;
}
void Number()
{
    IntConst();
    cout << "<Number>" << endl;
}
void IntConst()
{
    go(); // 获取一个整数
    cout << "<IntConst>" << endl;
}
void UnaryExp()
{
    PrimaryExp();
    cout << "<UnaryExp>" << endl;
}

void UnaryOp() {}
void FuncRParams() {}
void MulExp()
{
    UnaryExp();
    cout << "<MulExp>" << endl;
}
void RelExp() {}
void EqExp() {}
void LAndExp() {}
void identifier() {}
void identifier_nondigit() {}
void digit() {}
void integer_const() {}
void decimal_const() {}
void nonzero_digit() {}

bool isSpace() // 判断是否为空格
{
    return ch == ' ';
}

bool isNewline() // 判断是否为换行符
{
    return ch == '\n';
}

bool isTab() // 判断是否为Tab
{
    return ch == '\t';
}

bool isLetter() // 判断是否为字母
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    else
        return false;
}

bool isDigit() // 判断是否为数字
{
    if (ch >= '1' && ch <= '9')
        return true;
    else
        return false;
}

bool isColon() // 判断是否为冒号
{
    return ch == ':';
}

bool isComma() // 判断是否为逗号
{
    return ch == ',';
}

bool isSemi() // 判断是否为分号
{
    return ch == ';';
}

bool isEqu() // 判断是否为等号
{
    return ch == '=';
}

bool isPlus() // 判断是否为加号
{
    return ch == '+';
}

bool isMinus() // 判断是否为减号
{
    return ch == '-';
}

bool isDivi() // 判断是否为除号
{
    return ch == '/';
}

bool isStar() // 判断是否为星号（乘*）
{
    return ch == '*';
}

bool isDouble()
{
    return ch == '"';
}

void Lexical()
{

    token = "";
    symbol = "";
    num = 0;

    ch = fileContent[idx];
    while (isSpace() || isNewline() || isTab())
        ch = fileContent[++idx];
    if (isLetter())
    {
        while (isLetter() || isDigit())
        {
            token += ch;
            ch = fileContent[++idx]; // 会多读一个字符
        }
        idx--; // 减去多读的一个
        if (mp.count(token))
            symbol = mp[token];
        else
            symbol = "IDENFR";
    }
    else if (isDigit())
    {
        num = 0;
        while (isDigit())
        {
            token += ch;
            ch = fileContent[++idx]; // 多读一个字符
        }
        idx--; // 减去多读的一个
        num = stoi(token);
        symbol = "INTCON";
    }
    else if (isColon())
    {
        token += ch;
        symbol = "COLON";
    }
    else if (isPlus())
    {
        token += ch;
        symbol = "PLUS";
    }
    else if (isMinus())
    {
        token += ch;
        symbol = "MINUS";
    }
    else if (isStar())
    {
        token += ch;
        symbol = "MULT";
    }
    else if (isComma())
    {
        token += ch;
        symbol = "COMMA";
    }
    else if (isSemi())
    {
        token += ch;
        symbol = "SEMICN";
    }
    else if (ch == '>')
    {
        char ch_ = ch;
        token += ch;

        if (ch == '=')
        {
            token += ch;
            symbol = mp[token];
        }
        else
        {
            // 处理多读的字符
            chl = ch;
            lch = 1;
            symbol = mp[token];
        }
    }
    else if (ch == '<')
    {
        char ch_ = ch;
        token += ch;
        ch = fileContent[idx + 1];
        if (ch = fileContent[idx + 1] == '=')
        {
            token += ch;
            symbol = mp[token];
        }
        else
        {
            symbol = mp[token];
        }
    }
    else if (ch == '=')
    {
        char ch_ = ch;
        token += ch;
        ch = fileContent[idx + 1];
        if (ch = fileContent[idx + 1] == '=')
        {
            token += ch;
            symbol = mp[token];
        }
        else
        {
            symbol = mp[token];
        }
    }
    else if (ch == '&')
    {
        token += ch;
        idx++;
        symbol = mp["&&"];
    }
    else if (ch == '|')
    {
        token += ch;
        idx++;
        symbol = mp["||"];
    }
    else if (ch == '(')
    {
        token += ch;
        symbol = mp[token];
    }
    else if (ch == ')')
    {
        token += ch;
        symbol = mp[token];
    }
    else if (ch == '[' || ch == '{')
    {
        token += ch;
        symbol = mp[token];
    }
    else if (ch == ']' || ch == '}')
    {
        token += ch;
        symbol = mp[token];
    }
    else if (isDivi())
    {
        char ch_ = ch;
        ch = fileContent[idx + 1];
        if (isStar()) // 判断注释
        {
            while (ch != '\n')
            {
                ch = fileContent[++idx];
            }
            idx--;
        }
        else if (isDivi())
        {
            while (ch != '\n')
            {
                ch = fileContent[++idx];
            }
            idx--;
        }
        else
        {
            token += ch_;
            symbol = "DIV";
        }
    }
    else if (isDouble())
    {
        do
        {
            token += ch;
            ch = fileContent[++idx];
        } while (!isDouble());
        token += ch;
        symbol = "STRCON";
    }
    // if (symbol == "INTCON")
    //     cout << symbol << " " << num << endl;
    // else if (ch != '\n') //处理到注释时，ch最终读到的是\n，此时symbol和token都是空，所以要跳过
    //     cout << symbol << " " << token << endl;
}

int main()
{
    std::ifstream inputFile("test.txt");

    if (inputFile.is_open())
    {
        string line;

        // 逐行读取文件内容
        while (getline(inputFile, line))
        {
            // 去除每一行的空格和制表符
            // line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            fileContent += line;
        }

        inputFile.close();
    }
    // 词法分析
    for (; idx < fileContent.size(); idx++)
    {
        Lexical();
        appendNode(&head); // 将token添加到链表末尾
    }
    CompUnit();
    freeList(head);
    return 0;
}
