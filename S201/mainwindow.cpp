#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Menu "File"
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    // Actions
    QAction *openJsonAction = new QAction(tr("Open JSON"), this);
    QAction *openXmlAction  = new QAction(tr("Open XML"), this);
    QAction *saveAction     = new QAction(tr("Save"), this);
    QAction *saveAsAction   = new QAction(tr("Save As"), this);
    QAction *quitAction     = new QAction(tr("Quit"), this);

    // Ajout des actions au menu
    fileMenu->addAction(openJsonAction);
    fileMenu->addAction(openXmlAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    // Connexion Quit
    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);

    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    QWidget *master = new QWidget();
    QWidget *detail = new QWidget();

    splitter->addWidget(master);
    splitter->addWidget(detail);

    QVBoxLayout *layoutMaster = new QVBoxLayout();
    master->setLayout(layoutMaster);

    QLineEdit *filtre = new QLineEdit();
    QListWidget *liste = new QListWidget();

    layoutMaster->addWidget(filtre);
    layoutMaster->addWidget(liste);

    QFormLayout *layoutDetail = new QFormLayout();
    detail->setLayout(layoutDetail);

    QLineEdit *lastName = new QLineEdit();
    QDateEdit *date = new QDateEdit();

    layoutDetail->addRow(tr("LastName :"), lastName);
    layoutDetail->addRow(tr("Date :"), date);
}

MainWindow::~MainWindow()
{

}