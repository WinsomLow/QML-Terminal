#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "terminal_backend.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  TerminalBackend backend;

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("terminalBackend", &backend);
  const QUrl url(QStringLiteral("qrc:/QmlTerminal/qml/Main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated,
      &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
          QCoreApplication::exit(-1);
        }
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
