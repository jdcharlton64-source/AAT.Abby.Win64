#include "raw_javascript.h"
#include <string>

namespace raw_javascript {
std::string loadingMessage() {
  return R"(
      if (!document.getElementById('loading-message-div')) {
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='loading-modal' class='loading-modal'>
            <span class='loading-message-text'></span>
          </dialog>
          <style>
            .loading-modal {
              border: none;
              outline: none;
              border-radius: 5px;
              background-color: rgba(228, 227, 227, 0.5);
              padding: 10px;
              margin: 0;          
            }
            .loading-message-text {
              font-size: 30px;
              font-family: "Helvetica", sans-serif;
              color: #333;
            }
            .loading-message {
              position: fixed;
              background-color: transparent;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);
              margin: 0;
            }
          </style>
        `);
        }
      document.getElementById('loading-modal').showModal();
      window.cefQuery({ request: 'LoadingDialogLoaded:'});
      )";
}

std::string residentPopup() {
  return R"(
      if (!document.getElementById('resident-popup-modal')) {
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='resident-popup-modal' class='resident-popup-dialog'>
            <span id='name' class='text-base resident-name'></span>
            <span id='keynum' class='text-base resident-keynum'></span>
            <img id='picture' class='resident-picture' src='' alt=''></img>
          </dialog>
          <style>
            .text-base {
              font-family: "Helvetica", sans-serif;
              display: block;
            }
            .resident-name {
              font-size: 20px;
            }
            .resident-keynum {
              font-size: 14px;
              margin-bottom: 5px;
            }
            .resident-picture {
              border-radius: 50%;
              width: 40px;
              height: 40px;
            }
            .resident-popup-dialog {
              position: fixed;
              top: auto;
              left: auto;
              bottom: 0;
              right: 0;
              border: none;
              outline: none;
              border-radius: 5px;
              background-color: rgba(228, 227, 227, 0.5);
              padding: 10px;
              margin: 0;
            }
          </style>
        `);
        var _popupTimeoutHandle=null;
        function stopPopupTimeout() {
          clearTimeout(_popupTimeoutHandle);
        }
        function initPopupTimeout() {
          _popupTimeoutHandle = setTimeout(() => {
            stopPopupTimeout();
            document.getElementById('resident-popup-modal').close();
          }, 2000);
        }  
      }
      document.getElementById('resident-popup-modal').showModal();
      window.cefQuery({ request: 'ResidentPopupLoaded:'});
      initPopupTimeout();
    )";
}

std::string invalidKeyCardPopup() {
  return R"(
      if (!document.getElementById('invalid-keycard-popup-modal')) {
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='invalid-keycard-popup-modal' class='invalid-keycard-popup-dialog'>
            <span id='message' class='message-text'></span>
          </dialog>
          <style>
            .message-text {
              font-size: 20px;
              font-family: "Helvetica", sans-serif;
              display: block;
            }
            .invalid-keycard-popup-dialog {
              position: fixed;
              top: auto;
              left: auto;
              bottom: 0;
              right: 0;
              border: none;
              outline: none;
              border-radius: 5px;
              background-color: rgba(228, 227, 227, 0.5);
              padding: 10px;
              margin: 0;
            }
          </style>
        `);
        var _popupTimeoutHandle=null;
        function stopPopupTimeout() {
          clearTimeout(_popupTimeoutHandle);
        }
        function initPopupTimeout() {
          _popupTimeoutHandle = setTimeout(() => {
            stopPopupTimeout();
            document.getElementById('invalid-keycard-popup-modal').close();
          }, 2000);
        }  
      }
      document.getElementById('invalid-keycard-popup-modal').showModal();
      window.cefQuery({ request: 'InvalidKeyCardPopupLoaded:'});
      initPopupTimeout();
    )";
}

std::string caregiverCloseButton(CaregiverCloseButtonType buttonType) {
  switch (buttonType) {
    case CaregiverMedia:
      return R"(
          if (!document.getElementById('close-button-div')) {
            document.body.insertAdjacentHTML('beforeend', `
              <div id='close-button-div'>
                <a href='#' onclick='raiseCaregiverEvent(EVENT_ID_CAREGIVER_MEDIA_CLOSED);' class='close-button'>
                  <img width='60px 'height='60px' src='../../images/close.png' alt=''/>
                </a>
              </div>
              <style>
                .close-button {
                  position: absolute;
                  top: 5px;
                  right: 5px;
                }
              </style>
            `);
            }
          )";
    case CaregiverActivity:
      return R"(
            if (!document.getElementById('close-button-div')) {
              document.body.insertAdjacentHTML('beforeend', `
                <div id='close-button-div'>
                  <a href='#' onclick='raiseCaregiverEvent(EVENT_ID_CAREGIVER_ACTIVITY_CLOSED);' class='close-button'>
                    <img width='60px 'height='60px' src='../../images/close.png' alt=''/>
                  </a>
                </div>
                <style>
                  .close-button {
                    position: absolute;
                    top: 5px;
                    right: 5px;
                  }
                </style>
              `);
              }
            )";
    default:
      return "";
  }
}

std::string volumeDialog() {
  return R"(
      if (!document.getElementById('volume-modal')) {
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='volume-modal' class='volume-dialog'>
            <a href='#' onclick=\"stopVolumeTimeout();closeDialog();\" class='close-button'>
              <img width='30px 'height='30px' src='../../images/close_red.png'/>
            </a>
            <div class='header-container'>
              <h1 id='header-label'></h1>
            </div>
            <div class='progress-container'>
              <div class='progress-bar' style='width: 50%;'></div>
              <span class='progress-label'>50%</span>
            </div>
            <form method='dialog'>
              <a href="#">
                <div class='button'>
                <img width='40px' height='40px' src='../../images/volume_up.png' onclick='volumeUp();' class='button-image' />
                </div>
              </a>
              <a href="#">
                <div class='button'>
                <img width='40px' height='40px' src='../../images/volume_down.png' onclick='volumeDown();' class='button-image' />
                </div>
              </a>
            </form>
          </dialog>
          <style>
            .volume-dialog {
              position: fixed;
              background-color: #b2d3f8ff;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);
              border: 1px;
              outline: none;
              margin: 0;
              padding: 20px;
              border-radius: 5px;
            }
            .dialog-content {
              position: relative;
            }
            .button-image {
              display: block;
              margin-left: auto;
              margin-right: auto;
              z-index: 1;
            }
            .button {
              background-color: #2194ffff;
              padding: 2px 6px 2px 6px;
              border-top: 1px solid #CCCCCC;
              border-right: 1px solid #333333;
              border-bottom: 1px solid #333333;
              border-left: 1px solid #CCCCCC;
            }
            .close-button {
              position: absolute;
              top: 5px;
              right: 5px;
            }
            .header-container {
              width: 100%;
              background-color: #e0e0e0;
              font-size: 14px;
              font-family: "Helvetica", sans-serif;
              margin-top: 20px;
              margin-bottom: 20px
            }
            .progress-container {
              width: 100%;
              height: 20px;
              background-color: #e0e0e0;
              border-radius: 5px;
              position: relative;
              margin-bottom: 20px;
            }
            progress {
              width: 100%;
              height: 40px;
              background-color: #ffffff;
              border-radius: 5px;
            }
            .progress-bar {
              height: 100%;
              background-color: hsl(110, 100%, 50%);
              border-radius: 5px;
              transition: width 0.4s ease;
            }
            .progress-label {
              position: absolute;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);
              color: #333333;
              font-weight: bold;
              font-size: 14px;
              font-family: Arial;
            }
          </style>
        `);
        var _volumeTimeoutHandle=null;
        function closeDialog() {
          window.cefQuery({ request: 'VolumeDialogUnloaded:'});
          document.getElementById('volume-modal').close();
        }
        function stopVolumeTimeout() {
          clearTimeout(_volumeTimeoutHandle);
        }
        function initVolumeTimeout() {
          _volumeTimeoutHandle = setTimeout(() => {
            stopVolumeTimeout();
            closeDialog();
          }, 6000);
        }
        function resetVolumeTimeout() {
          stopVolumeTimeout();
          initVolumeTimeout();
        }
        function updateValue(volume) {
          document.querySelector('.progress-bar').style.width = volume + '%';
          document.querySelector('.progress-label').textContent = volume + '%';
          resetVolumeTimeout();
        }
        function initVolume(volume, headerLabel) {
          document.getElementById('header-label').innerHTML = headerLabel;
          updateValue(volume);
        }
        function volumeUp() {
          window.cefQuery({ request: 'VolumeAdjust:1'});
        }
        function volumeDown() {
          window.cefQuery({ request: 'VolumeAdjust:-1'});
        }
      }
      document.getElementById('volume-modal').showModal();
      window.cefQuery({ request: 'VolumeDialogLoaded:'});
      initVolumeTimeout();
    )";
}

std::string alertAuto() {
  return R"(
      if (!document.getElementById('alert-modal')) {
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='alert-modal' class='alert-modal'>
            <div class='text-container'>
              <span id='alert-text'></span>
            </div>
          </dialog>
          <style>
            .alert-modal {
              position: fixed;
              background-color: rgba(228, 227, 227, 0.6);
              margin: 0;
              padding: 20px;
              border-radius: 5px;
              border: none;
              outline: none;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);
            }
            .text-container {
              font-size: 20px;
              font-family: "Helvetica", sans-serif;
              background-color: transparent;
            }
          </style>
        `);
        var _alertTimeoutHandle=null;
        function stopAlertTimeout() {
          clearTimeout(_alertTimeoutHandle);
        }
        function initAlertTimeout() {
          _alertTimeoutHandle = setTimeout(() => {
            stopAlertTimeout();
            document.getElementById('alert-modal').close();
          }, 1500);
        }
      }
      document.getElementById('alert-modal').showModal();
      window.cefQuery({ request: 'AlertLoaded:'});
      initAlertTimeout();
    )";
}

std::string alertClosable() {
  return R"(
      if (!document.getElementById('alert-modal')) {
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='alert-modal' class='alert-modal'>
            <a href='#' onclick=\"document.getElementById('alert-modal').close();\" class='close-button'>
              <img width='30px 'height='30px' src='../../images/close_red.png'/>
            </a>
            <div class='text-container'>
              <span id='alert-text'></span>
            </div>
          </dialog>
          <style>
            .alert-modal {
              position: fixed;
              background-color: rgba(228, 227, 227, 0.6);
              padding: 20px;
              border-radius: 5px;
              border: none;
              outline: none;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);
            }
            .text-container {
              margin-top: 20px;
              font-size: 20px;
              font-family: "Helvetica", sans-serif;
              background-color: transparent;
            }
            .close-button {
              position: absolute;
              top: 5px;
              right: 5px;
              margin-bottom: 10px;
            }
          </style>
        `);
      }
      document.getElementById('alert-modal').showModal();
      window.cefQuery({ request: 'AlertLoaded:'});
    )";
}

std::string alert(bool autoClose) {
  return autoClose ? alertAuto() : alertClosable();
}

std::string noSpeakerAttached() {
  return R"(
      if (!document.getElementById('no-speaker-modal')) {
       if (document.getElementById('volume-modal')) {
          document.getElementById('volume-modal').close();
        }
        document.body.insertAdjacentHTML('beforeend', `
          <dialog id='no-speaker-modal' class='no-speaker-modal'>
            <div class='image-container'>
              <img width='150px 'height='150px' src='../../images/no-speaker.png' alt=''/>
            </div>
          </dialog>
          <style>
            .no-speaker-modal {
              position: fixed;
              background-color: rgba(228, 227, 227, 0.6);
              margin: 0;
              padding: 20px;
              border-radius: 5px;
              border: none;
              outline: none;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);
            }
            .image-container {
              background-color: transparent;
            }
          </style>
        `);
        var _noSpeakerTimeoutHandle=null;
        function stopNoSpeakerTimeout() {
          clearTimeout(_noSpeakerTimeoutHandle);
        }
        function initNoSpeakerTimeout() {
          _NoSpeakerTimeoutHandle = setTimeout(() => {
            stopNoSpeakerTimeout();
            document.getElementById('no-speaker-modal').close();
          }, 1500);
        }
      }
      document.getElementById('no-speaker-modal').showModal();
      //window.cefQuery({ request: 'NoSpeakerLoaded:'});
      initNoSpeakerTimeout();
    )";
}
}  // namespace raw_javascript