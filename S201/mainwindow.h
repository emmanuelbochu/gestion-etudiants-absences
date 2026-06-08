#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListWidget>
#include <QLineEdit>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QString>
#include <QPushButton>

#include "studentmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLineEdit *filtre;
    QListWidget *liste;
    QLineEdit *lastName;
    QLineEdit *firstName;
    QDateEdit *birthDate;
    QLineEdit *studentId;
    QLineEdit *ineNumber;
    QLineEdit *phoneNumber;
    QLineEdit *amuEmail;
    QLineEdit *personalEmail;
    QLineEdit *personalAddress;
    QLineEdit *parentAddress;
    QLabel *photoPath;
    QComboBox *section;
    QPushButton *btnSaveStudent;

    StudentManager studentManager;
    int currentIndex;

    void setupConnections();
    void populateMasterList();
    void populateDetailForm(int index);
    void clearDetailForm();
    bool checkUnsavedChanges();
    void closeEvent(QCloseEvent* event) override;
    void retranslateUi();

private slots:
    void onOpenJson();
    void onOpenXml();
    void onSave();
    void onSaveAs();
    void onStudentSelected(int index);
    void onFilterChanged(QString text);
    void onAddStudent();
    void onDeleteStudent();
    void onSaveStudent();
    void onAddAbsence();
    void onEditAbsence(int index);
    void onDeleteAbsence(int index);
    void onLoadPhoto();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H