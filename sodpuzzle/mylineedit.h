#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QApplication>
#include <QLineEdit>
#include <QString>
#include <QWidget>


class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(int x, int y, QWidget *parent = 0);

public:
    int getX(){
        return this->x;
    }
    int getY(){
        return this->y;
    }
    void setX(int x){
        this->x= x;
    }
    void setY(int y){
        this->y= y;
    }

private:
    int x;
    int y;
};

#endif // MYLINEEDIT_H
