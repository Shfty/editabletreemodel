#ifndef RECTEDITORWIDGET_H
#define RECTEDITORWIDGET_H

#include <QWidget>

class QSpinBox;

class RectEditorWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit RectEditorWidget(QWidget *parent = nullptr);

		void setRect(QRect rect);
		QRect getRect();

	signals:

	public slots:

	private:
		QSpinBox* spinBoxX;
		QSpinBox* spinBoxY;
		QSpinBox* spinBoxW;
		QSpinBox* spinBoxH;

};

#endif // RECTEDITORWIDGET_H