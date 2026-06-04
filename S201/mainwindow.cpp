#include "mainwindow.h"
#include "xmlmanager.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(tr("IUT Students Management (Master-Detail)"));
    resize(900, 600);

    createMenusAndActions();

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    temporaryMasterList = new QListWidget(splitter);

    detailWidget = new DetailWidget(splitter);

    splitter->addWidget(temporaryMasterList);
    splitter->addWidget(detailWidget);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    setCentralWidget(splitter);

    connect(temporaryMasterList, &QListWidget::currentRowChanged, this, &MainWindow::onMasterSelectionChanged);

    Student test1("Mine", "Maxence", "22601234");
    test1.setIne("1234567890X");
    test1.setSchoolEmail("maxence.mine@etu.univ-amu.fr");
    test1.addAbsence(Absence(QDate::currentDate(), false, tr("Soutenance")));

    Student test2("Dupont", "Jean", "22605678");
    test2.setIne("0987654321A");

    studentList.append(test1);
    studentList.append(test2);

    updateMasterList();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenusAndActions() {
    fileMenu = menuBar()->addMenu(tr("&File"));

    openXmlAction = new QAction(tr("&Open XML..."), this);
    saveXmlAction = new QAction(tr("&Save XML"), this);
    saveAsXmlAction = new QAction(tr("Save XML &As..."), this);
    exitAction = new QAction(tr("&Exit"), this);

    fileMenu->addAction(openXmlAction);
    fileMenu->addAction(saveXmlAction);
    fileMenu->addAction(saveAsXmlAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(openXmlAction, &QAction::triggered, this, &MainWindow::onOpenXmlTriggered);
    connect(saveXmlAction, &QAction::triggered, this, &MainWindow::onSaveXmlTriggered);
    connect(saveAsXmlAction, &QAction::triggered, this, &MainWindow::onSaveAsXmlTriggered);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::updateMasterList() {
    temporaryMasterList->clear();
    for (int i = 0; i < studentList.size(); ++i) {
        Student s = studentList.at(i);
        QString itemText = s.getLastName().toUpper() + " " + s.getFirstName() + " (" + s.getStudentID() + ")";
        temporaryMasterList->addItem(itemText);
    }
}

void MainWindow::onMasterSelectionChanged() {
    int index = temporaryMasterList->currentRow();
    if (index >= 0 && index < studentList.size()) {
        detailWidget->setStudent(&studentList[index]);
    }
}

void MainWindow::onOpenXmlTriggered() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", "XML Files (*.xml)");
    if (!filePath.isEmpty()) {
        studentList = XmlManager::loadFromXml(filePath);
        currentFilePath = filePath;
        updateMasterList();
        if (!studentList.isEmpty()) {
            temporaryMasterList->setCurrentRow(0);
        }
    }
}

void MainWindow::onSaveXmlTriggered() {
    if (currentFilePath.isEmpty()) {
        onSaveAsXmlTriggered();
    } else {
        XmlManager::saveToXml(currentFilePath, studentList);
    }
}

void MainWindow::onSaveAsXmlTriggered() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save XML File"), "", "XML Files (*.xml)");
    if (!filePath.isEmpty()) {
        currentFilePath = filePath;
        XmlManager::saveToXml(currentFilePath, studentList);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, tr("Exit Application"),
                                                               tr("Are you sure you want to exit?\nMake sure you saved your data."),
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}
