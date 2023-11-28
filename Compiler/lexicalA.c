#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

char ch, chl;
bool lch;
string token;
string symbol;
int num;

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

void catToken() // 将ch于token字符数组拼接
{
    token += ch;
}

int transNum(string token) // 将token中字符串转为整数值，并返回这个值
{
    return stoi(token);
}

void getsym()
{
    token = "";
    symbol = "";

    // 读入字符，通过lch判断上一次有没有多读的字符
    if (!lch)
        ch = getchar();
    else
    {
        ch = chl;
        lch = 0;
    }

    while (isSpace() || isNewline() || isTab())
        ch = getchar();
    if (isLetter())
    {
        while (isLetter() || isDigit())
        {
            token += ch;
            ch = getchar(); // 会多读一个字符
        }

        // 处理多读的字符
        chl = ch;
        lch = 1;

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
            ch = getchar(); // 多读一个字符
        }

        // 处理多读的字符
        chl = ch;
        lch = 1;
        num = stoi(token);
        symbol = "INTCON";
    }
    else if (isColon())
    {
        token += ch;
        symbol = "COLONSY";
    }
    else if (isPlus())
    {
        token += ch;
        symbol = "PLUSSY";
    }
    else if (isMinus())
    {
        token += ch;
        symbol = "MINUSSY";
    }
    else if (isStar())
    {
        token += ch;
        symbol = "STARSY";
    }
    else if (isComma())
    {
        token += ch;
        symbol = "COMMSY";
    }
    else if (isSemi())
    {
        token += ch;
        symbol = "SEMISY";
    }
    else if (ch == '=')
    {
        char ch_ = ch;
        token += ch;
        ch = getchar();
        if (ch == '=')
        {
            token += ch;
            symbol = mp[token];
        }
        else
            symbol = mp[token];
    }
    else if (ch == '&')
    {
        token += ch;
        symbol = mp["&&"];
    }
    else if (ch == '|')
    {
        token += ch;
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
        ch = getchar();
        if (isStar()) // 判断注释
        {
            while (ch != '\n')
                ch = getchar();
        }
        else if (isDivi())
        {
            while (ch != '\n')
                ch = getchar();
        }
        else
        {
            token += ch_;
            symbol = "DIVISY";
        }
    }
    else if (isDouble())
    {
        do
        {
            token += ch;
            ch = getchar();
        } while (!isDouble());
        token += ch;
        symbol = "STRCON";
    }
}

int main()
{
    while (1)
    {
        getsym();
        if (symbol == "INTCON")
            cout << symbol << " " << num << endl;
        else if (ch != '\n') // 处理到注释时，ch最终读到的是\n，此时symbol和token都是空，所以要跳过
            cout << symbol << " " << token << endl;
    }
    return 0;
}