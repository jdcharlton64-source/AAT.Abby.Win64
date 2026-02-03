// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "src/abby/client_impl.h"

#include <algorithm>

#include "include/wrapper/cef_helpers.h"

#include "src/shared/client_util.h"
#include "src/shared/resource_util.h"
#include "src/shared/client_manager.h"

#include "constants.h"
#include "client_structs.h"
#include "raw_javascript.h"

#include <filesystem>
#include <vector>
#include <map>
#include <deque>
#include <nlohmann/json.hpp>
#include <fstream>
#include <dbaccess.h>

namespace abby {
    struct AppConfig {
      ContentState ambientContentState{};

      std::vector<std::string> kioskKeyTagIds{};
      std::vector<ResponseType> responseTypes{};
      std::vector<ResponseType> randomResponseTypes{};
      std::vector<std::map<int, int>> rotationalResponses{};
      std::vector<MediaPathType> mediaPathTypes{};
      std::vector<ConfigDetail> configDetails{};
      std::vector<InteractiveActivityType> interactiveActivityTypes{};
      std::vector<Resident> residents{};

      int kioskId{0};
      int kioskLanguageId{0};
      std::string kioskProfileLabel;
      std::string volumeAdjustLabel;
      std::string keyCardNotFoundLabel;
      CaregiverMediaSource communityMediaSource{};

      // std::string gadgetTypeCapsense;
      std::string gadgetTypeSwitch;
    };
namespace {

constexpr char kTestMessageName[] = "MessageRouterTest";

// Handle messages in the browser process.
class MessageHandler : public CefMessageRouterBrowserSide::Handler {
 public:
  explicit MessageHandler(const CefString& startup_url)
      : startup_url_(startup_url) {}
  MessageHandler(const MessageHandler&) = delete;
  MessageHandler& operator=(const MessageHandler&) = delete;

  // Called due to cefQuery execution in abby.html.
  bool OnQuery(CefRefPtr<CefBrowser> browser,
               CefRefPtr<CefFrame> frame,
               int64_t query_id,
               const CefString& request,
               bool persistent,
               CefRefPtr<Callback> callback) override {
    // Only handle messages from the startup URL.
    const std::string& url = frame->GetURL();
    if (url.find(startup_url_) != 0)
      return false;

    const std::string& message_name = request;
    if (message_name.find(kTestMessageName) == 0) {
      // Reverse the string and return.
      std::string result = message_name.substr(sizeof(kTestMessageName));
      std::reverse(result.begin(), result.end());
      callback->Success(result);
      return true;
    }

    return false;
  }

 private:
  const CefString startup_url_;
};

}  // namespace

Client::Client(const CefString& startup_url)
    : startup_url_(startup_url), browser_ct_(0) {}

void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title) {
  // Call the default shared implementation.
  shared::OnTitleChange(browser, title);
}

bool Client::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message) {
  CEF_REQUIRE_UI_THREAD();

  return message_router_->OnProcessMessageReceived(browser, frame,
                                                   source_process, message);
}

void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  if (!message_router_) {
    // Create the browser-side router for query handling.
    CefMessageRouterConfig config;
    message_router_ = CefMessageRouterBrowserSide::Create(config);

    // Register handlers with the router.
    message_handler_ = std::make_unique<MessageHandler>(startup_url_);
    message_router_->AddHandler(message_handler_.get(), false);
  }

  browser_ct_++;

  //std::string result = DataAccess::getKiosk();
  //json j = json::parse(result);
  //Kiosk kiosk = j.get<Kiosk>();

  // Call the default shared implementation.
  shared::OnAfterCreated(browser);
}

bool Client::DoClose(CefRefPtr<CefBrowser> browser) {
  // Call the default shared implementation.
  return shared::DoClose(browser);
}

void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  if (--browser_ct_ == 0) {
    // Free the router when the last browser is closed.
    message_router_->RemoveHandler(message_handler_.get());
    message_handler_.reset();
    message_router_ = nullptr;
  }

  // Call the default shared implementation.
  shared::OnBeforeClose(browser);
}

bool Client::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefRequest> request,
                            bool user_gesture,
                            bool is_redirect) {
  CEF_REQUIRE_UI_THREAD();

  message_router_->OnBeforeBrowse(browser, frame);
  return false;
}

CefRefPtr<CefResourceRequestHandler> Client::GetResourceRequestHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    bool is_navigation,
    bool is_download,
    const CefString& request_initiator,
    bool& disable_default_handling) {
  CEF_REQUIRE_IO_THREAD();
  return this;
}

void Client::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                       TerminationStatus status,
                                       int error_code,
                                       const CefString& error_string) {
  CEF_REQUIRE_UI_THREAD();

  message_router_->OnRenderProcessTerminated(browser);
}

CefRefPtr<CefResourceHandler> Client::GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) {
  CEF_REQUIRE_IO_THREAD();

  const std::string& url = request->GetURL();

  // This is a minimal implementation of resource loading. For more complex
  // usage (multiple files, zip archives, custom handlers, etc.) you might want
  // to use CefResourceManager. See the "resource_manager" target for an
  // example implementation.
  const auto resource_path = shared::GetResourcePath(url);
  if (resource_path)
    return shared::GetResourceHandler(*resource_path);

  return nullptr;
}

}  // namespace abby
