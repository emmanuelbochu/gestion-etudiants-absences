#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListWidget>
#include "student.h"
#include "detailwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QList<Student> studentList;
    QString currentFilePath;

    QListWidget *temporaryMasterList;
    DetailWidget *detailWidget;

    QMenu *fileMenu;
    QAction *openXmlAction;
    QAction *saveXmlAction;
    QAction *saveAsXmlAction;
    QAction *exitAction;

    void createMenusAndActions();
    void updateMasterList();

private slots:
    void onOpenXmlTriggered();
    void onSaveXmlTriggered();
    void onSaveAsXmlTriggered();
    void onMasterSelectionChanged();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
