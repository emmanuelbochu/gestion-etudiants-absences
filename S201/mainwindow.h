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

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
