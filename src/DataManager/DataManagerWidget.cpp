#include "DataManagerWidget.h"
#include "ui_DataManagerWidget.h"

DataManagerWidget::DataManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataManagerWidget)
{
    ui->setupUi(this);

    data_manager_thread_ = new QThread();
    data_manager_ = new DataManager();
    data_manager_moved_ = false;

    this->ui->textEdit_database_type->setText("QMYSQL");
    this->ui->textEdit_connectionm_name->setText("Shop_Heat_Map");
    this->ui->textEdit_database_filename->setText("ShopHeatMap");
    this->ui->textEdit_host_name->setText("192.168.0.167");
    this->ui->textEdit_port->setText("4000");
    this->ui->textEdit_username->setText("root");
    this->ui->textEdit_password->setText("123456");

    this->ui->textEdit_table_name->setText("CameraCoord");
    this->ui->textEdit_QueryKey->setText("CoordNo");
    this->ui->textEdit_QueryValue->setText("1");

    QJsonObject insert_Obj;
//    insert_Obj.insert(query_key, query_value);

//    bool success = data_manager_->insertData("", query_Obj);
}

DataManagerWidget::~DataManagerWidget()
{
    delete ui;
    delete data_manager_;
    delete data_manager_thread_;
}

void DataManagerWidget::init()
{
    QString database_type = this->ui->textEdit_database_type->toPlainText();
    QString connectionm_name = this->ui->textEdit_connectionm_name->toPlainText();
    QString database_filename = this->ui->textEdit_database_filename->toPlainText();
    QString host_name = this->ui->textEdit_host_name->toPlainText();
    QString port = this->ui->textEdit_port->toPlainText();
    QString username = this->ui->textEdit_username->toPlainText();
    QString password = this->ui->textEdit_password->toPlainText();

    if(data_manager_moved_)
    {
        data_manager_->database_manager_->disconnect();
    }

    data_manager_->init(database_type,
                        connectionm_name,
                        database_filename,
                        host_name,
                        port,
                        username,
                        password);

    if(!data_manager_moved_)
    {
        data_manager_->moveToThread(data_manager_thread_);
        data_manager_thread_->start();
        data_manager_moved_ = true;
    }
}

void DataManagerWidget::on_Btn_Connect_clicked()
{
    init();
}

void DataManagerWidget::on_Btn_Query_clicked()
{
    QString table_name = this->ui->textEdit_table_name->toPlainText();
    QString query_key = this->ui->textEdit_QueryKey->toPlainText();
    QString query_value = this->ui->textEdit_QueryValue->toPlainText();

    if(table_name == "")
    {
        qDebug() << "DataManagerWidget::on_Btn_Query_clicked : please input query table_name.";
        return;
    }
    if(query_key == "")
    {
        qDebug() << "DataManagerWidget::on_Btn_Query_clicked : please input query query_key.";
        return;
    }
    if(query_value == "")
    {
        qDebug() << "DataManagerWidget::on_Btn_Query_clicked : please input query query_value.";
        return;
    }

    QJsonObject query_Obj;
    query_Obj.insert(query_key, query_value);

    bool success = data_manager_->queryData(table_name, query_Obj);

    if(!success)
    {
        qDebug() << "DataManagerWidget::on_Btn_Query_clicked : query failed.";
        return;
    }

    QList<QJsonObject> query_result = data_manager_->getQueryResultList();

    if(query_result.size() == 0)
    {
        qDebug() << "DataManagerWidget::on_Btn_Query_clicked : no data in sql.";
        return;
    }

    QDateTime date_time;

    this->ui->plainTextEdit_msg->appendPlainText(date_time.currentDateTime().toString());

    for(int i = 0; i < query_result.size(); ++i)
    {
        QStringList keys = query_result[i].keys();

        QString output_msg = QString::number(i) + " :\n";

        for(QString key : keys)
        {
            output_msg += "\t" + key + " : " + query_result[i].value(key).toString() + "\n";
        }

        this->ui->plainTextEdit_msg->appendPlainText(output_msg);
    }
}
