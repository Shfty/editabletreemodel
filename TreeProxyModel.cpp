#include "TreeProxyModel.h"

#include <QRect>
#include <QTextStream>
#include <QDebug>

#include "Win.h"
#include "TreeEnums.h"

TreeProxyModel::TreeProxyModel()
{

}

QVariant TreeProxyModel::data(const QModelIndex& index, int role) const
{
	if(role == Qt::EditRole || role != Qt::DisplayRole)
	{
		return QIdentityProxyModel::data(index, role);
	}

	const QModelIndex sourceIndex = mapToSource(index);
	const QVariant originalData = sourceModel()->data(sourceIndex, Qt::DisplayRole);

	if(originalData.userType() == qMetaTypeId<QRect>()) {
		QRect rect = qvariant_cast<QRect>(originalData);

		QString text;
		QTextStream out(&text);
		out << "Pos: ";
		out << rect.x() << ", " << rect.y();
		out << "\n";
		out << "Size: ";
		out << rect.width() << ", " << rect.height();

		return text;
	}
	else if(originalData.userType() == qMetaTypeId<Axis>()) {
		Axis axis = qvariant_cast<Axis>(originalData);

		switch(axis)
		{
			case Axis::A_None:
				return "None";
			case Axis::A_Horizontal:
				return "Horizontal";
			case Axis::A_Vertical:
				return "Vertical";
		}
	}
	else if(originalData.userType() == qMetaTypeId<Layout>()) {
		Layout layout = qvariant_cast<Layout>(originalData);

		switch(layout)
		{
			case Layout::L_None:
				return "None";
			case Layout::L_Tabbed:
				return "Tabbed";
			case Layout::L_Split:
				return "Split";
		}
	}
	else if(originalData.userType() == qMetaTypeId<HWND>()) {
		HWND hwnd = qvariant_cast<HWND>(originalData);
		return "0x" + QString("%1").arg((quint64)hwnd, 0, 16);
	}

	return QIdentityProxyModel::data(index, role);
}

bool TreeProxyModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{
	const QModelIndex sourceIndex = mapToSource(sourceParent);
	const QModelIndex destinationIndex = mapToSource(destinationParent);
	return sourceModel()->moveRows(sourceIndex, sourceRow, count, destinationIndex, destinationChild);
}
