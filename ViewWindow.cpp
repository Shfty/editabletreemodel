#include "ViewWindow.h"
#include "ui_ViewWindow.h"

ViewWindow::ViewWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ViewWindow)
{
	ui->setupUi(this);

	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

ViewWindow::~ViewWindow()
{
	delete ui;
}

void ViewWindow::setViewModel(QAbstractItemModel* model)
{
	ui->view->setModel(model);
}
