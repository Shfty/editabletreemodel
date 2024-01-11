#ifndef WINDOWVIEW_H
#define WINDOWVIEW_H

#include <QAbstractItemView>

class WindowView : public QAbstractItemView
{
	public:
		WindowView(QWidget* parent);

		QRect visualRect(const QModelIndex &index) const override;
		void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
		QModelIndex indexAt(const QPoint &point) const override;

		QModelIndex moveCursor(CursorAction cursorAction,
									   Qt::KeyboardModifiers modifiers) override;

		int horizontalOffset() const override;
		int verticalOffset() const override;

		bool isIndexHidden(const QModelIndex &index) const override;

		void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
		QRegion visualRegionForSelection(const QItemSelection &selection) const override;

		void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
		void rowsInserted(const QModelIndex &parent, int start, int end) override;
		void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) override;

		void paintEvent(QPaintEvent* paintEvent) override;
};

#endif // WINDOWVIEW_H