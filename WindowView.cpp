#include "WindowView.h"

#include <QDebug>
#include <QPainter>

WindowView::WindowView(QWidget* parent):
	QAbstractItemView(parent)
{

}

QRect WindowView::visualRect(const QModelIndex& index) const
{
	QVector<QModelIndex> treePath;
	treePath.append(index);
	while(treePath.front().parent().isValid())
	{
		treePath.push_front(treePath.front().parent());
	}

	QRect rect(0, 0, geometry().width(), geometry().height());

	for(int i = 0; i < treePath.length(); ++i)
	{
		QModelIndex treeItem = treePath.at(i);
		int width = rect.width() / model()->rowCount(treeItem.parent());
		rect.setLeft(rect.left() + treeItem.row() * width);
		rect.setWidth(width);

		rect.setTop(i * 20);
		rect.setHeight(20);

		if(index.column() == 4 && model()->data(index) != QVariant::Invalid)
		{
			rect.setTop(rect.top() + 20);
			rect.setHeight(geometry().height() - 20 - i * 20);
		}
	}

	return rect;
}

void WindowView::scrollTo(const QModelIndex& index, QAbstractItemView::ScrollHint hint)
{

}

QModelIndex WindowView::indexAt(const QPoint& point) const
{
	return QModelIndex();
}

QModelIndex WindowView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	return QModelIndex();
}

int WindowView::horizontalOffset() const
{
	return 0;
}

int WindowView::verticalOffset() const
{
	return 0;
}

bool WindowView::isIndexHidden(const QModelIndex& index) const
{
	return false;
}

void WindowView::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command)
{

}

QRegion WindowView::visualRegionForSelection(const QItemSelection& selection) const
{
	return QRegion();
}

void WindowView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
	viewport()->update();
	//qDebug() << "Data Changed";
	//qDebug() << topLeft.row() << ", " << topLeft.column() << ", " << bottomRight.row() << ", " << bottomRight.column() << roles;
}

void WindowView::rowsInserted(const QModelIndex& parent, int start, int end)
{
	viewport()->update();
	//qDebug() << "Rows Inserted";
	//qDebug() << parent.row() << ", " << parent.column() << start << ", " << end;
}

void WindowView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
	viewport()->update();
	//qDebug() << "Rows about to be removed";
	//qDebug() << parent.row() << ", " << parent.column() << start << ", " << end;
}

QVector<QModelIndex> toRender(QAbstractItemModel* model, QModelIndex parent = QModelIndex())
{
	QVector<QModelIndex> candidates;

	for(int r = 0; r < model->rowCount(parent); ++r)
	{
		QModelIndex titleIndex = model->index(r, 0, parent);
		candidates << titleIndex;

		QModelIndex hwndIndex = model->index(r, 4, parent);
		if(model->data(hwndIndex) != QVariant::Invalid)
		{
			candidates << hwndIndex;
		}

		if( model->hasChildren(titleIndex) ) {
			candidates << toRender(model, titleIndex);
		}
	}

	return candidates;
}

void WindowView::paintEvent(QPaintEvent* paintEvent)
{
	if (!itemDelegate())
		return;

	QStyleOptionViewItem option;
	QPainter painter(viewport());
	const QVector<QModelIndex> toBeRendered = toRender(model());
	const int rowCount = this->model()->rowCount();
	QVector<QModelIndex>::const_iterator end = toBeRendered.constEnd();
	for (QVector<QModelIndex>::const_iterator it = toBeRendered.constBegin(); it != end; ++it) {
		Q_ASSERT((*it).isValid());

		if (rowCount == 1)
			option.viewItemPosition = QStyleOptionViewItem::OnlyOne;
		else if ((*it).row() == 0)
			option.viewItemPosition = QStyleOptionViewItem::Beginning;
		else if ((*it).row() == rowCount - 1)
			option.viewItemPosition = QStyleOptionViewItem::End;
		else
			option.viewItemPosition = QStyleOptionViewItem::Middle;

		option.rect = visualRect(*it);
		option.state |= QStyle::State_Enabled;

		painter.fillRect(option.rect + QMargins(-1, -1, -1, -1), QBrush(QColor(100, 100, 255, 127)));
		painter.drawRect(option.rect + QMargins(0, 0, -1, -1));

		itemDelegate()->paint(&painter, option, *it);
	}
}
