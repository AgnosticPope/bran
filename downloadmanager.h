#pragma once

#include <QObject>
#include <QWebEngineProfile>
#include <QDir>
#include <QUrl>
#include <QAbstractListModel>

class QWebEngineDownloadItem;
class DownloadManager: public QAbstractListModel
{
	Q_OBJECT
public:
	enum CustomRole {
		State = Qt::UserRole,
	};
	DownloadManager(QWebEngineProfile *profile, QObject *parent=0);
	bool setDownloadPath(const QString& path);
	virtual int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;
	virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const Q_DECL_OVERRIDE;

public slots:
	void downloadRequested(QWebEngineDownloadItem *download);
	void downloadActivated(const QModelIndex &index);
protected:
	QString path(const QString& suggested, const QString& mimetype) const;
private:
	QWebEngineProfile *m_profile;
	QString m_downloadPath;
	bool m_downloadsEnabled;
	QList<QWebEngineDownloadItem *> m_downloads;
};
