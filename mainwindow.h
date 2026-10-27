#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <math.h>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString operand;
    QString opcode;
    QStack<QString> operands;
    QStack<QString> opcodes;
    QMap<int , QPushButton *> digBtns;
    QMap<Qt::Key, QPushButton*> doubleOpBtns;
    QMap<Qt::Key, QPushButton*> funcBTNs;




    QString calcTwoOp(QString n1, QString op, QString n2);
    void doCalculate();
    QString calcSingleOp(QString op, QString numStr);

private slots:
    void on_BtnDot_clicked();
    void on_BtnBack_clicked();
    void on_BtnClear_clicked();

    void on_BtnNum_clicked();
    void on_BtnOp_clicked();
    void on_BtnSingleOp_clicked();

    void on_BtnEq_clicked();
    void on_BtnCe_clicked();

    virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
