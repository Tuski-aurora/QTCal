#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->BtnNum0,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum1,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum2,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum3,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum4,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum5,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum6,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum7,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum8,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));
    connect(ui->BtnNum9,SIGNAL(clicked()),this,SLOT(on_BtnNum_clicked()));


    //单操作数信号
    connect(ui->BtnSquare,SIGNAL(clicked()),this,SLOT(on_BtnSingleOp_clicked()));
    connect(ui->BtnSqrt,SIGNAL(clicked()),this,SLOT(on_BtnSingleOp_clicked()));
    connect(ui->BtnReciprocal,SIGNAL(clicked()),this,SLOT(on_BtnSingleOp_clicked()));
    connect(ui->BtnPercent,SIGNAL(clicked()),this,SLOT(on_BtnSingleOp_clicked()));
    connect(ui->BtnNeg,SIGNAL(clicked()),this,SLOT(on_BtnSingleOp_clicked()));


    connect(ui->BtnAdd,SIGNAL(clicked()),this,SLOT(on_BtnOp_clicked()));
    connect(ui->BtnDiv,SIGNAL(clicked()),this,SLOT(on_BtnOp_clicked()));
    connect(ui->BtnMul,SIGNAL(clicked()),this,SLOT(on_BtnOp_clicked()));
    connect(ui->BtnSub,SIGNAL(clicked()),this,SLOT(on_BtnOp_clicked()));

MainWindow::~MainWindow()
{
    delete ui;
}

//输入小数点无重复
void MainWindow::on_BtnDot_clicked()
{
    if(!operand.contains(".") && operand != "")
    {
        operand += qobject_cast<QPushButton *>(sender())->text();
        ui->Display->setText(operand);
    }

}

//输入不以0开头的
void MainWindow::on_BtnNum_clicked()
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();
    if(operand == "0")
    {
        operand = "";
    }
    operand += digit;

    ui->Display->setText(operand);

}

//双操作
void MainWindow::on_BtnOp_clicked()
{
    QString newOp;
    QString objName = sender()->objectName();
    if(objName == "BtnAdd")    newOp = "+";
    else if(objName == "BtnSub") newOp = "-";
    else if(objName == "BtnMul") newOp = "*";
    else if(objName == "BtnDiv") newOp = "/";

    // 情况A：当前有正在输入的数字
    if(!operand.isEmpty())
    {
        operands.push(operand);
        // 已有2个操作数，先计算
        if(operands.size() >= 2)
        {
            doCalculate();
        }
    }

    // 【统一处理运算符：弹出旧运算符，压入新运算符】
    if(!opcodes.isEmpty())
    {
        opcodes.pop();
    }
    opcodes.push(newOp);
    opcode = newOp;

    // 清空当前输入区
    operand = "";
    ui->Display->setText("");
}

//后退
void MainWindow::on_BtnBack_clicked()
{
    if(!operand.isEmpty())
     {
            operand = operand.left(operand.length() - 1);
            ui->Display->setText(operand);
     }
}

//清屏
void MainWindow::on_BtnClear_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    ui->Display->setText(operand);

}

//双操作数计算
QString MainWindow::calcTwoOp(QString n1, QString op, QString n2)
{
    double num1 = n1.toDouble();
    double num2 = n2.toDouble();
    double res = 0.0;

    // 全部四则运算，在同一个函数内
    if(op == "+")
        res = num1 + num2;
    else if(op == "-")
        res = num1 - num2;
    else if(op == "*")
        res = num1 * num2;
    else if(op == "/")
    {
        // 除数为0判断
        if(fabs(num2) < 1e-9)
            return "除数不能为0";
        res = num1 / num2;
    }
    return QString::number(res);
}

//栈处理函数，调用 calcTwoOp
void MainWindow::doCalculate()
{
    if(operands.size() >= 2 && !opcodes.isEmpty())
    {
        QString num2 = operands.pop();
        QString op   = opcodes.pop();
        QString num1 = operands.pop();

        QString result = calcTwoOp(num1, op, num2);
        if(result == "除数不能为0")
        {
            ui->Display->setText("除数不能为0");
        }
        else
        {
            operands.push(result);
            operand = result;
            ui->Display->setText(result);
        }
    }
}


//等于
void MainWindow::on_BtnEq_clicked()
{
    if(!operand.isEmpty())
        {
            operands.push(operand);
        }
        doCalculate();
        opcodes.clear();
}

void MainWindow::on_BtnSingleOp_clicked()
{
    if(operand.isEmpty() && !operands.isEmpty())
    {
        operand = operands.top();
    }
    QString opName = sender()->objectName();
    QString opTag;
    if(opName == "BtnSquare") opTag = "sq";
    else if(opName == "BtnSqrt") opTag = "sqrt";
    else if(opName == "BtnReciprocal") opTag = "rec";
    else if(opName == "BtnPercent") opTag = "percent";
    else if(opName == "BtnNeg") opTag = "neg";

    //调用独立单目计算函数
    QString result = calcSingleOp(opTag, operand);
    if(result == "负数不能开根号" || result == "0没有倒数")
    {
        ui->Display->setText(result);
    }
    else
    {
        operand = result;
        if(!operands.isEmpty())
        {
            operands.pop();
            operands.push(operand);
        }
        ui->Display->setText(operand);
    }

}

QString MainWindow::calcSingleOp(QString op, QString numStr)
{


    double num = numStr.toDouble();
    if(op == "sq")
    {
        return QString::number(num * num);
    }
    else if(op == "sqrt")
    {
        if(num < 0)
        {
            return  "负数不能开根号";
        }
        return QString::number(sqrt(num));
    }
    else if(op == "rec")
    {
        if(fabs(num) < 1e-9)
        {
            return "0没有倒数";
        }
        return QString::number(1.0 / num);
    }
    else if(op == "percent")
    {
        return QString::number(num / 100.0);
    }
    else if(op == "neg")
    {
        if(fabs(num) < 1e-9)
        {
            // 是0，直接原样返回，不做正负翻转
            return numStr;
        }
        return QString::number(-num);
    }

    return numStr;
}



void MainWindow::on_BtnCe_clicked()
{
    operand.clear();
    ui->Display->setText("0");
}


