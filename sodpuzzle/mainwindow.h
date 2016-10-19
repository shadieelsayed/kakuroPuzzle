#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <mylineedit.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT




public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    enum {row=3};
    enum {col=3};
    enum {subrow=3};
    enum {subcol=3};
    enum {x=9};
    enum {y=9};

    QFont* font;
    QFont* font2;
    int currentX;
    int currentY;

    QPushButton* easyButton;
    QPushButton* hardButton;
    QPushButton* returnButton;
    QPushButton* possibleButton;
    QPushButton* saveButton;
    QPushButton* loadButton;
    QPushButton* a;
    QPushButton* replayButton;
    QLabel* duplicateError;
    QStringList* stepValue;
    QList<int>* stepLocation;


    MyLineEdit* slot[x][y];
    QFrame* mainframe;
    QFrame* subFrame[row][col];
    QGridLayout* mainLayout;
    QGridLayout* subLayout[row][col];
    QFrame* possibleBox;

    QVector<QAction*> vector;
    QAction* action;

    QVector<MyLineEdit*> undoList;

    QLabel* DeveloperDetail1;
    QLabel* DeveloperDetail2;
    QLabel* DeveloperDetail3;

public slots:
    void gameBegin();
    void handleButton();
    void possibleValue();
    void currentSlot();
    void drawPossibleBox(QStringList valueArray);
    void enterPossibleValue();
    void undo();
    void highlightSlot();
    void clear();
    void save();
    void open();
    void replay();
    void inputDefaultFile();
    void forward();
    void checkDuplicate();
};

#endif // MAINWINDOW_H
