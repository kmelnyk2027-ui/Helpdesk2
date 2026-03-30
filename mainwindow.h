#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ticketmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewTicket();
    void onViewTicket();
    void onEditTicket();
    void onDeleteTicket();
    void onExit();
    void onAbout();
    void onTableSelectionChanged();

private:
    Ui::MainWindow *ui;
    TicketTableModel *model;
    int currentSelectedRow() const;
};

#endif
