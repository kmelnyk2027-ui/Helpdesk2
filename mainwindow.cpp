#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ticketdialog.h"
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QApplication>
#include <QStyle>
#include <QStatusBar>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new TicketTableModel(this);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->tableView->horizontalHeader()->setSectionResizeMode(TicketTableModel::TitleColumn, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(TicketTableModel::IdColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(TicketTableModel::PriorityColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(TicketTableModel::StatusColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(TicketTableModel::CreatedAtColumn, QHeaderView::ResizeToContents);

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onTableSelectionChanged);

    ui->actionNew->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    ui->actionView->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogInfoView));
    ui->actionEdit->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    ui->actionDelete->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));
    ui->actionExit->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->actionAbout->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewTicket);
    connect(ui->actionView, &QAction::triggered, this, &MainWindow::onViewTicket);
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onEditTicket);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteTicket);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

    onTableSelectionChanged();
    statusBar()->showMessage(tr("Ready"), 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::currentSelectedRow() const
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    if (selection && selection->hasSelection()) {
        return selection->selectedRows().first().row();
    }
    return -1;
}

void MainWindow::onTableSelectionChanged()
{
    bool hasSelection = (currentSelectedRow() != -1);
    ui->actionView->setEnabled(hasSelection);
    ui->actionEdit->setEnabled(hasSelection);
    ui->actionDelete->setEnabled(hasSelection);
}

void MainWindow::onNewTicket()
{
    TicketDialog *dialog = new TicketDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMode(TicketDialog::Mode::New);
    connect(dialog, &TicketDialog::ticketSaved, [this](const Ticket &ticket) {
        model->addTicket(ticket);
        statusBar()->showMessage(tr("Ticket added."), 2000);
    });
    dialog->show();
    statusBar()->showMessage(tr("Creating new ticket..."), 2000);
}

void MainWindow::onViewTicket()
{
    int row = currentSelectedRow();
    if (row < 0) return;

    const Ticket &t = model->ticketAt(row);
    TicketDialog *dialog = new TicketDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMode(TicketDialog::Mode::View);
    dialog->setTicket(t);
    dialog->show();
    statusBar()->showMessage(tr("Viewing ticket..."), 2000);
}

void MainWindow::onEditTicket()
{
    int row = currentSelectedRow();
    if (row < 0) return;

    const Ticket &t = model->ticketAt(row);
    TicketDialog *dialog = new TicketDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMode(TicketDialog::Mode::Edit);
    dialog->setTicket(t);
    connect(dialog, &TicketDialog::ticketSaved, [this, row](const Ticket &ticket) {
        model->updateTicket(row, ticket);
        statusBar()->showMessage(tr("Ticket updated."), 2000);
    });
    dialog->show();
    statusBar()->showMessage(tr("Editing ticket..."), 2000);
}

void MainWindow::onDeleteTicket()
{
    int row = currentSelectedRow();
    if (row < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Delete Ticket"),
        tr("Are you sure you want to delete the selected ticket?"),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        model->removeTicket(row);
        statusBar()->showMessage(tr("Ticket deleted."), 2000);
    }
}

void MainWindow::onExit()
{
    close();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("About Helpdesk"),
                       tr("<b>Helpdesk</b><br/>Version 1.0<br/><br/>"
                          "Ticket management application.<br/>"
                          "Designed with Qt Widgets."));
}
