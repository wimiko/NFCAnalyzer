#ifndef NFC_ANALYZER_SETTINGS
#define NFC_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class NFCAnalyzerSettings : public AnalyzerSettings
{
public:
	NFCAnalyzerSettings();
	virtual ~NFCAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();


	Channel mTxChannel;
	Channel mRxChannel;
	U32 mBitRate;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mTxChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRxChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //NFC_ANALYZER_SETTINGS
