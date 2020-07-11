#include "NFCSimulationDataGenerator.h"
#include "NFCAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

NFCSimulationDataGenerator::NFCSimulationDataGenerator()
:	mSerialText( "My first analyzer, woo hoo!" ),
	mStringIndex( 0 )
{
}

NFCSimulationDataGenerator::~NFCSimulationDataGenerator()
{
}

void NFCSimulationDataGenerator::Initialize( U32 simulation_sample_rate, NFCAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

  mTxSimulationData = mNFCSimulationChannels.Add(mSettings->mTxChannel, mSimulationSampleRateHz, BIT_HIGH);
	mRxSimulationData = mNFCSimulationChannels.Add(mSettings->mRxChannel, mSimulationSampleRateHz, BIT_HIGH);

}

U32 NFCSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mTxSimulationData->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateSerialByte();
	}

	*simulation_channels = mNFCSimulationChannels.GetArray();
	return mNFCSimulationChannels.GetCount();
	//return 1;
}

void NFCSimulationDataGenerator::CreateSerialByte()
{
	U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mBitRate;

	U8 byte = mSerialText[ mStringIndex ];
	mStringIndex++;
	if( mStringIndex == mSerialText.size() )
		mStringIndex = 0;

	//we're currenty high
	//let's move forward a little
	mTxSimulationData->Advance( samples_per_bit * 100 );
	mRxSimulationData->Advance( samples_per_bit * 100 );

	mTxSimulationData->Transition();  //low-going edge for start bit
	mTxSimulationData->Advance( samples_per_bit );  //add start bit time
	mRxSimulationData->Advance( samples_per_bit );  //add start bit time

	U8 mask = 0x1 << 7;
	for( U32 i=0; i<8; i++ )
	{
		if( ( byte & mask ) != 0 )
			mTxSimulationData->TransitionIfNeeded( BIT_HIGH );
		else
			mTxSimulationData->TransitionIfNeeded( BIT_LOW );

		mTxSimulationData->Advance( samples_per_bit );
		mRxSimulationData->Advance( samples_per_bit );

		mask = mask >> 1;
	}

	mTxSimulationData->TransitionIfNeeded( BIT_HIGH ); //we need to end high

	//lets pad the end a bit for the stop bit:
	mTxSimulationData->Advance( samples_per_bit );
	mRxSimulationData->Advance( samples_per_bit );
}
