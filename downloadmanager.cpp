#include "downloadmanager.h"
#include <QWebEngineDownloadItem> 
#include <QDebug>
#include <QUrl>
#include <QIcon>
#include <QTimer>
#include <QMimeDatabase>
#include "page.h"

DownloadManager::DownloadManager(QWebEngineProfile *profile, QObject *parent)
:QAbstractListModel(parent), m_profile(profile), m_downloadsEnabled(false)
{
	connect(m_profile, &QWebEngineProfile::downloadRequested,
			this, &DownloadManager::downloadRequested);
}

void DownloadManager::downloadRequested(QWebEngineDownloadItem *download)
{
	if (!m_downloadPath.isEmpty()) {
		download->setPath(path(download->path(), download->mimeType()));
	}
	download->accept();

	beginInsertRows(QModelIndex(), 0, 0);
	m_downloads.insert(0, download);
	endInsertRows();
}

bool DownloadManager::setDownloadPath(const QString& path)
{
	QFileInfo fi(path);
	QDir().mkpath(path);
	m_downloadPath = fi.absoluteFilePath();
	return fi.exists() &&
		fi.isWritable();
}

int DownloadManager::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return 0;
	}
	return m_downloads.size();
}

QVariant DownloadManager::data(const QModelIndex &index, int role) const
{
	if (index.row() < 0 || m_downloads.size() < index.row()) {
		return QVariant();
	}
	if (role == Qt::DisplayRole) {
		if (m_downloads.at(index.row())->state() == QWebEngineDownloadItem::DownloadCompleted) {
			return m_downloads.at(index.row())->path();
		} else {
			return m_downloads.at(index.row())->url();
		}
	} else if (role == Qt::ToolTipRole) {
		if (m_downloads.at(index.row())->state() == QWebEngineDownloadItem::DownloadCompleted) {
			return m_downloads.at(index.row())->url();
		} else {
			return m_downloads.at(index.row())->path();
		}
	} else if (role == Qt::DecorationRole) {
		switch(m_downloads.at(index.row())->state()) {
		case QWebEngineDownloadItem::DownloadInProgress:
			return QIcon(":/icons/go-down.png");
		case QWebEngineDownloadItem::DownloadCancelled:
		case QWebEngineDownloadItem::DownloadInterrupted:
			return QIcon(":/icons/dialog-close.png");
		case QWebEngineDownloadItem::DownloadCompleted:
			return QIcon(":/icons/dialog-ok-apply.png");
		default:
			return QIcon(":/icons/download-later.png");
		}
	} else {
		return QVariant();
	}
}

void DownloadManager::downloadActivated(const QModelIndex &index)
{
	if (index.row() < 0 || m_downloads.size() < index.row()) {
		return;
	}
//	emit dataChanged(index, index);
}

QString DownloadManager::path(const QString& suggested, const QString& mimetype) const
{
	QFileInfo fi = QFileInfo(suggested).baseName();
	QString baseName = fi.baseName();
	QString suffix = fi.completeSuffix();
	QDir dir = QDir(m_downloadPath);

	QString path = dir.absoluteFilePath(fi.fileName());
	if (suffix.isEmpty()) {
		suffix = QMimeDatabase().mimeTypeForName(mimetype).preferredSuffix();
		if (!suffix.isEmpty()) {
			path += "." + suffix;
		}
	}

	for (int i=1; QFileInfo::exists(path); ++i) {
		path = QString("%1(%2).%3").arg(baseName).arg(i).arg(suffix);
		if (i >= 99) {
			path = dir.absoluteFilePath(fi.fileName());
			break;
		}
	}
	return path;
}

