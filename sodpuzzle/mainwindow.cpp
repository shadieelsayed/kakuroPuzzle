#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QLabel>
#include <QList>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Sodoku");
    this->setGeometry(50,30,800,500);

    //validate input
    QRegExp rx("[1-9]\\d{0,0}");
    QValidator *validator = new QRegExpValidator(rx, this);

    //create font
    font= new QFont("Times",30,QFont::Bold);

    //create easy and hard button
    easyButton= new QPushButton("Easy", this);
    hardButton= new QPushButton("Hard", this);

    easyButton->setGeometry(320,150,120,70);
    hardButton->setGeometry(320,250,120,70);

    easyButton->setFont(*font);
    hardButton->setFont(*font);

    //action when click easy or hard button
    connect(this->easyButton,SIGNAL(clicked(bool)),this,SLOT(gameBegin()));
    connect(this->hardButton,SIGNAL(clicked(bool)),this,SLOT(gameBegin()));

    //Developer detail
    DeveloperDetail1 = new QLabel;
    DeveloperDetail1->setParent(this);
    DeveloperDetail1->setText("ANH KHIET VUONG - VUOKY002");
    DeveloperDetail1->setGeometry(320,350,250,70);
    DeveloperDetail1->setVisible(true);

    DeveloperDetail2 = new QLabel;
    DeveloperDetail2->setParent(this);
    DeveloperDetail2->setText("NHAN TRONG NGUYEN - NGUTY147");
    DeveloperDetail2->setGeometry(320,370,2500,70);
    DeveloperDetail2->setVisible(true);

    DeveloperDetail3 = new QLabel;
    DeveloperDetail3->setParent(this);
    DeveloperDetail3->setText("ANH QUOC TRUONG - TRUQA001");
    DeveloperDetail3->setGeometry(320,390,250,70);
    DeveloperDetail3->setVisible(true);

    //create mainframe for mainwindow
    mainframe= new QFrame;
    mainLayout= new QGridLayout;
    mainframe->setParent(this);
    mainframe->setLayout(mainLayout);
    mainframe->setFixedSize(500,500);
    mainframe->setStyleSheet("background-color:gray");
    mainframe->setVisible(false);

    //create list to store value and coordinate
    stepLocation = new QList<int>();
    stepValue = new QStringList();

    //create mainframe for possible button box
    possibleBox = new QFrame;

    //create user button
    possibleButton= new QPushButton("Possible", this);
    returnButton= new QPushButton("Return", this);
    saveButton= new QPushButton("Save", this);
    loadButton= new QPushButton("Open", this);
    replayButton= new QPushButton("Replay", this);
    possibleButton->setGeometry(520,20,120,70);
    returnButton->setGeometry(660,20,120,70);
    saveButton->setGeometry(520,90,120,70);
    loadButton->setGeometry(660,90,120,70);
    replayButton->setGeometry(520,160,120,70);
    possibleButton->setVisible(false);
    returnButton->setVisible(false);
    saveButton->setVisible(false);
    loadButton->setVisible(false);
    replayButton->setVisible(false);
    connect(this->possibleButton,SIGNAL(clicked(bool)),this,SLOT(possibleValue()));
    connect(returnButton,SIGNAL(clicked(bool)),this,SLOT(undo()));
    connect(saveButton,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(loadButton,SIGNAL(clicked(bool)),this,SLOT(open()));
    connect(replayButton,SIGNAL(clicked(bool)),this,SLOT(replay()));

    font2= new QFont("Times",12,QFont::Bold);

    duplicateError = new QLabel;
    duplicateError->setParent(this);
    duplicateError->setStyleSheet("background-color:red");
    duplicateError->setText("The number you enter is duplicate");
    duplicateError->setFont(*font2);
    duplicateError->setGeometry(505,230,280,70);
    duplicateError->setVisible(false);


    //create subframe and sublayout and add to the main
    for (int i=0;i<3;++i){
        for (int j=0;j<3;++j){
            subFrame[i][j]= new QFrame;
            subLayout[i][j]= new QGridLayout;
            subFrame[i][j]->setFixedSize(162,162);
            subLayout[i][j]->setSpacing(0);
            subLayout[i][j]->setContentsMargins(0,0,0,0);

            subFrame[i][j]->setLayout(subLayout[i][j]);
            subFrame[i][j]->setStyleSheet("background-color:black");

            mainLayout->addWidget(subFrame[i][j],i,j);
            mainLayout->setSpacing(0);

        }
    }

    //Create cells for enter values
    for (int row=0;row<3;row++){
        for (int col=0;col<3;col++){
            for (int subrow=0;subrow<3;subrow++){
                for (int subcol=0;subcol<3;subcol++){
                    int x= row*3+subrow;
                    int y = col*3+subcol;
                    slot[x][y]= new MyLineEdit(x,y);
                    slot[x][y]->setStyleSheet("background-color:yellow");
                    slot[x][y]->setFixedSize(49,49);
                    slot[x][y]->setValidator(validator);
                    slot[x][y]->setFrame(0);
                    slot[x][y]->setFont(*font);
                    slot[x][y]->setAlignment(Qt::AlignCenter);
                    slot[x][y]->setContextMenuPolicy(Qt::PreventContextMenu);
                    subLayout[row][col]->addWidget(slot[x][y],subrow,subcol);

                }
            }
        }
    }

    //input the question for the game from text file
    inputDefaultFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gameBegin()
{
    DeveloperDetail1->setVisible(false);
    DeveloperDetail2->setVisible(false);
    DeveloperDetail3->setVisible(false);
    this->easyButton->setVisible(false);
    this->hardButton->setVisible(false);
    possibleButton->setVisible(true);
    returnButton->setVisible(true);
    saveButton->setVisible(true);
    loadButton->setVisible(true);
    replayButton->setVisible(true);
    this->mainframe->setVisible(true);
    currentX= -1;
    currentY= -1;

}

void MainWindow::handleButton()
{
    qDebug()<<"Pressed";
    possibleBox->setVisible(false);
    if (currentX>=0 && currentY>=0){
        slot[currentX][currentY]->setStyleSheet("background-color:yellow");
    }
    MyLineEdit* entry = (MyLineEdit*) QObject::sender();
    qDebug()<<entry->text();
    if(entry->text()!=""){
        qDebug()<<entry->getX();
        stepLocation->push_back(entry->getX());
        qDebug()<<entry->getY();
        stepLocation->push_back(entry->getY());
        stepValue->append(entry->text());
        undoList.append(entry);
    }
    currentX = entry->getX();
    currentY = entry->getY();
    highlightSlot();
    checkDuplicate();
}


void MainWindow::possibleValue()
{
    if (currentX>=0 && currentY>=0){
        QStringList valueArray;
        valueArray.clear();
        valueArray<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9";
        for(int i=0; i<9;i++){
            if(slot[i][currentY]->text().isEmpty()==false){
                valueArray.removeOne(slot[i][currentY]->text());
            }
        }
        for(int i=0; i<9;i++){
            if(slot[currentX][i]->text().isEmpty()==false){
                valueArray.removeOne(slot[currentX][i]->text());
            }
        }

        int a = currentX/3*3;
        int c= a+3;
        int b = currentY/3*3;
        int d=b+3;
        for(int m=a;m<c;m++){
            for(int n=b;n<d;n++){
                if(slot[m][n]->text().isEmpty()==false){
                    valueArray.removeOne(slot[m][n]->text());             }
            }
        }

        drawPossibleBox(valueArray);
    }


}

void MainWindow::currentSlot()
{
    possibleBox->setVisible(false);
    if (currentX>=0 && currentY>=0){
        slot[currentX][currentY]->setStyleSheet("background-color:yellow");
    }
    MyLineEdit* entry = (MyLineEdit*) QObject::sender();
    entry->setStyleSheet("background-color:blue");

    qDebug()<<"X: "<<entry->getX()<<" Y: "<<entry->getY();
    currentX = entry->getX();
    currentY = entry->getY();
}

void MainWindow::drawPossibleBox(QStringList valueArray){
    delete possibleBox;
    possibleBox = new QFrame;
    possibleBox->setParent(this);
    possibleBox->setGeometry(520, 370, 280, 130);
    for (int i = 0; i < valueArray.size(); ++i){
        QPushButton* q= new QPushButton(valueArray.at(i), possibleBox);
        if(i<5){
            q->setGeometry(0+i*50,0,50,50);
        }
        else{
            q->setGeometry(0+(i-5)*50,51,50,50);
        }
        connect(q,SIGNAL(clicked(bool)),this,SLOT(enterPossibleValue()));
    }
    possibleBox->setVisible(true);
}

void MainWindow::enterPossibleValue(){
    QPushButton* entry = (QPushButton*) QObject::sender();
    slot[currentX][currentY]->setText(entry->text());
    if(entry->text()!=""){
        stepLocation->append(currentX);
        stepLocation->append(currentY);
        stepValue->append(entry->text());
    }
    undoList.append(slot[currentX][currentY]);
    highlightSlot();
}

void MainWindow::undo()
{
    for (int i=0;i<undoList.size();++i){
        qDebug()<<undoList.at(i)->text();
    }
    if (undoList.size()!=0){
        int i= undoList.size()-1;
        int x= undoList.at(i)->getX();
        int y= undoList.at(i)->getY();
        slot[x][y]->setText("");
        slot[x][y]->setStyleSheet("background-color:yellow");
        undoList.removeAt(i);
    }
}

void MainWindow::highlightSlot(){
    int count =9;
    int hlSlotX;
    int hlSlotY;

    // check row
    for(int i=0;i<9;i++){
        if(slot[currentX][i]->text().isEmpty()==true){
            hlSlotX = slot[currentX][i]->getX();
            hlSlotY = slot[currentX][i]->getY();
        }
        else if(slot[currentX][i]->text().isEmpty()==false){
            count =count -1;
        }
    }
    if(count == 1){
        slot[hlSlotX][hlSlotY]->setStyleSheet("background-color:green");
    }

    //check column
    count =9;
    for(int i=0;i<9;i++){
        if(slot[i][currentY]->text().isEmpty()==true){
            hlSlotX = slot[i][currentY]->getX();
            hlSlotY = slot[i][currentY]->getY();
        }
        else if(slot[i][currentY]->text().isEmpty()==false){
            count =count -1;
        }
    }
    if(count == 1){
        slot[hlSlotX][hlSlotY]->setStyleSheet("background-color:green");
    }

    //check square
    count =9;
    int a = currentX/3*3;
    int c= a+3;
    int b = currentY/3*3;
    int d=b+3;
    for(int m=a;m<c;m++){
        for(int n=b;n<d;n++){
            if(slot[m][n]->text().isEmpty()==true){
                hlSlotX = slot[m][n]->getX();
                hlSlotY = slot[m][n]->getY();
            }
            else if(slot[m][n]->text().isEmpty()==false){
                count =count -1;
            }
        }
    }
    if(count == 1){
        slot[hlSlotX][hlSlotY]->setStyleSheet("background-color:green");
    }
}

void MainWindow::clear() // clears the grid completely
{
    int N = subrow*subrow;
    for (int x = 0; x < N; ++x)
    {
        for (int y = 0; y < N; ++y)
        {
            slot[x][y]->clear();
        }
    }
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Sudoku Save (*.txt)"),"",tr("Text (*.txt)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        QTextStream out(&file);
        int N = subrow*subrow;
        QString state;
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());

            return;
        }
        for (int x = 0; x < N; ++x)
        {
            int a = 0;
            for (int y = 0; y < N; ++y)
            {
                if (slot[x][y]->text().length()>0)
                {
                    state.append(slot[x][y]->text());
                    a++;
                }
                else{

                    state.append(tr("0"));
                    a++;
                }
                if (a == 9){
                    state.append(tr("\r\n"));
                }
            }
            qDebug() << "a" << a;
        }
        out << state;
    }
}

void MainWindow::open()
{
    this->clear();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Reload your sudoku"), "", tr("Text (*.txt)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QFile inputFile(fileName);
        if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::information(this, tr("Unable to open file"),inputFile.errorString());
            return;
        }
        QTextStream in(&inputFile);
        QString state;
        int k = 0;
        while (!in.atEnd()){
            state = in.readLine();
            for(int i=0; i<state.size(); ++i){
                QString s(state[i]);
                if (state.at(i)=='0'){
                }
                else{
                    slot[k][i]->setText(state.at(i));
                    slot[k][i]->setDisabled(true);

                }
            }
            ++k;
        }
    }
}

void MainWindow::inputDefaultFile(){
    QFile inputFile("Sodoku.txt");
    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in (&inputFile);
        int k=0;

        while (!in.atEnd()){
            QString line= in.readLine();

            for(int i=0; i<line.size(); ++i){
                if (line.at(i)=='0'){
                    connect(slot[k][i],SIGNAL(selectionChanged()),this,SLOT(currentSlot()));
                    connect(slot[k][i],SIGNAL(textEdited(const QString&)),this,SLOT(handleButton()));
                }
                else{
                    slot[k][i]->setText(line.at(i));
                    slot[k][i]->setDisabled(true);
                }
            }
            ++k;
        }
        inputFile.close();
    }
}

void MainWindow::replay(){
    //reset to default
    for(int i =0; i<undoList.size(); i++){
        int x= undoList.at(i)->getX();
        int y= undoList.at(i)->getY();
        slot[x][y]->setText("");
    }

    QPushButton* forwardButton= new QPushButton("Forward", this);
    forwardButton->setGeometry(660,160,120,70);
    forwardButton->setVisible(true);
    connect(forwardButton,SIGNAL(clicked(bool)),this,SLOT(forward()));

}

void MainWindow::forward(){
    if(stepValue->size()>0){
        int x = stepLocation->at(0);
        stepLocation->pop_front();
        int y = stepLocation->at(0);
        stepLocation->pop_front();
        slot[x][y]->setText(stepValue->at(0));
        stepValue->pop_front();
    }
}

void MainWindow::checkDuplicate(){
    duplicateError->setVisible(false);
    if(slot[currentX][currentY]->text()==""){

    }
    else{
        // check row
        for(int i=0;i<9;i++){
            if(slot[currentX][i]->text().isEmpty()==false){
                if(i==currentY){
                }
                else if(slot[currentX][i]->text()== slot[currentX][currentY]->text()){
                    slot[currentX][currentY]->setText("");
                    duplicateError->setVisible(true);
                    break;
                }
                //return true;
            }
        }

        // check column
        for(int i=0;i<9;i++){
            if(slot[i][currentY]->text().isEmpty()==false){
                if(i==currentX){
                }
                else if(slot[i][currentY]->text()== slot[currentX][currentY]->text()){
                    slot[currentX][currentY]->setText("");
                    duplicateError->setVisible(true);
                    break;
                }
                //return true;
            }
        }

        //check square
        int a = currentX/3*3;
        int c= a+3;
        int b = currentY/3*3;
        int d=b+3;
        for(int m=a;m<c;m++){
            for(int n=b;n<d;n++){
                if(m==currentX&&n==currentY){
                }
                else if(slot[m][n]->text()== slot[currentX][currentY]->text()){
                    slot[currentX][currentY]->setText("");
                    duplicateError->setVisible(true);
                    break;
                }
                // true;
            }
        }
    //return false;
    }
}
