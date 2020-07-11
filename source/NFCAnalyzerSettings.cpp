#include "NFCAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


NFCAnalyzerSettings::NFCAnalyzerSettings()
:	mTxChannel( UNDEFINED_CHANNEL ),
  mRxChannel( UNDEFINED_CHANNEL ),
	mBitRate( 106000 )
{
	mTxChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mTxChannelInterface->SetTitleAndTooltip( "NfcTX", "Standard NFC protocol analyzer" );
	mTxChannelInterface->SetChannel( mTxChannel );

	mRxChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mRxChannelInterface->SetTitleAndTooltip( "NfcRx", "Standard NFC protocol analyzer" );
	mRxChannelInterface->SetChannel( mRxChannel );


	mBitRateInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mBitRateInterface->SetTitleAndTooltip( "Bit Rate (Bits/S)",  "Specify the bit rate in bits per second." );
	mBitRateInterface->SetMax( 6000000 );
	mBitRateInterface->SetMin( 1 );
	mBitRateInterface->SetInteger( mBitRate );

	AddInterface( mTxChannelInterface.get() );
	AddInterface( mRxChannelInterface.get() );
	AddInterface( mBitRateInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mTxChannel, "NfcTx", false );
	AddChannel( mRxChannel, "NfcRx", false );

}

NFCAnalyzerSettings::~NFCAnalyzerSettings()
{
}

bool NFCAnalyzerSettings::SetSettingsFromInterfaces()
{
	mTxChannel = mTxChannelInterface->GetChannel();
	mRxChannel = mRxChannelInterface->GetChannel();
	mBitRate = mBitRateInterface->GetInteger();

	ClearChannels();
	AddChannel( mTxChannel, "NFC TX", true );
	AddChannel( mRxChannel, "NFC RX", true );

	return true;
}

void NFCAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mTxChannelInterface->SetChannel( mTxChannel );
	mRxChannelInterface->SetChannel( mRxChannel );

	mBitRateInterface->SetInteger( mBitRate );
}

void NFCAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mTxChannel;
	text_archive >> mRxChannel;
	text_archive >> mBitRate;

	ClearChannels();
	AddChannel( mTxChannel, "NFC TX", true );
	AddChannel( mRxChannel, "NFC RX", true );

	UpdateInterfacesFromSettings();
}

const char* NFCAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mTxChannel;
	text_archive << mRxChannel;
	text_archive << mBitRate;

	return SetReturnString( text_archive.GetString() );
}
