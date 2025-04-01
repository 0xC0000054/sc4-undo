////////////////////////////////////////////////////////////////////////
//
// This file is part of sc4-undo, a DLL Plugin for SimCity 4 that
// adds an undo keyboard shortcut as an April fools day joke.
//
// Copyright (c) 2025 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////

#include "version.h"
#include "cGZPersistResourceKey.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZMessage2.h"
#include "cIGZMessageServer2.h"
#include "cIGZPersistResourceManager.h"
#include "cIGZString.h"
#include "cIGZWin.h"
#include "cIGZWinKeyAcceleratorRes.h"
#include "cISC4App.h"
#include "cISC4View3DWin.h"
#include "cRZMessage2COMDirector.h"
#include "cRZAutoRefCount.h"
#include "cRZBaseString.h"
#include "GZServPtrs.h"
#include "SC4NotificationDialog.h"
#include <array>

static constexpr uint32_t kUndoDllDirectorID = 0xA42E844B;

static constexpr uint32_t kSC4MessagePostCityInit = 0x26D31EC1;

static constexpr uint32_t kUndoShortcutKey = 0x013C7EEF;

static constexpr uint32_t kGZWin_WinSC4App = 0x6104489A;
static constexpr uint32_t kGZWin_SC4View3DWin = 0x9A47B417;

static constexpr uint32_t kGZIID_cISC4View3DWin = 0xFA47B3F9;

static constexpr std::array<uint32_t, 2> MessageIDs
{
	kSC4MessagePostCityInit,
	kUndoShortcutKey
};

class UndoDllDirector : public cRZMessage2COMDirector
{
public:

	UndoDllDirector()
	{
	}

	uint32_t GetDirectorID() const
	{
		return kUndoDllDirectorID;
	}

	bool DoMessage(cIGZMessage2* pMsg)
	{
		switch (pMsg->GetType())
		{
		case kSC4MessagePostCityInit:
			RegisterKeyboardShortcuts();
			break;
		case kUndoShortcutKey:
			ProcessUndoCommand();
			break;
		}

		return true;
	}

	void ProcessUndoCommand()
	{
		cRZBaseString message("Happy April Fools Day.");
		cRZBaseString caption("SC4Undo");

		SC4NotificationDialog::ShowDialog(message, caption);
	}

	void RegisterKeyboardShortcuts()
	{
		cIGZPersistResourceManagerPtr pResMan;

		if (pResMan)
		{
			// We load our shortcuts from a private KEYCFG file and add them
			// into the game when the city is initialized.
			// This avoids the compatibility issues that come with overriding
			// the game's main city/3D view KEYCFG file.

			cGZPersistResourceKey key(0xA2E3D533, 0xE3EE5442, 0x816AEF11);

			cRZAutoRefCount<cIGZWinKeyAcceleratorRes> pKeyAcceleratorRes;

			// GetPrivateResource fetches the resource without adding it to the game's
			// in-memory resource cache.
			if (pResMan->GetPrivateResource(
				key,
				kGZIID_cIGZWinKeyAcceleratorRes,
				pKeyAcceleratorRes.AsPPVoid(),
				0,
				nullptr))
			{
				cISC4AppPtr pSC4App;

				if (pSC4App)
				{
					cIGZWin* mainWindow = pSC4App->GetMainWindow();

					if (mainWindow)
					{
						cIGZWin* pParentWin = mainWindow->GetChildWindowFromID(kGZWin_WinSC4App);

						if (pParentWin)
						{
							cRZAutoRefCount<cISC4View3DWin> pView3D;

							if (pParentWin->GetChildAs(
								kGZWin_SC4View3DWin,
								kGZIID_cISC4View3DWin,
								pView3D.AsPPVoid()))
							{
								cIGZWinKeyAccelerator* pKeyAccelerator = pView3D->GetKeyAccelerator();

								if (pKeyAccelerator)
								{
									pKeyAcceleratorRes->RegisterResources(pKeyAccelerator);
								}
							}
						}
					}
				}
			}
		}
	}

	bool PostAppInit()
	{
		cIGZMessageServer2Ptr pMsgServ;
		if (pMsgServ)
		{
			for (uint32_t messageID : MessageIDs)
			{
				pMsgServ->AddNotification(this, messageID);
			}
		}

		return true;
	}

	bool OnStart(cIGZCOM* pCOM)
	{
		mpFrameWork->AddHook(this);	
		return true;
	}
};

cRZCOMDllDirector* RZGetCOMDllDirector() {
	static UndoDllDirector sDirector;
	return &sDirector;
}