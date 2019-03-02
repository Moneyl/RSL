#pragma once
//#include "KeenNamespace.h"

enum EResult
{
	k_EResultOK = 0x1,
	k_EResultFail = 0x2,
	k_EResultNoConnection = 0x3,
	k_EResultInvalidPassword = 0x5,
	k_EResultLoggedInElsewhere = 0x6,
	k_EResultInvalidProtocolVer = 0x7,
	k_EResultInvalidParam = 0x8,
	k_EResultFileNotFound = 0x9,
	k_EResultBusy = 0xA,
	k_EResultInvalidState = 0xB,
	k_EResultInvalidName = 0xC,
	k_EResultInvalidEmail = 0xD,
	k_EResultDuplicateName = 0xE,
	k_EResultAccessDenied = 0xF,
	k_EResultTimeout = 0x10,
	k_EResultBanned = 0x11,
	k_EResultAccountNotFound = 0x12,
	k_EResultInvalidSteamID = 0x13,
	k_EResultServiceUnavailable = 0x14,
	k_EResultNotLoggedOn = 0x15,
	k_EResultPending = 0x16,
	k_EResultEncryptionFailure = 0x17,
	k_EResultInsufficientPrivilege = 0x18,
	k_EResultLimitExceeded = 0x19,
	k_EResultRevoked = 0x1A,
	k_EResultExpired = 0x1B,
	k_EResultAlreadyRedeemed = 0x1C,
	k_EResultDuplicateRequest = 0x1D,
	k_EResultAlreadyOwned = 0x1E,
	k_EResultIPNotFound = 0x1F,
	k_EResultPersistFailed = 0x20,
	k_EResultLockingFailed = 0x21,
	k_EResultLogonSessionReplaced = 0x22,
	k_EResultConnectFailed = 0x23,
	k_EResultHandshakeFailed = 0x24,
	k_EResultIOFailure = 0x25,
	k_EResultRemoteDisconnect = 0x26,
	k_EResultShoppingCartNotFound = 0x27,
	k_EResultBlocked = 0x28,
	k_EResultIgnored = 0x29,
	k_EResultNoMatch = 0x2A,
	k_EResultAccountDisabled = 0x2B,
	k_EResultServiceReadOnly = 0x2C,
	k_EResultAccountNotFeatured = 0x2D,
	k_EResultAdministratorOK = 0x2E,
	k_EResultContentVersion = 0x2F,
	k_EResultTryAnotherCM = 0x30,
	k_EResultPasswordRequiredToKickSession = 0x31,
	k_EResultAlreadyLoggedInElsewhere = 0x32,
	k_EResultSuspended = 0x33,
	k_EResultCancelled = 0x34,
	k_EResultDataCorruption = 0x35,
	k_EResultDiskFull = 0x36,
	k_EResultRemoteCallFailed = 0x37,
	k_EResultPasswordUnset = 0x38,
	k_EResultExternalAccountUnlinked = 0x39,
	k_EResultPSNTicketInvalid = 0x3A,
	k_EResultExternalAccountAlreadyLinked = 0x3B,
	k_EResultRemoteFileConflict = 0x3C,
	k_EResultIllegalPassword = 0x3D,
	k_EResultSameAsPreviousValue = 0x3E,
	k_EResultAccountLogonDenied = 0x3F,
	k_EResultCannotUseOldPassword = 0x40,
	k_EResultInvalidLoginAuthCode = 0x41,
	k_EResultAccountLogonDeniedNoMail = 0x42,
	k_EResultHardwareNotCapableOfIPT = 0x43,
	k_EResultIPTInitError = 0x44,
	k_EResultParentalControlRestricted = 0x45,
	k_EResultFacebookQueryError = 0x46,
	k_EResultExpiredLoginAuthCode = 0x47,
	k_EResultIPLoginRestrictionFailed = 0x48,
	k_EResultAccountLockedDown = 0x49,
	k_EResultAccountLogonDeniedVerifiedEmailRequired = 0x4A,
	k_EResultNoMatchingURL = 0x4B,
	k_EResultBadResponse = 0x4C,
	k_EResultRequirePasswordReEntry = 0x4D,
	k_EResultValueOutOfRange = 0x4E,
	k_EResultUnexpectedError = 0x4F,
	k_EResultDisabled = 0x50,
	k_EResultInvalidCEGSubmission = 0x51,
	k_EResultRestrictedDevice = 0x52,
	k_EResultRegionLocked = 0x53,
	k_EResultRateLimitExceeded = 0x54,
	k_EResultAccountLoginDeniedNeedTwoFactor = 0x55,
	k_EResultItemDeleted = 0x56,
	k_EResultAccountLoginDeniedThrottle = 0x57,
	k_EResultTwoFactorCodeMismatch = 0x58,
	k_EResultTwoFactorActivationCodeMismatch = 0x59,
	k_EResultAccountAssociatedToMultiplePartners = 0x5A,
	k_EResultNotModified = 0x5B,
	k_EResultNoMobileDevice = 0x5C,
	k_EResultTimeNotSynced = 0x5D,
	k_EResultSmsCodeFailed = 0x5E,
	k_EResultAccountLimitExceeded = 0x5F,
	k_EResultAccountActivityLimitExceeded = 0x60,
	k_EResultPhoneActivityLimitExceeded = 0x61,
	k_EResultRefundToWallet = 0x62,
	k_EResultEmailSendFailure = 0x63,
	k_EResultNotSettled = 0x64,
	k_EResultNeedCaptcha = 0x65,
	k_EResultGSLTDenied = 0x66,
	k_EResultGSOwnerDenied = 0x67,
	k_EResultInvalidItemType = 0x68,
	k_EResultIPBanned = 0x69,
	k_EResultGSLTExpired = 0x6A,
	k_EResultInsufficientFunds = 0x6B,
	k_EResultTooManyPending = 0x6C,
	k_EResultNoSiteLicensesFound = 0x6D,
	k_EResultWGNetworkSendExceeded = 0x6E,
	k_EResultAccountNotFriends = 0x6F,
	k_EResultLimitedUserAccount = 0x70,
};

struct IUnknownVtbl;
struct IUnknown
{
	IUnknownVtbl *lpVtbl;
};

typedef int HRESULT;
struct IUnknownVtbl
{
	HRESULT(__stdcall* QueryInterface)(IUnknown*, _GUID* const, void**);
	unsigned int(__stdcall* AddRef)(IUnknown*);
	unsigned int(__stdcall* Release)(IUnknown*);
};

struct IDispatch : IUnknown
{

};

struct INetworkListManager : IDispatch
{

};

/* 2657 */
struct CCallbackBase
{
  CCallbackBaseVtbl *vfptr;
  char m_nCallbackFlags;
  int m_iCallback;
};

/* 2658 */
struct CCallbackBaseVtbl
{
  void (__thiscall *Run)(CCallbackBase* This, void*, bool, unsigned __int64);
  void (__thiscall *Run)(CCallbackBase* This, void*);
  int (__thiscall *GetCallbackSizeBytes)(CCallbackBase* This);
};

/* 2659 */
struct CCallbackImpl__24 : CCallbackBase
{

};

/* 2661 */
struct __declspec(align(8)) LobbyDataUpdate_t
{
  unsigned __int64 m_ulSteamIDLobby;
  unsigned __int64 m_ulSteamIDMember;
  char m_bSuccess;
};

/* 2664 */
struct XAUDIO2_VOICE_DETAILS
{
  unsigned int CreationFlags;
  unsigned int InputChannels;
  unsigned int InputSampleRate;
};

/* 2670 */
struct CCallbackImpl__16 : CCallbackBase
{

};

/* 2739 */
struct CCallbackImpl__32 : CCallbackBase
{
	
};

	/* 2729 */
struct LobbyInvite_t
{
  unsigned __int64 m_ulSteamIDUser;
  unsigned __int64 m_ulSteamIDLobby;
  unsigned __int64 m_ulGameID;
};

/* 2730 */
struct __declspec(align(8)) LobbyEnter_t
{
  unsigned __int64 m_ulSteamIDLobby;
  unsigned int m_rgfChatPermissions;
  bool m_bLocked;
  unsigned int m_EChatRoomEnterResponse;
};

/* 2731 */
struct __declspec(align(8)) LobbyChatUpdate_t
{
  unsigned __int64 m_ulSteamIDLobby;
  unsigned __int64 m_ulSteamIDUserChanged;
  unsigned __int64 m_ulSteamIDMakingChange;
  unsigned int m_rgfChatMemberStateChange;
};

/* 2732 */
struct LobbyChatMsg_t
{
  unsigned __int64 m_ulSteamIDLobby;
  unsigned __int64 m_ulSteamIDUser;
  char m_eChatEntryType;
  unsigned int m_iChatID;
};

/* 2733 */
struct __declspec(align(8)) LobbyKicked_t
{
  unsigned __int64 m_ulSteamIDLobby;
  unsigned __int64 m_ulSteamIDAdmin;
  char m_bKickedDueToDisconnect;
};

/* 2734 */
struct LobbyCreated_t
{
  EResult m_eResult;
  unsigned __int64 m_ulSteamIDLobby;
};