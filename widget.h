#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QStringList>
#include <QFont>
#include <iostream>
#include <stack>
#include <sstream>
using namespace std;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    double evaluateExpression(const string& expression);
    bool isOperator(char c);
    double applyOperation(double a, double b, char op);

private slots:
    void value_clicked();
    void op_react();
    void del_react();
    void clear_react();
    void res_react();

private:
    QPushButton *pb[18];
    QLineEdit *le;
};
#endif // WIDGET_H
