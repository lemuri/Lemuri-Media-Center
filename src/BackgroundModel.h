#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H

#include <QStringListModel>

class BackgroundModel : public QStringListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit BackgroundModel(QObject *parent);

    int count() const;
    Q_INVOKABLE QString get(int row) const;

signals:
    void countChanged();
};

#endif // BACKGROUNDMODEL_H
