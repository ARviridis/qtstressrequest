#include "replytimeout.h"

ReplyTimeout::ReplyTimeout()
{
    static void set(QNetworkReply* reply, const int timeout, HandleMethod method = Abort)
    {
      new ReplyTimeout(reply, timeout, method);
    }

    void timerEvent(QTimerEvent * ev)
    {
      if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
        return;
      auto reply = static_cast<QNetworkReply*>(parent());
      if (reply->isRunning()) {
        if (m_method == Close)
          reply->close();
        else if (m_method == Abort)
          reply->abort();
      m_timer.stop();
    }
  };
}
