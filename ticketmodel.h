#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDateTime>

struct Ticket
{
    int id;
    QString title;
    QString priority;
    QString status;
    QDateTime createdAt;
};

class TicketTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        IdColumn = 0,
        TitleColumn,
        PriorityColumn,
        StatusColumn,
        CreatedAtColumn,
        ColumnCount
    };

    explicit TicketTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addTicket(const Ticket &ticket);
    void updateTicket(int row, const Ticket &ticket);
    void removeTicket(int row);

    const Ticket &ticketAt(int row) const;

private:
    QVector<Ticket> tickets;
    int nextId;
};

#endif
