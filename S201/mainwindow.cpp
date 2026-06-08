#include "mainwindow.h"
#include <section.h>

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
    connect(openJsonAction, &QAction::triggered, this, &MainWindow::onOpenJson);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAs);

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
    section->addItems({"Info BUT 1", "Info BUT 2", "Info BUT 3", "MMI BUT 1", "MMI BUT 2", "MMI BUT 3"});
    btnSaveStudent = new QPushButton(tr("Sauvegarder"));

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
    layoutDetail->addRow(btnSaveStudent);

    studentManager.populateWithDefaultTestData();
    populateMasterList();
    setupConnections();
}

MainWindow::~MainWindow()
{

}

void MainWindow::populateMasterList()
{
    liste->clear();
    QList<Student> etudiants = studentManager.getStudents();

    for (const Student& s : etudiants)
    {
        QString name = s.getDisplayName();
        liste->addItem(name);
    }
}

void MainWindow::setupConnections()
{
    connect(liste, &QListWidget::currentRowChanged, this, &MainWindow::onStudentSelected);
    connect(btnSaveStudent, &QPushButton::clicked, this, &MainWindow::onSaveStudent);

}

void MainWindow::populateDetailForm(int index)
{
    Student s = studentManager.getStudent(index);
    lastName->setText(s.getLastName());
    firstName->setText(s.getFirstName());
    birthDate->setDate(s.getBirthDate());
    studentId->setText(s.getStudentId());
    ineNumber->setText(s.getIneNumber());
    phoneNumber->setText(s.getPhoneNumber());
    amuEmail->setText(s.getAmuEmail());
    personalEmail->setText(s.getPersonalEmail());
    personalAddress->setText(s.getPersonalAddress());
    parentAddress->setText(s.getParentAddress());
    photoPath->setText(s.getPhotoPath());
    section->setCurrentText(sectionToString(s.getSection()));

}

void MainWindow::clearDetailForm()
{
}

bool MainWindow::checkUnsavedChanges()
    {
        if (!studentManager.isModified()) {
            return true;
        }

        QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Pending modifications"),
                                                               tr("Unsaved changes.\n"
                                                                  "Do you want to save them before leaving ?"),
                                                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            onSave();
            return true;
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }

        return true;
    }

void MainWindow::closeEvent(QCloseEvent *event)
    {
        if (checkUnsavedChanges()) {
            event->accept();
        } else {
            event->ignore();
        }
    }

void MainWindow::retranslateUi()
{
}

void MainWindow::onOpenJson()
{
    if (!checkUnsavedChanges()) return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir JSON"), "", tr("Fichiers JSON (*.json)"));
    if (fileName.isEmpty()) return;

    if (studentManager.loadJson(fileName)) {
        populateMasterList();
        clearDetailForm();
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de lire ce fichier JSON."));
    }
}

void MainWindow::onOpenXml()
{
}

void MainWindow::onSave()
{
    QString currentPath = studentManager.getCurrentFilePath();
    if (currentPath.isEmpty()) {
        onSaveAs();
    } else {
        if (studentManager.saveJson(currentPath)) {
            QMessageBox::information(this, tr("Succès"), tr("Fichier mis à jour."));
        }
    }
}

void MainWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder JSON"), "", tr("Fichiers JSON (*.json)"));
    if (fileName.isEmpty()) return;

    if (studentManager.saveJson(fileName)) {
        QMessageBox::information(this, tr("Succès"), tr("Fichier sauvegardé avec succès."));
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de sauvegarder le fichier."));
    }
}

void MainWindow::onStudentSelected(int index)
{
    if (index < 0) return;
    populateDetailForm(index);
}

void MainWindow::onFilterChanged(QString /*text*/)
{
}

void MainWindow::onAddStudent()
{
}

void MainWindow::onDeleteStudent()
{
}

void MainWindow::onSaveStudent()
{
    int index = liste->currentRow();
    if (index < 0) return;

    Student s = studentManager.getStudent(index);

    s.setLastName(lastName->text());
    s.setFirstName(firstName->text());
    s.setBirthDate(birthDate->date());
    s.setStudentId(studentId->text());
    s.setIneNumber(ineNumber->text());
    s.setPhoneNumber(phoneNumber->text());
    s.setAmuEmail(amuEmail->text());
    s.setPersonalEmail(personalEmail->text());
    s.setPersonalAddress(personalAddress->text());
    s.setParentAddress(parentAddress->text());
    s.setPhotoPath(photoPath->text());
    s.setSection(sectionFromString(section->currentText()));

    studentManager.updateStudent(index, s);
    liste->currentItem()->setText(s.getDisplayName());
    studentManager.setModified(true);
}

void MainWindow::onAddAbsence()
{
}

void MainWindow::onEditAbsence(int /*index*/)
{
}

void MainWindow::onDeleteAbsence(int /*index*/)
{
}

void MainWindow::onLoadPhoto()
{
}