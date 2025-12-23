#include "terminal_backend.h"

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QMetaObject>

#include <atomic>
#include <thread>

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#define NOMINMAX
#include <windows.h>

#include <vector>
#else
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined(__APPLE__)
#include <util.h>
#else
#include <pty.h>
#endif
#endif

TerminalBackend::TerminalBackend(QObject *parent) : QObject(parent) {}

TerminalBackend::~TerminalBackend() {
  stopProcess();
}

void TerminalBackend::start() {
  startProcess();
}

void TerminalBackend::sendInput(const QString &text) {
#if defined(_WIN32)
  if (!running_ || !inputWrite_) {
    return;
  }
  const QByteArray data = text.toUtf8();
  DWORD written = 0;
  WriteFile(static_cast<HANDLE>(inputWrite_), data.constData(),
            static_cast<DWORD>(data.size()), &written, nullptr);
#else
  if (!running_ || masterFd_ < 0) {
    return;
  }
  const QByteArray data = text.toUtf8();
  (void)::write(masterFd_, data.constData(),
                static_cast<size_t>(data.size()));
#endif
}

void TerminalBackend::resize(int columns, int rows) {
#if defined(_WIN32)
  if (!running_ || !pseudoConsole_) {
    return;
  }
  COORD size;
  size.X = static_cast<SHORT>(columns);
  size.Y = static_cast<SHORT>(rows);
  ResizePseudoConsole(static_cast<HPCON>(pseudoConsole_), size);
#else
  if (!running_ || masterFd_ < 0) {
    return;
  }
  struct winsize ws;
  ws.ws_col = static_cast<unsigned short>(columns);
  ws.ws_row = static_cast<unsigned short>(rows);
  ws.ws_xpixel = 0;
  ws.ws_ypixel = 0;
  ioctl(masterFd_, TIOCSWINSZ, &ws);
#endif
}

void TerminalBackend::startProcess() {
  if (running_) {
    return;
  }
#if defined(_WIN32)
  startWindowsProcess();
#else
  startPosixProcess();
#endif
}

void TerminalBackend::stopProcess() {
#if defined(_WIN32)
  stopWindowsProcess();
#else
  stopPosixProcess();
#endif
}

#if defined(_WIN32)
void TerminalBackend::startWindowsProcess() {
  HANDLE conIn = nullptr;
  HANDLE conOut = nullptr;
  HANDLE inputWrite = nullptr;
  HANDLE outputRead = nullptr;

  SECURITY_ATTRIBUTES sa = {};
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = nullptr;

  if (!CreatePipe(&conIn, &inputWrite, &sa, 0)) {
    return;
  }
  if (!CreatePipe(&outputRead, &conOut, &sa, 0)) {
    CloseHandle(conIn);
    CloseHandle(inputWrite);
    return;
  }

  SetHandleInformation(inputWrite, HANDLE_FLAG_INHERIT, 0);
  SetHandleInformation(outputRead, HANDLE_FLAG_INHERIT, 0);

  COORD size = {120, 30};
  HPCON hPC = nullptr;
  if (FAILED(CreatePseudoConsole(size, conIn, conOut, 0, &hPC))) {
    CloseHandle(conIn);
    CloseHandle(conOut);
    CloseHandle(inputWrite);
    CloseHandle(outputRead);
    return;
  }

  CloseHandle(conIn);
  CloseHandle(conOut);

  SIZE_T attrListSize = 0;
  InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
  auto *attrList = reinterpret_cast<PPROC_THREAD_ATTRIBUTE_LIST>(
      HeapAlloc(GetProcessHeap(), 0, attrListSize));
  if (!attrList) {
    ClosePseudoConsole(hPC);
    CloseHandle(inputWrite);
    CloseHandle(outputRead);
    return;
  }
  if (!InitializeProcThreadAttributeList(attrList, 1, 0, &attrListSize)) {
    HeapFree(GetProcessHeap(), 0, attrList);
    ClosePseudoConsole(hPC);
    CloseHandle(inputWrite);
    CloseHandle(outputRead);
    return;
  }
  if (!UpdateProcThreadAttribute(attrList, 0,
                                 PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, hPC,
                                 sizeof(hPC), nullptr, nullptr)) {
    DeleteProcThreadAttributeList(attrList);
    HeapFree(GetProcessHeap(), 0, attrList);
    ClosePseudoConsole(hPC);
    CloseHandle(inputWrite);
    CloseHandle(outputRead);
    return;
  }

  STARTUPINFOEXW si = {};
  si.StartupInfo.cb = sizeof(STARTUPINFOEXW);
  si.lpAttributeList = attrList;

  PROCESS_INFORMATION pi = {};
  std::wstring command = L"cmd.exe /k chcp 65001 >nul";
  std::vector<wchar_t> cmdLine(command.begin(), command.end());
  cmdLine.push_back(L'\0');

  BOOL created = CreateProcessW(
      nullptr, cmdLine.data(), nullptr, nullptr, FALSE,
      EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
      &si.StartupInfo, &pi);

  DeleteProcThreadAttributeList(attrList);
  HeapFree(GetProcessHeap(), 0, attrList);

  if (!created) {
    ClosePseudoConsole(hPC);
    CloseHandle(inputWrite);
    CloseHandle(outputRead);
    return;
  }

  pseudoConsole_ = hPC;
  inputWrite_ = inputWrite;
  outputRead_ = outputRead;
  processHandle_ = pi.hProcess;
  threadHandle_ = pi.hThread;
  running_ = true;
  readerThread_ = std::thread([this]() { readWindowsOutput(); });
  emit started();
}

void TerminalBackend::stopWindowsProcess() {
  if (!running_) {
    return;
  }
  running_ = false;

  if (outputRead_) {
    CloseHandle(static_cast<HANDLE>(outputRead_));
  }
  if (readerThread_.joinable()) {
    readerThread_.join();
  }

  if (processHandle_) {
    TerminateProcess(static_cast<HANDLE>(processHandle_), 0);
  }
  if (threadHandle_) {
    CloseHandle(static_cast<HANDLE>(threadHandle_));
  }
  if (processHandle_) {
    CloseHandle(static_cast<HANDLE>(processHandle_));
  }
  if (inputWrite_) {
    CloseHandle(static_cast<HANDLE>(inputWrite_));
  }
  if (pseudoConsole_) {
    ClosePseudoConsole(static_cast<HPCON>(pseudoConsole_));
  }

  inputWrite_ = nullptr;
  outputRead_ = nullptr;
  pseudoConsole_ = nullptr;
  processHandle_ = nullptr;
  threadHandle_ = nullptr;
  emit stopped(0);
}

void TerminalBackend::readWindowsOutput() {
  HANDLE outHandle = static_cast<HANDLE>(outputRead_);
  if (!outHandle) {
    return;
  }

  char buffer[4096];
  while (running_) {
    DWORD read = 0;
    if (!ReadFile(outHandle, buffer, sizeof(buffer), &read, nullptr)) {
      break;
    }
    if (read > 0) {
      const QString text = QString::fromUtf8(buffer, read);
      QMetaObject::invokeMethod(
          this, [this, text]() { emit outputReceived(text); },
          Qt::QueuedConnection);
    }
  }
}
#else
void TerminalBackend::startPosixProcess() {
  int masterFd = -1;
  pid_t pid = forkpty(&masterFd, nullptr, nullptr, nullptr);
  if (pid < 0) {
    return;
  }

  if (pid == 0) {
    const char *shell = getenv("SHELL");
    if (!shell) {
      shell = "/bin/bash";
    }
    execlp(shell, shell, "-i", nullptr);
    _exit(1);
  }

  int flags = fcntl(masterFd, F_GETFL, 0);
  if (flags != -1) {
    fcntl(masterFd, F_SETFL, flags | O_NONBLOCK);
  }

  masterFd_ = masterFd;
  childPid_ = static_cast<int>(pid);
  notifier_ = new QSocketNotifier(masterFd_, QSocketNotifier::Read, this);
  connect(notifier_, &QSocketNotifier::activated, this,
          &TerminalBackend::readPosixOutput);
  running_ = true;
  emit started();
}

void TerminalBackend::stopPosixProcess() {
  if (!running_) {
    return;
  }
  running_ = false;

  if (notifier_) {
    notifier_->setEnabled(false);
    notifier_->deleteLater();
    notifier_ = nullptr;
  }

  if (childPid_ > 0) {
    kill(childPid_, SIGTERM);
    int status = 0;
    waitpid(childPid_, &status, WNOHANG);
  }

  if (masterFd_ >= 0) {
    close(masterFd_);
  }

  masterFd_ = -1;
  childPid_ = -1;
  emit stopped(0);
}

void TerminalBackend::readPosixOutput() {
  if (masterFd_ < 0) {
    return;
  }
  char buffer[4096];
  ssize_t count = ::read(masterFd_, buffer, sizeof(buffer));
  if (count > 0) {
    emit outputReceived(QString::fromUtf8(buffer, count));
    return;
  }
  if (count == 0) {
    stopPosixProcess();
    return;
  }
  if (errno == EAGAIN || errno == EWOULDBLOCK) {
    return;
  }
  stopPosixProcess();
}
#endif
