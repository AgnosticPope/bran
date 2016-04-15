#include <QGuiApplication>
#include <QtWebEngineWidgets>
#include <QCommandLineParser>
#include "requestinterceptor.h"
#include "cookiejar.h"
#include "mainwindow.h"
#include "downloadmanager.h"

int main(int argc, char **argv)
{
	QApplication *app = new QApplication(argc, argv);

	QCommandLineParser parser;
	parser.addOptions({
		{"allow-fonts",
		QCoreApplication::translate("main", "Allow loading WebFonts")},
		{"log-requests",
		QCoreApplication::translate("main", "Load HTTP requests to file"),
		QCoreApplication::translate("main", "log-requests")},
		{"block-hosts",
		QCoreApplication::translate("main", "Block hosts in the given file"),
		QCoreApplication::translate("main", "block-hosts")},
		{"cookies",
		QCoreApplication::translate("main", "Load cookies from file"),
		QCoreApplication::translate("main", "cookies")},
		{"save-cookies",
		QCoreApplication::translate("main", "Save cookies to file"),
		QCoreApplication::translate("main", "save-cookies")},
		{"no-images",
		QCoreApplication::translate("main", "Disable image loading")},
		{"no-javascript",
		QCoreApplication::translate("main", "Disable javascript")},
		{"user-agent",
		QCoreApplication::translate("main", "Use this user agent header"),
		QCoreApplication::translate("main", "useragent")},
		{"inspector-port",
		QCoreApplication::translate("main", "Enable the inspector at 127.0.0.1:port"),
		QCoreApplication::translate("main", "inspector-port")},
		{"zoom",
		QCoreApplication::translate("main", "Set default zoom factor [0.25: 5.0]"),
		QCoreApplication::translate("main", "zoom")},
		{"download",
		QCoreApplication::translate("main", "Download path"),
		QCoreApplication::translate("main", "download")},
	});
	parser.addHelpOption();
	parser.addPositionalArgument("url", "Open url", "[url]");
	parser.process(app->arguments());

	if (parser.isSet("inspector-port")) {
		qputenv("QTWEBENGINE_REMOTE_DEBUGGING",
				parser.value("inspector-port").toLocal8Bit());
	}

	RequestInterceptor *interceptor = new RequestInterceptor();
	interceptor->setAllowFonts(parser.isSet("allow-fonts"));
	if (parser.isSet("block-hosts")) {
		if (!interceptor->loadHostBlacklist(parser.value("block-hosts"))) {
			qWarning() << "Unable to open file" << parser.values("block-hosts");
			return -1;
		}
	}
	if (parser.isSet("log-requests")) {
		interceptor->setLogFile(parser.value("log-requests"));
	}

	// We don't want to use the default profile, create our own off
	// the record profile
	QWebEngineProfile *profile = new QWebEngineProfile();
	Q_ASSERT(profile->isOffTheRecord());
	profile->setRequestInterceptor(interceptor);

	QWebEngineSettings *settings = profile->settings();
	settings->setAttribute(QWebEngineSettings::AutoLoadImages, !parser.isSet("no-images"));
	settings->setAttribute(QWebEngineSettings::JavascriptEnabled, !parser.isSet("no-javascript"));
	settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
	settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, false);
	settings->setAttribute(QWebEngineSettings::XSSAuditingEnabled, true);
	settings->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, false);
	settings->setAttribute(QWebEngineSettings::PluginsEnabled, false);
	settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, false);

	CookieJar jar(profile->cookieStore());
	if (parser.isSet("cookies")) {
		if (!jar.loadFile(parser.value("cookies"))) {
			qWarning() << "Unable to load cookies from" << parser.values("cookies");
			return -1;
		}
	}

	DownloadManager *dman = new DownloadManager(profile, &jar);
	if (parser.isSet("download")) {
		dman->setDownloadPath(parser.value("download"));
	}

	Browser *browser = new Browser(profile);
	if (parser.isSet("zoom")) {
		bool ok = false;
		float factor = parser.value("zoom").toFloat(&ok);
		if (ok == false || 5.0 < factor || factor < 0.25) {
			qWarning() << "Invalid argument for --zoom";
			return -1;
		}
		browser->setDefaultZoom(factor);
	}

	MainWindow *window = new MainWindow(browser);
	window->setDownloadModel(dman);
	window->show();

	QObject::connect(window, &MainWindow::downloadActivated,
			dman, &DownloadManager::downloadActivated);

	if (!parser.positionalArguments().isEmpty()) {
		window->loadUrl(QUrl::fromUserInput(parser.positionalArguments().at(0)));
	}

	if (parser.isSet("user-agent")) {
		profile->setHttpUserAgent(parser.value("user-agent"));
	} else {
		profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.116 Safari/537.36");
	}

	int rc = app->exec();

	if (parser.isSet("save-cookies")) {
		if (!jar.saveAs(parser.value("save-cookies"))) {
			qWarning() << "Unable to open file" << parser.values("save-cookies");
			return -1;
		}
	}

	return rc;
}
