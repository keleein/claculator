#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    QStringList str = {"0","1","2","3","4","5","6","7","8",
                       "9",".","+","-","*","/","<-","C","="};
    for(int i = 0; i < str.size(); i++)
        pb[i] = new QPushButton(str[i]);

    le = new QLineEdit("0");
    le->setAlignment(Qt::AlignRight);

    QGridLayout *gbox = new QGridLayout;

    gbox->addWidget(le,0,0,1,5);

    gbox->addWidget(pb[7],1,0,1,1);
    gbox->addWidget(pb[8],1,1,1,1);
    gbox->addWidget(pb[9],1,2,1,1);
    gbox->addWidget(pb[11],1,3,1,1);
    gbox->addWidget(pb[15],1,4,1,1);

    gbox->addWidget(pb[4],2,0,1,1);
    gbox->addWidget(pb[5],2,1,1,1);
    gbox->addWidget(pb[6],2,2,1,1);
    gbox->addWidget(pb[12],2,3,1,1);
    gbox->addWidget(pb[16],2,4,1,1);

    gbox->addWidget(pb[1],3,0,1,1);
    gbox->addWidget(pb[2],3,1,1,1);
    gbox->addWidget(pb[3],3,2,1,1);
    gbox->addWidget(pb[13],3,3,1,1);
    gbox->addWidget(pb[17],3,4,2,1);
    pb[17]->setFixedHeight(105);

    gbox->addWidget(pb[0],4,0,1,2);
    gbox->addWidget(pb[10],4,2,1,1);
    gbox->addWidget(pb[14],4,3,1,1);

    this->setLayout(gbox);

    QFont font("Times", 25, QFont::Bold);
    this->setFont(font);

    for(int i = 0; i < 11; i++)
        connect(pb[i],SIGNAL(clicked(bool)),this,SLOT(value_clicked()));

    for(int i = 11; i < 15; i++)
        connect(pb[i],SIGNAL(clicked(bool)),this,SLOT(op_react()));

    connect(pb[15],SIGNAL(clicked(bool)),this,SLOT(del_react()));
    connect(pb[16],SIGNAL(clicked(bool)),this,SLOT(clear_react()));
    connect(pb[17],SIGNAL(clicked(bool)),this,SLOT(res_react()));
}

Widget::~Widget() {}


void Widget::value_clicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString str = le->text();
    if(str == "0")
    {
        if(btn->text() == '.')
        {
            str += btn->text();
            le->setText(str);
        }else{
            le->setText(btn->text());
        }
    }else{
        if(btn->text() == '.')
        {
            char lastchar = str.at(str.size()-1).toLatin1();
            if(lastchar == '.')
            {
                return;
            }else{
                str += btn->text();
                le->setText(str);
            }
        }else{
            str += btn->text();
            le->setText(str);
        }
    }
}

void Widget::clear_react()
{
    le->setText("0");
}

void Widget::op_react()
{
    QString str = le->text();
    QString lastchar = str[str.size()-1];
    if(lastchar != "+" && lastchar != "-" && lastchar != "*" && lastchar != "/")
    {
        QPushButton *btn = qobject_cast<QPushButton*>(sender());
        str += btn->text();
        le->setText(str);
    }else{
        le->setText("非法输入");
    }
}

void Widget::del_react()
{
    string str = le->text().toStdString();
    str.pop_back();
    le->setText(QString::fromStdString(str));
}

void Widget::res_react()
{
    QString str = le->text();
    const string expr = str.toStdString();
    double result = evaluateExpression(expr);
    le->setText(QString::number(result));
}

//判断是否为操作运算符
bool Widget::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

//根据操作符进行运算
double Widget::applyOperation(double a, double b, char op) {
    switch(op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if(b == 0) {
            le->setText("除数不能为0");
            exit(EXIT_FAILURE);
        }
        return a / b;
    default:
        cerr << "Error: Invalid operator!" << endl;
        exit(EXIT_FAILURE);
    }
}

//表达式计算
double Widget::evaluateExpression(const string& expression) {
    //定义操作数栈和操作符栈
    stack<double> operands;
    stack<char> operators;

    //定义字符串流对象
    istringstream iss(expression);
    char token;

    //判断字符情况
    while(iss >> token) {
        if(token == '(') {
            operators.push(token);
        } else if(isdigit(token)) {     //isdigit()判断是否为数字字符
            //将当前字符放回字符操作流
            iss.putback(token);
            double operand;
            //读取操作数直到遇到非数值字符或流末尾
            iss >> operand;
            operands.push(operand);
        } else if(isOperator(token)) {
            while(!operators.empty() && operators.top() != '(' && ((token != '*' && token != '/') || (operators.top() == '*' || operators.top() == '/'))) {
                char op = operators.top();
                operators.pop();
                double b = operands.top();
                operands.pop();
                double a = operands.top();
                operands.pop();
                operands.push(applyOperation(a, b, op));
            }
            operators.push(token);
        } else if(token == ')') {
            while(!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                double b = operands.top();
                operands.pop();
                double a = operands.top();
                operands.pop();
                operands.push(applyOperation(a, b, op));
            }
            if(operators.empty()) {
                cerr << "Error: Mismatched parentheses!" << endl;
                exit(EXIT_FAILURE);
            }
            operators.pop(); // Pop '('
        }
    }

    while(!operators.empty()) {
        char op = operators.top();
        operators.pop();
        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();
        operands.push(applyOperation(a, b, op));
    }

    if(operands.size() != 1) {
        cerr << "Error: Invalid expression!" << endl;
        exit(EXIT_FAILURE);
    }

    return operands.top();
}

