#include "ticketmodel.h"

TicketTableModel::TicketTableModel(QObject *parent)
    : QAbstractTableModel(parent), nextId(1)
{
    Ticket t;
    t.id = 1; t.title = "Cannot login"; t.priority = "High"; t.status = "Open";
    t.createdAt = QDateTime::currentDateTime(); tickets.append(t);
    t.id = 2; t.title = "Slow network"; t.priority = "Medium"; t.status = "In Progress";
    tickets.append(t);
    t.id = 3; t.title = "Feature request"; t.priority = "Low"; t.status = "Closed";
    tickets.append(t);
    nextId = 4;
}

int TicketTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tickets.size();
}

int TicketTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant TicketTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    int row = index.row();
    int col = index.column();
    if (row < 0 || row >= tickets.size()) return QVariant();
    const Ticket &t = tickets.at(row);
    switch (col) {
    case IdColumn: return t.id;
    case TitleColumn: return t.title;
    case PriorityColumn: return t.priority;
    case StatusColumn: return t.status;
    case CreatedAtColumn: return t.createdAt.toString("yyyy-MM-dd hh:mm");
    default: return QVariant();
    }
}

QVariant TicketTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case IdColumn: return "ID";
    case TitleColumn: return "Title";
    case PriorityColumn: return "Priority";
    case StatusColumn: return "Status";
    case CreatedAtColumn: return "Created At";
    default: return QVariant();
    }
}

void TicketTableModel::addTicket(const Ticket &ticket)
{
    beginInsertRows(QModelIndex(), tickets.size(), tickets.size());
    Ticket newTicket = ticket;
    newTicket.id = nextId++;
    tickets.append(newTicket);
    endInsertRows();
}

void TicketTableModel::updateTicket(int row, const Ticket &ticket)
{
    if (row < 0 || row >= tickets.size()) return;
    tickets[row] = ticket;
    emit dataChanged(index(row, 0), index(row, ColumnCount - 1));
}

void TicketTableModel::removeTicket(int row)
{
    if (row < 0 || row >= tickets.size()) return;
    beginRemoveRows(QModelIndex(), row, row);
    tickets.removeAt(row);
    endRemoveRows();
}

const Ticket &TicketTableModel::ticketAt(int row) const
{
    static Ticket empty;
    if (row < 0 || row >= tickets.size()) return empty;
    return tickets.at(row);
}
