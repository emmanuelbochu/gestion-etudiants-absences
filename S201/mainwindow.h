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


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
