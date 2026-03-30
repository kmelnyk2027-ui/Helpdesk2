#ifndef TICKETDIALOG_H
#define TICKETDIALOG_H

#include <QDialog>
#include "ticketmodel.h"

namespace Ui {
class TicketDialog;
}

class TicketDialog : public QDialog
{
    Q_OBJECT

public:
    enum class Mode { View, Edit, New };
    explicit TicketDialog(QWidget *parent = nullptr);
    ~TicketDialog();

    void setMode(Mode mode);
    void setTicket(const Ticket &ticket);

signals:
    void ticketSaved(const Ticket &ticket);

private slots:
    void onEditClicked();
    void onSaveClicked();
    void onCancelClicked();

private:
    Ui::TicketDialog *ui;
    Mode currentMode;
    void updateUiForMode();
    Ticket currentTicket;
};

#endif
