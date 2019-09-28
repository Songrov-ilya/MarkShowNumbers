#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ManagerLabelNumber.h"
#include <QMainWindow>
#include <QtEvents>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ManagerLabelNumber manager;
    QSettings settings;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void setupMenuBar();
    void setupConnections();
    void createItems();
    void createTable(QTableWidget &tableWidget, QPoint labelPoint);
    void refreshPosition(QTableWidget &tableWidget);
    void readSettings();
    void writeSettings();

protected:
    virtual void closeEvent(QCloseEvent *);

public slots:
    void slotShow();
    void slotHide();
    void onActionOpenImage();
    void onActionAboutApp();
    void onActionDeveloper();
    void onActionPositionNumber();
    void onActionSizeImage();
    void onActionOriginalSizeImage();
    void onActionClearImages();
    void slotItemPressed(QListWidgetItem *item);
    void slotItemActivated(QListWidgetItem *);
    void slotSelectionChanged();

};

#endif // MAINWINDOW_H
