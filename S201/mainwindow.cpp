#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openJsonAction = new QAction(tr("Open JSON"), this);
    QAction *openXmlAction  = new QAction(tr("Open XML"), this);
    QAction *saveAction     = new QAction(tr("Save"), this);
    QAction *saveAsAction   = new QAction(tr("Save As"), this);
    QAction *quitAction     = new QAction(tr("Quit"), this);

    fileMenu->addAction(openJsonAction);
    fileMenu->addAction(openXmlAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);

    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    QWidget *master = new QWidget();
    QWidget *detail = new QWidget();

    splitter->addWidget(master);
    splitter->addWidget(detail);

    QVBoxLayout *layoutMaster = new QVBoxLayout();
    master->setLayout(layoutMaster);

    filtre = new QLineEdit();
    liste = new QListWidget();

    layoutMaster->addWidget(filtre);
    layoutMaster->addWidget(liste);

    QFormLayout *layoutDetail = new QFormLayout();
    detail->setLayout(layoutDetail);

    lastName = new QLineEdit();
    firstName = new QLineEdit();
    birthDate = new QDateEdit();
    studentId = new QLineEdit();
    ineNumber = new QLineEdit();
    phoneNumber = new QLineEdit();
    amuEmail = new QLineEdit();
    personalEmail = new QLineEdit();
    personalAddress = new QLineEdit();
    parentAddress = new QLineEdit();
    photoPath = new QLabel();
    section = new QComboBox();

    layoutDetail->addRow(tr("LastName :"), lastName);
    layoutDetail->addRow(tr("FirstName :"), firstName);
    layoutDetail->addRow(tr("BirthDate :"), birthDate);
    layoutDetail->addRow(tr("StudentId :"), studentId);
    layoutDetail->addRow(tr("IneNumber :"), ineNumber);
    layoutDetail->addRow(tr("PhoneNumber :"), phoneNumber);
    layoutDetail->addRow(tr("AmuEmail :"), amuEmail);
    layoutDetail->addRow(tr("PersonalEmail :"), personalEmail);
    layoutDetail->addRow(tr("PersonalAddress :"), personalAddress);
    layoutDetail->addRow(tr("ParentAddress :"), parentAddress);
    layoutDetail->addRow(tr("PhotoPath :"), photoPath);
    layoutDetail->addRow(tr("Section :"), section);
}

MainWindow::~MainWindow()
{

}