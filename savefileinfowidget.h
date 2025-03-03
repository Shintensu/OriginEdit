#ifndef SAVEFILEINFOWIDGET_H
#define SAVEFILEINFOWIDGET_H

#include <QWidget>

#include "SaveSlot.h"

namespace Ui {
class SaveFileInfoWidget;
}

class SaveFileInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SaveFileInfoWidget(QWidget *parent = nullptr);
    ~SaveFileInfoWidget();

public slots:
    void ReadValues(SaveSlot* saveSlot);
    void WriteValues(SaveSlot* saveSlot);

    void onHUpdate(int hour);
    void onMUpdate(int minute);
    void onSUpdate(int second);
    void onMSUpdate(double millisecond);

    void onRealYUpdate(qint32 year);
    void onRealDUpdate(qint32 day);
    void onRealHUpdate(qint32 hour);
    void onRealMUpdate(qint32 minute);
    void onRealSUpdate(qint32 second);
    void onRealMSUpdate(qint32 millisecond);
    void onRealMCSUpdate(qint32 microsecond);

    void UpdateAreaNameComboBoxIndex();
    void UpdateCurrentAreaNameID();

    QString GetAreaName(SaveSlot* saveSlot);
    QString GetTime(SaveSlot* saveSlot);

private:
    void InitializeComboBox();

    void ReadRealtime(quint64 time);
    quint64 WriteRealtime();

    void ReadIGTime(quint32 time);
    quint32 WriteIGtime();

    void ReadPlaytime(qint32 playtime);
    qint32 WritePlaytime();

    Ui::SaveFileInfoWidget *ui;
};

#endif // SAVEFILEINFOWIDGET_H
