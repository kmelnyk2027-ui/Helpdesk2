#include "ticketdialog.h"
#include "ui_ticketdialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QStyle>

TicketDialog::TicketDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TicketDialog)
    , currentMode(Mode::View)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->editButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    ui->saveButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));

    connect(ui->editButton, &QPushButton::clicked, this, &TicketDialog::onEditClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &TicketDialog::onSaveClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &TicketDialog::onCancelClicked);
}

TicketDialog::~TicketDialog()
{
    delete ui;
}

void TicketDialog::setMode(Mode mode)
{
    currentMode = mode;
    updateUiForMode();

    if (mode == Mode::New) {
        setWindowTitle(tr("New Ticket"));
        ui->titleEdit->clear();
        ui->statusCombo->setCurrentIndex(0);
        ui->priorityCombo->setCurrentIndex(0);
        ui->assigneeEdit->clear();
        ui->descriptionEdit->clear();
        currentTicket = Ticket();
        currentTicket.createdAt = QDateTime::currentDateTime();
    } else if (mode == Mode::Edit) {
        setWindowTitle(tr("Edit Ticket"));
    } else {
        setWindowTitle(tr("View Ticket"));
    }
}

void TicketDialog::setTicket(const Ticket &ticket)
{
    currentTicket = ticket;
    ui->titleEdit->setText(ticket.title);
    int idx = ui->statusCombo->findText(ticket.status);
    if (idx >= 0) ui->statusCombo->setCurrentIndex(idx);
    idx = ui->priorityCombo->findText(ticket.priority);
    if (idx >= 0) ui->priorityCombo->setCurrentIndex(idx);
    ui->assigneeEdit->setText("");
    ui->descriptionEdit->setPlainText("Sample description for demonstration.");
}

void TicketDialog::updateUiForMode()
{
    bool editable = (currentMode == Mode::Edit || currentMode == Mode::New);
    ui->titleEdit->setReadOnly(!editable);
    ui->statusCombo->setEnabled(editable);
    ui->priorityCombo->setEnabled(editable);
    ui->assigneeEdit->setReadOnly(!editable);
    ui->descriptionEdit->setReadOnly(!editable);

    ui->editButton->setVisible(currentMode == Mode::View);
    ui->saveButton->setVisible(editable);
    ui->cancelButton->setVisible(editable);
}

void TicketDialog::onEditClicked()
{
    setMode(Mode::Edit);
}

void TicketDialog::onSaveClicked()
{
    if (ui->titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation"), tr("Title cannot be empty."));
        return;
    }
    currentTicket.title = ui->titleEdit->text().trimmed();
    currentTicket.priority = ui->priorityCombo->currentText();
    currentTicket.status = ui->statusCombo->currentText();
    emit ticketSaved(currentTicket);
    if (currentMode == Mode::New) {
        close();
    } else {
        setMode(Mode::View);
    }
}

void TicketDialog::onCancelClicked()
{
    if (currentMode == Mode::New) {
        close();
    } else {
        setMode(Mode::View);
    }
}
