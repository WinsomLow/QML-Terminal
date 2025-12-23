#pragma once

#include <QObject>
#include <QString>

#include <atomic>
#include <thread>

class QSocketNotifier;

class TerminalBackend : public QObject {
  Q_OBJECT

public:
  explicit TerminalBackend(QObject *parent = nullptr);
  ~TerminalBackend() override;

  Q_INVOKABLE void start();
  Q_INVOKABLE void sendInput(const QString &text);
  Q_INVOKABLE void resize(int columns, int rows);

signals:
  void outputReceived(const QString &text);
  void started();
  void stopped(int exitCode);

private:
  void startProcess();
  void stopProcess();

#if defined(_WIN32)
  void startWindowsProcess();
  void stopWindowsProcess();
  void readWindowsOutput();

  void *pseudoConsole_ = nullptr;
  void *inputWrite_ = nullptr;
  void *outputRead_ = nullptr;
  void *processHandle_ = nullptr;
  void *threadHandle_ = nullptr;
  std::atomic<bool> running_{false};
  std::thread readerThread_;
#else
  void startPosixProcess();
  void stopPosixProcess();
  void readPosixOutput();

  int masterFd_ = -1;
  int childPid_ = -1;
  QSocketNotifier *notifier_ = nullptr;
  std::atomic<bool> running_{false};
#endif
};
