#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_ipc = new MCIPC("MCMediaGui",this);
	m_ipc->connectToHost("127.0.0.1",12345);
//	connect(m_ipc,SIGNAL(si_incomingSubscribedMessage(QString,QByteArray)),this,SLOT(incomingSubscribed(QString,QByteArray)));
	connect(m_ipc,SIGNAL(si_publishMessage(QString,QByteArray)),this,SLOT(incomingSubscribed(QString,QByteArray)));
	connect(m_ipc,SIGNAL(si_connected()),this,SLOT(ipcConnected()));
	ui->tableWidget->setColumnCount(4);
	connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(addButtonClicked()));
	connect(ui->savePushButton,SIGNAL(clicked(bool)),this,SLOT(saveButtonClicked()));
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->playlistTableWidget->setColumnCount(4);
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::incomingSubscribed(QString id, QByteArray data)
{
	QJsonDocument doc = QJsonDocument::fromJson(data);
	QJsonArray docarray = doc.array();
	for (int i=0;i<docarray.size();i++)
	{
		QJsonObject mediafile = docarray.at(i).toObject();
		ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
		ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(mediafile.value("artist").toString()));
		ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(mediafile.value("title").toString()));
		ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(mediafile.value("album").toString()));
		ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(mediafile.value("path").toString()));
	}
}
void MainWindow::addButtonClicked()
{
	if (ui->tableWidget->selectedItems().size() == 0)
	{
		return;
	}
	QItemSelectionModel *select = ui->tableWidget->selectionModel();
	if (!select->hasSelection())
	{
		return;
	}
	for (int i=0;i<select->selectedRows().size();i++)
	{
		ui->playlistTableWidget->setRowCount(ui->playlistTableWidget->rowCount()+1);
		ui->playlistTableWidget->setItem(ui->playlistTableWidget->rowCount()-1,0,new QTableWidgetItem(ui->tableWidget->item(select->selectedRows().at(i).row(),0)->text()));
		ui->playlistTableWidget->setItem(ui->playlistTableWidget->rowCount()-1,1,new QTableWidgetItem(ui->tableWidget->item(select->selectedRows().at(i).row(),1)->text()));
		ui->playlistTableWidget->setItem(ui->playlistTableWidget->rowCount()-1,2,new QTableWidgetItem(ui->tableWidget->item(select->selectedRows().at(i).row(),2)->text()));
		ui->playlistTableWidget->setItem(ui->playlistTableWidget->rowCount()-1,3,new QTableWidgetItem(ui->tableWidget->item(select->selectedRows().at(i).row(),3)->text()));
	}
}
void MainWindow::saveButtonClicked()
{
	//Send this playlist to the server!
	QJsonObject playlist;
	QJsonArray playlistarray;
	for (int i=0;i<ui->playlistTableWidget->rowCount();i++)
	{
		playlistarray.append(ui->playlistTableWidget->item(i,3)->text());
	}
	playlist.insert("playlist",playlistarray);
	playlist.insert("title",ui->lineEdit->text());
	QJsonDocument doc(playlist);
	QByteArray jsonbytes = doc.toJson();
	m_ipc->sendMessage("MCMediaLibrary",jsonbytes);
}

void MainWindow::ipcConnected()
{
	m_ipc->subscribeMessage("MediaList");
}
