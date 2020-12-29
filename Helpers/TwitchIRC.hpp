// clang-format off
//
//    Copyright (C) 2020 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#pragma warning(push, 0)
#include <cstddef>  // size_t
#include <cstdint>  // integer types
#include <deque>
#include <functional>
#include <regex>
#include <string>
#include <thread>
// ASIO
#include <asio.hpp>
// fmt
#include "Helpers/fmt/format.h"
#pragma warning(pop)

namespace TwitchIRC {
  struct TwitchUser {
    std::uint64_t mId;
    bool          mIsSubscriber;

    TwitchUser() : mId(NULL), mIsSubscriber(false) {}
    TwitchUser(std::uint64_t id, bool isSubscriber) : mId(id), mIsSubscriber(isSubscriber) {}
  };
  struct TwitchMessage {
    TwitchUser  mSender;
    std::string mType;
    std::string mContent;

    TwitchMessage() = default;
    TwitchMessage(TwitchUser sender, const std::string& type, const std::string& content) :
        mSender(sender), mType(type), mContent(content) {}
  };

  class TwitchTalk {
    constexpr static char CONSTANT_hostname[] = "irc.chat.twitch.tv";
    constexpr static char CONSTANT_port[]     = "6667";  // SSL: 6697

    static inline const auto STATIC_regexMessage =
        std::regex(R"__(.*badges=.*?(subscriber\/\d+).*user-id=(\d+).*tmi\.twitch\.tv ([0-z]+).* :(.*))__");

    asio::io_context                      mASIO_ctx;
    asio::ip::tcp::resolver::results_type mASIO_endpoints;
    asio::ip::tcp::socket                 mASIO_socket;

    std::string                                mChannelName, mUsername, mUserOAuth;
    std::deque<std::string>                    mSendQueue;
    std::function<void(bool isConnected)>      mCallbackConnect;
    std::function<void(TwitchMessage message)> mCallbackReceivedMessage;
    std::string                                mLastReadMessage;

    void _closeSocket() {
      if (!mASIO_socket.is_open()) return;

      asio::post(mASIO_ctx, [this]() {
        mASIO_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        mASIO_socket.close();
      });
    }
    void _recv() {
      asio::async_read_until(
          mASIO_socket, asio::dynamic_buffer(mLastReadMessage, 4096), '\n',
          [this](std::error_code ec, std::size_t lenMsg) {
            if (ec) {
              _closeSocket();
              return;
            }

            if (mLastReadMessage._Starts_with("PING")) {
              _queueMessageToWrite("PONG :tmi.twitch.tv");
            } else {
              std::smatch _matches;
              if (std::regex_match(mLastReadMessage, _matches, STATIC_regexMessage)) {
                // user id not matched
                if (!_matches[2].matched) return;

                mCallbackReceivedMessage(
                    {{std::stoull(_matches[2].str()), _matches[1].matched}, _matches[3].str(), _matches[4].str()});
              }
            }
            mLastReadMessage.erase(0, lenMsg);
            _recv();
          });
    }
    void _send() {
      asio::async_write(mASIO_socket, asio::buffer(mSendQueue.front().data(), mSendQueue.front().length()),
                        [this](std::error_code ec, std::size_t) {
                          if (ec) {
                            _closeSocket();
                            return;
                          }

                          mSendQueue.pop_front();
                          if (!mSendQueue.empty()) _send();
                        });
    }
    void _queueMessageToWrite(const std::string& message) {
      asio::post(mASIO_ctx, [this, message]() {
        bool _isWriting = !mSendQueue.empty();
        mSendQueue.push_back(message);
        if (!_isWriting) _send();
      });
    }

    void _queueLoginMessages() {
      asio::post(mASIO_ctx, [this]() {
        bool _isWriting = !mSendQueue.empty();
        mSendQueue.insert(std::begin(mSendQueue), fmt::format("PASS {}\n", mUserOAuth));
        mSendQueue.insert(std::begin(mSendQueue) + 1, fmt::format("NICK {}\n", mUsername));
        mSendQueue.insert(std::begin(mSendQueue) + 2, fmt::format("JOIN #{}\n", mChannelName));
        mSendQueue.insert(std::begin(mSendQueue) + 3, "CAP REQ :twitch.tv/tags twitch.tv/commands\n");
        if (!_isWriting) _send();
      });
    }

    TwitchTalk()                  = delete;
    TwitchTalk(const TwitchTalk&) = delete;
    TwitchTalk(TwitchTalk&&)      = delete;

   public:
    void SendMessageToChannel(const std::string& message) {
      return _queueMessageToWrite(fmt::format("PRIVMSG #{} :{}\n", mChannelName, message));
    }

    // Spawns a thread
    void StartTalk(const std::string& channelName, const std::string& username, const std::string& userOAuth) {
      mChannelName = channelName;
      mUsername    = username;
      mUserOAuth   = userOAuth;

      asio::async_connect(mASIO_socket, mASIO_endpoints, [this](std::error_code ec, asio::ip::tcp::endpoint) {
        if (ec) {
          mCallbackConnect(false);
          return;
        }

        _queueLoginMessages();
        asio::async_read_until(mASIO_socket, asio::dynamic_buffer(mLastReadMessage, 1024), ":Welcome, GLHF!",
                               [this](std::error_code ec, std::size_t lenMsg) {
                                 if (ec) {
                                   _closeSocket();
                                   mCallbackConnect(false);
                                   return;
                                 }
                                 mCallbackConnect(true);
                                 mLastReadMessage.erase(0, lenMsg);
                                 _recv();
                               });
      });
      std::thread([this]() { mASIO_ctx.run(); }).detach();
    }
    void StopTalk() {
      if (mASIO_socket.is_open()) {
        _queueMessageToWrite(fmt::format("PART #{}\n", mChannelName));
        _closeSocket();
      }
    }

    TwitchTalk(const std::function<void(bool isConnected)>&      callbackConnect,
               const std::function<void(TwitchMessage message)>& callbackReceivedMessage) :
        mCallbackConnect(callbackConnect), mCallbackReceivedMessage(callbackReceivedMessage), mASIO_socket(mASIO_ctx) {
      asio::ip::tcp::resolver resolver(mASIO_ctx);
      mASIO_endpoints = resolver.resolve(CONSTANT_hostname, CONSTANT_port);
    }
    ~TwitchTalk() {
      _closeSocket();
      asio::post(mASIO_ctx, [this]() { mASIO_ctx.stop(); });
    }
  };
}  // namespace TwitchIRC
