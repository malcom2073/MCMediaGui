#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mcipc.h"
#include <QByteArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	MCIPC *m_ipc;
private slots:
	void ipcConnected();
	void incomingSubscribed(QString id, QByteArray data);
	void addButtonClicked();
	void saveButtonClicked();
};

#endif // MAINWINDOW_H
