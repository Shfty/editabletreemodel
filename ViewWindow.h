#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>

class QAbstractItemModel;

namespace Ui {
	class ViewWindow;
}

class ViewWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit ViewWindow(QWidget *parent = nullptr);
		~ViewWindow();

		void setViewModel(QAbstractItemModel* model);

	private:
		Ui::ViewWindow *ui;
};

#endif // VIEWWINDOW_H
